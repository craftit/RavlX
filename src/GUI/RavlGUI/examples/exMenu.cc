// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exMenu.cc"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Charles Galambos"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/RawCanvas.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Menu.hh"
#include "Ravl/GUI/MenuCheck.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/FileSelector.hh"
#include "Ravl/GUI/StatusBar.hh"

using namespace RavlGUIN;

namespace
{

  const RealT g_statusBarDelay = 2;
  const int g_popupMenuButton = 2;

}

bool addMenu(MenuC &menu)
{
  static int count = 1;
  StringC menuTitle;

  menuTitle.form("Sub menu %d", count);
  MenuC newMenu = MenuC(menuTitle);
  menu.GUIAdd(newMenu);

  menuTitle.form("Add to sub menu %d", count);
  MenuItemC newNenuItem = MenuItem(menuTitle, &addMenu, newMenu);
  menu.GUIAdd(newNenuItem);

  count++;

  return true;
}

bool MousePress(MouseEventC &me, MenuC &menu)
{
  if(!me.IsPressed(g_popupMenuButton) && me.HasChanged(g_popupMenuButton))
    menu.Popup(me);

  return true;
}

bool onQuit()
{
  Manager.Quit(); // Initate shutdown.
  return true;
}

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  
  WindowC win(100,100,"Hello");
  
  FileSelectorC fs;

  StatusBarC statusBar("Hello");

  MenuC subMenu("Sub menu 0");
  MenuC anotherEmbeddedMenu("Another sub menu");
  subMenu.GUIAdd(anotherEmbeddedMenu);

  RealT delay = g_statusBarDelay;
  MenuBarC menuBar(MenuC("File",
                     MenuItemShow("Open", fs) +
                     MenuItemSeparator() +
                     MenuItem("Not Implemented 1", statusBar, &StatusBarC::PushTimedF, delay, StringC("Not Implemented 1...")) +
                     MenuItem("Not Implemented 2", statusBar, &StatusBarC::PushF, StringC("Not Implemented 2...")) +
                     MenuItemSeparator() +
                     subMenu +
                     MenuItem("Add to sub menu 0", &addMenu, subMenu) +
                     MenuItemSeparator() +
                     MenuCheckItemC("Check me out") +
                     MenuItemSeparator() +
                     MenuItem("Quit", onQuit)));

  MenuC popupMenu("Popup");
  popupMenu.GUIAdd(MenuItem("Not Implemented 3", statusBar, &StatusBarC::PushTimedF, g_statusBarDelay, StringC("Not Implemented 3...")));
  MenuC popupSubMenu("SubPop");
  popupSubMenu.GUIAdd(MenuItem("Not Implemented 4", statusBar, &StatusBarC::PushTimedF, g_statusBarDelay, StringC("Not Implemented 4...")));
  popupMenu.GUIAdd(popupSubMenu);

  RawCanvasC canvas(100, 100);
  MouseEventC dummyMouseEvent;
  Connect(canvas.Signal("button_press_event"), &MousePress, dummyMouseEvent, popupMenu);
  
  win.Add(VBox(menuBar + 
            canvas +
            statusBar));
  win.Show();
  statusBar.Next("Ready...");
  
  Manager.Start();
  std::cerr << "Finished.\n";
  return 0;
}
