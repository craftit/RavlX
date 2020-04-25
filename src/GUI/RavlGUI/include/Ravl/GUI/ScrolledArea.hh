// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_SCROLLEDAREA_HEADER
#define RAVLGUI_SCROLLEDAREA_HEADER 1
/////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! author="Charles Galambos"
//! file="Ravl/GUI/GTK/ScrolledArea.hh"
//! date="03/04/1999"
//! example=exScrolledArea.cc
//! docentry="Ravl.API.Graphics.GTK.Layout"

#include "Ravl/GUI/OneChild.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: ScrolledArea body.
  
  class ScrolledAreaBodyC 
    : public OneChildBodyC
  {
  public:
    ScrolledAreaBodyC(const WidgetC &achild,IntT isx = -1,IntT isy = -1,bool vauto = true,bool hauto = true);
    //: Constructor.

    ScrolledAreaBodyC(const WidgetC &achild,
		      IntT isx = -1,IntT isy = -1,
		      GtkPolicyType vpolicy = GTK_POLICY_AUTOMATIC,GtkPolicyType hpolicy = GTK_POLICY_AUTOMATIC);
    //: Constructor.
    // isx,isy set the inital size of the area.
    // Values for policy types are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKPOLICYTYPE">here</A>.
    
    virtual bool Create();
    //: Create the widget.
    
  protected:
    IntT isx,isy; // Inital area size.
    GtkPolicyType vpolicy;
    GtkPolicyType hpolicy;
  };
  
  //! userlevel=Normal
  //: ScrolledArea handle.
  
  class ScrolledAreaC
    : public OneChildC
  {
  public:
    ScrolledAreaC(const WidgetC &achild,IntT isx = -1,IntT isy = -1,bool vauto = true,bool hauto = true)
      : OneChildC(*new ScrolledAreaBodyC(achild,isx,isy,vauto,hauto))
    {}
    //: Create a scrolled area.
    // isx,isy set the inital size of the area.
    // Values for policy types are listed <A HREF="http://library.gnome.org/devel/gtk/stable/gtk-Standard-Enumerations.html#GTKPOLICYTYPE">here</A>.

    ScrolledAreaC(const WidgetC &achild ,
		  IntT isx,IntT isy,
		  GtkPolicyType vpolicy,GtkPolicyType hpolicy = GTK_POLICY_AUTOMATIC)
      : OneChildC(*new ScrolledAreaBodyC(achild,isx,isy,vpolicy,hpolicy))
    {}
    //: Constructor.
    
  };
  

}
#endif
