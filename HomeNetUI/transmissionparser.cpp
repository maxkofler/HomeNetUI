#include "transmissionparser.h"

TransmissionParser::TransmissionParser(Log* log){
    this->log = log;
}

Transmission TransmissionParser::parseFromString(std::string data){
    Transmission trans;
    bool lineEnd = false;

    std::vector<std::string> blocks;

    int posStart = -1;
    int posEnd = -1;
    while (!lineEnd){
        posStart = data.find('<');
        posEnd = data.find('>');
        if (posStart >= 0 && posEnd >= 0){
            blocks.push_back(data.substr(posStart+1, posEnd-1));
            data = data.substr(posEnd+1, data.length());
            log->log(cN + "parseFromString()", "Parsed \"" + blocks.back() + "\"!", Log::D3);
        }else{
           lineEnd = true;
        }
    }
    trans.blocks = new std::string[blocks.size()];
    for (int i = 0; i < (int)blocks.size(); i++){
        trans.blocks[i] = blocks.at(i);
    }
    trans.size = blocks.size();
    blocks.clear();
    return trans;
}
