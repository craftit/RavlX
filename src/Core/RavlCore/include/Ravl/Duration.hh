// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2003-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DURATION_HH
#define RAVL_DURATION_HH
//! lib=RavlCore

#include "Ravl/TimeCode.hh"
#include "Ravl/Pair.hh"
#include "Ravl/String.hh"

//! docentry="Ravl.API.Core.Misc"
//! author="Bill Christmas"

//! userlevel=Normal

namespace RavlN {

  //: A time interval
  // Implemented as a pair of timecodes

  class DurationC {

    PairC<TimeCodeC> tc;

  public:
    DurationC()
      : tc(TimeCodeC(0,0), TimeCodeC(0,0))
    {}
    //: Default constructor
    
    DurationC(const TimeCodeC& Start, const TimeCodeC& End)
      : tc(Start, End)
    {}
    //: Constructor from start and end times
    
    DurationC(const TimeCodeC& Time)
      : tc(Time, Time)
    {}
    //: Constructor for item of zero duration
    
    TimeCodeC& Start()
    { return tc.A(); }
    //: Access start time
    
    const TimeCodeC& Start() const
    { return tc.A(); }
    //: Returns start time
    
    TimeCodeC& End() 
    { return tc.B(); }
    //: Access end time
    
    const TimeCodeC& End() const
    { return tc.B(); }
    //: Returns end time
    
    IntT NoOfFrames() const
    { return tc.B().getFrameCount() - tc.A().getFrameCount() + 1; }
    //: Length of interval
    
    bool operator==(const DurationC& d) const
    { return ((Start() == d.Start()) && (End() == d.End())); }
    //: True if start and end of this and <code>d</code> both match
    
    bool operator<(const DurationC& d) const
    { return (End() < d.Start()); }
    //: True if end of this is earlier than start of <code>d</code>
    
    DurationC operator&&(const DurationC& d) const {
      TimeCodeC start = (this->Start() < d.Start()) ? d.Start() : this->Start(); 
      TimeCodeC end = (this->End() > d.End()) ? d.End() : this->End(); 
      return DurationC(start, end);
    }
    //: Returns the intersection of the two time intervals
    
    bool Contains(const TimeCodeC& tc) const
    { return (tc >= this->Start() && tc <= this->End()); }
    //: True if <code>tc</code> is within this
    
    bool Contains(const DurationC& d) const
    { return (d.IsValid() && d.Start() >= this->Start() && d.End() <= this->End()); }
        //: True if <code>d</code> is a subset of this

    StringC ToText() const
    { return Start().ToText() + " ... " + End().ToText(); }
    //: Returns interval in text form

    bool IsValid() const
    { return (Start().IsValid() && (End() >= Start())); }
    //: Check if duration is valid  

  };

  inline std::ostream &operator<<(std::ostream &s, const DurationC &out)
  { s << out.Start() << ' ' << out.End(); return s; }
  //: Write time interval out to stream.

  inline std::istream &operator>>(std::istream &s, DurationC &in)
  { s >> in.Start() >> in.End(); return s; }
  //: Write time interval out to stream.
}
#endif
