// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NAMEDVAR_HEADER
#define RAVLLOGIC_NAMEDVAR_HEADER 1
//////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! docentry="Ravl.API.Logic"
//! author="Charles Galambos"
//! file="Ravl/Logic/Base/NamedVar.hh"

#include "Ravl/Logic/Var.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Named literal body.
  
  class NamedVarBodyC 
    : public VarBodyC 
  {
  public:
    NamedVarBodyC()
    {}
    //: Default constructor.
    
    NamedVarBodyC(const StringC &nname)
      : name(nname)
    {}
    //: Constructor.

    NamedVarBodyC(std::istream &strm);
    //: Construct from a stream.
    
    NamedVarBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to stream 'out'.
    
    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    virtual StringC Name() const;
    //: Get the name of symbol.
    
    virtual bool IsVariable() const;
    //: Is this a variable ?
    
    virtual bool IsGrounded() const;
    //: Is this a simple object with no variables ?
    
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
  
  class NamedVarC 
    : public VarC 
  {
  public:
    NamedVarC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    NamedVarC(const StringC &name)
      : VarC(*new NamedVarBodyC(name))
    {}
    //: Constructor.

    NamedVarC(const char *name)
      : VarC(*new NamedVarBodyC(StringC(name)))
    {}
    //: Constructor.
    
    NamedVarC(const LiteralC &oth)
      : VarC(dynamic_cast<const NamedVarBodyC *>(BodyPtr(oth)))
    {}
    //: Base class constructor.
    // if 'oth' isn't a named literal an invalid handle
    // will be created.
    
    NamedVarC(std::istream &strm);
    //: Load from stream.
    
    NamedVarC(BinIStreamC &strm);
    //: Load from binary stream.
    
  };
  
}

#endif
