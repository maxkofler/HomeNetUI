#ifndef WSVALUE_H
#define WSVALUE_H

#include <QObject>

class WSValue;

#include "log.h"
#include "transmissionparser.h"

class WSValue : public QObject{
    Q_OBJECT
public:
    WSValue(Log*);

    void parseFromString(std::string data);

signals:
    void onWrongTransmissionLengthError();
    void onParseError();

private:
    std::string cN = "WSValue::";
    Log* log;

    std::string m_driverName;
    int m_driverID;
    std::string m_valueName;
    std::string m_valueUnit;
    std::string m_displayType;
    int m_valueID;
    std::string m_value;
    std::string m_valueType;
};

#endif // WSVALUE_H
