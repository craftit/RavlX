// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLAUDIO_TRANSCRIPTION_HEADER
#define RAVLAUDIO_TRANSCRIPTION_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.Feature Extraction"
//! rcsid="$Id$"
//! lib=RavlAudioUtil
//! file="Ravl/Audio/Util/Transcription.hh"

#include "Ravl/Audio/Segment.hh"
#include "Ravl/DList.hh"

namespace RavlAudioN {

  //! userlevel=Normal
  //: Transcription of file
  
  class TranscriptionC 
    : public DListC<SegmentC> 
  {
  public:
    TranscriptionC()
    {}
    //: Default constructor.
    // Creates an empty transcription
    
    TranscriptionC(const StringC &transFile,const StringC &dataFile = StringC());
    //: Load from a file.
    
    TranscriptionC(const DListC<SegmentC> &lst)
      : DListC<SegmentC> (lst)
    {}
    //: Constructor.
    
    bool Load(const StringC &fn);
    //: Load transcription from file.
    
    bool Save(const StringC &fn);
    //: Save transcription to a file.
    
    StringC &DataFile()
    { return dataFile; }
    //: Access data file.
    
    const StringC &DataFile() const
    { return dataFile; }
    //: Access data file.
    
  protected:
    StringC dataFile;
  };
}

#endif
