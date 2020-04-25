// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DMABUFFER_HEADER
#define RAVL_DMABUFFER_HEADER 1
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.OS.Hardware"
//! author="Charles Galambos"
//! lib=RavlOS
//! file="Ravl/OS/Misc/DMABuffer.hh"
#include <stdio.h>

#include "Ravl/Buffer.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Buffer suitable for DMA transfers
  
  template<class DataT>
  class DMABufferBodyC 
    : public BufferBodyC<DataT>
  {
  public:
    DMABufferBodyC(SizeT size,UIntT align = 64) 
      : BufferBodyC<DataT>(size,0,false,false)
    {
      size_t pad = align-1;
      raw = new char [(size * sizeof(DataT)) + pad];
      void *aligned = (void *) ((((size_t) raw) + pad) & (~pad));
      //cerr << "Raw:" << raw << " Aligned:" << aligned << "\n";
      this->buff = ((DataT *)aligned);
    }
    //: Construct an aligned buffer.
    // - align must be a power of 2. <p>
    // - This does not initalise the elements of the buffer, and should
    // only be used for trivial objects.
    
    virtual ~DMABufferBodyC() {
      delete [] ((char *) raw);
      this->buff = 0;
    }
    //: Destructor.
    
  protected:
    void *raw;
  };
  
  //! userlevel=Advanced
  //: Buffer suitable for DMA transfers
  
  template<class DataT>
  class DMABufferC 
    : public BufferC<DataT>
  {
  public:
    DMABufferC(UIntT size,UIntT align = 64) 
      : BufferC<DataT>(*new DMABufferBodyC<DataT>(size,align))
    {}
    //: Construct an aligned buffer of 'size' elements.
    // - alignment must be a power of 2. <p>
    // - This does not initalise the elements of the buffer, and should
    // only be used for trivial objects. <p>
    
    DMABufferC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
  
    protected: 
    
    DMABufferC<DataT> ( DMABufferBodyC<DataT> & bod )
    : BufferC<DataT> ( bod ) 
    {}
  //: Constructor from body 
    
  };
  
}

#endif
