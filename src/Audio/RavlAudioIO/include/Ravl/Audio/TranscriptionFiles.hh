// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_TRANSCRIPTIONFILESIO_HEADER
#define RAVLAUDIO_TRANSCRIPTIONFILESIO_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.Feature Extraction"
//! rcsid="$Id$"
//! lib=RavlAudioIO
//! file="Ravl/Audio/IO/TranscriptionFiles.hh"

#include "Ravl/DP/FileListIO.hh"
#include "Ravl/String.hh"

namespace RavlAudioN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: List of trancription files.
  
  class DPITranscriptionFilesBodyC
    : public DPIFileListBodyC
  {
  public:
    DPITranscriptionFilesBodyC();
    //: Constructor.
    // Creates an empty list.
    
    DPITranscriptionFilesBodyC(const StringC &dir,const StringC &transExt);
    //: Construct from a search
    
    DPITranscriptionFilesBodyC(const StringC &fileList);
    //: Construct from file list
  };

  //! userlevel=Normal
  //: List of trancription files
  
  class DPITranscriptionFilesC
    : public DPIFileListC
  {
  public:
    DPITranscriptionFilesC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.

    DPITranscriptionFilesC(const StringC &fileList)
      : DPEntityC(*new DPITranscriptionFilesBodyC(fileList))
	{}
    //: Construct from file list

    DPITranscriptionFilesC(const StringC &dir,const StringC &transExt)
      : DPEntityC(*new DPITranscriptionFilesBodyC(dir,transExt))
    {}
    //: Constructor.
    
    DPITranscriptionFilesC(bool)
      : DPEntityC(*new DPITranscriptionFilesBodyC())
    {}
    //: Create an empty list.
    
  protected:
    DPITranscriptionFilesBodyC &Body()
    { return dynamic_cast<DPITranscriptionFilesBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPITranscriptionFilesBodyC &Body() const
    { return dynamic_cast<const DPITranscriptionFilesBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
  };

  
}


#endif
