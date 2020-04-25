// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_SUBSAMPLE_HEADER
#define RAVLIMAGE_SUBSAMPLE_HEADER 1
////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Images.Scaling and Warping"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Tools/SubSample.hh"
//! author="Charles Galambos"
//! userlevel=Normal

#include "Ravl/Image/Image.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dSqr3Iter.hh"
#include "Ravl/Traits.hh"

namespace RavlImageN {
  
  template<typename PixelT>
  ImageC<PixelT> FilteredSubSample2(const  ImageC<PixelT> & img, ImageC<PixelT> out=ImageC<PixelT>()) {
    typedef typename RavlN::NumericalTraitsC<PixelT>::AccumT AccumT;
    Index2dC origin(img.Frame().Origin().Row() / 2,img.Frame().Origin().Col() / 2);
    Index2dC size(((img.Frame().Rows()-1) / 2)-1,
		  ((img.Frame().Cols()-1) / 2)-1);
    //cerr << "Origin=" << origin << " Size=" << size << "\n";
    if (size != out.Frame().End() - out.Frame().Origin()) {
      Index2dC end = origin + size;
      IndexRange2dC newRange(origin,end);
      out = ImageC<PixelT>(newRange);
    }
    Array2dIterC<PixelT> oit(out);
    for(Array2dSqr3IterC<PixelT> it(img);it;it += 2,oit++) {
      AccumT val = static_cast<AccumT>(it.DataTM())
                 + static_cast<AccumT>(it.DataBM())
                 + static_cast<AccumT>(it.DataMR())
                 + static_cast<AccumT>(it.DataML());
      val *= 2;
      val += static_cast<AccumT>(it.DataBL())
           + static_cast<AccumT>(it.DataBR())
           + static_cast<AccumT>(it.DataTR())
           + static_cast<AccumT>(it.DataTL());
      val += static_cast<AccumT>(it.DataMM()) * 4;
      *oit = static_cast<PixelT>(val / 16);
    }
    RavlAssert(!oit);
    return out;
  }
  //: Subsamples the image with filtering by a factor of 2
  // Uses a separable 3&times;3 filter with coeffs of &frac14;, &frac12;, &frac34;; sample points correspond to the middle of each sample.
  

  template<class PixelT>
  ImageC<PixelT> SubSample(const  ImageC<PixelT> & img,  const UIntT factor =2) {    
    ImageRectangleC oldRect  (img.Rectangle() ) ; 
    ImageRectangleC newRect = oldRect / factor ; 
    ImageC<PixelT>  subSampled (newRect) ;
    IndexC oldRow, newRow ;
    for ( oldRow = oldRect.TRow(), newRow = newRect.TRow();
          (oldRow <= oldRect.BRow()) && (newRow <= newRect.BRow()) ;
          oldRow+= factor, ++newRow  ) 
      {
        BufferAccessIterC<PixelT> newCol = subSampled [newRow];
	for ( BufferAccessIterC<PixelT> oldCol = img [oldRow] ; (oldCol.IsElm()) && (newCol.IsElm()) ; oldCol += factor, ++newCol  ) 
	  newCol.Data() = oldCol.Data() ;
      }
    return subSampled ; 
  }
  //: Subsamples the image by the given factor 
  // Pixel at top left-hand corner is always sampled first. <b>No</b> filtering is performed.
  
  template <class PixelT> 
  ImageC <PixelT> UpSample ( const ImageC<PixelT> & img, const UIntT factor=2 ) {
    ImageRectangleC oldRect (img.Rectangle() ) ; 
    ImageRectangleC newRect ( oldRect * factor ) ; 
    ImageC<PixelT> upSampled ( newRect ) ;  
    
    // iterate thougth rows of origial image 
    IndexC oldRow , newRow ; 
    UIntT counter ; 
    for ( oldRow = oldRect.TRow() , newRow = newRect.TRow() ; oldRow <= oldRect.BRow() ; ++ oldRow ) 
      {
	// iterate through rows of upsampled image 
	for ( UIntT rowCounter = 1 ; (newRow <= newRect.BRow() ) && ( rowCounter <= factor) ; ++ newRow , ++ rowCounter )
	  {
	    BufferAccessIterC<PixelT> newCol ( upSampled [newRow] ) ; 
	    BufferAccessIterC<PixelT>  oldCol ( img[oldRow] ) ; 
	    // now iterate the cols and do the copy 
	    for (  ; oldCol.IsElm() ; oldCol++ ) // each pixel in the old row  
	      // iterate through cols of the new image 
	      for ( counter = 1 ; newCol && counter <= factor ; ++ counter,  newCol++  ) 
		newCol.Data() = oldCol.Data() ; 
	  }
      }
    return upSampled ;     
  }
  //: Up-Samples an image by the given factor. 
  
  
}


#endif
