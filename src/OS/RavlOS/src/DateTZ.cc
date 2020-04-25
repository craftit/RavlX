
#undef _ANSI_SOURCE
#undef _POSIX_C_SOURCE

#include "Ravl/config.h"
#include "Ravl/SysLog.hh"

#if RAVL_OS_SOLARIS
#define _POSIX_PTHREAD_SEMANTICS 1
#define _REENTRANT 1
//#define __STDC__ 0
#include <time.h>
#endif

#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE 1
#endif

#if RAVL_OS_WIN32
// Turn off deprecated warnings for now, they're not deprecated on other platforms
// may introduce more platform specific fixes later.
#define _CRT_SECURE_NO_DEPRECATE 1
#include <sys/timeb.h>
#endif

#include <time.h>

#ifdef RAVL_OS_MACOSX
extern "C" {
  char *asctime_r(const struct tm *, char *);
  char *ctime_r(const time_t *, char *);
  struct tm *gmtime_r(const time_t *, struct tm *);
  struct tm *localtime_r(const time_t *, struct tm *);
}
#endif

#include "Ravl/Exception.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/String.hh"
#include "Ravl/StrStream.hh"

#include <stdlib.h>
#include <stdio.h>

#if RAVL_HAVE_WIN32_THREADS
#include <windows.h>
#endif

#if !RAVL_COMPILER_VISUALCPP
#include <sys/time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <string.h>
#else
#include <string.h>

#include <sys/types.h>

char *ctime_r(const time_t *s,char *buff) {
  strcpy(buff,ctime(s));
  return buff;
}

struct tm *localtime_r( const time_t *timer,struct tm *b) {
  memcpy(b,localtime(timer),sizeof(tm));
  return b;
}

extern int sleep(int x); // A hack, I hope it doesn't cause problems later...

#endif

#include "Ravl/OS/DateTZ.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {

  //: Default constructor
  DateTZC::DateTZC()
   : m_timeZoneOffsetMinutes(0)
  {}

  //: Construct from a real in seconds.
  DateTZC::DateTZC(RealT val)
   : m_localTime(val),
     m_timeZoneOffsetMinutes(0)
  {}

  //: Construct from a date and a timezone offset
  DateTZC::DateTZC(const DateC &localTime,IntT timezoneOffsetMinutes)
   : m_localTime(localTime),
     m_timeZoneOffsetMinutes(timezoneOffsetMinutes)
  {}

  //: Constructor.
  //!param: year - Year (Must be from 1901 to 2038 inclusive)
  //!param: month - Month in year 1-12 (Unlike mktime, which goes 0 to 11)
  //!param: day - Day in the month 1 to 31
  //!param: hour - 0 to 23
  //!param: min - 0 to 59
  //!param: sec - 0 to 59
  //!param: usec - 1000000's of a second.
  //!param: useLocalTimeZone - When true assume parameters are in the local time zone and convert to UTC
  DateTZC::DateTZC(IntT year,IntT month,IntT day,IntT hour,IntT min,IntT sec,IntT usec,IntT timezoneOffsetMinutes)
   : m_localTime(year,month,day,hour,min,sec,usec,false),
     m_timeZoneOffsetMinutes(timezoneOffsetMinutes)
  {}

  //: Get the time now

  DateTZC DateTZC::Now()
  { return DateTZC(DateC::NowLocal(),DateC::TimeZoneOffset().TotalSeconds()); }

  //: Get an invalid time.

  DateTZC DateTZC::InvalidTime()
  { return DateTZC(DateC::InvalidTime(),0); }

  //: Time in UTC.

  RavlN::DateC DateTZC::UTC() const
  {
    RavlN::DateC ret = m_localTime;
    ret += m_timeZoneOffsetMinutes * 60.0;
    return ret;
  }

  //: Generate date from ISO8601 string.
  // Note this may not support all variants, if the string fails to parse and exception will be thrown.
  DateTZC DateTZC::FromISO8601String(const StringC &dateString)
  {
    UIntT year = 0;
    UIntT month = 1;
    UIntT day = 1;
    UIntT hour = 0;
    UIntT min = 0;
    UIntT sec = 0;
    UIntT usec = 0;
    IntT tzOffset = 0;

    // Check for undefind date/time
    if(dateString == "undefined" || dateString == "")
      return DateTZC::InvalidTime();

    if(!DateC::DecomposeISO8601String(dateString,
                               year,
                               month,
                               day,
                               hour,
                               min,
                               sec,
                               usec,
                               tzOffset)) {

      RavlDebug("Failed to parse date: '%s' ",dateString.data());
      throw ExceptionOperationFailedC("Parse error in date.");
    }

    return DateTZC(year,month,day,hour,min,sec,usec,tzOffset);
  }

  //: Interpret from a string using 'strptime' formatting.
  DateTZC DateTZC::FromStringFormat(const StringC &dataString,const char *format)
  {
    struct tm timeInfo;
    memset(&timeInfo,0,sizeof(tm));
    if(strptime(dataString.c_str(),format,&timeInfo) == 0)
      return RavlN::DateTZC::InvalidTime();
    return RavlN::DateTZC(1900 + timeInfo.tm_year,timeInfo.tm_mon+1,timeInfo.tm_mday,timeInfo.tm_hour,timeInfo.tm_min,timeInfo.tm_sec,0,timeInfo.tm_isdst);
  }

  //: Format as ISO8601 string
  StringC DateTZC::ISO8601() const
  {
    struct tm b;

    if(!m_localTime.IsValid())
      return "undefined";

    time_t s = (time_t) m_localTime.TotalSeconds();
#if !RAVL_COMPILER_VISUALCPP
    gmtime_r(&s,&b);
#else
    // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
    // in lieu os anythings else
    // In VC++ the result is stored in thread local storage, so this should
    // be thread safe.
    b = *gmtime(&s);
#endif
    StringC buf;

    double sec = (double) b.tm_sec + ((double) m_localTime.USeconds() / 1000000.0);
    if(m_timeZoneOffsetMinutes == 0) {
      if(b.tm_sec < 10) {
        buf.form("%04u-%02u-%02uT%02u:%02u:0%1fZ",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec);
      } else {
        buf.form("%04u-%02u-%02uT%02u:%02u:%2fZ",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec);
      }
      return buf;
    }
    int absOff;
    char signChar;
    if(m_timeZoneOffsetMinutes < 0) {
      absOff = -m_timeZoneOffsetMinutes;
      signChar = '-';
    } else {
      absOff = m_timeZoneOffsetMinutes;
      signChar = '+';
    }
    int tzHour = absOff/60;
    int tzMin = absOff%60;
    if(tzMin == 0) {
      if(b.tm_sec < 10) {
        buf.form("%04u-%02u-%02uT%02u:%02u:0%1f%c%02u",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec,signChar,tzHour);
      } else {
        buf.form("%04u-%02u-%02uT%02u:%02u:%0f%c%02u",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec,signChar,tzHour);
      }
      return buf;
    }
    if(b.tm_sec < 10) {
      buf.form("%04u-%02u-%02uT%02u:%02u:0%1f%c0%1u:%02u",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec,signChar,tzHour,tzMin);
    } else {
      buf.form("%04u-%02u-%02uT%02u:%02u:%0f%c%02u:%02u",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec,signChar,tzHour,tzMin);
    }
    return buf;
  }

  const DateTZC &DateTZC::operator-=(const DateTZC &val)
  {
    m_localTime -= val.UTC();
    return *this;
  }

  const DateTZC &DateTZC::operator+=(const DateTZC &val)
  {
    m_localTime += val.UTC();
    return *this;
  }

  const DateTZC &DateTZC::operator-=(const DateC &val)
  {
    m_localTime -= val;
    return *this;
  }

  const DateTZC &DateTZC::operator+=(const DateC &val)
  {
    m_localTime += val;
    return *this;
  }


  ostream &operator <<(ostream &strm,const DateTZC &date)
  {
    strm << date.ISO8601();
    return strm;
  }
  //: Stream operator.

  istream &operator >>(istream &strm,DateTZC &date)
  {
    StringC str;
    strm >> str;
    date = DateTZC::FromISO8601String(str);
    return strm;
  }
  //: Stream operator.

  BinOStreamC &operator <<(BinOStreamC &strm,const DateTZC &date)
  {
    ByteT version = 1;
    strm << version;
    strm << date.LocalTime() << (Int32T) date.TimeZoneOffsetMinutes();
    return strm;
  }
  //: Stream operator.

  BinIStreamC &operator >>(BinIStreamC &strm,DateTZC &date)
  {
    ByteT version = 1;
    strm >> version;
    if(version != 1)
      throw ExceptionUnexpectedVersionInStreamC("DateTZC");
    Int32T tzOffset = 0;
    DateC localTime;
    strm >> localTime >> tzOffset;
    date = DateTZC(localTime,tzOffset);
    return strm;
  }
  //: Stream operator.


}
