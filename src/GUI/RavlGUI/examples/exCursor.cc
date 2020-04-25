// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exCursor.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/RawCanvas.hh"
#include "Ravl/GUI/Cursor.hh"

using namespace RavlGUIN;

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  
  WindowC win(100,100,"Hello");
  CursorC curs("GDK_HAND1");
  RawCanvasC canvas(100,100);
  win.Add(canvas);
  win.SetCursor(curs);
  win.Show();
  Manager.Start();
  
  return 0;
}
