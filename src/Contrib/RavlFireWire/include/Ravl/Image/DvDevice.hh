// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_DVDEVICE_HH
#define RAVLIMAGE_DVDEVICE_HH
////////////////////////////////////////////////////////////////////////////
//! author="Kieron J Messer"
//! date="23/9/2000"
//! lib=RavlFireWire
//! docentry="Ravl.API.Images.Video.Video IO.DV;Ravl.Applications.Video"
//! rcsid="$Id$"
//! file="Ravl/Contrib/FireWire/DvDevice.hh"

#include <libavc1394/rom1394.h>
#include <libavc1394/avc1394.h>
#include <libavc1394/avc1394_vcr.h>
#include <libraw1394/raw1394.h>
#include <libraw1394/csr.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include"Ravl/TimeCode.hh"
#include"Ravl/Image/PalFrame.hh"
#include"Ravl/Image/WavFile.hh"

#define __USE_BSD 1

namespace RavlImageN {
  
  using namespace RavlN;

  //! userlevel=Normal
  //: DV Device controller.
  
  class DvDeviceC 
  {
  public:
    DvDeviceC();
    //: Default constructor  
    
    DvDeviceC(istream &in);
    //: Stream constructor
    
    virtual ~DvDeviceC() { raw1394_destroy_handle(handle); } 
    //: Destroy the deviceq
    
    friend ostream &operator<<(ostream &s, const DvDeviceC &out);
    //: Output stream operator
    
    friend istream &operator>>(istream &s, DvDeviceC &in);
    //: Input stream operator
    
    bool IsPlaying() const;
    //: Is the VCR in play mode
    
    bool IsRecording() const;
    //: Is the VCR in record mode
    
    void Play() const;
    //: Start play
    
    void NextFrame() const;
    //: Goto to the next frame
    
    void TrickPlay(const int speed=1) const;
    //: Play at speed
    
    void Pause() const;
    //: Pause play
    
    void Stop() const;
    //: Stop player
    
    void Rewind() const;
    //: Rewind player
    
    void ForwardWind() const;
    //: Forward wind player
    
    TimeCodeC GetTimeCode() const;
    //: get the current timecode

    //void UpdateTC() const ; 
    //: update the current timecode ; 
    
    //void GotoTimeCode(const TimeCodeC &tc) const {}
    //: goto user specified timecode

    bool GotoTimeCodeFwd (const TimeCodeC & tc) const ;
    //: Go forward to timecode. returns true on susccess. 
    // This function only searches forward, and does no other seeking. 


    StringC Status() const;
    //: get the status of the VCR
    
    TimeCodeC GrabFrame();
    //: this fills the PalFrame buffer
    
    ImageC<ByteRGBValueC> GrabFrame(const TimeCodeC & tcGrab);
    //: get an image from the vcr
    
    ImageC<ByteRGBValueC> GrabImage();
    //: get an image from the vcr
    
    PalFrameC Frame() const { return frame; }
    //: access to currently held frame
    
    bool GrabSequence(const char * filename, const TimeCodeC & tcStart, const TimeCodeC & tcEnd);
    //: grabs a number of frames from DV player into file, in DV format
    
    bool GrabWav(const char * filename, const TimeCodeC & tcStart, const TimeCodeC & tcEnd);
    //: grabs a wave file directly from the vcr

    bool GrabImageSequence(const StringC & prefix, const TimeCodeC & tcStart, const TimeCodeC & tcEnd, UIntT nFrames);
    //: grab a set of images between the two timecodes and nFrame intervals and saves them according to prefix
    
  protected:

    //inline IntT FramesToTarget( const TimeCodeC & target) const { return (target.getFrameCount() - GetTimeCode().getFrameCount()) ; } 
    //:  the number of frames to the target timecode. 
    // Negative values means we need to seek backwards. 

  protected:
    //: Put all your class members here
    rom1394_directory rom_dir;
    raw1394handle_t handle;
    int device;
    PalFrameC frame; // a frame of DV PAL data
    // TimeCodeC currentTC ; // a timecode object, used by various card methods. 
  };
  
  ostream &operator<<(ostream &s, const DvDeviceC &out);
  //: output stream operator
  
  inline
  istream &operator>>(istream &s, DvDeviceC &in) {
    in = DvDeviceC(s);
    return s;
  }
  //: input stream operator

} // end namespace

#endif
