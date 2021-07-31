#ifndef __HNVALUES_H__
#define __HNVALUES_H__

#include "log.h"

#include "hnnetworking.h"

#include <QHostAddress>

class HNValues{

public:
    HNValues();
    ~HNValues();

    bool                                init();

    bool                                connect(QHostAddress serverIP, int serverPort);

private:
    HNNetworking*                       _server;

};

#endif
