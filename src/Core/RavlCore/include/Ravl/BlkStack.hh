// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BLKSTACK_HEADER
#define RAVL_BLKSTACK_HEADER 1
/////////////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Stacks"
//! lib=RavlCore
//! date="26/08/2001"
//! file="Ravl/Core/Container/Misc/BlkStack.hh"

#include "Ravl/Assert.hh"

namespace RavlN {

  template<class DataT> class BlkStackIterC; 

  //! userlevel=Advanced
  //: Block stack.
  // This is a small object <p>
  // This is designed to handle simple data types efficiently. It
  // has some draw backs however, there is no guarantee when the
  // data held on the stack will have its destructor called, nor
  // are there any guarantees in what order they well happen. If
  // this is a problem use StackC instead as it handles these cases
  // properly.
  
  template<class DataT>
  class BlkStackC
  {
  public:
    BlkStackC(int sizeEst = 32);
    //: Constructor.
    // Give an estimate of the maximum size of the stack.
    
    ~BlkStackC();
    //: Destructor.
    
    DataT &Top()
    { return *top; }
    //: Access top of stack.

    const DataT &Top() const
    { return *top; }
    //: Access top of stack.
    
    bool IsEmpty() const
    { return  top == bob; }
    //: Test if stack is empty.
    
    inline void Push(const DataT &data);
    //: Push item onto stack.

    inline DataT Pop();
    //: Pop item from stack.
    
    inline void   DelTop();
    //: Remove an element from the top of the stack. 
    // Same as Pop(), but no value returned.
    // This is slightly faster than Pop
    
    inline void   DelFirst()  
    { DelTop(); }
    //: Remove an element from the top of the stack. 
    // Same as Pop(), but no value returned.
    // This may be faster then Pop
    
    inline void   InsFirst(const DataT& d) 
    { Push(d);}
    //: Push element onto stack.
    // Same as Push().
    
    inline void   Insert(const DataT& d)   
    { Push(d);}
    //: Push element onto stack.
    // Same as Push().

    void Empty();
    //: Empty stack of all contents.
    
  protected:
    struct SBlkC {
      SBlkC(int nsize,SBlkC *nlast)
	: size(nsize),
	  last(nlast)
      { data = new DataT[size]; }
      //: Construct a stack block.
      
      ~SBlkC()
      { delete [] data; }
      //: Destructor.
      
      int size;
      SBlkC *last;
      DataT *data;
    };
    
    inline SBlkC *PushBlk(int sizeEst);
    //: Push a new block onto the stack.

    inline bool PopBlk();
    //: Pop a block from the stack.
    
    DataT *top; // Current top of stack.  
    DataT *eob; // End of block guard
    DataT *bob; // Begining of block guard.
    SBlkC *topBlk;
    SBlkC *lastBlk; // Last block allocated.

    friend class BlkStackIterC<DataT>;
  };
  
  template<class DataT>
  typename BlkStackC<DataT>::SBlkC *BlkStackC<DataT>::PushBlk(int sizeEst) {
    if(lastBlk != 0) {
      lastBlk->last = topBlk;
      topBlk = lastBlk; // We have one saved from earlier, use it.
      lastBlk = 0;
    } else
      topBlk = new SBlkC(sizeEst,topBlk);
    bob = &topBlk->data[-1];
    eob = &topBlk->data[topBlk->size];
    top = bob;
    return topBlk;
  }

  template<class DataT>
  bool BlkStackC<DataT>::PopBlk() {
    RavlAssert(topBlk != 0);
    SBlkC *oTop = topBlk;
    topBlk = oTop->last;
    RavlAssert(topBlk != 0);
    if(lastBlk == 0)
      lastBlk = oTop; // Save the last blk for later, if needed.
    else
      delete oTop;
    bob = &topBlk->data[-1];
    eob = &topBlk->data[topBlk->size];    
    top = eob-1;
    return true;
  }
  
  template<class DataT>
  BlkStackC<DataT>::BlkStackC(int sizeEst)
    : topBlk(0),
      lastBlk(0)
  {
    if(sizeEst < 16)
      sizeEst = 16;
    PushBlk(sizeEst);
  }

  //: Empty stack of all contents.
  
  template<class DataT>
  void BlkStackC<DataT>::Empty() {
    while(topBlk->last != 0)
      PopBlk();
    top = bob;
  }
  
  template<class DataT>
  BlkStackC<DataT>::~BlkStackC() {
    Empty();
    delete lastBlk;
    delete topBlk;
  }
  
  template<class DataT>
  void BlkStackC<DataT>::Push(const DataT &data) {
    top++;
    if(top != eob) {
      *top = data;
      return ;
    }
    PushBlk(topBlk->size * 2);
    top++;
    *top = data;
  }

  
  template<class DataT>
  DataT BlkStackC<DataT>::Pop() {
    DataT ret = *top;
    RavlAssertMsg(top != bob,"Pop on empty stack. ");
    top--;
    if(top != bob) 
      return ret;
    if(topBlk->last == 0)
      return ret; // Stack is now empty.
    PopBlk();
    return ret;
  }

  template<class DataT>
  void BlkStackC<DataT>::DelTop() {
    top--;
    if(top != bob) 
      return ;
    if(topBlk->last == 0)
      return ; // Stack is now empty.
    PopBlk();
    return ;
  }
  
}

#endif
