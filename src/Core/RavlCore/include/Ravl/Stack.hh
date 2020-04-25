// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STACKC_HEADER
#define RAVL_STACKC_HEADER
//! date="05/10/1995"
//! userlevel=Default
//! file="Ravl/Core/Container/Misc/Stack.hh"
//! lib=RavlCore
//! author="George (Jiri) Matas"
//! docentry="Ravl.API.Core.Stacks"
//! example=exStack.cc
//! rcsid="$Id$"

#include "Ravl/Types.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Exception.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <new>
#else
#include <new.h>
#endif

namespace RavlN {
  
  //! userlevel=Develop
  //: Basic stack types.
  
  class BaseStackTypesG {
  public:
#if !RAVL_COMPILER_VISUALCPP && !defined(__sgi__)
    static const int incrBlkSize = 2; 
#else
    static int incrBlkSize;
#endif
    // must be kept even!!, else Size() incorrect
  };
  
  
  //! userlevel=Develop
  //: Base stack.
  // Complexity proportional to sqrt(N); not N!
  
  template <class T> 
  class  BaseStackC 
    : public BaseStackTypesG
  {
  public:
    inline        BaseStackC();
    //: Default constructor
    // create an empty stack.
    
    inline        BaseStackC(const BaseStackC<T>&);
    //: Copy constructor.
    
    inline       T& Top()         ;
    //: Access element at the top of the stack.
    // i.e. the next one returned if Pop() is called.
  
    inline const T& Top()   const ;
    //: Access element at the top of the stack.
    // i.e. the next one returned if Pop() is called.
    
    inline       T& First()          { return Top();}
    //: Access element at the top of the stack.
    // i.e. the next one returned if Pop() is called.
    // Same as Top().
    
    inline const T& First() const    { return Top();}
    //: Access element at the top of the stack.
    // i.e. the next one returned if Pop() is called.
    // Same as Top().
    
    inline bool   IsEmpty() const; 
    //: Test if stack is empty.
    // Returns true if it is empty.
    
    inline SizeT   Size()    const;
    //: Get the number of elements in the stack.
    
    void DbPrint(){printf("stack -> %d, %d, %p\n",top,blkSize,this->topBlk);}
    //: Usefull for debuging the stack
    // Not usefull for normal users.
  
  protected:
    inline        BaseStackC<T>& operator=(const BaseStackC<T>&) ;
    
    //! userlevel=Develop
    //--------- private function for memory allocation 
    // place at the top of the file to allow inlining
    
    struct s_Blk {s_Blk* link; T d[1];};
    
    inline s_Blk* AllocBlk() const
    { return (s_Blk*) new char[sizeof(s_Blk) + sizeof(T)* (blkSize-1)]; }
    
    int     blkSize;    // size of the first (top) block in the stack 
    s_Blk   *topBlk;    // pointer to the first (top) block  
    int     top;        // index of the first *empty* slot in the last block
    // this choice reduces the number of if statements;
    // as 'top'  can't point to the top of the
    // stack (first occupied elem.) if the stack is empty
  };
  
  
  //! userlevel=Normal
  //: Templated, dynamicly sized stack.
  // Complexity proportional to sqrt(N); not N!
  // Note: This is not reference counted.
  
  template <class T> 
  class  StackC
    : public BaseStackC<T>
  {
  public:
    inline StackC()
      : BaseStackC<T>() 
    {}
    //: Default constructor.
    // constructs an empty stack.

    StackC(const T& d)
      : BaseStackC<T>()
    { Push(d); }
    //: Constructor with single element
    
    StackC(const StackC<T>&);
    //: Copy constructor.

    StackC(const StackC<T>& other, const T& d);
    //: Copy and push constructor
    
    ~StackC()
    { Empty(); }
    //: Destructor.
    
    inline T Pop();
    //: Pop the element from the top of the stack.
    
    inline T GetFirst() 
    { return Pop(); }
    //: Pop the element from the top of the stack.
    // Same as Pop().
    
    inline T Get()      
    { return Pop(); }
    //: Pop the element from the top of the stack.
    // Same as Pop().
    
    inline void   Push(const T&);
    //: Push element onto stack.

    inline void   Push();
    //: Push element created with its default constructor onto the stack.

    inline void   InsFirst(const T& d) 
    { Push(d); }
    //: Push element onto stack.
    // Same as Push().
    
    inline void   Insert(const T& d)   
    { Push(d); }
    //: Push element onto stack.
    // Same as Push().
  
    inline void   DelTop();
    //: Remove an element from the top of the stack. 
    // Same as Pop(), but no value returned.
    // This may be faster then Pop
    
    inline void   DelFirst()  
    { DelTop(); }
    //: Remove an element from the top of the stack. 
    // Same as Pop(), but no value returned.
    // This may be faster then Pop
    
    inline void   Empty();        
    //: remove all elements on the stack
    
  private:
    
    void AddBlk();
    void DelBlk();
    void CopyBlk(const StackC<T>&);
    
    inline void DecrTop();
  };
  
  //------------------ BaseStack ---------------------------------------------
  template <class T>
  inline BaseStackC<T>::BaseStackC()
    : blkSize(0),
      topBlk(0),
      top(0)
  {}
  
  template <class T>
  inline   BaseStackC<T>::BaseStackC(const BaseStackC<T>& bs): 
    blkSize(bs.blkSize),
    topBlk(bs.topBlk),
    top(bs.top)
  {}
  
  template <class T>
  inline  BaseStackC<T>&   BaseStackC<T>::operator=(const BaseStackC<T>& bs)  {
    blkSize=bs.blkSize;
    topBlk =bs.topBlk;
    top    =bs.top;
    return *this; 
  }

  //-----------------------------
  template <class T>
  inline bool BaseStackC<T>::IsEmpty()const {
    // the stack can have 2 empty states:
    // 1.top==0,blkSize=0,topBlk==NULL        (after initalisation or Empty(),)
    //   with no allocated memory
    // 2.top==0,blkSize=incrBlkSize,topBlk==<address>  (after Pop(),DelTop(),..)
    //   with one allcated block containing no data 
    return top==0 && blkSize<=incrBlkSize;
  }
  
  //-----------------------------
  template <class T>
  inline SizeT BaseStackC<T>::Size() const
  { return top + (blkSize/incrBlkSize-1)*blkSize/2; }
  
  //-----------------------------
  template <class T>
  inline T& BaseStackC<T>::Top() {
    return  (top != 0) ? topBlk->d[top-1]:
      topBlk->link->d[blkSize-incrBlkSize-1];
  }
  
  template <class T>
  inline const T& BaseStackC<T>::Top() const {
    return  (top != 0) ? topBlk->d[top-1]:
      topBlk->link->d[blkSize-incrBlkSize-1];
  }
  
  
  //---------------------- Stack ---------------------------------------------
  
  template <class T>
  void  StackC<T>::AddBlk() {
    typename StackC<T>::s_Blk * current = this->topBlk;
    this->topBlk  = this->AllocBlk();
    this->topBlk->link = current;
  }
  
  template <class T>
  void StackC<T>::DelBlk() {
    typename StackC<T>::s_Blk *last = this->topBlk->link;
    delete [] ((char *) this->topBlk);
    this->topBlk = last;
  }
  
  //--------------------------------------------------------------------------
  template <class T>
  inline void StackC<T>::Push(const T& data) {
    if (this->top == this->blkSize) { // run out of space in current block 
      this->blkSize += this->incrBlkSize;
      AddBlk();
      this->top     =  0;
    }
    new(&this->topBlk->d[this->top++]) T(data);   // like a straightforward array impl.
  }

  //--------------------------------------------------------------------------
  template <class T>
  inline void StackC<T>::Push()
  {
    if (this->top == this->blkSize) { // run out of space in current block
      this->blkSize += this->incrBlkSize;
      AddBlk();
      this->top     =  0;
    }
    new(&this->topBlk->d[this->top++]) T();   // like a straightforward array impl.
  }

  //--------------------------------------------------------------------------
  
  template <class T>
  inline void
  StackC<T>::DecrTop() {
    if (this->top == 0) { 
#if RAVL_HAVE_EXCEPTIONS
      if (this->IsEmpty())
	throw ExceptionOutOfRangeC("Removing element from an empty stack!");
#else
      RavlAssertMsg(!IsEmpty(),"Removing element from an empty stack!");
#endif
      DelBlk();
      this->blkSize -= this->incrBlkSize;
      this->top      = this->blkSize ;
    }
    --this->top;
  }
  
  //--------------------------------------------------------------------------
  template <class T>
  inline void StackC<T>::DelTop() {
    DecrTop();
    (&this->topBlk->d[this->top])->~T();
  }
  
  //--------------------------------------------------------------------------
  template <class T>
  inline T StackC<T>::Pop() {
    DecrTop();
    T tmp=this->topBlk->d[this->top];
    (&this->topBlk->d[this->top])->~T();
    return tmp;
  }

  //--------------------------------------------------------------------------
  template <class T>
  void StackC<T>::Empty()  {
    while(!this->IsEmpty()) 
      DelTop(); 
    // leaves stack empty but with alloated blk.
    // remove the block, changed to the initial 
    // state  with nothing allocated 
    if(this->blkSize>0) { 
      DelBlk();                 // Note:
      this->blkSize=0;		// 1. if this wasn't done the destruct.
    }                           // would leak
  }

  template <class T>
  void StackC<T>::CopyBlk(const StackC<T>& s) {
    if(this->blkSize != 0 ) { 
      // stack in inital state, no copy needed
      // to prevent gcc 2.7.2 complaining about possible uninitialised use
      // s could still be empty, with top==0 and one alloc-ed block!
      typename StackC<T>::s_Blk *copiedBlk = s.topBlk;
      this->topBlk = this->AllocBlk();
      typename StackC<T>::s_Blk *firstBlk = this->topBlk;
      do {
	while(this->top > 0) {                // copy one block
	  --this->top;
	  new(&this->topBlk->d[this->top]) T(copiedBlk->d[this->top]);  
	}
	copiedBlk    = copiedBlk->link;
	if(copiedBlk == 0) break;    //**  loop exits here ***;
	// at least one more block to copy..
	this->blkSize     -= this->incrBlkSize;   
	this->top          = this->blkSize;
	this->topBlk->link = this->AllocBlk();
	this->topBlk       = this->topBlk->link;
      } while(1);
      this->topBlk->link = 0;
      this->topBlk       = firstBlk;
      this->top     = s.top;
      this->blkSize = s.blkSize;
    }
  }
  
  //------- last, the only member requiring and iterator ---------------------
  
  template <class T>
  StackC<T>::StackC(const StackC& s)
    : BaseStackC<T>(s) 
  {
    CopyBlk(s);
  }

  template <class T>
  StackC<T>::StackC(const StackC& s, const T& d)
    : BaseStackC<T>(s)
  {
    CopyBlk(s);
    Push(d);
  }
}


#endif
