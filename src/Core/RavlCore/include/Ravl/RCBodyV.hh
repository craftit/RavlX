// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RCBODYV_HEADER
#define RAVL_RCBODYV_HEADER 1
//! docentry="Ravl.API.Core.Reference Counting"
//! lib=RavlCore
//! userlevel=Normal
//! rcsid="$Id$"
//! author="Charles Galambos"
//! example=exRefCounterInherit.cc
//! file="Ravl/Core/Base/RCBodyV.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/Exception.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Base class for all reference counted objects, where derivation is expected.
  // This holds a count of the number of handles that
  // are available for this object.
  
  class RCBodyVC 
    : public RCBodyC
  {
  public:
    virtual ~RCBodyVC();
    //: Destructor.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to stream 'out'.
    
    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    virtual RCBodyVC &Copy() const;
    //: Make copy of body.
    // This should be provided in derived classes.
    // this function will issue an assertion failure if called.

    virtual RCBodyC &DeepCopy(UIntT levels = ((UIntT) -1)) const;
    //: Make a deep copy of body.
    // This should be provided in derived classes.
    // this function will issue an assertion failure if called.

    RCBodyVC()
    {}
    //: Default constructor.
    // Creates a handle with 0 reference counts.
    
  protected:

    RCBodyVC(std::istream &strm)
    {}
    //: Construct from a stream.
    
    RCBodyVC(BinIStreamC &strm)
    {}
    //: Construct from a binary stream.
    
    friend class RCAbstractC ;
  };
  
  std::istream &operator>>(std::istream &,RCBodyVC &obj);
  //: Input virtual body.
  // No-op.

  std::ostream &operator<<(std::ostream &,const RCBodyC &obj);
  //: Output virtual body.
  // No-op.
  
  BinIStreamC &operator>>(BinIStreamC &strm,RCBodyVC &obj);
  //: Input virtual body.
  // No-op.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const RCBodyVC &obj);
  //: Output virtual body.
  // No-op.
  
  
};

#endif
