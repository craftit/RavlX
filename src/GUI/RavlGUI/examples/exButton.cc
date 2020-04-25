// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exButton.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Pixmap.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

bool buttonA(IntT &v)
{
  std::cerr << "ButtonA:" << v << "\n";
  return true;
}

bool buttonB(IntT &v)
{
  std::cerr << "ButtonB:" << v << "\n";
  return true;
}

bool buttonC(IntT &v)
{
  std::cerr << "ButtonC:" << v << "\n";
  return true;
}

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  OptionC opts(nargs,args);
  opts.Check();
  
  WindowC win(100,100,"Hello");
  
  // An empty button
  ButtonC buttonPix = Button("Test", buttonC, 3);
  
  // Build and show the GUI
  win.Add(
           VBox
           (
             Button("Button 1", buttonA, 1) +
             Button("Button 2", buttonB, 2) +
             buttonPix
           )
         );
  win.GUIShow();
 
  // Set a pixmap
  PixmapC pix("info.xpm");
  buttonPix.GUISetImage(pix);
  
  // Start the GUI
  Manager.Start();
  std::cerr << "Finished... \n";
  return 0;
}
