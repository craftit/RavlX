// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_OR_HEADER
#define RAVLLOGIC_OR_HEADER 1
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Condition"
//! author="Charles Galambos"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Or.hh"

#include "Ravl/Logic/Condition.hh"
#include "Ravl/HSet.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Coniditional Or.
  
  class OrBodyC
    : public ConditionBodyC
  {
  public:
    OrBodyC();
    //: Default constructor.
    
    OrBodyC(UIntT arity);
    //: Constructor.
    //!param: arity - Number of terms to create
    
    OrBodyC(const SArray1dC<LiteralC> &nterms,bool useArrayDirectly = false);
    //: Constructor.
    // If useArrayDirectly is true then use the array directly, the first
    // element must be literalOr.
    
    OrBodyC(const LiteralC &term);
    //: Constructor.
    
    OrBodyC(std::istream &strm);
    //: Construct from a binary stream.
    
    OrBodyC(BinIStreamC &strm);
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
    //: Numnber of terms in or.

    virtual LiteralIterC Solutions(const StateC &state,BindSetC &binds) const;
    //: Return iterator through possibile matches to this literal in 'state', if any.
    
    void OrAdd(const LiteralC &lit);
    //: Add literal.
    
    void OrAdd(const SArray1dC<LiteralC> &lits);
    //: Add literals.
    
    virtual bool Substitute(const BindSetC &binds,LiteralC &result) const;
    //: Substitute variables in 'binds' for their bound values.
    // This builds a new literal with the substute values (if there
    // are any). The new value is assigned to 'result' <p>
    // Returns true if at least one substitution has been made,
    // false if none.
    
    virtual void ListConditionTerms(HSetC<LiteralC> &posTerms,HSetC<LiteralC> &negTerms) const;
    //: Generate a set of positive and negative terms used in the condition.
    
  };
  
  //! userlevel=Normal
  //: Or a set of conditions.
  
  class OrC
    : public ConditionC
  {
  public:
    OrC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    explicit OrC(bool)
      : ConditionC(*new OrBodyC())
    {}
    //: Constructor from a single term.
    
    explicit OrC(UIntT arity)
      : ConditionC(*new OrBodyC(arity))
    {}
    //: Constructor with n terms.
    //!param: arity - Number of terms to create
    
    OrC(const SArray1dC<LiteralC> &nterms,bool useArrayDirectly = false)
      : ConditionC(*new OrBodyC(nterms,useArrayDirectly))
    {}
    //: Construct from an array of Literals.
    // If useArrayDirectly is true then use the array directly, the first
    // element must be literalOr.
    
    OrC(std::istream &strm);
    //: Load from stream.
    
    OrC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    OrC(OrBodyC &bod)
      : ConditionC(bod)
    {}
    //: Body constructor.
    
    OrC(const OrBodyC *bod)
      : ConditionC(bod)
    {}
    //: Body constructor.
    
    OrBodyC &Body()
    { return static_cast<OrBodyC &>(LiteralC::Body()); }
    //: Access body.

    const OrBodyC &Body() const
    { return static_cast<const OrBodyC &>(LiteralC::Body()); }
    //: Access body.
    
  public:
    OrC(const LiteralC &term)
      : ConditionC(dynamic_cast<const OrBodyC *>(BodyPtr(term)))
    {}
    //: Construct from base class.
    
    OrC(bool add,const LiteralC &term)
      : ConditionC(*new OrBodyC(term))
    {}
    //: Construct from term.
    
    SArray1dC<LiteralC> &Terms()
    { return Body().Terms(); }
    //: Access set of terms.
    
    const SArray1dC<LiteralC> &Terms() const
    { return Body().Terms(); }
    //: Access set of terms.
    
    SizeT Size() const
    { return Body().Size(); }
    //: Numnber of terms in or.

    void OrAdd(const LiteralC &lit)
    { Body().OrAdd(lit); }
    //: Add literal.
    
    void OrAdd(const SArray1dC<LiteralC> &lits)
    { Body().OrAdd(lits); }
    //: Add literals.
    
    const OrC &operator+=(const LiteralC &lit) {
      OrAdd(lit);
      return *this;
    }
    //: Add literal.
    
    friend class OrBodyC;
  };
  
  TupleC operator+(const LiteralC &l1,const LiteralC &l2);
  //: Or two literals.

  extern const LiteralC literalOr;
  
}

#endif
