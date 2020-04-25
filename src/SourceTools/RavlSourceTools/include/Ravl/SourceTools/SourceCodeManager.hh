// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_SRCCHECK_HEADER 
#define RAVL_SRCCHECK_HEADER 1
/////////////////////////////////////////////////////////////
//! file="Ravl/SourceTools/CodeManager/SourceCodeManager.hh"
//! author="Charles Galambos"
//! userlevel=Advanced
//! date="2/4/1998"
//! docentry="Ravl.API.Source Tools.Code Manager"
//! rcsid="$Id$"
//! lib=RavlSourceTools

#include "Ravl/RefCounter.hh"
#include "Ravl/DList.hh"
#include "Ravl/Hash.hh"
#include "Ravl/Calls.hh"
#include "Ravl/SourceTools/DefsFile.hh"

namespace RavlN {
  
  //! userlevel=Advanced
  //: Base class for source tree processing tools.
  // This class allows operations to be performed on the whole source tree,
  // or sub-tree's of it..
  
  class SourceCodeManagerC
  {
  public:
    SourceCodeManagerC();
    //: Default constructor.
    
    SourceCodeManagerC(const StringC &adir,bool enabled = true);
    //: Directory constructor
    
    DefsMkFileC &DefsMkFile(const StringC &name);
    //: Access a defs file.
    // 'name' should be the path to the directory starting from
    // the root directory of the project.  The root defs.mk file
    // is indexed by '.'
    
    bool ForAllDirs(CallFunc2C<StringC &,DefsMkFileC &,bool> op,bool inActiveAsWell = false);
    //: For all directories in project do 'op'.
    // 
    
    bool MissingSubDir(const CallFunc1C<StringC &,bool> &missingDir) { 
      onMissingDirectory = missingDir;
      return true;
    } 
    //: What to call if  sub directory is missing.    
    
    bool IsVerbose() const { return verbose; }
    //: In verbose mode ?
    
    void SetVerbose(bool v) { verbose = v; }
    //: Set verbose mode.
    
    const StringC &RootDir() const { return rootDir; }
    //: Get root director of source.
    
    void SetSkipSubdir(bool val) { skipSub = val; }
    //: Set Skip Subdirectory flag.
    
    
  private:
    StringC rootDir; // Root source directory 
    
    HashC<StringC,DefsMkFileC> defsFiles; // Cache of defs.mk file info.
    
    CallFunc1C<StringC&,bool> onMissingDirectory; // What to do when you find a missing directory.
    
    bool abortOnError; // Abort processing on error ?
    bool verbose;      // Be verbose about what we're doing ?
    bool skipSub;      // Flag DoOp can set to force all subdirectories to be skipped.    
  };

}
#endif
