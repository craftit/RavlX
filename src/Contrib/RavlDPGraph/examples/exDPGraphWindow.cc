// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlDPGraph
//! file="Ravl/Contrib/Plotting/DPGraph/exDPGraphWindow.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Graphics.Plotting.DPGraph"
//! userlevel=Normal

#include "Ravl/Array1dIter.hh"
#include "Ravl/IO.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Point2d.hh"

using namespace RavlN;

int main() {

  // Plot a line graph
  
  Array1dC<RealT> data(100);
  RealT val = 0;
  for(Array1dIterC<RealT> it(data);it;it++,val += (RavlConstN::pi/40))
    *it = Sin(val);
  
  if(!RavlN::Save("@GRAPH:exDPGraphWindow",data,"",true)) {
    cerr << "Failed to save graph. \n";
    return 1;
  }

  // Plot a circle.
  
  Array1dC<Point2dC> dataxy(100);
  val = 0;
  for(Array1dIterC<Point2dC> it2(dataxy);it2;it2++,val += (RavlConstN::pi/50))
    *it2 = Point2dC(Sin(val),Cos(val));
  
  if(!RavlN::Save("@GRAPH:exDPGraphWindow2",dataxy,"",true)) {
    cerr << "Failed to save graph. \n";
    return 1;
  }


  // Update the line graph
  
  Sleep(3);
  //for(Array1dIterC<RealT> it(data);it;it++,val += (RavlConstN::pi/40))
  // *it = Sin(val);
  data[50] = 1;
  if(!RavlN::Save("@GRAPH:exDPGraphWindow",data,"",true)) {
    cerr << "Failed to save graph. \n";
    return 1;
  }
  
  return 0;
}
