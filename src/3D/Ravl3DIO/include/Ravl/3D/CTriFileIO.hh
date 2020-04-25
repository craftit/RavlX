// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CTRIFILEIO_HEADER
#define RAVL_CTRIFILEIO_HEADER 1
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! author="Jonathan Starck"
//! docentry="Ravl.API.3D.IO"
//! userlevel=Normal
//! file="Ravl/3D/MeshIO/CTriFileIO.hh"

#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Stream.hh"

namespace Ravl3DN {
  
  //! userlevel=Develop
  //: Load TexTriMeshC from .ctri files
  
  class DPICTriFileBodyC
    : public DPIPortBodyC<TexTriMeshC>
  {
  public:
    DPICTriFileBodyC(const StringC &fn);
    //: Open file.
    
    virtual bool IsGetEOS() const;
    //: Is valid data ?
    
    virtual TexTriMeshC Get();
    //: Get next piece of data.
    
  protected:
    IStreamC inf;
    bool done;
    StringC path;
  };
  
  //! userlevel=Advanced
  //: Load TexTriMeshC from .ctri files

  class DPICTriFileC
    : public DPIPortC<TexTriMeshC>
  {
  public:
    DPICTriFileC(const StringC &fn)
      : DPEntityC(*new DPICTriFileBodyC(fn))
    {}
    //: Open file.
    
  };

  
  //! userlevel=Develop
  //: Save TexTriMeshC to .ctri files
  
  class DPOCTriFileBodyC
    : public DPOPortBodyC<TexTriMeshC>
  {
  public:
    DPOCTriFileBodyC(const StringC &fn);
    //: Open file.
    
    virtual bool Put(const TexTriMeshC &dat);
    //: Put data.
    
    virtual bool IsPutReady() const;
    //: Is port ready for data ?
    
  protected:
    OStreamC outf;
    bool done;
    StringC path;
  };
  
  //! userlevel=Advanced
  //: Save TexTriMeshC to .ctri files

  class DPOCTriFileC
    : public DPOPortC<TexTriMeshC>
  {
  public:
    DPOCTriFileC(const StringC &fn)
      : DPEntityC(*new DPOCTriFileBodyC(fn))
    {}
    //: Open file.
    
  };


}


#endif
