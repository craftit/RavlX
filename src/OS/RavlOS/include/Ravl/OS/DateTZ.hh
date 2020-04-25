// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATETZ_HEADER
#define RAVL_DATETZ_HEADER 1
//////////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! lib=RavlOS
//! docentry="Ravl.API.OS.Time"
//! file="Ravl/OS/Time/Date.hh"
//! date="13/06/2014"

#include "Ravl/OS/Date.hh"

namespace RavlN {
  class StringC;
  class BinIStreamC;
  class BinOStreamC;

  //: Date, Time and Timezone information.
  // The timezone is stored as offset in minute from GMT
  // SMALL OBJECT.<br>

  class DateTZC
  {
  public:
    DateTZC();
    //: Default constructor
    //: Creates time at the epoch, with GMT time zone.

    DateTZC(RealT val);
    //: Construct from a real in seconds.

    DateTZC(const DateC &localTime,IntT timezoneOffsetMinutes = 0);
    //: Construct from a date and a timezone offset

    DateTZC(IntT year,IntT month,IntT day,IntT hour = 0,IntT min = 0,IntT sec = 0,IntT usec = 0,IntT timezoneOffsetMinutes = 0);
    //: Constructor.
    //!param: year - Year (Must be from 1901 to 2038 inclusive)
    //!param: month - Month in year 1-12 (Unlike mktime, which goes 0 to 11)
    //!param: day - Day in the month 1 to 31
    //!param: hour - 0 to 23
    //!param: min - 0 to 59
    //!param: sec - 0 to 59
    //!param: usec - 1000000's of a second.
    //!param: useLocalTimeZone - When true assume parameters are in the local time zone and convert to UTC

    static DateTZC FromISO8601String(const StringC &dataString);
    //: Generate date from ISO8601 string.
    // Note this may not support all variants, if the string fails to parse and exception will be thrown.

    static DateTZC FromStringFormat(const StringC &dataString,const char *format);
    //: Interpret from a string using 'strptime' formatting.

    static DateTZC Now();
    //: Get the time now

    static DateTZC InvalidTime();
    //: Get an invalid time.

    RavlN::DateC UTC() const;
    //: Time in UTC.

    inline bool IsValid() const
    { return m_localTime.IsValid(); }
    //: Check we have a valid time.

    double Double() const
    { return UTC().Double(); }
    //: Access as number of seconds since epoc.

    operator RavlN::DateC () const
    { return UTC(); }
    //: Convert to a plain time in UTC.

    StringC ISO8601() const;
    //: Format as ISO8601 string

    //! Access local time
    const DateC &LocalTime() const
    { return m_localTime; }

    //! Access local time
    DateC &LocalTime()
    { return m_localTime; }

    //! Set local time
    void SetLocalTime(const DateC &theTime)
    { m_localTime = theTime; }

    //! Access the timezone offset in minutes.
    int TimeZoneOffsetMinutes() const
    { return m_timeZoneOffsetMinutes; }

    //! Set timezone in minutes.
    void SetTimeZoneOffsetMinutes(int min)
    { m_timeZoneOffsetMinutes = min; }

    const DateTZC &operator-=(const DateTZC &val);
    //: Subtract a time.

    const DateTZC &operator+=(const DateTZC &val);
    //: Add a time.

    const DateTZC &operator-=(const DateC &val);
    //: Subtract a time.

    const DateTZC &operator+=(const DateC &val);
    //: Add a time.

    size_t Hash() const
    { return UTC().Hash(); }
    //: Generate a hash value.

  private:
    DateC m_localTime;
    int m_timeZoneOffsetMinutes; // Offset from GMT in minutes.
  };

  std::ostream &operator <<(std::ostream &out,const DateTZC &date);
  //: Stream operator.

  std::istream &operator >>(std::istream &in,DateTZC &date);
  //: Stream operator.

  BinOStreamC &operator <<(BinOStreamC &out,const DateTZC &date);
  //: Stream operator.

  BinIStreamC &operator >>(BinIStreamC &in,DateTZC &date);
  //: Stream operator.

  inline bool operator==(const RavlN::DateTZC &time1,const RavlN::DateTZC &time2)
  { return time1.UTC() == time2.UTC(); }
  //: Compare times as if they were both in UTC()

  inline bool operator!=(const RavlN::DateTZC &time1,const RavlN::DateTZC &time2)
  { return time1.UTC() != time2.UTC(); }
  //: Compare times as if they were both in UTC()

  inline bool operator>(const DateTZC &oth1,const DateTZC &oth2)
  { return oth1.UTC() > oth2.UTC(); }
  //: Compare times as if they were both in UTC()

  inline bool operator<(const DateTZC &oth1,const DateTZC &oth2)
  { return oth1.UTC() < oth2.UTC(); }
  //: Compare times as if they were both in UTC()

  inline bool operator>=(const DateTZC &oth1,const DateTZC &oth2)
  { return oth1.UTC() >= oth2.UTC(); }
  //: Compare times as if they were both in UTC()

  inline bool operator<=(const DateTZC &oth1,const DateTZC &oth2)
  { return oth1.UTC() <= oth2.UTC(); }
  //: Compare times as if they were both in UTC()

  inline DateTZC operator+(const DateTZC &oth1,const DateTZC &oth2)
  { return DateTZC(oth1.LocalTime() + oth2.UTC(),oth1.TimeZoneOffsetMinutes()); }
  //: Compare times as if they were both in UTC()

  inline DateTZC operator-(const DateTZC &oth1,const DateTZC &oth2)
  { return DateTZC(oth1.LocalTime() - oth2.UTC(),oth1.TimeZoneOffsetMinutes()); }
  //: Compare times as if they were both in UTC()

}

#endif
