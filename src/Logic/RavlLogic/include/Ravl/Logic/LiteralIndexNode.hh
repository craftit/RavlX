// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGICLITERALINDEXNODE_HEADER
#define RAVLLOGICLITERALINDEXNODE_HEADER
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Index"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexNode.hh"

#include "Ravl/Logic/LiteralIndexElement.hh"
#include "Ravl/Logic/LiteralIndex.hh"

namespace RavlLogicN {
  
  //! userlevel=Develop
  //: Literal index node.
  
  class LiteralIndexNodeBodyC 
    : public LiteralIndexElementBodyC
  {
  public:
    LiteralIndexNodeBodyC(UIntT nn = 0)
      : term(nn),
	index(true)
    {}
    
    UIntT Term() const
    { return term; }
    //: Access element of tuple we're indexing.
    
    virtual bool Lookup(const TupleC &key,LiteralIndexElementC &next);
    //: Lookup next level in tree.
    // returns true if this level is a leaf and leave next unchanged.
    // if no suitable node is found 'next' is made invalid.
    
    virtual bool Del(const TupleC &key);
    //: Delete branch with index 'key'.
    
    virtual LiteralMapIterC<LiteralIndexElementC> Filter(const LiteralC &key,LiteralIndexElementC &next,BindSetC &binds,LiteralC &var);
    //: Filter next level in tree.
    // If there's a choice return an iterator otherwise the next branch
    // in the tree is assigned to 'next'.  var is the variable to witch
    // the value of the iterator should be bound.
    
    virtual bool Insert(const TupleC &key,LiteralIndexElementC &next);
    //: Insert/Replace element into the node.

    virtual StringC Name();
    //: Get name for element.
    // usefull for debuging.
    
    virtual UIntT Size() const;
    //: Return the size of the index.
    // The number of branches from this node is returned.
    
    virtual void Dump(ostream &out,int level = 0) const;
    //: Dump index in human readable form.
    // For debugging only.
    
  protected:
    UIntT term; // Position in tuple we're indexing.
    LiteralIndexC<LiteralIndexElementC> index;
    LiteralIndexElementC none; // Used for tuples with arity smaller than 'term'. 
  };
  
  //! userlevel=Advanced
  //: Literal index node.
  
  class LiteralIndexNodeC
    : public LiteralIndexElementC
  {
  public:
    LiteralIndexNodeC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    LiteralIndexNodeC(UIntT term)
      : LiteralIndexElementC(*new LiteralIndexNodeBodyC(term))
    {}
    //: Constructor.
   
  protected:
    LiteralIndexNodeC(LiteralIndexNodeBodyC &bod)
      : LiteralIndexElementC(bod)
    {}
    //: Body constructor
    
    LiteralIndexNodeC(const LiteralIndexNodeBodyC *bod)
      : LiteralIndexElementC(bod)
    {}
    //: Body constructor
    
    LiteralIndexNodeBodyC &Body()
    { return static_cast<LiteralIndexNodeBodyC &>(LiteralIndexElementC::Body()); }
    //: Access body.

    const LiteralIndexNodeBodyC &Body() const 
    { return static_cast<const LiteralIndexNodeBodyC &>(LiteralIndexElementC::Body()); }
    //: Access body.

  public:
    LiteralIndexNodeC(LiteralIndexElementC &base)
      : LiteralIndexElementC(dynamic_cast<const LiteralIndexNodeBodyC *>(BodyPtr(base)))
    {}
    //: Base constructor

    UIntT Term() const
    { return Body().Term(); }
    //: Access element of tuple we're indexing.
  };
}

#endif
