#include "hnvalues.h"

HNValues::HNValues(){
    FUN();

    this->_server = new HNNetworking();
}

HNValues::~HNValues(){
    FUN();

    delete this->_server;
}
