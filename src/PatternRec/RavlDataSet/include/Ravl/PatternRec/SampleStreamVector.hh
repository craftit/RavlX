// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLESTREAMVECTOR_HEADER
#define RAVL_SAMPLESTREAMVECTOR_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! examples=exSampleStream.cc
//! file="Ravl/PatternRec/DataSet/SampleStreamVector.hh"

#include "Ravl/PatternRec/SampleStream.hh"
#include "Ravl/PatternRec/Function.hh"
#include "Ravl/Vector.hh"

namespace RavlN {
  class MeanCovarianceC;
  class MatrixC;
  
  //! userlevel=Normal
  //: Stream of sample vectors
  
  class SampleStreamVectorC 
    : public SampleStreamC<VectorC>
  {
  public:
    SampleStreamVectorC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle
    
    SampleStreamVectorC(const SampleStreamC<VectorC> &stream)
      : DPEntityC(stream)
    {}
    //: Base constructor.
    
    SampleStreamVectorC(const DPISPortC<VectorC> &stream, const FunctionC & function);
    //: Apply a function to the vectors as you get them
    
    SampleStreamVectorC(const DPISPortC<VectorC> &port)
      : DPEntityC(port)
    {}
    //: Base constructor.

    SampleStreamVectorC(const SampleStreamC<TVectorC<float> > &port);
    //: Convert from a stream of float vectors.

    MeanCovarianceC MeanCovariance();
    //: Find the mean and covariance of the sample
    
    MatrixC SumOuterProducts();
    //: Compute the sum of the outerproducts.
    
  protected:

    
  };
}


#endif
