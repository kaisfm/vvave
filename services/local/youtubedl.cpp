/*
   Babe - tiny music player
   Copyright (C) 2017  Camilo Higuita
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

   */


#include "youtubedl.h"
#include "../../pulpo/pulpo.h"
#include "../../db/collectionDB.h"
#include "../../utils/babeconsole.h"

#if (defined (Q_OS_LINUX) && !defined (Q_OS_ANDROID))
#include "kde/notify.h"
#endif

using namespace BAE;

youtubedl::youtubedl(QObject *parent) : QObject(parent)
{
#if (defined (Q_OS_LINUX) && !defined (Q_OS_ANDROID))
    this->nof = new Notify(this);
#endif
}

youtubedl::~youtubedl(){}

void youtubedl::fetch(const QString &json)
{
    QJsonParseError jsonParseError;
    auto jsonResponse = QJsonDocument::fromJson(json.toUtf8(), &jsonParseError);

    if (jsonParseError.error != QJsonParseError::NoError) return;
    if (!jsonResponse.isObject()) return;

    QJsonObject mainJsonObject(jsonResponse.object());
    auto data = mainJsonObject.toVariantMap();

    auto id = data.value("id").toString().trimmed();
    auto title = data.value("title").toString().trimmed();
    auto artist = data.value("artist").toString().trimmed();
    auto album = data.value("album").toString().trimmed();
    auto playlist = data.value("playlist").toString().trimmed();
    auto page = data.value("page").toString().replace('"',"").trimmed();

    bDebug::Instance()->msg("Fetching from Youtube: "+id+" "+title+" "+artist);

    DB infoMap;
    infoMap.insert(KEY::TITLE, title);
    infoMap.insert(KEY::ARTIST, artist);
    infoMap.insert(KEY::ALBUM, album);
    infoMap.insert(KEY::URL, page);
    infoMap.insert(KEY::ID, id);
    infoMap.insert(KEY::PLAYLIST, playlist);

    if(!this->ids.contains(infoMap[KEY::ID]))
    {
        this->ids << infoMap[KEY::ID];

        auto process = new QProcess(this);
        process->setWorkingDirectory(YoutubeCachePath);
        //connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(processFinished()));
        //connect(process, SIGNAL(finished(int)), this, SLOT(processFinished_totally(int)));
        connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                [=](int exitCode, QProcess::ExitStatus exitStatus)
        {
            //                qDebug()<<"processFinished_totally"<<exitCode<<exitStatus;
            processFinished_totally(exitCode, infoMap, exitStatus);
            process->deleteLater();
        });

#if (defined (Q_OS_LINUX) && !defined (Q_OS_ANDROID))
        this->nof->notify("Song received!", infoMap[KEY::TITLE]+ " - "+ infoMap[KEY::ARTIST]+".\nWait a sec while the track is added to your collection :)");
#endif
        auto command = ydl;

        command = command.replace("$$$",infoMap[KEY::ID])+" "+infoMap[KEY::ID];
        bDebug::Instance()->msg(command);
        process->start(command);
    }
}

void youtubedl::processFinished_totally(const int &state,const DB &info,const QProcess::ExitStatus &exitStatus)
{
    auto track = info;

    auto doneId = track[KEY::ID];
    auto file = YoutubeCachePath+track[KEY::ID]+".m4a";

    if(!BAE::fileExists(file)) return;

    ids.removeAll(doneId);
    track.insert(KEY::URL,file);
    bDebug::Instance()->msg("Finished collection track with youtube-dl");

    //    qDebug()<<track[KEY::ID]<<track[KEY::TITLE]<<track[KEY::ARTIST]<<track[KEY::PLAYLIST]<<track[KEY::URL];

    /*here get metadata*/
    TagInfo tag;
    if(exitStatus == QProcess::NormalExit)
    {
        if(BAE::fileExists(file))
        {
            tag.feed(file);
            tag.setArtist(track[KEY::ARTIST]);
            tag.setTitle(track[KEY::TITLE]);
            tag.setAlbum(track[KEY::ALBUM]);
            tag.setComment(track[KEY::URL]);

            bDebug::Instance()->msg("Trying to collect metadata of downloaded track");
            Pulpo pulpo;
            pulpo.registerServices({PULPO::SERVICES::LastFm, PULPO::SERVICES::Spotify});
            pulpo.setOntology(PULPO::ONTOLOGY::TRACK);
            pulpo.setInfo(PULPO::INFO::METADATA);

            QEventLoop loop;
            QTimer timer;
            connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

            timer.setSingleShot(true);
            timer.setInterval(1000);

            connect(&pulpo, &Pulpo::infoReady, [&loop](const BAE::DB &track, const PULPO::RESPONSE &res)
            {
                bDebug::Instance()->msg("Setting collected track metadata");
                if(!res[PULPO::ONTOLOGY::TRACK][PULPO::INFO::METADATA].isEmpty())
                {
                    bDebug::Instance()->msg(res[PULPO::ONTOLOGY::TRACK][PULPO::INFO::METADATA][PULPO::CONTEXT::ALBUM_TITLE].toString());
                    bDebug::Instance()->msg(res[PULPO::ONTOLOGY::TRACK][PULPO::INFO::METADATA][PULPO::CONTEXT::TRACK_NUMBER].toString());

                    TagInfo tag;
                    tag.feed(track[KEY::URL]);

                    auto albumRes = res[PULPO::ONTOLOGY::TRACK][PULPO::INFO::METADATA][PULPO::CONTEXT::ALBUM_TITLE].toString();

                    if(!albumRes.isEmpty() && albumRes != BAE::SLANG[W::UNKNOWN])
                        tag.setAlbum(res[PULPO::ONTOLOGY::TRACK][PULPO::INFO::METADATA][PULPO::CONTEXT::ALBUM_TITLE].toString());
                    else tag.setAlbum(track[KEY::TITLE]);

                    if(!res[PULPO::ONTOLOGY::TRACK][PULPO::INFO::METADATA][PULPO::CONTEXT::TRACK_NUMBER].toString().isEmpty())
                        tag.setTrack(res[PULPO::ONTOLOGY::TRACK][PULPO::INFO::METADATA][PULPO::CONTEXT::TRACK_NUMBER].toInt());
                }

                loop.quit();
            });

            pulpo.feed(track, PULPO::RECURSIVE::OFF);

            timer.start();
            loop.exec();
            timer.stop();

            bDebug::Instance()->msg("Process finished totally for "+QString(state)+" "+doneId+" "+QString(exitStatus));

            bDebug::Instance()->msg("Need to delete the id "+ doneId);
            bDebug::Instance()->msg("Ids left to process: " + this->ids.join(","));
        }
    }


    tag.feed(file);
    auto album = BAE::fixString(tag.getAlbum());
    auto trackNum = tag.getTrack();
    auto title = BAE::fixString(tag.getTitle()); /* to fix*/
    auto artist = BAE::fixString(tag.getArtist());
    auto genre = tag.getGenre();
    auto sourceUrl = QFileInfo(file).dir().path();
    auto duration = tag.getDuration();
    auto year = tag.getYear();

    BAE::DB trackMap =
    {
        {BAE::KEY::URL,file},
        {BAE::KEY::TRACK,QString::number(trackNum)},
        {BAE::KEY::TITLE,title},
        {BAE::KEY::ARTIST,artist},
        {BAE::KEY::ALBUM,album},
        {BAE::KEY::DURATION,QString::number(duration)},
        {BAE::KEY::GENRE,genre},
        {BAE::KEY::SOURCES_URL,sourceUrl},
        {BAE::KEY::BABE, file.startsWith(BAE::YoutubeCachePath)?"1":"0"},
        {BAE::KEY::RELEASE_DATE,QString::number(year)}
    };

    CollectionDB con(nullptr);
    con.addTrack(trackMap);
    con.trackPlaylist({file}, track[KEY::PLAYLIST]);

    if(this->ids.isEmpty()) emit this->done();

}


void youtubedl::processFinished()
{
    /* QByteArray processOutput;
    processOutput = process->readAllStandardOutput();

    if (!QString(processOutput).isEmpty())
        qDebug() << "Output: " << QString(processOutput);*/
}
