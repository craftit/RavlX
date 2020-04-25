// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SYSLOG_HEADER
#define RAVL_SYSLOG_HEADER 1
//! author="Charles Galambos"
//! example=exSysLog.cc
//! docentry="Ravl.API.OS.SysLog"
//! lib=RavlCore
//! file="Ravl/Core/Base/SysLog.hh"

#include "Ravl/Stream.hh"

namespace RavlN {
  enum SysLogPriorityT {
    SYSLOG_EMERG = 0,
    SYSLOG_ALERT,
    SYSLOG_CRIT,
    SYSLOG_ERR,
    SYSLOG_WARNING,
    SYSLOG_NOTICE,
    SYSLOG_INFO,
    SYSLOG_DEBUG,
    SYSLOG_DEBUG2
  };
  
  std::ostream &operator<<(std::ostream &strm,SysLogPriorityT priority);
  //: Write log level to stream

  std::istream &operator>>(std::istream &strm,SysLogPriorityT &priority);
  //: Read log level from stream

  bool SysLogOpen(const StringC &name,bool logPid = false,bool sendStdErr = true,bool stdErrOnly = false,int facility = -1,bool logFileLine = false);
  //: Open connection to system logger.
  // Facility is set to 'LOG_USER' by default. <br>
  // If logPid is true the processes id will be recorded in the log. <br>
  // If sendStdErr is set the messages will also be send the standard error channel.
  
  void SysLogFileAndLine(bool enable);
  //: Enable/Disable logging file and line numbers.

  bool SysLogClose();
  //: Close connection to system logger.
  // The call of this function is optional.
  
  OStreamC SysLog(SysLogPriorityT priority = SYSLOG_DEBUG,unsigned lineno =0,const char *filename = 0);
  //: Send a message to the log file
  // Usage: <br>
  // SysLog(SYSLOG_DEBUG) << "Send message to log";
  
  void SysLog(SysLogPriorityT priority,const char *format ...);
  //: Send a message to the log file
  // Usage: <br>
  // SysLog(SYSLOG_DEBUG,"msg",args...);
  
  void SysLog(SysLogPriorityT priority,unsigned lineno,const char *filename,const char *format ...);
  //: Send a message to the log file
  // Usage: <br>
  // RavlSysLog(SYSLOG_DEBUG,"msg",args...);

  bool SysLogLevel(SysLogPriorityT level);
  //: Set the level of messages to send to the system.
  // Only messages with a priority higher or equal to 'level' 
  // we be sent.
  // This can be used to avoid flooding the log system 
  // with debug messages for example.
  
  bool SysLogLevelStdErr(SysLogPriorityT level);
  //: Set the level of messages to send to standard error.
  // This controls the level of messages to send to
  // std::cerr. <br>
  // Only messages with a priority higher or equal to 'level' we be sent.
  
  const StringC &SysLogApplicationName();
  //: Get the name of the current application.
  
  bool SysLogRedirect(void (*logFunc)(SysLogPriorityT level,const char *message,unsigned lineno,const char *filename));
  //: Register function to redirect log messages.
  // Calling with a null function pointer restores the default behaviour.

#define RavlSysLog(PRIORITY) RavlN::SysLog(PRIORITY,__LINE__,__FILE__)
#define RavlSysLogf(PRIORITY,...) RavlN::SysLog(PRIORITY,__LINE__,__FILE__,__VA_ARGS__)
#define RavlError(...) RavlN::SysLog(RavlN::SYSLOG_ERR,__LINE__,__FILE__,__VA_ARGS__)
#define RavlWarning(...) RavlN::SysLog(RavlN::SYSLOG_WARNING,__LINE__,__FILE__,__VA_ARGS__)
#define RavlInfo(...) RavlN::SysLog(RavlN::SYSLOG_INFO,__LINE__,__FILE__,__VA_ARGS__)
#define RavlDebug(...) RavlN::SysLog(RavlN::SYSLOG_DEBUG,__LINE__,__FILE__,__VA_ARGS__)

#define RavlErrorIf(logLevel,...) { if(logLevel >= RavlN::SYSLOG_ERR) RavlN::SysLog(RavlN::SYSLOG_ERR,__LINE__,__FILE__,__VA_ARGS__); }
#define RavlWarningIf(logLevel,...) { if(logLevel >= RavlN::SYSLOG_WARNING) RavlN::SysLog(RavlN::SYSLOG_WARNING,__LINE__,__FILE__,__VA_ARGS__); }
#define RavlInfoIf(logLevel,...) { if(logLevel >= RavlN::SYSLOG_INFO) RavlN::SysLog(RavlN::SYSLOG_INFO,__LINE__,__FILE__,__VA_ARGS__); }
#define RavlDebugIf(logLevel,...) { if(logLevel >= RavlN::SYSLOG_DEBUG) RavlN::SysLog(RavlN::SYSLOG_DEBUG,__LINE__,__FILE__,__VA_ARGS__); }
#define RavlDebug2If(logLevel,...) { if(logLevel >= RavlN::SYSLOG_DEBUG2) RavlN::SysLog(RavlN::SYSLOG_DEBUG,__LINE__,__FILE__,__VA_ARGS__); }
}

#endif
