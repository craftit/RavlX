// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlGUI
//! file="Ravl/GUI/GTK/PackInfo.cc"

#include "Ravl/GUI/PackInfo.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlGUIN {

  //: Default constructor.
  PackInfoBodyC::PackInfoBodyC()
    : expand(true),
      fill(true),
      padding(0)
  {
    ONDEBUG(std::cerr << "PackInfoBodyC::PackInfoBodyC(), Called \n");
  }
    
  //: Constructor.
  PackInfoBodyC::PackInfoBodyC(const WidgetC &widge,bool nexpand,bool nfill,IntT npadding)
    : OneChildBodyC(widge),
      expand(nexpand),
      fill(nfill),
      padding(npadding)
  {
    ONDEBUG(std::cerr << "PackInfoBodyC::PackInfoBodyC(), Called " << expand <<" "<< fill <<" "<< padding <<"\n");
  }
  
  //: Destructor.

  PackInfoBodyC::~PackInfoBodyC() {
    ONDEBUG(std::cerr << "PackInfoBodyC::~PackInfoBodyC(), Called \n");
  }

  //: Create the widget.
  
  bool PackInfoBodyC::Create() {
    if(!child.IsValid())
      return false;
    if(!child.Create())
      return false;
    widget = child.Widget();
    return true;
  }
  
  //: Undo all references.
  
  void PackInfoBodyC::Destroy() {
    if(child.IsValid())
      child.Destroy();
    widget = 0; // It'll be the child widget....
    WidgetBodyC::Destroy();
  }
  
  //: Construct packing info from other widget.
  
  PackInfoC::PackInfoC(WidgetC &other)
    : OneChildC(other)
  {
    if(dynamic_cast<PackInfoBodyC *>(&WidgetC::Body()) == 0)
      Invalidate();
  }
  
}
