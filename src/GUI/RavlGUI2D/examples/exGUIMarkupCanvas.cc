// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006, Omniperception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: fixSource.cfg 5642 2006-06-23 10:51:11Z craftit $"
//! lib=RavlGUI2D
//! docentry="Ravl.API.Graphics.GTK.Markup Canvas"

#include "Ravl/GUI/GUIMarkupCanvas.hh"
#include "Ravl/GUI/MarkupImageRGB.hh"
#include "Ravl/GUI/MarkupPolygon2d.hh"
#include "Ravl/GUI/MarkupNote.hh"
#include "Ravl/GUI/MarkupPoint2d.hh"
#include "Ravl/GUI/MarkupLine2d.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Slider.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/PackInfo.hh"
#include "Ravl/GUI/Button.hh"

using namespace RavlGUIN;

//======== Globals

GUIMarkupCanvasC canvas;

//======== Constants

enum ZOrderT
{
  ZO_None = -1,
  ZO_Image = 0,
  ZO_Polygon,
  ZO_Text
};

enum LayerIdT
{
  LI_Polygons = 1,    // Default for ZO_Polygon
  LI_Text,            // Default for ZO_Text
  LI_DottedPolygons,
  LI_Selected
};

//======== Functions

bool UpdateROffset(RealT &roff) {
  Vector2dC tmp = canvas.Offset();
  tmp[0] = roff;
  canvas.GUISetOffset(tmp);
  canvas.GUIRefresh();
  return true;
}

bool UpdateCOffset(RealT &coff) {
  Vector2dC tmp = canvas.Offset();
  tmp[1] = coff;
  canvas.GUISetOffset(tmp);
  canvas.GUIRefresh();
  return true;
}

bool ShowLayerList() {
  canvas.GUIShowLayerDialog();
  return true;
}

bool BackMenuReq() {
  std::cerr << "Back menu requested. \n";
  return true;
}

bool DoublePressEvent() {
  std::cerr << "Double press event. \n";
  return true;
}

//======== Main

int main(int nargs,char **argv) {
  Manager.Init(nargs,argv); 
  WindowC win(400,400,"Polygon canvas");
  
  // Set up the canvas
  FrameMarkupC fm;
  canvas = GUIMarkupCanvasC(100,100);
  
  // Create the layers
  //--
  MarkupLayerInfoC infoPolygon(LI_Polygons, "Polygon", ByteRGBValueC(255, 0, 255), MLI_LineNormal, true);
  infoPolygon.AddZOrder(ZO_Polygon);
  canvas.GUIAddLayerInfo(infoPolygon);
  //--
  MarkupLayerInfoC infoText(LI_Text, "Text", ByteRGBValueC(128, 0, 128));
  infoText.AddZOrder(ZO_Text);
  canvas.GUIAddLayerInfo(infoText);
  //--
  canvas.GUIAddLayerInfo(MarkupLayerInfoC(LI_DottedPolygons, "Dotted Polygons",  MLI_LineDotted));
  //--
  canvas.GUIAddLayerInfo(MarkupLayerInfoC(LI_Selected,       "Selected",         ByteRGBValueC(255, 255, 0)));

  // Set the selected layer id
  canvas.SetSelectedLayer(LI_Selected);
  
  // Connect back menu request.
  Connect(canvas.SignalBackMenu(),&::BackMenuReq);

  // Connect double press event.
  Connect(canvas.SigDoublePress(),&::DoublePressEvent);

  // Create an image.
  ImageC<ByteRGBValueC> img(256,256);
  for(int i = 0;i <256;i++)
    for(int j = 0;j < 256;j++)
      img[i][j] = ByteRGBValueC(i,j,0);

  fm.Markup().InsLast(MarkupImageRGBC(-1,ZO_Image,img));
  
  // Create a small poly.
  Polygon2dC smallPoly;
  smallPoly.InsLast(Point2dC(20,20));
  smallPoly.InsLast(Point2dC(20,50));
  smallPoly.InsLast(Point2dC(50,50));
  smallPoly.InsLast(Point2dC(50,20));

  MarkupPolygon2dC smallPolyMarkup(1,ZO_Polygon,smallPoly);
  smallPolyMarkup.PushLayer(LI_DottedPolygons);
  fm.Markup().InsLast(smallPolyMarkup);
  
  // Create a poly.
  Polygon2dC poly;
  poly.InsLast(Point2dC(10,-20));
  poly.InsLast(Point2dC(10,100));
  poly.InsLast(Point2dC(100,100));
  poly.InsLast(Point2dC(100,-20));
  
  MarkupPolygon2dC polyMarkup(2,ZO_Polygon,poly);
  polyMarkup.Insert(smallPolyMarkup);
  fm.Markup().InsLast(polyMarkup);
  
  // Create a note
  MarkupNoteC noteMarkup(-1,ZO_Text,"Hello",polyMarkup);
  fm.Markup().InsLast(noteMarkup);

  // Create markup eye point
  Point2dC point(50,100);
  MarkupPoint2dC pointMarkup(3,ZO_Polygon,point,MP2DS_DiagonalCross);
  pointMarkup.PushLayer(LI_DottedPolygons);
  fm.Markup().InsLast(pointMarkup);
  
  // Create markup line
  fm.Markup().InsLast(MarkupLine2dC(-1,ZO_Polygon,Point2dC(30,30),Vector2dC(10,0),100.0));
  
  canvas.GUIUpdateMarkup(fm);
  
  // Create the sliders
  SliderC roffset = SliderH(1,0,100,1,&UpdateROffset);
  SliderC coffset = SliderH(1,0,100,1,&UpdateCOffset);
  
  // Pack and show
  win.Add(VBox(PackInfoC(canvas,true,true) +
               PackInfoC(roffset,false,true) +
               PackInfoC(coffset,false,true) +
               PackInfoC(Button("Layer List", ShowLayerList), false, true)));
  win.GUIShow();
  Manager.Start();

  return 0;
}
