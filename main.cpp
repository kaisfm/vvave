#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QQmlContext>
#include <QApplication>
#include <QIcon>
#include "babe.h"
#include "services/local/player.h"
#include <QLibrary>
#include <QQuickStyle>
#include <QStyleHints>
#ifdef Q_OS_ANDROID
#include "./3rdparty/kirigami/src/kirigamiplugin.h"
//#include "java/notificationclient.h"
#endif

#include "utils/bae.h"
#include <QCommandLineParser>
#include "services/web/youtube.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setApplicationName(BAE::App);
    app.setApplicationVersion(BAE::Version);
    app.setWindowIcon(QIcon("qrc:/assets/babe.png"));
    app.setDesktopFileName(BAE::App);

    /*needed for mobile*/
    if(BAE::isMobile())
    {
        int pressAndHoldInterval = 1000; // in [ms]
        QGuiApplication::styleHints()->setMousePressAndHoldInterval(pressAndHoldInterval);
    }

    QCommandLineParser parser;
    parser.setApplicationDescription("Babe music player");
    const QCommandLineOption versionOption = parser.addVersionOption();
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    bool version = parser.isSet(versionOption);

    if(version)
    {
        printf("%s %s\n", qPrintable(QCoreApplication::applicationName()),
               qPrintable(QCoreApplication::applicationVersion()));
        return 0;
    }

    Player player;
    Babe bae;

    /* Services */
    YouTube youtube;

    QFontDatabase::addApplicationFont(":/utils/materialdesignicons-webfont.ttf");

    qDebug()<<QQuickStyle::availableStyles();

    QQmlApplicationEngine engine;

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, [&]()
    {
        qDebug()<<"FINISHED LOADING QML APP";
        bae.refreshCollection();
    });

    auto context = engine.rootContext();
    context->setContextProperty("player", &player);
    context->setContextProperty("bae", &bae);
    context->setContextProperty("youtube", &youtube);

#ifdef Q_OS_ANDROID 
    KirigamiPlugin::getInstance().registerTypes();
#else
    QQuickStyle::setStyle("nomad");
#endif

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
