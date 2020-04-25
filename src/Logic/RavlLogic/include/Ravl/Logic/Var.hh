// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_VAR_HEADER
#define RAVLLOGIC_VAR_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic"
//! lib=RavlLogic
//! author="Charles Galambos"
//! file="Ravl/Logic/Base/Var.hh"

#include "Ravl/Logic/Literal.hh"

namespace RavlLogicN {
  using namespace RavlN;

  //! userlevel=Develop
  //: Variable body.
  
  class VarBodyC 
    : public LiteralBodyC
  {
  public:
    VarBodyC()
    {}
    //: Default constructor.

    VarBodyC(std::istream &strm);
    //: Construct from a stream.
    
    VarBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to stream 'out'.
    
    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    virtual bool IsVariable() const;
    //: Is this a variable ?
    
    virtual bool IsGrounded() const;
    //: Is this a simple object with no variables ?

    virtual void Dump(std::ostream &out);
    //: Dump info in human readable format to stream 'out'.

    virtual StringC Name() const;
    //: Get the name of symbol.

    virtual bool Substitute(const BindSetC &binds,LiteralC &result) const;
    //: Substitute variables in 'binds' for their bound values.
    // This builds a new literal with the substute values (if there
    // are any). The new value is assigned to 'result' <p>
    // Returns true if at least one substitution has been made,
    // false if none.
    
    virtual bool ReplaceVars(HashC<LiteralC,LiteralC> &subs,LiteralC &result) const;
    //: Replace all vars in this literal with new ones.
    // The mapping between the replacements and the new values is returned in 'subs'
    // If no new replacements where found, false is returned.

  protected:    
    virtual bool Unify(const LiteralC &oth,BindSetC &bs) const;
    //: Unify with another variable.

    virtual bool UnifyLiteral(const LiteralBodyC &oth,BindSetC &bs) const;
    //: Unify 
  };
  
  //! userlevel=Normal
  //: Variable
  
  class VarC 
    : public LiteralC 
  {
  public:
    VarC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit VarC(bool)
      : LiteralC(*new VarBodyC())
    {}
    //: Constructor.
    // Construct an anonymous var.
    
    VarC(std::istream &strm);
    //: Load from stream.

    VarC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    VarC(VarBodyC &bod)
      : LiteralC(bod)
    {}
    //: Body constructor.

    VarC(const VarBodyC *bod)
      : LiteralC(bod)
    {}
    //: Body ptr constructor.
    
    friend class VarBodyC;
  };
  
  inline VarC Var()
  { return VarC(true); }
  //: Creat an anonymous variable.
}

#endif
