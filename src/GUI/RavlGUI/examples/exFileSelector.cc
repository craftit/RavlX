// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! file="Ravl/GUI/GTK/exFileSelector.cc"

#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/FileSelector.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/TextBox.hh"

using namespace RavlN;
using namespace RavlGUIN;

//: Callback, Open a file.

bool LoadImage(StringC &filename,FileSelectorC &fs,TextBoxC &textBox) {
  textBox.Insert(filename + "\n");  // Display filename in textbox.
  return true;
}

//: Callback, Quit the application.

bool gui_quit()  {
  Manager.Quit(); // Initate shutdown.
  return true;
}


int main(int nargs,char **argv) {
  Manager.Init(nargs,argv);
  
  // Create a textbox to show files user has requested.
  
  TextBoxC textBox("",true);
  
  // Create a fileselector 
  
  FileSelectorC fs = FileSelector("Load Image","img.ppm",&LoadImage,textBox);
  
  // Create a menubar
  
  MenuBarC menuBar(MenuC("File",
			 MenuItem("Quit",gui_quit)  +
			 MenuItemShow("Open",fs)     // Show fileselector.
			 ));
  
  // Create a window with a menu bar and textbox
  
  WindowC win(100,100,"FileSelectorC example");
  win.Add(VBox(menuBar + 
	       textBox));
  win.Show();
  
  // Start the GUI.
  
  Manager.Start();

  return 0;
}
