// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exCombo.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Lee Gregory"
//! rcsid="$Id$"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Combo.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

//: A callback to handle seleced items
bool HandleCombo (ComboC &  combo, StringC & id)
{
  std::cerr << "\n id: " << id << "\t" << combo.Selected() ; 
  return true ; 
}

int main(int nargs,char *args[]) 
{
  // Start the GUI manager - pass through any command line options
  Manager.Init(nargs,args);
  
  // Setup a list of things to put inside the combo box. 
  DListC<StringC> comboData ; 
  comboData.InsLast("item A") ; 
  comboData.InsLast("item B") ; 
  comboData.InsLast("item C") ; 
  comboData.InsLast("item D") ; 
  
  // Window creation
  WindowC win(100,100,"Combo Test");
  ComboC combo(comboData, true);
  Connect(combo.SigSelected(), HandleCombo, combo, StringC(""));
  win.Add(combo);
  win.GUIShow();
  
  // Cursor manipulation
  const IntT curPos = 3;
  std::cerr << "Setting cursor position to: " << curPos << std::endl;
  combo.GUISetCursorPosition(curPos);
  std::cerr << "Cursor pos reported at: " << combo.GetCursorPosition() << endl << std::endl;

  // Selection manipulation
  const IntT curStart = 2;
  const IntT curEnd = 3;
  std::cerr << "Setting selection from " << curStart << " to " << curEnd << std::endl;
  combo.GUISetCursorSelection(curStart, curEnd);

  IntT start = -1, end = -1;
  if (combo.GetCursorSelection(start, end))
  {
    std::cerr << "Selection reported from " << start << " to " << end << endl << std::endl;
  }
  else
  {
    std::cerr << "No current selection" << endl << std::endl;
  }
  
  // Start the UI
  Manager.Start();
  std::cerr << "Finished... \n";
  return 0;
}
