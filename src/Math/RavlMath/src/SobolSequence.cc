// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Sequence/SobolSequence.cc"

#include "Ravl/StdMath.hh"
#include "Ravl/SobolSequence.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/PrimitiveBinaryPolynomial.hh"
#include "Ravl/BfAccIter3.hh"

namespace RavlN {

  static UIntT initseq [6][4] = 
  {{ 1, 3, 5, 15 },
   { 1, 1, 7, 11 },
   { 1, 3, 7,  5 },
   { 1, 3, 3, 15 },
   { 1, 1, 3, 13 },
   { 1, 1, 5,  9 }};
  
  static int initdeg [6]  = { 1, 2, 3, 3, 4, 4 };
  static int initpoly [6] = { 0, 1, 1, 2, 1, 4 };
  
  void SobolSequenceC::Init()  {
    s = 0;
    work = SArray1dC<UIntT>(n);
    work.Fill(0);
    result = SArray1dC<RealT>(n);
    vx = SArray2dC<UIntT>(bits,n);
    done = false;
    frac = 1.0/(1 << bits);
    for(IntT k = 0;k < n;k++) {
      SArray1dC<UIntT> init (initseq[k],initdeg[k],false);
      PrimitiveBinaryPolynomialC bp(initpoly[k],initdeg[k],init);
      SArray1dC<UIntT> seq = bp.Evaluate(bits);
      
      // Work out direction numbers.
      int shift = bits-1;
      for(IntT i = 0;i < bits;i++) 
	vx[i][k] = seq[i] << (shift--);
    }
    Next();
  }
  
  // Goto first in sequence.
  
  bool SobolSequenceC::First()  {
    work.Fill(0);
    s = 0;
    Next();
    return true;
  }
  
  // Goto next point in sequence.
  
  bool SobolSequenceC::Next()  {
    if(done)
      return false;
    int b;
    int k = s++;
    // Find the least unset bit from seqence position.
    for(b = 0;b <= bits;b++) {
      if(!(k & 1))
	break;
      k >>= 1;
    }
    //cerr << "b:" << b << " " << s << "\n";
    if(b == bits) {
      done = true;
      return false;
    }
    // Go through working set..
    for(BufferAccessIter3C<UIntT,UIntT,RealT> it(work,vx[b],result);it;it++) {
      it.Data1() ^= it.Data2();
      //cerr << "R:" << it.Data1() << "\n";
      it.Data3() = (RealT) it.Data1() * frac;
    }
    return true;
  }

}
