// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLPLOT_DPGRAPHWINDOWOPORT_HEADER
#define RAVLPLOT_DPGRAPHWINDOWOPORT_HEADER 1
/////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPGraph
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Plotting.DPGraph"
//! file="Ravl/Contrib/Plotting/DPGraph/DPGraphWindowOPort.hh"

#include "Ravl/Plot/DPGraphWindow.hh"

namespace RavlPlotN {

  class DPGraphWindowOPortBaseC {
  public:  
    DPGraphWindowOPortBaseC(const DPGraphWindowC &nwin,const StringC &name);
    //: Constructor.
    
  protected:
    bool accumulate;
    StringC name;
    DPGraphWindowC win;
  };
  
  //! userlevel=Develop
  //: Window OPort body.
  
  template<class DataT>
  class DPGraphWindowOPortBodyC
    : public DPOPortBodyC<DataT>,
      public DPGraphWindowOPortBaseC
  {
  public:
    DPGraphWindowOPortBodyC(DPGraphWindowC &win,const StringC &name,bool accum = false)
      : DPGraphWindowOPortBaseC(win,name)
    {}
    //: Constructor.
    
    virtual bool Put(const DataT &newObj) {
      if(!win.IsValid())
	return false;
      win.Update(name,newObj);
      return true;
    }
    //: Process in coming display objects.
    
  };

  //! userlevel=Normal
  //: Window OPort.
  
  template<class DataT>
  class DPGraphWindowOPortC
    : public DPOPortC<DataT>
  {
  public:
    DPGraphWindowOPortC(DPGraphWindowC &win,const StringC &name,bool accum = false)
      : DPEntityC(*new DPGraphWindowOPortBodyC<DataT>(win,name,accum))
    {}
    //: Constructor.
    
    DPGraphWindowOPortC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // creates an invalid handle.
  };
  
  
}

#endif
