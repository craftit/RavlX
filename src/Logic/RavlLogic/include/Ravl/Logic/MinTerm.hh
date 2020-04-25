// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_MINTERM_HEADER
#define RAVLLOGIC_MINTERM_HEADER 1
////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Condition"
//! lib=RavlLogic
//! author="Charles Galambos"
//! file="Ravl/Logic/Base/MinTerm.hh"

#include "Ravl/SArray1d.hh"
#include "Ravl/Logic/And.hh"
#include "Ravl/Logic/Or.hh"

namespace RavlLogicN {

  class MinTermC;
  
  //! userlevel=Develop
  //: Logical Minterm.
  // This is a set of negative and positive conditions which are anded together.
  
  class MinTermBodyC 
    : public AndBodyC
  {
  public:
    MinTermBodyC();
    //: Default constructor.
    
    MinTermBodyC(const SArray1dC<LiteralC> &ts,const SArray1dC<LiteralC> &ns,bool useArrayDirectly = false);
    //: Constructor
    
    MinTermBodyC(const AndC &at,const OrC &ot);
    //: Constructor.
    
    MinTermBodyC(const LiteralC &lit,bool negate);
    //: Construct from a single literal.
    // Effectively add NotC(lit) if negate is true.

    MinTermBodyC(std::istream &strm);
    //: Construct from a binary stream.
    
    MinTermBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to binary stream 'out'.

    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    const SArray1dC<LiteralC> &Pos() const { 
      RavlAssert(t.IsValid());
      return t.Args(); 
    }
    //: Positive terms.
    
    const SArray1dC<LiteralC> &Neg() const { 
      RavlAssert(n.IsValid());
      return n.Args(); 
    }
    //: Negated terms.
    
    RCBodyVC &Copy() const;
    //: Copy minterm.
    
    bool AndAdd(const LiteralC &lit);
    //: Add another term to the minterm.

    bool AndNotAdd(const LiteralC &lit);
    //: Add a negated term to the minterm.
    
    bool Covers(const MinTermC &mt,BindSetC &bs) const;
    //: Does this minterm cover all terms of mt ?
    
    const AndC &PosTerm() const
    { return t; }
    //: Positive terms.
    
    const OrC &NegTerm() const
    { return n; }
    //: Negated terms.

    AndC &PosTerm()
    { return t; }
    //: Positive terms.
    
    OrC &NegTerm()
    { return n; }
    //: Negated terms.
    
    virtual bool Substitute(const BindSetC &binds,LiteralC &result) const;
    //: Substitute variables in 'binds' for their bound values.
    // This builds a new literal with the substute values (if there
    // are any). The new value is assigned to 'result' <p>
    // Returns true if at least one substitution has been made,
    // false if none.
    
  protected:
    void SetTerms(const SArray1dC<LiteralC> &nt,const SArray1dC<LiteralC> &nn,bool useArrayDirectly = false);
    //: Setup terms.
    
    AndC t;
    OrC n;
  };
  
  //! userlevel=Normal
  //: Logical Minterm.
  // This is a set of negative and positive conditions which are anded together.
  
  class MinTermC 
    : public AndC
  {
  public:
    MinTermC()
    {}
    //: Default constructor
    
   explicit  MinTermC(bool)
      : AndC(*new MinTermBodyC())
    {}
    //: Constructor
    // Creates a valid but empty minterm.
    
    MinTermC(const SArray1dC<LiteralC> &ts,const SArray1dC<LiteralC> &ns,bool useArrayDirectly = false)
      : AndC(*new MinTermBodyC(ts,ns,useArrayDirectly)) 
    {}
    //: Contructor
    
    MinTermC(const AndC &at,const OrC &ot)
      : AndC(*new MinTermBodyC(at,ot))
    {}
    //: Constructor.
    
    MinTermC(const LiteralC &lit,bool negate) 
      : AndC(*new MinTermBodyC(lit,negate))
    {}
    //: Construct from a literal.
    // Effectively add NotC(lit) if negate is true.
    
    MinTermC(const LiteralC &lit) 
      : AndC(dynamic_cast<const MinTermBodyC *>(BodyPtr(lit)))
    {}
    //: Base constructor
    
    MinTermC(std::istream &strm);
    //: Load from stream.
    
    MinTermC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    MinTermC(MinTermBodyC &bod)
      : AndC(bod)
    {}
    //: Body constructor.

    MinTermC(MinTermBodyC *bod)
      : AndC(bod)
    {}
    //: Body constructor.
    
    MinTermBodyC &Body()
    { return static_cast<MinTermBodyC &>(LiteralC::Body()); }
    //: Access body.
    
    const MinTermBodyC &Body() const
    { return static_cast<const MinTermBodyC &>(LiteralC::Body()); }
    //: Access body.
    
  public:        
    const SArray1dC<LiteralC> &Pos() const
    { return Body().Pos(); }
    //: Positive terms.
    
    const SArray1dC<LiteralC> &Neg() const
    { return Body().Neg(); }
    //: Negated terms.

    const AndC &PosTerm() const
    { return Body().PosTerm(); }
    //: Positive terms.
    
    const OrC &NegTerm() const
    { return Body().NegTerm(); }
    //: Negated terms.
    
    AndC &PosTerm()
    { return Body().PosTerm(); }
    //: Positive terms.
    
    OrC &NegTerm()
    { return Body().NegTerm(); }
    //: Negated terms.
    
    MinTermC Copy() const { 
      if(!IsValid())
	      return MinTermC();
      return MinTermC(static_cast<MinTermBodyC &>(Body().Copy()));
    }
    //: Copy this min term.
    
    bool AndNotAdd(const LiteralC &lit) 
    { return Body().AndNotAdd(lit); }
    //: Add a negated term to the minterm.
    
    const MinTermC &operator*=(const LiteralC &lit) { 
      Body().AndAdd(lit); 
      return *this;
    }
    
    bool Covers(const MinTermC &mt,BindSetC &bs) const
    { return Body().Covers(mt,bs); }
    //: Does this minterm cover all terms of mt ?

    friend class MinTermBodyC;
  };
  
  MinTermC operator*(const MinTermC &mt1,const MinTermC &mt2);
  //: And two min-terms.
  
  MinTermC operator*(const MinTermC &mt1,const LiteralC &mt2);
  //: And a min term and a literal.
  
  MinTermC operator*(const LiteralC &lit1,const MinTermC &mt2);
  //: And a min term and a literal.
  
}

#endif
