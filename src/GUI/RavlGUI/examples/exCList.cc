// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exCList.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/CList.hh"


using namespace RavlGUIN;

bool GUISelectSalvo(CListEventC &cle) {
  std::cerr << "GUISelectSalvo(), Called \n";
  return true;
}

bool GUIUnselectSalvo(CListEventC &cle) {
  std::cerr << "GUIUnselectSalvo(), Called \n";
  return true;
}

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  
  WindowC win(100,100,"Hello");
    
  const char *titles[] = {"one","two","three",0};
  CListC aList(titles);
  
  Connect(aList.Signal("select_row"),&GUISelectSalvo);
  Connect(aList.Signal("unselect_row"),&GUIUnselectSalvo);

  win.Add(aList);
  win.Show();
  Manager.Execute();
  
  //aList.Add(StringC("Hi!!!"));
  SArray1dC<StringC> lineInfo(3);
  lineInfo[0] = "x";
  lineInfo[1] = "y";
  lineInfo[2] = "z";
  int val = 0;
  aList.AppendLine(val,lineInfo);
  val = 1;
  aList.AppendLine(val,lineInfo);
  
  Manager.Wait();

  return 0;
}
