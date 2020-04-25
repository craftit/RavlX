// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exComboBoxEntryText.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Warren Moore"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/ComboBoxEntryText.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

bool HandleCombo(StringC& text, ComboBoxEntryTextC &combo)
{
  std::cerr << "Combo selected: '" << text << "'\t'" << combo.TextSelected() << "'\n";
  return true ;
}

bool HandleEntry(StringC &text, TextEntryC& entry)
{
  std::cerr << "Entry activate: '" << text << "'\t'" << entry.Text() << "'\n";
  return true ;
}

int main(int nargs,char *args[]) 
{
  // Start the GUI manager: pass through any command line options.
  Manager.Init(nargs,args);
  
  // Setup a list of things to put inside the combo box. 
  DListC<StringC> comboData ; 
  comboData.InsLast("item A") ; 
  comboData.InsLast("item B") ; 
  comboData.InsLast("item C") ; 
  comboData.InsLast("item D") ; 

  // Create the combo box.
  ComboBoxEntryTextC combo(comboData);
  Connect(combo.SigTextSelected(), HandleCombo, StringC(""), combo);

  // Create the window.
  WindowC win(100, 100, "Combo Box Entry Text");
  win.Add(combo);
  win.GUIShow();

  // Get the text entry.
  TextEntryC entry = combo.TextEntry();
  Connect(entry.Activate(), HandleEntry, StringC(""), entry);
  
  // Start the UI.
  Manager.Start();
  std::cerr << "Finished...\n";
  return 0;
}
