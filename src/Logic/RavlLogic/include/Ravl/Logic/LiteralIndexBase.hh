// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_LITERALINDEXBASE_HEADER
#define RAVLLOGIC_LITERALINDEXBASE_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Logic.Index"
//! author="Charles Galambos"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexBase.hh"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/LiteralIndexElement.hh"
#include "Ravl/Logic/LiteralIndexLeaf.hh"
#include "Ravl/Hash.hh"

namespace RavlLogicN {
  class LiteralIndexFilterBaseBodyC;  
  class LiteralIndexBaseIterC;
  class LiteralIndexLeafBodyIterC;
  class LiteralIndexLeafVarBodyIterC;
  
  //! userlevel=Develop
  //: Base class for index access.
  
  class LiteralIndexBaseBodyC
    : public RCBodyVC
  {
  public:
    LiteralIndexBaseBodyC();
    //: Default constructor.

    LiteralIndexBaseBodyC(BinIStreamC &strm);
    //: Stream constructor.

    virtual bool Load(BinIStreamC &strm);
    //: Save to a binary stream.
    
    virtual bool Save(BinOStreamC &strm) const;
    //: Save to a binary stream.
    
    LiteralIndexLeafC *Lookup(const LiteralC &key);
    //: Lookup exact match in index.
    // returns an invalid handle if not found.

    const LiteralIndexLeafC *Lookup(const LiteralC &key) const;
    //: Lookup exact match in index.
    // returns an invalid handle if not found.
    
    LiteralIndexLeafC Insert(const LiteralC &key);
    //: Insert key into index.
    // Will return handle to leaf if key already exists.
    
    virtual LiteralIndexLeafC NewLeaf(const LiteralC &key);
    //: Generate a new leaf.
    
    bool Del(const LiteralC &key,bool exactMatchOnly = false);
    //: Delete key from index.
    // returns true if key existed, false otherwise.
	     
    SizeT Size() const
    { return map.Size(); }
    //: Get the number of elements in the index.

    void Dump(ostream &out,int level = 0) const;
    //: Dump index in human readable form.
    
    bool IsGrounded() const
    { return indexGrounded; }
    // Is index entirely grounded.
    
  protected:
    virtual UIntT PickNextTerm(const SArray1dC<bool> &used,const LiteralC &key);
    //: Pick next term to use in the index.
    // used is an array set to true for all the terms already used.
    // 'key' is the key we're updating the index for. <p>
    // The default version of this function just uses terms in numerical order, first
    // to last.  You may want to put a more clever heuristic in here.
    
    virtual bool SaveEntry(BinOStreamC &strm,const LiteralC &key,const LiteralIndexLeafC &leaf) const;
    //: Save an entry from the index.
    
    virtual bool LoadEntry(BinIStreamC &strm);
    //: Load an entry into the index.
    
    static void SaveLiteral(BinOStreamC &strm,const LiteralC &lit);
    //: Helper to avoid including Ravl/PointerManager.hh
    
    static void LoadLiteral(BinIStreamC &strm,LiteralC &lit);
    //: Helper to avoid including Ravl/PointerManager.hh
    
    HashC<LiteralC,LiteralIndexLeafC> varMap; // Direct lookup of simple variables.
    HashC<LiteralC,LiteralIndexLeafC> map; // Direct lookup table.
    LiteralIndexElementC root; // Root of tree.
    UIntT maxArity;
    bool indexGrounded; // Is index entirely grounded.
    friend class LiteralIndexBaseIterC;
    friend class LiteralIndexFilterBaseBodyC;
    friend class LiteralIndexLeafBodyIterC;
    friend class LiteralIndexLeafVarBodyIterC;
  };
  
  //! userlevel=Develop
  //: Base class for index access.

  class LiteralIndexBaseC
    : public RCHandleC<LiteralIndexBaseBodyC>
  {
  public:
    LiteralIndexBaseC()
    {}
    //: Default constructor.
    // creates an invalid handle.

    LiteralIndexBaseC(bool)
      : RCHandleC<LiteralIndexBaseBodyC>(new LiteralIndexBaseBodyC())
    {}
    //: Creates an empty index
    
    LiteralIndexBaseC(BinIStreamC &strm)
      : RCHandleC<LiteralIndexBaseBodyC>(*new LiteralIndexBaseBodyC())
    { Load(strm); }
    //: Binary stream constructor.
    
  protected:
    LiteralIndexBaseC(LiteralIndexBaseBodyC &bod)
      : RCHandleC<LiteralIndexBaseBodyC>(bod)
    {}
    //: Body constructor.
    
    LiteralIndexBaseC(const LiteralIndexBaseBodyC *bod)
      : RCHandleC<LiteralIndexBaseBodyC>(bod)
    {}
    //: Body constructor.
    
    LiteralIndexBaseBodyC &Body()
    { return RCHandleC<LiteralIndexBaseBodyC>::Body(); }
    //: Access body.
    
    const LiteralIndexBaseBodyC &Body() const
    { return RCHandleC<LiteralIndexBaseBodyC>::Body(); }
    //: Access body.
    
    bool Load(BinIStreamC &strm)
    { return Body().Load(strm); }
    //: Save to a binary stream.
    
  public:
    bool Save(BinOStreamC &strm) const
    { return Body().Save(strm); }
    //: Save to a binary stream.
    
    LiteralIndexLeafC *Lookup(const LiteralC &key)
    { return Body().Lookup(key); }
    //: Lookup value associated with the key in the index.
    // returns an invalid handle if not found.
    
    const LiteralIndexLeafC *Lookup(const LiteralC &key) const
    { return Body().Lookup(key); }
    //: Lookup value associated with the key in the index.
    // returns an invalid handle if not found.
    
    LiteralIndexLeafC Insert(const LiteralC &key)
    { return Body().Insert(key); }
    //: Insert key into index.
    // Returns false if key already exists.
    
    SizeT Size() const
    { return Body().Size(); }
    //: Get the number of elements in the index.
    
    bool Del(const LiteralC &key,bool exactMatchOnly = false)
    { return Body().Del(key,exactMatchOnly); }
    //: Delete key from index.
    // returns true if key existed, false otherwise.
    
    void Dump(ostream &out,int level = 0) const 
    { Body().Dump(out,level); }
    //: Dump index in human readable form.
    // For debugging only.
    
    bool IsGrounded() const
    { return Body().IsGrounded(); }
    // Is index entirely grounded.
    
    friend class LiteralIndexBaseIterC;
    friend class LiteralIndexFilterBaseBodyC;
    friend class LiteralIndexLeafBodyIterC;
    friend class LiteralIndexLeafVarBodyIterC;                 
    friend class LiteralIndexBaseBodyC;
  };
  
  ostream &operator<<(ostream &strm,const LiteralIndexBaseC &index);
  //: Output to stream.
  
  istream &operator>>(istream &strm,LiteralIndexBaseC &index);
  //: Input from stream.

}


#endif
