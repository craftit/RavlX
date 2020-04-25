// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_DPWINDOW3DOPORT_HEADER
#define RAVLGUI_DPWINDOW3DOPORT_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPDisplay3d
//! file="Ravl/GUI/DPDisplay3d/DPWindow3dOPort.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.3D Data Display"

#include "Ravl/GUI/DPWindow3d.hh"

namespace RavlGUIN {

  //! userlevel=Develop
  //: Window OPort body.
  
  class DPWindow3dOPortBodyC
    : public DPOPortBodyC<DObject3DC>
  {
  public:
    DPWindow3dOPortBodyC(DPWindow3dC &win,bool accum = false,bool resetWin = false);
    //: Constructor.
    
    virtual ~DPWindow3dOPortBodyC() { }
    //: Need virtual destructor for class with virtual methods

    virtual bool Put(const DObject3DC &newObj);
    //: Process in coming display objects.
    
  protected:
    bool accumulate;
    bool m_resetWin;
    DPWindow3dC win;
  };

  //! userlevel=Normal
  //: Window OPort.
  
  class DPWindow3dOPortC
    : public DPOPortC<DObject3DC>
  {
  public:
    DPWindow3dOPortC(DPWindow3dC &win,bool accum = false,bool resetWin = false)
      : DPEntityC(*new DPWindow3dOPortBodyC(win,accum,resetWin))
    {}
    //: Constructor.
    
    DPWindow3dOPortC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // creates an invalid handle.
  };
  
  
}

#endif
