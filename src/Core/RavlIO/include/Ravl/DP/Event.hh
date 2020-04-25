// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPEVENT_HEADER
#define RAVL_DPEVENT_HEADER 1
////////////////////////////////////////////
//! file="Ravl/Core/IO/Event.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.Events"

#include "Ravl/DP/Entity.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Event body.
  
  class DPEventBodyC 
    : public DPEntityBodyC 
  {
  public:
    inline DPEventBodyC()
    {}
    //: Default Constructor
    
    virtual bool Wait();
    //: Wait for event.
    // Return when event occurres returns true.
    // or if event will never occurre returns false.
    // This will decrement the pending event count by one.
    
    virtual IntT Occurred();
    //: Has event occured ?
    // Returns number of events since last call.
    
    virtual bool Save(std::ostream &out) const;
    //: Write to output.
  };
  
  //! userlevel=Normal
  //: Event body.
  
  class DPEventC 
    : public DPEntityC 
  {
  public:
    DPEventC() 
      : DPEntityC(true)
    {}
    //: Default Constructor
    // Creates an invalid handle
    
    DPEventC(std::istream &frm)
      : DPEntityC(frm)
    {}
    //: Stream constructor.
    
    DPEventC(DPEventBodyC &bod) 
      : DPEntityC(bod)
    {}
    //: Body constructor.
  protected:
    
    DPEventBodyC &Body() 
    { return static_cast<DPEventBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPEventBodyC &Body() const
    { return static_cast<const DPEventBodyC &>(DPEntityC::Body()); }
    //: Access body.
  
  public:
    inline bool Wait()
    { return Body().Wait(); }
    //: Wait for event.
    // Return when event occurres.
    
    inline IntT Occurred()
    { return Body().Occurred(); }
    //: Has event occured ?
    // Returns number of events since last call.
    
  private:
    void Dummy(void);
    //: Dummy function.
  };
  
  inline std::ostream &operator<<(std::ostream &strm,const DPEventC &obj) { 
    obj.Save(strm);
    return strm; 
  }
  
  inline std::istream &operator>>(std::istream &strm,DPEventC &obj) { 
    obj = DPEventC(strm);
    return strm; 
  }
  
}
#endif
