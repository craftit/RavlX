// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RCHANDLEV_HEADER
#define RAVL_RCHANDLEV_HEADER 1
//! docentry="Ravl.API.Core.Reference Counting"
//! lib=RavlCore
//! userlevel=Normal
//! rcsid="$Id$"
//! author="Charles Galambos"
//! example=exRefCounterInherit.cc
//! file="Ravl/Core/Base/RCHandleV.hh"

#include "Ravl/RCBodyV.hh"
#include "Ravl/Exception.hh"
#if RAVL_HAVE_ANSICPPHEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

#define RAVL_VIRTUALCONSTRUCTOR(strm,targType) RavlN::VCLoad(strm,(targType *) 0)

namespace RavlN {
  class RCAbstractC;
  class BinIStreamC;
  class BinOStreamC;
  template<class BodyT> class RCHandleVC;

  //! Convert a smart pointer to a RCAbstract handle
  template<typename ValueT>
  RCAbstractC ToRCAbstract(const RavlN::RCHandleVC<ValueT> &value);

  //! Convert a smart pointer to from a RCAbstract handle
  template<typename ValueT>
  void FromRCAbstract(const RavlN::RCAbstractC &val,RavlN::RCHandleVC<ValueT> &value);

  
  RCBodyVC *VCLoad(std::istream &s);
  //: Load object from a stream via a virtual constructor
  
  RCBodyVC *VCLoad(BinIStreamC &s);
  //: Load object from a binary stream via a virtual constructor
  
  //! userlevel=Normal
  //: Handle from objects derived from RCBodyVC.
  
  template<class BodyT>
  class RCHandleVC
    : public RCHandleC<BodyT>
  {
  public:
    RCHandleVC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    RCHandleVC(const RCAbstractC &oth);
    //: Copy Constructor.
    // Creates a new reference to 'oth'

    RCHandleVC(std::istream &strm)
      : RCHandleC<BodyT>(dynamic_cast<BodyT *>(VCLoad(strm)))
    {}
    //: Stream constructor.
    
    RCHandleVC(BinIStreamC &strm)
      : RCHandleC<BodyT>(dynamic_cast<BodyT *>(VCLoad(strm)))
    {}
    //: Binary stream constructor.
    
  protected:    
    RCHandleVC(BodyT &bod)
      : RCHandleC<BodyT>(bod)
    {}
    //: Body constructor.
    
    RCHandleVC(const BodyT *bod)
      : RCHandleC<BodyT>(bod)
    {}
    //: Construct from a body pointer.
    // The pointer may be 0.
    
    const BodyT &Body() const 
    { return RCHandleC<BodyT>::Body(); }
    //: Constant access to body of object.
    
    BodyT &Body()
    { return RCHandleC<BodyT>::Body(); }
    //: Constant access to body of object.
    
  public:

    bool Save(std::ostream &out) const
    { return Body().Save(out); }
    //: Save to stream 'out'.
    
    bool Save(BinOStreamC &out) const
    { return Body().Save(out); }
    //: Save to binary stream 'out'.
    
    RCAbstractC Abstract();
    //: Create an abstract handle.    
    
    RCHandleVC<BodyT> Copy() const
    { return RCHandleVC<BodyT>(Body().Copy()); }
    //: Make copy of body.
    
    RCHandleVC<BodyT> DeepCopy(UIntT levels = ((UIntT) -1)) const
    { return RCHandleVC<BodyT>(Body().DeepCopy(levels)); }
    //: Make a deep copy of body.

#if !RAVL_COMPILER_VISUALCPP
    //! Convert a smart pointer to a RCAbstract handle
    friend RCAbstractC ToRCAbstract<>(const RavlN::RCHandleVC<BodyT> &value);

    //! Convert a smart pointer to from a RCAbstract handle
    friend void FromRCAbstract<>(const RavlN::RCAbstractC &val,RavlN::RCHandleVC<BodyT> &value);
#endif
  };
  
  typedef RCHandleC<RCBodyVC> AbstractC;
  //: Abstract object handle.
  // Note: Objects which used abstract handles MUST be derived
  // from RCBodyVC.
  
  template<class BodyT>
  BodyT *VCLoad(std::istream &s,BodyT *) { 
    RCBodyVC *bp = VCLoad(s);
    BodyT *ret = dynamic_cast<BodyT *>(bp);
    if(ret == 0) {
      delete bp;
      throw ExceptionErrorCastC("Virtual constructor failed.",typeid(RCBodyVC),typeid(BodyT));
    }
    return ret; 
  }
  
  template<class BodyT>
  BodyT *VCLoad(BinIStreamC &s,BodyT *) { 
    RCBodyVC *bp = VCLoad(s);
    BodyT *ret = dynamic_cast<BodyT *>(bp);
    if(ret == 0) {
      delete bp; 
      throw ExceptionErrorCastC("Virtual construction failed.",typeid(RCBodyVC),typeid(BodyT));
    }
    return ret; 
  }
  
  template<class BodyT>
  std::ostream &operator<<(std::ostream &strm,const RCHandleVC<BodyT> &obj) {
    RavlAssertMsg(obj.IsValid(),"Attempt to write an invalid object handle.");
    obj.Save(strm);
    return strm;
  }
  //: Write a handle to a stream.
  
  template<class BodyT>
  BinOStreamC &operator<<(BinOStreamC &strm,const RCHandleVC<BodyT> &obj) {
    RavlAssertMsg(obj.IsValid(),"Attempt to write an invalid object handle.");
    obj.Save(strm);
    return strm;
  }
  //: Write binary handle to a stream.
  
  template<class BodyT>
  std::istream &operator>>(std::istream &strm,RCHandleVC<BodyT> &obj) {
    obj = RCHandleVC<BodyT>(strm);
    return strm;
  }
  //: Read a handle from a stream.
  
  template<class BodyT>
  BinIStreamC &operator>>(BinIStreamC &strm,RCHandleVC<BodyT> &obj) {
    obj = RCHandleVC<BodyT>(strm);
    return strm;
  }
  //: Read a handle from a binary stream.
  
};

#endif
