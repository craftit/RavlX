// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEXELEM_HEADER
#define RAVLLOGIC_LITERALINDEXELEM_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Index"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexElement.hh"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/LiteralMapIter.hh"

namespace RavlLogicN 
{
  class TupleC;
  class LiteralIndexLeafC;
  class LiteralIndexElementC;

  //! userlevel=Develop
  //: Index element.
  // This class is an abstract base class for the two types of
  // node found in an index tree, LiteralIndexNodeC and LiteralIndexLeafC.
  
  class LiteralIndexElementBodyC 
    : public RCBodyVC 
  {
  public:
    LiteralIndexElementBodyC()
    {}
    //: Default constructor.
    
    virtual StringC Name();
    //: Get name for element.
    // usefull for debuging.
    
    virtual bool Lookup(const TupleC &key,LiteralIndexElementC &next);
    //: Lookup next level in tree.
    // returns true if this level is a leaf and leave next unchanged.
    // if no suitable node is found 'next' is made invalid.
    
    virtual bool Del(const TupleC &key);
    //: Delete branch with index 'key'.
    
    virtual bool Insert(const TupleC &key,LiteralIndexElementC &next);
    //: Insert/Replace element into the node.
    
    virtual LiteralMapIterC<LiteralIndexElementC> Filter(const LiteralC &key,
                                                         LiteralIndexElementC &next,
                                                         BindSetC &binds,
                                                         LiteralC &var
                                                         );
    //: Filter next level in tree.
    // If there's a choice return an iterator otherwise the next branch
    // in the tree is assigned to 'next'.  var is the variable to witch
    // the value of the iterator should be bound.
    
    virtual UIntT Size() const;
    //: Return the size of the index.
    // The number of branches from this node is returned.
    
    virtual void Dump(ostream &out,int level = 0) const;
    //: Dump index in human readable form.
    // For debugging only.
    
    virtual LiteralMapIterC<LiteralIndexElementC> Iterator();
    //: Access iterator for all branches from this node.
    
    virtual LiteralMapIterC<LiteralIndexElementC>  Filter(const LiteralC &filt);
    //: Access iterator for all branches from this node matching filter filt.
    
  };
  
  //! userlevel=Advanced
  //: Index element.
  // This class is an abstract base class for the two types of
  // node found in an index tree, LiteralIndexNodeC and LiteralIndexLeafC.
  
  class LiteralIndexElementC 
    : public RCHandleC<LiteralIndexElementBodyC> 
  {
  public:
    LiteralIndexElementC()
    {}
    //: Default constructor.
    // This creates an invalid handle.
  
  protected:
    LiteralIndexElementC(LiteralIndexElementBodyC &bod)
      : RCHandleC<LiteralIndexElementBodyC>(bod)
    {}
    //: Body constructor.
    
    LiteralIndexElementC(const LiteralIndexElementBodyC *bod)
      : RCHandleC<LiteralIndexElementBodyC>(bod)
    {}
    //: Body constructor.
    
    LiteralIndexElementBodyC &Body()
    { return RCHandleC<LiteralIndexElementBodyC>::Body(); }
    //: Access body.

    const LiteralIndexElementBodyC &Body() const
    { return RCHandleC<LiteralIndexElementBodyC>::Body(); }
    //: Access body.
    
  public:
    
    StringC Name() { 
      if(!IsValid())
	return StringC("-NIL-");
      return Body().Name(); 
    }
    //: Get name for element.
    // usefull for debuging.
    
    bool Lookup(const TupleC &key,LiteralIndexElementC &next)
    { return Body().Lookup(key,next); }
    //: Lookup next level in tree.
    // returns true if this level is a leaf and leave next unchanged.
    // if no suitable node is found 'next' is made invalid.
    
    bool Del(const TupleC &key)
    { return Body().Del(key); }
    //: Delete branch with index 'key'.
    
    LiteralMapIterC<LiteralIndexElementC> Filter(const LiteralC &key,LiteralIndexElementC &next,BindSetC &binds,LiteralC &var)
    { return Body().Filter(key,next,binds,var); }
    //: Filter next level in tree.
    // If there's a choice return an iterator otherwise the next branch
    // in the tree is assigned to 'next'.  var is the variable to witch
    // the value of the iterator should be bound.
    
    bool Insert(const TupleC &key,LiteralIndexElementC &next) 
    { return Body().Insert(key,next); }
    //: Insert/Replace element into the node.
    
    UIntT Size() const
    { return Body().Size(); }
    //: Return the size of the index.
    // The number of branches from this node is returned.
    
    void Dump(ostream &out,int level = 0) const
    { Body().Dump(out,level); }
    //: Dump index in human readable form.
    // For debugging only.
    
    LiteralMapIterC<LiteralIndexElementC> Iterator() 
    { return Body().Iterator(); }
    //: Access iterator for all branches from this node.
    
    LiteralMapIterC<LiteralIndexElementC>  Filter(const LiteralC &filt) 
    { return Body().Filter(filt); }
    //: Access iterator for all branches from this node matching filter ex.
  };
  
  BinOStreamC &operator<<(BinOStreamC &strm,const LiteralIndexElementC &index);
  //: Output to binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,LiteralIndexElementC &index);
  //: Input from binary stream.

  
}


#endif
