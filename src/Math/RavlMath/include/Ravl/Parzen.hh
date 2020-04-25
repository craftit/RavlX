// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2007, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PARZENWINDOW_HEADER
#define RAVL_PARZENWINDOW_HEADER 1

//! author="Bill Christmas"
//! docentry="Ravl.API.Math.Statistics"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/Histogram/Parzen.hh"

#include "Ravl/Array1d.hh"

//! userlevel=Normal

namespace RavlN {

  class MeanVarianceC;

  //: Generate Parzen estimates of a p.d.f. from a set of samples
  // The smoothing kernel is a Gaussian of unit "variance".<br/>
  // The actual smoothing width used is h<sub>1</sub> &times; s<sub>N-1</sub> / &radic;N, where s<sub>N-1</sub> is the sample standard deviation of the N samples.<br/>
  
  class ParzenWindowC : private Array1dC<RealT> {
    
  public:
    ParzenWindowC (const Array1dC<RealT>& samples, RealT h_1);
    //: Constructor
    //!param: samples - array of samples whose density is to be estimated
    //!param: h_1 - smoothing parameter h<sub>1</sub>, typically about 1.0
    
    RealT PdfEstimate(RealT arg);
    //: Estimate the pdf at the value "arg"

  private:
    RealT bandwidth;
  };
}

#endif
