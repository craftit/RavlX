// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_PALFRAME_HH
#define RAVLIMAGE_PALFRAME_HH
////////////////////////////////////////////////////////////////////////////
//! author="Kieron J Messer"
//! date="24/9/2000"
//! lib=RavlFireWire
//! docentry="Ravl.API.Images.Video.Video IO.DV;Ravl.API.Audio.IO.Drivers.Linux1394"
//! rcsid="$Id$"
//! file="Ravl/Contrib/FireWire/PalFrame.hh"
  
#include"Ravl/Array1d.hh"
#include"Ravl/Image/Image.hh"
#include"Ravl/Image/ByteRGBValue.hh"
#include"Ravl/TimeCode.hh"
#include"libdv/dv.h"

namespace RavlImageN {
  
  using namespace RavlN;
  
  typedef struct Pack
  {
    // the five bytes of a packet
    unsigned char data[5];
  } Pack;
  
  
  typedef struct AudioInfo
  {
    int frames;
    int frequency;
    int samples;
  } AudioInfo;
  

  //! userlevel=Normal
  //: DV "PAL" frame.
  // Also includes 1 frame's worth of audio
  
  class PalFrameC 
  {
  public:
    PalFrameC();
    //: Default constructor
      
    ~PalFrameC();
    //: Destructor.
    
    PalFrameC(istream &in);
    //: Stream constructor
      
    const ByteT * getData() const { return data; }
    //: access to data
      
    dv_decoder_t * Decoder() const { return decoder;}
    // access to the decoder
      
    void setData(ByteT *dta);
    //: set the data
      
    int getBytes() const {return bw;}
    //: the number of valid bytes written
      
    void setBytes(const int bts=0){ bw=bts;}
    //: reset number of bytes written
      
    bool isValid();
    //: checks whether frame contains all valid bytes
      
    void writeDifBlock(int section_type, int dif_sequence, int dif_block, int length, ByteT * data);
    //: write a dif block to buffer
      
    ImageC<ByteRGBValueC>Image();
    //: decodes frame and returns image
      
    bool GetAudioInfo(AudioInfo &info) const;
    // gets audio properties of the frame
      
    Array1dC<char> Sound();
    // gets the sound for a frame
      
    TimeCodeC extractTimeCode() const;
    //: extract a timecode from a frame of data
      
    bool IsPAL() const;
    //: is it a pal
      
    friend ostream &operator<<(ostream &s, const PalFrameC &out);
    //: Output stream operator
      
    friend istream &operator>>(istream &s, PalFrameC &in);
    //: Input stream operator
      
  protected:
    //: Put all your class members here
      
    ByteT * data; 
    //: the DV encoded PAL frame
      
    int bw;
    //: the number of bytes written
      
    dv_decoder_t *decoder;
    //: the libdv decoder
      
    Array1dC<ByteRGBValueC>decoded;
    // a buffer to store the decoded video stream
      
    Array1dC<char>sound;
    // a buffer to store the sound in
      
    int frameno;
    bool init;
    bool maps_initialized;
      
    // lookup tables for collecting the shuffled audio data
    Array1dC<IntT> palmap_ch1;
    Array1dC<IntT> palmap_ch2;
    Array1dC<IntT> palmap_2ch1;
    Array1dC<IntT> palmap_2ch2;
    Array1dC<IntT> ntscmap_ch1;
    Array1dC<IntT> ntscmap_ch2;
    Array1dC<IntT> ntscmap_2ch1;
    Array1dC<IntT> ntscmap_2ch2;
    short compmap[4096];
      
    bool GetSSYBPack(int packNum, Pack &pack) const;
    bool GetVAUXPack(int packNum, Pack &pack) const;
    bool GetAAUXPack(int packNum, Pack &pack) const;
      
      
  };
  
   
  ostream &operator<<(ostream &s, const PalFrameC &out);
  //: output stream operator
  
  inline
  istream &operator>>(istream &s, PalFrameC &in) {
    in = PalFrameC(s);
    return s;
  }
  //: input stream operator
  
} // end namespace RavlImageN
#endif
