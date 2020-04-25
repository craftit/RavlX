// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/config.h"
#include "Ravl/3D/FormatCTriFile.hh"
#include "Ravl/3D/CTriFileIO.hh"
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/TypeName.hh"
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! file="Ravl/3D/MeshIO/FormatCTriFile.cc"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace Ravl3DN {

  void InitCTriFormat() 
  { }
    
  // CTri ///////////////////////////////////////////////////////////////
  
  //: Constructor.
  
  FileFormatCTriBodyC::FileFormatCTriBodyC()
    : FileFormatBodyC("ctri","3D Mesh with texture.")
  {}
  
  //: Is stream in std stream format ?
  
  const type_info &
  FileFormatCTriBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatCTriBodyC::ProbeLoad(), IStreamC, ok \n");
    return typeid(TexTriMeshC);
  }
  
  const type_info & FileFormatCTriBodyC::ProbeLoad(const StringC &nfilename,IStreamC &in,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatCTriBodyC::ProbeLoad(), on '" << nfilename << "' Ext='" << Extension(nfilename) << "'\n");
    if(Extension(nfilename) != StringC("ctri") && nfilename != "-")
      return typeid(void);
    return ProbeLoad(in,obj_type);
  }
  
  const type_info & FileFormatCTriBodyC::ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat ) const {
    ONDEBUG(cerr << "FileFormatCTriBodyC::ProbeSave() on '" << filename << "' Ext='" << Extension(filename) << "'\n");
    if(forceFormat)
      return typeid(TexTriMeshC);
    if(Extension(filename) != StringC("ctri") && filename != "-")
      return typeid(void);
    return typeid(TexTriMeshC);
  }
  
  //: Create a input port for loading from file 'filename'.
  DPIPortBaseC FileFormatCTriBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const
  {
    if(obj_type == typeid(TexTriMeshC)) return DPICTriFileC(filename);
    return DPIPortBaseC();
  }
    
  //: Create a output port for saving to file 'filename'..
  DPOPortBaseC FileFormatCTriBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  {
    if(obj_type == typeid(TexTriMeshC)) return DPOCTriFileC(filename);
    return DPOPortBaseC();
  }
        
  //: Create a input port for loading.
  // This is not supported and will create an invalid port.
  DPIPortBaseC FileFormatCTriBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const
  { return DPIPortBaseC(); }
    
  //: Create a output port for saving.
  // This is not supported and will create an invalid port.
  DPOPortBaseC FileFormatCTriBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const
  { return DPOPortBaseC(); }
    
  
  //: Get prefered IO type.
  const type_info &FileFormatCTriBodyC::DefaultType() const 
  { return typeid(TexTriMeshC); }
  
  
  //////////////////////////////////////////////////////////////////

  //: Register file IO for CTRI format 
  FileFormatCTriC RegisterFileFormatCTri;
  
}
