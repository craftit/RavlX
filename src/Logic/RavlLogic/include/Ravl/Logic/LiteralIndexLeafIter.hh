// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEXLEAFITER_HEADER
#define RAVLLOGIC_LITERALINDEXLEAFITER_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Index"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexLeafIter.hh"

#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/Logic/LiteralIndex.hh"
#include "Ravl/Logic/LiteralMapIter.hh"
#include "Ravl/HashIter.hh"

namespace RavlLogicN {

  //! userlevel=Develop
  //: Literal Index leaf iterator.
  // This class is used by LiteralIndexFilterBaseC to iterate through an
  // index as if it was a node in the index tree. This is when the filter
  // is just a simple variable to bind it to every key found in the
  // index.
  
  class LiteralIndexLeafBodyIterC 
    : public LiteralMapIterBodyC<LiteralIndexElementC>
  {
  public:
    LiteralIndexLeafBodyIterC()
    {}
    //: Default constructor
    
    LiteralIndexLeafBodyIterC(const LiteralIndexBaseC &ind,bool varsOnly = false);
    //: Constructor
    
    virtual bool First();
    //: Goto first element.
    
    virtual bool Next();
    //: Goto next element.
    // returns true if its valid.
    
    virtual bool IsElm() const;
    //: At a valid element ?
    
    virtual LiteralC Data();
    //: Get key for current item.
    
    virtual LiteralIndexElementC &MappedData();
    //: Access data.
    
    virtual const LiteralIndexElementC &MappedData() const;
    //: Access data.
    
  protected:    
    HashIterC<LiteralC,LiteralIndexLeafC>  hit;
    bool varsOnly;
  };


  //! userlevel=Advanced
  //: Literal Index leaf iterator.
  // This class is used by LiteralIndexFilterBaseC to iterate through an
  // index as if it was a node in the index tree. This is when the filter
  // is just a simple variable to bind it to every key found in the
  // index.
  
  class LiteralIndexLeafIterC 
    : public LiteralMapIterC<LiteralIndexElementC>
  {
  public:
    LiteralIndexLeafIterC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    LiteralIndexLeafIterC(const LiteralIndexBaseC &ind,bool varsOnly = false);
    //: Constructor.
    
  };
}
#endif
