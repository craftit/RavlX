// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DVDECODE_HEADER
#define RAVLIMAGE_DVDECODE_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/Contrib/DV/DvDecode.hh"
//! lib=RavlDV
//! author="Kieron Messer"
//! docentry="Ravl.API.Images.Video.Video IO.DV"
//! date="15/05/2002"

#include "Ravl/DP/Process.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Array1d.hh"
#include "Ravl/AVFrame.hh"
#include "libdv/dv.h"


namespace RavlImageN {
  
  using namespace RavlN;
  
  // --------------------------------------------------------------------------
  // **********  DvDecodeBodyC  ********************************************
  // --------------------------------------------------------------------------
  //! userlevel=Develop
  //: 
  // You should not use this class directly, but rather the handle class
  // DvDecode.
  //
  
  
  class DvDecodeBodyC
    : public DPProcessBodyC<ByteT *, ImageC<ByteRGBValueC> >
  {
  
  public:
    DvDecodeBodyC (bool deinterlace);
    //: Constructor
    // (See handle class DvDecode)
    
    ~DvDecodeBodyC();
    //: Destructor.
    
    void Initialize (ByteT * arr) ; 
    //: Initialze the attributes (samplerate, samplebits) from the header of  a given frame. 
    // In this implementation, this is only done once, and not once per frame

    AVFrameC Apply (ByteT * arr) ; 
    //: Converts PAL DV frame to AVFrameC
    // Calls Initialize if not already done 

    virtual bool IsStateless() const 
    { return false; }
    //: Is operation stateless ?
    
    bool Deinterlace() const
    { return deinterlace; }
    //: Are we deinterlaceing ?
    
    bool Deinterlace(bool val)
    { return deinterlace = val; }
    //: Set deinterlace flag.
    
    const RealT & SampleRate (void) const 
      { return samplerate ; } 
    //: Query the sample rate 
    
    const  IntT & SampleBits (void) const 
      { return samplebits ; } 
    //: Query the sampe bits 

  private:
    dv_decoder_t *decoder;
    //: The decoder 

    uint8_t *decoded;
    //: Decoded image data 

    bool init;
    //: Initialization flag 

    bool deinterlace;
    //: DeInterlace flag

    static const IntT maxAudioChannels = 4 ; 
    //: Maximum number of Audio channels 

    static const IntT maxAudioSamples = 1944 ; 
    //: Maximum number of Audio samples per frame (per channel) 

    int16_t * audio_buffers[maxAudioChannels] ; 
    //: Tempory audio buffers 

    IntT samplebits ; 
    //: The number of sample bits 

    RealT samplerate ; 
    //: The sample rate

    IntT samplesThisFrame ; 
    //: The number of samples in this frame


  };
  
  ///////////////////////////////////////////////////
  
  
  // --------------------------------------------------------------------------
  // **********  DvDecode  ********************************************
  // --------------------------------------------------------------------------
  
  //! userlevel=Normal
  //: Function to decode raw DV into a RAVL image
  
  class DvDecodeC
    : public DPProcessC<ByteT *, ImageC<ByteRGBValueC> >
  {
  public:  
    DvDecodeC (bool deinterlace=false)
      : DPProcessC<ByteT *, ImageC<ByteRGBValueC> >(*new DvDecodeBodyC(deinterlace))
    {}
    //: Constructs DvDecodeC 

    inline void Initialize ( ByteT * data ) 
      { return Body().Initialize (data) ; } 
    //: Initialze the attributes (samplerate, samplebits) from the header of  a given frame


  protected:
    DvDecodeBodyC &Body() 
    { return dynamic_cast<DvDecodeBodyC &>(DPEntityC::Body()); }
    //: Body access.
    
    const DvDecodeBodyC &Body() const
    { return dynamic_cast<const DvDecodeBodyC &>(DPEntityC::Body()); }
    //: Body access.

  public:
    AVFrameC Apply (ByteT * arr)
    { return Body().Apply(arr); }
    //: Converts PAL DV frame to AVFrameC 
    
    bool Deinterlace() const
    { return Body().Deinterlace(); }
    //: Are we deinterlaceing ?
    
    bool Deinterlace(bool val)
    { return Body().Deinterlace(val); }
    //: Set deinterlace flag.
  
    const RealT & SampleRate (void) const 
      { return Body().SampleRate()  ; } 
    //: Query the sample rate 
    
    const  IntT & SampleBits (void) const 
      { return Body().SampleBits()  ; } 
    //: Query the sample bits 

  };
  
} // end namespace RavlImageN

#endif
