// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/WaitForExit.cc"

#include "Ravl/GUI/WaitForExit.hh"
#include "Ravl/GUI/Manager.hh"
#include <stdlib.h>

namespace RavlGUIN {
  static bool noWaitForGUIExit = false;
  static bool waitForGUIEnabled = false;
  
  void DisableWaitForGUIExit() {
    noWaitForGUIExit = true;
  }
  
}

extern "C" {
  void waitForGUIExit() {
    using namespace RavlGUIN;
    if(noWaitForGUIExit)
      return ;
    if(!Manager.IsManagerStarted())
      return ;
    std::cerr << "Program exited, waiting for GUI to be shutdown. \n";
    Manager.Wait();
  }
  
}

namespace RavlGUIN {
  
  //: Force program wait GUI to have quit before allowing program to exit.
  
  void WaitForGUIExit() {
    if(waitForGUIEnabled)
      return ;
    waitForGUIEnabled = true;
    
    // Ensure the manager is started.
    if(!Manager.IsManagerStarted()) {
      Manager.Execute();
      Manager.WaitForStartup();
    }
    
    atexit(waitForGUIExit);
  }

}
