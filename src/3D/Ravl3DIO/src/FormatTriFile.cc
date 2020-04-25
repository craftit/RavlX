// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/FormatTriFile.cc"

#include "Ravl/config.h"
#include "Ravl/3D/FormatTriFile.hh"
#include "Ravl/3D/TriFileIO.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/TypeName.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {

  void InitTriFormat() {
  }
    
  // Tri ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  
  FileFormatTriBodyC::FileFormatTriBodyC()
    : FileFormatBodyC("tri","3D Mesh.")
  {}
  
  //: Is stream in std stream format ?
  
  const type_info &
  FileFormatTriBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatTriBodyC::ProbeLoad(), IStreamC, ok \n");
    return typeid(TriMeshC);
  }
  
  const type_info & FileFormatTriBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatTriBodyC::ProbeLoad(), on '" << nfilename << "' Ext='" << Extension(nfilename) << "'\n");
    if(Extension(nfilename) != StringC("tri") && nfilename != "-")
      return typeid(void);
    return ProbeLoad(in,obj_type);
  }
  
  const type_info & FileFormatTriBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat ) const {
    ONDEBUG(cerr << "FileFormatTriBodyC::ProbeSave() on '" << filename << "' Ext='" << Extension(filename) << "'\n");
    if(forceFormat)
      return typeid(TriMeshC);
    if(Extension(filename) != StringC("tri") && filename != "-")
      return typeid(void);
    return typeid(TriMeshC);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatTriBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const {
    if(obj_type == typeid(TriMeshC))
      return DPITriFileC(in);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatTriBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const  {
    if(obj_type == typeid(TriMeshC))
      return  DPOTriFileC(out);
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatTriBodyC::DefaultType() const 
  { return typeid(TriMeshC); }
  
  
  //////////////////////////////////////////////////////////////////
  
  FileFormatTriC RegisterFileFormatTri;
  
}
