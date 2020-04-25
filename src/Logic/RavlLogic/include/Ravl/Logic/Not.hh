// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NOT_HEADER
#define RAVLLOGIC_NOT_HEADER 1
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Condition"
//! lib=RavlLogic
//! author="Charles Galambos"
//! file="Ravl/Logic/Base/Not.hh"

#include "Ravl/Logic/Condition.hh"
#include "Ravl/HSet.hh"

namespace RavlLogicN {
  class AndC;
  class OrC;
  
  //! userlevel=Develop
  //: Not operator body
  
  class NotBodyC
    : public ConditionBodyC
  {
  public:
    NotBodyC();
    //: Default constructor.
    
    NotBodyC(const LiteralC &nterm);
    //: Constructor.

    NotBodyC(std::istream &strm);
    //: Construct from a binary stream.
    
    NotBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to binary stream 'out'.

    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    LiteralC &Term()
    { return args[1]; }
    //: Access set of terms.
    
    const LiteralC &Term() const
    { return args[1]; }
    //: Access set of terms.
    
    virtual bool IsEqual(const LiteralC &oth) const;
    //: Is this equal to another condition ?

    virtual bool IsGrounded() const;
    //: Is this a simple expression with no variables ?

    virtual bool Unify(const LiteralC &oth,BindSetC &bs) const;
    //: Unify with another variable.
    
    virtual StringC Name() const;
    //: Get the name of symbol.
    
    virtual bool Test(const StateC &state,BindSetC &binds) const;
    //: Test if condition is true in 'state'.
    
    virtual LiteralIterC Solutions(const StateC &state,BindSetC &binds) const;
    //: Return iterator through possibile matches to this literal in 'state', if any.

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
  //: Not a condition.
  // Gives the inverse truth condition.
  
  class NotC
    : public ConditionC
  {
  public:
    NotC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    explicit NotC(bool)
      : ConditionC(*new NotBodyC())
    {}
    //: Constructor from a single term.
    
    NotC(bool,const LiteralC &term)
      : ConditionC(*new NotBodyC(term))
    {}
    //: Constructor from a single term.
    
    NotC(std::istream &strm);
    //: Load from stream.
    
    NotC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    NotC(NotBodyC &bod)
      : ConditionC(bod)
    {}
    //: Body constructor.
    
    NotC(NotBodyC *bod)
      : ConditionC(bod)
    {}
    //: Body constructor.
    
    NotBodyC &Body()
    { return static_cast<NotBodyC &>(LiteralC::Body()); }
    //: Access body.

    const NotBodyC &Body() const
    { return static_cast<const NotBodyC &>(LiteralC::Body()); }
    //: Access body.
    
  public:
    NotC(const LiteralC &term)
      : ConditionC(dynamic_cast<const NotBodyC *>(BodyPtr(term)))
    {}
    //: Constructor from a single term.
    
    LiteralC &Term()
    { return Body().Term(); }
    //: Access set of terms.
    
    friend class NotBodyC;
  };
  
  //! userlevel=Normal
  
  LiteralC operator!(const LiteralC &lit);
  //: Not operator.

  extern LiteralC literalNot;
  
}

#endif
