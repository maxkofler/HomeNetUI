#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "weathersens.h"
#include "log.h"

static std::string ip = "192.168.1.24";
static int port = 8090;
static std::string queryMessage = "@va";

int main(int argc, char *argv[])
{
    Log log(20);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    Weathersens ws(&log);
    ws.setAddr(QHostAddress(QString().fromStdString(ip)), port, 1000);

    ws.sync();

    return app.exec();
}
