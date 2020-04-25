// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NAMEDLITERAL_HEADER
#define RAVLLOGIC_NAMEDLITERAL_HEADER 1
/////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/NamedLiteral.hh"

#include "Ravl/Logic/Literal.hh"

namespace RavlLogicN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: Named literal body.
  
  class NamedLiteralBodyC 
    : public LiteralBodyC 
  {
  public:
    NamedLiteralBodyC()
    {}
    //: Default constructor.
    
    NamedLiteralBodyC(const StringC &nname)
      : name(nname)
    {}
    //: Constructor.
    
    NamedLiteralBodyC(std::istream &strm);
    //: Construct from a binary stream.
    
    NamedLiteralBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to binary stream 'out'.

    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.

    virtual StringC Name() const;
    //: Get the name of symbol.
    
    virtual UIntT Hash() const;
    //: Get hash value for symbol.
    
    virtual bool IsEqual(const LiteralC &oth) const;
    //: Is this equial to another LiteralC ?
    
  protected:
    StringC name;
  };
  
  //! userlevel=Develop
  //: Named literal.
  // All literals with the same name are considered equivelent. 
  // Care should be taken not to use those names used for
  // unnamed literals, i.e. L:(number) or V:(number) as unpredictable
  // behavour will result.
  
  class NamedLiteralC 
    : public LiteralC 
  {
  public:
    NamedLiteralC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    NamedLiteralC(const StringC &name);
    //: Construct from a string.
    // NB. NamedLiterals with the same name are considered
    // equivelent in every way.
    
    NamedLiteralC(const LiteralC &oth)
      : LiteralC(dynamic_cast<const NamedLiteralBodyC *>(BodyPtr(oth)))
    {}
    //: Base class constructor.
    // if 'oth' isn't a named literal an invalid handle
    // will be created.
    
    NamedLiteralC(std::istream &strm);
    //: Load from stream.
    
    NamedLiteralC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    NamedLiteralC(NamedLiteralBodyC &oth)
      : LiteralC(oth)
    {}
    //: Body constructor.
    
    NamedLiteralC(const NamedLiteralBodyC *oth)
      : LiteralC(oth)
    {}
    //: Body constructor.
    
  };

  inline NamedLiteralC Literal(const char *name)
  { return NamedLiteralC(StringC(name)); }
  //: Create a named symbol.
  
}

#endif
