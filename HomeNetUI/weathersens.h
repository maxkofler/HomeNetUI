#ifndef WEATHERSENS_H
#define WEATHERSENS_H

#include <QObject>
#include <QHostAddress>

#include "wsvalue.h"
#include "networkmanager.h"
#include "log.h"

class Weathersens : public QObject
{
    Q_OBJECT
public:
    Weathersens(Log*);
    ~Weathersens();

    void setAddr(QHostAddress server_address, int port, int timeout);

    bool sync();

private:
    Log* log;
    std::string cN = "Weathersens::";

    //Server connection
    NetworkManager *m_networkManager;
    QHostAddress m_server_addr;
    int m_port;
    int m_timeout;

    std::vector<WSValue*> m_values;


    std::vector<WSValue*> parseValues(std::string msg);
};

#endif // WEATHERSENS_H
