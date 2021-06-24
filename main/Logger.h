#ifndef Logger_H
#define Logger_H

#include "Arduino.h"
class Logger
{
  public:
    enum LogLevel {Info, Warning, Severe};
    struct LogConfig
    {
      bool info = false;
      bool warning = true;
      bool severe = true;
    };
  
    Logger(LogConfig conf);
    Logger();
    static LogConfig generateConf(bool info, bool warning, bool severe);
    void printline(String in, String level);
    void printline(String in, LogLevel level);

    
  private:
    LogConfig loggerConf;
};

#endif
