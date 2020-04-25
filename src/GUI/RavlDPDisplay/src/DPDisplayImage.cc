// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay
//! file="Ravl/GUI/DPDisplay/DPDisplayImage.cc"


#include "Ravl/GUI/DPDisplayView.hh"
#include "Ravl/GUI/RawCanvas.hh"
#include <gtk/gtk.h>
#include "Ravl/DP/Converter.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/UInt16RGBValue.hh"
#include "Ravl/GUI/DPDisplayImage.hh"
#include "Ravl/GUI/MarkupImageRGB.hh"
#include "Ravl/GUI/MarkupImageByte.hh"
#include "Ravl/GUI/MarkupImageByte.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {

  void InitDPDisplayImage()
  {}
  
  //: Constructor
  
  DPDisplayImageRGBBodyC::DPDisplayImageRGBBodyC(const ImageC<ByteRGBValueC> &nimg)
    : img(nimg)
  {
#if 0
    if(!img.IsContinuous()) {
      std::cerr << "DPDisplayImageRGBBodyC::DPDisplayImageRGBBodyC(), WARNING: Image not continuous in memory, making copy. \n";
      img = ImageC<ByteRGBValueC>(img.Copy()); // Make image continuous.
    }
#endif
  }
  
  //: Draw object to canvas.
  
  bool DPDisplayImageRGBBodyC::Draw(FrameMarkupC &markup) {
    markup.Markup().InsLast(MarkupImageRGBC(m_id,0,img));
    return true;
  }
  
  //: Find bounding box for object.
  
  IndexRange2dC DPDisplayImageRGBBodyC::Frame() const
  { return img.Frame(); }

  //: Query a point in the display.
  // Return true if point is within object.
  
  bool DPDisplayImageRGBBodyC::Query(const Vector2dC &pnt,StringC &text) {
    ONDEBUG(std::cerr << "DPDisplayImageRGBBodyC::Query(), Point=" << pnt << "\n");
    Index2dC pix(pnt[0],pnt[1]);
    if(!img.Frame().Contains(pix))
      return false;
    StrOStreamC ss;
    ss << img[pix];
    text = ss.String();
    return true;
  }

  //: Save to a file.
  
  bool DPDisplayImageRGBBodyC::Save(const StringC &str) const {
    return RavlN::Save(str,img);
  }
  
  
  // ---------------------------------------------------------------

  //: Constructor

  DPDisplayImageRealRGBBodyC::DPDisplayImageRealRGBBodyC(const ImageC<RealRGBValueC> &nimg)
    : realImg(nimg),
      img(nimg.Frame())
  {
    RealT maxValue = 0;
    for(Array2dIterC<RealRGBValueC> it(nimg);it;it++) {
      maxValue = Max(it->Red(),maxValue);
      maxValue = Max(it->Green(),maxValue);
      maxValue = Max(it->Blue(),maxValue);
    }
    for(Array2dIter2C<ByteRGBValueC,RealRGBValueC> it(img,realImg);it;it++) {
      it.Data1().Red()   = Max((it.Data2().Red()   * 255.0)/maxValue,0.0);
      it.Data1().Green() = Max((it.Data2().Green() * 255.0)/maxValue,0.0);
      it.Data1().Blue()  = Max((it.Data2().Blue()  * 255.0)/maxValue,0.0);
    }
  }

  //: Draw object to canvas.

  bool DPDisplayImageRealRGBBodyC::Draw(FrameMarkupC &markup) {
    markup.Markup().InsLast(MarkupImageRGBC(m_id,0,img));
    return true;
  }

  //: Find bounding box for object.

  IndexRange2dC DPDisplayImageRealRGBBodyC::Frame() const
  { return img.Frame(); }

  //: Query a point in the display.
  // Return true if point is within object.

  bool DPDisplayImageRealRGBBodyC::Query(const Vector2dC &pnt,StringC &text) {
    ONDEBUG(std::cerr << "DPDisplayImageRGBBodyC::Query(), Point=" << pnt << "\n");
    Index2dC pix(pnt[0],pnt[1]);
    if(!realImg.Frame().Contains(pix))
      return false;
    StrOStreamC ss;
    ss << realImg[pix];
    text = ss.String();
    return true;
  }

  //: Save to a file.

  bool DPDisplayImageRealRGBBodyC::Save(const StringC &str) const {
    return RavlN::Save(str,realImg);
  }


  // ---------------------------------------------------------------

  //: Constructor

  DPDisplayImageRealRGBABodyC::DPDisplayImageRealRGBABodyC(const ImageC<RealRGBAValueC> &nimg)
    : realImg(nimg),
      img(nimg.Frame())
  {
    RealT maxValue = 0;
    for(Array2dIterC<RealRGBAValueC> it(nimg);it;it++) {
      maxValue = Max(it->Red(),maxValue);
      maxValue = Max(it->Green(),maxValue);
      maxValue = Max(it->Blue(),maxValue);
    }
    for(Array2dIter2C<ByteRGBValueC,RealRGBAValueC> it(img,realImg);it;it++) {
      RealRGBAValueC tmp = it.Data2();
      tmp.Red()   = Max((tmp.Red()   * 255.0)/maxValue,0.0);
      tmp.Green() = Max((tmp.Green() * 255.0)/maxValue,0.0);
      tmp.Blue()  = Max((tmp.Blue()  * 255.0)/maxValue,0.0);
      it.Data1().Set(tmp.Red(),tmp.Green(),tmp.Blue());
    }
  }

  //: Draw object to canvas.

  bool DPDisplayImageRealRGBABodyC::Draw(FrameMarkupC &markup) {
    markup.Markup().InsLast(MarkupImageRGBC(m_id,0,img));
    return true;
  }

  //: Find bounding box for object.

  IndexRange2dC DPDisplayImageRealRGBABodyC::Frame() const
  { return img.Frame(); }

  //: Query a point in the display.
  // Return true if point is within object.

  bool DPDisplayImageRealRGBABodyC::Query(const Vector2dC &pnt,StringC &text) {
    ONDEBUG(std::cerr << "DPDisplayImageRealRGBABodyC::Query(), Point=" << pnt << "\n");
    Index2dC pix(pnt[0],pnt[1]);
    if(!realImg.Frame().Contains(pix))
      return false;
    StrOStreamC ss;
    ss << realImg[pix];
    text = ss.String();
    return true;
  }

  //: Save to a file.

  bool DPDisplayImageRealRGBABodyC::Save(const StringC &str) const {
    return RavlN::Save(str,realImg);
  }



  //------------------------------------------------------------------
  // Register some conversions as well.
  
  DPDisplayObjC ImageRGB2DPDisplayImageRGB(const ImageC<ByteRGBValueC> &img) 
  { return DPDisplayImageRGBC(img.Copy()); }
  
  DP_REGISTER_CONVERSION_NAMED(ImageRGB2DPDisplayImageRGB,1,"DPDisplayObjC RavlGUIN::Convert(const ImageC<ByteRGBValueC> &)");

  DPDisplayObjC ImageRGB2DPDisplayImageRealRGB(const ImageC<RealRGBValueC> &img)
  { return DPDisplayImageRealRGBC(img.Copy()); }

  DP_REGISTER_CONVERSION_NAMED(ImageRGB2DPDisplayImageRealRGB,1,"DPDisplayObjC RavlGUIN::Convert(const ImageC<RealRGBValueC> &)");

  DPDisplayObjC ImageRGB2DPDisplayImageRealRGBA(const ImageC<RealRGBAValueC> &img)
  { return DPDisplayImageRealRGBAC(img.Copy()); }

  DP_REGISTER_CONVERSION_NAMED(ImageRGB2DPDisplayImageRealRGBA,1,"DPDisplayObjC RavlGUIN::Convert(const ImageC<RealRGBAValueC> &)");
  
  DPDisplayObjC Image16RGB2DPDisplayImageRGB(const ImageC<UInt16RGBValueC> &img) 
  { 
    ImageC<ByteRGBValueC> tmp(img.Frame());
    for (Array2dIter2C<UInt16RGBValueC,ByteRGBValueC> i(img,tmp); i; ++i)
      i.Data2() = i.Data1()/256;
    return DPDisplayImageRGBC(tmp);
  }
  
  DP_REGISTER_CONVERSION_NAMED(Image16RGB2DPDisplayImageRGB,1,"DPDisplayObjC RavlGUIN::Convert(const ImageC<UInt16RGBValueC> &)");

  //:--------------------------------------------------------------------
  
  //: Constructor
  
  DPDisplayImageRealBodyC::DPDisplayImageRealBodyC(const ImageC<RealT> &nimg)
    : realImg(nimg)
  {
    img = ScaleImage(realImg);
  }
  
  //: Draw object to canvas.
  
  bool DPDisplayImageRealBodyC::Draw(FrameMarkupC &markup) {
    markup.Markup().InsLast(MarkupImageByteC(m_id,0,img));
    return true;
  }
  
  //: Find bounding box for object.
  
  IndexRange2dC DPDisplayImageRealBodyC::Frame() const
  { return img.Frame(); }

  
  //: Offset and scale image, then convert to ByteT image.
  
  ImageC<ByteT> DPDisplayImageRealBodyC::ScaleImage(const ImageC<RealT> &inimg) {
    RealT min = RavlConstN::maxReal;
    RealT max = RavlConstN::minReal;
    for(Array2dIterC<RealT> it(inimg);it;it++) {
      if(min > it.Data())
	min = it.Data();
      if(max < it.Data())
	max = it.Data();
    }
    ImageC<ByteT> tImg(inimg.Rectangle());
    if(min == max) {
      tImg.Fill(128);
      return tImg;
    }
    RealT scale = 255.0/(max - min);
    ONDEBUG(std::cerr << "DPDisplayImageRealBodyC::ScaleImage(), Min:" << min << " Max:" << max << " Scale:" << scale << "\n");
    for(Array2dIter2C<ByteT,RealT> it(tImg,inimg);it;it++) 
      it.Data1() = (ByteT)((it.Data2() - min) * scale);
    return tImg;
  }

  //: Query a point in the display.
  // Return true if point is within object.
  
  bool DPDisplayImageRealBodyC::Query(const Vector2dC &pnt,StringC &text) {
    ONDEBUG(std::cerr << "DPDisplayImageRealBodyC::Query(), Point=" << pnt << "\n");
    Index2dC pix(pnt[0],pnt[1]);
    if(!realImg.Frame().Contains(pix))
      return false;
    text = StringC(realImg[pix]);
    return true;
  }
  
  //: Save to a file.
  
  bool DPDisplayImageRealBodyC::Save(const StringC &str) const {
    return RavlN::Save(str,realImg);
  }

  //------------------------------------------------------------------
  // Register some conversions as well.
  
  DPDisplayObjC ImageReal2DPDisplayImageReal(const ImageC<RealT> &img) 
  { return DPDisplayImageRealC(img.Copy()); }
  
  DP_REGISTER_CONVERSION_NAMED(ImageReal2DPDisplayImageReal,1,"DPDisplayObjC RavlGUIN::Convert(const ImageC<RealT> &)");

  
  //------------------------------------------------------------------

  //: Constructor
  
  DPDisplayImageByteBodyC::DPDisplayImageByteBodyC(const ImageC<ByteT> &nimg)
    : img(nimg)
  {
#if 0
    if(!img.IsContinuous()) {
      std::cerr << "DPDisplayImageByteBodyC::DPDisplayImageByteBodyC(), WARNING: Image not continuous in memory, making copy. \n";
      img = ImageC<ByteT>(img.Copy()); // Make image continuous.
    }
#endif
  }
  
  //: Draw object to canvas.
  
  bool DPDisplayImageByteBodyC::Draw(FrameMarkupC &markup) {
    markup.Markup().InsLast(MarkupImageByteC(m_id,0,img)); 
    return true;
  }
  
  //: Find bounding box for object.
  
  IndexRange2dC DPDisplayImageByteBodyC::Frame() const
  { return img.Frame(); }

  //: Query a point in the display.
  // Return true if point is within object.
  
  bool DPDisplayImageByteBodyC::Query(const Vector2dC &pnt,StringC &text) {
    ONDEBUG(std::cerr << "DPDisplayImageByteBodyC::Query(), Point=" << pnt << "\n");
    Index2dC pix(pnt[0],pnt[1]);
    if(!img.Frame().Contains(pix))
      return false;
    text = StringC((int) img[pix]);
    return true;
  }
  
  //: Save to a file.
  
  bool DPDisplayImageByteBodyC::Save(const StringC &str) const {
    return RavlN::Save(str,img);
  }
  
  //------------------------------------------------------------------
  // Register some conversions as well.
  
  DPDisplayObjC ImageByte2DPDisplayImageByte(const ImageC<ByteT> &img) 
  { return DPDisplayImageByteC(img.Copy()); }
  
  DP_REGISTER_CONVERSION_NAMED(ImageByte2DPDisplayImageByte,1,"DPDisplayObjC RavlGUIN::Convert(const ImageC<ByteT> &)");
  

  //------------------------------------------------------------------

  //: Constructor
  
  DPDisplayImageBoolBodyC::DPDisplayImageBoolBodyC(const ImageC<bool> &nimg)
    : img(nimg)
  {
#if 0
    if(!img.IsContinuous()) {
      std::cerr << "DPDisplayImageBoolBodyC::DPDisplayImageBoolBodyC(), WARNING: Image not continuous in memory, making copy. \n";
      img = ImageC<bool>(img.Copy()); // Make image continuous.
    }
#endif
  }
  
  //: Draw object to canvas.
  
  bool DPDisplayImageBoolBodyC::Draw(FrameMarkupC &markup) {
    ImageC<ByteT> out(img.Frame());
    for (Array2dIter2C<bool,ByteT>i(img,out); i; ++i)
      i.Data2() = (i.Data1() ? 255 : 0);
    markup.Markup().InsLast(MarkupImageByteC(m_id,0,out)); 
    return true;
  }
  
  //: Find bounding box for object.
  
  IndexRange2dC DPDisplayImageBoolBodyC::Frame() const
  { return img.Frame(); }

  //: Query a point in the display.
  // Return true if point is within object.
  
  bool DPDisplayImageBoolBodyC::Query(const Vector2dC &pnt,StringC &text) {
    ONDEBUG(std::cerr << "DPDisplayImageBoolBodyC::Query(), Point=" << pnt << "\n");
    Index2dC pix(pnt[0],pnt[1]);
    if(!img.Frame().Contains(pix))
      return false;
    text = (img[pix] ? "true" : "false");
    return true;
  }
  
  //: Save to a file.
  
  bool DPDisplayImageBoolBodyC::Save(const StringC &str) const {
    return RavlN::Save(str,img);
  }
  
  //------------------------------------------------------------------
  // Register some conversions as well.
  
  DPDisplayObjC ImageBool2DPDisplayImageByte(const ImageC<bool> &img) 
  { return DPDisplayImageBoolC(img.Copy()); }
  
  DP_REGISTER_CONVERSION_NAMED(ImageBool2DPDisplayImageByte,1,"DPDisplayObjC RavlGUIN::Convert(const ImageC<bool> &)");
  

}
  
