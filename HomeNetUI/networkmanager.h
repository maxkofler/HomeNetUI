#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

#include <QTcpSocket>
#include <QHostAddress>
#include <QEventLoop>
#include <QTimer>

#include "log.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager(Log*);
    ~NetworkManager();

    //Function to connect to a new socket
    //  @ret        int         -1 if already connected, -2 on socket error
    int open(QString, int);

    //Function to close the socket
    void close();

    //Function to send a request to a server, answer comes in form of the "onDataReceived" signal
    void sendRequest(const char*);

    //Function to read the currently received data
    QString getData(){if (this->m_data != NULL) return this->m_data; else return NULL;}

    QTcpSocket* getSocket(){return &this->m_socket;}

public slots:
    void ReadyRead();
    void SocketClosed(bool expected);
    void SocketStateChanged(QAbstractSocket::SocketState);

private slots:
    void connectTimeout();

signals:
    void onDataReceived(QString data);
    void onSocketConnected();
    void onSocketTimeout();
    void onSocketClosed(bool expected);
    void onSocketStateChanged(QAbstractSocket::SocketState);

private:
    Log* log;
    std::string cN = "NetworkManager::";

    QTcpSocket m_socket;

    QString m_ip_addr;
    int m_port;

    QString m_data;

    //Checked when the socked has been closed expectedly
    bool m_sockClosedExpected;

    //Connection
    QEventLoop connectLoop;
    bool connectionTimeout;
};

#endif // NETWORKMANAGER_H
