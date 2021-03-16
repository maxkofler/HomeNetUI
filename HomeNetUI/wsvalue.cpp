#include "wsvalue.h"

WSValue::WSValue(Log* log){
    this->log = log;
}

void WSValue::parseFromString(std::string string_data){
    TransmissionParser parser(this->log);
    Transmission data = parser.parseFromString(string_data);
    if (data.size == 8){
        try{
            this->m_driverName = data.blocks[0];
            this->m_driverID = std::stoi(data.blocks[1]);
            this->m_valueName = data.blocks[2];
            this->m_valueUnit = data.blocks[3];
            this->m_displayType = data.blocks[4];
            this->m_valueID = std::stoi(data.blocks[5]);
            this->m_value = data.blocks[6];
            this->m_valueType = data.blocks[7];
            log->log(cN + "parseFromString()", "Parsed data successfully!", Log::D2);
        }catch(const std::exception& e){
            log->log(cN + "parseFromString()", "Error in parsing data: " + std::string(e.what()), Log::E);
            log->sig(cN, "onParseError");
            emit onParseError();
        }catch(...){
            log->log(cN + "parseFromString()", "Unknown error in parsing data!", Log::E);
            log->sig(cN, "onParseError");
            emit onParseError();
        }
    }else{
        log->log(cN + "parseFromString()", "The parsed data has the wrong length (" + std::to_string(data.size) + "/8), leading to unability to parse the data!", Log::E);
        log->sig(cN, "onWrongTransmissionLengthError");
        emit onWrongTransmissionLengthError();
    }
}
