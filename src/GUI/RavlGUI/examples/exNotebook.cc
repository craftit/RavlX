// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exNotebook.cc"
//! author="Charles Galambos"
//! date="23/03/1999"
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! userlevel=Normal

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Notebook.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"

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

bool pageChanged(IntT &v)
{
  std::cerr << "Page:" << v << "\n";
  return true;
}

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  
  WindowC win(100,100,"Hello");
  
  NotebookC nb(GTK_POS_TOP);
  Connect(nb.SigChanged(), &pageChanged);
  nb.Add(VBox(Button("ButtonA 1",&buttonA,1) + Button("ButtonA 3",&buttonA,3)) + 
	 VBox(Button("ButtonB 2",&buttonB,2) + Button("ButtonB 4",&buttonB,4))
	 );
  win.Add(nb);
  win.Show();

  Manager.Start();
  return 0;
}
