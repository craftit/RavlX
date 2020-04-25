// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioUtil
//! file="Ravl/Audio/Util/AudioFrame.cc"

#include "Ravl/Audio/AudioFrame.hh"
#include "Ravl/BinStream.hh"

namespace RavlAudioN {

  //: Constructor from stereo
  //: --------------------------------------------------------------------------------------------------------------------------
  AudioFrameBodyC::AudioFrameBodyC(const SArray1dC<SampleElemC<2,Int16T> > &data,RealT nfreq) 
    : stereoData(data),
      channels(2),
      freq(nfreq)
  {}


  //: Constructor from stream
  //: --------------------------------------------------------------------------------------------------------------------------
  AudioFrameBodyC::AudioFrameBodyC ( BinIStreamC & stream )
   : channels(0),
     freq(0)
  {
    stream >> channels >> freq ;
    if ( channels  == 2 )  stream >> stereoData ;
  }


  //: Constructor from binary stream 
  //: --------------------------------------------------------------------------------------------------------------------------
  AudioFrameBodyC::AudioFrameBodyC ( istream & stream ) 
  : channels(0),
    freq(0)
  {
    stream >> channels >> freq ;
    if ( channels == 2 )  stream >> stereoData ;
  }


  //: Save to stream 
  //: --------------------------------------------------------------------------------------------------------------------------
  bool AudioFrameBodyC::Save(ostream & stream) const
  {
    stream << channels << " " << freq << " " ;
    if ( channels == 2 )
      stream << stereoData ;
    else
      return false;
    return true ;
  }


  //: Save to binary stream 
  //: --------------------------------------------------------------------------------------------------------------------------
  bool  AudioFrameBodyC::Save ( BinOStreamC & stream ) const 
  {
    stream << channels << freq ;
    if ( channels == 2 )
      stream << stereoData ;
    else
      return false;
    return true ;
  }

}
