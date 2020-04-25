// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAudioIO
//! file="Ravl/Audio/IO/TranscriptionFiles.cc"

#include "Ravl/Audio/TranscriptionFiles.hh"
#include "Ravl/OS/Directory.hh"

namespace RavlAudioN {
  
  //: Constructor.
  // Creates an empty list.
  
  DPITranscriptionFilesBodyC::DPITranscriptionFilesBodyC()
  {}
  
  //: Constructor.
  
  DPITranscriptionFilesBodyC::DPITranscriptionFilesBodyC(const StringC &fn)
    : DPIFileListBodyC(fn)
  {}

  //: Constructor.
  
  DPITranscriptionFilesBodyC::DPITranscriptionFilesBodyC(const StringC &dirName,const StringC &transExt)
    : DPIFileListBodyC(dirName,transExt)
  {}
  
}
