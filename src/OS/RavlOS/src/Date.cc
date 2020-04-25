// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Time/Date.cc"

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

#if defined(__linux__)
#include <sched.h>
#endif

#if RAVL_HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#define DODEBUG 1
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif


namespace RavlN {

  DateC::DateC(bool setval,bool useVirt)  {
    if(setval)
      SetToNow(useVirt);
    else {
      sec = 0;
      usec = -1;
    }
  }

  //: Construct from a stream
  
  DateC::DateC(istream &in) { 
    in >> sec >> usec; 
  }
  
  //: Write to a stream.
  
  void DateC::Save(ostream &out) const { 
    out << sec << " " << usec; 
  }
  
  //: Construct from a real in seconds.
  
  DateC::DateC(RealT val) {
    sec = (Int64T) val;
    usec = (long) ((RealT) (val - (RealT) sec) * MaxUSeconds());
  }
  
  //: String constructor
  // Expects data in sec.usec format 
  // as produced by Text() method.
  
  DateC::DateC(const StringC &str) 
    : sec(0), usec(0)
  {
    int sep = str.index('.');
    if(sep < 0) sep = str.index(':');
    if(sep < 0) {
      cerr << "DateC::DateC(), ERROR: String in unrecognised format. '" << str << "'\n";
      return ;
    }
    StringC numS(str);
    sec = atol(StringC(numS.before(sep)));
    for (UIntT i(sep+1); i<str.Size(); ++i)
      usec = (usec *10 + atoi(StringC(str[i])));
  }
  
  //: Is give year a leap year ?
  
  bool DateC::IsLeapYear(int year)  {
    if((year % 4) != 0)
      return false;
    if((year % 100) != 0)
      return true;
    // Is a leap year if century is not divisable by 4.
    return ((year % 400) == 0);
  }
  
  //: Convert year to days since 1970
  DateC::SecondT DateC::YearToDaysSince1970(int year)  {
    SecondT yearx =  year - 1970;
    SecondT yearl = year - 1;
    return (yearx * 365) + (yearl / 4) - (yearl / 100) + (yearl / 400) - 477;
  }
  
  //: Constructor.
  
  DateC::DateC(IntT year,IntT month,IntT day,
               IntT hour,IntT min,IntT asec,
               IntT nusec,bool useLocalTimeZone)
    : sec(0),
      usec(nusec)
  {
    if(!useLocalTimeZone) {
      //                                        Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov,Dec,x
      //static SecondT daysin[14]         = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };
      static SecondT daysintoyear[14]     = { 0,  0, 31, 59, 90,120,151,181,212,243,273,304,334, 365};
      //static SecondT daysleapin[14]     = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };
      static SecondT daysintoleapyear[14] = { 0,  0, 31, 60, 91,121,152,182,213,244,274,305,335, 366};
      // Normalise month.
      if(month < 1) {
        int yearOffset = ((month-1) / 12)-1;
        year += yearOffset;
        month -= 12 * yearOffset;
      }
      if(month > 12) {
        int yearOffset = ((month-1) / 12);
        year += yearOffset;
        month -= 12 * yearOffset;
      }

      // Convert year into an offset in seconds.
      sec = YearToDaysSince1970(year) * 24 * 60 * 60;
      // Convert month into an offset in seconds.
      if(IsLeapYear(year)) {
	sec += daysintoleapyear[month] * 24 * 60 * 60;
        //daysInMonth = daysleapin[month];
      } else {
	sec += daysintoyear[month] * 24 * 60 * 60;
        //daysInMonth = daysin[month];
      }
      
      // The rest of the parameters don't need normalising.
      // as things work out properly anyway
      sec += (day-1) * (24 * 60 * 60);
      sec += hour * (60 * 60);
      sec += min * 60;
      sec += asec;
    } else {
      struct tm ttm;
      ttm.tm_year = year - 1900;
      ttm.tm_mon = month-1; 
      ttm.tm_mday = day;
      ttm.tm_hour = hour;
      ttm.tm_min = min;
      ttm.tm_sec = asec;
      ttm.tm_wday = -1;
      ttm.tm_yday = -1;
      ttm.tm_isdst = -1;
      sec = mktime(&ttm);
      if(sec == -1) {
	std::cerr << "DateC::DateC(), Illegal date. " << sec << "\n";
      }
    }
  }
  

  //: Get the time in local timezone
  
  DateC DateC::NowLocal() {
#if !RAVL_COMPILER_VISUALCPP    
    // Get time of day
    struct timeval tv;
    gettimeofday(&tv,0);
    DateC ret(tv.tv_sec,tv.tv_usec);
    
    // Go and get the local time offset.
    struct tm b;
    time_t s = (time_t) tv.tv_sec;
    localtime_r(&s,&b);
    
    //cerr << "DateC::NowUTC " << tz.tz_minuteswest << " Raw=" << ret.ODBC() << " \n";
    ret += DateC((time_t)b.tm_gmtoff, 0l);
    return ret;
#else
#if RAVL_OS_WIN32
    struct _timeb ltime;
    _ftime(&ltime);
	return DateC((long)ltime.time, (long)ltime.millitm * 1000);
#else
    throw ExceptionC("DateC::NowLocal(), Not implemented. ");
#endif
#endif
  }
  
  //: Get an invalid time.

  DateC DateC::InvalidTime() {
    DateC ret;
    ret.SetInvalid();
    return ret;
  }

  //: Get a time far in the past.

  DateC DateC::FarPast() {
    // Though we can support 64 bit numbers, quite a few of the functions
    // may use 32 bit integers, so we'll stick with the highest possible value
    // for them for the moment.
    return DateC((SecondT) -2147483646,0);
  }

  //: Get a time as far in the future as possible.

  DateC DateC::FarFuture() {
    // Though we can support 64 bit numbers, quite a few of the functions
    // may use 32 bit integers, so we'll stick with the highest possible value
    // for them for the moment.
    return DateC((SecondT) 2147483647,0);
  }

  //: Get the current time in Coordinated Universal Time  (UTC)
  
  DateC DateC::NowUTC() {
#if !RAVL_COMPILER_VISUALCPP
    struct timeval tv;
    gettimeofday(&tv,0);
    return DateC(tv.tv_sec,tv.tv_usec);
#else
#if RAVL_OS_WIN32
    struct _timeb ltime;
    _ftime(&ltime);
    return DateC((long)ltime.time, (long)ltime.millitm * 1000);
#else
    throw ExceptionC("DateC::NowLocal(), Not implemented. ");
#endif
#endif
  }
  
  //: Get the time since the process started.
  
  DateC DateC::NowVirtual() {
#if !RAVL_OS_WIN32
    DateC ret((long)0,(long)clock());
    ret.NormalisePos();
    return ret;
#else
    throw ExceptionC("DateC::NowVirtual(), Not implemented. ");
#endif
  }
  
  
  //: Set value of variable to now!
  
  void DateC::SetToNow(bool useVirt)  {
#if !RAVL_COMPILER_VISUALCPP
    if(useVirt) {
      usec = clock(); // FIXME :- This will fail after about 36 mintes.
      sec = 0;
      NormalisePos();    
      return ;
    }
    struct timeval tv;
    gettimeofday(&tv,0);
    sec =tv.tv_sec;
    usec =tv.tv_usec;
#else
#if RAVL_OS_WIN32
    struct _timeb ltime;
    _ftime(&ltime);
    sec = ltime.time;
    usec = ltime.millitm * 1000;
#else
    throw ExceptionC("DateC::SetToNow(), Not implemented. ");
#endif
#endif
  }
  
  //: Get the local timezone offset.  (Note around daylight saving this may change.)
  
  DateC DateC::TimeZoneOffset() {
#if !RAVL_COMPILER_VISUALCPP    
    struct timeval tv;
    gettimeofday(&tv,0);
    
    // Get time of day
    // Go and get the local time offset.
    struct tm b;
    time_t s = (time_t) tv.tv_sec;
    localtime_r(&s,&b);
    
    return DateC((time_t)-b.tm_gmtoff, 0l);
#else
    throw ExceptionC("DateC::TimeZoneOffset(), Not implemented. ");
#endif    
  }
  
  //: Get the time in string form.
  
  StringC DateC::Text() const  { 
    StringC str;
    str.form("%d.%06d", (unsigned int)sec, (unsigned int)usec);
    return str; 
  }


  //: Return the date and time in ODBC format
  
  StringC DateC::ODBC(bool convertUTCToLocal,bool factionalSeconds) const {
    StringC str;
    if(factionalSeconds) {
      UIntT seconds = Seconds(convertUTCToLocal);
      // Fix formating problem where leading zero is removed from floats.
      if(seconds < 10) {
        str.form("%04d-%02d-%02d %02d:%02d:0%1f",
            Year(convertUTCToLocal),
            Month(convertUTCToLocal),
            DayInMonth(convertUTCToLocal),
            Hour(convertUTCToLocal),
            Minute(convertUTCToLocal),
            (double) seconds + (usec / 1000000.0)
        );
      } else {
        str.form("%04d-%02d-%02d %02d:%02d:%02f",
            Year(convertUTCToLocal),
            Month(convertUTCToLocal),
            DayInMonth(convertUTCToLocal),
            Hour(convertUTCToLocal),
            Minute(convertUTCToLocal),
            (double) seconds + (usec / 1000000.0)
        );
      }
    } else {
      str.form("%04d-%02d-%02d %02d:%02d:%02d",
               Year(convertUTCToLocal),
               Month(convertUTCToLocal),
               DayInMonth(convertUTCToLocal),
               Hour(convertUTCToLocal),
               Minute(convertUTCToLocal),
               Seconds(convertUTCToLocal)
               );
    }
    return str;
  }


  bool DateC::DecomposeISO8601String(const StringC &dataString,
                                  UIntT &year,
                                  UIntT &month,
                                  UIntT &day,
                                  UIntT &hour,
                                  UIntT &min,
                                  UIntT &sec,
                                  UIntT &usec,
                                  IntT & tzOffset)
  {
    const StringC &work = dataString;
    int at = 0;
    year = 0;
    month = 1;
    day = 1;
    hour = 0;
    min = 0;
    sec = 0;
    usec = 0;
    tzOffset = 0;

    //2012-01-30 10:00Z
    //2012-01-30T10:00Z
    //2012-01-30T10:00:00Z

    if(dataString.IsEmpty())
      return false;
    if(!isdigit(work[at]))
      return false;
    int elems = sscanf(&work[at],"%4u-%2u-%2u",&year,&month,&day);
    if(elems != 3) {
      ONDEBUG(RavlDebug("Ran out of characters in: '%s' ",work.c_str()));
      return false;
    }
    if(work.Size() <= 10) {
      // No timezone, so must be local time.
      tzOffset = DateC::TimeZoneOffset().TotalSeconds();
      return true;
    }
    at += 10;
    if(work[at] != ' ' && work[at] != 'T') {
      ONDEBUG(RavlDebug("Ran out of characters."));
      return false;
    }
    at++;
    if(at >= work.Size()) {
      ONDEBUG(RavlDebug("Ran out of characters in '%s' ",dataString.c_str()));
      return false;
    }
    //RavlDebug("Scanning time:%s ",work.c_str());
    double secf;
    elems = sscanf(&work[at],"%2u:%2u:%lf",&hour,&min,&secf);
    if(elems != 3) {
      ONDEBUG(RavlDebug("Not 3 elements."));
      return false;
    }
    sec = secf;
    usec = RavlN::Round((secf - floor(secf)) * 1000000.0);
    at += 8;
    if(at >= work.Size()) {
      // No timezone, so must be local time.
      tzOffset = DateC::TimeZoneOffset().TotalSeconds();
      return true;
    }
    if(work[at] == '.') {
      at++;
      // Fractional seconds ?
      for(;at < work.Size();at++) {
        if(!isdigit(work[at]))
          break;
      }
    }
    if(work.Size() == at) {
      // No timezone, so must be local time.
      tzOffset = DateC::TimeZoneOffset().TotalSeconds();
      return true;
    }
    if(work[at] == 0) {
      // No timezone, so must be local time.
      tzOffset = DateC::TimeZoneOffset().TotalSeconds();
      return true;
    }

    if(work[at] == 'Z') {
      // UTZ timezone
      return true;
    }
    if(work[at] != '+' && work[at] != '-') {
      RavlError("Invalid timezone %s ",work.c_str());
      return false;
    }
    bool isNeg = work[at] == '-';
    at++;
    if(at >= work.Size()) {
      ONDEBUG(RavlDebug("Ran out of characters."));
      return false;
    }
    int tzHours = 0;
    int tzMinutes = 0;
    elems = sscanf(&work[at],"%2u",&tzHours);
    if(work.Size()-at > 2) {
      at += 2;
      if(work[at]==':')
        at++;
      elems = sscanf(&work[at],"%2u",&tzMinutes);
    }

    tzOffset = tzHours * 60 + tzMinutes;
    if(isNeg) tzOffset *= -1;

    return true;
  }

  //: Generate date from ISO8601 string.
  // Note this may not support all variants, if the string fails to parse and exception will be thrown.

  DateC DateC::FromISO8601String(const StringC &dateString,bool storeInUTC)
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
      return DateC::InvalidTime();

    if(!DecomposeISO8601String(dateString,
                               year,
                               month,
                               day,
                               hour,
                               min,
                               sec,
                               usec,
                               tzOffset)) {

      //RavlDebug("Failed to parse date from: '%s' ",dateString.data());
      throw ExceptionOperationFailedC("Parse error in date.");
    }
    DateC ret(year,month,day,hour,min,sec,usec);
    if(!storeInUTC)
      ret += tzOffset * 60.0;
    return ret;
  }

  //: Generate date from odbc string.
  
  DateC DateC::FromODBCString(const StringC &dateStr, bool isLocalTimeZone) {
    DateC ret;
    ret.SetODBC(dateStr, isLocalTimeZone);
    return ret;
  }
  
  //: Generate date from a string.
  //!param: dataString - ODBC style date e.g. '2012-11-22 00:00:00'
  //!param: isLocalTimeZone - When true assumes parameters are in the local timezone.  It will convert to UTC.
  DateC DateC::FromString(const StringC &dateString, bool isLocalTimeZone)
  {
    DateC ret;
    ret.Set(dateString, isLocalTimeZone);
    return ret;
  }

  //: Interpret from a string using 'strptime' formatting.
  DateC DateC::FromStringFormat(const StringC &dataString,const char *format)
  {
    struct tm timeInfo;
    memset(&timeInfo,0,sizeof(tm));
    if(strptime(dataString.c_str(),format,&timeInfo) == 0)
      return RavlN::DateC::InvalidTime();
    return RavlN::DateC(1900 + timeInfo.tm_year,timeInfo.tm_mon+1,timeInfo.tm_mday,timeInfo.tm_hour,timeInfo.tm_min,timeInfo.tm_sec,0);
  }

  //: Set date to time string, year/month/day hour:min:sec
  // Returns true if conversion successful, false
  // if string is not recognised.
  //!param: odbcStr - ODBC style date e.g. '2012-11-22 00:00:00'
  //!param: isLocalTimeZone - When true assumes parameters are in the local timezone.  It will convert to UTC.
  bool DateC::Set(const StringC &odbcStr, bool isLocalTimeZone)
  {
    // Empty field ?
    if(odbcStr.Size() < 19)
      return false;

    IntT seconds, minute, hour, day, month, year;
    StringC tmp = odbcStr.Copy();
    char *str = &tmp[0];
    char *ptr = str;

    // Check the stream is formated correctly.
    if((str[4] != '-' && str[4] != '/') || (str[7] != '-' && str[7] != '/') || str[10] != ' ' || str[13] != ':' || str[16] != ':')
      return false;

    // Get seconds
    ptr = str + 17;
    double intPart;
    double secondsFloat = atof(ptr);
    long usecPart = Round(modf(secondsFloat,&intPart) * 1000000.0);
    seconds = Round(intPart);

    // Get minute
    str[16] = 0;
    ptr = str + 14;
    minute = atoi(ptr);
    // Get hour
    str[13] = 0;
    ptr = str + 11;
    hour = atoi(ptr);
    // Get day
    str[10] = 0;
    ptr = str + 8;
    day = atoi(ptr);
    // Get month
    str[7] = 0;
    ptr = str + 5;
    month = atoi(ptr);
    // Get year
    str[4] = 0;
    ptr = str;
    year = atoi(ptr);

    // Create date structure
    *this = DateC(year, month, day, hour, minute, seconds, usecPart, isLocalTimeZone);

    return true;
  }

  //: Set date to odbc specified time string.
  // Returns true if conversion successful, false
  // if string is not recognised.
  
  bool DateC::SetODBC(const StringC &odbcStr, bool isLocalTimeZone) {
   
    // Empty field ?
    
    if(odbcStr.Size() < 19)
      return false;
    
    IntT seconds, minute, hour, day, month, year;
    StringC tmp = odbcStr.Copy();
    char *str = &tmp[0];
    char *ptr = str;
    
    // Check the stream is formated correctly.
    if(str[4] != '-' || str[7] != '-'  || str[10] != ' ' || str[13] != ':' || str[16] != ':')
      return false;
    
    // Get seconds
    ptr = str + 17;
    double intPart;
    double secondsFloat = atof(ptr);
    long usecPart = Round(modf(secondsFloat,&intPart) * 1000000.0);
    seconds = Round(intPart);
    
    // Get minute
    str[16] = 0;
    ptr = str + 14;
    minute = atoi(ptr);
    // Get hour
    str[13] = 0;
    ptr = str + 11;
    hour = atoi(ptr);
    // Get day
    str[10] = 0;
    ptr = str + 8;
    day = atoi(ptr);
    // Get month
    str[7] = 0;
    ptr = str + 5;
    month = atoi(ptr);
    // Get year
    str[4] = 0;
    ptr = str;
    year = atoi(ptr);
    
    // Create date structure
    *this = DateC(year, month, day, hour, minute, seconds, usecPart, isLocalTimeZone);
    
    return true;
  }

  //: Format as ISO8601 string

  StringC DateC::ISO8601(bool asUTC) const
  {
    struct tm b;
    if(!IsValid())
      return "undefined";

    time_t s = (time_t) TotalSeconds();
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

    double sec = (double) b.tm_sec + ((double)USeconds() / 1000000.0);
    if(asUTC) {
      if(b.tm_sec < 10) {
        buf.form("%04u-%02u-%02uT%02u:%02u:0%1fZ",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec);
      } else {
        buf.form("%04u-%02u-%02uT%02u:%02u:%2fZ",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec);
      }
    } else {
      if(b.tm_sec < 10) {
        buf.form("%04u-%02u-%02uT%02u:%02u:0%1f",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec);
      } else {
        buf.form("%04u-%02u-%02uT%02u:%02u:%2f",b.tm_year + 1900,b.tm_mon+1,b.tm_mday,b.tm_hour,b.tm_min,sec);
      }
    }
    return buf;
  }

  //: Returns results equivalent to calling ctime().
  
  StringC DateC::CTime(bool useUTCToLocal) const  {
    char buff[50];
    time_t s = (time_t) sec;
    StringC ret;
    if(useUTCToLocal) {
      ret = StringC(ctime_r(&s,buff));
    } else {
      time_t s = (time_t) sec;
#if !RAVL_COMPILER_VISUALCPP
      struct tm b;
      ret = StringC(asctime_r(gmtime_r(&s,&b),buff));
#else
      // VC++ does not support asctime_r or gmtime_r so use the other versions
      // in lieu of anything else.
      ret = StringC(asctime(gmtime(&s)));
#endif
    }
    ret.del("\n"); // Get rid of return.
    return ret;
  }
  
  //: Returns a short string containing date/time.
  
  StringC DateC::CTimeShort(bool useUTCToLocal) const {
    StringC buf;
    struct tm b;
    time_t s = (time_t) sec;
    if (useUTCToLocal) {
      localtime_r(&s,&b);
    } else {
#if !RAVL_COMPILER_VISUALCPP
      gmtime_r(&s,&b);
#else
      // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
      // in lieu os anythings else
      // In VC++ the result is stored in thread local storage, so this should
      // be thread safe.
      b = *gmtime(&s);
#endif
    }
    buf += StringC(b.tm_hour) + ":" + StringC(b.tm_min) + ":" + StringC(b.tm_sec);
    buf += "-";
    buf += StringC(b.tm_mday) + "/" + StringC(b.tm_mon) + "/" + StringC(b.tm_year + 1900);
    return buf;
  }
  
  
  //: Return number of seconds after minuite. 0,61 (61 for leap seconds.)
  
  IntT DateC::Seconds(bool useUTCToLocal) const  {
    struct tm b;
    time_t s = (time_t) sec;
    if (useUTCToLocal) {
      localtime_r(&s,&b);
    } else {
#if !RAVL_COMPILER_VISUALCPP
      gmtime_r(&s,&b);
#else
      // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
      // in lieu os anythings else
      // In VC++ the result is stored in thread local storage, so this should
      // be thread safe.
      b = *gmtime(&s);
#endif
    }
    return b.tm_sec;
  }
  
  //: Get minute.
  
  IntT DateC::Minute(bool useUTCToLocal) const  {
    struct tm b;
    time_t s = (time_t) sec;
    if (useUTCToLocal) {
      localtime_r(&s,&b);
    } else {
#if !RAVL_COMPILER_VISUALCPP
      gmtime_r(&s,&b);
#else
      // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
      // in lieu os anythings else
      // In VC++ the result is stored in thread local storage, so this should
      // be thread safe.
      b = *gmtime(&s);
#endif
    }
    return b.tm_min;
  }
  
  
  //: Hours since midnight. 0-23
  
  IntT DateC::Hour(bool useUTCToLocal) const  {
    struct tm b;
    time_t s = (time_t) sec;
    if (useUTCToLocal) {
      localtime_r(&s,&b);
    } else {
#if !RAVL_COMPILER_VISUALCPP
      gmtime_r(&s,&b);
#else
      // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
      // in lieu os anythings else
      // In VC++ the result is stored in thread local storage, so this should
      // be thread safe.
      b = *gmtime(&s);
#endif
    }
    return b.tm_hour;  
  }
  
  //: Get month 1-31
  
  IntT DateC::Month(bool useUTCToLocal) const  {
    struct tm b;
    time_t s = (time_t) sec;
    if (useUTCToLocal) {
      localtime_r(&s,&b);
    } else {
#if !RAVL_COMPILER_VISUALCPP
      gmtime_r(&s,&b);
#else
      // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
      // in lieu os anythings else
      // In VC++ the result is stored in thread local storage, so this should
      // be thread safe.
      b = *gmtime(&s);
#endif
    }
    return b.tm_mon + 1;
  }
  
  
  //: Get year.
  
  IntT DateC::Year(bool useUTCToLocal) const  {
    struct tm b;
    time_t s = (time_t) sec;
    if (useUTCToLocal) {
      localtime_r(&s,&b);
    } else {
#if !RAVL_COMPILER_VISUALCPP
      gmtime_r(&s,&b);
#else
      // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
      // in lieu os anythings else
      // In VC++ the result is stored in thread local storage, so this should
      // be thread safe.
      b = *gmtime(&s);
#endif
    }
    return b.tm_year + 1900;  
  }
  
  //: Get day in month.  1,31
  
  IntT DateC::DayInMonth(bool useUTCToLocal) const  {
    struct tm b;
    time_t s = (time_t) sec;
    if (useUTCToLocal) {
      localtime_r(&s,&b);
    } else {
#if !RAVL_COMPILER_VISUALCPP
      gmtime_r(&s,&b);
#else
      // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
      // in lieu os anythings else
      // In VC++ the result is stored in thread local storage, so this should
      // be thread safe.
      b = *gmtime(&s);
#endif
    }
    return b.tm_mday;    
  }
  
  //: Get day of year. 0-365
  
  IntT DateC::DayInYear(bool useUTCToLocal) const  {
    struct tm b;
    time_t s = (time_t) sec;
    if (useUTCToLocal) {
      localtime_r(&s,&b);
    } else {
#if !RAVL_COMPILER_VISUALCPP
      gmtime_r(&s,&b);
#else
      // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
      // in lieu os anythings else
      // In VC++ the result is stored in thread local storage, so this should
      // be thread safe.
      b = *gmtime(&s);
#endif
    }
    return b.tm_yday;
  }
  
  
  //: Get day of week. Since sunday, 0-6

  IntT DateC::DayInWeek(bool useUTCToLocal) const  {
    struct tm b;
    time_t s = (time_t) sec;
    if (useUTCToLocal) {
      localtime_r(&s,&b);
    } else {
#if !RAVL_COMPILER_VISUALCPP
      gmtime_r(&s,&b);
#else
      // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
      // in lieu os anythings else
      // In VC++ the result is stored in thread local storage, so this should
      // be thread safe.
      b = *gmtime(&s);
#endif
    }
    return b.tm_wday;  
  }

  //: Get day of week in text form.
  
  const StringC &DateC::TextDayInWeek(bool useUTCToLocal) const {
    static StringC days[7] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    return days[DayInWeek(useUTCToLocal)];
  }
  
  //: Are we daylight saveing ?
  // True = yes
  
  bool DateC::DaylightSaving() const  {
    struct tm b;
    time_t s = (time_t) sec;

#if !RAVL_COMPILER_VISUALCPP
    gmtime_r(&s,&b);
#else
    // VC++ does not support asctime_r or gmtime_r so use the non-thread-safe versions
    // in lieu os anythings else
    // In VC++ the result is stored in thread local storage, so this should
    // be thread safe.
    b = *gmtime(&s);
#endif
    return b.tm_isdst > 0;
  }
  
  ostream &operator <<(ostream &out,const DateC &date) {
    date.Save(out);
    return out;
  }
  
  //: Stream operator.
  
  istream &operator >>(istream &in,DateC &date) {
    date = DateC(in);
    return in; 
  }

#if 1
  //: Wait until this time.
  
  bool DateC::Wait() const {
#if !RAVL_COMPILER_VISUALCPP
    struct timeval timeout;
#if RAVL_OS_LINUX || RAVL_OS_LINUX64
    // Linux select modifies 'timeout' to time not slept, so we only have to setup once.
    DateC now = DateC::NowUTC();
    if(now >= *this)
      return true;
    DateC toGo = *this - now;
    //cerr << "Delay=" << toGo << "\n";
    timeout.tv_sec = toGo.TotalSeconds();
    timeout.tv_usec = toGo.USeconds();
    while(timeout.tv_sec > 0 || timeout.tv_usec > 0) {
      select(0,0,0,0,&timeout);
      // A signal may throw us out of select early so check we're finished.
    };
    
#else
    do {
      DateC now = DateC::NowUTC();
      if(now >= *this)
	return true;
      DateC toGo = *this - now;
      timeout.tv_sec = toGo.TotalSeconds();
      timeout.tv_usec = toGo.USeconds();
      select(0,0,0,0,&timeout);
      // A signal may throw us out of select early.
    } while(1);
#endif
#else
#if RAVL_HAVE_WIN32_THREADS
    do {
      DateC now = DateC::NowUTC();
      if(now >= *this)
	return true;
      DateC toGo = *this - now;
      ::Sleep(Round(toGo.Double() * 1000));
      // A signal may throw us out of select early.
    } while(1);
#else
    throw ExceptionC("DateC::Wait(), Not implemented. ");
#endif
#endif
    return true;
  }

  //: Pause execution for 'delay' seconds.
  
  bool Sleep(RealT delay) {
#if RAVL_HAVE_WIN32_THREADS
    ::Sleep(Round(delay * 1000));
    return true;
#else
#if RAVL_OS_LINUX || RAVL_OS_LINUX64
    //cerr << "Delay=" << toGo << "\n";
    if(delay <= 0)
      return true;
    struct timeval timeout;
    double dummy;
    timeout.tv_sec = Floor(delay);
    timeout.tv_usec = Round(modf(delay,&dummy) * 1000000.0);
    while(timeout.tv_sec > 0 || timeout.tv_usec > 0) {
      //std::cerr << " Sec=" << timeout.tv_sec << " usec=" << timeout.tv_usec << "\n";
      select(0,0,0,0,&timeout);
      // A signal may throw us out of select early so check we're finished.
    };
    return true;
#else
    DateC now(true);
    now += delay;
    return now.Wait();
#endif
#endif
  }
#endif

  const DateC &DateC::operator+=(double val) {
    double frac;
    if(!IsValid())
      return *this;
    usec += Round(modf(val,&frac) * 1000000);
    sec += (SecondT) frac;
    NormalisePos();
    return *this;
  }

  const DateC &DateC::operator-=(double val) {
    double frac;
    if(!IsValid())
      return *this;
    usec -= Round(modf(val,&frac) * 1000000);
    sec -= (SecondT) frac;
    NormaliseNeg();
    return *this;
  }

  void DateC::NormalisePos()  {
    if(usec >= MaxUSeconds()) {
      int diff = usec / MaxUSeconds();
      usec -= diff * MaxUSeconds();
      sec += diff;
    }
  }

  void DateC::NormaliseNeg()  {
    if(usec < 0) {
      int diff = (-usec / MaxUSeconds())+1;
      usec += diff * MaxUSeconds();
      sec -= diff;
    }
  }

  DateC DateC::operator+(const DateC &oth) const  {
    DateC ret;
    if(!IsValid() || !oth.IsValid()) return *this;
    ret.sec = sec + oth.sec;
    ret.usec = usec + oth.usec;
    ret.NormalisePos();
    return ret;
  }

  DateC DateC::operator-(const DateC &oth) const  {
    DateC ret;
    if(!IsValid() || !oth.IsValid()) return *this;
    ret.sec = sec - oth.sec;
    ret.usec = usec - oth.usec;
    ret.NormaliseNeg();
    return ret;
  }

  const DateC &DateC::operator-=(const DateC &val)
  {
    if(!IsValid() || !val.IsValid()) return *this;
    sec -= val.sec;
    usec -= val.usec;
    NormaliseNeg();
    return *this;
  }

  const DateC &DateC::operator+=(const DateC &val)  {
    if(!IsValid() || !val.IsValid()) return *this;
    sec += val.sec;
    usec += val.usec;
    NormalisePos();
    return *this;
  }

    
}
