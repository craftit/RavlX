// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/Button.cc"


#include "Ravl/GUI/Button.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/GUI/WidgetStyle.hh"
#include "Ravl/Stream.hh"
#include "Ravl/GUI/Pixbuf.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include <gtk/gtk.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlGUIN
{
  
  
  
  const static IntT g_pixmapBorder = 1;
  const static IntT g_pixmapPadding = 0;
  
  
  
  static void find_label_iter(GtkWidget *widget, gpointer data)
  {
    // Stop early if we already have our label
    GtkWidget **label = (GtkWidget**)data;
    if (*label != NULL)
      return;
    
    // Keep checking containers
    if (GTK_IS_CONTAINER(widget))
    {
      gtk_container_foreach(GTK_CONTAINER(widget), &find_label_iter, label);
      return;
    }
    
    // Do we have a label
    if (GTK_IS_LABEL(widget))
      *label = widget;
  }
  
  
  
  static void find_pixmap_iter(GtkWidget *widget, gpointer data)
  {
    // Stop early if we already have our label
    GtkWidget **pixmap = (GtkWidget**)data;
    if (*pixmap != NULL)
      return;
    
    // Do we have a pixmap
    if (GTK_IS_PIXMAP(widget))
      *pixmap = widget;
  }
  
  // Default constructor.
  
  ButtonBodyC::ButtonBodyC(const char *nlabel,const char *ntooltip)
    : state(false),
      label(nlabel)
    
  { SetToolTip(ntooltip); }
  
  //: Constructor.
  
  ButtonBodyC::ButtonBodyC(const char *nlabel,const PixmapC &pixm,const char *tooltip)
    : state(false),
      pix(pixm),
      label(nlabel)
  { SetToolTip(tooltip); }

  //: XMLFactory constructor.

  static bool InvokeTrigger(TriggerC &trig) {
    trig.Invoke();
    return true;
  }

  ButtonBodyC::ButtonBodyC(const XMLFactoryContextC &factory)
   : WidgetBodyC(factory),
     label(factory.AttributeString("name","<anon>"))
  {
    Signal0C sigClicked;
    if(factory.UseComponent("SigClicked",sigClicked,true,typeid(Signal0C)))
      Connect(Signal("clicked"),sigClicked,&Signal0C::Invoke);

    XMLFactoryContextC childContext;
    if(factory.ChildContext("Triggers",childContext)) {
      for(RavlN::DLIterC<RavlN::XMLTreeC> it(childContext.Children());it;it++) {
        TriggerC trigger;
        if(!childContext.UseComponent(it->Name(),trigger)) {
          continue;
        }
        Connect(Signal("clicked"),&InvokeTrigger,trigger);
      }
    }
  }

  GtkWidget *ButtonBodyC::BuildWidget(const char *aLab) {
    if (aLab == NULL)
      return gtk_button_new();
    return gtk_button_new_with_label(aLab);
  }

  bool ButtonBodyC::GUISetStyle(WidgetStyleC& style) {    
    // Set widget style
    WidgetBodyC::GUISetStyle(style);    
    // Set style of child label
    if (widget != 0) {
      GtkWidget *child = GTK_BIN(widget)->child;
      if(child != 0) {
        if(GTK_IS_LABEL(child)) {
          gtk_widget_set_style(GTK_WIDGET(child),style.Style());
        } else {
          GtkWidget *label = NULL;
          gtk_container_foreach(GTK_CONTAINER(child), &find_label_iter, &label);
          if(label != 0)
            gtk_widget_set_style(label,style.Style());
        }
      }
    }
    return true;
  }

  //: Set toggle label.
  // GUI thread only.
  
    bool ButtonBodyC::GUISetLabel(const StringC &text) {
    label = text;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    
    // Check the button child
    GtkWidget *child = GTK_BIN(widget)->child;
    if (child != 0)
    {
      // Is it the label?
      GtkWidget *label = NULL;
      if (GTK_IS_LABEL(child))
      {
        label = child;
      }
      else
      {
        // Find the first label in the nest of children
        if (GTK_IS_CONTAINER(child))
        {
          gtk_container_foreach(GTK_CONTAINER(child), &find_label_iter, &label);
        }
      }

      // If a label was found, reset the text and leave
      if (label != NULL)
      {
        gtk_label_set(GTK_LABEL(label), text.chars());
        return true;
      }
    }
    
    return false;
  }
  
  //: Set toggle label.
  
  void ButtonBodyC::SetLabel(const StringC &text) {
    Manager.Queue(Trigger(ButtonC(*this),&ButtonC::GUISetLabel,text));
  }



  bool ButtonBodyC::GUISetImage(const PixmapC &pixmap)
  {
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    
    ONDEBUG(std::cerr << "ButtonBodyC::GUISetImage()\n");
    PixmapC tempPixmap = pixmap;
    GtkWidget *tempPixmapWidget = NULL;
    if (tempPixmap.IsValid())
    {
      // Create the bitmap
      tempPixmap.Create(); // This mishapes the window if it hasn't already been shown
      tempPixmapWidget = tempPixmap.Widget();
      gtk_widget_show(tempPixmapWidget);
    }
    else
      return false;
    
    if (tempPixmapWidget == NULL)
      return false;
    
    // Check the button child
    GtkWidget *child = GTK_BIN(widget)->child;
    if (child != 0)
    {
      // Is it the label?
      if (GTK_IS_PIXMAP(child) || GTK_IS_IMAGE(child))
      {
        ONDEBUG(std::cerr << "ButtonBodyC::GUISetImage() replacing label for pixmap\n");
        
        // Remove the old, add the new and store the pixmap
        gtk_container_remove(GTK_CONTAINER(widget), child);
        gtk_container_add(GTK_CONTAINER(widget), tempPixmapWidget);
        pix = tempPixmap;
      }
      else
      {
        if (GTK_IS_LABEL(child))
        {
          ONDEBUG(std::cerr << "ButtonBodyC::GUISetImage() replacing label for pixmap and label\n");
          
          // Remove the existing label
          gtk_container_remove(GTK_CONTAINER(widget), child);
          
          // Create an hbox
          GtkWidget *box = gtk_hbox_new(false, 0);
          gtk_container_set_border_width(GTK_CONTAINER(box), g_pixmapBorder);
          
          // Add the label
          GtkWidget *lw = gtk_label_new(label.chars());
          gtk_box_pack_start(GTK_BOX(box), tempPixmapWidget, true, true, g_pixmapPadding);
          gtk_box_pack_start(GTK_BOX(box), lw, true, true, g_pixmapPadding);
          
          // Build and add to the button
          gtk_container_add(GTK_CONTAINER(widget), box);
          gtk_widget_show(lw);
          gtk_widget_show(box);
          
          // Store the pixmap
          pix = tempPixmap;
        }
        else
        {
          // Find the first label in the nest of children
          if (GTK_IS_CONTAINER(child))
          {
            ONDEBUG(std::cerr << "ButtonBodyC::GUISetImage() replacing pixmap child\n");
            
            // Find an existing pixmap
            GtkWidget *oldPixmapWidget = NULL;
            gtk_container_foreach(GTK_CONTAINER(child), &find_pixmap_iter, &oldPixmapWidget);
            
            if (oldPixmapWidget != NULL)
            {
              if (GTK_IS_BOX(child))
              {
                // Remove the old, add the new, shuffle it to the front and store the pixmap
                gtk_container_remove(GTK_CONTAINER(child), oldPixmapWidget);
                gtk_box_pack_start(GTK_BOX(child), tempPixmapWidget, false, false, 3);
                gtk_box_reorder_child(GTK_BOX(child), tempPixmapWidget, 0);
                pix = tempPixmap;
              }
            }
          }
        }
      }
    }
    if(m_style.IsValid())
      GUISetStyle(m_style);
    return false;
  }



  void ButtonBodyC::SetImage(const PixmapC &pixmap)
  {
    Manager.Queue(Trigger(ButtonC(*this), &ButtonC::GUISetImage, pixmap));
  }



  bool  ButtonBodyC::Create()
  {
    if (widget != 0)
      return true;
    
    // Replace an empty string with a question mark for text only buttons
    StringC strLabel(label.Size() == 0 ? "?" : label);
    
    if (pix.IsValid())
    {
      // Create the bitmap
      pix.Create(); // This mishapes the window if it hasn't already been shown
      GtkWidget *pixmapWidget = pix.Widget();
      gtk_widget_show(pixmapWidget);
      
      // Create the widget
      widget = BuildWidget(label.Size() == 0 ? NULL : label.chars());
      if (label.Size() == 0) 
      {
        ONDEBUG(std::cerr << "ButtonBodyC::Create() Pixmap\n");
        gtk_container_add(GTK_CONTAINER(widget), pixmapWidget);
      }
      else
      {
        // Create an hbox
        ONDEBUG(std::cerr << "ButtonBodyC::Create() Pixmap Label:'" << strLabel << "'\n");
        GtkWidget *box = gtk_hbox_new(false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(box), g_pixmapBorder);
        
        // Add the label
        GtkWidget *lw = gtk_label_new(strLabel.chars());
        gtk_box_pack_start(GTK_BOX(box), pixmapWidget, true, true, g_pixmapPadding);
        gtk_box_pack_start(GTK_BOX(box), lw, true, true, g_pixmapPadding);
        
        // Build and add to the button
        gtk_container_add(GTK_CONTAINER(widget), box);
        gtk_widget_show(lw);
        gtk_widget_show(box);
      }
    } else
    {
      ONDEBUG(std::cerr << "ButtonBodyC::Create() Label:'" << strLabel << "'\n");
      widget = BuildWidget(strLabel);
    }
    
    // Update style if set.
    if(m_style.IsValid())
      GUISetStyle(m_style);
    
    ConnectSignals();
    
    return true;
  }

  //: Create with a widget supplied from elsewhere.
  
  bool ButtonBodyC::Create(GtkWidget *newWidget) {
    widget = newWidget;
    ConnectSignals();
    return true;
  }
  
  ////////////////////////////////
  //: Create a button.
  
  ButtonC::ButtonC(const char *nlabel,const char *ntooltip)
    : WidgetC(*new ButtonBodyC(nlabel,ntooltip))
  {}
  
  static XMLFactoryRegisterHandleConvertC<ButtonC,WidgetC> g_registerXMLFactoryButton("RavlGUIN::ButtonC");


}
