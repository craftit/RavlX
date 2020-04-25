// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_CONDITION_HEADER
#define RAVLLOGIC_CONDITION_HEADER 1
////////////////////////////////////////////////////////////////
//! rcsid=$Id$"
//! docentry="Ravl.API.Logic.Condition"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/Condition.hh"
//! author="Charles Galambos"

#include "Ravl/RefCounter.hh"
#include "Ravl/Logic/Tuple.hh"

namespace RavlLogicN {

  class ConditionC;
  class StateC;
  
  //! userlevel=Develop
  //: Abstract condition
  
  class ConditionBodyC
    : public TupleBodyC
  {
  public:
    ConditionBodyC()
    {}
    //: Default constructor.

    ConditionBodyC(const SArray1dC<LiteralC> &lits)
      : TupleBodyC(lits)
    {}
    //: Default constructor.

    ConditionBodyC(UIntT arity)
      : TupleBodyC(arity)
    {}
    //: Default constructor.
    
    ConditionBodyC(std::istream &strm);
    //: Construct from a binary stream.
    
    ConditionBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to binary stream 'out'.
    
    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    virtual bool Test(const StateC &state,BindSetC &binds) const;
    //: Test if condition is true in 'state'.
    
    virtual bool IsEqual(const LiteralC &oth) const;
    //: Is this equal to another condition ?
    
    virtual UIntT Hash() const;
    //: Generate has value for condition.
    
    virtual bool Substitute(const BindSetC &binds,LiteralC &result) const;
    //: Substitute variables in 'binds' for their bound values.
    // This builds a new literal with the substute values (if there
    // are any). The new value is assigned to 'result' <p>
    // Returns true if at least one substitution has been made,
    // false if none.
    
    virtual bool IsCondition() const;
    //: Test if this is a condition.
    
    virtual void ListConditionTerms(HSetC<LiteralC> &posTerms,HSetC<LiteralC> &negTerms) const;
    //: Generate a set of positive and negative terms used in the condition.
    
  protected:
    void AddTerms(const SArray1dC<LiteralC> &terms);
    //: Add some terms.
    
    void AddTerm(const LiteralC &term);
    //: Add a term.
    
    friend class ConditionC;
  };
  
  //! userlevel=Normal
  //: Abstract condition
  
  class ConditionC
    : public TupleC
  {
  public:
    ConditionC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    ConditionC(std::istream &strm);
    //: Load from stream.
    
    ConditionC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    ConditionC(ConditionBodyC &bod)
      : TupleC(bod)
    {}
    //: Body constructor.

    ConditionC(const ConditionBodyC *bod)
      : TupleC(bod)
    {}
    //: Body constructor.
    
    ConditionBodyC &Body()
    { return static_cast<ConditionBodyC &>(LiteralC::Body()); }
    //: Access body.
    
    const ConditionBodyC &Body() const
    { return static_cast<const ConditionBodyC &>(LiteralC::Body()); }
    //: Access body.
    
    void AddTerms(const SArray1dC<LiteralC> &terms)
    { Body().AddTerms(terms); }
    //: Add some terms.
    
    void AddTerm(const LiteralC &term)
    { Body().AddTerm(term); }
    //: Add some terms.
    
  public:
    ConditionC(const LiteralC &term)
      : TupleC(dynamic_cast<const ConditionBodyC *>(BodyPtr(term)))
    {}
    //: Construct from base class.
    
    bool IsEqual(const ConditionC &oth) const
    { return Body().IsEqual(oth); }
    //: Is this equal to another condition ?
    
    bool operator==(const ConditionC &oth) const
    { return Body().IsEqual(oth); }
    //: Is this equal to another condition ?
    
    
    friend class ConditionBodyC;
  };

}

#endif
