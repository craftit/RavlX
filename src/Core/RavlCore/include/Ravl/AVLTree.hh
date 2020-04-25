// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAVLTREE_HEADER
#define RAVLAVLTREE_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Trees"
//! author="Charles Galambos"
//! lib=RavlCore
//! example=exAVLTree.cc
//! file="Ravl/Core/Container/Trees/AVLTree.hh"

#include "Ravl/BinaryTree.hh"

namespace RavlN {

#define RAVL_USE_AVL_RECURSIVE 0

#if RAVL_USE_AVL_RECURSIVE
  class AVLTreeOverWriteC {
  public:
    AVLTreeOverWriteC()
    {}
  };
#endif

  //! userlevel=Develop
  //: AVL tree body.
  
  template<class KeyT,class DataT>
  class AVLTreeBodyC
    : public BinaryTreeBodyC<KeyT,DataT> 
  {
  public:
    AVLTreeBodyC()
    {}
    //: Default constructor.
    
    virtual bool Insert(const KeyT &key,const DataT &dat,bool overwrite = true);
    //: Insert a node into the tree.
    // If 'overwrite' is true, and a node with the given
    // key is already in the tree then it will be overwritten
    // otherwise 'false' will be returned.
    
    virtual bool Remove(const KeyT &key);
    //: Remove an item from tree.

  protected:
    
#if RAVL_USE_AVL_RECURSIVE
    BinaryTreeNodeC<KeyT,DataT> *AVLInsert(BinaryTreeNodeC<KeyT,DataT> *node,const KeyT &key,const DataT &dat,bool overwrite = true);
#endif
    
    BinaryTreeNodeC<KeyT,DataT> *SingleRotateWith0(BinaryTreeNodeC<KeyT,DataT> *node);
    //: Do a single rotation with child 0.
    // 'node' must have a child 0. <p>
    // returns new root.
    
    BinaryTreeNodeC<KeyT,DataT> *SingleRotateWith1(BinaryTreeNodeC<KeyT,DataT> *node);
    //: Do a single rotation with child 1.
    // 'node' must have a child 1. <p>
    // returns new root.
    
    BinaryTreeNodeC<KeyT,DataT> *DoubleRotateWith0(BinaryTreeNodeC<KeyT,DataT> *node);
    //: Do a single rotation with child 0.
    // 'node' must have a child 0. <p>
    // returns new root.
    
    BinaryTreeNodeC<KeyT,DataT> *DoubleRotateWith1(BinaryTreeNodeC<KeyT,DataT> *node);
    //: Do a single rotation with child 1.
    // 'node' must have a child 1. <p>
    // returns new root.

  };
  
  //! userlevel=Normal
  //: Adelson-Velskii and Landis binary tree.
  // <p>Adelson-Velskii and Landis (AVL) binary trees take approximately 
  // O(log N) time for all operations. </p>
  //
  // <p>Deletion is partly lazy, it will only remove nodes that are
  // easy, the rest it flags for removal later.</p>
  // <p>Use <code>BinaryTreeUpIterC</code> and <code>BinaryTreeDownIterC</code>
  // as iterators.
  
  template<class KeyT,class DataT>
  class AVLTreeC
    : public BinaryTreeC<KeyT,DataT> 
  {
  public:
    AVLTreeC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit AVLTreeC(bool)
      : BinaryTreeC<KeyT,DataT>(*new AVLTreeBodyC<KeyT,DataT>())
    {}
    //: Default constructor.
    // Creates an invalid handle.

  protected:
    
  };

  
  template<class KeyT,class DataT>
  BinaryTreeNodeC<KeyT,DataT> *AVLTreeBodyC<KeyT,DataT>::SingleRotateWith0(BinaryTreeNodeC<KeyT,DataT> *k2) {    
    BinaryTreeNodeC<KeyT,DataT> *k1 = k2->Child0();
    RavlAssert(k1 != 0);
    k2->Child0() = k1->Child1();
    k1->Child1() = k2;
    k2->Balance() = Max(k2->ChildBalance0(),k2->ChildBalance1())+1;
    k1->Balance() = Max(k1->ChildBalance0(),k2->Balance())+1;
    return k1;
  }
  
  template<class KeyT,class DataT>
  BinaryTreeNodeC<KeyT,DataT> * AVLTreeBodyC<KeyT,DataT>::SingleRotateWith1(BinaryTreeNodeC<KeyT,DataT> *k2) {
    BinaryTreeNodeC<KeyT,DataT> *k1 = k2->Child1();
    RavlAssert(k1 != 0);
    k2->Child1() = k1->Child0();
    k1->Child0() = k2;
    k2->Balance() = Max(k2->ChildBalance0(),k2->ChildBalance1())+1;
    k1->Balance() = Max(k1->ChildBalance1(),k2->Balance())+1;
    return k1;
  }
  
  template<class KeyT,class DataT>
  BinaryTreeNodeC<KeyT,DataT> *AVLTreeBodyC<KeyT,DataT>::DoubleRotateWith0(BinaryTreeNodeC<KeyT,DataT> *k3) {
    k3->Child0() = SingleRotateWith1(k3->Child0());
    return SingleRotateWith0(k3);
  }
  
  template<class KeyT,class DataT>
  BinaryTreeNodeC<KeyT,DataT> *AVLTreeBodyC<KeyT,DataT>::DoubleRotateWith1(BinaryTreeNodeC<KeyT,DataT> *k3){
    k3->Child1() = SingleRotateWith0(k3->Child1());
    return SingleRotateWith1(k3);
  }

#if RAVL_USE_AVL_RECURSIVE
  template<class KeyT,class DataT>
  bool AVLTreeBodyC<KeyT,DataT>::Insert(const KeyT &key,const DataT &dat,bool overwrite) {
    try {
      this->root = AVLInsert(root,key,dat,overwrite);
      return true;
    } catch(AVLTreeOverWriteC &) {}
    return false;
  }

  template<class KeyT,class DataT>
  BinaryTreeNodeC<KeyT,DataT> *AVLTreeBodyC<KeyT,DataT>::AVLInsert(BinaryTreeNodeC<KeyT,DataT> *node,const KeyT &key,const DataT &data,bool overwrite) {
    if(node == 0) {
      this->size++;
      return new BinaryTreeNodeC<KeyT,DataT>(key,data);
    }
    if(key < node->Key()) {
      node->Child0() = AVLInsert(node->Child0(),key,data,overwrite);
      if((node->ChildBalance0() - node->ChildBalance1()) >= 2) {
	if(key < node->Child0()->Key())
	  node = SingleRotateWith0(node);
	else
	  node = DoubleRotateWith0(node);
      } 
      node->Balance() = Max(node->ChildBalance0(),node->ChildBalance1())+1;
      return node;
    }
    if(key > node->Key()) {
      node->Child1() = AVLInsert(node->Child1(),key,data,overwrite);
      if((node->ChildBalance1() - node->ChildBalance0()) >= 2) {
	if(key > node->Child1()->Key())
	  node = SingleRotateWith1(node);
	else
	  node = DoubleRotateWith1(node);
      }
      node->Balance() = Max(node->ChildBalance0(),node->ChildBalance1())+1;
      return node;
    }
    if(!node->IsDeleted()) {
      if(!overwrite)   
	throw AVLTreeOverWriteC();
    } else
      this->size++;
    node->Data() = data;
    node->SetDeleted(false);
    return node;
  }
#else
  template<class KeyT,class DataT>
  bool AVLTreeBodyC<KeyT,DataT>::Insert(const KeyT &key,const DataT &dat,bool overwrite) {
    BinaryTreeNodeC<KeyT,DataT> *next,*place = this->root;
    if(place == 0) {
      this->root = new BinaryTreeNodeC<KeyT,DataT>(key,dat);
      this->size++;
      return true;
    }
    BlkStackC<BinaryTreeNodeC<KeyT,DataT> *> stk;
    for(;;) {
      RavlAssert(place != 0);
      stk.Push(place);
      if(key < place->Key()) {
	next = place->Child0();
	if(next == 0) {
	  place->Child0() = new BinaryTreeNodeC<KeyT,DataT>(key,dat);
	  this->size++;
	  if(place->ChildBalance1() >= 0)
	    return true; // No change in depth.
	  place->Balance() = 1;
	  break;
	}
	place = next;
	continue;
      }
      if(key > place->Key()) {
	next = place->Child1();
	if(next == 0) {
	  place->Child1() = new BinaryTreeNodeC<KeyT,DataT>(key,dat);
	  this->size++;
	  if(place->ChildBalance0() >= 0)
	    return true; // No change in depth.
	  place->Balance() = 1;
	  break;
	}
	place = next;
	continue;
      }
      // Found key in the tree.
      if(!overwrite && !place->IsDeleted()) 
	return false;
      this->size++;
      place->Data() = dat;
      place->SetDeleted(false);
      return true;
    }
    // Add new node.
    //cerr << "Insert, Rebalancing nodes from " << place->Key() <<" \n";
    // Now we have to go back through the stack checking
    // that tree remains balanced.
    next = 0;
    for(;!stk.IsEmpty();stk.DelTop()) {
      place = stk.Top();
      IntT orgBalance = place->Balance();
      //cerr << "Insert, backtrack to " << place->Key() <<" \n";
      if(key < place->Key()) {	
	if(next != 0) 
	  place->Child0() = next; // Assign new root.
	RavlAssert(place->Child0() != 0);
	//cerr << "Balance0= " << (place->ChildBalance0() - place->ChildBalance1()) <<" \n";
	if((place->ChildBalance0() - place->ChildBalance1()) >= 2) {
	  if(key < place->Child0()->Key())
	    place = SingleRotateWith0(place);
	  else
	    place = DoubleRotateWith0(place);
	} else {
	  place->Balance() = Max(place->ChildBalance0(),place->ChildBalance1())+1;
	  place = 0;
	}
      } else {
	if(next != 0)
	  place->Child1() = next; // Assign new root.
	RavlAssert(place->Child1() != 0);
	//cerr << "Balance1= " << (place->ChildBalance1() - place->ChildBalance0()) <<" \n";
	if((place->ChildBalance1() -place->ChildBalance0()) >= 2) {
	  if(key > place->Child1()->Key())
	    place = SingleRotateWith1(place);
	  else
	    place = DoubleRotateWith1(place);
	} else {
	  place->Balance() = Max(place->ChildBalance0(),place->ChildBalance1())+1;
	  place = 0;
	}
      }
      if(place != 0) {
	int newBal = Max(place->ChildBalance0(),place->ChildBalance1())+1;
	if(newBal == orgBalance) { // Has the balance changed?
	  //cerr << "New balance equals old one, finishing. \n";
	  // No, We're nearly finish, just have to
	  // check the new roots of any rotations are
	  // put into the approprate place.
	  stk.DelTop();
	  if(stk.IsEmpty()) {
	    this->root = place;
	  } else {
	    if(key < stk.Top()->Key()) 
	      stk.Top()->Child0() = place;
	    else
	      stk.Top()->Child1() = place;
	  }
	  return true; // Balance matches old value, nothing more to do.
	}
	place->Balance() = newBal;
      }
      next = place;
    }
    if(next != 0)
      this->root = next;
    return true;
  }
  
#endif
  //: Remove item from tree.
  
  template<class KeyT,class DataT>
  bool AVLTreeBodyC<KeyT,DataT>::Remove(const KeyT &key) {
    BinaryTreeNodeC<KeyT,DataT> *place = this->root;
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
    if(ret) this->size--;
    place->SetDeleted(true);
#if 1
    // Go back through path to node deleting as much as we can easly.
    // This is necissary to ensure MaxNode and MinNode work  properly.
    // FIXME:- We should really make some attempt to balance the
    // tree after deleting nodes. As it is we leave it to the
    // insert operation to try and sort things out.
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
	this->root = oth;
	break;
      }
      BinaryTreeNodeC<KeyT,DataT> *top = stk.Top();
      if(top->Child0() == place) 
	top->Child0() = oth;
      else 
	top->Child1() = oth;
      top->Balance() = Max(top->ChildBalance0(),top->ChildBalance1())+1;
      delete place;
      place = top;
      if(!place->IsDeleted())
	break; // Node not deleted, nothing more to do.
      stk.DelTop();
    } while(1);
#endif    
    return ret;
  }
  


}

#endif
