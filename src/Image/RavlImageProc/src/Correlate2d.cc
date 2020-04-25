// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! file="Ravl/Image/Processing/Filters/Correlate2d.cc"

#include "Ravl/config.h"

#if RAVL_USE_MMX  
#include "Ravl/mmx.hh"
#endif

#include "Ravl/Image/Correlate2d.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

// This code could be loads better, but its a start.

namespace RavlImageN {
  
#if RAVL_USE_MMX
  static void ShortCorrelate2d(const ImageC<short> &in,ImageC<short> &result,const ImageC<short> &kernel) {
    ONDEBUG(cerr << "ShortCorrelate2d(), Using MMX functions. \n");
    RavlAssertMsg(kernel.Frame().Area() > 0,"Convolution kernel too small. ");
    ImageRectangleC resRect = in.Rectangle();
    resRect.TRow() -= kernel.Rectangle().TRow();
    resRect.BRow() -= kernel.Rectangle().BRow();
    resRect.LCol() -= kernel.Rectangle().LCol();
    resRect.RCol() -= kernel.Rectangle().RCol();
    RavlAssertMsg(resRect.Area() > 0,"Convole2dC::Apply(), ERROR: Input rectangle too small.");
    if(!result.Rectangle().Contains(resRect)) // Check the result rectangle is large enough.
      result = ImageC<short>(resRect); // If its not make another.
    
    Array2dIterC<short> res(result,resRect);
    INITMMX
    switch(static_cast<size_t>(kernel.Cols())) {
    case 2: 
      for(Rectangle2dIterC rit(in.Frame(),kernel.Frame());rit;rit++,res++) {
	BufferAccess2dIter2C<short,short> it(kernel,kernel.Range1(),kernel.Range2(),
					     in,rit.Window().Range1(),rit.Window().Range2());
	int sum = 0;
	do {
	  MMXMULT4S(&it.Data2(), &it.Data1(), sum, MMX2 );
	} while(it.NextRow()) ;
	*res = (short) sum;
      }
      break;
    case 3: 
      for(Rectangle2dIterC rit(in.Frame(),kernel.Frame());rit;rit++,res++) {
	BufferAccess2dIter2C<short,short> it(kernel,kernel.Range1(),kernel.Range2(),
					     in,rit.Window().Range1(),rit.Window().Range2());
	int sum = 0;
	do {
	  MMXMULT4S(&it.Data2(), &it.Data1(), sum, MMX3 );
	} while(it.NextRow()) ;
	*res = (short) sum;
      }
      break;
    case 4:  
      for(Rectangle2dIterC rit(in.Frame(),kernel.Frame());rit;rit++,res++) {
	BufferAccess2dIter2C<short,short> it(kernel,kernel.Range1(),kernel.Range2(),
					     in,rit.Window().Range1(),rit.Window().Range2());
	int sum = 0;
	do {
	  MMXMULT4S(&it.Data2(), &it.Data1(), sum, MMX4 );
	} while(it.NextRow()) ;
	*res = (short) sum;
      }
      break;	
    case 5: 
      for(Rectangle2dIterC rit(in.Frame(),kernel.Frame());rit;rit++,res++) {
	BufferAccess2dIter2C<short,short> it(kernel,kernel.Range1(),kernel.Range2(),
					     in,rit.Window().Range1(),rit.Window().Range2());
	int sum = 0;
	do {
	  MMXMULT8S(&it.Data2(),&((&it.Data2())[4]), &it.Data1(),&((&it.Data1())[4]),sum, MMX5 );
	} while(it.NextRow()) ;
	*res = (short) sum;
      }
      break;	
    case 6:
      for(Rectangle2dIterC rit(in.Frame(),kernel.Frame());rit;rit++,res++) {
	BufferAccess2dIter2C<short,short> it(kernel,kernel.Range1(),kernel.Range2(),
					     in,rit.Window().Range1(),rit.Window().Range2());
	int sum = 0;
	do {
	  MMXMULT8S(&it.Data2(),&((&it.Data2())[4]), &it.Data1(),&((&it.Data1())[4]),sum, MMX6 );
	} while(it.NextRow()) ;
	*res = (short) sum;
      }	
      break;	
    case 7:  
      for(Rectangle2dIterC rit(in.Frame(),kernel.Frame());rit;rit++,res++) {
	BufferAccess2dIter2C<short,short> it(kernel,kernel.Range1(),kernel.Range2(),
					     in,rit.Window().Range1(),rit.Window().Range2());
	int sum = 0;
	do {
	  MMXMULT8S(&it.Data2(),&((&it.Data2())[4]), &it.Data1(),&((&it.Data1())[4]),sum, MMX7 );
	} while(it.NextRow()) ;
	*res = (short) sum;
      }	
      break; 
    case 8:
      for(Rectangle2dIterC rit(in.Frame(),kernel.Frame());rit;rit++,res++) {
	BufferAccess2dIter2C<short,short> it(kernel,kernel.Range1(),kernel.Range2(),
					     in,rit.Window().Range1(),rit.Window().Range2());
	int sum = 0;
	do {
	  MMXMULT8S(&it.Data2(),&((&it.Data2())[4]), &it.Data1(),&((&it.Data1())[4]),sum, MMX8 );
	} while(it.NextRow()) ;
	  *res = (short) sum;
      }		
      break; 
    default:
      // Default implementation
      for(Rectangle2dIterC rit(in.Frame(),kernel.Frame());rit;rit++,res++) {
	BufferAccess2dIter2C<short,short> it(kernel,kernel.Range1(),kernel.Range2(),
					     in,rit.Window().Range1(),rit.Window().Range2());
	int sum = 0;
	for(;it;it++)
	  sum += (it.Data1() * it.Data2());
	*res = (short) sum;
      }
      break;
    }
    CLOSEMMX
  }
  
  template<>
  void Correlate2dC<short,short,short,short>::Apply(const ImageC<short> &in,ImageC<short> &result) const {
    ShortCorrelate2d(in,result,kernel);    
  }
  
  template<>
  void Correlate2dC<short,short,short,int>::Apply(const ImageC<short> &in,ImageC<short> &result) const {
    ShortCorrelate2d(in,result,kernel);    
 }
#endif

  
}
