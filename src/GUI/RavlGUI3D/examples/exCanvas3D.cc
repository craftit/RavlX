// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI3D
//! file="Ravl/GUI/3D/exCanvas3D.cc"
//! docentry="Ravl.API.Graphics.3D"
//! userlevel=Normal

//: 3D GL Canvas 

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/Slider.hh"
#include "Ravl/GUI/LBox.hh"
#include "Ravl/GUI/Canvas3D.hh"
#include "Ravl/GUI/DViewPoint3D.hh"
#include "Ravl/GUI/DCube3D.hh"
#include "Ravl/GUI/DTriMesh3D.hh"
#include "Ravl/GUI/DLight3D.hh"
#include "Ravl/GUI/MouseEvent.hh"
#include "Ravl/Option.hh"
#include "Ravl/GUI/Bitmap.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/OS/Date.hh"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

using namespace RavlN;
using namespace RavlGUIN;


bool InitGL()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  //glEnable(GL_DEPTH_TEST);

  glEnable(GL_COLOR_MATERIAL);

  // some effects 


  return true;
}

bool pressFunc(MouseEventC &me) {
  std::cerr << "Press " << me.Row() << " " << me.Col() << "\n";
  return true;
}

bool releaseFunc(MouseEventC &me) {
  std::cerr << "Release " << me.Row() << " " << me.Col() << "\n";
  return true;
}

bool moveFunc(MouseEventC &me) {
  std::cerr << "Move " << me.Row() << " " << me.Col() << "\n";
  return true;
}

using namespace Ravl3DN;

int main(int nargs,char *args[]) 
{
  Manager.Init(nargs,args);
  
  OptionC opts(nargs,args); // Make sure help option is detected.
  bool doMotion = opts.Boolean("m",false,"Detect motion events.");
  bool useDisplayList = opts.Boolean("dl",false,"Use display lists. ");
  RealT increment     = opts.Real("i",1,"control the size of the step in the loop") ;
 
  opts.Check();

  DObject3DC object = DCube3DC(Vector3dC(1,1,1),RealRGBValueC(1.0,1.0,0));
  
  WindowC win(100,100,"Hello");
  Canvas3DC area(800,800);
  area.SetRenderMode(C3D_SMOOTH);
  Connect(area.Signal("button_press_event"),&pressFunc);
  Connect(area.Signal("button_release_event"),&releaseFunc);
  if(doMotion)
    Connect(area.Signal("motion_notify_event"),&moveFunc);
  
  win.Add(area);
  std::cerr << "Starting gui. \n";
  Manager.Execute();
  win.Show();
  Sleep(1);
  area.Render(&InitGL); // Set things up.
  area.Light(RealRGBValueC(0.95,0.95,0.95),Point3dC(3,3,10));
  area.ViewPoint(90,Point3dC(0,0,3)); // Setup view point.
  
  if(useDisplayList)
    object.EnableDisplayList();
  
  
  Vector3dC rotation ( .1,1,.1) ; 
  rotation.MakeUnit() ;
  
  for(RealT i = 0;i < 10000;i++) {
    // Draw rotated cube
    area.ClearBuffers() ;
   
    area.Transform (i * increment, rotation, object); // Object to draw.
    area.SwapBuffers();
    Sleep(0.05);
  }
  std::cerr << "Execute done, waiting... \n";
  Manager.Wait();
  return 0;
}
 
