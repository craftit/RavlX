// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_COMPOSITEBUFFER_HEADER
#define RAVL_COMPOSITEBUFFER_HEADER 1
///////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Arrays.Buffer"
//! file="Ravl/Core/Container/Buffer/CompositeBuffer.hh"
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="10/04/2002"

#include "Ravl/Buffer.hh"
#include "Ravl/RCAbstract.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Composite buffer body.
  
  template<class DataT>
  class CompositeBufferBodyC
    : public BufferBodyC<DataT>
  {
  public:
    CompositeBufferBodyC(UIntT elements,UIntT nsize = 0,DataT *start = 0,bool deletable = false) 
      : BufferBodyC<DataT>(nsize,start,false,deletable),
	noElements(elements),
	elements(new RCAbstractC[elements])
    {}
    //: Default constructor.
    
    ~CompositeBufferBodyC() 
    { delete [] elements; }
    //: Destructor.
    
    UIntT Elements() const
    { return noElements; }
    //: Get the number of elements in the buffer.
    
    void SetElement(UIntT i,const RCAbstractC &sb) {
      RavlAssert(i < noElements);
      elements[i] = sb;
    }
    //: Set an element of the composite buffer.
    
  protected:
    UIntT noElements; // Number of elements in the buffer.
    RCAbstractC *elements;
  }; 
  
  //! userlevel=Advanced
  //: Composite buffer.
  // This is useful if the you need to access part of another buffer
  // as a different type. Or if you the buffer your actually using is made
  // up of several parts.
  
  template<class DataT>
  class CompositeBufferC
    : public BufferC<DataT>
  {
  public:
    CompositeBufferC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    CompositeBufferC(UIntT elements,UIntT nsize = 0,DataT *start = 0,bool deletable = false) 
      : BufferC<DataT>(*new CompositeBufferBodyC<DataT>(elements,nsize,start,deletable))
    {}
    //: Constructor.
    
  protected:
    CompositeBufferC(CompositeBufferBodyC<DataT> &body)
      : BufferC<DataT>(body)
    {}
    //: Body constructor.
    
    CompositeBufferBodyC<DataT> &Body()
    { return static_cast<CompositeBufferBodyC<DataT> &>(BufferC<DataT>::Body()); }
    //: Access body.

    const CompositeBufferBodyC<DataT> &Body() const
    { return static_cast<const CompositeBufferBodyC<DataT> &>(BufferC<DataT>::Body()); }
    //: Access body.
    
  public:
    UIntT Elements() const
    { return Body().Elements(); }
    //: Get the number of elements in the buffer.
    
    void SetElement(UIntT i,const RCAbstractC &sb) 
    { Body().SetElement(i,sb); }
    //: Set an element of the composite buffer.
  };
  
  
}


#endif
