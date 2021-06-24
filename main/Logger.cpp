#include "Arduino.h"
#include "Logger.h"
  
    Logger::Logger(LogConfig conf)
    {
      loggerConf = conf;
    }
    Logger::Logger()
    {
      loggerConf = generateConf(true,true,true,true);
    }
    void Logger::init(Stream *inStream)
    {
      stream = inStream;
      stream->println("Serial Initialized");
    }
    Logger::LogConfig Logger::generateConf(bool debug = true, bool info = true, bool warning = true, bool severe = true)
    {
      LogConfig out;
      out.debug = debug;
      out.info = info;
      out.warning = warning;
      out.severe = severe;
      return out;
    }
    void Logger::setConf(LogConfig conf)
    {
      loggerConf = conf;
    }

    void Logger::printline(String in)
    {
      printline(in, "info");
    }
    void Logger::printline(String in, String level)
    {
      if(level == "debug")
      {
          printline(in, Debug);
      }    
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
        case Debug:
          if(loggerConf.debug)
          {
            stream->println("DEBUG: " + (String)in);
          }
          break;        
        case Info:
          if(loggerConf.info)
          {
            stream->println("INFO: " + (String)in);
          }
          break;
        case Warning:
          if(loggerConf.warning)
          {
            stream->println("WARNING: " + (String)in);
          }
          break;         
        case Severe:
          if(loggerConf.severe)
          {
            stream->println("SEVERE: " + (String)in);
          }
          break;         
      }   
    }
