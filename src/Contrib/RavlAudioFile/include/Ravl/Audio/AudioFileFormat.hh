// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DEVAUDIOFORMAT_HEADER
#define RAVL_DEVAUDIOFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.IO.Audio File"
//! example=exAudioIO.cc
//! lib=RavlAudioFile
//! rcsid="$Id$"
//! file="Ravl/Contrib/Audio/AudioFile/AudioFileFormat.hh"

#include "Ravl/DP/FileFormat.hh"

namespace RavlAudioN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: AudioFile File format information.
  
  class FileFormatAudioFileBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatAudioFileBodyC(const StringC &vName);
    //: Constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain AudioFile stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain AudioFile stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  protected:
    bool CheckFilenameExtension(const StringC& filename) const;
    //: Check the filename has a supported extension.
    //!return: True if the extension is supported.

    StringC vName;
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a AudioFile File Format.
  
  class FileFormatAudioFileC 
    : public FileFormatC<Int16T > 
  {
  public:
    FileFormatAudioFileC(const StringC &vName)
      : FileFormatC<Int16T>(*new FileFormatAudioFileBodyC(vName))
    {}
  };
  
}
#endif
