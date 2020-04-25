// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exFrame.cc"
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Frame.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  OptionC opts(nargs,args);
  opts.Check();
  
  WindowC win(100,100,"Frame Example");
  
  win.Add(FrameC(Label(" Widget "),"Frame title"));
  win.Show();
  
  Manager.Start();
  return 0;
}
