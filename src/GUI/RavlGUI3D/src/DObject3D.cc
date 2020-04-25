// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI3D
//! file="Ravl/GUI/3D/DObject3D.cc"

#include "Ravl/GUI/DObject3D.hh"
#include "Ravl/DLIter.hh"

namespace RavlGUIN {

  //: Destructor.
  // Make sure display list is free'd
  DObject3DBodyC::~DObject3DBodyC() {
    // Well we should.............
  }

  //: Render object.
  //bool DObject3DBodyC::GUIRender(Canvas3DC &c3d) {
  //  std::cerr << "DObject3DBodyC::GUIRender  Error!!! Need to overload this.\n";
  //  RavlAssert(0); // Abstract method.
  //  return true;
  //}

  //: Render, checking for display lists.
  bool DObject3DBodyC::GUIRenderDL(Canvas3DC &c3d) {
    bool ret = true;
    //cerr << "Calling Genlist. " << id << "\n";
    if(id >= 0) {
      //cerr << "Calling Genlist. " << id << "\n";
      glCallList(id);
    }
    else if(id == -2)
    { // Generate display list.
      id = glGenLists(1);
      //cerr << "New Genlist. " << id << "\n";
      glNewList(id, GL_COMPILE);
      ret = GUIRender(c3d);
      glEndList();
      glCallList(id);
    }
    else  // Don't use a display list...
      ret = GUIRender(c3d);
    return ret;
  }

  //////////////////////////////////////////////////

  std::ostream &operator<<(std::ostream &strm,const DObject3DC &) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }

  std::istream &operator>>(std::istream &strm,DObject3DC &) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  
  BinOStreamC &operator<<(BinOStreamC &strm,const DObject3DC &) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Write an object set to a binary stream.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,DObject3DC &) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }
  //: Read an object set to a binary stream.
  // Not implemented
  

  /// DObjectSet3DBodyC ///////////////////////////////////////////////
  //: Default constructor.
  DObjectSet3DBodyC::DObjectSet3DBodyC()
    : gotCenter(false),
      center(0,0,0),
      gotExtent(false),
      extent(1)
  {}

  //: Render object.
  bool DObjectSet3DBodyC::GUIRender(Canvas3DC &c3d) const
  {
    //cerr << "DObjectSet3DBodyC::GUIRender\n";
    for(DLIterC<DObject3DC> it(parts);it.IsElm();it.Next())
      it.Data().GUIRender(c3d);
    return true;
  }

  //: Get center of object.
  // defaults to 0,0,0
  Vector3dC DObjectSet3DBodyC::GUICenter() const
  {
    if(!gotCenter)
    {
      center = Vector3dC(0, 0, 0);
      if(parts.Size() != 0)
      {
        RealT count = 0;
        for(DLIterC<DObject3DC> it(parts); it; it++, count++)
          center += it.Data().GUICenter();
        center /= count;
      }
      gotCenter = true;
    }
    return center;
  }

  //: Get extent of object.
  // defaults to 1
  RealT DObjectSet3DBodyC::GUIExtent() const
  {
    if(!gotExtent)
    {
      extent = 1;
      if(parts.Size() != 0)
      {
        Vector3dC at = GUICenter();
        RealT maxDist = 0;
        for(DLIterC<DObject3DC> it(parts); it; it++)
        {
          RealT dist = at.EuclidDistance(it.Data().GUICenter()) +
                                         it.Data().GUIExtent();
          if(dist > maxDist)
            maxDist = dist;
        }
        extent = maxDist;
      }
      gotExtent = true;
    }
    return extent;
  }

  std::ostream &operator<<(std::ostream &strm,const DObjectSet3DC &) {
    RavlAssert(0);
    return strm;
  }

  std::istream &operator>>(std::istream &strm,DObjectSet3DC &) {
    RavlAssert(0);
    return strm;
  }

  //// DOpenGLBodyC ////////////////////////////////////////////////////////
  //: Render object.
  bool DOpenGLBodyC::GUIRender(Canvas3DC &c3d) const
  {
    //cerr << "DOpenGLBodyC::GUIRender\n";
    if(sigEvent.IsValid())
      sigEvent.Invoke();
    return true;
  }

  std::ostream &operator<<(std::ostream &strm,const DOpenGLC &) {
    RavlAssert(0);
    return strm;
  }

  std::istream &operator>>(std::istream &strm,DOpenGLC &) {
    RavlAssert(0);
    return strm;
  }
}

