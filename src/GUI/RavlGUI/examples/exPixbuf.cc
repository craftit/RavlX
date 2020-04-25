// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exPixbuf.cc"
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! userlevel=Normal

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Pixbuf.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  OptionC opts(nargs,args);
  opts.Check();
  
  WindowC win(100,100,"Hello");

  PixbufC iconPix(PROJECT_OUT "/share/RAVL/pixmaps/CalibrationChart.png");
  win.GUISetDefaultIcon(iconPix);

  win.GUIShow();

  Manager.Start();
  return 0;
}
