/*****************************************************************************
 * Author:   Alexey Kostin
 *
 *****************************************************************************
 * This code is based on StdioNodeC class from RLog library written by Valient Gough
 *
 * This library is free software; you can distribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (LGPL), as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL in the file COPYING for more
 * details.
 *
 */
//! lib=RavlRLog
// file-header-ends-here
		                                                                                

#include "Ravl/StdioDateNode.hh"

#include <rlog/RLogPublisher.h>
#include <rlog/RLogChannel.h>
#ifndef _WIN32
#include <pthread.h>
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"

#  ifdef HAVE_SSTREAM
#    include <sstream>
#  else
// old STL - use strstream instead of stringstream
#    include <strstream.h>
#    define USE_STRSTREAM
#  endif

#else
#  include <sstream>
#endif

#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
#  include <io.h>
#  define write(fd, buf, n) _write((fd), (buf), static_cast<unsigned>(n))
#else
#  include <unistd.h>
#  define USE_COLOURS
#endif

using namespace rlog;
using namespace std;


const char kNormalColor[] = "\033[0m";
const char kRedColor[]    = "\033[31m";
const char kGreenColor[]  = "\033[32m";
const char kYellowColor[] = "\033[33m";


StdioDateNode::StdioDateNode(int _fdOut, int flags)
    : StdioNode(_fdOut, flags)
{
}

StdioDateNode::StdioDateNode(int _fdOut, bool colorizeIfTTY)
    : StdioNode(_fdOut, colorizeIfTTY)
{
}

StdioDateNode::~StdioDateNode()
{
}

void StdioDateNode::publish( const RLogData &data )
{
  char timeStamp[32];
  time_t errTime = data.time;
  tm currentTime;
   
#ifdef HAVE_LOCALTIME_R
  localtime_r( &errTime, &currentTime );
#else
  currentTime = *localtime( &errTime );
#endif

  const char *color = NULL;
  if(colorize)
  {
    sprintf(timeStamp, "%s%04i-%02i-%02i %02i:%02i:%02i%s ",
            kGreenColor,
            currentTime.tm_year + 1900,
            currentTime.tm_mon + 1,
            currentTime.tm_mday,
            currentTime.tm_hour,
            currentTime.tm_min,
            currentTime.tm_sec,
            kNormalColor);
    
    string channel = data.publisher->channel->name();
    LogLevel level = data.publisher->channel->logLevel();

    switch(level)
    {
    case Log_Critical:
    case Log_Error:
      color = kRedColor;
      break;
    case Log_Warning:
      color = kYellowColor;
      break;
    case Log_Notice:
    case Log_Info:
    case Log_Debug:
    case Log_Undef:
      break;
    }
  } else {
    sprintf(timeStamp, "%04i-%02i-%02i %02i:%02i:%02i ",
            currentTime.tm_year + 1900,
            currentTime.tm_mon + 1,
            currentTime.tm_mday,
            currentTime.tm_hour,
            currentTime.tm_min,
            currentTime.tm_sec);
  }

#ifdef USE_STRSTREAM
  ostrstream ss;
#else
  ostringstream ss;
#endif

  ss << timeStamp;
  if (outputChannel) {
    ss << '[' << data.publisher->channel->name() << "] ";
  }
  if (outputContext) {
    ss << "(" << data.publisher->fileName << ':'
       << data.publisher->lineNum << ") ";
  }
#ifndef _WIN32
  if (outputThreadId) {
    char tid[16] = "";
    snprintf(tid,15,"%lu",pthread_self());
    ss << "[tid:" << tid << "] ";
  }
#endif

  if(color)
    ss << color;

  ss << data.msg;
    
  if(color)
    ss << kNormalColor;

  ss << '\n';

  string out = ss.str();
  write( fdOut, out.c_str(), out.length() );
}

