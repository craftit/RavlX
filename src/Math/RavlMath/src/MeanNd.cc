// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/MeanNd.cc"

#include "Ravl/MeanNd.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/DLIter.hh"

namespace RavlN {
  
  MeanNdC::MeanNdC(const SArray1dC<VectorC> &vecs) {
    number = vecs.Size();
    if(number == 0)
      return ;
    SArray1dIterC<VectorC> it(vecs);
    Mean() = it->Copy();
    it++;
    for(;it;it++)
      Mean() += *it;
    Mean() /= ((RealT) number);
  }

  //: Caluclate the mean of a list of vectors.
  // All the vectors must have the same size, if
  // the list is empty a zero mean is generated.
  
  MeanNdC::MeanNdC(const DListC<VectorC> &vecs) {
    number = vecs.Size();
    if(number == 0)
      return ;
    DLIterC<VectorC> it(vecs);
    Mean() = it->Copy();
    it++;
    for(;it;it++)
      Mean() += *it;
    Mean() /= ((RealT) number);
  }

  
  MeanNdC MeanNdC::Copy() const 
  { return MeanNdC(number, VectorC::Copy()); }
  
  ostream &operator<<(ostream & s, const MeanNdC & mean) {
    s << mean.Number() << ' ' << mean.Mean();
    return s;
  }
  
  istream &operator>>(istream & s, MeanNdC & mean) {
    s >>  mean.number >>  mean.Mean();
    return s;
  }

  // Saves the statistical description of the set 'mean' into the output
  // stream 'outS'.
  
  BinOStreamC & operator<<(BinOStreamC & s, const MeanNdC & mean) {
    s << mean.Number() << mean.Mean();
    return s;
  }

  
  // Reads and sets the statistical description of the set 'mean'
  // according to the information in the input stream 'inS'.
  
  BinIStreamC & operator>>(BinIStreamC & s, MeanNdC & mean) {
    s >>  mean.number >>  mean.Mean();
    return s;
  }

}
