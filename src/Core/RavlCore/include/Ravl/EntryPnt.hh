// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_ENTRYPNT_HEADER
#define RAVL_ENTRYPNT_HEADER 1
//////////////////////////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Misc"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/EntryPnt.hh"
//! date=""

// This head add makes sure all Ravl exceptions are caught and
// given to the user.  This avoids anonymous exits from the program.

namespace RavlN {  
  bool UsingRavlMain();
  //: Are we using RAVL_ENTRY_POINT ?
  // Used to decide how to handle some internal errors.
  
  typedef int (*FuncMainCallT)(int argc, char* argv[]);
  typedef int (*FuncMainCallManagerT)(int argc, char* argv[],FuncMainCallT userMain);

  FuncMainCallManagerT RetrieveMainCallManager();
  //: Access current MainCallManager.

  bool RegisterMainCallManager(FuncMainCallManagerT entryPoint);
  //: Register an alternate call to start the main program.
  // This is used on MacOSX where for many libraries expect the main
  // thread to be running in an event loop. It should be registered
  // in the constructor of a static variable to ensure its setup
  // before main() is entered.

  int RavlMain(int argc, char* argv[],int (*func)(int argc, char* argv[]));
  //: Call through function.
}

#define RAVL_ENTRY_POINT(Function)      \
  int main(int argc, char* argv[])     \
  { return RavlN::RavlMain(argc,argv,Function); }



#endif
