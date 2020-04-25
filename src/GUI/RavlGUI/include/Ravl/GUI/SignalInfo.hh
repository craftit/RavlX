// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_SIGNALINFO_HEADER
#define RAVLGUI_SIGNALINFO_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! userlevel=Develop
//! file="Ravl/GUI/GTK/SignalInfo.hh"
//! lib=RavlGUI
//! userlevel=Advanced
//! docentry="Ravl.API.Graphics.GTK.Internal"
//! author="Charles Galambos"
//! date="23/09/1999"

//**** This header is for internal use ONLY ******

#include "Ravl/GUI/GTKTypes.hh"
#include <gtk/gtk.h>

namespace RavlGUIN {
  //: Signal types.

  enum SignalTypeT
  {
    SigTypeUnknown,
    SigTypeGeneric,
    SigTypeEvent,
    SigTypeEventDelete,
    SigTypeEventMouseButton,
    SigTypeEventMouseMotion,
    SigTypeEventScroll,
    SigTypeEventKeyboard,
    SigTypeEventFocus,
    SigTypeString,
    SigTypeCListSel,
    SigTypeCListCol,
    SigTypeDNDContext,
    SigTypeDNDPosition,
    SigTypeDNDData,
    SigTypeInt,
    SigTypeWidgetInt,
    SigTypeTreeRow,
    SigTypeTreePathCol,
    SigTypeTreePath,
  };

  //: Signal info

  struct GTKSignalInfoC {
    GTKSignalInfoC()
      : name(0),
  signalType(SigTypeUnknown)
    {}
    //: Constructor to ensure name defaults to 0.

    GTKSignalInfoC(const char *nname,GtkSignalFunc nfunc,SignalTypeT nsignalType)
      : name(nname),
  func(nfunc),
  signalType(nsignalType)
    {}
    //: Constructor.

    const char *name;
    GtkSignalFunc func;
    SignalTypeT signalType;
  };

}
#endif
