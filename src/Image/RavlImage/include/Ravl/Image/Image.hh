// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMAGE_HEADER
#define RAVLIMAGE_IMAGE_HEADER 1
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Image/Base/Image.hh"
//! lib=RavlImage
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"
//! docentry="Ravl.API.Images"
//! example=exImage.cc

#include "Ravl/Array2d.hh"
#include "Ravl/TFVector.hh"
#include "Ravl/Image/ImageRectangle.hh"
#include "Ravl/Array2dIter2.hh"

namespace RavlImageN {
  using namespace RavlN;
#if RAVL_VISUALCPP_NAMESPACE_BUG
  using RavlN::Array2dC;
  using RavlN::UIntT;
  using RavlN::IndexRangeC;
  using RavlN::BufferC;
  using RavlN::TFVectorC;
  using RavlN::RealT;
  using RavlN::ByteT;
#endif
  
  //! userlevel=Normal
  //: Basic Image
  // ImageC is an extension of <a href="RavlN.Array2dC.html">Array2dC</a>.  The main extra feature is that you can have several different handles to the same image, each with a different bounding rectangle.
  
  template <class PixelT>
  class ImageC
    : public Array2dC<PixelT>
  {
  public:
    ImageC()
    {}
    //: Default constructor.
    // creates an empty image.
    
    ImageC(const Array2dC<PixelT> &oth)
      : Array2dC<PixelT>(oth)
    {}
    //: Base constructor.
    //!param: oth - Array to be treated as an image.
    // Note: the array is NOT copied.
    
    ImageC(const IndexRange2dC &rng)
      : Array2dC<PixelT>(rng)
    {}
    //: Create an image of size 'rng'
    //!param: rng - 2d range of accesses to create the image over
    
    ImageC(const IndexRange2dC &rng,const PixelT &val)
      : Array2dC<PixelT>(rng)
    { this->Fill(val); }
    //: Create an image of size 'rng' filled with value 'val'
    //!param: rng - 2d range of accesses to create the image over
    //!param: val - Value to fill the image with.
    
    ImageC(const Array2dC<PixelT> &oth,const IndexRange2dC &rect)
      : Array2dC<PixelT>(oth,rect)
    {}
    //: Construct an image as a sub rectangle 'rect' on oth.
    //!param: oth - Original image.
    //!param: rect - Sub rectangle within the original image which will accessed by the new handle.
    // Note: This does NOT copy the image area, it just creates a more limited access to the existing image.
    
    ImageC(SizeC rows,SizeC cols)
      : Array2dC<PixelT>(rows,cols)
    {}
    //: Construct an image with origin at top left corner and size rows x cols.
    //!param: rows - Number of rows in the image.
    //!param: cols - Number of column's in the image
    
    ImageC(const IndexRangeC &rows,const IndexRangeC &cols)
      : Array2dC<PixelT>(rows,cols)
    {}
    //: Construct an image with the given range of indexes.
    //!param: rows - Range of rows to construct image over.
    //!param: cols - Range of column's to construct image over.
    
    ImageC(UIntT rows,UIntT cols,const BufferC<PixelT> &buf)
      : Array2dC<PixelT>(IndexRange2dC(0,rows-1,0,cols-1),buf)
    {}
    //: Construct an image with origin at top left corner and size rows x cols, with space in 'buf'
    //!param: rows - Number of rows in the image.
    //!param: cols - Number of column's in the image
    //!param: buf - Raw pixel buffer, must contain at least row * cols elements.

    ImageC(UIntT rows,UIntT cols,const SArray1dC<PixelT> &buf)
      : Array2dC<PixelT>(IndexRange2dC(0,rows-1,0,cols-1),buf)
    {}
    //: Construct an image with origin at top left corner and size rows x cols, with space in 'buf'
    //!param: rows - Number of rows in the image.
    //!param: cols - Number of column's in the image
    //!param: buf - Raw pixel buffer, must contain at least row * cols elements.

    ImageC(UIntT rows,UIntT cols,PixelT *data,bool deletable = true)
      : Array2dC<PixelT>(IndexRange2dC(0,rows-1,0,cols-1),BufferC<PixelT>(rows * cols,data,false,deletable))
    {}
    //: Construct an image with origin at top left corner and size rows x cols, with space in 'buf'
    //!param: rows - Number of rows in the image.
    //!param: cols - Number of column's in the image
    //!param: data - Pointer to continuous array of memory containing image data (rows * cols pixels).
    //!param: deletable - If set to true the memory will be deleted with the 'delelte []' operator when is finished with. Otherwise it is the users responsibility to manage the memory.
    
    ImageC<PixelT> Copy() const
    { return ImageC<PixelT>(Array2dC<PixelT>::Copy()); }
    //: Make copy of an image.

    ImageC<PixelT> Expand(IntT width, PixelT fill) const {
      ImageC<PixelT> expanded(Frame().Expand(width));
      expanded.Fill(fill);
      for (Array2dIter2C<PixelT,PixelT> i(*this, expanded, Frame()); i; ++i)
        i.Data2() = i.Data1();
      return expanded;
    }
    //: Expand image by "width" pixels and fill border with "fill"
    // The returned image is a new image, not a link to this one.
    
    ImageC<PixelT> Expand(IntT width) const {
      ImageC<PixelT> expanded(Frame().Expand(width));
      for (Array2dIter2C<PixelT,PixelT> i(*this, expanded, Frame()); i; ++i)
        i.Data2() = i.Data1();
      return expanded;
    }
    //: Expand image by "width" pixels, but do not initialise border.
    // The returned image is a new image, not a link to this one.

    SizeT Rows() const
    { return this->Range1().Size(); }
    //: Number of rows in image.
    
    SizeT Cols() const
    { return this->Range2().Size(); }
    //: Number of cols in image.
    
    inline IndexC TRow() const
    { return this->Range1().Min(); }
    // Returns the top row index.
    
    inline IndexC LCol() const
    { return this->Range2().Min(); }
    // Returns the left side column index.
    
    inline IndexC BRow() const
    { return this->Range1().Max(); }
    // Returns the bottom row index.
    
    inline IndexC RCol() const
    { return this->Range2().Max(); }
    // Returns the right side column index.

    inline ImageRectangleC Frame() const
    { return Array2dC<PixelT>::Frame(); }
    // Returns the frame of the image

    inline ImageRectangleC Rectangle() const
    { return Array2dC<PixelT>::Rectangle(); }
    // Returns the frame of the image

    PixelT *Row(IndexC row) 
    { return &(RangeBufferAccessC<BufferAccessC<PixelT> >::operator[](row)[this->rng2.Min()]); }
    //: Get a pointer to begining of row.
    // Note: this does not give access to the ImageC row pointer; to access the pointer itself use <a href="#RowPtrObIndexCCb">RowPtr</a> or <a href="#SetRowPtrObIndexC_const_BufferAccessCStDataTBt_RefCb">SetRowPtr</a>
    //!param: row - Row for which the pointer is returned.
    
    const PixelT *Row(IndexC row) const
    { return &(RangeBufferAccessC<BufferAccessC<PixelT> >::operator[](row)[this->rng2.Min()]); }
    //: Get a pointer to begining of row for const image.
    // Note: this does not give access to the ImageC row pointer; to access the pointer itself use <a href="#RowPtrObIndexCCb">RowPtr</a> or <a href="#SetRowPtrObIndexC_const_BufferAccessCStDataTBt_RefCb">SetRowPtr</a>
    //!param: row - Row for which the pointer is returned.
    
    ImageC<PixelT> Rotate90(Index2dC centre = Index2dC(0,0)) const;
    //: The values of image "originalImage" rotated about PI/2 rad 
    //: and shifted to have the original upper-left corner and
    //: are saved into this image.
    
    ImageC<PixelT> Rotate180(Index2dC centre = Index2dC(0,0)) const;
    //: Create a copy of the image which is rotated 180 degree's.
    // The image is rotated around the centre given.
    //!param: centre - Centre arround which to rotate.
    
    ImageC<PixelT> Rotate270(Index2dC centre = Index2dC(0,0)) const;
    //: The values of image "originalImage" rotated about 3/2*PI rad 
    //: and shifted to have the original upper-left corner and
    //: are saved into this image.
  };
  
  template <class PixelT>
  ImageC<PixelT> ImageC<PixelT>::Rotate180(Index2dC centre) const {    
    ImageC<PixelT> flipped(Rectangle().Rotate180(centre));
    BufferAccess2dIterC<PixelT> it((*this),this->Range2());
    IntT frow = flipped.BRow().V();
    PixelT *place = &(flipped[frow][flipped.RCol()]);
    while(it.IsElm()) {
      *place = it.Data();
      if(it.Next()) {
        place--;
        continue;
      }
      // Next row ?
      if(!it.IsElm())
        break;
      frow--;
      place = &(flipped[frow][flipped.RCol()]);
    }
    return flipped;
  }
  
  
  template <class PixelT>
  ImageC<PixelT> ImageC<PixelT>::Rotate90(Index2dC centre) const
  {
    Index2dC org(this->Frame().Origin().Col() - centre.Col() + centre.Row(),-this->Frame().Origin().Row() + centre.Col() + centre.Row());
    ImageC<PixelT> flipped(Rectangle().Rotate90(centre));
    Index2dC at = org;
    for(BufferAccess2dIterC<PixelT> it((*this),this->Range2());it;) {
      do {
        flipped[at] = *it;
        at.Row()++;
      } while(it.Next()) ;
      at.Row() = org.Row();
      at.Col()--;
    }
    return flipped;
  }
  
  template <class PixelT>
  ImageC<PixelT> ImageC<PixelT>::Rotate270(Index2dC centre) const
  {   
    Index2dC org(-this->Frame().Origin().Col() + centre.Col() + centre.Row(),this->Frame().Origin().Row() - centre.Col() + centre.Row());
    ImageC<PixelT> flipped(Rectangle().Rotate270(centre));
    Index2dC at = org;
    for(BufferAccess2dIterC<PixelT> it((*this),this->Range2());it;) {
      do {
        flipped[at] = *it;
        at.Row()--;
      } while(it.Next()) ;
      at.Col()++;
      at.Row() = org.Row();
    }
    return flipped;
  }
  
  
  
  std::ostream &operator<<(std::ostream &s,const ImageC<ByteT> &img);
  //: Save byte image to stream 
  // Override default IO to handle byte images correctly.
  
  std::istream &operator>>(std::istream &s,ImageC<ByteT> &img);
  //: Load byte image from stream 
  // Override default IO to handle byte images correctly.

  BinOStreamC &operator<<(BinOStreamC &out,const ImageC<ByteT> &img);
  //: Save byte image to binary stream 
  
  BinIStreamC &operator>>(BinIStreamC &in,ImageC<ByteT> &img);  
  //: Load byte image from binary stream 
  
}

#endif
