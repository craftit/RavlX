// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exRadioButton.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/RadioButton.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

bool buttonA(bool &v)
{
  std::cerr << "ButtonA:" << v << "\n";
  return true;
}

bool buttonB(bool &v)
{
  std::cerr << "ButtonB:" << v << "\n";
  return true;
}

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  OptionC opts(nargs,args);
  opts.Check();
  
  WindowC win(100,100,"Hello");
  DListC<StringC> sel;
  sel.InsFirst("Hello1");
  sel.InsFirst("Hello2");
  sel.InsFirst("Hello3");
  //MenuBarC menuBar;
  //menuBar +
  RadioButtonGroupT buttonGrp;
  
  win.Add(
	  HBox(
	       RadioButton("A Button 1",buttonGrp,buttonA) + 
	       RadioButton("A Button 2",buttonGrp,buttonB) +
	       RadioButton("A Button 3",buttonGrp) + 
	       RadioButton("A Button 4",buttonGrp) 
	       ) 
	  );
  win.Show();
  
  Manager.Start();
  std::cerr << "Finished... \n";
  return 0;
}
