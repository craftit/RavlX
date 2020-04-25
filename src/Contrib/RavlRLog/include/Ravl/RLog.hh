// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RLOG_HEADER
#define RAVL_RLOG_HEADER
//! lib=RavlRLog

#ifndef RLOG_COMPONENT
#define RLOG_COMPONENT Ravl
#endif

#include "Ravl/String.hh"
#include "Ravl/StrStream.hh"
#include <rlog/rlog.h>
#undef RLOG_SECTION
#define RLOG_SECTION


#define RLOG_STREAM(RLOG_CMD, ARGS)   \
  RavlN::StrOStreamC strm;            \
  strm << ARGS;                       \
  RLOG_CMD("%s",strm.String().data());     \


#define rInfoS(ARGS)    { RLOG_STREAM(rInfo,    ARGS) }
#define rDebugS(ARGS)   { RLOG_STREAM(rDebug,   ARGS) }
#define rWarningS(ARGS) { RLOG_STREAM(rWarning, ARGS) }
#define rErrorS(ARGS)   { RLOG_STREAM(rError,   ARGS) }

#define rThrowOnFailS(FUNC, ARGS) { \
  if (!(FUNC)) { \
    RLOG_STREAM(rError, ARGS); \
    throw RavlN::ExceptionOperationFailedC(strm.String().data(), true); \
  } \
}

#define rThrowBadConfigOnFailS(FUNC, ARGS) { \
  if (!(FUNC)) { \
    RLOG_STREAM(rError, ARGS); \
    throw RavlN::ExceptionBadConfigC(strm.String().data(), true); \
  } \
}

namespace RavlN {

  //! Initialise rlog to filename (filename can be stderr)
  bool RLogInit(int argc, char **argv, const char *filename, bool verbose,
                bool showDate = false, bool showThreadId = false);

  //! Initialise rlog to filename (filename can be stderr)
  bool RLogInit(const StringC &filename, const StringC &verbose, const StringC &logLevel,
                bool showDate = false, bool showThreadId = false);

  //! Initialise rlog to standard out
  bool RLogInit(bool verbose = false, bool showDate = false, bool showThreadId = false);

  //! Subscribe to a model.
  bool RLogSubscribe(const char *moduleName,const char *path = "",rlog::LogLevel level = rlog::Log_Undef);

  //! Subscribe to a channel (_RLDebugChannel, _RLInfoChannel, _RLWarningChannel, _RLErrorChannel)
  bool RLogSubscribe(rlog::RLogChannel *Channel);

  //! Subscribe according to log level
  bool RLogSubscribeL(const char *LogLevel);
}

#endif
