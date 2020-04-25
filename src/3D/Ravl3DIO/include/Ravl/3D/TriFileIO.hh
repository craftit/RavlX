// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TRIFILEIO_HEADER
#define RAVL_TRIFILEIO_HEADER 1
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! docentry="Ravl.API.3D.IO;Ravl.API.3D.Mesh"
//! file="Ravl/3D/MeshIO/TriFileIO.hh"

#include "Ravl/3D/TriMesh.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Stream.hh"

namespace Ravl3DN {
  
  //! userlevel=Develop
  //: Load TriMeshC from .tri files
  
  class DPITriFileBodyC
    : public DPIPortBodyC<TriMeshC>
  {
  public:
    DPITriFileBodyC(const StringC &fn);
    //: Open file.
    
    DPITriFileBodyC(IStreamC &is);
    //: Open stream.
    
    virtual bool IsGetEOS() const;
    //: Is valid data ?
    
    virtual TriMeshC Get();
    //: Get next piece of data.
    
  protected:
    IStreamC inf;
    bool done;
  };
  
  //! userlevel=Normal
  //: Load TriMeshC from .tri files

  class DPITriFileC
    : public DPIPortC<TriMeshC>
  {
  public:
    DPITriFileC(const StringC &fn)
      : DPEntityC(*new DPITriFileBodyC(fn))
    {}
    //: Open file.
    
    DPITriFileC(IStreamC &is)
      : DPEntityC(*new DPITriFileBodyC(is))
    {}
    //: Open a stream.
    
  };

  
  //! userlevel=Develop
  //: Save TriMeshC to .tri files
  
  class DPOTriFileBodyC
    : public DPOPortBodyC<TriMeshC>
  {
  public:
    DPOTriFileBodyC(const StringC &fn);
    //: Open file.
    
    DPOTriFileBodyC(OStreamC &is);
    //: Open stream.
    
    virtual bool Put(const TriMeshC &dat);
    //: Put data.
    
    virtual bool IsPutReady() const;
    //: Is port ready for data ?
    
  protected:
    OStreamC outf;
    bool done;
  };
  
  //! userlevel=Normal
  //: Save TriMeshC to .tri files

  class DPOTriFileC
    : public DPOPortC<TriMeshC>
  {
  public:
    DPOTriFileC(const StringC &fn)
      : DPEntityC(*new DPOTriFileBodyC(fn))
    {}
    //: Open file.
    
    DPOTriFileC(OStreamC &is)
      : DPEntityC(*new DPOTriFileBodyC(is))
    {}
    //: Open a stream.
    
  };


}


#endif
