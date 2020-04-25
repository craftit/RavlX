// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLESTREAMVECTORFLOAT_HEADER
#define RAVL_SAMPLESTREAMVECTORFLOAT_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! rcsid="$Id: SampleStreamVector.hh 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlPatternRec
//! examples=exSampleStream.cc
//! file="Ravl/PatternRec/DataSet/SampleStreamVector.hh"

#include "Ravl/PatternRec/SampleStream.hh"
#include "Ravl/Vector.hh"

namespace RavlN {
  class MeanCovarianceC;
  class MatrixC;
  
  //! userlevel=Normal
  //: Stream of sample vectors
  
  class SampleStreamVectorFloatC
    : public SampleStreamC<TVectorC<float> >
  {
  public:
    SampleStreamVectorFloatC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle
    
    SampleStreamVectorFloatC(const SampleStreamC<TVectorC<float> > &stream)
      : DPEntityC(stream)
    {}
    //: Base constructor.
    
    SampleStreamVectorFloatC(const DPISPortC<TVectorC<float> > &port)
      : DPEntityC(port)
    {}
    //: Base constructor.

    MeanCovarianceC MeanCovariance();
    //: Find the mean and covariance of the sample
    
    MatrixC SumOuterProducts();
    //: Compute the sum of the outer products.
  };

}


#endif
