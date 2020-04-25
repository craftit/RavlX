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

#include "Ravl/Exception.hh"
#include "Ravl/OS/DateRange.hh"
#include "Ravl/String.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/SysLog.hh"

namespace RavlN {
  
  //: Default constructor
  DateRangeC::DateRangeC()
  {
    *this = Today();
  }
  
  DateRangeC::DateRangeC(const DateC & from, const DateC & to) :
      m_from(from), m_to(to)
  {
    // FIXME: Check date range is valid.  Throw exception?
  }
  
  DateRangeC::DateRangeC(DateRangeT dateRange)
  {
    if (dateRange == DR_TODAY) {
      *this = Today();
    } else if (dateRange == DR_YESTERDAY) {
      *this = Yesterday();
    } else if (dateRange == DR_LAST_HOUR) {
      *this = Hours(1.);
    } else if (dateRange == DR_LAST_24HOURS) {
      *this = Hours(24.0);
    } else if(dateRange == DR_LAST_SEVENDAYS) {
      *this = Days(7.);
    } else if (dateRange == DR_LAST_THREEANDAHALFDAYS) {
      *this = Days(3.5);
    } else {
      RavlWarning("Unknown DataRangeT value, setting to today");
      *this = Today();
    }
  }

  //: Construct from a stream
  
  DateRangeC::DateRangeC(istream &in)
  {
    in >> m_from >> m_to;
  }
  
  //: Write to a stream.
  
  void DateRangeC::Save(ostream &out) const
  {
    out << m_from << " " << m_to;
  }
  
  DateRangeC DateRangeC::Today()
  {
    DateC now = DateC::NowLocal();

    // This will get midnight of today
    DateC start(now.Year(), now.Month(), now.DayInMonth(), 0, 0, 0, 0, true);

    // We want to go to the last second
    DateC end = start + (3600. * 24.) - 1e-5;

    DateRangeC dr(start, end);
    return dr;
  }

  DateRangeC DateRangeC::Yesterday()
  {
    DateC now = DateC::NowLocal();

    // This will get midnight of today
    DateC start(now.Year(), now.Month(), now.DayInMonth(), 0, 0, 0, 0, true);
    start -= (3600. * 24.);

    // We want to go to the last second
    DateC end = start + (3600. * 24.) - 1e-5;

    DateRangeC dr(start, end);
    return dr;
  }

  DateRangeC DateRangeC::Days(RealT days)
  {
    DateC now = DateC::NowUTC();

    // This will get midnight of today
    DateC start(now.Copy());
    start -= (days * 3600. * 24.);

    DateRangeC dr(start, now);
    return dr;
  }

  DateRangeC DateRangeC::Hours(RealT hours)
  {
    DateC now = DateC::NowUTC();

    // This will get midnight of today
    DateC start(now.Copy());
    start -= (3600. * hours);

    DateRangeC dr(start, now);
    return dr;
  }

  DateRangeC DateRangeC::Minutes(RealT minutes)
  {
    DateC now = DateC::NowUTC();

    // This will get midnight of today
    DateC start(now.Copy());
    start -= (minutes * 60.);

    DateRangeC dr(start, now);
    return dr;
  }

  //: Stream operator.
  
  BinOStreamC &operator <<(BinOStreamC &strm, const DateRangeC &date)
  {
    strm << date.From() << date.To();
    return strm;
  }
  
  //: Stream operator.
  
  BinIStreamC &operator >>(BinIStreamC &strm, DateRangeC &date)
  {
    DateC from, to;
    strm >> from >> to;
    date = DateRangeC(from, to);
    return strm;
  }
  
  ostream &operator <<(ostream &out, const DateRangeC &date)
  {
    date.Save(out);
    return out;
  }
  
  //: Stream operator.
  
  istream &operator >>(istream &in, DateRangeC &date)
  {
    date = DateRangeC(in);
    return in;
  }

}
