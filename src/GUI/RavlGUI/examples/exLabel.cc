// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/exLabel.cc"
//! docentry="Ravl.API.Graphics.GTK.Layout"
//! userlevel=Normal

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Label.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Frame.hh"
#include "Ravl/Option.hh"

using namespace RavlGUIN;

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  OptionC opts(nargs,args);
  opts.Check();
  
  WindowC win(100,100,"Hello");
  
  LabelC label1("Left Align");
  GtkJustification justify = GTK_JUSTIFY_LEFT;
  label1.Justify(justify);

  LabelC label2("Right Align");
  justify = GTK_JUSTIFY_RIGHT;
  label2.Justify(justify);

  LabelC label3("Centered");
  justify = GTK_JUSTIFY_CENTER;
  label3.Justify(justify);

  LabelC label4("Justified (and ancient)");
  justify = GTK_JUSTIFY_FILL;
  label4.Justify(justify);

  LabelC label5("This is a very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very very long line.");
  bool wrap(true);
  label5.Wrap(wrap);

  LabelC label6("<span font_size=\"xx-large\" weight=\"bold\">Large bold Pango markup</span>", true);

  win.Add(VBox(FrameC(label1, 1) +
               FrameC(label2, 1) +
               FrameC(label3, 1) +
               FrameC(label4, 1) +
               FrameC(label5, 1) +
               FrameC(label6, 1)));
  
  win.Show();  
  Manager.Start();
  return 0;
}
