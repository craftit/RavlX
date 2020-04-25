// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEXLEAF_HEADER
#define RAVLLOGIC_LITERALINDEXLEAF_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Index"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexLeaf.hh"

#include "Ravl/Logic/LiteralIndexElement.hh"

namespace RavlLogicN {

  //! userlevel=Develop
  //: LiteralIndex leaf
  
  class LiteralIndexLeafBodyC
    : public LiteralIndexElementBodyC
  {
  public:
    LiteralIndexLeafBodyC(const LiteralC &nkey)
      : key(nkey)
    {}

    const LiteralC &Key() const
    { return key; }
    //: Access key for this leaf.
    
    virtual bool Lookup(const TupleC &key,LiteralIndexElementC &next);
    //: Lookup next level in tree.
    // returns true if this level is a leaf and leave next unchanged.
    // if no suitable node is found 'next' is made invalid.
   
    virtual StringC Name();
    //: Get name for element.
    // usefull for debuging.

    virtual void Dump(ostream &out,int level = 0) const;
    //: Dump index in human readable form.
    // For debugging only.
    
    virtual LiteralMapIterC<LiteralIndexElementC> Filter(const LiteralC &key,LiteralIndexElementC &next,BindSetC &binds,LiteralC &var);
    //: Filter next level in tree.
    // If there's a choice return an iterator otherwise the next branch
    // in the tree is assigned to 'next'.  var is the variable to witch
    // the value of the iterator should be bound.


  protected:
    LiteralC key;
  };
  
  //! userlevel=Advanced
  //: Index leaf with user data.
  // Internal class for a literal index which associates
  // the user data with a leaf in the tree.
  
  class LiteralIndexLeafC
    : public LiteralIndexElementC
  {
  public:
    LiteralIndexLeafC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    LiteralIndexLeafC(const LiteralC &nkey)
      : LiteralIndexElementC(*new LiteralIndexLeafBodyC(nkey))
    {}
    //: Constructor.
   
  protected:
    LiteralIndexLeafC(LiteralIndexLeafBodyC &bod)
      : LiteralIndexElementC(bod)
    {}
    //: Body constructor
    
    LiteralIndexLeafC(const LiteralIndexLeafBodyC *bod)
      : LiteralIndexElementC(bod)
    {}
    //: Body constructor
    
  public:
    LiteralIndexLeafBodyC &Body()
    { return static_cast<LiteralIndexLeafBodyC &>(LiteralIndexElementC::Body()); }
    //: Access body.

    const LiteralIndexLeafBodyC &Body() const 
    { return static_cast<const LiteralIndexLeafBodyC &>(LiteralIndexElementC::Body()); }
    //: Access body.


    LiteralIndexLeafC(const LiteralIndexElementC &base)
      : LiteralIndexElementC(dynamic_cast<const LiteralIndexLeafBodyC *>(BodyPtr(base)))
    {}
    //: Base class constructor.
    // An invalid handle is created if base is not of this type.
    
    const LiteralC &Key() const
    { return Body().Key(); }
    //: Access key for this leaf.
    
  };
  
  //! userlevel=Develop
  //: LiteralIndex data.
  
  template<class DataT>
  class LiteralIndexLeafDataBodyC
    : public LiteralIndexLeafBodyC 
  {
  public:
    LiteralIndexLeafDataBodyC(const LiteralC &key,const DataT &dat)
      : LiteralIndexLeafBodyC(key),
	data(dat)
    {}
    //: Construct from key and data.

    LiteralIndexLeafDataBodyC(const LiteralC &key)
      : LiteralIndexLeafBodyC(key)
    {}
    //: Construct from key.
    // uses the default constructor for the data object.
    
    DataT &Data()
    { return data; }
    //: Access data.
    
    const DataT &Data() const
    { return data; }
    //: Access data.
    
  protected:
    DataT data;
  };
  
  //! userlevel=Advanced
  //: Index leaf with user data.
  // Internal class for a literal index which associates
  // the user data with a leaf in the tree.
  
  template<class DataT>
  class LiteralIndexLeafDataC
    : public LiteralIndexLeafC
  {
  public:
    LiteralIndexLeafDataC()
    {}
    //: Default constructor.
    // creates an invalid handle.

    LiteralIndexLeafDataC(const LiteralC &key,const DataT &dat)
      : LiteralIndexLeafC(*new LiteralIndexLeafDataBodyC<DataT>(key,dat))
    {}
    //: Construct from key and data.
    
    LiteralIndexLeafDataC(bool,const LiteralC &key)
      : LiteralIndexLeafC(*new LiteralIndexLeafDataBodyC<DataT>(key))
    {}
    //: Construct from key only.
    // the first bool is not used, and should be set to false. 
    
  protected:
    LiteralIndexLeafDataC(LiteralIndexLeafDataBodyC<DataT> &bod)
      : LiteralIndexLeafC(bod)
    {}
    //: Body constructor.
    
    LiteralIndexLeafDataC(const LiteralIndexLeafDataBodyC<DataT> *bod)
      : LiteralIndexLeafC(bod)
    {}
    //: Body constructor.
    
  public:
    LiteralIndexLeafDataBodyC<DataT> &Body()
    { return static_cast<LiteralIndexLeafDataBodyC<DataT> & >(LiteralIndexElementC::Body()); }
    //: Access body.
    
    const LiteralIndexLeafDataBodyC<DataT> &Body() const
    { return static_cast<const LiteralIndexLeafDataBodyC<DataT> & >(LiteralIndexElementC::Body()); }
    //: Access body.
    
    LiteralIndexLeafDataC(const LiteralIndexLeafC &base)
      : LiteralIndexLeafC(dynamic_cast<const LiteralIndexLeafDataBodyC<DataT> *>(BodyPtr(base)))
    {}
    //: Base constructor.
    // if 'base' is not a LiteralIndexLeafC, an invalid handle
    // will be created.
    
    DataT &Data()
    { return Body().Data(); }
    //: Access data.
    
    const DataT &Data() const
    { return Body().Data(); }
    //: Access data.
  };

}

#endif
