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

int NetworkManager::open(QHostAddress address, int port, int timeout){
    if (!this->m_socket.isOpen()){
        this->m_sockClosedExpected = false;
        this->m_hostaddr = address;
        this->m_port = port;
        QTimer timer;
        QEventLoop connectLoop;
        QElapsedTimer stop;

        //Connecting dataReceived and timeout
        connect(this, &NetworkManager::onSocketConnected, &connectLoop, &QEventLoop::quit);
        connect(&timer, &QTimer::timeout, &connectLoop, &QEventLoop::quit);


        log->log(cN + "open()", "Waiting for socket to change to connected state...", Log::D);
        stop.start();
        this->m_socket.connectToHost(m_hostaddr, this->m_port);

        timer.start(timeout);
        connectLoop.exec();
        log->log(cN + "open()", "Waiter for connection quit with " + std::to_string(timeout - stop.elapsed()) + "ms left!", Log::D3);

        if (stop.elapsed() >= timeout){
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


bool NetworkManager::sendRequest(const char* msg){
    std::string fN = cN + "sendRequest()";
    if (this->m_socket.isOpen()){
        this->m_socket.write(QByteArray(msg));
        log->log(fN, "Sending a request to the connected Server...", Log::D);
        log->log(fN, "Sending message: \"" + std::string(msg) + "\"", Log::D2);
        return true;
    }else{
        log->log(fN, "The socket was not connected! The request has not been sent!", Log::E);
        return false;
    }
}

QString NetworkManager::sendRequestForAnswer(const char* msg, int timeout){
    std::string fN = cN + "sendRequestForAnswer()";
    QElapsedTimer stop;
    QEventLoop waiter;
    QTimer timer;
    connect(this, &NetworkManager::onDataReceived, &waiter, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &waiter, &QEventLoop::quit);

    //Now send the request
    if (sendRequest(msg)){
        stop.start();
        timer.start(timeout);
        waiter.exec();
        log->log(fN, "Waiter for answer quit with " + std::to_string(timeout - stop.elapsed()) + "ms left!", Log::D3);

        //Check if there was a timeout
        if (stop.elapsed() >= timeout){
            log->log(fN, "Request timed out!", Log::W);
            log->sig(cN, "onRequestTimeout");
            emit onRequestTimeout();
            return NULL;
        }else{
            return this->getData();
        }
    }else{
        log->log(fN, "Unable to send the request! (Socket was not opened properly!)", Log::E);
        log->sig(cN, "onSocketError");
        emit onSocketError();
        return NULL;
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

    emit onDataReceived();
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
