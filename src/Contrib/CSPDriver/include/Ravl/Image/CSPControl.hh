// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_CSPCONTROL_HEADER
#define RAVLIMAGE_CSPCONTROL_HEADER 1
////////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Images.Video.Video IO.ClipStationPro"
//! lib=CSPDriver
//! file="Ravl/Contrib/ClipStationPro/CSPControl.hh"
//! example=exCSPControl.cc

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteYUV422Value.hh"
#include "Ravl/TimeCode.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/String.hh"
#include "Ravl/OS/DMABuffer.hh"

extern "C" {
#include <dvs_clib.h>
#include <dvs_fifo.h>
}


namespace RavlN {
  template<class DataT> class DListC; 
  class AttributeCtrlBodyC ;
}
// forward delclarations 


namespace RavlImageN {
  


 enum CSPModeT { BLACK, COLOURBAR, LIVE, DELAY }  ; 

 
    //: some diagnostic modes 
    //:!param BLACK - Sets video raster to black 
    //:!param COLOURBAR - Sets video raster to generate colourbars 
    //:!param LIVE - Sets video raster to live mode (ouput mirros input) 
    //:!param DELAY - Sets video raster output to mirror video input but with a 1 frame delay 

 enum ColourModeT { YUV, RGB } ; 

  //! userlevel=Develop
  //: Clip station pro control class body.
  //: This class is ThreadSafe
  class ClipStationProDeviceBodyC : public RCBodyC {
    
  public:

  
    ClipStationProDeviceBodyC(const StringC &devName, ColourModeT mode);
    //: Constructor.
    //:!param devName - The name of the device, typical form is "PCI,card:0", "PCI,card:1"
    
    ~ClipStationProDeviceBodyC();
    //: Destructor.
    
    DMABufferC<ByteYUV422ValueC> GetFrame() const;
    //: Get one field of video.
    
    DMABufferC<char> GetFrameGeneric() const ; 
    //: Get one frame of video . 
        
    ColourModeT ColourMode ( void ) const { return colourMode ; }  
    //: Queries to see what mode the card is in
    
    bool  PutFrame (const DMABufferC<ByteYUV422ValueC> & buffer ) const ; 
    //: Put one frame of video to the output of the card. 
    //!depricated: 
   
    bool PutFrame (const DMABufferC<char> & buffer ) const ; 
    //: Put one frame of video to the output of the card. 
    
    bool BuildAttributesIn (AttributeCtrlBodyC & attrCtrl) ; 
    //: Builds the list of available attributes for an input 

    bool GetAttrIn (const StringC & attrName,  StringC & attrValue ) const ; 
    //: Gets the value for a given output attribute 

    bool SetAttrIn (const StringC & attrName, const StringC & attrValue ) ; 
    //: Sets the value for a given input attribute 

    bool BuildAttributesOut (AttributeCtrlBodyC & attrCtrl) ; 
    //: Builds the list of available attributes for output 

    bool GetAttrOut (const StringC & attrName,  StringC & attrValue) const; 
    //: Gets the value for a given output attribute 

    bool SetAttrOut (const StringC & attrName, const StringC & attrValue) ; 
    //: Sets the value for a given output attribute 
    
    bool SetMode (CSPModeT) ; 
    //: Sets the mode of card
    // Usefull for test signals. 
    
    UIntT AlignDMA (void) const { return alignDMA ; } 
    //: Access teh dma alignment  
    
    UIntT BufferSize(void) const { return fifo_config.vbuffersize + fifo_config.abuffersize; }
    //: Access the buffer size for the current setup 
    
    DMABufferC<ByteYUV422ValueC> GetDMABuffer(void) const 
      { 
	IntT dmaBufferSize = fifo_config.vbuffersize + fifo_config.abuffersize;
	return DMABufferC<ByteYUV422ValueC> (dmaBufferSize / sizeof(ByteYUV422ValueC), alignDMA) ; }
    //!depricated: 
    //: A usefull method which returns a buffer suitable for DMA transfers 
    // The returned buffer has the correct size and dma alignment for the underlying hardware

    DMABufferC<char> GetDMABufferGeneric(void) const 
    {
    IntT dmaBufferSize = fifo_config.vbuffersize + fifo_config.abuffersize ; 
    return DMABufferC<char> (dmaBufferSize, alignDMA ) ; 
    }
      //: A usefull method which returns a buffer suitable for DMA transfers 
    // The returned buffer has the correct size and dma alignment for the underlying hardware
    // The space allocated by this buffer will 
    
    const ImageRectangleC & Rectangle(void) const 
      { return rect ; }
    //: Access the iamge Rectangle const 

    
    //ImageRectangleC & Rectangle(void) 
    // { return rect ; } 
    //: Access the image Rectangle 

    sv_handle * Device(void) 
      { return dev ; } 
    //: Access the low level device handle 
    //!bug Use with care ! 

    //ClipStationProDeviceBodyC & operator = (const ClipStationProDeviceC & other ) ; 
    //: assignment operator
   
  protected :
    

    bool Init(void);
    //: Setup video modes to a suitable default.
    // does not lock class


    bool CheckError( int retVal, const char * errorMessage, ostream & output = cerr ) const ; 
    //: Checks the return value retVal, of sv device functions
    //: in event of an error, errorMessage is printed to output and false is returned  


    
  protected:
    
    StringC deviceName ;               // The device name

    sv_handle *dev;                   // the sv_device 
    sv_fifo * inputFifo;                    // the fifo 
    sv_fifo * outputFifo ; 
    sv_fifo_configinfo fifo_config;   // info about the fifo 
    
    ImageRectangleC rect;             // The capture rectangle 

    const bool useDMA;                // Use DMA transfer's ?
    const bool doInput;               // do Input 
    const bool fifoMode;              // Use fifo ?
    const bool sharedFifo ;           // are the input and output fifos shared ? 
    const bool captureAudio;          // Capture audio ?
    const bool captureVideo;          // Capture video ?

    int frameBufferSize;              // size of framebuffer 

    mutable TimeCodeC timecode_from_getframe; // Timecode of last grabbed frame
    
    mutable RWLockC rwLock ;                   // a resource lock 

    const UIntT alignDMA ; 
    
    ColourModeT colourMode ; 

};
  
  
  
  
  
  
  //! userlevel=Develop
  //: Clip station pro control class handle.
  class ClipStationProDeviceC : public RCHandleC<ClipStationProDeviceBodyC> 
  {    
    
  public: 
    
    ClipStationProDeviceC ( const StringC &devName, ColourModeT mode = YUV ) ; 
    // : RCHandleC<ClipStationProDeviceBodyC> ( new ClipStationProDeviceBodyC(devName) ) {}
    //: Constructor.
    //:!param devName - The name of the device, typical form is "PCI,card:0", "PCI,card:1"
   
    ColourModeT ColourMode (void) const { return Body().ColourMode() ; } 
    //: Access the colour mode 
    
    //inline bool GetFrame(void *buff,int x,int y)
    // { return Body().GetFrame( buff,x,y) ; } 
    //: Get one frame of video.
    
    inline DMABufferC<ByteYUV422ValueC> GetFrame(void)
      { return Body().GetFrame() ; } 
    //!depricated: 
    //: Get one frame of video.
    
    inline DMABufferC<char> GetFrameGeneric(void) 
    { return Body().GetFrameGeneric() ; }
    //: Get one frame of video from card
   
    
    inline bool PutFrame(const DMABufferC<ByteYUV422ValueC> & buff) 
      { return Body().PutFrame(buff) ; } 
      //!depricated: 
//: Put one frame of video onto the output of the card
//: This uses dma and is fast. 

inline bool PutFrame( const DMABufferC<char> & buff )
{ return Body().PutFrame(buff) ; } 
//: Put one frame of video onto the output of the card. 

    
inline bool SetMode (CSPModeT mode) 
{ return Body().SetMode(mode) ; }  
    //: Sets the mode of card
    // Usefull for test signals. 

//inline bool PutFrame(void *buff,int x,int y) 
//{ return Body().PutFrame( buff,x,y) ; } 
//: Put one frame of video to the output of the card .

inline bool BuildAttributesIn (AttributeCtrlBodyC & attrCtrl) 
{ return Body().BuildAttributesIn (attrCtrl) ; } 
//: Builds the list of available attributes for an input 

inline bool GetAttrIn (const StringC & attrName, StringC & attrValue ) const
{ return Body().GetAttrIn(attrName, attrValue) ; }  
//: Gets the value for a given output attribute 

inline bool SetAttrIn (const StringC & attrName, const StringC & attrValue )
{ return Body().SetAttrIn (attrName, attrValue) ; }  
    //: Sets the value for a given input attribute 

inline bool BuildAttributesOut (AttributeCtrlBodyC & attrCtrl) 
{ return Body().BuildAttributesOut ( attrCtrl ) ; }  
//: Builds the list of available attributes for output 

inline bool GetAttrOut (const StringC & attrName, StringC & attrValue) const
{ return Body().GetAttrOut ( attrName, attrValue) ; } 
    //: Gets the value for a given output attribute 

inline bool SetAttrOut (const StringC & attrName, const StringC & attrValue) 
{ return Body().SetAttrOut (attrName, attrValue) ; } 
//: Sets the value for a given output attribute 
    

UIntT AlignDMA (void) const { return Body().AlignDMA() ; } 
//: Access teh DMA alignment 

UIntT BufferSize(void) const { return Body().BufferSize() ; } 
//: Access the buffer size 

//template<PixelTypeT> 
inline DMABufferC<ByteYUV422ValueC> GetDMABuffer(void) const 
{ return Body().GetDMABuffer() ; } 
//!depricated: 
//: A usefull method which returns a buffer suitable for DMA transfers 
// The returned buffer has the correct size and dma alignment for the underlying hardware


inline DMABufferC<char> GetDMABufferGeneric(void) const 
{ return Body().GetDMABufferGeneric() ; } 
//: A usefull method which returns a buffer suitable for DMA transfers 
// The returned buffer has the correct size and dma alignment for the underlying hardware


inline const ImageRectangleC & Rectangle(void) const 
{ return Body().Rectangle() ; } 
//: Access Capture Rectangle


inline sv_handle * Device(void) 
{ return Body().Device() ; } 
//: Access the low level device handle 
// Use with care !

protected : 

}; 

}

#endif
