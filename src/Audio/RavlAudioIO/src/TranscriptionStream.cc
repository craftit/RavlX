// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! userlevel=Normal
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlAudioIO
//! file="Ravl/Audio/IO/TranscriptionStream.cc"

#include "Ravl/Audio/TranscriptionStream.hh"
#include "Ravl/Audio/TranscriptionFiles.hh"
#include "Ravl/DP/Method2Proc.hh"
#include "Ravl/DP/Compose.hh"

namespace RavlAudioN {
  
  //: Load a transcription
  
  TranscriptionC TranscriptionStreamC::Apply(const StringC &transFile) 
  { return TranscriptionC(transFile,StringC(transFile).before('.',-1) + '.' + dataExt); }
  
  //:------------------------------------------------------------------
  
  //: Open a file containing a list of transcriptions
  
  DPIPortC<TranscriptionC> OpenTranscriptionList(const StringC &transFileList,const StringC &dataExt) {
    DPITranscriptionFilesC ins(transFileList);
    return ins >> RavlN::Process(TranscriptionStreamC(dataExt),&TranscriptionStreamC::Apply);
  }
  
  //: Generate a list of transcriptions to process by searching a directory.
  
  DPIPortC<TranscriptionC> OpenTranscriptionSearch(const StringC &dir,const StringC &transExt,const StringC &dataExt) {
    DPITranscriptionFilesC ins(dir,transExt);
    return ins >> RavlN::Process(TranscriptionStreamC(dataExt),&TranscriptionStreamC::Apply);
  }


}
