// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_DECISIONTREEBASE_HEADER
#define RAVLLOGIC_DECISIONTREEBASE_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/DecisionTreeBase.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Decision Tree"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/DecisionTreeElement.hh"
#include "Ravl/Logic/DecisionTreeLeaf.hh"
#include "Ravl/Logic/Discriminator.hh"
#include "Ravl/PatternRec/DataSet2.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Decision tree body.
  
  class DecisionTreeBaseBodyC 
    : public RCBodyC
  {
  public:
    DecisionTreeBaseBodyC();
    //: Default constructor.
    // Use the default discriminator.
    
    DecisionTreeBaseBodyC(const DiscriminatorC &desc);
    //: Constructor.
    
    DecisionTreeBaseBodyC(istream &strm);
    //: Construct from a stream.
    
    DecisionTreeBaseBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    bool Save(ostream &out) const;
    //: Save to stream 'out'.
    
    bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    DecisionTreeLeafC Find(const StateC &state);
    //: Find the decision for given 'state'.
    
    bool Add(const StateC &state,const LiteralC &decision);
    //: Add a new example to the tree.
    
    void Train(const DataSet2C<SampleStateC,SampleLiteralC > &data);
    //: Train the decision tree with the new data.
    
    void SetDiscriminator(const DiscriminatorC &desc);
    //: Set the discriminator to use for training.

    void Dump(ostream &out) const;
    //: Dump the tree in a human readable form to stream out.
    
    StateC BuildRuleSet() const;
    //: Build a rule set from the decision tree.
    
  protected:
    DiscriminatorC discriminator; // Used in building the tree.
    DecisionTreeElementC root; // Root of tree.    
  };
  
  //! userlevel=Advanced
  //: Decision tree.
  
  class DecisionTreeBaseC 
    : public RCHandleC<DecisionTreeBaseBodyC>
  {
  public:
    DecisionTreeBaseC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    explicit DecisionTreeBaseC(bool)
      : RCHandleC<DecisionTreeBaseBodyC>(* new DecisionTreeBaseBodyC())
    {}
    //: Constructor.
    // Create a decision tree with the default discriminator.
    
    DecisionTreeBaseC(const DiscriminatorC &desc)
      : RCHandleC<DecisionTreeBaseBodyC>(* new DecisionTreeBaseBodyC(desc))
    {}
    //: Constructor.
    
    DecisionTreeBaseC(istream &strm)
      : RCHandleC<DecisionTreeBaseBodyC>(* new DecisionTreeBaseBodyC(strm))      
    {}
    //: Construct from a stream.
    
    DecisionTreeBaseC(BinIStreamC &strm)
      : RCHandleC<DecisionTreeBaseBodyC>(* new DecisionTreeBaseBodyC(strm))
    {}
    //: Construct from a binary stream.
    
  protected:
    DecisionTreeBaseC(DecisionTreeBaseBodyC &bod)
      : RCHandleC<DecisionTreeBaseBodyC>(bod)
      {}
    //: Body constructor.
    
    DecisionTreeBaseC(const DecisionTreeBaseBodyC *bod)
      : RCHandleC<DecisionTreeBaseBodyC>(bod)
      {}
    //: Body constructor.
    
    DecisionTreeBaseBodyC &Body()
    { return RCHandleC<DecisionTreeBaseBodyC>::Body(); }
    //: Access body.

    const DecisionTreeBaseBodyC &Body() const
    { return RCHandleC<DecisionTreeBaseBodyC>::Body(); }
    //: Access body.
    
  public:
    bool Save(ostream &out) const
    { return Body().Save(out); }
    //: Save to stream 'out'.
    
    bool Save(BinOStreamC &out) const
    { return Body().Save(out); }
    //: Save to binary stream 'out'.
    
    DecisionTreeLeafC Find(const StateC &state)
    { return Body().Find(state); }
    //: Find the decision for given 'state'.
    
    bool Add(const StateC &state,const LiteralC &decision)
    { return Body().Add(state,decision); }
    //: Add a new example to the tree.
    
    void Train(const DataSet2C<SampleStateC,SampleLiteralC> &data)
    { Body().Train(data); }
    //: Train the decision tree with the new data.
    
    void SetDiscriminator(const DiscriminatorC &desc)
    { Body().SetDiscriminator(desc); }
    //: Set the discriminator to use for training.
    
    void Dump(ostream &out) const;
    //: Dump the tree in a human readable form to stream out.
    
    StateC BuildRuleSet() const
    { return Body().BuildRuleSet(); }
    //: Build a rule set from the decision tree.
  };
  
  inline istream &operator>>(istream &strm,DecisionTreeBaseC &obj) {
    obj = DecisionTreeBaseC(strm);
    return strm;
  }
  //: Load from a stream.
  
  inline ostream &operator<<(ostream &out,const DecisionTreeBaseC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DecisionTreeBaseC &obj) {
    obj = DecisionTreeBaseC(strm);
    return strm;
  }
  //: Load from a binary stream.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DecisionTreeBaseC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  
}

#endif
