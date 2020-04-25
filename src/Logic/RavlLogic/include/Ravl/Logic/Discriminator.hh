// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_DISCRIMINATOR_HEADER
#define RAVLLOGIC_DISCRIMINATOR_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/Discriminator.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Decision Tree"

#include "Ravl/Logic/DecisionExamples.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/Logic/SampleLiteral.hh"
#include "Ravl/Logic/SampleState.hh"

namespace RavlLogicN {
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Tool to find an expression which discriminates between sets of states.
  
  class DiscriminatorBodyC
    : public RCBodyVC
  {
  public:
    DiscriminatorBodyC()
    {}
    //: Default constructor.
    
    DiscriminatorBodyC(istream &strm);
    //: Construct from a stream.
    
    DiscriminatorBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(ostream &out) const;
    //: Save to stream 'out'.
    
    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.
    
    virtual LiteralC Distinguish(const DecisionExamplesC &set1,const DecisionExamplesC &set2);
    //: Distinguish between the the two sets of examples.
    
    virtual LiteralC BestDiscriminator(const DecisionExamplesC &set);
    //: Choose the best distriminator to seperate the decisions made in 'set'. 
    
    virtual LiteralC BestDiscriminator(const DataSet2C<SampleStateC,SampleLiteralC> &data);
    //: Choose the best distriminator to seperate the decisions made in 'data'. 
  protected:
  };

  //! userlevel=Normal
  //: Tool to find an expression which discriminates between sets of states.
  
  class DiscriminatorC
    : public RCHandleVC<DiscriminatorBodyC>
  {
  public:
    DiscriminatorC()
    {}
    //: Constructor.
    
    explicit DiscriminatorC(bool)
      : RCHandleVC<DiscriminatorBodyC>(*new DiscriminatorBodyC())
    {}
    //: Construct a default discriminator.

    DiscriminatorC(istream &strm);
    //: Load from stream.

    DiscriminatorC(BinIStreamC &strm);
    //: Load from binary stream.
    
  protected:
    DiscriminatorC(DiscriminatorBodyC &bod)
      : RCHandleVC<DiscriminatorBodyC>(bod)
    {}
    //: Body constructor.

    DiscriminatorC(DiscriminatorBodyC *bod)
      : RCHandleVC<DiscriminatorBodyC>(bod)
    {}
    //: Body constructor.
    
    DiscriminatorBodyC &Body()
    { return RCHandleC<DiscriminatorBodyC>::Body(); }
    //: Access body.
    
    const DiscriminatorBodyC &Body() const
    { return RCHandleC<DiscriminatorBodyC>::Body(); }
    //: Access body.
    
  public:
    LiteralC Distinguish(const DecisionExamplesC &set1,const DecisionExamplesC &set2)
    { return Body().Distinguish(set1,set2); }
    //: Distinguish between the the two sets of examples.
    
    LiteralC BestDiscriminator(const DecisionExamplesC &set)
    { return Body().BestDiscriminator(set); }
    //: Choose the best distriminator to seperate the decisions made in 'set'. 
    
    LiteralC BestDiscriminator(const DataSet2C<SampleStateC,SampleLiteralC> &data)
    { return Body().BestDiscriminator(data); }
    //: Choose the best distriminator to seperate the decisions made in 'data'. 
    
  };
  
  inline istream &operator>>(istream &strm,DiscriminatorC &obj) {
    obj = DiscriminatorC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.
  
  inline ostream &operator<<(ostream &out,const DiscriminatorC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
  
  inline BinIStreamC &operator>>(BinIStreamC &strm,DiscriminatorC &obj) {
    obj = DiscriminatorC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.
  
  inline BinOStreamC &operator<<(BinOStreamC &out,const DiscriminatorC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}

#endif
