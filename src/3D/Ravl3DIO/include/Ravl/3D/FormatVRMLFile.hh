// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VRMLFORMAT_HEADER
#define RAVL_VRMLFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! author="Jonathan Starck"
//! docentry="Ravl.API.3D.IO.Formats"
//! userlevel=Normal
//! file="Ravl/3D/MeshIO/FormatVRMLFile.hh"

#include "Ravl/DP/FileFormat.hh"
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/3D/TriMesh.hh"

namespace Ravl3DN {
  
  //! userlevel=Develop
  //: VRML file format information.
  
  class FileFormatVRMLBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatVRMLBodyC();
    //: Default constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.

    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'.
    // Will create an Invalid port if not supported.
        
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
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
  
  //: VRML file format for TriMeshC  
  class FileFormatVRMLC 
    : public FileFormatC<TriMeshC> {
  public:
    FileFormatVRMLC()
      : FileFormatC<TriMeshC>(*new FileFormatVRMLBodyC())
      {}
  };
  
}
#endif
