// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! date="7/10/2003"
//! file="Ravl/GUI/GTK/AspectFrame.cc"

#include "Ravl/GUI/AspectFrame.hh"
#include "Ravl/GUI/Manager.hh"
#include "Ravl/Vector2d.hh"
#include <gtk/gtk.h>


namespace RavlGUIN {
  
  //: Default constructor.
  
  AspectFrameBodyC::AspectFrameBodyC()
    : alignx(0.5),
      aligny(0.5),
      screenCorrection(1),
      aspect(1),
      obeyChild(false),
      useTrueAspect(false)
  {}
  
  AspectFrameBodyC::AspectFrameBodyC(const WidgetC &widge,RealT naspect,int nborder,const StringC &ntitle)
    : FrameBodyC(widge,nborder,ntitle),
      alignx(0.5),
      aligny(0.5),
      screenCorrection(1),
      aspect(naspect),
      obeyChild(false),
      useTrueAspect(false)
  {}
  
  //: Create the widget.
  
  bool AspectFrameBodyC::Create() {
    if(widget != 0)
      return true;
    //cerr << "EventBoxBodyC::Create(), Called. \n";
    
    {
      Vector2dC sizePix(Manager.ScreenSize()[0].V(),Manager.ScreenSize()[1].V());
      Vector2dC sizemm = Manager.PhysicalScreenSize();
      Vector2dC pixelSize = sizemm/sizePix;
      screenCorrection = pixelSize[0] / pixelSize[1]; // row/col
      //cerr << "Correction=" << screenCorrection << "\n";
      // Used: xsize/ysize
    }
    
    if(title == "")
      widget = gtk_aspect_frame_new(0,static_cast<gfloat>(alignx),static_cast<gfloat>(aligny),static_cast<gfloat>(aspect),obeyChild);
    else
      widget = gtk_aspect_frame_new(title.chars(),static_cast<gfloat>(alignx),static_cast<gfloat>(aligny),static_cast<gfloat>(aspect),obeyChild);
    if(border != 0)
      gtk_container_set_border_width (GTK_CONTAINER (widget), border);
    gtk_frame_set_shadow_type (GTK_FRAME(widget),shadow);
    if(child.IsValid()) {
      if(!child.Create())
	return false;
      gtk_container_add (GTK_CONTAINER (widget), child.Widget());
      gtk_widget_show (child.Widget());
    }
    //gtk_container_set_resize_mode(resizeMode);
    ConnectSignals();
    
    return true;
  }

  //: Set obey child.
  
  void AspectFrameBodyC::GUIObeyChild(bool nobeyChild) {
    obeyChild = nobeyChild;
    if(widget == 0)
      return ;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    RealT uar = aspect;
    if(useTrueAspect)
      uar *= screenCorrection;
    gtk_aspect_frame_set (GTK_ASPECT_FRAME(widget),static_cast<gfloat>(alignx),static_cast<gfloat>(aligny),static_cast<gfloat>(uar),obeyChild);
  }
  
  //: Set aspect ratio.
  // GUI thread only.
  
  bool AspectFrameBodyC::GUIAspect(RealT ratio) {
    aspect = ratio;
    if(widget == 0)
      return true;
    RavlAssertMsg(Manager.IsGUIThread(),"Incorrect thread. This method may only be called on the GUI thread.");
    RealT useRatio = aspect;
    if(useTrueAspect)
      useRatio *= screenCorrection;
    gtk_aspect_frame_set (GTK_ASPECT_FRAME(widget),static_cast<gfloat>(alignx),static_cast<gfloat>(aligny),static_cast<gfloat>(useRatio),obeyChild);
    return true;
  }
  
  //: Set aspect ratio.
  
  bool AspectFrameBodyC::Aspect(RealT ratio) {
    Manager.Queue(Trigger(AspectFrameC(*this),&AspectFrameC::GUIAspect,ratio));
    return true;
  }
 
}


