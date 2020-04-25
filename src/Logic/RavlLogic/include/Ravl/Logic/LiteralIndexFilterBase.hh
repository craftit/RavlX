// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEXFILTERBASE_HEADER
#define RAVLLOGIC_LITERALINDEXFILTERBASE_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Index"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexFilterBase.hh"

#include "Ravl/Logic/LiteralIndexBase.hh"
#include "Ravl/Logic/Var.hh"
#include "Ravl/Logic/BindSet.hh"
#include "Ravl/Logic/LiteralIndexElement.hh"
#include "Ravl/Logic/LiteralIndexLeaf.hh"
#include "Ravl/Logic/LiteralIter.hh"
#include "Ravl/Stack.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Filter choice point.
  // Internal use only <p>
  // Used to store points in the tree search its worth backtracking to.
  
  class LiteralIndexFilterChoicePointC 
  {
  public:
    LiteralIndexFilterChoicePointC()
    {}
    //: Default constructor.
    
    LiteralIndexFilterChoicePointC(const LiteralIndexElementC &elem,
				   BindMarkT bm,
				   LiteralC &nvar,
				   const LiteralMapIterC<LiteralIndexElementC> &it)
      : node(elem),
	bindMark(bm),
	var(nvar),
	iter(it)
    {}
    //: Constructor.
    
    LiteralMapIterC<LiteralIndexElementC> &Iter()
    { return iter; }
    //: Access iterator.
    
    BindMarkT BindMark() const
    { return bindMark; }
    //: Mark binds.
    
    LiteralIndexElementC &Node()
    { return node; }
    //: Access node we're at.
 
    LiteralC &Var()
    { return var; }
    //: Variable we're binding values to.
      
  protected:
    LiteralIndexElementC node; // Place in tree.
    BindMarkT bindMark;        // Bind Marker, where to unwind the stack to.
    LiteralC var;                  // Variable we're binding values to.
    LiteralMapIterC<LiteralIndexElementC> iter;         // Iterator.  Iterator used to go through possible matches.
  };
  
  
  //! userlevel=Develop
  //: Filter contents of an index.
  // This implements most of the filter operations, and returns 
  // the matching leafs in the index tree.
  
  class LiteralIndexFilterBaseBodyC 
  {
  public:
    LiteralIndexFilterBaseBodyC()
    {}
    //: Default constructor
    
    LiteralIndexFilterBaseBodyC(const LiteralIndexBaseC &nindex,const LiteralC &nfilter);
    //: Construct a new filter.
    
    LiteralIndexFilterBaseBodyC(const LiteralIndexBaseC &nindex,const LiteralC &nfilter,BindSetC &binds);
    //: Construct a new filter with variable bindings.
    
    bool Next();
    //: Goto next data element.
    // returns true if next element is valid.
    
    bool First();
    //: Goto first data element.
    // returns true if next element is valid.
    
    inline bool IsElm() const
    { return at.IsValid(); }
    //: At a valid element ?
    // returns true if iterator is at a valid
    // element.
    
    LiteralIndexLeafC &Data()
    { return at; }
    //: Get current leaf.
    
    const LiteralIndexLeafC &Data() const
    { return at; }
    //: Get current leaf.
    
  protected:
    bool ExploreMatches(LiteralIndexElementC place);
    //: Expore matches.
    // returns true if one is found.
    
    BindMarkT initialMark;
    BindSetC binds;
    LiteralC filter;
    LiteralIndexBaseC index;
    StackC<LiteralIndexFilterChoicePointC> stack;
    LiteralIndexLeafC at;
  };
  
  
}


#endif
