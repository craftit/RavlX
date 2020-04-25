// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPENTITY_HEADER
#define RAVL_DPENTITY_HEADER 1
////////////////////////////////////////////////////
//! userlevel=Develop
//! example=exDataProc.cc
//! file="Ravl/Core/IO/Entity.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="16/06/1998"
//! docentry="Ravl.API.Core.Data Processing.Internal"

#include "Ravl/RCHandleV.hh"
#include "Ravl/String.hh"
#include "Ravl/SmartPtr.hh"

namespace RavlN {
  class XMLFactoryContextC;
  class DPEntityC;
  
  std::istream &operator>>(std::istream &strm,DPEntityC &obj);
  
  //! userlevel=Develop
  //: Data processing entity body.
  // This is the base class for most of the classes in
  // data processing system.  It main function is to
  // provide a virtual 'save' function. 
  
  class DPEntityBodyC 
    : public RCBodyVC
  {
  public:
    DPEntityBodyC();
    //: Default constructor.
    
    DPEntityBodyC(const DPEntityBodyC &oth);
    //: Copy constructor.

    DPEntityBodyC(const XMLFactoryContextC &factory);
    //: XMLFactory constructor.

    DPEntityBodyC(const StringC &aName);
    //: Construct from a name

    DPEntityBodyC(std::istream &in);
    //: Stream constructor.
    
    DPEntityBodyC(BinIStreamC &in);
    //: Binary stream constructor.
    
    void Setup(const XMLFactoryContextC &factory);
    //: Factory constructor.
    // You must include RavlXMLFactory to use

    virtual bool Save(std::ostream &out) const; 
    //: Save to std::ostream.  
    
    virtual bool Save(BinOStreamC &out) const; 
    //: Save to binary stream.  
    
    virtual RCBodyVC &Copy() const;
    //: Create a copy of this object.

    void SetEntityName(const StringC &aName)
    { m_entityName = aName; }
    //: Set name of entity.

    const StringC &EntityName() const
    { return m_entityName; }
    //: Get name for entity.

    typedef SmartPtrC<DPEntityBodyC> RefT;
    //: Handle to DPEntityC.
  protected:
    StringC m_entityName;
  };
  
  //! userlevel=Advanced
  //: Data processing entity.
  // This is the base class for most of the classes in
  // data processing system.  It main function is to
  // provide a virtual 'save' function. 
  
  class DPEntityC 
    : public RCHandleVC<DPEntityBodyC> 
  {
  public:
    DPEntityC(bool)
    {}
    //: Default constructor.
    // This is explicit to avoid problems with 
    // virtual derived classes using the wrong constructor.
    
    DPEntityC(const DPEntityC &oth) 
      : RCHandleVC<DPEntityBodyC>(oth)
    {}
    //: Copy constructor.
    
    DPEntityC(DPEntityBodyC &nbod) 
      : RCHandleVC<DPEntityBodyC>(nbod)
    {}
    //: Body constructor.

    DPEntityC(const RCAbstractC &abst);
    //: Constructor from an abstract handle.

    DPEntityC(const SmartPtrC<DPEntityBodyC> &ptr)
      : RCHandleVC<DPEntityBodyC>(ptr.BodyPtr())
    {}
    //: Convert from ptr.

    DPEntityC(std::istream &in);
    //: Load from stream using virtual constructor.

    DPEntityC(BinIStreamC &in);
    //: Load from stream using virtual constructor.
    
    inline const DPEntityC &operator= (const DPEntityC &dat) 
    { RCHandleVC<DPEntityBodyC>::operator= (dat); return *this; }
    //: Assignment.
    
    inline DPEntityBodyC& Body() 
    { return RCHandleVC<DPEntityBodyC>::Body(); }
    //: Access body.
    
    inline const DPEntityBodyC& Body() const
    { return RCHandleVC<DPEntityBodyC>::Body(); }
    //: Access body.

    void SetEntityName(const StringC &aName)
    { Body().SetEntityName(aName); }
    //: Set name of entity.

    const StringC &EntityName() const
    { return Body().EntityName(); }
    //: Get name for entity.

  protected:
    DPEntityC(const DPEntityBodyC *nbod) 
      : RCHandleVC<DPEntityBodyC>(nbod)
    {}
    //: Body ptr constructor.
  };
  
  std::istream &operator>>(std::istream &strm,DPEntityC &obj);
  //: Load from a stream.
  // Uses virtual constructor.
  
  std::ostream &operator<<(std::ostream &out,const DPEntityC &obj);
  //: Save to a stream.
  // Uses virtual constructor.
  
  BinIStreamC &operator>>(BinIStreamC &strm,DPEntityC &obj);
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  BinOStreamC &operator<<(BinOStreamC &out,const DPEntityC &obj);
  //: Save to a stream.
  // Uses virtual constructor.
  
}
#endif
