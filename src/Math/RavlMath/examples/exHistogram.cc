// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/Histogram/exHistogram.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Math.Statistics.Histogram"
//! userlevel=Normal

#include "Ravl/Histogram.hh"
#include "Ravl/StdMath.hh"

using namespace RavlN;

int main() {
    
  HistogramC<int> hist;
  
  hist.Vote(1);
  hist.Vote(2);
  hist.Vote(2);
  hist.Vote(2);
  hist.Vote(3);
  hist.Vote(4);
  
  cout << "Votes:" << hist.Total() << "\n";
  cout << "Information:" << hist.Information() << "\n";
  cout << "Energy:" << hist.Energy() << "\n";
  return 0;
}
