// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POVRAYFORMAT_HEADER
#define RAVL_POVRAYFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! author="James Smith"
//! docentry="Ravl.API.3D.IO.Formats"
//! userlevel=Normal
//! file="Ravl/3D/MeshIO/FormatPOVRayFile.hh"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/3D/TriMesh.hh"

namespace Ravl3DN {
  
  //! userlevel=Develop
  //: POVRay file format information.
  
  class FileFormatPOVRayBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatPOVRayBodyC();
    //: Default constructor.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'.
    // Will create an Invalid port if not supported.
        
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // Will create an Invalid port if not supported.
    
    virtual const type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return false; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  };
  
  /////////////////////////////
  //! userlevel=Advanced

  //: POVRay file format for TriMeshC  
  class FileFormatPOVRayC 
    : public FileFormatC<TriMeshC> {
  public:
    FileFormatPOVRayC()
      : FileFormatC<TriMeshC>(*new FileFormatPOVRayBodyC())
    {}
  };
  
  //: POVRay file format for TexTriMeshC  
  class FileFormatTexPOVRayC 
    : public FileFormatC<TexTriMeshC> {
  public:
    FileFormatTexPOVRayC()
      : FileFormatC<TexTriMeshC>(*new FileFormatPOVRayBodyC())
    {}
  };
  
  
}
#endif
