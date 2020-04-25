// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TRANSCRIPTIONFILESIO_HEADER
#define RAVL_TRANSCRIPTIONFILESIO_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing" 
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/FileListIO.hh"

#include "Ravl/DP/ListIO.hh"
#include "Ravl/String.hh"

namespace RavlAudioN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: Access a list of file names as a stream
  
  class DPIFileListBodyC
    : public DPISListBodyC<StringC >
  {
  public:
    DPIFileListBodyC();
    //: Constructor.
    // Creates an empty list.
    
    DPIFileListBodyC(const StringC &dir,const StringC &transExt);
    //: Construct from a search
    
    DPIFileListBodyC(const StringC &fileList);
    //: Construct from file list
    
    bool Search(const StringC &dir,const StringC &transExt);
    //: Search a directory tree for transcriptions
    
    bool Load(const StringC &fn);
    //: Load a file list
    // This assumes there is one filename per line.
    // Lines beginning with # are ignored.
  };

  //! userlevel=Normal
  //: Access a list of file names as a stream
  
  class DPIFileListC
    : public DPISListC<StringC >
  {
  public:
    DPIFileListC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPIFileListC(const StringC &dir,const StringC &transExt)
      : DPEntityC(*new DPIFileListBodyC(dir,transExt))
    {}
    //: Constructor.
      
    DPIFileListC(bool)
      : DPEntityC(*new DPIFileListBodyC())
    {}
    //: Create an empty list.
    
  protected:
    DPIFileListBodyC &Body()
    { return dynamic_cast<DPIFileListBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIFileListBodyC &Body() const
    { return dynamic_cast<const DPIFileListBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    bool Load(const StringC &fn)
    { return Body().Load(fn); }
    //: Load a file list.
    // This assumes there is one filename per line.
    // Lines beginning with # are ignored. <p>
    // Returns true on success
    
    bool Search(const StringC &dir,const StringC &transExt);
    //: Search a directory tree for transcriptions
    // Directory tree 'dir' is searched for files with extention 'transExt' 
    
    
  };

  
}


#endif
