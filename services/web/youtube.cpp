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


#include "youtube.h"
#include "../../utils/babeconsole.h"

#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QDomDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QVariantMap>

using namespace BAE;

YouTube::YouTube(QObject *parent) : QObject(parent)
{

}

YouTube::~YouTube(){}

bool YouTube::getQuery(const QString &query)
{
    QUrl encodedQuery(query);
    encodedQuery.toEncoded(QUrl::FullyEncoded);

    auto url = this->API[METHOD::SEARCH];

    url.append("q="+encodedQuery.toString());
    url.append("&maxResults=5&part=snippet");
    url.append("&key="+this->KEY);

    qDebug()<< url;
    auto array = this->startConnection(url);

    if(array.isEmpty()) return false;

    return this->packQueryResults(array);
}

bool YouTube::packQueryResults(const QByteArray &array)
{
    QJsonParseError jsonParseError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(static_cast<QString>(array).toUtf8(), &jsonParseError);

    if (jsonParseError.error != QJsonParseError::NoError)
        return false;

    if (!jsonResponse.isObject())
        return false;

    QJsonObject mainJsonObject(jsonResponse.object());
    auto data = mainJsonObject.toVariantMap();
    auto items = data.value("items").toList();

    if(items.isEmpty()) return false;

    QVariantList res;

    for(auto item : items)
    {
        auto itemMap = item.toMap().value("id").toMap();
        auto id = itemMap.value("videoId").toString();
        auto url = "https://www.youtube.com/embed/"+id;

        if(!url.isEmpty())
        {
            qDebug()<<url;
            QVariantMap map = { {BAE::KEYMAP[BAE::KEY::ID], id}, {BAE::KEYMAP[BAE::KEY::URL], url}  };
            res << map;
        }
    }

    emit this->queryResultsReady(res);
    return true;
}

void YouTube::getId(const QString &results)
{

}

void YouTube::getUrl(const QString &id)
{

}

QByteArray YouTube::startConnection(const QString &url, const QMap<QString, QString> &headers)
{
    if(!url.isEmpty())
    {
        QUrl mURL(url);
        QNetworkAccessManager manager;
        QNetworkRequest request (mURL);

        if(!headers.isEmpty())
            for(auto key: headers.keys())
                request.setRawHeader(key.toLocal8Bit(), headers[key].toLocal8Bit());

        QNetworkReply *reply =  manager.get(request);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop,
                SLOT(quit()));

        loop.exec();

        if(reply->error())
        {
            qDebug() << reply->error();
            return QByteArray();
        }

        if(reply->bytesAvailable())
        {
            auto data = reply->readAll();
            reply->deleteLater();

            return data;
        }
    }

    return QByteArray();
}

QUrl YouTube::fromUserInput(const QString &userInput)
{
    if (userInput.isEmpty())
        return QUrl::fromUserInput("about:blank");
    const QUrl result = QUrl::fromUserInput(userInput);
    return result.isValid() ? result : QUrl::fromUserInput("about:blank");
}

