// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exTextBox.cc"
//! author="Charles Galambos"
//! date="23/03/1999"
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/TextBox.hh"
#include "Ravl/GUI/TextEntry.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

TextEntryC readonly("TextEntryC (read-only)", -1, false, false);
ButtonC button("Toggle read-only");

bool ToggleReadOnly()
{
  bool editable = readonly.Editable();
  std::cerr << "Read-only button is (" << (editable ? "editable" : "read-only") << ")" << std::endl;
  
  readonly.GUISetEditable(!editable);
  std::cerr << "Read-only button set (" << (!editable ? "editable" : "read-only") << ")" << std::endl;
  
  StringC buttonLabel("Toggle ");
  buttonLabel += (!editable ? "editable" : "read-only");
  button.GUISetLabel(buttonLabel);
  return true;
}

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  OptionC opts(nargs,args);
  opts.Check();
  
  // Create the window and text boxes
  WindowC win(100,100,"Hello");
  TextEntryC entry("TextEntryC (normal)");
  TextEntryC password("TextEntryC (password)");
  TextBoxC textBox("TextBoxC\n(multi-line)");
  
  // Configure the widgets
  bool bTrue = true;
  password.GUIHideText(bTrue);
  Connect(button.SigClicked(), &ToggleReadOnly);
  
  // Compose and display the GUI
  win.Add(VBox(entry + HBox(readonly + button) + password + textBox));
  win.GUIShow();  
  Manager.Start();
  return 0;
}
