// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexBase.cc"

#include "Ravl/Logic/LiteralIndexBase.hh"
#include "Ravl/Logic/LiteralIndexNode.hh"
#include "Ravl/Logic/LiteralIndexFilterBase.hh"
#include "Ravl/Logic/Tuple.hh"
#include "Ravl/Logic/Unify.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Stack.hh"
#include "Ravl/DArray1dIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {

  //: Default constructor.
  
  LiteralIndexBaseBodyC::LiteralIndexBaseBodyC()
    : maxArity(1),
      indexGrounded(true)
  {}
  
  LiteralIndexBaseBodyC::LiteralIndexBaseBodyC(BinIStreamC &strm)
  : maxArity(1),
    indexGrounded(true)
  {
    RavlIssueError("not implemented");
  }
  //: Stream constructor.

  //: Load from a binary stream.
  
  bool LiteralIndexBaseBodyC::Load(BinIStreamC &strm) {
    IntT version;
    UIntT size;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("LiteralIndexBaseBodyC::LiteralIndexBaseBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> indexGrounded >> size ;
    for(UIntT i = 0;i < size;i++)
      LoadEntry(strm);
    return true;
  }

  //: Save to a binary stream.
  
  bool LiteralIndexBaseBodyC::Save(BinOStreamC &strm) const {
    IntT version = 0;
    UIntT size = map.Size();
    strm << version << indexGrounded << size;
    for(HashIterC<LiteralC,LiteralIndexLeafC>  hit(map);hit;hit++)
      SaveEntry(strm,hit.Key(),hit.Data());
    return true;
  }
  
  //: Save an entry from the index.
  
  bool LiteralIndexBaseBodyC::SaveEntry(BinOStreamC &strm,const LiteralC &key,const LiteralIndexLeafC &) const {
    strm << ObjIO(key);
    return true;
  }
  
  //: Load an entry into the index.
  
  bool LiteralIndexBaseBodyC::LoadEntry(BinIStreamC &strm) {
    LiteralC key;
    strm >> ObjIO(key);
    Insert(key);
    return true;
  }
  
  //: Helper to avoid including Ravl/PointerManager.hh
  
  void LiteralIndexBaseBodyC::SaveLiteral(BinOStreamC &strm,const LiteralC &lit)
  { strm << ObjIO(lit); }
  
  //: Helper to avoid including Ravl/PointerManager.hh
  
  void LiteralIndexBaseBodyC::LoadLiteral(BinIStreamC &strm,LiteralC &lit)
  { strm >> ObjIO(lit); } 
  
  //: Lookup value associated with the key in the index.
  // return an invalid handle if not found.
  
  LiteralIndexLeafC *LiteralIndexBaseBodyC::Lookup(const LiteralC &key) {
    return map.Lookup(key);
  }

  //: Lookup value associated with the key in the index.
  // return an invalid handle if not found.
  
  const LiteralIndexLeafC *LiteralIndexBaseBodyC::Lookup(const LiteralC &key) const {
    return map.Lookup(key);
  }
  
  //: Generate a new leaf.
  
  LiteralIndexLeafC LiteralIndexBaseBodyC::NewLeaf(const LiteralC &key) {
    //RavlAssertMsg(0,"ERROR: Default routine not overriden. ");
    return LiteralIndexLeafC(key);
  }
  
  //: Pick next term to use in the index.
  // used is an array set to true for all the terms already used.
  // 'key' is the key we're updating the index for.
  
  UIntT LiteralIndexBaseBodyC::PickNextTerm(const SArray1dC<bool> &used,const LiteralC &key) {
    for(SArray1dIterC<bool> it(used);it ;it++) {
      if(*it) 
	continue;
      return it.Index().V();
    }
    cerr << "Used:" << used << " Key=" << key << "\n";
    RavlAssertMsg(0,"LiteralIndexBaseBodyC::PickNextTerm(), I'm confused! Out of terms to index with. "); 
    return 0;
  }
  
  //: Insert key into index.
  // Returns false if key already exists.
  
  LiteralIndexLeafC LiteralIndexBaseBodyC::Insert(const LiteralC &key) {
    ONDEBUG(cerr << "LiteralIndexBaseBodyC::Insert() Key=" << key.Name() << "\n");
    if(!key.IsGrounded())
      indexGrounded = false;
    TupleC tuple(key);
    if(!tuple.IsValid()) {
      LiteralIndexLeafC &elem = map[key];
      if(!elem.IsValid())
	elem = NewLeaf(key);
      if(key.IsVariable())
	varMap[key] = elem;
      RavlAssert(elem.IsValid());
      return elem;
    }
    // Do a quick check to see if its here already.
    LiteralIndexLeafC elem;
    if(map.Lookup(key,elem)) {
      ONDEBUG(cerr << "LiteralIndexBaseBodyC::Insert(), Key '" << key << "' is already in index. \n");
      return elem;
    }
    // There's no identical so add one.
    if(tuple.Arity() > maxArity)
      maxArity = tuple.Arity();
    SArray1dC<bool> used(maxArity); // Set of used index's in tuple.
    used.Fill(false);
    
    LiteralIndexElementC last,place;
    place = root;
    do {
      ONDEBUG(cerr << "LiteralIndexBaseBodyC::Insert(), Inital Place=" << place.Name() << "\n");
      while(place.IsValid()) {
	LiteralIndexNodeC lin(place);
	if(!lin.IsValid()) 
	  break; // Its a leaf...
	used[lin.Term()] = true; // Mark as used.
	last = place;
	place.Lookup(tuple,place);
      }
      if(!place.IsValid()) { // No leaf in the way, just add a new one.
	LiteralIndexLeafC lil = NewLeaf(key);
	RavlAssert(lil.IsValid());
	map[key] = lil; // Put it in the map as well.
	if(last.IsValid())
	  last.Insert(tuple,lil);
	else
	  root = lil;
	return lil;
      }
      // There's a leaf that doesn't match the new key in the tree.
      ONDEBUG(cerr << "LiteralIndexBaseBodyC::Insert(), Leaf Place=" << place.Name() << "\n");
      LiteralIndexLeafC lil(place);
      RavlAssert(lil.IsValid()); 
      // Is the leaf the one we want to insert ?
      if(lil.Key() == key) {
	ONDEBUG(cerr << "Matching key found. Key=" << key.Name() << "\n");
	return lil; // Leaf has the correct key, return it.
      }
      // We have to add a new node.
      ONDEBUG(cerr << "Adding new node for key " << key.Name() << "\n");
      // Find a term to index on.
      UIntT nInd = PickNextTerm(used,key);
      RavlAssert(!used[nInd]); // Check its sane.
      
      // Creat new branch in the tree.
      LiteralIndexNodeC newNode(nInd);
      if(!last.IsValid())
	root = newNode;
      else
	last.Insert(tuple,newNode); // Replace old leaf with new node.
      // Put the old leaf into the new branch.
      newNode.Insert(TupleC(lil.Key()),place);
      // Go around again and try and add a leaf to the new node.
      place = newNode;
    } while(1);
    RavlAssertMsg(0,"Should never get here. ");
    //: Should never get this far.
    return LiteralIndexLeafC();
  }
  
  //: Delete key from index.
  // returns true if key existed, false otherwise.
  
  bool LiteralIndexBaseBodyC::Del(const LiteralC &key,bool exactMatchOnly) {
    LiteralIndexLeafC elem;
    ONDEBUG(cerr << "LiteralIndexBaseBodyC::Del(), Key '" << key.Name() << "' Exact=" << exactMatchOnly << " \n");
    if(!exactMatchOnly) {
      // Look for all possible matches.
      DArray1dC<LiteralC> delSet; 
      for(LiteralIndexFilterBaseBodyC it(*this,key);it.IsElm();it.Next())
	delSet.Append(it.Data().Key());
      bool ret = false;
      for(DArray1dIterC<LiteralC> itds(delSet);itds;itds++)
	ret = ret || Del(*itds,true);
      return ret;
    }
    
    if(!map.Lookup(key,elem)) {
      ONDEBUG(cerr << "LiteralIndexBaseBodyC::Del(), Key '" << key.Name() << "' is not in index \n");
      return false;
    }
    map.Del(key);
    TupleC tkey(key);
    if(!tkey.IsValid()) {
      ONDEBUG(cerr << "LiteralIndexBaseBodyC::Del(), Key '" << key.Name() << "' not a tuple \n");
      return true;// Not a tuple. Easy!
    }
    
    StackC<LiteralIndexElementC> history;
    LiteralIndexElementC place,next;
    place = root;
    RavlAssert(place.IsValid());
    // Go through tree looking for leaf node.
    while(!place.Lookup(tkey,next)) {
      if(!next.IsValid())
	break;
      ONDEBUG(cerr << "  Push " << tkey.Name() << " from " << place.Hash() << " Size=" << place.Size() << "\n");
      history.Push(place);
      place = next;
      next.Invalidate();
    }
    // Go back through stack tidying up
    while(!history.IsEmpty()) {
      next = history.GetFirst();
      ONDEBUG(cerr << "  Deleting " << tkey.Name() << " from " << next.Hash() << " Size=" << place.Size() << "\n");
      next.Del(tkey); // Delete sub node from tree
      if(next.Size() > 0) // Does this node need deleting ?
	break;
    }
    if(root.Size() == 0) {
      ONDEBUG(cerr << "  Deleting root.\n");
      root.Invalidate(); // root node not needed
    }
    return true;
  }

  //: Dump index in human readable form.
  
  void LiteralIndexBaseBodyC::Dump(ostream &out,int level) const {
    out << Indent(level) << "IndexBase:" << ((void *) this) << " \n";
    for(HashIterC<LiteralC,LiteralIndexLeafC> it(map);it;it++) {
      TupleC tup(it.Key());
      if(tup.IsValid())
	continue;
      //out << Indent(level+1) << "Literal " << it.Key().Name() << " \n";
      it.Data().Dump(out,level+2);
    }
    if(root.IsValid())
      root.Dump(out,level+1);
  }
  
  ostream &operator<<(ostream &strm,const LiteralIndexBaseC &index) {
    index.Dump(strm);
    return strm;
  }
  //: Output to stream.
  
  istream &operator>>(istream &strm,LiteralIndexBaseC &index) {
    RavlAssert(0);    
    return strm;
  }
  //: Input from stream.
  
}
