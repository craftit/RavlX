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

//: 3D View window example.

#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/LBox.hh"
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
  StringC file = opts.String("i",PROJECT_OUT "/share/RAVL/data/cube.tri","input filename");
  bool verbose = opts.Boolean("v",false,"Verbose mode. ");
  bool texture = opts.Boolean("t",false,"Use texture. ");
  IntT shapeId = opts.Int("s",-1,"Use shape, 0=Plane, 1=Cube, 2=Sphere. ");
  opts.Check();

  TriMeshC mesh;
  switch(shapeId)
  {
  case -1:
    if (!RavlN::Load(file,mesh,"",verbose))
    {
      std::cerr << "Could not load input file " << file << std::endl;
      return 1;
    }
    break;
  case 0: // Plane
    mesh = CreateTriMeshPlane();
    break;
  case 1: // Cube
    mesh = CreateTriMeshCube(1., Point3dC(-1., 0., 0.));
    break;
  case 2: // Sphere
    mesh = CreateTriMeshSphere(5,8,1.0);
    break;
  default:
    std::cerr << "Invalid shape id " << shapeId << "\n";
    return -1;
    break;
  }

  DObject3DC object = texture ? DTexTriMesh3DC(mesh) : DTriMesh3DC(mesh);

  Manager.Init(nargs,args);

  WindowC win(100,100,file);
  LBoxC layout(false);
  View3DC view(400,400);
  View3DC view2(400,400);
  View3DC view3(400,400);
  layout.Add(view);
  layout.Add(view2);
  layout.Add(view3);
  win.Add(layout);
  win.Show();

  std::cout << "Starting gui. \n";
  Manager.Start();

  view.Add(object);
  view.SceneComplete();
  view2.Add(object);
  view2.SceneComplete();
  view3.Add(object);
  view3.SceneComplete();

  // Slave views together
  view.Master(true);
  view.Slave(false);
  Connect(view.SigRotationTx(), view2.SigRotationRx());
  Connect(view.SigRotationTx(), view3.SigRotationRx());
  view2.Master(true);
  view2.Slave(true);
  Connect(view2.SigRotationTx(), view.SigRotationRx());
  Connect(view2.SigRotationTx(), view3.SigRotationRx());
  view3.Master(true);
  view3.Slave(true);
  Connect(view3.SigRotationTx(), view.SigRotationRx());
  Connect(view3.SigRotationTx(), view2.SigRotationRx());

  Manager.Wait();

  return 0;
}
