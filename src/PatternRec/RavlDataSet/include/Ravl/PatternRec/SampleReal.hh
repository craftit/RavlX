// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLEREAL_HEADER
#define RAVL_SAMPLEREAL_HEADER 1
//! rcsid="$Id: SampleLabel.hh 5240 2005-12-06 17:16:50Z plugger $"
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleLabel.hh"

#include "Ravl/PatternRec/Sample.hh"

namespace RavlN {
  class VectorC;

  //! userlevel=Normal
  //: Sample of labels.

  class SampleRealC
    : public SampleC<RealT>
  {
  public:
    SampleRealC(SizeT maxSize=10)
      : SampleC<RealT>(maxSize)
    {}
    //: Create a sample of data with a maximum size

    SampleRealC(const SArray1dC<RealT> & dat)
      : SampleC<RealT>(dat)
    {}
    //: Create a sample of data from an array

    SampleRealC(const SampleC<RealT> &sample)
      : SampleC<RealT>(sample)
    {}
    //: Construct from base class.

    SampleRealC(const SampleC<float> &sample);
    //: Convert from float weights
  };

}

#endif
