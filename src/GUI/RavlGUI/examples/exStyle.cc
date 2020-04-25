// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI
//! author="James Smith"
//! date="28/10/2002"
//! userlevel=Normal
//! docentry="Ravl.API.Graphics.GTK.Styles"
//! file="Ravl/GUI/GTK/exStyle.cc"

#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Pixmap.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Frame.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/GUI/WidgetStyle.hh"
#include "Ravl/Option.hh"

using namespace RavlN;
using namespace RavlGUIN;
using namespace RavlImageN;

int main(int nargs,char *args[]) {

  OptionC opts(nargs,args);
  StringC file = opts.String("i",PROJECT_OUT "/share/RAVL/pixmaps/monkey.ppm","input filename");
  opts.Check();

  // Initialise GUI manager
  Manager.Init(nargs,args);

  // Load an image to use as background
  ImageC<ByteRGBValueC> img;
  if (!RavlN::Load(file,img)) {
    return 1;
  }

  // Create main window
  WindowC win(256,256,"exStyle");
  win.Show();
  // Create style
  WidgetStyleC windowstyle;
  // Create pixmap
  PixmapC pixmap(win,img);
  // Set background
  windowstyle.SetBackground(pixmap);
  // Set style
  win.SetStyle(windowstyle);

  // Add button
  ButtonC button("Test");
  win.Add(FrameC(button,"exStyle",100));
  // Setup button style
  WidgetStyleC buttonstyle;
  // Set font
  StringC font("Arial 30");
  buttonstyle.SetFont(font);
  // Set red background in all modes
  WidgetColourTypeT coltype = WIDGET_COLOUR_BG;
  ByteRGBValueC col(255,0,0);
  buttonstyle.SetColour(coltype,col);
  // Set green background in active mode
  col = ByteRGBValueC(0,255,0);
  GtkStateType state = GTK_STATE_ACTIVE;
  buttonstyle.SetColour(coltype,col,state);
  // Set yellow text when active
  coltype = WIDGET_COLOUR_FG;
  col = ByteRGBValueC(255,255,0);
  buttonstyle.SetColour(coltype,col,state);
  // Set Style
  button.SetStyle(buttonstyle);

  // Start GUI
  Manager.Execute();

  // Done
  Manager.Wait();
  return 0;
}
