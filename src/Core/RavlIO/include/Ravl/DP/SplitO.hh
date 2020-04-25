// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPSPLITO_HEADER
#define RAVL_DPSPLITO_HEADER 1
/////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Split"
//! lib=RavlIO
//! example=exDPSplit.cc
//! file="Ravl/Core/IO/SplitO.hh"
//! author="Charles Galambos"
//! date="21/07/98"

#include "Ravl/DP/Port.hh"
#include "Ravl/BlkQueue.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/DP/Plug.hh"

namespace RavlN {

  template<class DataT> class DPSplitOC;
  template<class DataT> class DPSplitOBodyC;
  
  ///////////////////////////
  
  //! userlevel=Develop
  //: Output port splitter body
  
  template<class DataT>
  class DPSplitOBodyC 
    : public DPOPortBodyC<DataT>,
      public DPOPlugBaseBodyC
  {
  public:
    DPSplitOBodyC()
    {}
    //: Constructor.

    DPSplitOBodyC(const XMLFactoryContextC &factory)
    { DPOPlugBaseBodyC::Setup(factory); }
    //: Constructor.

    DPSplitOBodyC(const DPOPortC<DataT> &aport)
    { ports.InsLast(aport); }
    //: Construct with an OPortC.
    
    virtual void PutEOS();
    //: Put End Of Stream marker.
    
    virtual bool Put(const DataT &);
    //: Put data.
    
    virtual IntT PutArray(const SArray1dC<DataT> &data);
    //: Put an array of data to stream.
    // returns the number of elements processed.
    
    virtual bool IsPutReady() const;
    //: Is port ready for data ?
    
    virtual bool ConnectPort(const DPOPortBaseC &port) {
      DPOPortC<DataT> oport(port);
      if(!oport.IsValid()) {
        RavlWarning("OPort type doesn't match. ");
        return false;
      }
      return Add(port);
    }
    //: set port.

  protected:
    bool Remove(const DPOPortC<DataT> &port);
    //: Remove output port.
    // This function removes ONE port which is
    // equal to 'port'
    // Returns true if port was found and removed.
    // otherwise it returns false.
    
    bool Add(const DPOPortC<DataT> &port) { 
      ports.InsLast(port);
      return true; 
    }
    //: Add output port.
    // Note no checks are made on how many times a
    // port is added.
    
  private:
    DListC<DPOPortC<DataT> > ports; // List of ports.
    friend class DPSplitOC<DataT>;
  };
  
  
  ////////////////////////
  //! userlevel=Normal
  //: Output port Splitter handle.
  // This will split output and put it to all outputs that
  // have been added by calling Add(...), or using the
  // >> operator.
  
  template<class DataT>
  class DPSplitOC : public DPOPortC<DataT> {
  public:
    DPSplitOC(const DPSplitOC<DataT> &oth);
    //: Copy constructor.
    
    DPSplitOC(bool makeBod)
      : DPEntityC(*new DPSplitOBodyC<DataT>())
    {}
    //: Constructor.
    // Creates a new splitter.

    DPSplitOC(const XMLFactoryContextC &factory)
    : DPEntityC(*new DPSplitOBodyC<DataT>(factory))
    {}
    //: XMLFactory constructor

    DPSplitOC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DPSplitOBodyC<DataT> &Body()
    { return dynamic_cast<DPSplitOBodyC<DataT> &>(DPEntityC::Body()); }
    
    const DPSplitOBodyC<DataT> &Body() const
    { return dynamic_cast<const DPSplitOBodyC<DataT> &>(DPEntityC::Body()); }


  public:
    inline bool Remove(const DPOPortC<DataT> &port)
    { return Body().Remove(port); }
    //: Remove output port.
    // This function removes ONE port which is
    // equal to 'port'
    
    inline bool Add(const DPOPortC<DataT> &port)
    { return Body().Add(port); }
    //: Add output port.
    // Note no checks are made on how many times a
    // port is added.
  };
  
  ///////////////////////
  // Composition.
  
  template<class DataT>
  DPOPortC<DataT> operator>>(DPSplitOC<DataT> &in,const DPOPortC<DataT> &out) {
    in.Add(out);
    return in;
  }
  
  template<class DataT>
  DPOPortC<DataT> operator>>(DPOPortC<DataT> &out,DPSplitOC<DataT> &in) {
    out = in;
    return out; // Is this right ?
  }
  
  template<class DataT>
  void DPSplitOBodyC<DataT>::PutEOS()  {
    for(DLIterC<DPOPortC<DataT> > it(ports);it.IsElm();it.Next()) {
      it.Data().PutEOS();
      it.Del(); // Remove from list.
    }
  }
  
  template<class DataT>
  bool DPSplitOBodyC<DataT>::Put(const DataT &dat) {
    bool oneWorked = false;
    for(DLIterC<DPOPortC<DataT> > it(ports);it.IsElm();it.Next()) {
      if(it.Data().Put(dat))
	oneWorked = true;
    }
    return oneWorked; // Is this really the right thing or should it be all Worked ??
  }
  
  template<class DataT>
  IntT DPSplitOBodyC<DataT>::PutArray(const SArray1dC<DataT> &data) {
    int max = 0;
    for(DLIterC<DPOPortC<DataT> > it(ports);it.IsElm();it.Next()) {
      IntT x = it.Data().PutArray(data);
      if(x > max)
	max = x;
    }
    return max; // Is this right ?
  }
  
  
  template<class DataT>
  bool DPSplitOBodyC<DataT>::IsPutReady() const {
    for(ConstDLIterC<DPOPortC<DataT> > it(ports);it.IsElm();it.Next()) {
      if(it.Data().IsPutReady())
	return true;
    }
    return false;
  }
  
  template<class DataT>
  bool DPSplitOBodyC<DataT>::Remove(const DPOPortC<DataT> &port) {
    for(DLIterC<DPOPortC<DataT> > it(ports);it.IsElm();it.Next()) {
      if(it.Data() == port) {
	it.Del();
	return true;
      }
    }
    return false;
  }
  
}

#endif
