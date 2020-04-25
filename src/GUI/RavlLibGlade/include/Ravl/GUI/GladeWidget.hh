// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_GLADEWIDGET_HEADER
#define RAVLGUI_GLADEWIDGET_HEADER 1
//! docentry="Ravl.API.Graphics.GTK.LibGlade"
//! lib=RavlLibGlade
//! file="Ravl/GUI/LibGlade/GladeWidget.hh"

#include "Ravl/GUI/Widget.hh"
#include "Ravl/GUI/GladeXML.hh"
#include "Ravl/SmartPtr.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Glade widget
  
  class GladeWidgetBodyC
    : public WidgetBodyC
  {
  public:
    GladeWidgetBodyC(const GladeXMLC &gladeXml,
                     const StringC &widgetName,
                     bool customWidget = true,
                     const StringC prefix = "");
    //: Constructor
    // prefix - String to prefix all names used for AddObject(..)
    
    GladeWidgetBodyC(const StringC &widgetName,bool customWidget = true);
    //: Constructor
    // NOTE: If using this constructor a Glade XML object must be set with SetXML()
    
    GladeWidgetBodyC(const XMLFactoryContextC &factory);
    //: Construct from an XMLFactory.

    bool SetXML(const GladeXMLC &gladeXml);
    //: Set a Glade XML object to use
    // NOTE: Will be ignored if one has already been set
    
    virtual bool Create()
    { return CommonCreate(); }
    //: Create the widget.
    
    virtual bool Create(GtkWidget *newWidget)
    { return CommonCreate(newWidget); }
    //: Create with a widget supplied from elsewhere.
    
    bool AddObject(const StringC &name,const WidgetC &newWidget, bool optional = false);
    //: Add named widget.
    //!param: name - Name of the widget to search for in the Glade file
    //!param: newWidget - Widget object to connect to the named object
    //!param: optional - If false, warn if the widget is not found
    
    bool AddObject(const StringC &name, const RavlN::SmartPtrC<WidgetBodyC> &newWidget, bool optional = false);
    //: Add named widget.
    //!param: name - Name of the widget to search for in the Glade file
    //!param: newWidget - Widget object to connect to the named object
    //!param: optional - If false, warn if the widget is not found

    template<class DataT>
    bool AddObject(const StringC &name, const RavlN::SmartPtrC<DataT> &newWidget, bool optional = false)
    { return AddObject(name,SmartPtrC<WidgetBodyC>(static_cast<const WidgetBodyC *>(newWidget.BodyPtr())),optional); }
    //: Add named widget.
    //!param: name - Name of the widget to search for in the Glade file
    //!param: newWidget - Widget object to connect to the named object
    //!param: optional - If false, warn if the widget is not found

    typedef RavlN::SmartPtrC<GladeWidgetBodyC> RefT;
    //: Handle to glade widget
  protected:
    bool InterceptDeleteEvent();
    //: Intercept the "delete-event", and just hide the widget.
    // Useful on windows where you just want to make them dissapear.
    // Note: Must be called after widget has been initialised
    
    virtual void WidgetDestroy();
    //: Called when gtk widget is destroyed.
    
    virtual bool CommonCreate(GtkWidget *newWidget = NULL);
    //: Common object creation
    
    GladeXMLC xml;
    StringC name;
    HashC<StringC, Tuple2C<RavlN::SmartPtrC<WidgetBodyC>, bool> > children;
    bool customWidget;
    StringC m_widgetPrefix;
    bool m_abortOnMissingWidget;
  };
  
  //! userlevel=Normal
  //: Glade widget
  
  class GladeWidgetC
    : public WidgetC
  {
  public:
    GladeWidgetC()
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    GladeWidgetC(const GladeXMLC &gladeXml,const StringC &widgetName,bool customWidget = true, const StringC &prefix = "")
      : WidgetC(*new GladeWidgetBodyC(gladeXml,widgetName,customWidget, prefix))
    {}
    //: Constructor
    
    GladeWidgetC(const StringC &widgetName,bool customWidget = true)
      : WidgetC(*new GladeWidgetBodyC(widgetName,customWidget))
    {}
    //: Constructor
    // NOTE: If using this constructor a Glade XML object must be set with SetXML()

    GladeWidgetC(const XMLFactoryContextC &factory)
      : WidgetC(*new GladeWidgetBodyC(factory))
    {}
    //: Construct from an XMLFactory.

    GladeWidgetC(const GladeWidgetBodyC *body)
      : WidgetC(body)
    {}
    //: Body ptr constructor

  protected:
    GladeWidgetC(GladeWidgetBodyC &body)
      : WidgetC(body)
    {}
    //: Body constructor
    
    GladeWidgetBodyC &Body()
    { return static_cast<GladeWidgetBodyC &>(WidgetC::Body()); }
    //: Access widget body.
    
    const GladeWidgetBodyC &Body() const
    { return static_cast<const GladeWidgetBodyC &>(WidgetC::Body()); }
    //: Access widget body.
    
  public:
    bool AddObject(const StringC &name,const WidgetC &widget, bool optional = false)
    { return Body().AddObject(name,widget,optional); }
    //: Add named widget.
    //!param: name - Name of the widget to search for in the Glade file
    //!param: widget - Widget object to connect to the named object
    //!param: optional - If false, warn if the widget is not found
    
    template<class DataT>
    bool AddObject(const StringC &name, const RavlN::SmartPtrC<DataT> &newWidget, bool optional = false)
    { return Body().AddObject(name,SmartPtrC<WidgetBodyC>(static_cast<const WidgetBodyC *>(newWidget.BodyPtr())),optional); }
    //: Add named widget.
    //!param: name - Name of the widget to search for in the Glade file
    //!param: newWidget - Widget object to connect to the named object
    //!param: optional - If false, warn if the widget is not found
  };

}


#endif
