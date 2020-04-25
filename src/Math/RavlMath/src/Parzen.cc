// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlMath

#include "Ravl/Parzen.hh"
#include "Ravl/MeanVariance.hh"

using namespace RavlN;

// Constructor
ParzenWindowC::ParzenWindowC (const Array1dC<RealT>& samples, RealT h_1)
  : Array1dC<RealT>(samples)
{
  MeanVarianceC stats(this->SArray1d(true));
  bandwidth = h_1*Sqrt(2.0*stats.Variance()/(this->Size()));
}
    
// Estimate the pdf at the value "arg"
RealT ParzenWindowC::PdfEstimate(RealT arg) {
  RealT estimate(0.0);
  for (Array1dIterC<RealT> i(*this); i; ++i) 
    estimate += exp(-Sqr((arg-(*i))/bandwidth));
  return estimate/(this->Size()*bandwidth*Sqrt(RavlConstN::pi));
}
