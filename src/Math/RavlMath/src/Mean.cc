// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/MeanCovariance/Mean.cc"

#include "Ravl/Mean.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Calculate the mean and variance from an array of numbers.
  
  MeanC::MeanC(const SArray1dC<RealT> &data) {
    n = data.Size();
    mean = 0;
    for(SArray1dIterC<RealT> it(data);it;it++)
      mean += *it;
    mean /= (RealT) n;
  }

  //: Add another Mean to this one.
  
  MeanC &MeanC::operator+=(const RealT &mv) {
    mean = (mean * n + mv) / (n+1);
    n++;
    return *this;
  }

  //: Remove another Mean from this one.
  
  MeanC &MeanC::operator-=(const RealT &mv) {
    mean = (mean * n - mv) / (n-1);
    n--;
    return *this;
  }
  
  //: Add another Mean to this one.
  
  MeanC &MeanC::operator+=(const MeanC &mv) {
    if(mv.Number() == 0)
      return *this;
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) mv.Number();
    const RealT nDen    = number1 + number2;
    const RealT p1 = number1 / nDen;
    const RealT p2 = number2 / nDen;
    
    // Update the mean.
    mean = mean * p1 + mv.Mean() * p2;
    n += mv.Number();
    return *this;
  }

  //: Remove another Mean from this one.
  
  MeanC &MeanC::operator-=(const MeanC &mv) { 
    if(mv.Number() == 0)
      return *this;
    const RealT number1 = (RealT) Number();
    const RealT number2 = (RealT) mv.Number();
    const RealT nDen    = number1 - number2;
    const RealT p1 = nDen / number1;
    const RealT p2 = number2 / number1;
    
    // Update the mean.
    mean = (mean - mv.Mean() * p2) / p1 ;
    
    n -= mv.Number();
    return *this;
  }
  
  
  //: Write to binary output stream.
  
  BinOStreamC& operator<<(BinOStreamC &s,const MeanC &mv) {
    s << mv.Number() << ' ' << mv.Mean() ;
    return s;
  }
  
  //: Write to output stream.
  
  BinIStreamC& operator>>(BinIStreamC &s, MeanC &mv) {
    SizeT n;
    RealT v1;
    s >> n >> v1;
    mv = MeanC(n,v1);
    return s;
  }
  
}
