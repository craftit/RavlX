// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VIRTUALCONSTRUCTOR_HEADER
#define RAVL_VIRTUALCONSTRUCTOR_HEADER 1
///////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Misc"
//! lib=RavlCore
//! file="Ravl/Core/System/VirtualConstructor.hh"

#include "Ravl/RCHandleV.hh"
#include "Ravl/Hash.hh"
#include "Ravl/TypeName.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <typeinfo>
#else
#include <typeinfo.h>
#endif

namespace RavlN {
  class VirtualConstructorC;
  
  //! userlevel=Develop
  //: Virtual constructor base class.
  
  class VirtualConstructorBodyC
    : public RCBodyVC
  {
  public:
    VirtualConstructorBodyC(const std::type_info &,const char *typesname);
    //: Default constructor.
    
    VirtualConstructorBodyC(std::istream &in);
    //: Construct from an std::istream.
    // Dummy method.

    VirtualConstructorBodyC(BinIStreamC &in);
    //: Construct from an std::istream.
    // Dummy method
    
    virtual RCBodyVC *Load(std::istream &in) const;
    //: Load an object of this type from an std::istream
    
    virtual RCBodyVC *Load(BinIStreamC &in) const;
    //: Load an object of this type from a BinIStreamC    
  };

  //! userlevel=Normal
  //: Virtual constructor base class.
  
  class VirtualConstructorC
    : public RCHandleC<VirtualConstructorBodyC>
  {
  public:
    VirtualConstructorC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    RCBodyVC *Load(std::istream &in) const
    { return Body().Load(in); }
    //: Load an object of this type from an std::istream
    
    RCBodyVC *Load(BinIStreamC &in) const
    { return Body().Load(in); }
    //: Load an object of this type from a BinIStreamC

  protected:
    VirtualConstructorC(VirtualConstructorBodyC &bod)
      : RCHandleC<VirtualConstructorBodyC>(bod)
    {}
    //: Body constructor.
    
    friend class VirtualConstructorBodyC;
  };
  
  //! userlevel=Develop
  //: Virtual constructor instance.
  
  template<class DataT>
  class VirtualConstructorInstBodyC
    : public VirtualConstructorBodyC
  {
  public:
    VirtualConstructorInstBodyC(const char *typesname)
      : VirtualConstructorBodyC(typeid(DataT),typesname)
    {}
    //: Default constructor.
    
    virtual RCBodyVC *Load(std::istream &in) const
    { return new DataT(in); }
    //: Load an object of this type from an std::istream.
    
    virtual RCBodyVC *Load(BinIStreamC &in) const
    { return new DataT(in); }
    //: Load an object of this type from a BinIStreamC using a virtual constructor.
    
  };
  
  //! userlevel=Normal
  //: Virtual constructor instance.

  template<class DataT>
  class VirtualConstructorInstC 
    : public VirtualConstructorC
  {
  public:
    VirtualConstructorInstC(const char *typesname)
      : VirtualConstructorC(*new VirtualConstructorInstBodyC<DataT>(typesname))
    {}
    //: Constructor.
  };
  
  RCBodyVC *VCLoad(std::istream &s);
  //: Load object from a stream via a virtual constructor
  
  RCBodyVC *VCLoad(BinIStreamC &s);
  //: Load object from a binary stream via a virtual constructor
  
  bool CreateVirtualConstructorAlias(const char *newName,const char *oldName);
  //: Create alias for type.
  //: This can be used to allow classes that have been renamed to be loaded for backward compatibility.
  
#define RAVL_INITVIRTUALCONSTRUCTOR(bodyname) \
  static RavlN::VirtualConstructorInstC<bodyname> vc_function_ ## bodyname(# bodyname );

#define RAVL_INITVIRTUALCONSTRUCTOR_NAMED(bodyname,name) \
  static RavlN::VirtualConstructorInstC<bodyname> vc_function_ ## bodyname(name );

  // Just initialise the virtual constructor.

#define RAVL_VIRTUALCONSTRUCTOR_HANDLE(bodyname,handlename,basename) \
 handlename::handlename(std::istream &strm) \
  : basename(RAVL_VIRTUALCONSTRUCTOR(strm,bodyname)) \
 {} \
    handlename::handlename(RavlN::BinIStreamC &strm) \
  : basename(RAVL_VIRTUALCONSTRUCTOR(strm,bodyname)) \
 {}

  // Sort out the constructors for the handle.
  
#define RAVL_INITVIRTUALCONSTRUCTOR_FULL(bodyname,handlename,basename) \
 RAVL_INITVIRTUALCONSTRUCTOR(bodyname) \
 RAVL_VIRTUALCONSTRUCTOR_HANDLE(bodyname,handlename,basename)

  // Initialise the virtual constructor and the handle

#define RAVL_INITVIRTUALCONSTRUCTOR_FULL_NAMED(bodyname,handlename,basename,name) \
 RAVL_INITVIRTUALCONSTRUCTOR_NAMED(bodyname,name) \
 RAVL_VIRTUALCONSTRUCTOR_HANDLE(bodyname,handlename,basename)

  
}

#endif
