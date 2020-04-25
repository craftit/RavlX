// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DRAWCROSS_HEADER
#define RAVLIMAGE_DRAWCROSS_HEADER 1
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! date="14/07/2002"
//! docentry="Ravl.API.Images.Drawing"
//! lib=RavlImage
//! userlevel=Normal
//! file="Ravl/Image/Base/DrawCross.hh"

#include "Ravl/Image/Image.hh"

namespace RavlImageN {
  
  template<class DataT>
  void DrawCross(Array2dC<DataT> &dat,const DataT &value,const Index2dC &at,UIntT size = 3) {
    // Cross entirely in the image ?
    if(dat.Frame().Contains(IndexRange2dC(at,size))) { 
      // Its completely inside, we can just change the pixels.
      dat[at] = value;
      for(IntT i = 1;i < (IntT) size;i++) {
	Index2dC off1(i,i);
	dat[at + off1] = value;
	dat[at - off1] = value;
	off1[0] = -i;
	dat[at + off1] = value;
	dat[at - off1] = value;
      }
    } else {
      // Cross is not entirely in the image.
      if(dat.Frame().Contains(at))
	dat[at] = value;
      for(int i = 1;i < (IntT) size;i++) {
	Index2dC off1(i,i);
	Index2dC pat =  at + off1;
	if(dat.Frame().Contains(pat))
	  dat[pat] = value;
	pat =  at - off1;
	if(dat.Frame().Contains(pat))
	  dat[pat] = value;
	off1[0] = -i;
	pat =  at + off1;
	if(dat.Frame().Contains(pat))
	  dat[pat] = value;
	pat =  at - off1;
	if(dat.Frame().Contains(pat))
	  dat[pat] = value;
      }      
    }
  }
  //: Draw a cross in an image.
  
}

#endif
