// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGRAPH_GRAPHDATA2D_HEADER
#define RAVLGRAPH_GRAPHDATA2D_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Plotting"
//! lib=RavlPlot
//! file="Ravl/Contrib/Plotting/Base/GraphData2d.hh"

#include "Ravl/Array1d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/String.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/Tuple2.hh"

namespace RavlPlotN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: 2D Graph description.
  
  class GraphData2dBodyC 
    : public RCBodyC
  {
  public:
    GraphData2dBodyC();
    //: Default constructor.
    
    GraphData2dBodyC(const Array1dC<RealT> &ydata,RealT cstart = 0,RealT cinc = 1);
    //: Constructor
    
    GraphData2dBodyC(const Array1dC<Point2dC> &xydata);
    //: Constructor.
    
    const StringC &Title() const 
    { return title; }
    //: Access title.
    
    const StringC &LabelY() const
    { return labelY; }
    //: Label for Y axis.
    
    const StringC &LabelX() const
    { return labelX; }
    //: Label for X axis
    
    SArray1dC<Tuple2C<Array1dC<Point2dC>,StringC> > &Data()
    { return data; }
    //: Access data, and associated legend
    
    const SArray1dC<Tuple2C<Array1dC<Point2dC>,StringC> > &Data() const 
    { return data; }
    //: Access data, and associated legend
    
  protected:
    StringC graphType; // linegraph,scatter,bar
    StringC title;
    StringC labelX;
    StringC labelY;
    
    SArray1dC<Tuple2C<Array1dC<Point2dC>,StringC> > data;
  };
  
  //! userlevel=Normal
  //: 2D Graph description.

  class GraphData2dC 
    : public RCHandleC<GraphData2dBodyC>
  {
  public:
    GraphData2dC()
    {}
    //: Default constructor.
    
    GraphData2dC(const Array1dC<RealT> &ydata,RealT cstart = 0,RealT cinc = 1)
      : RCHandleC<GraphData2dBodyC>(*new GraphData2dBodyC(ydata,cstart,cinc))
    {}
    //: Constructor
    
    const StringC &Title() const
    { return Body().Title(); }
    //: Access title.
    
    const StringC &LabelY() const
    { return Body().LabelY(); }
    //: Label for Y axis.
    
    const StringC &LabelX() const
    { return Body().LabelX(); }
    //: Label for X axis
    
    SArray1dC<Tuple2C<Array1dC<Point2dC>,StringC> > &Data()
    { return Body().Data(); }
    //: Access data, and associated legend
    
    const SArray1dC<Tuple2C<Array1dC<Point2dC>,StringC> > &Data() const
    { return Body().Data(); }
    //: Access data, and associated legend
  };
}


#endif
