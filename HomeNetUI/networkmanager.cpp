#include "networkmanager.h"

NetworkManager::NetworkManager(Log* log) {
    this->log = log;
    log->log(cN + "NetworkManager()", "connecting signals and slots...", Log::D2);
    connect(&m_socket, &QTcpSocket::readyRead, this, &NetworkManager::ReadyRead);
    connect(this, &NetworkManager::onSocketClosed, this, &NetworkManager::SocketClosed);
    connect(&m_socket, &QAbstractSocket::stateChanged, this, &NetworkManager::SocketStateChanged);
}

NetworkManager::~NetworkManager(){
    //Cleaning up and closing the socket if neccesary
    if (this->m_socket.isOpen()){
        this->close();
    }
}

int NetworkManager::open(QString ip_addr, int port){
    if (!this->m_socket.isOpen()){
        this->m_sockClosedExpected = false;
        this->m_ip_addr = ip_addr;
        this->m_port = port;
        QTimer timer;
        connect(&timer, &QTimer::timeout, this, &NetworkManager::connectTimeout);
        this->connectionTimeout = false;
        connect(this, &NetworkManager::onSocketConnected, &this->connectLoop, &QEventLoop::quit);

        log->log(cN + "open()", "Waiting for socket to change to connected state...", Log::D);
        this->m_socket.connectToHost(QHostAddress(this->m_ip_addr), this->m_port);

        timer.start(1000);
        this->connectLoop.exec();

        if (this->connectionTimeout){
            log->log(cN + "open()", "Connection timed out, closing socket!", Log::E);
            this->close();
            log->sig(cN, "onSocketTimeout");
            emit onSocketTimeout();
            return -3;
        }

    }else{
        log->log(cN + "connect()", "The socket was already connected!", Log::W);
        return -1;
    }
    return 0;
}

void NetworkManager::close(){
    if (this->m_socket.isOpen()){
        this->m_sockClosedExpected = true;
        this->m_socket.close();
    }
}


void NetworkManager::sendRequest(const char* msg){
    if (this->m_socket.isOpen()){
        this->m_socket.write(QByteArray(msg));
        log->log(cN + "sendRequest()", "Sending message: \"" + std::string(msg) + "\"", Log::D);
    }else{
        log->log(cN + "sendRequest()", "The socket was not connected! The request has not been sent!", Log::E);
    }
}

//
// PUBLIC SLOTS
//
void NetworkManager::ReadyRead(){
    log->slot(cN, "ReadyRead");
    this->m_data = m_socket.readAll();
    log->log(cN + "ReadyRead()", "Received data:\n|\n|\n" + this->m_data.toStdString() + "|\n|", Log::D3);
    log->sig(cN, "onDataReceived");
    emit onDataReceived(this->m_data);
}

void NetworkManager::SocketClosed(bool expected){
    log->slot(cN, "SocketClosed");
    if (expected){
        log->log(cN + "SocketClosed()", "The socket has been closed!", Log::I);
    }else{
        log->log(cN + "SockedClosed()", "Socket has closed unexpected or by server!", Log::E);
    }
}

void NetworkManager::SocketStateChanged(QAbstractSocket::SocketState state){
    log->slot(cN, "SocketStateChanged");
    if (state == QAbstractSocket::SocketState::ClosingState){
        log->sig(cN, "onSocketClosed");
        emit onSocketClosed(this->m_sockClosedExpected);
    }
    if (state == QAbstractSocket::SocketState::ConnectingState){
        log->log(cN + "SocketStateChanged()", "Establishing connection...", Log::I);
    }
    if (state == QAbstractSocket::SocketState::ConnectedState){
        log->log(cN + "SocketStateChanged()", "Connection established!", Log::I);
        log->sig(cN, "onSocketConnected");
        emit onSocketConnected();
    }

    emit onSocketStateChanged(state);
}

//
// PRIVATE SLOTS
//
void NetworkManager::connectTimeout(){
    log->slot(cN, "connectTimeout(private)");
    this->connectionTimeout = true;
    this->connectLoop.exit();
}
