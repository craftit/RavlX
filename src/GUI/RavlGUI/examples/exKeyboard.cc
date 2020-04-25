// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exKeyboard.cc"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Frame.hh"
#include "Ravl/Option.hh"
#include "Ravl/Threads/Signal1.hh"
#include <gdk/gdkevents.h>

using namespace RavlGUIN;

// This example shows how to intercept keypress events from a widget.
// Details about the contents of the structure 'GdkEventKey' can be found
// in the GTK documentation or at http://library.gnome.org/devel/gdk/stable/gdk-Event-Structures.html#GdkEventKey


// This routine displays the intercepted key presses
bool HandleKeyPress(GdkEventKey * keyEvent) {
  
  std::cerr << "Key pressed : " << keyEvent->string << "\n";
  
  return 0;
}



int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  OptionC opts(nargs,args);
  opts.Check();

  // Create a window.
  WindowC win(100,100,"Frame Example");
  
  // Make creat a few widgets so we have something
  // to capture key events from.
  FrameC frame(Label(" Widget "),"Frame title");
  
  // Put widget into the window.
  win.Add(frame);
  
  // Connect an event handler to the frame widget.
  Connect(frame.Signal("key_press_event"),&HandleKeyPress);
  
  // Show the window.
  win.Show();
  
  // Run the application
  Manager.Start();
  return 0;
}
