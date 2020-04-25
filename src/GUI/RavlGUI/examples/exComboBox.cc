// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2010, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exComboBox.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Warren Moore"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/ComboBox.hh"
#include "Ravl/GUI/ListStore.hh"
#include "Ravl/GUI/TreeStore.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Option.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/GUI/Pixmap.hh"

using namespace RavlGUIN;

bool HandleCombo(TreeModelIterC &row, ListStoreC &listStore)
{
  if (row.IsElm())
  {
    StringC val;
    if (listStore.GetValue(row, 0, val))
    {
      std::cerr << "Selected: '" << val<< "'" << std::endl;
    }
  }

  return true;
}

int main(int nargs,char *args[]) 
{
  // Start the GUI manager: pass through any command line options.
  Manager.Init(nargs,args);

  // Create the model.
  SArray1dC<AttributeTypeC> colTypes(4);
  colTypes[0] = AttributeTypeStringC("string value", "");
  colTypes[1] = AttributeTypeBoolC("bool value", "");
  colTypes[2] = AttributeTypeNumC<Int64T>("64-bit int value", "");
  colTypes[3] = AttributeTypeMiscC("image value", "...", AVT_ByteRGBImage);
  ListStoreC listStore(colTypes, 2);

  // Fill the model with data.
  PixbufC map(xpmData_OpenFile);
  for (Int64T index = 0; index < 4; ++index)
  {
    TreeModelIterC modelIter;
    if (listStore.AppendRow(modelIter))
    {
      StringC stringVal;
      stringVal.form("'Index %d'", index);
      listStore.GUISetValue(modelIter, 0, stringVal);
      listStore.GUISetValue(modelIter, 1, true);
      listStore.GUISetValue(modelIter, 2, (UINT_MAX * 2) - index);
      listStore.GUISetValue(modelIter, 3, map);
    }
  }

  // Create the list of visible columns.
  DListC<IntT> colList;
  colList.Append(0);
  colList.Append(2);
  colList.Append(1);
  colList.Append(3);

  // Create the combo box.
  ComboBoxC combo(listStore, colList);
  Connect(combo.SigRowSelected(), HandleCombo, TreeModelIterC(), listStore);

  // Create the window.
  WindowC win(100, 100, "Combo Box");
  win.Add(combo);
  win.GUIShow();
  
  // Set a selected row.
  combo.GUISetSelectionIndex(1);

  // Start the UI.
  Manager.Start();
  std::cerr << "Finished... \n";
  return 0;
}
