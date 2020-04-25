// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/exSampleStream.cc"
//! docentry="Ravl.API.Pattern_Recognition.Data Set"

//: Example implementation for a sample stream.

#include "Ravl/PatternRec/SampleStreamVector.hh"
#include "Ravl/RandomMersenneTwister.hh"
#include "Ravl/MeanCovariance.hh"

using namespace RavlN;

class RandomVectorStreamBodyC
  : public DPISPortBodyC<VectorC>
{
public:
  RandomVectorStreamBodyC(UIntT vectorSize,UIntT numSamples) 
    : n(vectorSize),
      size(numSamples),
      offset(0)
  {
    twister.Seed(1);
  }
  //: Default constructor.
  
  bool Seek(UIntT off) {
    // Can only seek to offset 0 in stream.
    if(off != 0)
      return false; 
    twister.Seed(1); // Re-seed random number generator.
    offset = 0;
    return true;
  }
  //: Seek to element.
  
  virtual bool Get(VectorC &ret) {
    if(offset >= size)
      return false;
    offset++;
    ret = VectorC(n);
    for(UIntT i = 0;i < n;i++)
      ret[i] = twister.Double();
    return true;
  }
  //: Get next piece of data.
  
protected:
  UIntT n;
  UIntT size;
  UIntT offset;
  RandomMersenneTwisterC twister;
};


class RandomVectorStreamC
  : public SampleStreamVectorC
{
public:
  RandomVectorStreamC(UIntT vectorSize,UIntT numSamples)
    : DPEntityC(*new RandomVectorStreamBodyC(vectorSize,numSamples))
  {}
  //: Constructor.
  // n is size of vector to return.
};


int main(int nargs,char **argv) {
  RandomVectorStreamC randomVectorStream(5,1000000);
  MeanCovarianceC meanCov = randomVectorStream.MeanCovariance();
  std::cerr << "Mean and Coverance of random stream =" << meanCov << "\n";
  return 0;
}
