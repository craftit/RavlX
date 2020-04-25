// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/SourceCodeManager.cc"

#include "Ravl/SourceTools/SourceCodeManager.hh"
#include "Ravl/StringList.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/BlkQueue.hh"
#include "Ravl/Stream.hh"

namespace RavlN {

  /////////////////////////////////
  // Default constructor.
  
  SourceCodeManagerC::SourceCodeManagerC() 
    : abortOnError(true),
      verbose(false),
      skipSub(false)
  {}
  
  SourceCodeManagerC::SourceCodeManagerC(const StringC &adir,bool doEnabled)
    : rootDir(adir.Copy()),
      abortOnError(true),
      verbose(false),
      skipSub(false)
  {}
  
  //: Access a defs file.
  
  DefsMkFileC &SourceCodeManagerC::DefsMkFile(const StringC &name) {
    DefsMkFileC *lu = defsFiles.Lookup(name);
    if(lu != 0)
      return *lu;
    DefsMkFileC &df = defsFiles[name];
    df = DefsMkFileC(rootDir + filenameSeperator + name);
    return df;
  }
  
  //: For all directories in project do 'op'.
  
  bool SourceCodeManagerC::ForAllDirs(CallFunc2C<StringC&,DefsMkFileC&,bool> op,bool inActiveAsWell) {
    BlkQueueC<StringC> toDo;
    RavlAssert(op.IsValid());
    toDo.InsLast(rootDir);
    while(!toDo.IsEmpty()) {
      StringC at = toDo.Pop();
      DefsMkFileC defs = DefsMkFileC(at + filenameSeperator + "defs.mk");
      if(verbose)
	cout << "Processing directory '" << at << "'\n";
      if(!op(at,defs)) {
	if(verbose) 
	  std::cerr << "SourceCodeManagerC::ForAll(), Error processing directory :'" << at << "'\n";
	break;
      }
      // Sort out subdirectories.
      StringListC subDirs = defs.Nested();
      
      //check subdirs for duplicates
      DListC<StringC> checkList;      
      
      DLIterC<StringC> it(subDirs);
      for(it.Last();it;it--)
      {
	toDo.InsFirst(at + filenameSeperator + *it);
	if(checkList.Contains(*it))
	{
	  std::cerr << "ERROR: duplicate nested dirs in " << at + filenameSeperator + "defs.mk" << std::endl;
	}
	else
	  checkList.InsLast(*it);
      }
    }
    return true;
  }  
  
}
