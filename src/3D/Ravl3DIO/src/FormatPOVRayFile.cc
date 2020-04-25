// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/config.h"
#include "Ravl/3D/FormatPOVRayFile.hh"
#include "Ravl/3D/POVRayFileIO.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/TypeName.hh"
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/FormatPOVRayFile.cc"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {

  void InitPOVRayFormat() 
  { }
    
  // POVRay ///////////////////////////////////////////////////////////////
  
  //: Constructor.
  FileFormatPOVRayBodyC::FileFormatPOVRayBodyC()
    : FileFormatBodyC("pov","POVRay mesh.")
  {}
  
  const type_info & FileFormatPOVRayBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat ) const {
    // NOT IMPLEMENTED YET!
    return typeid(void);
    ONDEBUG(cerr << "FileFormatPOVRayBodyC::ProbeSave() on '" << filename << "' Ext='" << Extension(filename) << "'\n");
    if(forceFormat)
      return typeid(TriMeshC);
    if(Extension(filename) != StringC("pov") && filename != "-")
      return typeid(void);
    if (obj_type == typeid(TexTriMeshC)) 
      return typeid(TexTriMeshC);
    return typeid(TriMeshC);
  }
  
  //: Create a output port for saving to file 'filename'.
  // Will create an Invalid port if not supported.
  DPOPortBaseC FileFormatPOVRayBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  {
    if(obj_type == typeid(TriMeshC))
      return  DPOPOVRayFileC(filename);
    if(obj_type == typeid(TexTriMeshC))
      return  DPOTexPOVRayFileC(filename);
    return DPOPortBaseC();
  }

  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  DPOPortBaseC FileFormatPOVRayBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const  {
    if(obj_type == typeid(TriMeshC))
      return  DPOPOVRayFileC(out);
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  const type_info &FileFormatPOVRayBodyC::DefaultType() const 
  { return typeid(TriMeshC); }
  
  
  //////////////////////////////////////////////////////////////////
  
  //: Register file IO for POVRay format.
  FileFormatPOVRayC RegisterFileFormatPOVRay;
  
  //: Register file IO for POVRay format with texture.
  FileFormatPOVRayC RegisterFileFormatTexPOVRay;
}
