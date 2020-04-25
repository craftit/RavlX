// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlOptimise
//! file="Ravl/Math/Optimisation/FitToSample.cc"

#include "Ravl/FitToSample.hh"

namespace RavlN {

  //: Fit parameters to sample of observations
  StateVectorC FitToSampleBodyC::FitModel(DListC<ObservationC> sample)
  {
    RavlAssertMsg(0,"FitToSampleBodyC::FitModel(DListC<ObservationC>(), Abstract method called ");
    return StateVectorC();
  }
}
