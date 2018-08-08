#ifndef BABE_H
#define BABE_H

#include <QObject>
#include <QVariantList>
#include "utils/bae.h"
#include "db/collectionDB.h"
#include "services/local/linking.h"
#include "services/local/player.h"

#if (defined (Q_OS_LINUX) && !defined (Q_OS_ANDROID))
class Notify;
#elif defined (Q_OS_ANDROID)
//class NotificationClient;
#endif

class CollectionDB;
class Pulpo;
class BabeSettings;
class ConThread;

using namespace BAE;

class Babe : public CollectionDB
{
    Q_OBJECT

public:
    explicit Babe(QObject *parent = nullptr);
    ~Babe();

    BabeSettings *settings;
    Linking link;
    Player player;

    //    Q_INVOKABLE void runPy();

    /* DATABASE INTERFACES */
    Q_INVOKABLE QVariantList get(const QString &queryTxt);
    Q_INVOKABLE QVariantList getList(const QStringList &urls);

    Q_INVOKABLE void set(const QString &table, const QVariantList &wheres);

    Q_INVOKABLE void trackPlaylist(const QStringList &urls, const QString &playlist);
    Q_INVOKABLE void trackLyrics(const QString &url);
    Q_INVOKABLE bool trackBabe(const QString &path);

    Q_INVOKABLE QString artistArt(const QString &artist);
    Q_INVOKABLE QString albumArt(const QString &album, const QString &artist);
    Q_INVOKABLE QString artistWiki(const QString &artist);
    Q_INVOKABLE QString albumWiki(const QString &album, const QString &artist);

    Q_INVOKABLE QVariantList getFolders();
    Q_INVOKABLE bool babeTrack(const QString &path, const bool &value);

    /* SETTINGS */
    Q_INVOKABLE void scanDir(const QString &url);
    Q_INVOKABLE void brainz(const bool &on);
    Q_INVOKABLE bool brainzState();
    Q_INVOKABLE void refreshCollection();
    Q_INVOKABLE void getYoutubeTrack(const QString &message);

    /* STATIC METHODS */
    Q_INVOKABLE static void saveSetting(const QString &key, const QVariant &value, const QString &group);
    Q_INVOKABLE static QVariant loadSetting(const QString &key, const QString &group, const QVariant &defaultValue);

    Q_INVOKABLE static void savePlaylist(const QStringList &list);
    Q_INVOKABLE static QStringList lastPlaylist();

    Q_INVOKABLE static void savePlaylistPos(const int &pos);
    Q_INVOKABLE static int lastPlaylistPos();

    Q_INVOKABLE static bool fileExists(const QString &url);
    Q_INVOKABLE static void showFolder(const QStringList &urls);

    /*COLORS*/
    Q_INVOKABLE static QString babeColor();

    /*UTILS*/
    Q_INVOKABLE void openUrls(const QStringList &urls);

    Q_INVOKABLE static QString moodColor(const int &pos);

    Q_INVOKABLE static QString homeDir();
    Q_INVOKABLE static QString musicDir();

    Q_INVOKABLE static QStringList defaultSources();

    /*USEFUL*/
    Q_INVOKABLE QString loadCover(const QString &url);
    Q_INVOKABLE QVariantList searchFor(const QStringList &queries);

    /*KDE*/
    Q_INVOKABLE void notify(const QString &title, const QString &body);
    Q_INVOKABLE void notifySong(const QString &url);

public slots:

private:
    Pulpo *pulpo;
    ConThread *thread;

#if (defined (Q_OS_LINUX) && !defined (Q_OS_ANDROID))
    Notify *nof;
#elif defined (Q_OS_ANDROID)
//    NotificationClient *nof;
#endif
    QString fetchCoverArt(DB &song);
    static QVariantList transformData(const DB_LIST &dbList);

    void fetchTrackLyrics(DB &song);
    void linkDecoder(QString json);

signals:
    void refreshTables(int size);
    void refreshTracks();
    void refreshAlbums();
    void refreshArtists();
    void trackLyricsReady(QString lyrics, QString url);
    void skipTrack();
    void babeIt();
    void message(QString msg);
    void openFiles(QVariantList tracks);
};


#endif // BABE_H
