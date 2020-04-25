// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DOBJECT3D_HEADER
#define RAVLGUI_DOBJECT3D_HEADER 1
//////////////////////////////////////////////////////////
//! docentry="Ravl.API.Graphics.3D"
//! rcsid="$Id$"
//! file="Ravl/GUI/3D/DObject3D.hh"
//! lib=RavlGUI3D
//! author="Charles Galambos"
//! date="12/04/1999"

#include "Ravl/RefCounter.hh"
#include "Ravl/Trigger.hh"
#include "Ravl/Image/RealRGBValue.hh"
#include "Ravl/Vector3d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Point3d.hh"
#include "Ravl/DList.hh"
#ifdef VISUAL_CPP
#include <windows.h>
#endif
#include <GL/gl.h>
#include "Ravl/Assert.hh"


namespace RavlGUIN {
  using namespace RavlN;
  using namespace RavlImageN;

  class Canvas3DC;

  //! userlevel=Develop
  //: Body of a basic display object in a 3D world.
  class DObject3DBodyC : public RCBodyVC
  {
  public:
    DObject3DBodyC()
      : id(-1)
    {}
    //: Default constructor.

    virtual ~DObject3DBodyC();
    //: Destructor.
    // Make sure display list is free'd

    virtual bool GUIRender(Canvas3DC &c3d) const = 0;
    //: Render object. Make sure you called Canvas3DC::GUIBeginGL just before

    virtual Vector3dC GUICenter() const = 0;
    //  { return Vector3dC(0,0,0); }
    //: Get center of object.
    // defaults to 0,0,0

    virtual RealT GUIExtent() const = 0;
    //  { return 1; }
    //: Get extent of object.
    // defaults to 1

    bool GUIRenderDL(Canvas3DC &c3d);
    //: Render, checking for display lists.

    static inline
    void GLColour(const RealRGBValueC &colour)
      { glColor3d(colour.Red(), colour.Green(), colour.Blue()); }
    //: Set current colour.

    static inline
    void GLVertex(const Point3dC &v)
      { glVertex3d(v.X(), v.Y(), v.Z()); }
    //: Set a vertex.

    static inline
    void GLNormal(const Vector3dC &v)
      { glNormal3d(v.X(), v.Y(), v.Z()); }
    //: Set a vertex.

    static inline
    void GLTexCoord(const Point2dC &p)
      { glTexCoord2d(p.Col(), p.Row()); }
    //: Set a texture coordinate

    void EnableDisplayList()
      { id = -2; }
    //: Enable generation of a display list.

    IntT DisplayListID() const
      { return id; }
    //: Access display list ID.

    IntT &DisplayListID()
      { return id; }
    //: Access display list ID.

  protected:
    IntT id; // Display list id. -1=None. -2=To be generated...
  };

  //! userlevel=Normal
  //: Basic object in a 3D world.
  class DObject3DC : public RCHandleC<DObject3DBodyC>
  {
  public:
    DObject3DC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    DObject3DC(DObject3DBodyC &bod)
      : RCHandleC<DObject3DBodyC>(bod)
    {}
    //: Body constructor.

    // *********************** GUI thread interface ****************************
    bool GUIRender(Canvas3DC &c3d) const
      { return Body().GUIRender(c3d); }
    //: Render object. Make sure you called Canvas3DC::GUIBeginGL just before

    Vector3dC GUICenter() const
      { return Body().GUICenter(); }
    //: Get center of object.
    // defaults to 0,0,0

    RealT GUIExtent() const
      { return Body().GUIExtent(); }
    //: Get extent of object.
    // defaults to 1


    // *********************** General interface *******************************


    // *********************** Unchecked functions *****************************

    bool GUIRenderDL(Canvas3DC &c3d)
      { return Body().GUIRenderDL(c3d); }
    //: Render object, checking for display list.

    void EnableDisplayList()
      { Body().EnableDisplayList(); }
    //: Enable generation of a display list.

    IntT DisplayListID() const
      { return Body().DisplayListID(); }
    //: Access display list ID.

    IntT &DisplayListID()
      { return Body().DisplayListID(); }
    //: Access display list ID.

  protected:
    DObject3DBodyC &Body()
      { return RCHandleC<DObject3DBodyC>::Body(); }
    //: Access body.

    const DObject3DBodyC &Body() const
      { return RCHandleC<DObject3DBodyC>::Body(); }
    //: Access body.

  };

  std::ostream &operator<<(std::ostream &strm,const DObject3DC &);
  std::istream &operator>>(std::istream &strm,DObject3DC &);
  
  BinOStreamC &operator<<(BinOStreamC &strm,const DObject3DC &);
  //: Write an object set to a binary stream.
  // Not implemented
  
  BinIStreamC &operator>>(BinIStreamC &strm,DObject3DC &);
  //: Read an object set to a binary stream.
  // Not implemented
  
  ////////////////////////////////////
  //! userlevel=Develop
  //: Body for OpenGL code invocation class.
  class DOpenGLBodyC : public DObject3DBodyC
  {
  public:
    DOpenGLBodyC(const TriggerC &se,
                 const Vector3dC &ncenter = Vector3dC(0,0,0), RealT nextent = 1)
      : sigEvent(se),
	center(ncenter),
	extent(nextent)
    {}
    //: Constructor.

    virtual Vector3dC GUICenter() const
      { return center; }
    //: Get center of object.
    // defaults to 0,0,0

    virtual RealT GUIExtent() const
      { return extent; }
    //: Get extent of object.
    // defaults to 1

  protected:
    virtual bool GUIRender(Canvas3DC &c3d) const;
    //: Render object.

    mutable TriggerC sigEvent;
    Vector3dC center;
    RealT extent;
  };

  //! userlevel=Normal
  //: OpenGL code invocation class.
  class DOpenGLC : public DObject3DC
  {
  public:
    DOpenGLC()
    {}
    //: Default constructor.
    // NB. Creates an invalid handle.

    DOpenGLC(const TriggerC &se,
             const Vector3dC &ncenter = Vector3dC(0,0,0), RealT nextent = 1)
      : DObject3DC(*new DOpenGLBodyC(se, ncenter, nextent))
    {}
    //: Constructor.
  };

  //! userlevel=Normal

  std::ostream &operator<<(std::ostream &strm,const DOpenGLC &);
  //: Write an DOpenGLC to a stream.

  std::istream &operator>>(std::istream &strm,DOpenGLC &);
  //: Read an DOpenGLC from a stream.


  ////////////////////////////////////

  //! userlevel=Develop
  //: Body of an  object set in a 3D world.
  class DObjectSet3DBodyC : public DObject3DBodyC
  {
  public:
    DObjectSet3DBodyC();
    //: Default constructor.

    virtual bool GUIRender(Canvas3DC &c3d) const;
    //: Render object.

    virtual Vector3dC GUICenter() const;
    //: Get center of object.
    // defaults to 0,0,0

    virtual RealT GUIExtent() const;
    //: Get extent of object.
    // defaults to 1

    inline void GUIAdd(const DObject3DC &obj)
      { parts.InsLast(obj); gotExtent = gotCenter = false; }
    //: Add object into list.

  protected:
    DListC<DObject3DC> parts;
    mutable bool gotCenter;
    mutable Vector3dC center;
    mutable bool gotExtent;
    mutable RealT extent;
  };

  //! userlevel=Normal
  //: Set of basic objects in a 3D world.
  class DObjectSet3DC 
    : public DObject3DC
  {
  public:
    DObjectSet3DC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    DObjectSet3DC(bool withBod)
      : DObject3DC(*new DObjectSet3DBodyC())
    {}
    //:  Constructor.

    DObjectSet3DC(DObjectSet3DBodyC &bod)
      : DObject3DC(bod)
    {}
    //: Body Constructor.

    const DObjectSet3DC &operator+=(const DObject3DC &obj)
      { Body().GUIAdd(obj); return *this; }
    //: Add object into list. this can be done only from GUI thread

  protected:
    DObjectSet3DBodyC &Body()
    { return dynamic_cast<DObjectSet3DBodyC &>(DObject3DC::Body()); }

    const DObjectSet3DBodyC &Body() const
    { return dynamic_cast<const DObjectSet3DBodyC &>(DObject3DC::Body()); }
  };

  //! userlevel=Normal

  std::ostream &operator<<(std::ostream &strm,const DObjectSet3DC &);
  //: Write an object set to a stream.

  std::istream &operator>>(std::istream &strm,DObjectSet3DC &);
  //: Read an object set to a stream.
  
  
}
#endif

