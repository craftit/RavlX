// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RCABSTRACT_HEADER
#define RAVL_RCABSTRACT_HEADER 1
//////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Reference Counting"
//! file="Ravl/Core/Base/RCAbstract.hh"
//! lib=RavlCore
//! rcsid="$Id$"
//! date="28/05/1998"

#include "Ravl/RCHandleV.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <typeinfo>
#else
#include <typeinfo.h>
#endif 

namespace RavlN {
  //: Abstract handle class for any object derived from 
  //: RCBodyVC.
  // See RCWrapC<> for example.
  
  class RCAbstractC 
    : public RCHandleVC<RCBodyVC>
  {
  public:  
    RCAbstractC() {}
    //: Default constructor.
    
    RCAbstractC(const RCAbstractC &oth)
      : RCHandleVC<RCBodyVC>(oth)
    {}
    //: Copy constructor.
    
    RCAbstractC(const RCBodyVC &oth)
      : RCHandleVC<RCBodyVC>(&oth)
    {}
    //: Constructor.

    RCAbstractC(const RCBodyVC *oth)
      : RCHandleVC<RCBodyVC>(oth)
    {}
    //: Constructor.

    RCAbstractC(std::istream &in);
    //: Constructor.

    RCAbstractC(BinIStreamC &in);
    //: Constructor.

    inline RCAbstractC Copy() const;
    //: Copy object contents.
    // FIXME :- Use RTTI to check copy is full.

    const std::type_info &BodyType() const
    {
      const RCBodyVC &body = Body();
      return typeid(body); // Use via reference to avoid warnings from clang.
    }
    //: Type of object held.
    
    void Dump(std::ostream &out) const;
    //: Dump info about handle

    RCBodyVC &Body()
    { return RCHandleC<RCBodyVC>::Body(); }
    //: Access body.

    const RCBodyVC &Body() const
    { return RCHandleC<RCBodyVC>::Body(); }
    //: Access body.

    RCBodyVC *BodyPtr()
    { return RCHandleC<RCBodyVC>::BodyPtr(); }
    //: Access body.

    const RCBodyVC *BodyPtr() const
    { return RCHandleC<RCBodyVC>::BodyPtr(); }
    //: Access body.
    
  };
  
  //////////////////////////////////////////////////////
  
  inline 
  RCAbstractC 
  RCAbstractC::Copy() const  {
    if(!IsValid()) // Check it a valid handle !
      return RCAbstractC();
    return RCAbstractC(Body().Copy()); 
  }

  //! Convert a smart pointer to a RCAbstract handle
  inline RCAbstractC ToRCAbstract(const RavlN::RCAbstractC &value)
  { return value; }

  //! Convert a smart pointer to from a RCAbstract handle
  inline void FromRCAbstract(const RavlN::RCAbstractC &val,RavlN::RCAbstractC &value)
  { value = val; }

  //////////////////////////////////////

  template<class BodyT>
  RCHandleVC<BodyT>::RCHandleVC(const RCAbstractC &oth)
    : RCHandleC<BodyT>(oth.IsValid() ? dynamic_cast<BodyT *>(const_cast<RCBodyVC *> (&oth.Body())) : (BodyT *) 0)
  {}
  //: Copy Constructor.
  // Creates a new reference to 'oth'

  template<class BodyT>
  RCAbstractC RCHandleVC<BodyT>::Abstract()
  { return RCAbstractC(Body()); }
  //: Create an abstract handle.

  //! Convert a smart pointer to a RCAbstract handle
  template<typename ValueT>
  inline RCAbstractC ToRCAbstract(const RavlN::RCHandleVC<ValueT> &value) {
    return RCAbstractC(value.Body());
  }

  //! Convert a smart pointer to from a RCAbstract handle
  template<typename ValueT>
  inline void FromRCAbstract(const RavlN::RCAbstractC &val,RavlN::RCHandleVC<ValueT> &value) {
    if(!val.IsValid()) {
      value.Invalidate();
      return ;
    }
    value = RavlN::RCHandleVC<ValueT>(dynamic_cast<const ValueT &>(val.Body()));
  }


}
#endif
