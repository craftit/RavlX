// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/DocExe.cc"

#include "Ravl/CxxDoc/Document.hh"
#include "Ravl/OS/ChildOSProcess.hh"
#include "Ravl/OS/Filename.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlCxxDocN {
  
  //: Execute a child process.
  
  bool DocumentBodyC::Exec(StringC &text) {
    int at = text.index(':');
    StringC prog;
    bool doIn = false;
    if(at >= 0) {
      doIn = true;
      prog = text.before(at);
    }
    FilenameC tmpfile("/tmp/docexetemp");
    bool exitOk = true;
    tmpfile = tmpfile.MkTemp();
    ChildOSProcessC proc(prog,tmpfile,true,doIn);
    if(!proc.IsRunning()) {
      Output() << "\n !!! Failed to run program '" << prog << "'. !!! \n";
      lastIf = false; // So templates can detect failures...
      return false;
    }
    if(doIn)
      proc.StdIn() << text.after(at);    
    if(!proc.Wait(30)) { // Give it 30 seconds to give us the results.
      proc.Terminate(5); // Kill it and continue on.
      exitOk = false;
    }
    IStreamC help(tmpfile);
    help.CopyTo(Output());
    tmpfile.Remove();
    if(!exitOk) 
      Output() << "\n !!! Timeout error when generating help. !!! \n";
    lastIf = true;
    return true;
  }

  //: Go through files in a directory.
  
  bool DocumentBodyC::ForDir(StringC &text) {
    RavlAssert(0);
    return true;
  }

}
