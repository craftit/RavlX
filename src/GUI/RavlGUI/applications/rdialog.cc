// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, Omniperception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/GUI/GTK/rdialog.cc"
//! author="James Smith"
//! docentry="Ravl.API.Graphics.GTK.Dialogs"
//! userlevel=Normal
//! lib=RavlGUI

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/MessageBox.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;
using namespace RavlN;

int g_iResult(1);

bool ok() {
  // Set return value to 0
  g_iResult = 0;
  // Stop GUI
  Manager.Quit();
  // Done
  return true;
}

bool okcancel(bool& bResult) {
  // Set return value to 1  
  g_iResult = bResult ? 0 : 1;
  // Stop GUI
  Manager.Quit();
  // Done
  return true;
}

int rdialog(int argc, char** argv) {

  // Get command line options
  OptionC opts(argc,argv);
  bool bYesNo = opts.Boolean("y",false,"Create yes/no dialog.");
  StringC strMessage = opts.String("","","Dialog text.");
  opts.Check();

  // Setup GUI
  Manager.Init(argc,argv);
  Manager.Execute();

  // Create message box
  if (bYesNo) {
    QuestionBox(strMessage,okcancel);
  }
  else {
    AlertBox(strMessage,ok);
  }
  
  // Wait for GUI to close
  Manager.Wait();

  // Return result;
  return g_iResult;
}


RAVL_ENTRY_POINT(rdialog);
