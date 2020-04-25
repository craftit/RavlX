// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_EDGESOBEL_HEADER
#define RAVLIMAGE_EDGESOBEL_HEADER 1
//////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Edges"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! file="Ravl/Image/Processing/Edges/EdgeSobel.hh"

#include "Ravl/Image/Image.hh"
#include "Ravl/Array2dSqr31Iter2.hh"
#include "Ravl/Array2dSqr311Iter3.hh"
#include "Ravl/TFVector.hh"

namespace RavlImageN {
  
  //! userlevel=Normal
  //: Sobel edge detection engine.
  // <p>The sign convention is: a +ve gradient computed if image intensity is
  // increasing in +ve direction in coordinate system.</p>
  //
  // <p>A vertical (horizontal) edge is defined as an edge in which the
  // <i>gradient is changing</i> in a vertical (horizontal) direction.</p>
  
  template<class DataInT,class DataOutT>
  class EdgeSobelC {
  public:
    EdgeSobelC()
    {}
    
    bool Apply(const ImageC<DataInT> &inImg,ImageC<TFVectorC<DataOutT,2> > &out);
    //: Apply Sobel operator to 'img', put result in 'out'
    // Output gradient vector is in the order: vertical, horizontal gradients
    
    bool Apply(const ImageC<DataInT> &inImg,ImageC<DataOutT> &Vert,ImageC<DataOutT> &Horz);
    //: Apply Sobel operator to 'img', put vertical and horizontal gradients in "Vert" and "Horz" respectively
    
  };
  
  template<class DataInT,class DataOutT>
  bool EdgeSobelC<DataInT,DataOutT>::Apply(const ImageC<DataInT> &img,ImageC<TFVectorC<DataOutT,2> > &out) {
    if(out.IsEmpty()) {
      ImageRectangleC ir = img.Rectangle();
      out = ImageC<TFVectorC<DataOutT,2> >(ir.Shrink(1));
    }
    for(Array2dSqr31Iter2C<DataInT,TFVectorC<DataOutT,2> > it(img,out);it;it++) {
      it.Data2()[0] = it.DataBL1() + it.DataBM1()*2 + it.DataBR1() - it.DataTL1() - it.DataTM1()*2 - it.DataTR1();
      it.Data2()[1] = it.DataBR1() + it.DataMR1()*2 + it.DataTR1() - it.DataBL1() - it.DataML1()*2 - it.DataTL1();
    }
    return true;
  }


  template<class DataInT,class DataOutT>
  bool EdgeSobelC<DataInT,DataOutT>::Apply(const ImageC<DataInT> &img,ImageC<DataOutT> &outX,ImageC<DataOutT> &outY) {
    if(outX.IsEmpty() || outY.IsEmpty()) {
      ImageRectangleC ir = img.Rectangle();
      ir = ir.Shrink(1);
      outX = ImageC<DataOutT>(ir);
      outY = ImageC<DataOutT>(ir);
    }
    for(Array2dSqr311Iter3C<DataInT,DataOutT,DataOutT> it(img,outX,outY);it;it++) {
      it.Data2() = it.DataBL1() + it.DataBM1()*2 + it.DataBR1() - it.DataTL1() - it.DataTM1()*2 - it.DataTR1();
      it.Data3() = it.DataBR1() + it.DataMR1()*2 + it.DataTR1() - it.DataBL1() - it.DataML1()*2 - it.DataTL1();
    }
    return true;    
  }
  
}

#endif
