// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VRMLFILEIO_HEADER
#define RAVL_VRMLFILEIO_HEADER 1
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! author="Jonathan Starck"
//! docentry="Ravl.API.3D.IO"
//! userlevel=Normal
//! file="Ravl/3D/MeshIO/VRMLFileIO.hh"

#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Stream.hh"

namespace Ravl3DN {

  //! userlevel=Develop
  //: Load TriMeshC from VRML files
  class DPIVRMLFileBodyC : public DPIPortBodyC<TriMeshC>
  {
  public:
    DPIVRMLFileBodyC(const StringC &fn);
    //: Open file.

    DPIVRMLFileBodyC(IStreamC &is);
    //: Open stream.

    virtual bool IsGetEOS() const;
    //: Is valid data ?

    virtual TriMeshC Get();
    //: Get next piece of data.

  protected:
    IStreamC inf;
    StringC path;
    bool done;
  };

  //! userlevel=Advanced
  //: Load TriMeshC from VRML files
  class DPIVRMLFileC : public DPIPortC<TriMeshC>
  {
  public:
    DPIVRMLFileC(const StringC &fn)
      : DPEntityC(*new DPIVRMLFileBodyC(fn))
      {}
    //: Open file.

    DPIVRMLFileC(IStreamC &is)
      : DPEntityC(*new DPIVRMLFileBodyC(is))
      {}
    //: Open a stream.

  };

  //! userlevel=Develop
  //: Load TexTriMeshC from VRML files
  class DPITexVRMLFileBodyC : public DPIPortBodyC<TexTriMeshC>
  {
  public:
    DPITexVRMLFileBodyC(const StringC &fn);
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
  //: Load TexTriMeshC from VRML files

  class DPITexVRMLFileC : public DPIPortC<TexTriMeshC>
  {
  public:
    DPITexVRMLFileC(const StringC &fn)
      : DPEntityC(*new DPITexVRMLFileBodyC(fn))
      {}
    //: Open file.
  };


  //! userlevel=Develop
  //: Save TriMeshC to VRML files
  class DPOVRMLFileBodyC : public DPOPortBodyC<TriMeshC>
  {
  public:
    DPOVRMLFileBodyC(const StringC &fn);
    //: Open file.

    DPOVRMLFileBodyC(OStreamC &is);
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
  //: Save TriMeshC to VRML files
  class DPOVRMLFileC : public DPOPortC<TriMeshC>
  {
  public:
    DPOVRMLFileC(const StringC &fn)
      : DPEntityC(*new DPOVRMLFileBodyC(fn))
    {}
    //: Open file.

    DPOVRMLFileC(OStreamC &is)
      : DPEntityC(*new DPOVRMLFileBodyC(is))
    {}
    //: Open a stream.

  };

  //! userlevel=Develop
  //: Save TexTriMeshC to VRML files
  class DPOTexVRMLFileBodyC : public DPOPortBodyC<TexTriMeshC>
  {
  public:
    DPOTexVRMLFileBodyC(const StringC &fn);
    //: Open file.

    virtual bool Put(const TexTriMeshC &dat);
    //: Put data.

    virtual bool IsPutReady() const;
    //: Is port ready for data ?

  protected:
    OStreamC outf;
    bool done;
    StringC path;
    StringC fullname;
  };

  //! userlevel=Advanced
  //: Save TexTriMeshC to VRML files
  class DPOTexVRMLFileC : public DPOPortC<TexTriMeshC>
  {
  public:
    DPOTexVRMLFileC(const StringC &fn)
      : DPEntityC(*new DPOTexVRMLFileBodyC(fn))
      {}
    //: Open file.
  };

}


#endif
