// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_WAVFILE_HH
#define RAVLIMAGE_WAVFILE_HH
////////////////////////////////////////////////////////////////////////////
//! author="Kieron J Messer"
//! date="24/9/2000"
//! lib=RavlFireWire
//! docentry="Ravl.API.Audio.IO"
//! rcsid="$Id$"
//! file="Ravl/Contrib/FireWire/WavFile.hh"

#include"Ravl/Stream.hh"
#include"Ravl/Array1d.hh"
#include"Ravl/OS/Filename.hh"

#define WAVE_FORMAT_PCM  0x0001

namespace RavlImageN {

  typedef unsigned short UShortT;
  typedef unsigned long ULongT;
  
  using namespace RavlN;
  
  //! userlevel=Normal
  //: Class for writing wav files.
  
  class WavFileC 
  {
  public:
    WavFileC() {}
    //: Default constructor
    
    WavFileC(FilenameC & fname, UIntT bitsPerSample, UIntT sampleRate, UIntT numChannels);
    
    void write(const Array1dC<char> & data);
    //: write a frame of audio data to file  
    
    void Close();
    //: close the wave file, must be called
    
  protected:
    FilenameC fname;
    OStreamC ofs;
    UIntT BitsPerSample;
    UIntT SampleRate;
    UIntT NumChannels;
    UIntT BytesWritten;
  };

} // end namespace 



#endif
