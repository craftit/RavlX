// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exTreeStore.cc"

//: Example of TreeStoreC
// Available on GTK+-2.0 and above only.

#include "Ravl/config.h"


#include "Ravl/GUI/TreeStore.hh"
#include "Ravl/GUI/TreeView.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Option.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Pixmap.hh"
#include "Ravl/DP/AttributeValueTypes.hh"
#include <gtk/gtk.h>

using namespace RavlGUIN;


TreeStoreC treeStore;


bool EditCallback(TreeModelIterC &at,StringC &str2) {
  std::cerr << "Edit callback. " << str2 << "\n";
  treeStore.GUISetValue(at,1,str2);
  return true;
}

bool ToggleCallback(TreeModelIterC &at) {
  std::cerr << "Toggle callback - ";
  bool inconsistent;
  treeStore.GetValue(at,5,inconsistent);
  if (inconsistent)
  {
    std::cerr << "inconsistent" << std::endl;
  }
  else
  {
    bool val;
    treeStore.GetValue(at,3,val);
    std::cerr << (val ? "on" : "off") << " -> " << (!val ? "on" : "off") << std::endl;
    val = !val;
    treeStore.GUISetValue(at,3,val);
  }
  return true;
}

int main(int nargs,char **argv) {
  Manager.Init(nargs,argv);
  OptionC opts(nargs,argv);
  opts.Check();
  
  WindowC win(100,100,"Hello");
  
  //: Create tree store.
  
  SArray1dC<AttributeTypeC> types(6);
  //types[0] = AttributeTypeNumC<IntT>("ANumber","...");
  types[0] = AttributeTypeStringC("Another","...");
  types[1] = AttributeTypeStringC("AString","...");
  types[2] = AttributeTypeMiscC("AImage","...",AVT_ByteRGBImage);
  types[3] = AttributeTypeBoolC("ABool","...");   
  types[4] = AttributeTypeStringC("Colour","..."); // This is used for controlling the colour of column 1
  types[5] = AttributeTypeBoolC("BoolState","..."); // This is used for controlling the inconsistent state of column 3
  treeStore = TreeStoreC(types);
  
  //: Put some data into the tree store.
  
  TreeModelIterC iter;
  treeStore.AppendRow(iter);
  //treeStore.GUISetValue(iter,0,true);
  treeStore.GUISetValue(iter,0,"meep");
  treeStore.GUISetValue(iter,1,"hello");
  PixbufC map(xpmData_OpenFile);
  treeStore.GUISetValue(iter,2, map);
  treeStore.GUISetValue(iter,3,true);
  treeStore.GUISetValue(iter,4,"red");
  treeStore.GUISetValue(iter,5,false);
  
  // Add another line to the tree store.
  TreeModelIterC iter1;
  treeStore.AppendRow(iter1);
  treeStore.GUISetValue(iter1,0,"foop");
  treeStore.GUISetValue(iter1,1,"ping");
  treeStore.GUISetValue(iter1,2, map);
  treeStore.GUISetValue(iter1,3,false);
  treeStore.GUISetValue(iter1,4,"#0000ff");
  treeStore.GUISetValue(iter1,5,true);

  // Add another line to the tree store.
  TreeModelIterC iter2;
  treeStore.AppendRow(iter2,iter);
  treeStore.GUISetValue(iter2,0,"igloo");
  treeStore.GUISetValue(iter2,1,"child");
  treeStore.GUISetValue(iter2,2, map); //GTK_STOCK_ADD
  treeStore.GUISetValue(iter2,3,false);
  treeStore.GUISetValue(iter2,4,"green");
  treeStore.GUISetValue(iter2,5,false);

  // Add another line to the tree store.
  TreeModelIterC iter3;
  treeStore.AppendRow(iter3,iter);
  treeStore.GUISetValue(iter3,0,"boink");
  treeStore.GUISetValue(iter3,1,"child");
  treeStore.GUISetValue(iter3,2, map);
  treeStore.GUISetValue(iter3,3,false);
  treeStore.GUISetValue(iter3,4,"yellow");
  treeStore.GUISetValue(iter3,5,false);
  
  // Make a list of columns we want to see from the store.
  
  DListC<StringC> cols;
  cols.InsLast("@-AImage|Another"); // Put AImage in the same column with no expansion
  cols.InsLast("AString");
  cols.InsLast("AImage");
  cols.InsLast("ABool");
  
  // Create a new tree view displaying the columns
  TreeViewC treeView(treeStore,cols);
  
  // Use the colour from column 'Colour' from the store to set the foreground of column 1
  treeView.SetAttribute("AString","foreground","Colour"); 
  
  // Allow bool to be inconsistent
  treeView.SetAttribute("ABool","inconsistent","BoolState");

  // Always set the editable flag to true for column 1.
  treeView.SetAttribute("AString","editable","1",false);
  
  // Always set the activatable flag to true for column 3.
  treeView.SetAttribute("ABool","activatable","1",false);

  // Always set the sortable flag to true for column 3.
  treeView.SetAttribute("Another","sortable","1",false);
  treeView.SetAttribute("AString","sortable","1",false);

  // Allow all columns to be reordered
  treeView.SetAttribute("Another","reorderable","1",false); 
  treeView.SetAttribute("AString","reorderable","1",false); 
  treeView.SetAttribute("AImage","reorderable","1",false);
  treeView.SetAttribute("ABool","reorderable","1",false);

  // Allow all columns to be resized (using column index instead of name)
  treeView.SetAttribute(0,"resizable","1",false); 
  treeView.SetAttribute(1,"resizable","1",false); 
  treeView.SetAttribute(2,"resizable","1",false);
  treeView.SetAttribute(3,"resizable","1",false);

  // Connect the changed signal for column 1 to 'EditCallback'
  Connect(treeView.ChangedSignal("AString"),&EditCallback);

  // Connect the changed signal for column 3 to 'ToggleCallback'  
  Connect(treeView.ChangedSignal("ABool"),&ToggleCallback);
  
  // Setup the widgets, and off we go!
  win.Add(treeView);
  
  treeView.GUISort("AString",true); 

  win.GUIShow();

  Manager.Start();
  return 0;
}
