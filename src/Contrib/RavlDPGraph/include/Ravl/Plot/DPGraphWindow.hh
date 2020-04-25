// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLPLOT_DPGRAPHWINDOW_HEADER
#define RAVLPLOT_DPGRAPHWINDOW_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlDPGraph
//! file="Ravl/Contrib/Plotting/DPGraph/DPGraphWindow.hh"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Plotting.DPGraph"

#include "Ravl/DP/Port.hh"
#include "Ravl/GUI/Window.hh"
#include "Ravl/Plot/GuppiGraph.hh"
#include "Ravl/Plot/GuppiCanvas.hh"
#include "Ravl/Plot/GuppiLineGraph.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Hash.hh"

namespace RavlPlotN {
  using namespace RavlGUIN;
  
  class DPGraphWindowC;
  
  //! userlevel=Develop
  //: DP Window base.
  
  class DPGraphWindowBodyC 
    : public RCBodyVC
  {
  public:
    DPGraphWindowBodyC(const StringC &nname,const IndexRange2dC &nWinSize = IndexRange2dC(IndexC(300),IndexC(300)));
    //: Default constructor.
    
    const StringC &Name() const
    { return name; }
    //: Access window name.
    
    void Init(const GuppiGraphC &initGraph);
    //: Construct widgets.
    
    void Update(const StringC &name,const Array1dC<RealT> &data);
    //: Update data.
    // Thread safe update.
    
    bool GUIUpdate(const StringC &name,const Array1dC<RealT> &data);
    //: Update data.
    // Call on the GUI thread only.
    
    void Update(const StringC &name,const Array1dC<Point2dC> &data);
    //: Update data.
    // Thread safe update.
    
    bool GUIUpdatePoint2d(const StringC &name,const Array1dC<Point2dC> &data);
    //: Update data.
    // Call on the GUI thread only.
    
    bool Clear();
    //: Clear the display list.
    
    bool Refresh();
    //: Refresh the display.
    
    bool WindowClosed();
    //: Called when window is closed.
    
  protected:    
    StringC name;
    MutexC accessMutex; // Make sure window is only setup at once.
    WindowC win;
    IndexRange2dC winSize;
    GuppiCanvasC canvas;
    GuppiGraphC graph;
    HashC<StringC,GuppiLineGraphC> plots;
    
    friend class DPGraphWindowC;    
  };
  
  //! userlevel=Normal
  //: DP Window.
  
  class DPGraphWindowC 
    : public RCHandleC<DPGraphWindowBodyC>
  {
  public:
    DPGraphWindowC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPGraphWindowC(const StringC &nname,const IndexRange2dC &nWinSize = IndexRange2dC(IndexC(100),
										      IndexC(100)))
      : RCHandleC<DPGraphWindowBodyC>(*new DPGraphWindowBodyC(nname,nWinSize))
    {}
    //: Construct a named window.

  protected:
    DPGraphWindowC(DPGraphWindowBodyC &body)
      : RCHandleC<DPGraphWindowBodyC>(body)
    {}
    //: Body constructor.
    
    DPGraphWindowBodyC &Body()
    { return RCHandleC<DPGraphWindowBodyC>::Body(); }
    //: Access body.
    
    const DPGraphWindowBodyC &Body() const
    { return RCHandleC<DPGraphWindowBodyC>::Body(); }
    //: Access body.

    
  public:
    
    void Update(const StringC &name,const Array1dC<RealT> &data)
    { Body().Update(name,data); }
    //: Update data.
    // Thread safe update.
    
    bool GUIUpdate(const StringC &name,const Array1dC<RealT> &data)
    { return Body().GUIUpdate(name,data); }
    //: Update data.
    // GUI thread only update.
    
    void Update(const StringC &name,const Array1dC<Point2dC> &data)
    { Body().Update(name,data); }
    //: Update data.
    // Thread safe update.
    
    bool GUIUpdatePoint2d(const StringC &name,const Array1dC<Point2dC> &data)
    { return Body().GUIUpdatePoint2d(name,data); }
    //: Update data.
    // GUI thread only update.
    
    bool Clear()
    { return Body().Clear(); }
    //: Clear the display list.
    
    bool Refresh()
    { return Body().Refresh(); }
    //: Refresh the display.
    
    bool WindowClosed()
    { return Body().WindowClosed(); }
    //: Called when window is closed.

    friend class DPGraphWindowBodyC;
  };
  
}



#endif
