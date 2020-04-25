// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlLibGlade
//! file="Ravl/GUI/LibGlade/GladeWidget.cc"

#include "Ravl/GUI/GladeWidget.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/DP/Converter.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {
  
  using RavlN::SmartPtrC;
  
  //: Constructor
  
  GladeWidgetBodyC::GladeWidgetBodyC(const GladeXMLC &gladeXml,
                                     const StringC &widgetName,
                                     bool aCustomWidget,
                                     const StringC prefix) 
    : xml(gladeXml),
      name(widgetName),
      customWidget(aCustomWidget),
      m_widgetPrefix(prefix),
      m_abortOnMissingWidget(false)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GladeWidgetBodyC::GladeWidgetBodyC(name=" << widgetName << ", customWidget=" << aCustomWidget << ", prefix = " << prefix << ")" << endl);
    if(!m_widgetPrefix.IsEmpty() && m_widgetPrefix.lastchar() != '.')
      m_widgetPrefix += ".";
    
    if(customWidget)
      xml = GladeXMLC(gladeXml.Filename(),name,gladeXml.ModuleName());
  }
  
  //: Constructor
  
  GladeWidgetBodyC::GladeWidgetBodyC(const StringC &widgetName,bool aCustomWidget) 
    : name(widgetName),
      customWidget(aCustomWidget),
      m_abortOnMissingWidget(false)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GladeWidgetBodyC::GladeWidgetBodyC(name=" << widgetName << ", customWidget=" << aCustomWidget << ")");
  }
  
  GladeWidgetBodyC::GladeWidgetBodyC(const XMLFactoryContextC &factory)
   : WidgetBodyC(factory),
     name(factory.AttributeString("widgetName",factory.Name())),
     customWidget(factory.AttributeBool("customWidget",false)),
     m_widgetPrefix(factory.AttributeString("widgetPrefix","")),
     m_abortOnMissingWidget(factory.AttributeBool("abortOnMissingWidget",true))
  {
    if(!factory.UseComponent("GladeXML",xml,false,typeid(GladeXMLC))) {
      RavlSysLog(SYSLOG_ERR) << "Failed to find glade xml file.  ";
    }
    if(!m_widgetPrefix.IsEmpty() && m_widgetPrefix.lastchar() != '.')
      m_widgetPrefix += ".";
    if(customWidget) {
      xml = GladeXMLC(xml.Filename(),name,xml.ModuleName());
    }
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GladeWidgetBodyC::GladeWidgetBodyC(name=" << name << ", customWidget=" << customWidget << ", prefix = " << m_widgetPrefix << ")");
    for(RavlN::DLIterC<RavlN::XMLTreeC> it(factory.Children());it;it++) {
      if(it->Name() == "GladeXML" || it->AttributeString("typename","").IsEmpty())
        continue;
      XMLFactoryContextC childContext;
      if(!factory.ChildContext(it->Name(),childContext)) {
        // This just shouldn't happen.
        RavlAssert(0);
      }
      // Need to read the attributes before we construct the objects,
      // so the post construction check for used attributes is aware of them.
      StringC widgetName = childContext.AttributeString("widgetName",it->Name());
      if(widgetName.IsEmpty())
        continue;
      bool optional = childContext.AttributeBool("optional",false);
      
      WidgetC widget;
      if(!factory.UseComponent(it->Name(),widget,true)) {
        RavlSysLog(SYSLOG_WARNING) << "Failed to load component " << it->Name() << " from " << factory.Path() << " ";
        continue;
      }
      AddObject(widgetName,widget,optional);
    }
  }
  
  bool GladeWidgetBodyC::SetXML(const GladeXMLC &gladeXml)
  {
    if (xml.IsValid())
    {
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GladeWidgetBodyC::SetXML already set");
      return true;
    }
    
    if(customWidget)
      xml = GladeXMLC(gladeXml.Filename(),name,gladeXml.ModuleName());
    else
      xml = gladeXml;
    
    return xml.IsValid();
  }


  
  bool GladeWidgetBodyC::CommonCreate(GtkWidget *newWidget) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GladeWidgetBodyC::CommonCreate(GtkWidget *), Called. Name=" << name << " ");

    if (!xml.IsValid())
    {
      RavlSysLog(SYSLOG_ERR) << "GladeWidgetBodyC::CommonCreate called with no XML set";
      return false;
    }

    if(widget != NULL)
      return true;
    
    if (newWidget == NULL)
      widget = xml.Widget(name);
    else
    {
      widget = newWidget;

      if (customWidget)
      {
        if (!GTK_IS_CONTAINER(widget))
        {
          RavlSysLog(SYSLOG_ERR) << "ERROR: Custom widget for " << name << " isn't a container. (Try putting an event box here.) \n";
          RavlAssert(0);
          return false;
        }
        
        ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Adding custom child widget '" << name << "' into parent. ");
        GtkWidget *childWidget = xml.Widget(name);
        if (childWidget == 0)
        {
          RavlSysLog(SYSLOG_ERR) << "ERROR: No child widget found for '" << name << "' ";
          if(m_abortOnMissingWidget)
            throw RavlN::ExceptionBadConfigC("Failed to find widget ");
          return false;
        }
        
        gtk_widget_show(childWidget);
        gtk_container_add(GTK_CONTAINER(widget), childWidget);
      }
    }
    
    if (widget == NULL)
    {
      RavlSysLog(SYSLOG_ERR) << "ERROR: Failed to find widget '" << name << "' in '" << xml.Filename() << "' ";
      RavlAssertMsg(false,"Failed to find widget.");
      return false;
    }
    
    for (HashIterC<StringC, Tuple2C<SmartPtrC<WidgetBodyC>, bool> > it(children); it; it++)
    {
      GtkWidget *childWidget = xml.Widget(it.Key());
      if (childWidget == NULL)
      {
        if (!it.Data().Data2()) {
          RavlSysLog(SYSLOG_WARNING) << "WARNING: Can't find widget for '" << it.Key() << "' ";
          if(m_abortOnMissingWidget)
            throw RavlN::ExceptionBadConfigC("Failed to find widget ");
        }
        continue;
      }
      if (it->Data1().IsValid())
        it->Data1()->Create(childWidget);
      else {
        RavlSysLog(SYSLOG_WARNING) << "WARNING: Invalid handle for widget '" << it.Key() << "' ";
        if(m_abortOnMissingWidget)
          throw RavlN::ExceptionBadConfigC("Failed to find widget ");
      }
    }
    
    ConnectSignals();    
    
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "GladeWidgetBodyC::CommonCreate(GtkWidget *), Done. Name=" << name << " ");
    return true;
  }

  //: Add named widget.
  
  bool GladeWidgetBodyC::AddObject(const StringC &name,const WidgetC &newWidget, bool optional) {
    StringC useName = name;
    if(!m_widgetPrefix.IsEmpty())
      useName = m_widgetPrefix + name;
    children[useName] = Tuple2C<SmartPtrC<WidgetBodyC>, bool>(newWidget, optional);
    return true;
  }


  
  bool GladeWidgetBodyC::AddObject(const StringC &name, const SmartPtrC<WidgetBodyC> &newWidget, bool optional)
  {
    StringC useName = name;
    if(!m_widgetPrefix.IsEmpty())
      useName = m_widgetPrefix + name;
    children[name] = Tuple2C<SmartPtrC<WidgetBodyC>, bool>(newWidget, optional);
    return true;
  }


  
  extern "C" {
    static int StopDeleteEvent(GtkWidget *widget,GdkEvent *event,GladeWidgetBodyC *bod) {
      bod->GUIHide(); // Hide the window.
      return 1; // Return true so window isn't really destroyed
    }
  }
  
  //: Intercept the "delete-event", and just hide the widget.
  // Useful on windows where you just want to make them dissapear.
  
  bool GladeWidgetBodyC::InterceptDeleteEvent() {
    RavlAssert(widget != 0);
    gtk_signal_connect (GTK_OBJECT (widget),
                        "delete_event",         
                        GTK_SIGNAL_FUNC (StopDeleteEvent),
                        this);
    return true;
  }
  
  //: Called when gtk widget is destroyed.
  
  void GladeWidgetBodyC::WidgetDestroy() {
    // Just call back to parent.
    WidgetBodyC::WidgetDestroy();
  }

  static XMLFactoryRegisterHandleConvertC<GladeWidgetC,WidgetC> g_registerXMLFactoryGladeWidget("RavlGUIN::GladeWidgetC");

  static GladeWidgetC ConvertGladeWidgetBodyPtrToHandle(const GladeWidgetBodyC::RefT &ref)
  { return GladeWidgetC(ref.BodyPtr()); }

  DP_REGISTER_CONVERSION(ConvertGladeWidgetBodyPtrToHandle,1.0);

  void LinkGladeWidget()
  {}

}
