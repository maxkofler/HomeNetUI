#include "weathersens.h"

#include "sstream"

Weathersens::Weathersens(Log* log){
    this->log = log;
    this->m_networkManager = new NetworkManager(log);
}

Weathersens::~Weathersens(){
    for (int i = 0; i < (int)m_values.size(); i++){
        delete m_values.at(i);
    }
    log->destructor(cN, "Freed memory used for values!");
}

void Weathersens::setAddr(QHostAddress server_address, int port, int timeout){
    this->m_server_addr = server_address;
    this->m_port = port;
    this->m_timeout = timeout;
}

void Weathersens::sync(){
    if (this->m_networkManager->open(this->m_server_addr, this->m_port, m_timeout) >= 0){
        std::string msg = this->m_networkManager->sendRequestForAnswer("@va", m_timeout).toStdString();
        WSValue value(log);
        m_values = parseValues(msg);
    }
}





//
// Private functions
//
std::vector<WSValue*> Weathersens::parseValues(std::string msg){
    std::vector<WSValue*> vect_values;
    std::istringstream stream(msg.c_str());
    std::string line;
    while(getline(stream, line)){
        WSValue* value = new WSValue(log);
        value->parseFromString(line);
        vect_values.push_back(value);
    }
    return vect_values;
}
