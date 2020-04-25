// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, Omniperception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exMessageBox.cc"
//! author="James Smith"
//! docentry="Ravl.API.Graphics.GTK.Dialogs"
//! userlevel=Normal

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/MessageBox.hh"
#include "Ravl/GUI/ButtonBox.hh"

using namespace RavlGUIN;

bool AlertBoxDone()
{
  std::cerr << "Alert box finished!" << std::endl;
  return true;
}

bool AlertBoxStart()
{
  AlertBox("This is just a message box", AlertBoxDone);
  return true;
}

bool QuestionBoxDone(bool &bResult)
{
  std::cerr << "Question box finished! Result: ";
  if (bResult) {
    std::cerr << "Yes!" << std::endl;
  }
  else {
    std::cerr << "No!" << std::endl;
  }  
  return true;
}

bool QuestionBoxStart()
{
  QuestionBox("Do you want to run a pointless function?", QuestionBoxDone);
  return true;
}

bool OnButtonClick(UIntT &buttonNo)
{
  if(buttonNo == 0) 
    std::cerr << "Yes" << std::endl;
  else if(buttonNo ==1) 
    std::cerr << "No" << std::endl;
  else if(buttonNo == 2)
    std::cerr << "Cancel" << std::endl;
  else {
    std::cerr << "button no too high" << std::endl;
    return false;
  }
  return true;
}

bool ButtonBoxStart()
{
  SArray1dC<StringC> button(3);
  button[0] = "Yes";
  button[1] = "No";
  button[2] = "Cancel";
  ButtonBox("Do something here?", button, "Action Required", OnButtonClick); 
  return true;
}

bool QuitBoxDone(bool &bResult)
{
  if (bResult)
    Manager.Quit();
  return true;
}

bool QuitBoxStart()
{
  QuestionBox("Are you sure you want to quit?", QuitBoxDone);
  return true;
}

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);

  WindowC win(100, 100, "Message Box");
  
  win.Add(
           VBox
           (
             Button("Question Box", QuestionBoxStart) +
             Button("Alert Box", AlertBoxStart) + 
             Button("Button Box", ButtonBoxStart) +
             Button("Quit", QuitBoxStart)
           )
         );
  win.GUIShow();
 
  Manager.Start();

  std::cerr << "Finished... \n";
  return 0;
}
