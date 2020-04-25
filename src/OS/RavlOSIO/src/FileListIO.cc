// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/FileListIO.cc"

#include "Ravl/DP/FileListIO.hh"
#include "Ravl/OS/Directory.hh"

namespace RavlAudioN {
  
  //: Constructor.
  // Creates an empty list.
  
  DPIFileListBodyC::DPIFileListBodyC()
  {}
  
  //: Constructor.
  
  DPIFileListBodyC::DPIFileListBodyC(const StringC &fn)
  { Load(fn); }

  //: Constructor.
  
  DPIFileListBodyC::DPIFileListBodyC(const StringC &dirName,const StringC &transExt)
  { Search(dirName,transExt); }
  
  //: Search a directory tree for transcriptions
    
  bool DPIFileListBodyC::Search(const StringC &dirName,const StringC &transExt) {
    DirectoryC dir(dirName);
    if(!dir.IsDirectory())
      return false;
    DListC<StringC> files = dir.SearchTree(StringC("*.") + transExt);
    SetList(files);
    return true;
  }
  
  //: Load a file list
  
  bool DPIFileListBodyC::Load(const StringC &fn) {
    IStreamC inf(fn);
    if(!inf)
      return false;
    inf.SkipWhiteSpace();
    DListC<StringC> lst;
    while(inf) {
      StringC fileName = inf.ClipTo('\n').TopAndTail();
      if(fileName.firstchar() == '#')
	continue; // Skip comments.
      lst.InsLast(fileName);
    }
    SetList(lst);
    return true;
  }

}
