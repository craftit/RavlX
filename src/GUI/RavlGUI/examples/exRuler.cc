// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exRuler.cc"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! userlevel=Normal

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Ruler.hh"
#include "Ravl/GUI/Table.hh"
#include "Ravl/GUI/RawCanvas.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Cursor.hh"

using namespace RavlGUIN;

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  
  WindowC win(100,100,"Hello");
  TableC tab(2,2);
  win.Add(tab);
  RulerC hruler(false,20,100,20,120);
  RulerC vruler(true,20,100,20,120);
  RawCanvasC canvas(100,100);
  tab.AddObject(vruler,0,1,1,2,GTK_FILL,(GtkAttachOptions) (GTK_EXPAND|GTK_SHRINK|GTK_FILL));
  tab.AddObject(hruler,1,2,0,1,(GtkAttachOptions) (GTK_EXPAND|GTK_SHRINK|GTK_FILL),GTK_FILL);
  tab.AddObject(canvas,1,2,1,2);
  hruler.AttachTo(canvas);
  vruler.AttachTo(canvas);
  win.Show();
  Manager.Start();
  
  return 0;
}
