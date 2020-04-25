// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2010, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exComboBoxEntry.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Warren Moore"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/ComboBoxEntry.hh"
#include "Ravl/GUI/ListStore.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

bool HandleCombo(TreeModelIterC &row, TextEntryC& entry)
{
  std::cerr << "Combo changed: '" <<  entry.GUIText() << "'\n";
  return true;
}

bool HandleEntry(StringC &text)
{
  std::cerr << "Entry activated: '" << text << "'\n";
  return true ;
}

int main(int nargs,char *args[]) 
{
  // Start the GUI manager: pass through any command line options.
  Manager.Init(nargs,args);
  
  // Create the model.
  SArray1dC<AttributeTypeC> colTypes(1);
  colTypes[0] = AttributeTypeStringC("value", "");
  ListStoreC listStore = ListStoreC(colTypes);

  // Fill the model with data.
  DListC<StringC> comboData;
  comboData.InsLast("item A") ;
  comboData.InsLast("item B") ;
  comboData.InsLast("item C") ;
  comboData.InsLast("item D") ;

  for (DLIterC<StringC> listIter(comboData); listIter; listIter++)
  {
    TreeModelIterC modelIter;
    if (listStore.AppendRow(modelIter))
      listStore.GUISetValue(modelIter, 0, *listIter);
  }

  // Create the window.
  WindowC win(100, 100, "Combo Box Entry");
  ComboBoxEntryC combo(listStore, 0);
  win.Add(combo);
  win.GUIShow();

  // Get the text entry.
  TextEntryC entry = combo.TextEntry();
  Connect(entry.Activate(), HandleEntry, StringC(""));
  Connect(combo.SigRowSelected(), HandleCombo, TreeModelIterC(), entry);

  // Start the UI.
  Manager.Start();
  std::cerr << "Finished... \n";
  return 0;
}
