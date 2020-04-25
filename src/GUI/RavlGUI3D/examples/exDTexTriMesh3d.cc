// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI3D
//! docentry="Ravl.API.Graphics.3D"
//! author="James Smith"
//! userlevel=Normal

//: Textured mesh display example.

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/View3D.hh"
#include "Ravl/Option.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/GUI/DTexTriMesh3D.hh"
#include "Ravl/GUI/DTriMesh3D.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/3D/MeshShapes.hh"

#include <GL/gl.h>

using namespace RavlN;
using namespace RavlGUIN;
using namespace Ravl3DN;

int main(int nargs,char *args[]) 
{
  OptionC opts(nargs,args); // Make sure help option is detected.
  StringC file = opts.String("i",PROJECT_OUT "/share/RAVL/pixmaps/monkey.ppm","input filename");
  bool verbose = opts.Boolean("v",false,"Verbose mode. ");
  //bool texture = opts.Boolean("t",false,"Use texture. ");
  opts.Check();
  
  DObject3DC object;

  // Load an image.
  
  ImageC<ByteRGBValueC> img;
  if (!RavlN::Load(file,img,"",verbose)) {
    std::cerr << "Could not load input file " << file << std::endl;
    return 1;
  }
  
  // Create a simple mesh.
  
  TexTriMeshC mesh(CreateTriMeshPlane(),true);
  
  // Add a texture to the mesh.
  
  SArray1dC<ImageC<ByteRGBValueC> > textures(1);
  textures[0] = img;
  mesh.Textures() = textures;
  
  // Create a display object for the mesh
  
  object = DTexTriMesh3DC(mesh);
  
  Manager.Init(nargs,args);
  
  WindowC win(100,100,file);  
  View3DC view(400,400);  
  win.Add(view);
  win.Show();  
  
  std::cerr << "Starting gui. \n";
  Manager.Execute();
  
  view.Add(object);
  view.SceneComplete();
  
  Manager.Wait();

  return 0;
}
