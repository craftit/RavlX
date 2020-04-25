// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_POVRAYFILEIO_HEADER
#define RAVL_POVRAYFILEIO_HEADER 1
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=Ravl3DIO
//! author="James Smith"
//! docentry="Ravl.API.3D.IO"
//! userlevel=Normal
//! file="Ravl/3D/MeshIO/POVRayFileIO.hh"

#include "Ravl/3D/TexTriMesh.hh"
#include "Ravl/3D/TriMesh.hh"
#include "Ravl/DP/Port.hh"
#include "Ravl/Stream.hh"

namespace Ravl3DN {
  
  //! userlevel=Develop
  //: Save TriMeshC to POVRay .pov files
  // POV-Ray is a free, high-quality, cross-platform raytracing program.
  // For more info, see <A HREF="http://www.povray.org">povray.org</A>

  class DPOPOVRayFileBodyC
    : public DPOPortBodyC<TriMeshC>
  {
  public:
    DPOPOVRayFileBodyC(const StringC &fn);
    //: Open file.
    
    DPOPOVRayFileBodyC(OStreamC &is);
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
  //: Save TriMeshC to POVRay .pov files
  // POV-Ray is a free, high-quality, cross-platform raytracing program.
  // For more info, see <A HREF="http://www.povray.org">povray.org</A>

  class DPOPOVRayFileC
    : public DPOPortC<TriMeshC>
  {
  public:
    DPOPOVRayFileC(const StringC &fn)
      : DPEntityC(*new DPOPOVRayFileBodyC(fn))
    {}
    //: Open file.
    
    DPOPOVRayFileC(OStreamC &is)
      : DPEntityC(*new DPOPOVRayFileBodyC(is))
    {}
    //: Open a stream.
    
  };
  
  //! userlevel=Develop
  //: Save TexTriMeshC to POVRay .pov files
  // POV-Ray is a free, high-quality, cross-platform raytracing program.
  // For more info, see <A HREF="http://www.povray.org">povray.org</A>
  
  class DPOTexPOVRayFileBodyC
    : public DPOPortBodyC<TexTriMeshC>
  {
  public:
    DPOTexPOVRayFileBodyC(const StringC &fn);
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
  //: Save TexTriMeshC to POVRay .pov files
  // POV-Ray is a free, high-quality, cross-platform raytracing program.
  // For more info, see <A HREF="http://www.povray.org">povray.org</A>

  class DPOTexPOVRayFileC
    : public DPOPortC<TexTriMeshC>
  {
  public:
    DPOTexPOVRayFileC(const StringC &fn)
      : DPEntityC(*new DPOTexPOVRayFileBodyC(fn))
    {}
    //: Open file.
    
  };

}


#endif
