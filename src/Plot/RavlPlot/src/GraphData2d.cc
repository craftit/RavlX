// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPlot
//! file="Ravl/Contrib/Plotting/Base/GraphData2d.cc"

#include "Ravl/Plot/GraphData2d.hh"
#include "Ravl/Array1dIter2.hh"

namespace RavlPlotN {
  
  //: Default constructor.
  
  GraphData2dBodyC::GraphData2dBodyC() 
  {}
  
  //: Constructor.
  
  GraphData2dBodyC::GraphData2dBodyC(const Array1dC<Point2dC> &xydata)
  {
    data = SArray1dC<Tuple2C<Array1dC<Point2dC>,StringC> >(1);
    data[0].Data1()=xydata;
  }
  
  //: Constructor
  
  GraphData2dBodyC::GraphData2dBodyC(const Array1dC<RealT> &ydata,RealT cstart,RealT cinc) 
  {
    Array1dC<Point2dC> xydata(ydata.Size());
    RealT col = cstart;
    for(Array1dIter2C<Point2dC,RealT> it(xydata,ydata);it;it++) {
      it.Data1()[0] = it.Data2(); // Row
      it.Data1()[1] = col;
      col += cinc;
    }
    data = SArray1dC<Tuple2C<Array1dC<Point2dC>,StringC> >(1);
    data[0].Data1()=xydata;
  }
  
}

