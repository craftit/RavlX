// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_WAITFOREXIT_HEADER
#define RAVLGUI_WAITFOREXIT_HEADER 1
//! docentry="Ravl.API.Graphics.GTK.Util"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/WaitForExit.hh"

#include "Ravl/Types.hh"

namespace RavlGUIN {
  void DisableWaitForGUIExit();
  //! userlevel=Normal
  //: Disable waiting for GUI to finish on program exit.
  // Call this method to allow program to exit immediatly even if
  // there are DPWindowC's open.
  
  void WaitForGUIExit();
  //! userlevel=Develop
  //: Force program wait GUI to have quit before allowing program to exit.
  // This function will also start the GUI manager if it hasn't be started
  // already.
  
}

#endif
