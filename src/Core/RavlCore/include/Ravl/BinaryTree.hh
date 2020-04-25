// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BINARYTREE_HEADER
#define RAVL_BINARYTREE_HEADER 1
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Trees"
//! author="Charles Galambos"
//! lib=RavlCore
//! example=exBinaryTree.cc
//! file="Ravl/Core/Container/Trees/BinaryTree.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Math.hh"
#include "Ravl/BlkStack.hh"

#if RAVL_HAVE_TEMPLATEREQUIREALLDEFINITIONS 
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"
#endif

namespace RavlN {
  
  template<class KeyT,class DataT> class BinaryTreeBodyC;
  template<class KeyT,class DataT> class BinaryTreeUpIterC;
  template<class KeyT,class DataT> class BinaryTreeDownIterC;
  
  //! userlevel=Develop
  //: Node in binary tree.
  
  template<class KeyT,class DataT>
  class BinaryTreeNodeC {
  public:
    BinaryTreeNodeC()
      : deleted(false),
	balance(0)
    { child[0] = 0; child[1] = 0; }
    //: Constructor.

    BinaryTreeNodeC(const KeyT &nkey,const DataT &ndata,int nbalance = 0,bool ndel = false)
      : key(nkey),
	data(ndata),
	deleted(ndel),
	balance(nbalance)
    { child[0] = 0; child[1] = 0; }
    //: Constructor.
    
    ~BinaryTreeNodeC()
    { delete child[0]; delete child[1]; }
    //: Destructor.
    
    BinaryTreeNodeC<KeyT,DataT> *Copy() const;
    //: Copy this node and its child nodes.
    
    BinaryTreeNodeC *&Child0()
    { return child[0]; }
    //: Access child 0 ptr.
    
    BinaryTreeNodeC *&Child1()
    { return child[1]; }
    //: Access child 1 ptr.
    
    const KeyT &Key() const
    { return key; }
    //: Access key for this node.
    
    DataT &Data()
    { return data; }
    //: Access data at this node.

    const DataT &Data() const
    { return data; }
    //: Access data at this node.
    
    bool IsDeleted() const
    { return deleted; }
    //: Test if node has been deleted.
    
    void SetDeleted(bool val)
    { deleted = val; }
    //: Set deleted flag.
    
    IntT &Balance()
    { return balance; }
    //: Access balance counter.
    // This is used be derived classes for
    // keeping the tree balanced, its not used
    // for simple binary tree's.

    IntT ChildBalance0() const {
      if(child[0] == 0)
	return -1;
      return child[0]->Balance();
    }
    //: Get the balance of the child 0.
    // if no child exists, the balance is -1.
    
    IntT ChildBalance1() const {
      if(child[1] == 0)
	return -1;
      return child[1]->Balance();
    }
    //: Get the balance of the child 1.
    // if no child exists, the balance is -1.

    void Dump(std::ostream &out,int level = 0);
    //: Dump node in human readable format.
  protected:
    KeyT key;
    DataT data;
    bool deleted;
    BinaryTreeNodeC<KeyT,DataT> *child[2]; // 0=Lower, 1= higher  values
    // FIXME:- Balance is currently only used in AVLTrees, 
    // it could be moved to a derived class used by them.
    IntT balance; 
  };
  
  //! userlevel=Develop
  //: Binary tree body.
  // This class implements a simple binary tree. NB. No attempt
  // to balance the tree is made so only use this class if you
  // are sure data will be inserted in a random order.
  
  template<class KeyT,class DataT>
  class BinaryTreeBodyC 
    : public RCBodyC
  {
  public:
    BinaryTreeBodyC()
      : root(0),
	size(0)
    {}
    //: Constructor
    
    virtual ~BinaryTreeBodyC()
    { delete root;  }
    //: Destructor.
    
    RCBodyC &Copy() const { 
      if(size == 0)
	return *new BinaryTreeBodyC<KeyT,DataT>(); 
      return *new BinaryTreeBodyC<KeyT,DataT>(root->Copy(),size); 
    }
    //: Make copy of tree.
    
    void Empty()
    { delete root; root = 0; size = 0; }
    //: Empty the tree of all contents.
    
    SizeT Size() const
    { return size; }
    //: Number of elements in tree.
    
    bool IsEmpty() const 
    { return size == 0; } 
    //: Is tree empty ?
    
    virtual bool Find(const KeyT &key,DataT &result);
    //: Find a data item with 'key' in the tree.
    // return true if found, false otherwise.
    
    DataT &MinData();
    //: Get the data with smallest key in the tree.
    // Note, the tree must NOT be empty, this can be checked with IsEmpty().
    
    DataT &MaxData();
    //: Get the data at the largest key in the tree.
    // Note, the tree must NOT be empty, this can be checked with IsEmpty().

    const KeyT &MinKey() const;
    //: Get the smallest key in the tree.
    // Note, the tree must NOT be empty, this can be checked with IsEmpty().
    
    const KeyT &MaxKey() const;
    //: Get the largest key in the tree.
    // Note, the tree must NOT be empty, this can be checked with IsEmpty().
    
    virtual bool Remove(const KeyT &key);
    //: Remove an item from tree.
    
    virtual bool Insert(const KeyT &key,const DataT &dat,bool overwrite = true);
    //: Insert a node into the tree.
    // If 'overwrite' is true, and a node with the given
    // key is already in the tree then it will be overwritten
    // otherwise 'false' will be returned.
    
    void Dump(std::ostream &out) {
      if(root == 0)
	out << "(NIL)";
      else
	root->Dump(out,0);
      out << "\n";
    }
    //: Dump the stream in a human readable fomat.
    // Usefull for debuging.
    
  protected:
    BinaryTreeBodyC(BinaryTreeNodeC<KeyT,DataT> *nroot,SizeT nsize)
      : root(nroot),
	size(nsize)
    {}
    //: Constructor
    
    BinaryTreeNodeC<KeyT,DataT> *MinNode() const;
    //: Get the node with the smallest key.
    
    BinaryTreeNodeC<KeyT,DataT> *MaxNode() const;
    //: Get the node with the largest key.
    
    BinaryTreeNodeC<KeyT,DataT> *FindNode(const KeyT &key);
    //: Find a node in the tree.
    
    BinaryTreeNodeC<KeyT,DataT> *root;
    SizeT size;
    
    friend class BinaryTreeUpIterC<KeyT,DataT>;
    friend class BinaryTreeDownIterC<KeyT,DataT>;
  };  
  
  //! userlevel=Normal
  //: Binary tree.
  // This class implements a simple binary tree.  No attempt
  // to balance the tree is made so only use this class if you
  // are sure data will be inserted in a random order. Otherwise
  // use AVLTreeC which attempts to keep the tree balanced.
  
  template<class KeyT,class DataT>
  class BinaryTreeC 
    : public RCHandleC<BinaryTreeBodyC<KeyT,DataT> >
  {
  public:
    BinaryTreeC()
    {}
    //: Default constructor.
    // NB. Creates an invalid handle.
    
    explicit BinaryTreeC(bool)
      : RCHandleC<BinaryTreeBodyC<KeyT,DataT> >(*new BinaryTreeBodyC<KeyT,DataT>())
    {}
    //: Default constructor.
    // NB. Creates an invalid handle.
    
  protected:
    BinaryTreeC(BinaryTreeBodyC<KeyT,DataT> &body)
      : RCHandleC<BinaryTreeBodyC<KeyT,DataT> >(body)
    {}
    //: Body constructor.
    
    BinaryTreeBodyC<KeyT,DataT> &Body()
    { return RCHandleC<BinaryTreeBodyC<KeyT,DataT> >::Body(); }
    //: Access body.

    const BinaryTreeBodyC<KeyT,DataT> &Body() const
    { return RCHandleC<BinaryTreeBodyC<KeyT,DataT> >::Body(); }
    //: Access body.
    
  public:
    BinaryTreeC Copy() const 
    { return  BinaryTreeC(static_cast<BinaryTreeBodyC<KeyT,DataT> &>(Body().Copy())); }
    //: Make a copy of this tree.
    
    void Empty()
    { Body().Empty(); }
    //: Empty the tree of all contents.
    
    SizeT Size() const
    { return Body().Size(); }
    //: Number of elements in tree.
    
    bool IsEmpty() const 
    { return Body().IsEmpty(); } 
    //: Is tree empty ?
    
    bool Find(const KeyT &key,DataT &result)
    { return Body().Find(key,result); }
    //: Find a data item with 'key' in the tree.
    // return true if found, false otherwise.
    
    bool Remove(const KeyT &key)
    { return Body().Remove(key); }
    //: Remove item from tree.
    
    bool Insert(const KeyT &key,const DataT &dat,bool overwrite = true)
    { return Body().Insert(key,dat,overwrite); }
    //: Insert a node into the tree.
    // If 'overwrite' is true, and a node with the given
    // key is already in the tree then it will be overwritten
    // otherwise 'false' will be returned.

    DataT &MinData()
    { return Body().MinData(); }
    //: Get the data with smallest key in the tree.
    // Note, the tree must NOT be empty, this can be checked with IsEmpty().
    
    DataT &MaxData()
    { return Body().MaxData(); }
    //: Get the data at the largest key in the tree.
    // Note, the tree must NOT be empty, this can be checked with IsEmpty().

    const KeyT &MinKey() const
    { return Body().MinKey(); }
    //: Get the smallest key in the tree.
    // Note, the tree must NOT be empty, this can be checked with IsEmpty().
    
    const KeyT &MaxKey() const
    { return Body().MaxKey(); }
    //: Get the largest key in the tree.
    // Note, the tree must NOT be empty, this can be checked with IsEmpty().
    
    void Dump(std::ostream &out)
    { Body().Dump(out); }
    //: Dump the stream in a human readable fomat.
    // Usefull for debuging.
    
    friend class BinaryTreeUpIterC<KeyT,DataT>;
    friend class BinaryTreeDownIterC<KeyT,DataT>;
  };
  
  template<class KeyT,class DataT>
  void BinaryTreeNodeC<KeyT,DataT>::Dump(std::ostream &out,int level) {
    if(child[0] == 0) {
      for(int i = 0;i < level;i++)
	out << ' ';
      out << " (NULL)\n";
    } else
      child[0]->Dump(out,level+1);
    for(int i = 0;i < level;i++)
      out << ' ';
    out << "Key=" << key << " Data=" << data << " Del=" << ((int) deleted) <<'\n';
    if(child[1] == 0) {
      for(int i = 0;i < level;i++)
	out << ' ';
      out << " (NULL)\n";
    } else
      child[1]->Dump(out,level+1);
  }

  template<class KeyT,class DataT>
  BinaryTreeNodeC<KeyT,DataT> *BinaryTreeNodeC<KeyT,DataT>::Copy() const { 
    BinaryTreeNodeC<KeyT,DataT> *ret = new BinaryTreeNodeC<KeyT,DataT>(key,data,balance,deleted);
    if(child[0] != 0)
      ret->Child0()= child[0]->Copy();
    if(child[1] != 0)
      ret->Child1()= child[1]->Copy();
    return ret;
  }
  

  template<class KeyT,class DataT>
  BinaryTreeNodeC<KeyT,DataT> *BinaryTreeBodyC<KeyT,DataT>::FindNode(const KeyT &key) {
    BinaryTreeNodeC<KeyT,DataT> *place = root;
    while(place != 0) {
      if(key < place->Key()) {
	place = place->Child0();
	continue;
      }
      if(key > place->Key()) {
	place = place->Child1();
	continue;
      }
      break;
    }
    return place;
  }
  
  template<class KeyT,class DataT>
  bool BinaryTreeBodyC<KeyT,DataT>::Find(const KeyT &key,DataT &result) {
    BinaryTreeNodeC<KeyT,DataT> *place = FindNode(key);
    if(place == 0)
      return false;
    if(place->IsDeleted())
      return false;
    result = place->Data();
    return true;
  }
  
  //: Remove item from tree.
  
  template<class KeyT,class DataT>
  bool BinaryTreeBodyC<KeyT,DataT>::Remove(const KeyT &key) {
    BinaryTreeNodeC<KeyT,DataT> *place = root;
    BlkStackC<BinaryTreeNodeC<KeyT,DataT> *> stk;
    bool ret;
    for(;;) {
      if(place == 0)
	return false; // Key not found, return false;
      if(key < place->Key()) {
	stk.Push(place);
	place = place->Child0();
	continue;
      }
      if(key > place->Key()) {
	stk.Push(place);
	place = place->Child1();
	continue;
      }
      break; // Found it!
    }
    ret = !place->IsDeleted();
    if(ret) size--;
    place->SetDeleted(true);
    // Go back through path to node deleting as much as we can easly.
    // This is necissary to ensure MaxNode and MinNode work  properly.
    do {
      BinaryTreeNodeC<KeyT,DataT> *oth;
      //cerr << "Trying to remove " << place->Key() << "\n";
      if(place->Child0() == 0) {
	oth = place->Child1();
	place->Child1() = 0; // Stops deletion when place is deleted.
      } else if(place->Child1() == 0) {
	oth = place->Child0();
	place->Child0() = 0; // Stops deletion when place is deleted. 
      } else 
	break; // Both are set, just leave it.
      if(stk.IsEmpty()) {
	root = oth;
	break;
      }
      if(stk.Top()->Child0() == place)
	stk.Top()->Child0() = oth;
      else 
	stk.Top()->Child1() = oth;
      delete place;
      place = stk.Top();
      if(!place->IsDeleted())
	break; // Node not deleted, nothing more to do.
      stk.DelTop();
    } while(1);
    
    return ret;
  }

  //: Insert a node into the tree.
  
  template<class KeyT,class DataT>
  bool BinaryTreeBodyC<KeyT,DataT>::Insert(const KeyT &key,const DataT &dat,bool overwrite) {
    BinaryTreeNodeC<KeyT,DataT> *place = root;
    BinaryTreeNodeC<KeyT,DataT> *next;
    if(root == 0)
      root = new BinaryTreeNodeC<KeyT,DataT>(key,dat);
    else {
      for(;;) {
	if(key < place->Key()) {
	  next = place->Child0();
	  if(next == 0) {
	    place->Child0() = new BinaryTreeNodeC<KeyT,DataT>(key,dat);
	    break;
	  }
	  place = next;
	  continue;
	}
	if(key > place->Key()) {
	  next = place->Child1();
	  if(next == 0) {
	    place->Child1() = new BinaryTreeNodeC<KeyT,DataT>(key,dat);
	    break;
	  }
	  place = next;
	  continue;
	}
	if(!overwrite && !place->IsDeleted()) 
	  return false;
	place->Data() = dat;
	place->SetDeleted(false);
	return true;
      }
    }
    size++;
    return true;
  }

  //: Get the node with the smallest key.
  template<class KeyT,class DataT>
  BinaryTreeNodeC<KeyT,DataT> *BinaryTreeBodyC<KeyT,DataT>::MinNode() const {
    BinaryTreeNodeC<KeyT,DataT> *next,*place = root;
    while(place != 0) {
      next = place->Child0();
      if(next == 0) {
	if(!place->IsDeleted())
	  break;
	next = place->Child1();
	RavlAssert(next != 0); // Remove() should garantee this.
      }
      place = next;
    }
    return place; 
  }
  
  //: Get the node with the largest key.
  template<class KeyT,class DataT>
  BinaryTreeNodeC<KeyT,DataT> *BinaryTreeBodyC<KeyT,DataT>::MaxNode() const {
    BinaryTreeNodeC<KeyT,DataT> *next,*place = root;
    while(place != 0) {
      next = place->Child1();
      if(next == 0) {
	if(!place->IsDeleted())
	  break;
	next = place->Child0();
	RavlAssert(next != 0); // Remove() should garantee this.
      }
      place = next;
    }
    return place; 
  }
  
  template<class KeyT,class DataT>
  DataT &BinaryTreeBodyC<KeyT,DataT>::MinData() {
    BinaryTreeNodeC<KeyT,DataT> *place = MinNode();
    RavlAssertMsg(place != 0,"BinaryTreeC::MinData() must not be empty.");
    return place->Data();     
  }
  
  template<class KeyT,class DataT>
  DataT &BinaryTreeBodyC<KeyT,DataT>::MaxData() {
    BinaryTreeNodeC<KeyT,DataT> *place = MaxNode();
    RavlAssertMsg(place != 0,"BinaryTreeC::MaxData() must not be empty.");
    return place->Data();     
  }

  template<class KeyT,class DataT>
  const KeyT &BinaryTreeBodyC<KeyT,DataT>::MinKey() const {
    BinaryTreeNodeC<KeyT,DataT> *place = MinNode();
    RavlAssertMsg(place != 0,"BinaryTreeC::MinData() must not be empty.");
    return place->Key();     
  }
  
  template<class KeyT,class DataT>
  const KeyT &BinaryTreeBodyC<KeyT,DataT>::MaxKey() const {
    BinaryTreeNodeC<KeyT,DataT> *place = MaxNode();
    RavlAssertMsg(place != 0,"BinaryTreeC::MaxData() must not be empty.");
    return place->Key();     
  }
  
  
}

#endif
