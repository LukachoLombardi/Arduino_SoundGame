#ifndef Logger_H
#define Logger_H

#include "Arduino.h"
class Logger
{
  public:
    enum LogLevel {Debug, Info, Warning, Severe};
    struct LogConfig
    {
      bool debug = false;
      bool info = false;
      bool warning = true;
      bool severe = true;
    };
  
    Logger(LogConfig conf);
    Logger();
    void init(Stream *inStream);

    static LogConfig generateConf(bool debug, bool info, bool warning, bool severe);
    void setConf(LogConfig conf);

    void printline(String in);
    void printline(String in, String level);
    void printline(String in, LogLevel level);

    
  private:
    LogConfig loggerConf;
    Stream *stream;
};

#endif
