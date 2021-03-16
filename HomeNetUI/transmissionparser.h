#ifndef TRANSMISSIONPARSER_H
#define TRANSMISSIONPARSER_H

#include <QObject>

#include "log.h"

typedef struct s_transmission{
    std::string* blocks;
    int size;
}Transmission;

class TransmissionParser : public QObject
{
    Q_OBJECT
public:
    TransmissionParser(Log* log);

    Transmission parseFromString(std::string);

private:
    std::string cN = "TransmissionParser::";
    Log* log;
};

#endif // TRANSMISSIONPARSER_H
