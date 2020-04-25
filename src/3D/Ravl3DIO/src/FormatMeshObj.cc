// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/FormatMeshObj.cc"

#include "Ravl/config.h"
#include "Ravl/3D/FormatMeshObj.hh"
#include "Ravl/3D/MeshIOObj.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/TypeName.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {
  
  void InitObjFormat() {
  }
    
  // Tri ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  
  FileFormatObjBodyC::FileFormatObjBodyC()
    : FileFormatBodyC("obj","3D Mesh.")
  {}
  
  //: Is stream in std stream format ?
  
  const type_info &
  FileFormatObjBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatObjBodyC::ProbeLoad(), IStreamC, ok \n");
    return typeid(TriMeshC);
  }
  
  const type_info & FileFormatObjBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatObjBodyC::ProbeLoad(), on '" << nfilename << "' Ext='" << Extension(nfilename) << "'\n");
    if(Extension(nfilename) != StringC("obj") && nfilename != "-")
      return typeid(void);
    return ProbeLoad(in,obj_type);
  }
  
  const type_info & FileFormatObjBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat ) const {
    ONDEBUG(cerr << "FileFormatObjBodyC::ProbeSave() on '" << filename << "' Ext='" << Extension(filename) << "'\n");
    if(forceFormat)
      return typeid(TriMeshC);
    if(Extension(filename) != StringC("obj") && filename != "-")
      return typeid(void);
    if (obj_type == typeid(TexTriMeshC)) 
      return typeid(TexTriMeshC);
    return typeid(TriMeshC);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatObjBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatObjBodyC::CreateInput(StringC). Called. type='" << TypeName(obj_type) << "'\n");
    if(obj_type == typeid(TriMeshC))
      return DPIMeshObjC(filename);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatObjBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const  {
    ONDEBUG(cerr << "FileFormatObjBodyC::CreateOutput(StringC). Called. type='" << TypeName(obj_type) << "'\n");
    if(obj_type == typeid(TriMeshC))
      return  DPOMeshObjC(filename);
    if(obj_type == typeid(TexTriMeshC))
      return  DPOTexMeshObjC(filename);
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatObjBodyC::DefaultType() const 
  { return typeid(TriMeshC); }
  
  
  //////////////////////////////////////////////////////////////////
  
  FileFormatObjC RegisterFileFormatObj;
  
}
