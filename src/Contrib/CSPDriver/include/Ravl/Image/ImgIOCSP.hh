// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_IMGIOCSP_HEADER
#define RAVLIMAGE_IMGIOCSP_HEADER 1
//! rcsid="$Id$"
//! lib=CSPDriver
//! docentry="Ravl.API.Images.Video.Video IO.ClipStationPro"
//! author="Charles Galambos"
//! file="Ravl/Contrib/ClipStationPro/ImgIOCSP.hh"
//! example=exImgIOCSP.cc
#include "Ravl/Image/CSPControl.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/YUV422Value.hh"
#include "Ravl/Image/Deinterlace.hh"
#include "Ravl/Image/ImageConv.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include "Ravl/IO.hh"
#include "Ravl/CompositeBuffer.hh"
namespace RavlImageN {
  
  //! userlevel=Develop
  //: ClipStationPro frame grabber input port.
 
  template<class PixelT>
  class DPIImageClipStationProBodyC : public DPIPortBodyC<ImageC<PixelT> >
  {
    
  public:
  
  DPIImageClipStationProBodyC(const StringC &dev) ; 
    //: Constructor.
    //!param: dev   - The name of the hardware device eg. "PCI,card:0", "PCI,card:1" 
    // Multiple handles to the same device are allowed 
    
    ImageC<PixelT> Get(void) ; 
    //: Get next image.
  
    bool Get(ImageC<PixelT> &buff) ; 
    //: Get next image.  
    
    inline bool IsGetReady(void) const
    { return true; }
    //: Is some data ready ?
    // true = yes.
 
    
    inline  bool IsGetEOS(void) const
      { return (const_cast<ClipStationProDeviceC &> (cspDevice)).Device() == 0; }
    // : Has the End Of Stream been reached ?
    // true = yes.


    inline bool GetAttr(const StringC &attrName,StringC &attrValue){  
      if (cspDevice.GetAttrIn (attrName, attrValue)) 
	return true ;
      return DPPortBodyC::GetAttr(attrName, attrValue) ; } 
    //: Get an  Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,bool & attrValue){
      StringC val(attrValue)  ; 
      if (cspDevice.GetAttrIn (attrName, val)) 
	return true ;
      return AttributeCtrlBodyC::GetAttr(attrName, attrValue) ; }  
    //: Get an Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,RealT & attrValue){
      StringC val(attrValue) ; 
      if (cspDevice.GetAttrIn (attrName, val)) 
	return true ;
      return DPPortBodyC::GetAttr(attrName, attrValue) ; }  
    //: Get an Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,IntT & attrValue){
      StringC val (attrValue) ; 
      if (cspDevice.GetAttrIn (attrName,val )) 
	  return true ;
      return DPPortBodyC::GetAttr(attrName, attrValue) ; } 
    //: Get an Attribute Value 
    
    inline StringC GetAttr(const StringC & attrName)
      { StringC attrValue ; GetAttr (attrName, attrValue) ; return attrValue ; }  
    //: Get an Attribute Value 
    

    
    inline bool SetAttr(const StringC &attrName,const StringC &attrValue) { 
      if (cspDevice.SetAttrIn( attrName, attrValue)) 
	return true ; 
      return DPPortBodyC::SetAttr(attrName,attrValue) ; }
    //: Set an Attribute Value 
    
  
    
    inline bool SetAttr(const StringC & attrName,const IntT & attrValue){
      StringC val (attrValue) ; 
      if (cspDevice.SetAttrIn( attrName, val)) 
	return true ; 
      return DPPortBodyC::SetAttr(attrName,attrValue) ; } 
    //: Set an Attribute Value 
    
    inline bool SetAttr(const StringC & attrName,const RealT & attrValue){
      StringC val(attrValue) ; 
      if (cspDevice.SetAttrIn( attrName, val)) 
	return true ; 
      return DPPortBodyC::SetAttr(attrName,attrValue) ; } 
    //: Set an Attribute Value 
    
    bool SetAttr(const StringC & attrName,const bool & attrValue){
      StringC val(attrValue) ; 
      if (cspDevice.SetAttrIn( attrName, val)) 
	return true ; 
      return DPPortBodyC::SetAttr(attrName,attrValue) ; } 
    //: Set an attribute Value 
    
  protected: 

    ClipStationProDeviceC cspDevice ; // a handle to the csp device 

  };
  

  


  //! userlevel=Develop
  //: ClipStationPro frame grabber output port.
  template<class PixelT>
  class DPOImageClipStationProBodyC : public DPOPortBodyC<ImageC<PixelT> >
  {
    
  public:
    DPOImageClipStationProBodyC(const StringC &dev) 
      : cspDevice ( dev) { cspDevice.BuildAttributesOut(*this) ;} 
    //: Constructor.
    //!param: dev   - The name of the hardware device eg. "PCI,card:0", "PCI,card:1"

    
    bool Put( const ImageC<PixelT> & img) ; 
    //: Put an image on the card output 
    

    inline bool IsPutReady(void) const
      { return true; }
    //: Is some data ready ?
    // true = yes.

    
    inline  bool IsGetEOS(void) const
      { return cspDevice.Device() == 0; }
    // : Has the End Of Stream been reached ?
    // true = yes.
    

    
    inline bool GetAttr(const StringC &attrName,StringC &attrValue){  
      if (cspDevice.GetAttrOut (attrName, attrValue)) 
	return true ;
      return DPPortBodyC::GetAttr(attrName, attrValue) ; } 
    //: Get an  Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,bool & attrValue){
      StringC val (attrValue) ; 
      if (cspDevice.GetAttrOut (attrName, val)) 
	return true ;
      return AttributeCtrlBodyC::GetAttr(attrName, attrValue) ; }  
    //: Get an Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,RealT & attrValue){
      StringC val (attrValue) ; 
      if (cspDevice.GetAttrOut (attrName, val) ) 
	return true ;
      return DPPortBodyC::GetAttr(attrName, attrValue) ; }  
    //: Get an Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,IntT & attrValue){
      StringC val (attrValue) ; 
      if (cspDevice.GetAttrOut (attrName, val)) 
	return true ;
      return DPPortBodyC::GetAttr(attrName, attrValue) ; } 
    //: Get an Attribute Value 
    
    inline StringC GetAttr(const StringC & attrName)
      { StringC attrValue ; GetAttr (attrName, attrValue) ; return attrValue ; }  
    //: Get an Attribute Value 
    

    
    inline bool SetAttr(const StringC &attrName,const StringC &attrValue) { 
      if (cspDevice.SetAttrOut( attrName, attrValue)) 
	return true ; 
      return DPPortBodyC::SetAttr(attrName,attrValue) ; }
    //: Set an Attribute Value 
    
    
    inline bool SetAttr(const StringC & attrName,const IntT & attrValue){
      StringC val(attrValue) ; 
      if (cspDevice.SetAttrOut( attrName, val)) 
	return true ; 
      return DPPortBodyC::SetAttr(attrName,attrValue) ; } 
    //: Set an Attribute Value 
    
    inline bool SetAttr(const StringC & attrName,const RealT & attrValue){
      StringC val(attrValue) ; 
      if (cspDevice.SetAttrOut( attrName, val)) 
	return true ; 
      return DPPortBodyC::SetAttr(attrName,attrValue) ; } 
    //: Set an Attribute Value 
    
    inline bool SetAttr(const StringC & attrName,const bool & attrValue){
      StringC val(attrValue) ; 
      if (cspDevice.SetAttrOut( attrName, val)) 
	return true ; 
      return AttributeCtrlBodyC::SetAttr(attrName,attrValue) ; } 
    //: Set an attribute Value
  
  protected: 

    ClipStationProDeviceC cspDevice ; // a handle to the csp device 

  };
  






  

  //! userlevel=Normal
  //: ClipStationPro frame grabber input port.
  // This class provides a convenient user interface for access to the ClipStationPro capture cards. 
  // This interface provides attributes such as "timecode" and "FrameBufferSize" which can be accessed/modified through the 
  // GetAttr SetAttr methods. 
  // See the data processing <a href="../Tree/Ravl.API.Core.Data_Processing.Attributes.html"> attribute handling mechanism. </a>  For more details. 
  // Also see the <a href="../Tree/Ravl.API.Images.Video.html"> video section </a> for a list of common attributes. 
  // 
  
  template<class PixelT>
  class DPIImageClipStationProC
    : public DPIPortC<ImageC<PixelT> >
  {
  public:
    DPIImageClipStationProC(void)
      : DPEntityC(true) {}
    //: Default constructor.
    // creates an invalid handle.
    
    
    DPIImageClipStationProC(const StringC &dev)
      : DPEntityC(*new DPIImageClipStationProBodyC<PixelT>(dev)) {}
    //: Constructor.
    //!param: dev - The name of the hardware device eg. "PCI,card:0", "PCI,card:1" - multiple accesses to single device are possible 
    
    
  protected:
    DPIImageClipStationProBodyC<PixelT> &Body(void)
      { return dynamic_cast<DPIImageClipStationProBodyC<PixelT> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIImageClipStationProBodyC<PixelT> &Body(void) const
    { return dynamic_cast<const DPIImageClipStationProBodyC<PixelT> &>(DPEntityC::Body()); }
    //: Access body.
  public:  
    

inline bool GetAttr(const StringC &attrName,StringC &attrValue)
      { return Body().GetAttr(attrName, attrValue) ; } 
    //: Get an attribute value 

    inline bool GetAttr(const StringC & attrName,bool & attrValue)
      { return Body().GetAttr( attrName, attrValue) ; } 
    //: Get an Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,RealT & attrValue)
      { return Body().GetAttr( attrName, attrValue) ; } 
    //: Get an Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,IntT & attrValue)
      { return Body().GetAttr( attrName, attrValue) ; } 
    //: Get an Attribute Value 
    
    inline StringC GetAttr(const StringC & attrName)
      { return Body().GetAttr( attrName ) ; }  
    //: Get an Attribute Value 
    

    
    inline bool SetAttr(const StringC &attrName,const StringC &attrValue) 
      { return Body().SetAttr(attrName, attrValue) ; } 
    //: Set an Attribute Value 
    
    
    inline bool SetAttr(const StringC & attrName,const IntT & attrValue)
      { return Body().SetAttr(attrName, attrValue) ; } 
    //: Set an Attribute Value 
    
    inline bool SetAttr(const StringC & attrName,const RealT & attrValue)
      { return Body().SetAttr(attrName, attrValue) ; } 
    //: Set an Attribute Value 
    
    inline bool SetAttr(const StringC & attrName,const bool & attrValue)
      { return Body().SetAttr(attrName, attrValue) ; } 
    //: Set an attribute Value 

  };







  
  //! userlevel=Normal
  //: ClipStationPro frame grabber output port.
  // This class provides a convenient user interface for access to the ClipStationPro capture cards. 
  // This interface provides attributes such as "timecode" and "FrameBufferSize" which can be accessed/modified through the 
  // GetAttr SetAttr methods. 
  // See the data processing <a href="../Tree/Ravl.API.Core.Data_Processing.Attributes.html"> attribute handling mechanism. </a>  For more details. 
  // Also see the <a href="../Tree/Ravl.API.Images.Video.html"> video section </a> for a list of common attributes. 
  // The card can be switched into different modes through the attribute mechanism 
  // LIVE 1 - sets card into live mode 
  // COLOURBARS 1 sets card into colour bar mode 
  // BLACK 1 sets card raster to black 
  // DELAY 1 sets card into delay mode 
  
  template<class PixelT>
  class DPOImageClipStationProC
    : public DPOPortC<ImageC<PixelT> >
  {
  public:
    DPOImageClipStationProC(void)
      : DPEntityC(true){}
    //: Default constructor.
    // creates an invalid handle.
    
    
    DPOImageClipStationProC(const StringC &dev)
      : DPEntityC(*new DPOImageClipStationProBodyC<PixelT>(dev)){}
    //: Constructor.
    //!param: dev   - The name of the hardware device eg. "PCI,card:0", "PCI,card:1"
    
  protected:
    DPIImageClipStationProBodyC<PixelT> &Body(void)
      { return dynamic_cast<DPOImageClipStationProBodyC<PixelT> &>(DPEntityC::Body()); }
    //: Access body.

    const DPIImageClipStationProBodyC<PixelT> &Body(void) const
      { return dynamic_cast<const DPOImageClipStationProBodyC<PixelT> &>(DPEntityC::Body()); }
    //: Access body.
  public:  
    
    inline bool GetAttr(const StringC &attrName,StringC &attrValue)
      { return Body().GetAttr(attrName, attrValue) ; } 
    //: Get an attribute value 

    inline bool GetAttr(const StringC & attrName,bool & attrValue)
      { return Body().GetAttr( attrName, attrValue) ; } 
    //: Get an Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,RealT & attrValue)
      { return Body().GetAttr( attrName, attrValue) ; } 
    //: Get an Attribute Value 
    
    inline bool GetAttr(const StringC & attrName,IntT & attrValue)
      { return Body().GetAttr( attrName, attrValue) ; } 
    //: Get an Attribute Value 
    
    inline StringC GetAttr(const StringC & attrName)
      { return Body().GetAttr( attrName ) ; }  
    //: Get an Attribute Value 
    

    
    inline bool SetAttr(const StringC &attrName,const StringC &attrValue) 
      { return Body().SetAttr(attrName, attrValue) ; } 
    //: Set an Attribute Value 
    
    
    inline bool SetAttr(const StringC & attrName,const IntT & attrValue)
      { return Body().SetAttr(attrName, attrValue) ; } 
    //: Set an Attribute Value 
    
    inline bool SetAttr(const StringC & attrName,const RealT & attrValue)
      { return Body().SetAttr(attrName, attrValue) ; } 
    //: Set an Attribute Value 
    
    inline bool SetAttr(const StringC & attrName,const bool & attrValue)
      { return Body().SetAttr(attrName, attrValue) ; } 
    //: Set an attribute Value 
  };


  
    
  //: Get next image.
  //---------------------------------------------------------------------------------------------------------------------------------------
  template <class PixelT> 
  ImageC<PixelT> DPIImageClipStationProBodyC<PixelT>::DPIImageClipStationProBodyC::Get(void) {
  //cerr << "\n in Get(void) " ; 
  DMABufferC<char> buf = cspDevice.GetFrameGeneric();

  
  if(!buf.IsValid())
   	return ImageC<PixelT>();
  const ImageRectangleC & rect = cspDevice.Rectangle() ;   
  CompositeBufferC<PixelT> comp ( 1,rect.Rows() * rect.Cols(), (PixelT*) & buf.BufferAccess()[0]  ) ; 
  comp.SetElement(0, buf.Abstract() ) ; 
  ImageC<PixelT> img ( rect.Rows(), rect.Cols(), comp ) ; 
  return Interlace(img).Copy();
  }
  
  //: Get next image - now uses char buffers 
   //---------------------------------------------------------------------------------------------------------------------------------------
  template <class PixelT> 
  bool DPIImageClipStationProBodyC<PixelT>::DPIImageClipStationProBodyC::Get(ImageC<PixelT> &buff) {
  buff = Get() ; 
  return true ; 
  }
  
  
  //: Put next image 
  //---------------------------------------------------------------------------------------------------------------------------------------
  template <class PixelT> 
  bool DPOImageClipStationProBodyC<PixelT>::DPOImageClipStationProBodyC::Put( const ImageC<PixelT> & image) 
  {
  cerr << "\n in put() " << endl << "buffersize " << cspDevice.BufferSize() << "\t" << cspDevice.AlignDMA()  ; 
  DMABufferC<char> buffer ( cspDevice.BufferSize() , cspDevice.AlignDMA()) ;
  UIntT offset = 0 ;                                                   // offset into buffer 
  UIntT field2 = (image.Rows() / 2) * image.Cols() * sizeof(PixelT) ; // offset in samples of the second field 
  UIntT fieldCount = 1 ;

  
  // an iterator over rows 
  BufferAccess2dIterC<PixelT> iter2d ( image, image.Range2() ) ; 
     for (  ; iter2d.NextRow() ; ++fieldCount ) //iter2d.NextRow() ) 
  {
         RangeBufferAccessC<PixelT> rowAccess = iter2d.Row() ; // access the first row of the image 
         memcpy ( buffer.ReferenceElm() + offset , rowAccess.DataStart(), rowAccess.Size()*sizeof(PixelT) ) ; 
	 if (!iter2d.NextRow() ) break ;
	 rowAccess = iter2d.Row() ; 
       memcpy ( buffer.ReferenceElm() + offset + field2 , rowAccess.DataStart(), rowAccess.Size()*sizeof(PixelT) ) ; 
       offset += rowAccess.Size() * sizeof(PixelT) ; 
    } 
   cspDevice.PutFrame(buffer) ; 
   return true ;	 
  }
  

};


#endif
