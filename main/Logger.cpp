#include "Arduino.h"
#include "Logger.h"
    enum Logger::LogLevel {Info, Warning, Severe};
    struct Logger::LogConfig
    {
      bool info = false;
      bool warning = true;
      bool severe = true;
    };
  
    Logger::Logger(LogConfig conf)
    {
      Serial.begin(9600);
      loggerConf = conf;
    }
    Logger::Logger()
    {
      Serial.begin(9600);
      loggerConf = generateConf(true, true, true);
    }
    static LogConfig Logger::generateConf(bool info, bool warning, bool severe)
    {
      LogConfig out;
      out.info = info;
      out.warning = warning;
      out.severe = severe;
      return out;
    }
    void Logger::printline(String in, String level)
    {
    
      if(level == "info")
      {
          printline(in, Info);
      }
      if(level == "warning")
      {
          printline(in, Warning);
      }
      if(level == "severe")
      {
          printline(in, Severe);
      }
    }
    void Logger::printline(String in, LogLevel level)
    {
      switch(level)
      {
        case Info:
          if(loggerConf.info)
          {
            Serial.println("INFO: " + in);
          }
        case Warning:
          if(loggerConf.warning)
          {
            Serial.println("WARNING: " + in);
          }         
        case Severe:
          if(loggerConf.severe)
          {
            Serial.println("SEVERE: " + in);
          }         
      }      
    }
