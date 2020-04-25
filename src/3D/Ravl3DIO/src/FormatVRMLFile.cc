// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/FormatVRMLFile.cc"

#include "Ravl/config.h"
#include "Ravl/3D/FormatVRMLFile.hh"
#include "Ravl/3D/VRMLFileIO.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/TypeName.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {

  void InitVRMLFormat() 
  { }
  
  // VRML ///////////////////////////////////////////////////////////////
  
  //: Constructor.
  FileFormatVRMLBodyC::FileFormatVRMLBodyC()
    : FileFormatBodyC("wrl","VRML indexed face set.")
  {}
  
  //: Is stream in std stream format ?
  const type_info &
  FileFormatVRMLBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const {
    // NOT IMPLEMENTED YET!
    //return typeid(void);
    ONDEBUG(cerr << "FileFormatVRMLBodyC::ProbeLoad(), IStreamC, ok \n");
    if (obj_type == typeid(TriMeshC)) 
      return obj_type;
    return typeid(TexTriMeshC);
  }
  
  const type_info & FileFormatVRMLBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatVRMLBodyC::ProbeLoad(), on '" << nfilename << "' Ext='" << Extension(nfilename) << "'\n");
    if(Extension(nfilename) != StringC("wrl") && nfilename != "-")
      return typeid(void);
    return ProbeLoad(in,obj_type);
  }
  
  const type_info & FileFormatVRMLBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat ) const {
    ONDEBUG(cerr << "FileFormatVRMLBodyC::ProbeSave() on '" << filename << "' Ext='" << Extension(filename) << "'\n");
    if(forceFormat)
      return typeid(TriMeshC);
    if(Extension(filename) != StringC("wrl") && filename != "-")
      return typeid(void);
    if (obj_type == typeid(TriMeshC)) 
      return obj_type;
    return typeid(TexTriMeshC);
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported.
  DPIPortBaseC FileFormatVRMLBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const
  {
    if (obj_type == typeid(TriMeshC))
      return DPIVRMLFileC(filename);
    if (obj_type == typeid(TexTriMeshC)) 
      return DPITexVRMLFileC(filename);
    return DPIPortBaseC();
  }
    
  //: Create a output port for saving to file 'filename'.
  // Will create an Invalid port if not supported.
  DPOPortBaseC FileFormatVRMLBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  {
    if(obj_type == typeid(TriMeshC))
      return  DPOVRMLFileC(filename);
    if(obj_type == typeid(TexTriMeshC))
      return  DPOTexVRMLFileC(filename);
    return DPOPortBaseC();
  }

  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  DPIPortBaseC FileFormatVRMLBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const {
    if (obj_type == typeid(TriMeshC))
      return DPIVRMLFileC(in);
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  DPOPortBaseC FileFormatVRMLBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const  {
    if(obj_type == typeid(TriMeshC))
      return  DPOVRMLFileC(out);
    return DPOPortBaseC();
  }
  
  //: Get prefered IO type.
  const type_info &FileFormatVRMLBodyC::DefaultType() const 
  { return typeid(TexTriMeshC); }
  
  
  //////////////////////////////////////////////////////////////////
  
  //: Register file IO for VRML format.
  FileFormatVRMLC RegisterFileFormatVRML;
  
}
