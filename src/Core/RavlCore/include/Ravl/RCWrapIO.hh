// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RCWRAPIO_HEADER
#define RAVL_RCWRAPIO_HEADER 1
//! lib=RavlCore

#include "Ravl/RCWrap.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: RCWrap with full IO, body class
  
  template<class DataT>
  class RCWrapIOBodyC 
    : public RCWrapBodyC<DataT> 
  {
  public:
    RCWrapIOBodyC()
    {}
    //: Default constructor.
    
    explicit RCWrapIOBodyC(const DataT &val)
      : RCWrapBodyC<DataT>(val)
    {}
    //: Constructor.
    
    RCWrapIOBodyC(std::istream &in) 
      : RCWrapBodyC<DataT> (in)      
    { in >> this->data; }
    //: Construct from a stream.
    
    RCWrapIOBodyC(BinIStreamC &in) 
      : RCWrapBodyC<DataT> (in)
    { in >> this->data; }
    //: Construct from a stream.
    
    virtual bool Save(std::ostream &strm) const {
      if(!RCWrapBaseBodyC::Save(strm))
        return false;
      strm << this->data;
      return true;
    }
    //: Save to text stream.
    
    virtual bool Save(BinOStreamC &strm) const {
      if(!RCWrapBaseBodyC::Save(strm))
        return false;
      strm << this->data;
      return true;
    }
    //: Save to binary stream.
    
  };
  
  //! userlevel=Advanced
  //: RCWrapper handle.
  
  template<class DataT>
  class RCWrapIOC
    : public RCWrapC<DataT>
  {
  public:
    RCWrapIOC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    RCWrapIOC(bool makebod,bool){
      if(makebod)
	*this = RCWrapIOC(DataT());
    }
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit RCWrapIOC(const DataT &dat)
      : RCWrapC<DataT>(*new RCWrapIOBodyC<DataT>(dat))
    {}
    //: Construct from an instance.
    // Uses the copy constructor to creat a reference
    // counted copy of 'dat.
    
    RCWrapIOC(const RCWrapAbstractC &val,bool v)
      : RCWrapC<DataT>(dynamic_cast<const RCWrapIOBodyC<DataT> *>(this->BodyPtr(val)))
    {}
    //: Construct from an abstract handle.
    // if the object types do not match, an invalid handle
    // is created.
    
    RCWrapIOC(const RCAbstractC &val)
      : RCWrapC<DataT>(dynamic_cast<const RCWrapIOBodyC<DataT> *>(val.BodyPtr()))
    {}
    //: Construct from an abstract handle.
    
    RCWrapIOC(std::istream &in)
      : RCWrapC<DataT>(*new RCWrapIOBodyC<DataT>(in))
    {}
    //: Construct from a stream.

    RCWrapIOC(BinIStreamC &in)
      : RCWrapC<DataT>(*new RCWrapIOBodyC<DataT>(in))
    {}
    //: Construct from a stream.
    
  protected:
    RCWrapIOC(RCWrapBodyC<DataT> &bod)
      : RCWrapC<DataT>(bod)
    {}
    //: Body constructor.
    
    RCWrapIOBodyC<DataT> &Body()
    { return static_cast<RCWrapBodyC<DataT> &>(RCWrapAbstractC::Body()); }
    //: Body access.
    
    const RCWrapIOBodyC<DataT> &Body() const
    { return static_cast<const RCWrapBodyC<DataT> &>(RCWrapAbstractC::Body()); }
    //: Constant body access.
    
  public:
    RCWrapIOC<DataT> Copy() const 
    { return RCWrapIOC<DataT>(Body().Data()); }
    //: Make a copy of this handle.
    // NB. This assumes the wrapped object is SMALL, and so
    // just using the copy constructor is sufficent.

    RCWrapIOC<DataT> DeepCopy(UIntT levels = ((UIntT) -1)) const {
      switch(levels) {
      case 0: return *this;
      case 1: return Copy();
      case 2: return RCWrapIOC<DataT>(StdCopy(Body().Data()));
      default: break; 
      }
      levels--;
      return RCWrapIOC<DataT>(StdDeepCopy(Body().Data(),levels)); 
    }
    //: Make a copy of this handle.    
  };
  
  template<typename DataT>
  RCWrapAbstractC RCWrapIO(const DataT &val)
  { return RCWrapIOC<DataT>(val); }
  //: Helper function to Wrap a value.

}
#endif
