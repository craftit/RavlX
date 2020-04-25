// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEXLEAFVARITER_HEADER
#define RAVLLOGIC_LITERALINDEXLEAFVARITER_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Index"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexLeafVarIter.hh"

#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/Logic/LiteralIndex.hh"
#include "Ravl/Logic/LiteralMapIter.hh"
#include "Ravl/Logic/LiteralIndexLeaf.hh"
#include "Ravl/HashIter.hh"

namespace RavlLogicN {

  //! userlevel=Develop
  //: Literal Index leaf iterator.
  // This class is used by LiteralIndexFilterBaseC to iterate through an
  // index as if it was a node in the index tree. This is when the filter
  // is just a simple variable to bind it to every key found in the
  // index.
  
  class LiteralIndexLeafVarBodyIterC 
    : public LiteralMapIterBodyC<LiteralIndexElementC>
  {
  public:
    LiteralIndexLeafVarBodyIterC()
    {}
    //: Default constructor
    
    LiteralIndexLeafVarBodyIterC(const LiteralIndexBaseC &ind,const LiteralC &fixedLit,const LiteralIndexLeafC &fixedLeaf);
    //: Constructor
    
    virtual bool First();
    //: Goto first element.
    
    virtual bool Next();
    //: Goto next element.
    // returns true if its valid.
    
    virtual bool IsElm() const ;
    //: At a valid element ?
    
    virtual LiteralC Data();
    //: Get key for current item.
    
    virtual LiteralIndexElementC &MappedData();
    //: Access data.
    
    virtual const LiteralIndexElementC &MappedData() const;
    //: Access data.
    
  protected:  
    bool isFirst;
    LiteralC fixedLit;
    LiteralIndexLeafC fixedLeaf;
    HashIterC<LiteralC,LiteralIndexLeafC>  hit;
  };


  //! userlevel=Advanced
  //: Literal Index leaf iterator.
  // This class is used by LiteralIndexFilterBaseC to iterate through an
  // index as if it was a node in the index tree. This is when the filter
  // is just a simple variable to bind it to every key found in the
  // index.
  
  class LiteralIndexLeafVarIterC 
    : public LiteralMapIterC<LiteralIndexElementC>
  {
  public:
    LiteralIndexLeafVarIterC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    LiteralIndexLeafVarIterC(const LiteralIndexBaseC &ind,const LiteralC &fixedLit,const LiteralIndexLeafC &fixedLeaf);
    //: Constructor.
    
  };
}
#endif
