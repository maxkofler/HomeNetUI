#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <iostream>

class Log{

public:
    Log(){}
    Log(int curLevel){
        setLevel(curLevel);
    }

    void setLevel(int level){
        lLevel = level;
    }

    const static int V = 1;
    const static int I = 2;
    const static int W = 3;
    const static int E = 4;
    const static int D = 5;
    const static int D2 = 6;
    const static int SIG = 7;
    const static int SLO = 8;
    const static int D3 = 9;
    const static int CD = 10;


    //Default logging
    void log(std::string tag, std::string msg, int loglevel){
        if (loglevel <= lLevel)
            std::cout << "[" << loglevel << "]" << tag << " - " << msg << std::endl;
    }
    void log(std::string tag, std::string msg){
        std::cout << "[---]" << tag << " - " << msg << std::endl; 
    }
    void log(std::string msg){
        std::cout << "[---]" << msg << std::endl;
    }
    
    void sig(std::string className, std::string sigName){
        this->log("(SIGNAL)", "emited \"" + sigName + "\" from " + className.substr(0, className.length()-2) + "!", Log::SIG);
    }

    void slot(std::string className, std::string slotName){
        this->log("(SLOT)", "\"" + slotName + "\" triggered in " + className.substr(0, className.length()-2) + "!", Log::SIG);
    }

    void destructor(std::string className, std::string msg){
        this->log("(DESTRUCTOR - " + className.substr(0, className.length()-2) + ")", msg, Log::CD);
    }

    //Logging for program errors, like warnings for programmers
    void logprg(std::string function, std::string msg, int loglevel){
        std::cout << "(" << loglevel << ")" << "{prg-log}" << "[" << function << "]" << msg << std::endl;
    }



private:
    int lLevel;
};

#endif
