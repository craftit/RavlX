// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=Ravl3D

//! docentry="Ravl.API.3D.Mesh"
//! author="Charles Galambos"
//! date="22/2/2008"

#include "Ravl/3D/BuildTexTriMesh.hh"
#include "Ravl/IO.hh"
#include "Ravl/Option.hh"

using namespace RavlN;
using namespace RavlImageN;

int main(int nargs,char **argv) {
  OptionC opts(nargs,argv); // Make sure help option is detected.
  StringC file = opts.String("i",PROJECT_OUT "/share/RAVL/pixmaps/monkey.ppm","input filename");
  bool verbose = opts.Boolean("v",false,"Verbose mode. ");
  opts.Check();
  
  ImageC<ByteRGBValueC> img;
  if (!RavlN::Load(file,img,"",verbose)) {
    cerr << "Could not load input file " << file << endl;
    return 1;
  }
  
  Ravl3DN::BuildTexTriMeshC buildMesh;
  
  //! Append a new texture, its id is returned.
  UIntT texId = buildMesh.AddTexture(img);
  
  SArray1dC<Point2dC> outline = SArray1dC<Point2dC>(4);
  SArray1dC<Point3dC> positions = SArray1dC<Point3dC>(4);
  outline[0] = img.Frame().TopLeft();
  outline[1] = img.Frame().TopRight();
  outline[2] = img.Frame().BottomRight();
  outline[3] = img.Frame().BottomLeft();
  for(int i = 0;i < 4;i++)
    positions[i] = Point3dC(outline[i][0],outline[i][1],0);
  
  buildMesh.AddFace(texId,positions,outline);
  
  // Show mesh to the user.
  RavlN::Save("@GL:Example",buildMesh.Build());
  
  return 0;
}
