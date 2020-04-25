// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BUFFER_HEADER
#define RAVL_BUFFER_HEADER 1
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! file="Ravl/Core/Container/Buffer/Buffer.hh"
//! lib=RavlCore
//! userlevel=Default
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/Types.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/SBfAcc.hh"
#include <stdlib.h>

//: Ravl global namespace.

namespace RavlN {
  
  //! userlevel=Develop
  //: Buffer body.
  // This holds a handle to data used in various arrays.
  
  template<class DataT>
  class BufferBodyC 
    : public RCBodyVC,
      public SizeBufferAccessC<DataT>
  {
  public:
    BufferBodyC(SizeT nsize,const DataT &defaultVal)
      : SizeBufferAccessC<DataT>(new DataT[nsize.V()],nsize),
        deletable(true)
    { this->Fill(defaultVal); }
    //: Constructor a buffer of 'nsize' items, with default value.
    
    BufferBodyC(SizeT nsize)
      : SizeBufferAccessC<DataT>(new DataT[nsize.V()],nsize),
        deletable(true)
    {}
    //: Constructor a buffer of 'nsize' items.
    
    BufferBodyC()
      : deletable(false)
    {}
    //: Default constructor.
    
    BufferBodyC(SizeT nsize,DataT *dat,bool copy = false,bool deletable = false);
    //: Construct from data.
    
    BufferBodyC(DataT *dat,SizeT nsize,bool _deletable)
      : SizeBufferAccessC<DataT>(dat,nsize),
        deletable(_deletable)
    {}
    //: Construct from point and size. The data is not copied, and not deletable.
    
    virtual ~BufferBodyC() {
      if(this->buff != 0 && deletable)
	delete [] this->buff;
    }
    //: Destructor.
    
#if 1
    void *operator new(size_t s) 
    { return malloc(s); }
    //: Override default allocator to just use malloc.
    // This allows classes like SingleBuffer to do more efficient memory allocation.
    
    void *operator new(size_t s,void *mem) 
    { return mem; }
    //: Override default allocator to just pass through memory given.
    // This allows classes like SingleBuffer to do more efficient memory allocation.
    
    void operator delete(void *data,size_t s) 
    { free(data); }
    //: Override default allocator to just use free.
    // This allows classes like SingleBuffer to do more efficient memory allocation.
    
    void operator delete(void *data,void *mem) 
    { free(data); }
    //: Override default allocator to just use free.
    // This allows classes like SingleBuffer to do more efficient memory allocation.
#endif
    
  protected:    
    bool deletable;
  };
  
  //! userlevel=Develop
  //: Buffer handle.

  template<class DataT>
  class BufferC 
    : public RCHandleVC<BufferBodyC<DataT> >
  {
  public:
    BufferC()
    {}
    //: Default constructor
    // creates an invalid handle.
    
    BufferC(SizeT nsize)
      : RCHandleVC<BufferBodyC<DataT> >(*new BufferBodyC<DataT>(nsize))
    {}
    //: Constructor
    // Creates a buffer containing 'nsize' items.
    
    BufferC(SizeT nsize,DataT *dat,bool copy = false,bool deletable = false)
      : RCHandleVC<BufferBodyC<DataT> >(*new BufferBodyC<DataT>(nsize,dat,copy,deletable))
    {}
    //: Constructor
    // Creates a buffer containing 'nsize' items.
    
    explicit BufferC(const BufferBodyC<DataT> *bod)
      : RCHandleVC<BufferBodyC<DataT> >(bod)
    {}
    //: Body constructor
    
    BufferC(BufferBodyC<DataT> &bod)
      : RCHandleVC<BufferBodyC<DataT> >(bod)
    {}
    //: Body constructor
  protected:

    BufferBodyC<DataT> &Body()
    { return RCHandleC<BufferBodyC<DataT> >::Body(); }
    //: Access body.
    
    const BufferBodyC<DataT> &Body() const
    { return RCHandleC<BufferBodyC<DataT> >::Body(); }
    //: Constant access to body.
    
  public:
    SizeT Size() const
    { return Body().Size(); }
    //: Access size of buffer.
    
    inline DataT * ReferenceElm() const
    { return Body().ReferenceElm(); }
    //: Returns the pointer to the reference element of the attached buffer.
    // The reference element need not to be the valid element of the buffer.
    
    void Fill(const DataT &val)
    { Body().Fill(val); }
    //: Fill buffer with value.
    
    const SizeBufferAccessC<DataT> &BufferAccess() const
    { return Body(); }
    //: Access buffer.
    
    operator SizeBufferAccessC<DataT> &() const
    { return const_cast<SizeBufferAccessC<DataT> &>(((const SizeBufferAccessC<DataT> &)Body())); }
    //: Access as a size buffer.
    // I don't like this, but it gets around an irritating warning from gcc.
  };

  ////////////////////////////////////////////////////////////
  
  template<class DataT>
  BufferBodyC<DataT>::BufferBodyC(SizeT nsize,DataT *ndat,bool copy,bool ndeletable)
    : SizeBufferAccessC<DataT>(ndat,nsize),
      deletable(ndeletable)
  {
    if(!copy)
      return ;
    this->buff = new DataT[nsize.V()];
    DataT *place,*end = &this->buff[nsize.V()];
    const DataT *source = ndat;
    for(place = this->buff;place != end;place++,source++)
      *place = *source;
  }
  
}


#endif
