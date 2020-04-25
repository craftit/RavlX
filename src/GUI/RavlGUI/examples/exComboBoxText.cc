// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exComboBoxText.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Lee Gregory"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/ComboBoxText.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

bool HandleEntry(TreeModelIterC &row, ComboBoxTextC& combo)
{
  std::cerr << "Entry changed: '" <<  combo.GUITextSelected() << "'\n";
  return true;
}

bool HandleCombo(StringC& text, StringC& name)
{
  std::cerr << name << " ComboBoxTextC selected: '" << text << "'\n";
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

  // Create the combo boxes.
  ComboBoxTextC comboEditable(comboData, true);
  Connect(comboEditable.SigTextSelected(), HandleCombo, StringC(""), StringC("Editable"));
  Connect(comboEditable.SigRowSelected(), HandleEntry, TreeModelIterC(), comboEditable);

  ComboBoxTextC comboNotEditable(comboData, false);
  Connect(comboNotEditable.SigTextSelected(), HandleCombo, StringC(""), StringC("Not editable"));

  // Create the window.
  DListC<WidgetC> widgetList;
  widgetList.Append(comboEditable);
  widgetList.Append(comboNotEditable);
  LBoxC vbox(widgetList, true, 2, true, 2);

  WindowC win(100, 100, "Combo Box Text");
  win.Add(vbox);
  win.GUIShow();
  
  // Start the UI.
  Manager.Start();
  std::cerr << "Finished... \n";
  return 0;
}
