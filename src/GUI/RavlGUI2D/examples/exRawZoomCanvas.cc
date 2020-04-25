// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlGUI2D
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.GTK.Control"
//! file="Ravl/GUI/2D/exRawZoomCanvas.cc"

#include "Ravl/Image/Image.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/Image/ByteRGBAValue.hh"
#include "Ravl/GUI/RawZoomCanvas.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/Polygon2d.hh"
#include "Ravl/GUI/Slider.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/PackInfo.hh"

using namespace RavlGUIN;

ImageC<ByteRGBValueC> img;
ImageC<ByteRGBAValueC> img2;
Polygon2dC poly;
RawZoomCanvasC canvas;

bool Refresh() {
  canvas.GUIDrawImage(img);
  canvas.GUIDrawImage(img2,Index2dC(0,0)); // ,Index2dC(50,50)
  
  if(!poly.IsEmpty()) {
    Point2dC last = poly.Last();
    for(DLIterC<Point2dC> it(poly);it;it++) {
      canvas.GUIDrawLine(canvas.GUIDrawGCWhite(),last,*it);
      last = *it;
    }
  }

  return true;
}

bool UpdateScale(RealT &scale) {
  canvas.GUISetScale(Vector2dC(scale,scale));
  Refresh();
  return true;
}

bool UpdateROffset(RealT &roff) {
  Vector2dC tmp = canvas.Offset();
  tmp[0] = roff;
  canvas.GUISetOffset(tmp);
  Refresh();
  return true;
}

bool UpdateCOffset(RealT &coff) {
  Vector2dC tmp = canvas.Offset();
  tmp[1] = coff;
  canvas.GUISetOffset(tmp);
  Refresh();
  return true;
}

int main(int nargs,char **argv) {
  Manager.Init(nargs,argv); 
  WindowC win(400,400,"Polygon canvas");
  
  canvas = RawZoomCanvasC(100,100);
  
  // Create an image.
  
  img = ImageC<ByteRGBValueC> (256,256);
  for(int i = 0;i <256;i++)
    for(int j = 0;j < 256;j++)
      img[i][j] = ByteRGBValueC(i,j,0);
  
  img2 = ImageC<ByteRGBAValueC>(100,100);
  for(int x=0;x <100;x++) {
    for(int y=0;y <100;y++)
      img2[x][y] = ByteRGBAValueC(x*2+25,0,y*2+25,((x&0x2) ^ (y&0x2)) != 0 ? 255 : 0);
  }
  
  //canvas.SetBackground(img);
  
  // Create a poly.
  
  poly.InsLast(Point2dC(50,50));
  poly.InsLast(Point2dC(128,50));
  poly.InsLast(Point2dC(128,128));
  poly.InsLast(Point2dC(50,128));
  
  Connect(canvas.Signal("expose_event"),&Refresh);
  
  SliderC scale = SliderH(1,1,10,0.1,&UpdateScale);
  SliderC roffset = SliderH(1,0,100,0.1,&UpdateROffset);
  SliderC coffset = SliderH(1,0,100,0.1,&UpdateCOffset);
  
  win.Add(VBox(canvas + 
               PackInfoC(scale,false,true) + 
               PackInfoC(roffset,false,true) + 
               PackInfoC(coffset,false,true)));
  win.GUIShow();
  Manager.Start();

  return 0;
}
