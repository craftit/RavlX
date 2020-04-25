// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPIFAILOVER_HEADER
#define RAVL_DPIFAILOVER_HEADER 1
/////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Fail Over"
//! lib=RavlIO
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/Core/IO/IFailOver.hh"
//! date="25/08/1998"

#include "Ravl/DP/Port.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Fail over input switch.
  // This goes through a list of input streams until
  // it finds one that works.
  
  template<class DataT>
  class DPIFailOverBodyC 
    : public DPIPortBodyC<DataT> 
  {
  public:
  DPIFailOverBodyC() 
    {}
    //: Default constructor.
    
    virtual bool IsGetReady() const;
    //: Any data ready ?  
    
    virtual DataT Get();
    //: Get next piece of data.
    // May block if not ready, or it will return a constructed
    // with the default constructor.
    
    virtual bool Get(DataT &buff);
    //: Try and get next piece of data.
    // This will NOT block, if no data is ready
    // it will return false, and not set buff.
    
    void AddInput(const DPIPortC<DataT> &in)
    { ins.InsLast(in); }
    //: Add a new input.
    
  protected:
    DListC<DPIPortC<DataT> > ins; // Input list.
  };
  
  //! userlevel=Normal
  //: Fail over input switch.
  // This goes through a list of input streams until
  // it finds one that works.
  
  template<class DataT>
  class DPIFailOverC :  public DPIPortC<DataT> {
  public:
    DPIFailOverC()
      : DPEntityC(*new DPIFailOverBodyC<DataT>())
    {}
    //: Default constructor.
    
    inline  void AddInput(const DPIPortC<DataT> &in)
    { dynamic_cast<DPIFailOverBodyC<DataT> & >(DPEntityC::Body()).AddInput(in); }
    //: Add a new input.
  };
  
  template<class DataT>
  DPIPortC<DataT> operator>>(const DPIPortC<DataT> &from,DPIFailOverC<DataT> &to) {
    to.AddInput(from);
    return to;
  }
  
  template<class DataT>
  bool DPIFailOverBodyC<DataT>::IsGetReady() const  {
    for(DLIterC<DPIPortC<DataT> > it(const_cast<DListC<DPIPortC<DataT> > &>(ins));it.IsElm();it.Next()) {
      if(it.Data().IsGetReady())
	return true;
    }
    return false;
  }
  
  template<class DataT>
  DataT DPIFailOverBodyC<DataT>::Get() {
    for(DLIterC<DPIPortC<DataT> > it(ins);it.IsElm();it.Next()) {
      try {
	if(it.Data().IsGetReady())
	  return it.Data().Get();
      } catch(DataNotReadyC &) {
	// Hmmm, try the next one.
	continue;
      }
    }
    throw DataNotReadyC("DPIFailOverBodyC(), All fail overs not ready. "); 
  }
  
  template<class DataT>
  bool DPIFailOverBodyC<DataT>::Get(DataT &buff) {
    for(DLIterC<DPIPortC<DataT> > it(ins);it.IsElm();it.Next()) {
      try {
	if(!it.Data().Get(buff))
	  continue;
	return true;
      } catch(DataNotReadyC &) {
	// Hmmm, try the next one.
	continue;
      }
    }
    return false; // Nothing worked...
  }
  
}
#endif
