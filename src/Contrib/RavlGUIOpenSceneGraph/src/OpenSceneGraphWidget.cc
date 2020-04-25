// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! file = "Ravl/Contrib/OpenSceneGraph/OpenSceneGraphWidget.cc"
//! lib=RavlGUIOpenSceneGraph
//! author = "Warren Moore"

#include "Ravl/OpenSceneGraph/OpenSceneGraphWidget.hh"
#include "Ravl/OpenSceneGraph/CanvasManipulator.hh"

#include <gtk/gtkgl.h>
#include <gtk/gtkcontainer.h>
#include "Ravl/GUI/Manager.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/XMLFactoryRegister.hh"

#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/DriveManipulator>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlOSGN
{
  using namespace osg;

  void FuncGtkGlExtInit(int *argc, char **argv[])
  {
    gtk_gl_init(argc, argv);
  }

  bool RegisterGtkGlExtInit()
  {
    RavlGUIN::Manager.RegisterGtkGlInit(&FuncGtkGlExtInit);
    return true;
  }

  static bool g_gtkGlInitRegistered = RegisterGtkGlExtInit();

  OpenSceneGraphWidgetBodyC::OpenSceneGraphWidgetBodyC(int width, int height)
  : m_width(width),
    m_height(height),
    m_osgViewer(NULL),
    m_osgWindow(NULL),
    m_clearColour(0.5, 0.5, 0.5, 1.0),
    m_defaultManipulator("TrackerBall"),
    m_frameRate(60)
  {}

  OpenSceneGraphWidgetBodyC::OpenSceneGraphWidgetBodyC(const RavlN::XMLFactoryContextC &factory)
  : m_width(factory.AttributeInt("width",100)),
    m_height(factory.AttributeInt("height",100)),
    m_osgViewer(NULL),
    m_osgWindow(NULL),
    m_clearColour(0.5, 0.5, 0.5, 1.0),
    m_defaultManipulator(factory.AttributeString("manipulator","TrackerBall").data()),
    m_frameRate(factory.AttributeReal("frameRate",60))
  {
    factory.Attribute("clearColour",m_clearColour,m_clearColour);
    NodeC::RefT scene;
    if(factory.HasChild("Scene")) {
      rThrowBadConfigContextOnFailS(factory,UseComponent("Scene",scene),"Failed to load scene.");
      SetScene(*scene);
    }
  }
  
  OpenSceneGraphWidgetBodyC::~OpenSceneGraphWidgetBodyC()
  {
    if (m_osgViewer)
      m_osgViewer->stop();
  }

  bool OpenSceneGraphWidgetBodyC::Create()
  { return CommonCreate(); }

  bool OpenSceneGraphWidgetBodyC::Create(GtkWidget *newWidget)
  { return CommonCreate(newWidget); }

  bool OpenSceneGraphWidgetBodyC::CommonCreate(GtkWidget *newWidget)
  {
    if (!RavlN::UsingRavlMain())
    {
      std::cerr << "In order to use the OpenSceneGraphWidget the program must be started with RAVL_ENTRY_POINT See Ravl/EntryPnt.hh." << std::endl;
      return false;
    }

    m_osgViewer = new osgViewer::ViewerGtk();

#ifdef OSGGA_CAMERA_MANIPULATOR
    // Used in 2.9
    osgGA::CameraManipulator *manipulator = 0;
#else
    // Used in 2.8
    osgGA::MatrixManipulator *manipulator = 0;
#endif

    if(m_defaultManipulator == "TrackerBall") {
      manipulator = new osgGA::TrackballManipulator;
    } else if(m_defaultManipulator == "Terrain") {
      manipulator = new osgGA::TerrainManipulator;
    } else if(m_defaultManipulator == "Drive") {
      manipulator = new osgGA::DriveManipulator;
    } else if(m_defaultManipulator == "Flight") {
      manipulator = new osgGA::FlightManipulator;
    } else if(m_defaultManipulator == "KeySwitch") {
      manipulator = new osgGA::KeySwitchMatrixManipulator;
    } else if(m_defaultManipulator == "Canvas") {
      manipulator = new CanvasManipulatorC;
    } else {
      // Fall back to default.
      std::cerr << "Unknown manipulator:" << m_defaultManipulator << "\n";
      RavlAssertMsg(0,"Unknown manipulator specified.");
      manipulator = new osgGA::TrackballManipulator;
    }
    m_osgViewer->setCameraManipulator(manipulator);
    m_osgViewer->set_fps(m_frameRate);

    if (m_sceneNode.IsValid())
    {
      m_osgViewer->setSceneData(m_sceneNode->Node());
    }

    m_osgViewer->getCamera()->setClearColor(Vec4(m_clearColour.Red(), m_clearColour.Green(), m_clearColour.Blue(), m_clearColour.Alpha()));

    m_osgWindow = m_osgViewer->setup_viewer_in_gtk_window(m_width, m_height);
    
    if (newWidget == NULL)
    {
      widget = m_osgWindow->gtk_widget();
    }
    else
    {
      if (GTK_IS_CONTAINER(newWidget))
      {
        gtk_widget_show(m_osgWindow->gtk_widget());
        gtk_container_add(GTK_CONTAINER(newWidget), m_osgWindow->gtk_widget());

        widget = m_osgWindow->gtk_widget();
      }
      else
      {
        std::cerr << "OpenSceneGraphWidgetBodyC::CommonCreate Glade widget is not container." << std::endl;
      }
    }
    
    if (widget)
    {
      gtk_widget_set_can_focus(widget,true);
      
      ConnectSignals();
      ConnectR(Signal("configure_event"), *this, &OpenSceneGraphWidgetBodyC::OnConfigure);
      ConnectR(Signal("destroy"), *this, &OpenSceneGraphWidgetBodyC::OnDestroy);
    }

    m_osgViewer->run();
    
    return true;
  }
  
  bool OpenSceneGraphWidgetBodyC::SetScene(const NodeC::RefT &node)
  {
    m_sceneNode = node;

    if (!m_osgViewer)
      return false;

    m_osgViewer->setSceneData(m_sceneNode->Node());

    return true;
  }
  
  bool OpenSceneGraphWidgetBodyC::SetColour(const RavlImageN::RealRGBAValueC &colour)
  {
    m_clearColour = colour;

    if (!m_osgViewer)
      return false;

    m_osgViewer->getCamera()->setClearColor(Vec4(m_clearColour.Red(), m_clearColour.Green(), m_clearColour.Blue(), m_clearColour.Alpha()));

    return true;
  }

  //! Pick a position in the window
  bool OpenSceneGraphWidgetBodyC::PickPosition(float row,float col,osgUtil::LineSegmentIntersector::Intersections &intersections)
  {
    if(!m_osgViewer.valid())
      return false;
    // Convert coordinate systems.
    RavlN::Index2dC size = Size();
    float x = col;
    float y = size.Row().V() - row;

    osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, x, y);
    osgUtil::IntersectionVisitor iv(picker.get());
    m_osgViewer->getCamera()->accept(iv);
    if (!picker->containsIntersections())
      return false;
    intersections = picker->getIntersections();
    return true;
  }


  bool OpenSceneGraphWidgetBodyC::OnConfigure()
  {
    if (!widget || !m_osgViewer)
      return false;

    RavlN::Index2dC size = Size();
    m_osgViewer->getCamera()->setViewport(0, 0, size.Col().V(), size.Row().V());
    m_osgViewer->getCamera()->setProjectionMatrixAsPerspective(30.0f,
                                                               static_cast<double>(size.Col().V()) / static_cast<double>(size.Row().V()),
                                                               1.0f,
                                                               10000.0f);

    return true;
  }

  bool OpenSceneGraphWidgetBodyC::OnDestroy()
  {
    if (m_osgViewer)
      m_osgViewer->stop();

    return true;
  }

  static RavlN::XMLFactoryRegisterHandleC<OpenSceneGraphWidgetC> g_xmlFactoryRegisterOpenSceneGraphWidget("RavlOSGN::OpenSceneGraphWidgetC");

  static RavlGUIN::WidgetC ConvertOpenSceneGraphWidget2Widget(const OpenSceneGraphWidgetC &widget)
  {
    return RavlGUIN::WidgetC(widget);
  }

  DP_REGISTER_CONVERSION(ConvertOpenSceneGraphWidget2Widget, 1);

  void LinkOpenSceneGraphWidget()
  {}

}
