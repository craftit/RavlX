// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BUFFER3D_HEADER
#define RAVL_BUFFER3D_HEADER 1
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Buffer/Buffer3d.hh"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! author="Charles Galambos"

#include "Ravl/Buffer2d.hh"
#include "Ravl/RBfAcc2d.hh"

//: Ravl global namespace.

namespace RavlN {
  
  //! userlevel=Develop
  //: Buffer 3D
  // This holds a handle to data used in various 3d arrays.
  
  template<class DataT>
  class Buffer3dBodyC 
    : public BufferBodyC<BufferAccessC<BufferAccessC<DataT > > >
  {
  public:
    Buffer3dBodyC()
    {}
    //: Default constructor.
    
#if 0
    Buffer3dBodyC(SizeT nsize1,SizeT nsize2,SizeT nsize3)
      : BufferBodyC<BufferAccessC<BufferAccessC<DataT > > >(nsize1),
	dataIndex(nsize1 * nsize2),
	data(nsize1 * nsize2 * nsize3),
	size2(nsize2),
	size3(nsize3)
    {}
    //: Sized constructor.
    
    Buffer3dBodyC(const BufferC<DataT> &dat,SizeT nrng1,SizeT nrng2,SizeT nrng3)
      : BufferBodyC<BufferAccessC<BufferAccessC<DataT > > >(nrng1),
	data(dat),
	size2(nrng2),
	size3(nrng3)
    {}
    //: Buffer constructor.
#else
    Buffer3dBodyC(SizeT nsize1,SizeT nsize2,SizeT nsize3)
      : BufferBodyC<BufferAccessC<BufferAccessC<DataT > > >(nsize1),
	buffer2(nsize1 * nsize2,nsize3),
	size2(nsize2),
	size3(nsize3)
    {}
    //: Sized constructor.
    
    Buffer3dBodyC(const BufferC<DataT> &dat,SizeT nrng1,SizeT nrng2,SizeT nrng3)
      : BufferBodyC<BufferAccessC<BufferAccessC<DataT > > >(nrng1),
	buffer2(dat,nrng1 * nrng2),
	size2(nrng2),
	size3(nrng3)
    {}
    //: Buffer constructor.    
#endif
    
    BufferC<DataT> &Data()
    { return buffer2.Data(); }
    //: Access data buffer.
    
    const BufferC<DataT> &Data() const
    { return buffer2.Data(); }
    //: Access data buffer.
    
    BufferC<BufferAccessC<DataT> > &DataIndex()
    { return buffer2; }
    //: Access data buffer.
    
    const BufferC<BufferAccessC<DataT> > &DataIndex() const
    { return buffer2; }
    //: Access data buffer.
    
    SizeT Size1() const
    { return BufferBodyC<BufferAccessC<BufferAccessC<DataT > > >::Size(); }
    //: Get size of dimention 1
    
    SizeT Size2() const
    { return size2; }
    //: Get size of dimention 2
    
    SizeT Size3() const
    { return size3; }
    //: Get size of dimention 3
    
    Buffer2dC<DataT> &Buffer2d()
    { return buffer2; }
    //: Access 2d component of buffer.
    
  protected:
    Buffer2dC<DataT> buffer2;
    // BufferC<BufferAccessC<DataT> > dataIndex;
    // BufferC<DataT> data;
    SizeT size2;
    SizeT size3;
  };

  //! userlevel=Develop
  //: Buffer 3D 
  // This holds a handle to data used in various 3d arrays.
  
  template<class DataT>
  class Buffer3dC 
    : public BufferC<BufferAccessC<BufferAccessC<DataT > > >
  {
  public:
    Buffer3dC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    Buffer3dC(SizeT size1,SizeT size2,SizeT size3)
      : BufferC<BufferAccessC<BufferAccessC<DataT > > >(*new Buffer3dBodyC<DataT>(size1,size2,size3))
    {}
    //: Size constructor.
    
    Buffer3dC(const BufferC<DataT> &dat,SizeT rng1,SizeT rng2,SizeT rng3)
      : BufferC<BufferAccessC<BufferAccessC<DataT > > >(*new Buffer3dBodyC<DataT>(dat,rng1,rng2,rng3)) 
    {}
    //: Constructor.
    
  protected:

    Buffer3dBodyC<DataT> &Body()
    { return static_cast<Buffer3dBodyC<DataT> &>(BufferC<BufferAccessC<BufferAccessC<DataT > > >::Body()); }
    //: Access body.
    
    const Buffer3dBodyC<DataT> &Body() const
    { return static_cast<const Buffer3dBodyC<DataT> &>(BufferC<BufferAccessC<BufferAccessC<DataT > > >::Body()); }
    //: Constant access to body.
    
  public:
    
    BufferC<DataT> &Data()
    { return Body().Data(); }
    //: Access data buffer.
    
    const BufferC<DataT> &Data() const
    { return Body().Data(); }
    //: Access data buffer.

    BufferC<BufferAccessC<DataT> > &DataIndex()
    { return Body().DataIndex(); }
    //: Access data buffer.
    
    const BufferC<BufferAccessC<DataT> > &DataIndex() const
    { return Body().DataIndex(); }
    //: Access data buffer.
    
    SizeT Size1() const
    { return Body().Size1(); }
    //: Get size of dimention 1
    
    SizeT Size2() const
    { return Body().Size2(); }
    //: Get size of dimention 2
    
    SizeT Size3() const
    { return Body().Size3(); }
    //: Get size of dimention 3
    
    Buffer2dC<DataT> &Buffer2d()
    { return Body().Buffer2d(); }
    //: Access 2d component of buffer.
    
  };
  
}
  
#endif
