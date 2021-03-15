#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "networkmanager.h"
#include "log.h"

static std::string ip = "192.168.1.24";
static int port = 8090;
static std::string queryMessage = "@va";

int main(int argc, char *argv[])
{
    Log log(20);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    NetworkManager nM(&log);

    if (nM.open(QString().fromStdString(ip), port) >= 0){
        nM.sendRequest(queryMessage.c_str());
    }

    return app.exec();
}
