// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! userlevel=Normal
//! author="Charles Galambos"
//! file="Ravl/GUI/GTK/exScrolledArea.cc"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/CList.hh"
#include "Ravl/GUI/ScrolledArea.hh"

using namespace RavlGUIN;

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  
  WindowC win(100,100,"Hello");
    
  const char *titles[] = {"one","two","three",0};
  CListC aList(titles);
  
  win.Add(ScrolledAreaC(aList));
  win.Show();
  Manager.Execute();
  
  for(int i = 0;i < 20;i++) {
    //aList.Add(StringC("Hi!!!"));
    SArray1dC<StringC> lineInfo(3);
    lineInfo[0] = StringC(i);
    lineInfo[1] = "y";
    lineInfo[2] = "z";
    aList.AppendLine(i,lineInfo);
  }  
  Manager.Wait();
  return 0;
}
