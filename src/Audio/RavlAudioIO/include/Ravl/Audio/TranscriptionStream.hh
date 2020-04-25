// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_TRANSCRIPTIONSTREAM_HEADER
#define RAVLAUDIO_TRANSCRIPTIONSTREAM_HEADER 1
//! docentry="Ravl.API.Audio.Feature Extraction"
//! userlevel=Normal
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlAudioIO
//! examples=exTranscriptionStream.cc
//! file="Ravl/Audio/IO/TranscriptionStream.hh"

#include "Ravl/DP/Port.hh"
#include "Ravl/Audio/TranscriptionFiles.hh"
#include "Ravl/Audio/Transcription.hh"

namespace RavlAudioN {
  
  //! userlevel=Advanced
  //: Transcription stream.
  
  class TranscriptionStreamC
  {
  public:
    TranscriptionStreamC(const StringC &ndataExt)
      : dataExt(ndataExt)
    {}
    //: Constructor.
    
    TranscriptionC Apply(const StringC &transFile);
    //: Load a transcription
    
  protected:
    StringC dataExt;
  };
  
  //! userlevel=Normal
  
  DPIPortC<TranscriptionC> OpenTranscriptionList(const StringC &transFileList,const StringC &dataExt);
  //: Open a file containing a list of transcriptions
  
  DPIPortC<TranscriptionC> OpenTranscriptionSearch(const StringC &dir,const StringC &transExt,const StringC &dataExt);
  //: Generate a list of transcriptions to process by searching a directory.
  
}


#endif
