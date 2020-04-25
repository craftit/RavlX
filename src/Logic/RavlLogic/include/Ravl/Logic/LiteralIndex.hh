// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEX_HEADER
#define RAVLLOGIC_LITERALINDEX_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! docentry="Ravl.API.Logic.Index"
//! file="Ravl/Logic/Index/LiteralIndex.hh"
//! author="Charles Galambos"

#include "Ravl/Logic/LiteralIndexBase.hh"
#include "Ravl/Logic/LiteralIndexLeaf.hh"
#include "Ravl/BinStream.hh"

namespace RavlLogicN {
   
  template<class DataT> class LiteralIndexIterC;
  
  //! userlevel=Develop
  //: Literal index.
  
  template<class DataT>
  class LiteralIndexBodyC
    : public LiteralIndexBaseBodyC
  {
  public:
    LiteralIndexBodyC()
    {}
    //: Default constructor.
    
    LiteralIndexBodyC(BinIStreamC &strm)
      : LiteralIndexBaseBodyC(strm)
    {}
    //: Binary stream constructor.
    
  protected:

    virtual LiteralIndexLeafC NewLeaf(const LiteralC &key)
    { return LiteralIndexLeafDataC<DataT>(true,key); }
    //: Generate a new leaf.
    
    virtual bool SaveEntry(BinOStreamC &strm,const LiteralC &key,const LiteralIndexLeafC &leaf) const {
      LiteralIndexLeafDataC<DataT> val(leaf);
      SaveLiteral(strm,key);
      strm << val.Data();
      return true;
    }
    //: Save an entry from the index.
    
    virtual bool LoadEntry(BinIStreamC &strm) {
      DataT val;
      LiteralC key;
      LoadLiteral(strm,key);
      strm >> val;
      return Insert(key,val); 
    }
    //: Load an entry into the index.
    
  public:
    bool Lookup(const LiteralC &key,DataT &val) {
      LiteralIndexLeafC *leaf = LiteralIndexBaseBodyC::Lookup(key);
      if(leaf == 0) return false;
      RavlAssert(leaf->IsValid());
      LiteralIndexLeafDataBodyC<DataT> *t = dynamic_cast<LiteralIndexLeafDataBodyC<DataT> *>(&leaf->Body());
      RavlAssert(t != 0);
      val = t->Data();
      return true;
    }
    //: Lookup value associated with the key in the index.
    //  if cound copy the value to 'val' and true is return.
    
    DataT *Lookup(const LiteralC &key) {
      LiteralIndexLeafC *leaf = LiteralIndexBaseBodyC::Lookup(key);
      if(leaf == 0 ) return 0;
      RavlAssert(leaf->IsValid());
      LiteralIndexLeafDataBodyC<DataT> *t = dynamic_cast<LiteralIndexLeafDataBodyC<DataT> *>(&leaf->Body());
      RavlAssert(t != 0);
      return &t->Data();
    }
    //: Lookup value associated with the key in the index.
    //  if cound copy the value to 'val' and true is return.

    const DataT *Lookup(const LiteralC &key) const {
      const LiteralIndexLeafC *leaf = LiteralIndexBaseBodyC::Lookup(key);
      if(leaf == 0) return 0;
      RavlAssert(leaf->IsValid());
      const LiteralIndexLeafDataBodyC<DataT> *t = dynamic_cast<const LiteralIndexLeafDataBodyC<DataT> *>(&leaf->Body());
      RavlAssert(t != 0);
      return &t->Data();
    }
    //: Lookup value associated with the key in the index.
    //  if cound copy the value to 'val' and true is return.
    
    bool Insert(const LiteralC &key,const DataT &data) {
      LiteralIndexLeafDataC<DataT> t(LiteralIndexBaseBodyC::Insert(key));
      RavlAssert(t.IsValid());
      t.Data() = data;
      return true;
    }
    //: Insert data with given key into index.
    // Returns false if key already exists.
    
    DataT &Access(const LiteralC &key) {
      LiteralIndexLeafDataC<DataT> t(LiteralIndexBaseBodyC::Insert(key));
      RavlAssert(t.IsValid());
      return t.Data();
    }
    //: Access data associated with a literal.
    
    
  };
  
  //! userlevel=Normal
  //! autolink="on"
  //: Literal index.
  // This is an index of 'DataT's based on a Literal key. It maybe queried directly
  // with 'Lookup' or iterated through for data items matching a pattern with a 
  // 'LiteralIndexFilterC'
  
  template<class DataT>
  class LiteralIndexC
    : public LiteralIndexBaseC
  {
  public:
    LiteralIndexC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit LiteralIndexC(bool)
      : LiteralIndexBaseC(*new LiteralIndexBodyC<DataT>())
    {}
    //: Constructor.
    
    explicit LiteralIndexC(BinIStreamC &strm)
      : LiteralIndexBaseC(*new LiteralIndexBodyC<DataT>())
    { Load(strm); }
    //: Constructor.
    
  protected:
    LiteralIndexC(LiteralIndexBodyC<DataT> &bod)
      : LiteralIndexBaseC(bod)
    {}
    //: Body constructor.
    
    LiteralIndexC(const LiteralIndexBodyC<DataT> *bod)
      : LiteralIndexBaseC(bod)
    {}
    //: Body constructor.
    
    LiteralIndexBodyC<DataT> &Body()
    { return static_cast<LiteralIndexBodyC<DataT> &>(LiteralIndexBaseC::Body()); }
    //: Access body.

    const LiteralIndexBodyC<DataT> &Body() const
    { return static_cast<const LiteralIndexBodyC<DataT> &>(LiteralIndexBaseC::Body()); }
    //: Access body.
    
  public:
    bool Lookup(const LiteralC &key,DataT &val)
    { return Body().Lookup(key,val); }
    //: Lookup value associated with the key in the index.
    //  if cound copy the value to 'val' and true is return.
    
    DataT *Lookup(const LiteralC &key)
    { return Body().Lookup(key); }
    //: Lookup value associated with the key in the index.
    //  if cound copy the value to 'val' and true is return.
    
    const DataT *Lookup(const LiteralC &key) const
    { return Body().Lookup(key); }
    //: Lookup value associated with the key in the index.
    //  if cound copy the value to 'val' and true is return.
    
    bool Insert(const LiteralC &key,const DataT &leaf)
    { return Body().Insert(key,leaf); }
    //: Insert data with given key into index.
    // Returns false if key already exists.


    DataT &Access(const LiteralC &key) 
    { return Body().Access(key); }
    //: Access data associated with a literal.
    
    DataT &operator[](const LiteralC &key)
    { return Body().Access(key); }
    //: Access data associated with a literal.
    
    friend class LiteralIndexIterC<DataT>;
  };
  
  template<class DataT>  
  ostream &operator<<(ostream &strm,const LiteralIndexC<DataT> &index) { 
    strm << ((const LiteralIndexBaseC &) index); 
    return strm;
  }
  //: Output to stream.
  
  template<class DataT>  
  istream &operator>>(istream &strm,LiteralIndexC<DataT> &index) {
    strm >> ((LiteralIndexBaseC &) index); 
    return strm;
  }
  //: Input from stream.

  template<class DataT>  
  BinOStreamC &operator<<(BinOStreamC &strm,const LiteralIndexC<DataT> &index) { 
    index.Save(strm);
    return strm;
  }
  //: Output to binary stream.
  
  template<class DataT>  
  BinIStreamC &operator>>(BinIStreamC &strm,LiteralIndexC<DataT> &index) {
    index = LiteralIndexC<DataT>(strm);
    return strm;
  }
  //: Input from binary stream.
  
}


#endif
