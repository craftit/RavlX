// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DateRange_HEADER
#define RAVL_DateRange_HEADER 1
//////////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Kieron Messer"
//! lib=RavlOS
//! docentry="Ravl.API.OS.Time"
//! rcsid="$Id$"
//! file="Ravl/OS/Time/DateRange.hh"
//! date="23/11/2012"

#include "Ravl/OS/Date.hh"
#include "Ravl/String.hh"

namespace RavlN {
  class BinIStreamC;
  class BinOStreamC;
  
  //: Simple class for storing a date range.
  // SMALL OBJECT.<br>
  
  enum DateRangeT
  {
    DR_TODAY=0, DR_YESTERDAY, DR_LAST_HOUR, DR_LAST_24HOURS, DR_LAST_SEVENDAYS, DR_LAST_THREEANDAHALFDAYS
  };

  class DateRangeC
  {
  public:

    DateRangeC();
    //: Default constructor.
    //: Sets to yesterday
    
    DateRangeC(const DateC & from, const DateC & to);
    //: Construct between two dates.  The 'to' date must be > 'from' date.
    //!param: from - The from date.
    //!param: to - The to date.
    
    DateRangeC(DateRangeT dateRange);
    //: Construct from a pre-defined date range type

    DateRangeC(istream &in);
    //: Construct from a stream
    
    void Save(ostream &out) const;
    //: Write to a stream.
    
    static DateRangeC Today();
    //: Get the date range for today, i.e. from 00:00:00.00000 to 23:59:59.99999.
    //: The actual day date is measured locally
    
    static DateRangeC Yesterday();
    //: Get the date range for yesterday, i.e. from 00:00:00.00000 to 23:59:59.99999.
    //: The actual day date is measured locally
    
    static DateRangeC Days(RealT days);
    //: Get the date range for the last X days to present

    static DateRangeC Hours(RealT hours);
    //: Get the date range for the last X hours to present
    
    static DateRangeC Minutes(RealT minutes);
    //: Get the date range for the last X minutes to present

    const DateC & From() const
    {
      return m_from;
    }
    //: Get the from date
    
    const DateC & To() const
    {
      return m_to;
    }
    //: Get the to date
    
    bool IsValid() const
    {
      return m_to > m_from;
    }
    //: Is a valid date range?

    StringC Text(bool localTime = false, bool usec = false)
    {
      return m_from.ODBC(localTime, usec) + " " + m_to.ODBC(localTime, usec);
    }

  protected:
    DateC m_from;
    DateC m_to;
    
  };
  
  ostream &operator <<(ostream &out, const DateRangeC &date);
  //: Stream operator.
  
  istream &operator >>(istream &in, DateRangeC &date);
  //: Stream operator.
  
  BinOStreamC &operator <<(BinOStreamC &out, const DateRangeC &date);
  //: Stream operator.
  
  BinIStreamC &operator >>(BinIStreamC &in, DateRangeC &date);
//: Stream operator.

}

#endif
