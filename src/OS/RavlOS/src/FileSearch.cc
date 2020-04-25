// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/FileSearch.cc"

#include "Ravl/OS/Filename.hh"
#include "Ravl/StringList.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/Resource.hh"

namespace RavlN {
  
  //: Search through a list of paths for this file.
  // Returns this filename if is not found on path.
  
  FilenameC FilenameC::Search(const DListC<StringC> &dirs) const  {
    for(DLIterC<StringC> it(dirs);it.IsElm();it.Next()) {
      FilenameC tmp(it.Data() + RavlN::filenameSeperator + (*this));
      if(tmp.Exists())
        return tmp;
    }
    return (*this).Copy();  
  }
  
  //: Search through ':' separated list pathList of directories
  //: for filename.
  // Returns full path to file, or empty string if none.
  
  FilenameC FilenameC::Search(StringC pathList) const {
    return Search(StringListC(pathList,":"));
  }

  //: Searches for filename using following algorithm returns empty name if fail to find file
  // 1. if it starts from '~' look for file in home directory
  // 2. if absolute path (start from '/') check file presence
  // 3. check currentDirectory
  // 4. check resources directory
  FilenameC FilenameC::Search(const StringC& filename,
                              const StringC& currentDirectory,
                              const char *resourceModule) {
    FilenameC result;
    // Is there a file set at all ?
    if(filename.IsEmpty())
      return result; // No filename, no result!
    
    // Does the file exist from the current working directory ?
    result = filename;
    if(result.Exists())
      return result;
    
    // Check if the file just works.
    if(filename.firstchar() == '~') { //home directory
      StringC homeDir(getenv("HOME"));
      if(!homeDir.IsEmpty()) {
        result = homeDir + StringC(filename).after('~');
      }
    } else if(filename.firstchar() == RavlN::filenameSeperator) { //absolute path
      result = filename;
    } else {
      if(!currentDirectory.IsEmpty()) { //current directory
        result = currentDirectory + RavlN::filenameSeperator + filename;
        if(result.Exists())
          return result;
      }
      //finally go into resources directory
      result = Resource(resourceModule, filename);
    }

    if(!result.Exists())
      result = FilenameC();

    return result;
  }
}
