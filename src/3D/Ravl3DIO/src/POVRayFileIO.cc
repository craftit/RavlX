// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/3D/POVRayFileIO.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/DP/FileFormatIO.hh"
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/POVRayFileIO.cc"

namespace Ravl3DN {

  // TriMesh POVRay File IO /////////////////////////////////////////////

  //: Open file.
  DPOPOVRayFileBodyC::DPOPOVRayFileBodyC(const StringC &fn)
    : outf(fn),
      done(false)
  {}
  
  //: Open file.
  DPOPOVRayFileBodyC::DPOPOVRayFileBodyC(OStreamC &is)
    : outf(is),
      done(false)
  {}
  
  //: Put data.
  bool DPOPOVRayFileBodyC::Put(const TriMeshC &dat) {
    if(done || !outf)
      return false;

    // TO BE IMPLEMENTED
    std::cerr << "Error: not implemented yet!\n";
    RavlAssertMsg(0,"not implemented");
    return false;
  }
  
  //: Is port ready for data ?
  bool DPOPOVRayFileBodyC::IsPutReady() const 
  { return !done && outf; }

  // TexTriMesh POVRay File IO /////////////////////////////////////////////

  //: Open file.
  DPOTexPOVRayFileBodyC::DPOTexPOVRayFileBodyC(const StringC &fn)
    : outf(fn),
      done(false)
  {
    path = FilenameC(fn).PathComponent();
    if (!path.IsEmpty()) path += filenameSeperator ;
  }
  
  //: Put data.
  bool DPOTexPOVRayFileBodyC::Put(const TexTriMeshC &dat) 
  {
    if(done || !outf)
      return false;

    // TO BE IMPLEMENTED
    cerr << "Error: not implemented yet!\n";
    RavlAssertMsg(0,"not implemented");
    return false;
  }
  
  //: Is port ready for data ?
  bool DPOTexPOVRayFileBodyC::IsPutReady() const 
  { return !done && outf; }

}
