// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/GUI/GTK/exFileChooser.cc"
//! lib=RavlGUI
//! author = "Warren Moore"

#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/FileChooserDialog.hh"
#include "Ravl/GUI/FileChooserButton.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/TextBox.hh"

using namespace RavlN;
using namespace RavlGUIN;

bool FileChooserDialogResponse(StringC &filename, TextBoxC &textBox)
{
  StringC message;
  message.form("FileChooserDialogResponse filename(%s)\n", filename.chars());
  textBox.Insert(message);
  std::cerr << message;
  return true;
}

bool FileChooserButtonResponse(StringC &filename, TextBoxC &textBox)
{
  StringC message;
  message.form("FileChooserButtonResponse filename(%s)\n", filename.chars());
  textBox.Insert(message);
  std::cerr << message;
  return true;
}

bool DoQuit() 
{
  Manager.Quit();
  return true;
}

int main(int argc, char *argv[])
{
  Manager.Init(argc, argv);
  
  // Create a textbox to show files chosen.
  
  TextBoxC textBox("", true);
  
  // Create the file chooser dialog.

  FileChooserDialogC fileChooserDialog = FileChooserDialogC(FileChooserOpen, "File Chooser Dialog", "");
  Connect(fileChooserDialog.SigSelected(), &FileChooserDialogResponse, "", textBox);

  // Create the file chooser button.

  FileChooserButtonC fileChooserButton = FileChooserButtonC(FileChooserOpen, "File Chooser Button", "");
  Connect(fileChooserButton.SigSelected(), &FileChooserButtonResponse, "", textBox);

  // Add the filters.

  DListC<StringC> allList;
  allList.InsLast("*");
  fileChooserDialog.GUIAddFilter("All files (*)", allList);

  DListC<StringC> headerList;
  headerList.InsLast("*.hh");
  fileChooserDialog.GUIAddFilter("Header files (*.hh)", headerList);

  DListC<StringC> imageList;
  imageList.InsLast("*.jpg");
  imageList.InsLast("*.png");
  imageList.InsLast("*.bmp");

  fileChooserDialog.GUIAddFilter("All files (*)", allList);
  fileChooserDialog.GUIAddFilter("Header files (*.hh)", headerList);
  fileChooserDialog.GUISetFilter("Header files (*.hh)", headerList);

  fileChooserButton.GUIAddFilter("All files (*)", allList);
  fileChooserButton.GUIAddFilter("Header files (*.hh)", headerList);
  fileChooserButton.GUIAddFilter("Image files (*.jpg, *.png, *.bmp)", imageList);
  fileChooserButton.GUISetFilter("Image files (*.jpg, *.png, *.bmp)", imageList);

  // Create the menubar.
  
  MenuBarC menuBar(MenuC("File", 
                         MenuItemShow("Open", fileChooserDialog) +
                         MenuItem("Quit", DoQuit)));
  
  // Create a window with a menu bar and textbox.
  
  WindowC win(100, 100, "FileChooser Examples");
  win.Add(VBox(menuBar + textBox + fileChooserButton));
  win.Show();
  
  // Start the GUI.
  
  Manager.Start();
  
  return 0;
}
