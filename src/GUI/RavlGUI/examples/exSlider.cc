// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid=$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exSlider.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Slider.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/SpinButton.hh"

using namespace RavlGUIN;

bool sliderFunc(RealT &v,IntT &)
{
  std::cerr << "Slider:" << v << "\n";
  return true;
}

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  
  WindowC win(100,100,"Hello");
  
  win.Add(VBox(
	       SliderH(1.0,0.0,10.0,0.1,sliderFunc) +
	       SliderV(1.0,0.0,10.0,0.1,sliderFunc) +
	       SpinButtonC(4,1,1,0,2,1)
	       ));
  win.Show();
  
  Manager.Start();
  return 0;
}
