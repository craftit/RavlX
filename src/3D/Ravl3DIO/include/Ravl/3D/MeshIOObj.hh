// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MESHOBJIO_HEADER
#define RAVL_MESHOBJIO_HEADER 1
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! docentry="Ravl.API.3D.IO"
//! file="Ravl/3D/MeshIO/MeshIOObj.hh"

#include "Ravl/3D/TriMesh.hh"
#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Stream.hh"

namespace Ravl3DN {
  
  //! userlevel=Develop
  //: Tri file IO.
  
  class DPIMeshObjBodyC
    : public DPIPortBodyC<TriMeshC>
  {
  public:
    DPIMeshObjBodyC(const StringC &fn);
    //: Open file.
    
    DPIMeshObjBodyC(IStreamC &is);
    //: Open stream.
    
    virtual bool IsGetEOS() const;
    //: Is valid data ?
    
    virtual TriMeshC Get();
    //: Get next piece of data.
    
  protected:
    IStreamC inf;
    bool done;
  };
  
  //! userlevel=Advanced
  //: Tri file IO.

  class DPIMeshObjC
    : public DPIPortC<TriMeshC>
  {
  public:
    DPIMeshObjC(const StringC &fn)
      : DPEntityC(*new DPIMeshObjBodyC(fn))
    {}
    //: Open file.
    
    DPIMeshObjC(IStreamC &is)
      : DPEntityC(*new DPIMeshObjBodyC(is))
    {}
    //: Open a stream.
    
  };

  
  //! userlevel=Develop
  //: Tri file IO.
  
  class DPOMeshObjBodyC
    : public DPOPortBodyC<TriMeshC>
  {
  public:
    DPOMeshObjBodyC(const StringC &fn);
    //: Open file.
    
    DPOMeshObjBodyC(OStreamC &is);
    //: Open stream.
    
    virtual bool Put(const TriMeshC &dat);
    //: Put data.
    
    virtual bool IsPutReady() const;
    //: Is port ready for data ?
    
  protected:
    OStreamC outf;
    bool done;
  };
  
  //! userlevel=Advanced
  //: Tri file IO.

  class DPOMeshObjC
    : public DPOPortC<TriMeshC>
  {
  public:
    DPOMeshObjC(const StringC &fn)
      : DPEntityC(*new DPOMeshObjBodyC(fn))
    {}
    //: Open file.
    
    DPOMeshObjC(OStreamC &is)
      : DPEntityC(*new DPOMeshObjBodyC(is))
    {}
    //: Open a stream.
    
  };

  //! userlevel=Develop
  //: OBJ file IO.
  
  class DPOTexMeshObjBodyC
    : public DPOPortBodyC<TexTriMeshC>
  {
  public:
    DPOTexMeshObjBodyC(const StringC &fn);
    //: Open file.
    
    DPOTexMeshObjBodyC(OStreamC &is);
    //: Open stream.
    
    virtual bool Put(const TexTriMeshC &dat);
    //: Put data.
    
    virtual bool IsPutReady() const;
    //: Is port ready for data ?
    
  protected:
    OStreamC outf;
    bool done;
    StringC base;
    StringC path;
  };
  
  //! userlevel=Advanced
  //: OBJ file IO.

  class DPOTexMeshObjC
    : public DPOPortC<TexTriMeshC>
  {
  public:
    DPOTexMeshObjC(const StringC &fn)
      : DPEntityC(*new DPOTexMeshObjBodyC(fn))
    {}
    //: Open file.
    
    DPOTexMeshObjC(OStreamC &is)
      : DPEntityC(*new DPOTexMeshObjBodyC(is))
    {}
    //: Open a stream.
    
  };


}


#endif
