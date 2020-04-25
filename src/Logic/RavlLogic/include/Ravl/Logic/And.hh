// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_AND_HEADER
#define RAVLLOGIC_AND_HEADER 1
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Condition"
//! author="Charles Galambos"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/And.hh"

#include "Ravl/Logic/Condition.hh"

namespace RavlLogicN {

  //! userlevel=Develop
  //: And a set of conditions.
  
  class AndBodyC
    : public ConditionBodyC
  {
  public:
    AndBodyC();
    //: Default constructor.
    
    AndBodyC(UIntT arity);
    //: Create an and term with 'arity' elements.
    
    AndBodyC(const SArray1dC<LiteralC> &set,bool useArrayDirectly = false);
    //: Constructor.
    // If useArrayDirectly is true then use the array directly, the first
    // element must be literalAnd.

    AndBodyC(std::istream &strm);
    //: Construct from a binary stream.
    
    AndBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to binary stream 'out'.

    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    SArray1dC<LiteralC> &Terms()
    { return args; }
    //: Access set of terms.

    const SArray1dC<LiteralC> &Terms() const
    { return args; }
    //: Access set of terms.
    
    virtual bool Unify(const LiteralC &oth,BindSetC &bs) const;
    //: Unify with another variable.
    
    SizeT Size() const
    { return args.Size()-1; }
    //: Get the number of terms to be anded together.

    virtual LiteralIterC Solutions(const StateC &state,BindSetC &binds) const;
    //: Return iterator through possibile matches to this literal in 'state', if any.

    virtual bool Substitute(const BindSetC &binds,LiteralC &result) const;
    //: Substitute variables in 'binds' for their bound values.
    // This builds a new literal with the substute values (if there
    // are any). The new value is assigned to 'result' <p>
    // Returns true if at least one substitution has been made,
    // false if none.
    
    void AndAdd(const LiteralC &lit);
    //: Add literal.
    
    void AndAdd(const SArray1dC<LiteralC> &lits);
    //: Add literals.
    
    virtual void ListConditionTerms(HSetC<LiteralC> &posTerms,HSetC<LiteralC> &negTerms) const;
    //: Generate a set of positive and negative terms used in the condition.
    
  };
  
  //! userlevel=Normal
  //: And a set of conditions.
  
  class AndC
    : public ConditionC
  {
  public:
    AndC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    AndC(const SArray1dC<LiteralC> &set,bool useArrayDirectly = false)
      : ConditionC(*new AndBodyC(set,useArrayDirectly))
    {}
    //: Constructor from a single term.
    // If useArrayDirectly is true then use the array directly, the first
    // element must be literalAnd.
    
    explicit AndC(bool)
      : ConditionC(*new AndBodyC())
    {}
    //: Constructor.
    
    explicit AndC(UIntT arity)
      : ConditionC(*new AndBodyC(arity))
    {}
    //: Constructor.
    //!param: arity - Number of and terms
    
    AndC(std::istream &strm);
    //: Load from stream.
    
    AndC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    AndC(const AndBodyC *bod)
      : ConditionC(bod)
    {}
    //: Body constructor.
    
    AndC(AndBodyC &bod)
      : ConditionC(bod)
    {}
    //: Body constructor.
    
    AndBodyC &Body()
    { return static_cast<AndBodyC &>(LiteralC::Body()); }
    //: Access body.

    const AndBodyC &Body() const
    { return static_cast<const AndBodyC &>(LiteralC::Body()); }
    //: Access body.
    
  public:
    AndC(const LiteralC &term)
      : ConditionC(dynamic_cast<const AndBodyC *>(BodyPtr(term)))
    {}
    //: Construct from base class.
    
    SArray1dC<LiteralC> &Terms()
    { return Body().Terms(); }
    //: Access set of terms.

    const SArray1dC<LiteralC> &Terms() const
    { return Body().Terms(); }
    //: Access set of terms.
    
    SizeT Size() const
    { return Body().Size(); }
    //: Get the number of terms to be anded together.
    
    void AndAdd(const LiteralC &lit)
    { Body().AndAdd(lit); }
    //: Add literal.
    
    void AndAdd(const SArray1dC<LiteralC> &lits)
    { Body().AndAdd(lits); }
    //: Add literals.
    
    const AndC &operator*=(const LiteralC &lit) {
      AndAdd(lit);
      return *this;
    }
    //: Add some terms.
    
    const AndC &operator*=(const SArray1dC<LiteralC> &lits) {
      AndAdd(lits);
      return *this;
    }
    //: Add some terms.
    
    friend class AndBodyC;
  };
  
  TupleC operator*(const LiteralC &l1,const LiteralC &l2);
  //: And operator.

  extern const LiteralC literalAnd;
  
}

#endif
