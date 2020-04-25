// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPPORT_HEADER
#define RAVL_DPPORT_HEADER 1
////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Ports" 
//! file="Ravl/Core/IO/Port.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="16/06/1998"
//! userlevel=Normal

#include "Ravl/DP/Entity.hh"
#include "Ravl/DP/AttributeCtrl.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Exception.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Stream.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <typeinfo>
#else
#include <typeinfo.h>
#endif 

namespace RavlN {
  
  class DPPortC;
  class StringC;
  
  //! userlevel=Normal
  //: Exception, Data Not Ready.
  // This exception is thrown if a DPIPortC::Get is unable to
  // complete because there is no data available.
  
  class DataNotReadyC 
    : public ExceptionC 
  {
  public:
    DataNotReadyC(const char *msg = "")
      : ExceptionC(msg)
    {}
  };
  
  //! userlevel=Develop
  //: Abstract port body.
  
  class DPPortBodyC 
    : public AttributeCtrlBodyC 
  {
  public:
    DPPortBodyC();
    //: Default constructor.
    
    DPPortBodyC(const StringC &nportId)
      : portId(nportId)
    {}
    //: Constructor with a port id.
    
    DPPortBodyC(std::istream &in);
    //: Stream constructor.
    
    virtual ~DPPortBodyC();
    //: Destructor.
    
    void RegisterID(void) ;
    //: Register Port ID  

    virtual bool IsAsync() const;
    //: Does port work asynchronously ?
    // Can more than one thread read from this port safely ?
    
    virtual bool Save(std::ostream &out) const;
    //: Save to std::ostream.
    
    virtual DPPortC ConnectedTo() const;
    //: Is this port connected to another ?
    // If not returns invalid handle.
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,IntT &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const IntT &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,RealT &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const RealT &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,bool &attrValue);
    //: Get a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const bool &attrValue);
    //: Set a stream attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling stream attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttrList(DListC<StringC> &list) const;
    //: Get list of attributes available.
    // This method will ADD all available attribute names to 'list'.
    
    virtual bool GetAttrTypes(DListC<AttributeTypeC> &list) const;
    //: Get a list of available attribute types.
    
    virtual AttributeTypeC GetAttrType(const StringC &attrName) const;
    //: Get type of a particular attribute. 
    // Returns an invalid handle if attribute is unknown.
    
  protected:
    virtual AttributeCtrlC ParentCtrl() const;
    //: Get Parent attribute control.
    
    StringC portId; // Port ID, this can be accessed as the attribute 'id', defaults to empty string.
  };
  
  //! userlevel=Develop
  //: Input port base body.
  
  class DPIPortBaseBodyC 
    : public DPPortBodyC 
  {
  public:
    DPIPortBaseBodyC() 
    {}
    //: Default constructor.
    
    DPIPortBaseBodyC(const StringC &nportId)
      : DPPortBodyC(nportId)
    {}
    //: Constructor with a port id.
    
    DPIPortBaseBodyC(std::istream &in) 
      : DPPortBodyC(in)
    {}
    // Stream constructor.
    
    virtual bool IsGetReady() const;
    //: Is some data ready ?
    // true = yes.
    // Defaults to !IsGetEOS().
    
    virtual bool IsGetEOS() const;
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual const std::type_info &InputType() const;
    //: Input type.
    
    virtual bool Save(std::ostream &out) const 
    { return DPPortBodyC::Save(out); }
    //: Save to std::ostream.
    
    virtual bool Discard();
    //: Discard the next input datum.
  };
  
  //! userlevel=Develop
  //: Input port body.
  
  template<class DataT>
  class DPIPortBodyC 
    : public DPIPortBaseBodyC 
  {
  public:
    DPIPortBodyC() 
    {}
    //: Default constructor.

    DPIPortBodyC(const StringC &nportId)
      : DPIPortBaseBodyC(nportId)
    {}
    //: Constructor with a port id.
    
    DPIPortBodyC(std::istream &in) 
      : DPIPortBaseBodyC(in)
    {}
    //: Stream constructor.
    
    virtual DataT Get()  {
      cerr << "DPIPortBodyC<DataT>::Get(), ERROR: Abstract method called. \n";
      RavlAssert(0);
      return DataT();
    }
    //: Get next piece of data.
    // May block if not ready, or it could throw an 
    // DataNotReadyC exception.
    // NB. This function MUST be provided by client class.
    
    virtual bool Get(DataT &buff) { 
      try {
	buff = Get();
      } catch(DataNotReadyC &) {
	return false;
      }
      return true;
    }
    //: Try and get next piece of data.
    // This may not NOT block, if no data is ready
    // it will return false, and not set buff.
    // NB. The default version of this function uses
    // the Get() method defined above and so need 
    // not be provided by derived classes.
    
    virtual IntT GetArray(SArray1dC<DataT> &data);
    //: Get an array of data from stream.
    // returns the number of elements successfully processed.
    // NB. This need NOT be overridden in client classes 
    // unless fast handling of arrays of data elements is required.
    
    virtual const std::type_info &InputType() const { return typeid(DataT); }
    // Input type.  
    
    virtual bool Save(std::ostream &out) const 
    { return DPIPortBaseBodyC::Save(out); }
    //: Save to std::ostream.
    
    virtual bool Discard() { 
      DataT tmp;
      return Get(tmp);
    }
    //: Discard the next input datum.
    
  };
  
  template<class DataT>
  IntT DPIPortBodyC<DataT>::GetArray(SArray1dC<DataT> &data) {
    for(SArray1dIterC<DataT> it(data);it;it++) {
      if(!Get(*it))
	return it.Index().V();
    }
    return data.Size();
  }
  
  //! userlevel=Develop
  //: Output port base body.
  
  class DPOPortBaseBodyC 
    : public DPPortBodyC 
  {
  public:
    DPOPortBaseBodyC()
    {}
    //: Default constructor.

    DPOPortBaseBodyC(const StringC &nportId)
      : DPPortBodyC(nportId)
    {}
    //: Constructor with a port id.
    
    DPOPortBaseBodyC(std::istream &in)
      : DPPortBodyC(in)
    {}
    //: Stream constructor.
    
    virtual void PutEOS();
    //: Put End Of Stream marker.
    
    virtual bool IsPutReady() const;
    //: Is port ready for data ?
    
    virtual const std::type_info &OutputType() const;
    //: Output type.
    
    virtual bool Save(std::ostream &out) const 
    { return DPPortBodyC::Save(out); }
    //: Save to std::ostream.
  };
  
  //! userlevel=Develop
  //: Output port body.
  
  template<class DataT>
  class DPOPortBodyC 
    : public DPOPortBaseBodyC 
  {
  public:
    DPOPortBodyC() 
    {}
    //: Default constructor.
    
    DPOPortBodyC(const StringC &nportId)
      : DPOPortBaseBodyC(nportId)
    {}
    //: Constructor with a port id.
    
    DPOPortBodyC(std::istream &in)
      : DPOPortBaseBodyC(in)
    {}
    //: Default constructor.
    
    virtual bool Put(const DataT &) { 
      cerr << "DPOPortBodyC<DataT>::Put(), ERROR: Abstract method called. \n";
      RavlAssert(0);
      return false; 
    }
    //: Put data.
    // This function MUST be provided by client class.
    
    virtual IntT PutArray(const SArray1dC<DataT> &data);
    //: Put an array of data to stream.
    // returns number of elements processed.
    // NB. This need NOT be overridden in client classes 
    // unless fast handling of arrays of data elements is required.
    
    virtual const std::type_info &OutputType() const { return typeid(DataT); }
    //: Input type.
    
    virtual bool Save(std::ostream &out) const 
    { return DPOPortBaseBodyC::Save(out); }
    //: Save to std::ostream.
  };
  
  template<class DataT>
  IntT DPOPortBodyC<DataT>::PutArray(const SArray1dC<DataT> &data) {
    for(SArray1dIterC<DataT> it(data);it;it++) {
      if(!Put(*it))
	return it.Index().V();
    }
    return data.Size();
  }
    
  template<class DataT> class DPPlugC;
  
  ///////////////////////////
  //! userlevel=Develop
  //: Base port handle.
  
  class DPPortC 
    : public AttributeCtrlC 
  {
  public:
    DPPortC() 
      : DPEntityC(false)
    {}
    //: Default constructor.
    
    DPPortC(DPPortBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Constructor.
    
    DPPortC(std::istream &in)
      : DPEntityC(in)
    {}
    //: Stream constructor.
    
    DPPortC(const DPPortC &oth)
      : DPEntityC(oth),
        AttributeCtrlC()
    {}
    //: Copy constructor.
    
  protected:    
    inline DPPortBodyC &Body() 
    { return dynamic_cast<DPPortBodyC &> (DPEntityC::Body()); }
    //: Access body.
    
    inline const DPPortBodyC &Body() const
    { return dynamic_cast<const DPPortBodyC &> (DPEntityC::Body()); }
    //: Access body.
    
  public:
    
    inline bool IsAsync() const 
    { return Body().IsAsync(); }
    //: Does port work asynchronously ?
    // Can more than one thread read from this port safely ?
    
    inline DPPortC ConnectedTo() const
    { return Body().ConnectedTo(); }
    //: Is this port connected to another ?
    // If not returns invalid handle.
    
  };
  
  //////////////////////////
  //! userlevel=Develop
  //: Input port base class.
  
  class DPIPortBaseC 
    : public DPPortC 
  {
  public:
    DPIPortBaseC() 
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    DPIPortBaseC(const DPIPortBaseC &oth) 
      : DPEntityC(oth),
        DPPortC(oth)
    {}
    //: Copy constructor.
    
    DPIPortBaseC(DPIPortBaseBodyC &bod) 
      : DPEntityC(bod),
        DPPortC(bod)
    {}
    //: Body constructor.
    
    DPIPortBaseC(std::istream &strm) 
      : DPEntityC(strm)
    {}
    //: Stream constructor.
    
    DPIPortBaseC(const DPEntityC &bod)
      : DPEntityC(dynamic_cast<const DPIPortBaseBodyC *>(BodyPtr(bod)))
    {}
    //: Base class constructor.
    // Will create an invalid handle if types don't match.

  protected:
    inline DPIPortBaseBodyC &Body() 
    { return dynamic_cast<DPIPortBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    inline const DPIPortBaseBodyC &Body() const
    { return dynamic_cast<const DPIPortBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    inline const std::type_info &InputType() const 
    { return Body().InputType(); }
    // Get type of input port.
    
    inline bool IsGetReady() const 
    { return Body().IsGetReady(); }
    //: Is some data ready ?
    // true = yes.
    // Defaults to !IsGetEOS().
    
   
    inline bool IsGetEOS() const 
    { return Body().IsGetEOS(); }
    //: Has the End Of Stream been reached ? 
    // true = yes 
  
    inline bool Discard()
    { return Body().Discard(); }
    //: Discard the next input datum.
    // returns true on success.
  };
  
  /////////////////////////////////
  //! userlevel=Normal
  //: Input port.
  // Note that, after its creation, the port must be connected to a 
  // source, e.g. by using <a href=
  // "RavlN.OpenISequenceObDPIPortCLtDataTGt_Amp_const_StringC_Amp_const_StringC_Amp_boolCb.html">
  // OpenISequence</a>, before applying most of the methods.
  
  template<class DataT>
  class DPIPortC 
    : public DPIPortBaseC 
  {
  public:
    DPIPortC() 
      : DPEntityC(true)
    {}
    //: Default constructor.
    
#ifdef __sgi__
    DPIPortC(const DPIPortC<DataT> &oth) 
      : DPEntityC(oth),
	DPIPortBaseC(oth)
    {}
    //: Copy constructor.
#endif
    
    DPIPortC(DPIPortBodyC<DataT> &bod) 
      : DPEntityC(bod),
	DPIPortBaseC(bod)
    {}
    //: Body constructor.
    
    DPIPortC(const DPIPortBaseC &oth) 
      : DPEntityC(oth)
    { 
#if RAVL_CHECK
      if(IsValid()) {
	if(oth.InputType() != typeid(DataT)) {
	  cerr << "DPIPortC<DataT>() Type mismatch.  " << oth.InputType().name() << " given to " << typeid(DataT).name() << endl; 
	  RavlAssert(0);
	}
      }
#endif
    }
    //: Base constructor.
    
    DPIPortC(std::istream &in) 
      : DPEntityC(in)
    {}
    //: Stream constructor.
    
  protected:
    DPIPortBodyC<DataT> &Body() 
    { return dynamic_cast<DPIPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIPortBodyC<DataT> &Body() const
    { return dynamic_cast<const DPIPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
    inline DataT Get() 
    { return Body().Get(); }
    // Get next piece of data.
    
    inline bool Get(DataT &buff) 
    { return Body().Get(buff); }
    //: Try and get next piece of data.
    // If none, return false.
    // else put data into buff.  
    
    inline IntT GetArray(SArray1dC<DataT> &data)
    { return Body().GetArray(data); }
    //: Get an array of data from stream.
    // returns the number of elements  processed.
    
    friend class DPPlugC<DataT>;
  };
  
  template <class DataT>
  std::ostream & operator<<(std::ostream & s,const DPIPortC<DataT> &port) { 
    port.Save(s); 
    return s; 
  }
  
  template <class DataT>
  std::istream & operator>>(std::istream & s, DPIPortC<DataT> &port) { 
    DPIPortC<DataT> nport(s); port = nport; 
    return s; 
  }
  
  ////////////////////////////////
  //! userlevel=Develop
  //: Output port base.
  
  class DPOPortBaseC 
    : public DPPortC 
  {
  public:
    DPOPortBaseC() 
      : DPEntityC(true)
    {}
    //: Default constructor.
    
    DPOPortBaseC(const DPOPortBaseC &oth) 
      : DPEntityC(oth),
	DPPortC(oth)
    {}
    // Copy constructor.
    
    DPOPortBaseC(DPOPortBaseBodyC &bod) 
      : DPEntityC(bod),
	DPPortC(bod)
    {}
    // Body constructor.
    
    DPOPortBaseC(std::istream &strm) 
      : DPEntityC(strm)
    {}
    // Stream constructor.
    
    DPOPortBaseC(const DPEntityC &bod)
      : DPEntityC(dynamic_cast<const DPOPortBaseBodyC *>(BodyPtr(bod)))
    {}
    //: Base class constructor.
    // Will create an invalid handle if types don't match.

  protected:
    inline DPOPortBaseBodyC &Body() 
    { return dynamic_cast<DPOPortBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    inline const DPOPortBaseBodyC &Body() const
    { return dynamic_cast<const DPOPortBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    inline void PutEOS() 
    { Body().PutEOS(); }
    //: Put End Of Stream marker.
    
    inline const std::type_info &OutputType() const 
    { return Body().OutputType(); }
    //: Get type of output port.
    
    inline bool IsPutReady() const 
    { return Body().IsPutReady(); }
    // Is port ready for data ?  
  };
  
  //////////////////////////////
  //! userlevel=Normal
  //: Output port.
  // Note that, after its creation, the port must be connected to a 
  // destination, e.g. by using <a href=
  // "RavlN.OpenOSequenceObDPOPortCLtDataTGt_Amp_const_StringC_Amp_const_StringC_Amp_boolCb.html">
  // OpenOSequence</a>, before applying most of the methods.
  
  template<class DataT>
  class DPOPortC 
    : public DPOPortBaseC 
  {
  public:
    DPOPortC() 
      : DPEntityC(true)
    {}
    // Default constructor.

    DPOPortC(DPOPortBodyC<DataT> &bod) 
      : DPEntityC(bod),
	DPOPortBaseC(bod)
    {}
    //: Body constructor.
    
    DPOPortC(std::istream &in) 
      : DPEntityC(in)
    {}
    //: Stream constructor.
    
    DPOPortC(const DPOPortBaseC &oth) 
      : DPEntityC(oth)
    { 
#if RAVL_CHECK
      if(IsValid()) {
	if(oth.OutputType() != typeid(DataT)) {
	  cerr << "DPOPortC<DataT>() Type mismatch.  " << oth.OutputType().name() << " given to " << typeid(DataT).name() << endl; 
	  RavlAssert(0);
	}
      }
#endif
    }
    //: Base constructor.
    
  protected:
    DPOPortBodyC<DataT> &Body() 
    { return dynamic_cast<DPOPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPOPortBodyC<DataT> &Body() const
    { return dynamic_cast<const DPOPortBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    inline bool Put(const DataT &dat) 
    { return Body().Put(dat); }
    //: Put data to stream
    // Returns true if data is put into stream succesfully.
    
    inline IntT PutArray(const SArray1dC<DataT> &data)
    { return Body().PutArray(data); }
    //: Put an array of data elements into a stream.
    // returns the number of elements processed from the array.
    
    inline bool IsPutReady() const 
    { return Body().IsPutReady(); }
    // Is port ready for data ?  
  };
  
  template <class DataT>
  std::ostream & operator<<(std::ostream & s,const DPOPortC<DataT> &port) { 
    port.Save(s); 
    return s; 
  }
  
  template <class DataT>
  std::istream & operator>>(std::istream & s, DPOPortC<DataT> &port) { 
    DPOPortC<DataT> nport(s); port = nport; 
    return s; 
  }
  
}
#endif
