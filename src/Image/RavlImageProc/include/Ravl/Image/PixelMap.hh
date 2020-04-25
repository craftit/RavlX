// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLPIXELMAP_HEADER 
#define RAVLPIXELMAP_HEADER 1
/////////////////////////////////////////////////////////
//! file="Ravl/Image/Processing/Lines/PixelMap.hh"
//! date="16/7/97"
//! docentry="Ravl.API.Images.Lines"
//! author="Charles Galambos"
//! lib=RavlImageProc
//! rcsid="$Id$"

#include "Ravl/BinList.hh"
#include "Ravl/Index2d.hh"
#include "Ravl/Point2d.hh"

namespace RavlImageN {
  using namespace RavlN;
  
  template<class PixelT> class ImageC;
  
  /////////////////////
  //! userlevel=Normal
  //: Binned hash table of pixels in an image.
  // SMALL OBJECT
  
  class PixelMapC  {
  public:
    inline PixelMapC()
      : local(Point2dC(8,8))
      {}
    //: Default constructor.
    
    inline PixelMapC(const Point2dC &nBinSize)
      : local(nBinSize)
      {}
    //: Constructor.
    
    inline PixelMapC(RealT corridor)
      : local(Point2dC(corridor,corridor))
      {}
    //: Constructor.
    
    const PixelMapC &operator= (const ImageC<ByteT> &img);
    //: Clean map and put all pixels with non-zero values.
    //: into map.
    
    inline void Insert(const Index2dC &pix)
      { local.Insert(Point2dC(pix),pix); }
    //: Put a new pixel into map.
    
    inline DListC<Index2dC > &Bin(const Point2dC &pnt)
      { return local[pnt]; }
    //: List pixels in bin around Pnt.
    
    inline Index2dC BinKey(const Point2dC &pnt) 
      { return local.BinKey(pnt); }
    //: Pnt to bin conversion. FIXME :- Should be 'const'.
  
    inline Point2dC BinSize() const 
      { return local.BinSize(); }
    //: Get size of bins.
    
    inline Point2dC BinCentre(const Point2dC &at) const 
      { return local.BinCentre(at); }
    //: Get centre of bin.
    
    void Dump(ostream &Out);
    //: Dump pixels to stream.
    
    ImageC<ByteT>  Image(ByteT forground = 255,ByteT background = 0);
    //: Create an image with pixels marked.
    
    inline void Empty()
      { local.Empty(); }
    //: Empty the map of all data.
    
  private:
    BinListC<Point2dC,Index2dC,Index2dC> local;
    
    friend class PixelMapSearchBaseC;
  };
  
  
}

#endif
