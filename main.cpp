#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QQmlContext>
#include <QApplication>
#include <QIcon>
#include <QLibrary>
#include <QStyleHints>
#include <QCommandLineParser>
#include "babe.h"
#include "services/local/player.h"

#ifdef STATIC_KIRIGAMI
#include "./3rdparty/kirigami/src/kirigamiplugin.h"
#endif

#ifdef STATIC_MAUIKIT
#include "./mauikit/src/mauikit.h"
#endif

#ifdef Q_OS_ANDROID
#include <QtWebView/QtWebView>
#include <QGuiApplication>
#include <QIcon>
#else
#include <QApplication>
#include <QtWebEngine>
#endif

#include "utils/bae.h"
#include "services/web/youtube.h"
#include "services/web/Spotify/spotify.h"
#include "services/local/linking.h"
#ifdef Q_OS_ANDROID
Q_DECL_EXPORT
#endif

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef Q_OS_ANDROID
    QGuiApplication app(argc, argv);
    QGuiApplication::styleHints()->setMousePressAndHoldInterval(1000); // in [ms]
#else
    QApplication app(argc, argv);
#endif

    app.setApplicationName(BAE::App);
    app.setApplicationVersion(BAE::Version);
    app.setWindowIcon(QIcon("qrc:/assets/vvave.png"));
    app.setDesktopFileName(BAE::App);

    QCommandLineParser parser;
    parser.setApplicationDescription(BAE::Description);

    const QCommandLineOption versionOption = parser.addVersionOption();
    parser.process(app);
    //    bool version = parser.isSet(versionOption);

    //    if(version)
    //    {
    //        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
    //               qPrintable(QCoreApplication::applicationVersion()));
    //        return 0;
    //    }

    const QStringList args = parser.positionalArguments();

    QStringList urls;

    if(!args.isEmpty())
        urls = args;

    Babe bae;

    /* add translator */
    QString locale = QLocale::system().name();
    QTranslator translator;
    if (translator.load(":/translations/" + locale + ".qm"))
        app.installTranslator(&translator);
    //qDebug()<<"load translations :/translations/" + locale + ".qm";

    /* Services */
    YouTube youtube;
    Spotify spotify;

    QFontDatabase::addApplicationFont(":/assets/materialdesignicons-webfont.ttf");

    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, [&]()
    {
        qDebug()<<"FINISHED LOADING QML APP";
        bae.refreshCollection();
        if(!urls.isEmpty())
            bae.openUrls(urls);
    });

    auto context = engine.rootContext();
    context->setContextProperty("player", &bae.player);
    context->setContextProperty("bae", &bae);
    context->setContextProperty("youtube", &youtube);
    context->setContextProperty("spotify", &spotify);
    context->setContextProperty("link", &bae.link);

    qmlRegisterUncreatableMetaObject(
                LINK::staticMetaObject, // static meta object
                "Link.Codes",                // import statement (can be any string)
                1, 0,                          // major and minor version of the import
                "LINK",                 // name in QML (does not have to match C++ name)
                "Error: only enums"            // error in case someone tries to create a MyNamespace object
                );

#ifdef STATIC_KIRIGAMI
    KirigamiPlugin::getInstance().registerTypes();
#endif

#ifdef STATIC_MAUIKIT
    MauiKit::getInstance().registerTypes();
#endif

#ifdef Q_OS_ANDROID
    QtWebView::initialize();
#else
    //    if(QQuickStyle::availableStyles().contains("nomad"))
    //        QQuickStyle::setStyle("nomad");

    if(!BAE::isMobile())
        QtWebEngine::initialize();
#endif

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
