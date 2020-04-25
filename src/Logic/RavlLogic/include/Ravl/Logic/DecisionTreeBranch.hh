// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_DECISIONTREBRANCH_HEADER
#define RAVLLOGIC_DECISIONTREBRANCH_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/DecisionTree/DecisionTreeBranch.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Logic.Decision Tree"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/DecisionTreeElement.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Branch in the decision tree.
  
  class DecisionTreeBranchBodyC 
    : public DecisionTreeElementBodyC
  {
  public:
    DecisionTreeBranchBodyC();
    //: Default constructor.
    
    DecisionTreeBranchBodyC(istream &strm);
    //: Construct from a stream.
    
    DecisionTreeBranchBodyC(BinIStreamC &strm);
    //: Construct from a binary stream.
    
    virtual bool Save(ostream &out) const;
    //: Save to stream 'out'.
    
    virtual bool Save(BinOStreamC &out) const;
    //: Save to binary stream 'out'.

    virtual void Dump(ostream &out,IntT level = 0) const;
    //: Dump node in human readable form,
    
  protected:
    
  };
  
  //! userlevel=Advanced
  //: Branch in the decision tree.
  
  class DecisionTreeBranchC 
    : public DecisionTreeElementC
  {
  public:
    DecisionTreeBranchC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    DecisionTreeBranchC(istream &strm);
    //: Load from stream.
    
    DecisionTreeBranchC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    DecisionTreeBranchC(DecisionTreeBranchBodyC &bod)
      : DecisionTreeElementC(bod)
    {}
    //: Body constructor.
    
    DecisionTreeBranchC(DecisionTreeBranchBodyC *bod)
      : DecisionTreeElementC(bod)
    {}
    //: Body constructor.
    
    DecisionTreeBranchBodyC &Body()
    { return static_cast<DecisionTreeBranchBodyC &>(DecisionTreeElementC::Body()); }
    //: Access body.

    const DecisionTreeBranchBodyC &Body() const
    { return static_cast<const DecisionTreeBranchBodyC &>(DecisionTreeElementC::Body()); }
    //: Access body.
    
  public:
    
  };
  
}

#endif
