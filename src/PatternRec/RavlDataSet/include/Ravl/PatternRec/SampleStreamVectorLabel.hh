// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLESTREAMVECTORLABEL_HEADER
#define RAVL_SAMPLESTREAMVECTORLABEL_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! rcsid="$Id: SampleStreamVector.hh 7324 2009-10-21 16:23:20Z craftit $"
//! lib=RavlPatternRec
//! examples=exSampleStream.cc
//! file="Ravl/PatternRec/DataSet/SampleStreamVector.hh"

#include "Ravl/PatternRec/SampleStream.hh"
#include "Ravl/PatternRec/Function.hh"
#include "Ravl/Vector.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Stream of sample vectors and their corresponding labels
  
  class SampleStreamVectorLabelC 
    : public SampleStream2C<VectorC, UIntT>
  {
  public:
    SampleStreamVectorLabelC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle
    
    SampleStreamVectorLabelC(const SampleStream2C<VectorC, UIntT> & stream)
      : DPEntityC(stream)
    {}
    //: Base constructor.
    
    SampleStreamVectorLabelC(const DPISPortC<Tuple2C<VectorC, UIntT> > & port, const FunctionC & function);
    //: Apply a function to the vectors as you get them
    
    SampleStreamVectorLabelC(const DPISPortC<Tuple2C<VectorC, UIntT> > &port)
      : DPEntityC(port)
    {}
    //: Base constructor.

    SampleStreamVectorLabelC(const SampleStream2C<TVectorC<float>, UIntT> & port);
    //: Convert from a stream of float vectors.

    SampleStreamVectorLabelC(const SampleStream2C<TVectorC<ByteT>, UIntT> & port);
    //: Convert from a stream of byte vectors

#if 0    
    MeanCovarianceC MeanCovariance();
    //: Find the mean and covariance of the sample
    
    MatrixC SumOuterProducts();
    //: Compute the sum of the outerproducts.
#endif    

  protected:

    
  };
}


#endif
