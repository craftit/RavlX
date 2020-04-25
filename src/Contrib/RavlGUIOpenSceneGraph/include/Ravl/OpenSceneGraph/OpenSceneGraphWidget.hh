// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/OpenSceneGraphWidget.hh"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"
//! docentry = "Ravl.API.Graphics.OpenSceneGraph"
//! example = "exOpenSceneGraphWidget.cc"

#ifndef RAVLGUI_OPENSCENEGRAPHWIDGET_HEADER
#define RAVLGUI_OPENSCENEGRAPHWIDGET_HEADER 1

#include "Ravl/GUI/Widget.hh"
#include "Ravl/OpenSceneGraph/Node.hh"
#include <osgGtk/ViewerGtk.h>
#include <osgGtk/GraphicsWindowGtk.h>
#include "Ravl/Image/RealRGBAValue.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Point3d.hh"
#include "Ravl/Collection.hh"

namespace RavlOSGN
{
  using RavlN::Point2dC;
  using RavlN::Point3dC;
  using RavlN::CollectionC;
  
  //! userlevel=Normal
  //: GTK widget containing an OpenSceneGraph viewer.

  class OpenSceneGraphWidgetBodyC
  : public RavlGUIN::WidgetBodyC
  {
  public:
    OpenSceneGraphWidgetBodyC(int width, int height);
    //: Ctor.
    //!param: width - Initial width of the widget.
    //!param: height - Initial height of the widget.

    OpenSceneGraphWidgetBodyC(const RavlN::XMLFactoryContextC &factory);
    //: Ctor.
    //!param: factory - Construct from a factory context.

    virtual ~OpenSceneGraphWidgetBodyC();
    //: Dtor.

    virtual bool Create();
    //: Create the widget

    virtual bool Create(GtkWidget *newWidget);
    //: Create widget in place
    
    bool SetScene(const NodeC::RefT &node);
    //: Set the scene graph root object.

    bool SetColour(const RavlImageN::RealRGBAValueC &colour);
    //: Set the viewer background colour.

    bool PickPosition(float row,float col,osgUtil::LineSegmentIntersector::Intersections &intersections);
    //: Get a list of intersections from a pick position.

    osgViewer::Viewer *Viewer()
    { return m_osgViewer.get(); }
    //: Get pointer to viewer.
    
  protected:
    bool CommonCreate(GtkWidget *newWidget = NULL);

    bool OnConfigure();
    //: Change the viewport on resize.

    bool OnDestroy();
    //: Stop running on destroy.

    int m_width, m_height;
    osg::ref_ptr<osgViewer::ViewerGtk> m_osgViewer;
    osg::ref_ptr<osgViewer::GraphicsWindowGtk> m_osgWindow;

    NodeC::RefT m_sceneNode;
    RavlImageN::RealRGBAValueC m_clearColour;
    std::string m_defaultManipulator;
    float m_frameRate;
  };

  //! userlevel=Normal
  //: OpenSceneGraph widget.
  
  class OpenSceneGraphWidgetC
  : public RavlGUIN::WidgetC
  {
  public:
    OpenSceneGraphWidgetC()
    {}
    //: Default ctor.
    // Creates an invalid handle.

    OpenSceneGraphWidgetC(const RavlN::XMLFactoryContextC &factory)
    : WidgetC(*new OpenSceneGraphWidgetBodyC(factory))
    {}
    //: Ctor.
    //!param: factory - Construct from a factory context.

    OpenSceneGraphWidgetC(int width, int height)
    : WidgetC(*new OpenSceneGraphWidgetBodyC(width, height))
    {}
    //: Ctor.
    //!param: width - Initial width of the widget.
    //!param: height - Initial height of the widget.

    OpenSceneGraphWidgetC(OpenSceneGraphWidgetBodyC &body)
    : WidgetC(body)
    {}
    //: Body ctor.

    bool SetScene(const NodeC::RefT &node)
    { return Body().SetScene(node); }
    //: Set the scene graph root object.

    bool SetColour(const RavlImageN::RealRGBAValueC &colour)
    { return Body().SetColour(colour); }
    //: Set the viewer background colour.

    bool PickPosition(float row,float col,osgUtil::LineSegmentIntersector::Intersections &intersections)
    { return Body().PickPosition(row,col,intersections); }
    //: Get a list of intersections from a pick position.

    osgViewer::Viewer *Viewer()
    { return Body().Viewer(); }
    //: Get pointer to viewer.

  protected:
    OpenSceneGraphWidgetBodyC &Body()
    { return static_cast<OpenSceneGraphWidgetBodyC &>(RavlGUIN::WidgetC::Body()); }

    const OpenSceneGraphWidgetBodyC &Body() const
    { return static_cast<const OpenSceneGraphWidgetBodyC &>(RavlGUIN::WidgetC::Body()); }
  };

}

#endif
