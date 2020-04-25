// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_DECISIONTREELEAF_HEADER
#define RAVLLOGIC_DECISIONTREELEAF_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/DecisionTreeLeaf.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Decision Tree"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/DecisionTreeElement.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Leaf in the decision tree.
  
  class DecisionTreeLeafBodyC 
    : public DecisionTreeElementBodyC
  {
  public:
    DecisionTreeLeafBodyC();
    //: Default constructor.

    DecisionTreeLeafBodyC(const LiteralC &decision,const DecisionExamplesC &nexamples);
    //: Constructor.
    
    DecisionTreeLeafBodyC(istream &strm);
    //: Construct from a stream.
    
    DecisionTreeLeafBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(ostream &out) const;
    //: Save to stream 'out'.
    
    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.

    LiteralC &Decision()
    { return decision; }
    //: Value of decision of leaf.
    
    virtual bool IsLeaf() const
    { return true; }
    //: Is this a leaf in the decision tree ?
    
    virtual void BuildRuleSet(const LiteralC &preCond,StateC &ruleSet) const;
    //: Go through the tree building a rule set.
    
    virtual void Dump(ostream &out,IntT level = 0) const;
    //: Dump node in human readable form,
    
  protected:
    // List of examples.
    LiteralC decision; // Value of decision at the leaf.
  };
  
  //! userlevel=Advanced
  //: Leaf in the decision tree.
  
  class DecisionTreeLeafC 
    : public DecisionTreeElementC
  {
  public:
    DecisionTreeLeafC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    DecisionTreeLeafC(const LiteralC &decision,const DecisionExamplesC  &nexamples)
      : DecisionTreeElementC(*new DecisionTreeLeafBodyC(decision,nexamples))
    {}
    //: Constructor.
    
    DecisionTreeLeafC(const DecisionTreeElementC &oth)
      : DecisionTreeElementC(dynamic_cast<const DecisionTreeElementBodyC *>(BodyPtr(oth)))
    {}
    //: Base constructor.
    // creates an invalid handle if DecisionTreeElement isn't a leaf.
    
    DecisionTreeLeafC(istream &strm);
    //: Load from stream.
    
    DecisionTreeLeafC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    DecisionTreeLeafC(DecisionTreeLeafBodyC &bod)
      : DecisionTreeElementC(bod)
    {}
    //: Body constructor.
    
    DecisionTreeLeafC(DecisionTreeLeafBodyC *bod)
      : DecisionTreeElementC(bod)
    {}
    //: Body constructor.
    
    DecisionTreeLeafBodyC &Body()
    { return static_cast<DecisionTreeLeafBodyC &>(DecisionTreeElementC::Body()); }
    //: Access body.
    
    const DecisionTreeLeafBodyC &Body() const
    { return static_cast<const DecisionTreeLeafBodyC &>(DecisionTreeElementC::Body()); }
    //: Access body.
    
  public:
    
    LiteralC &Decision()
    { return Body().Decision(); }
    //: Value of decision of leaf.
   
  };
  
}

#endif
