// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERAL_HEADER
#define RAVLLOGIC_LITERAL_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic"
//! userlevel=Normal
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Literal.hh"
//! author="Charles Galambos"

#include "Ravl/RCHandleV.hh"
#include "Ravl/String.hh"
#include "Ravl/HSet.hh"

namespace RavlLogicN {
  using namespace RavlN;
  class StateC;
  
  class VarC;
  class VarBodyC;
  class BindSetC;
  class LiteralC;
  class LiteralIterC;
  class XMLIStreamC;
  class XMLOStreamC;
  
  //! userlevel=Develop
  //: Literal body
  
  class LiteralBodyC 
    : public RCBodyVC
  {
  public:
    LiteralBodyC()
    {}
    //: Default constructor.
    
    LiteralBodyC(std::istream &strm);
    //: Construct from a stream.
    
    LiteralBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to stream 'out'.
    
    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    virtual bool IsVariable() const;
    //: Is this a variable ?

    virtual bool IsGrounded() const;
    //: Is this a simple expression with no variables ?
    
    virtual bool IsTuple() const;
    //: Is this literal a tuple ?
    
    virtual bool IsCondition() const;
    //: Test if this is a condition.
    
    virtual bool Unify(const LiteralC &oth,BindSetC &bs) const;
    //: Unify with another variable.
    
    virtual void Dump(std::ostream &out);
    //: Dump info in human readable format to stream 'out'.

    virtual StringC Name() const;
    //: Get the name of symbol.
    
    virtual UIntT Hash() const { 
#if RAVL_CPUTYPE_64
      return ((UInt64T) this) >> 2; 
#else
      return ((UIntT) this) >> 2; 
#endif
    }
    //: Get hash value for symbol.
    
    virtual bool IsEqual(const LiteralC &oth) const;
    //: Is this equial to another LiteralC ?
    
    virtual bool Test(const StateC &state,BindSetC &binds) const;
    //: Test if condition is true in 'state'.
    
    virtual LiteralIterC Solutions(const StateC &state,BindSetC &binds) const;
    //: Return an iterator through possibile matches to this literal in 'state', if any.
    
    virtual void SubLiterals(HSetC<LiteralC> &lits) const;
    //: Get a set of all sub literals.
    
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
    
    virtual void ListConditionTerms(HSetC<LiteralC> &posTerms,HSetC<LiteralC> &negTerms) const;
    //: Generate a set of positive and negative terms used in the condition.
    
  protected:
    virtual bool UnifyLiteral(const LiteralBodyC &oth,BindSetC &bs) const;
    //: Unify with simple literal.
    
    friend class LiteralC;
  };

  //! userlevel=Normal
  //: Literalol
  
  class LiteralC 
    : public RCHandleVC<LiteralBodyC>
  {
  public:
    LiteralC()
    {}
    //: Default constructor.
    
    explicit LiteralC(bool)
      : RCHandleVC<LiteralBodyC>(*new LiteralBodyC())
    {}
    //: Constructor.
    
    LiteralC(const char *name);
    //: Create a named literal.
    
    explicit LiteralC(std::istream &strm);
    //: Load from stream.
    
    explicit LiteralC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    LiteralC(LiteralBodyC &bod)
      : RCHandleVC<LiteralBodyC>(bod)
    {}
    //: Body constructor.

    LiteralC(const LiteralBodyC *bod)
      : RCHandleVC<LiteralBodyC>(bod)
    {}
    //: Body constructor.
    
  public:
    LiteralBodyC &Body()
    { return RCHandleVC<LiteralBodyC>::Body(); }
    //: Access body.
    
    const LiteralBodyC &Body() const
    { return RCHandleVC<LiteralBodyC>::Body(); }
    //: Access body.
    
    StringC Name() const { 
      if(!IsValid())
	return StringC("-NIL-");
      return Body().Name(); 
    }
    //: Get the name of symbol.
    
    bool IsVariable() const
    { return Body().IsVariable(); }
    //: Is this a variable ?

    bool IsGrounded() const
    { return Body().IsGrounded(); }
    //: Is this a simple expression with no variables ?
    
    bool IsTuple() const
    { return Body().IsTuple(); }
    //: Is this literal a tuple ?
    
    bool IsCondition() const
    { return Body().IsCondition(); }
    //: Test if this is a condition.
    
    bool Unify(const LiteralC &val,BindSetC &bs) const
    { return Body().Unify(val,bs); }
    //: Unify this and 'val'.
    
    UIntT Hash() const 
    { return Body().Hash(); }
    //: Get hash value.
    
    bool IsEqual(const LiteralC &oth) const
    { return Body().IsEqual(oth); }
    //: Is this equial to another LiteralC ?
    
    bool operator==(const LiteralC &oth) const 
    { return Body().IsEqual(oth); }
    //: Equality test.

    bool operator!=(const LiteralC &oth) const 
    { return !Body().IsEqual(oth); }
    //: Inequality test.
    
    void Dump(std::ostream &out)
    { Body().Dump(out); }
    //: Dump info in human readable format to stream 'out'.

    bool Test(const StateC &state,BindSetC &binds) const
    { return Body().Test(state,binds); }
    //: Test if condition is true in 'state'.
    
    LiteralIterC Solutions(const StateC &state,BindSetC &binds) const;
    //: Return iterator through possibile matches to this literal in 'state', if any.
    
    void SubLiterals(HSetC<LiteralC> &lits) const
    {  Body().SubLiterals(lits); }
    //: Get a set of all sub literals.
    
    bool Substitute(const BindSetC &binds,LiteralC &result) const
    { return Body().Substitute(binds,result); }
    //: Substitute variables in 'binds' for their bound values.
    // This builds a new literal with the substute values (if there
    // are any). The new value is assigned to 'result' <p>
    // Returns true if at least one substitution has been made,
    // false if none.
    
    bool ReplaceVars(HashC<LiteralC,LiteralC> &subs,LiteralC &result) const
    { return Body().ReplaceVars(subs,result); } 
    //: Replace all vars in this literal with new ones.
    // The mapping between the replacements and the new values is returned in 'subs'
    // If no new replacements where found, false is returned.
    
    void ListConditionTerms(HSetC<LiteralC> &posTerms,HSetC<LiteralC> &negTerms) const
    { Body().ListConditionTerms(posTerms,negTerms); }
    //: Generate a set of positive and negative terms used in the condition.
    
    friend class LiteralBodyC;
    friend class VarBodyC;
  };

  inline LiteralC Literal()
  { return LiteralC(true); }
  //: Create an anonymous symbol.
  
  inline std::istream &operator>>(std::istream &strm,LiteralC &obj) {
    obj = LiteralC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline std::ostream &operator<<(std::ostream &out,const LiteralC &obj) {
    //obj.Save(out);
    out << obj.Name();
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,LiteralC &obj) {
    obj = LiteralC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const LiteralC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}

#endif
