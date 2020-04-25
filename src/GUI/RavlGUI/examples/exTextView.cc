// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/exTextView.cc"
//! lib=RavlGUI
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.GTK.Control"

#include "Ravl/Option.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/TextView.hh"
#include "Ravl/GUI/Manager.hh"

using namespace RavlGUIN;

int main(int argc, char *argv[]) 
{
  Manager.Init(argc, argv);
  OptionC opts(argc, argv);
  opts.Check();
  
  // Create the window and text boxes
  WindowC win(100, 100, "Text View Example");
  
  TextViewC textView("I wandered lonely as a cloud\n\
That floats on high o'er vales and hills,\n\
When all at once I saw a crowd,\n\
A host, of golden daffodils;\n\
Beside the lake, beneath the trees,\n\
Fluttering and dancing in the breeze.");

  win.Add(textView);
  win.GUIShow();
  
  Manager.Start();
  return 0;
}
