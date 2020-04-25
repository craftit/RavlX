// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI2D
//! file="Ravl/GUI/2D/exCanvas.cc"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! author="Charles Galambos"
//! date="17/03/1999"

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Canvas.hh"
#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Image/ByteRGBAValue.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

int exCanvas(int nargs,char *args[]) 
{
  using namespace RavlGUIN;
  
  Manager.Init(nargs,args); // Initalise GUI manager, let it read args.
  
  // Do normal option processing ....
  
  OptionC option(nargs,args);
  option.Check();
  
  // Setup GUI...
  
  CanvasC draw(200,200); // Make a canvas to draw into.  
  
  WindowC win(300,300,"Hello"); // Make a new window called hello.
  win.Add(LBoxC(draw,false));   // Put draw area inside a window.
  win.Show();                    // Display this lot on the screen.
  
  // Do some work with the gui...
  
  ONDEBUG(std::cerr << "Drawing stuff... \n");
  
  ImageC<ByteRGBValueC> img(100,100);
  for(int x=0;x <100;x++) {
    for(int y=0;y <100;y++)
      img[x][y] = ByteRGBValueC(x*2+25,y*2+25,0);
  }
  draw.DrawImage(img,Index2dC(50,50));
  
  
  draw.DrawLine(10,10,100,100,1); // Draw a line from 10,10 to 100,100 colour 1
  draw.DrawLine(100,10,10,100,2); // Draw a line from 100,10 to 10,100 colour 2
  draw.DrawLine(10,10,100,10,3);
  draw.DrawLine(10,100,100,100,0);
  draw.DrawText(10,50,"Hello!");
  
  ONDEBUG(std::cerr << "Executing manager... \n");
  
  Manager.Start();              // Wait for GUI to finish.
  
  return 0;
}


RAVL_ENTRY_POINT(exCanvas);
