// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BUFFER2D_HEADER
#define RAVL_BUFFER2D_HEADER 1
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/Buffer2d.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! author="Charles Galambos"

#include "Ravl/Buffer.hh"
#include "Ravl/SingleBuffer.hh"

//: Ravl global namespace.

namespace RavlN {
  
  //! userlevel=Develop
  //: Buffer2D
  // This holds a handle to data used in various 2d arrays.
  
  template<class DataT>
  class Buffer2dBodyC :
    public BufferBodyC<BufferAccessC<DataT> >
  {
  public:
    Buffer2dBodyC()
    {}
    //: Default constructor.

    Buffer2dBodyC(SizeT size1,SizeT size2) :
      BufferBodyC<BufferAccessC<DataT> >(size1,ptrArray,false,false),
      data(SingleBufferC<DataT>(size2 * size1))
    {}
    //: Sized constructor.
    
    Buffer2dBodyC(SizeT size1,SizeT size2,DataT *data,bool copy = false,bool deletable = true) :
      BufferBodyC<BufferAccessC<DataT> >(size1,ptrArray,false,false),
      data(size1 * size2,data,copy, deletable)
    {}
    //: Sized constructor.
    
    Buffer2dBodyC(const BufferC<DataT> &dat,SizeT size1) :
      BufferBodyC<BufferAccessC<DataT> >(size1,ptrArray,false,false),
      data(dat)
    {}
    //: Construct a buffer with data area 'dat' and an access buffer with 'size1' entries.
    
    BufferC<DataT> &Data()
    { return data; }
    //: Access data buffer.
    
    const BufferC<DataT> &Data() const
    { return data; }
    //: Access data buffer.
    
    SizeT Size1() const
    { return BufferBodyC<BufferAccessC<DataT> >::Size(); }
    //: Get size 1

    SizeT Size2() const
    { return data.Size() / Size1(); }
    //: Get size 2 (estimate.)
    
  protected:
    BufferC<DataT> data;    
    
    BufferAccessC<DataT> ptrArray[1];
  };
  
  //! userlevel=Develop
  //: Buffer2D 
  // This holds a handle to data used in various 2d arrays.

  template<class DataT>
  class Buffer2dC 
    : public BufferC<BufferAccessC<DataT> >
  {
  public:
    Buffer2dC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    Buffer2dC(SizeT size1,SizeT size2) :
      BufferC<BufferAccessC<DataT> >(*new(AllocBody(size1)) Buffer2dBodyC<DataT>(size1,size2))
    {}
    //: Sized constructor.
    
    Buffer2dC(const BufferC<DataT> &dat,SizeT size1) :
      BufferC<BufferAccessC<DataT> >(*new(AllocBody(size1)) Buffer2dBodyC<DataT>(dat,size1))
    {}
    //: Construct a buffer with data area 'dat' and an access buffer with 'size1' entries.
    
    Buffer2dC(SizeT size1,SizeT size2,DataT *data,bool copy = false,bool deletable = true) :
      BufferC<BufferAccessC<DataT> >(*new(AllocBody(size1)) Buffer2dBodyC<DataT>(size1,size2,data,copy,deletable))
    {}
    //: Construct from an existing buffer.
    
  protected:
    static Buffer2dBodyC<DataT> *AllocBody(SizeT size) {
      if(size < 1) size = 1;
      return reinterpret_cast<Buffer2dBodyC<DataT> *> (malloc(sizeof(Buffer2dBodyC<DataT>) + (size-1) * sizeof(BufferAccessC<DataT>)));
    }
    //: Allocate a body object plus some space.
    
    Buffer2dBodyC<DataT> &Body()
    { return static_cast<Buffer2dBodyC<DataT> &>(BufferC<BufferAccessC<DataT> >::Body()); }
    //: Access body.
    
    const Buffer2dBodyC<DataT> &Body() const
    { return static_cast<const Buffer2dBodyC<DataT> &>(BufferC<BufferAccessC<DataT> >::Body()); }
    //: Constant access to body.
    
  public:

    BufferC<DataT> &Data()
    { return Body().Data(); }
    //: Access data buffer.
    
    const BufferC<DataT> &Data() const
    { return Body().Data(); }
    //: Access data buffer.
    
    SizeT Size1() const
    { return Body().Size1(); }
    //: Get size of buffer along dimention 1

    SizeT Size2() const
    { return Body().Size2(); }
    //: Get size of buffer along dimention 2 (estimate.)
    
  };
  
}
  
#endif
