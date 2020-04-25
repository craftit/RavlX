// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DMEDIABUFFER_HEADER
#define RAVLIMAGE_DMEDIABUFFER_HEADER 1
/////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DMedia/Buffer.hh"
//! lib=RavlDMedia
//! userlevel=Default
//! author="Charles Galambos"
//! date="22/11/1999"
//! docentry="Ravl.API.Images.Video.Video IO.SGI DMedia"

#include "Ravl/Buffer.hh"
#include "Ravl/DMedia/Memory.hh"

namespace RavlImageN
{
  template<class DataT>
  class DMBufferBodyC 
    : public BufferBodyC<DataT>
  {
  public:
    DMBufferBodyC(const VLNodePoolDataC &dat)
      : buffHandle(dat)
    {
      if(buffHandle.IsValid())
	Attach(SizeBufferAccessC<DataT>((DataT *)buffHandle.Data(),buffHandle.Size() / sizeof(DataT)));
    }
    //: Constructor.
    
    virtual ~DMBufferBodyC() {
      // Make sure memory isn't deleted.
      Attach(SizeBufferAccessC<DataT>(0,0));
      // The rest is handled when buffHandle is defrefrenced.
    }
    
    VLNodePoolDataC &PoolData()
    { return buffHandle; }
    //: Access pool data.
    
  protected:
    VLNodePoolDataC buffHandle;
  };
  
  template<class DataT>
  class DMBufferC 
    : public BufferC<DataT>
  {
  public:    
    DMBufferC(const VLNodePoolDataC &dat)
      : BufferC<DataT>(*new DMBufferBodyC<DataT>(dat))
    {}
    //: Constructor.
    
    DMBufferC(const BufferC<DataT> &buff)
      : BufferC<DataT>(dynamic_cast<const DMBufferBodyC<DataT> *>(RCHandleC<BufferBodyC<DataT> >::BodyPtr(buff)))
    {}
    //: Constructor.
    
  protected:
    DMBufferBodyC<DataT> &Body()
    { return dynamic_cast<DMBufferBodyC<DataT> &>(BufferC<DataT>::Body()); }
    //: Access body.
    
    const DMBufferBodyC<DataT> &Body() const
    { return dynamic_cast<const DMBufferBodyC<DataT> &>(BufferC<DataT>::Body()); }
    //: Access body.
    
  public:
    
    VLNodePoolDataC &PoolData()
    { return Body().PoolData(); }
    //: Access pool data.
    
  };
  
  
}

#endif
