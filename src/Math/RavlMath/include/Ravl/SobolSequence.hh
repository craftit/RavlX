// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SOBOLSEQUENCE_HEADER
#define RAVL_SOBOLSEQUENCE_HEADER 1
////////////////////////////////////////////////////
//! file="Ravl/Math/Sequence/SobolSequence.hh"
//! lib=RavlMath
//! author="Charles Galambos"
//! rcsid="$Id$"
//! docentry="Ravl.API.Math.Sequences"
//! date="15/03/1999"

#include "Ravl/SArray1d.hh"
#include "Ravl/SArray2d.hh"

namespace RavlN {
  //! userlevel=Normal
  //: Generate a Sobol sequence.
  // Re-implementation from numerical recipies 2nd edition. pg 312
  
  class SobolSequenceC {
  public:
    inline SobolSequenceC(int n = 0,int bits = 30);
    //: Genererate a sequence with n dimensions.
    
    IntT N() const 
    { return n; }
    //: Get dimension of sequence.
    
    bool First();
    //: Goto first point in sequence.
    
    bool IsElm() const 
    { return !done; }
    //: Are the numbers left in the sequence ?

    operator bool() const
    { return !done; }
    //: Are the numbers left in the sequence ?
    
    bool Next();
    //: Goto next point in sequence.
    // Return: false if out of numbers.
    
    void operator++(int)
    { Next(); }
    //: Goto next value in the sequence.
    
    const SArray1dC<RealT> &Data() const 
    { return result; }
    //: Get point. each element will have a value between
    // 0 and one.
    
  private:  
    void Init();
    // Initalise static's
    
    IntT n;        // Dimension of sequence.  
    IntT s;        // Position in sequence.
    IntT bits;     // Bits to use.
    bool done;
    RealT frac;
    SArray1dC<UIntT> work;
    SArray1dC<RealT> result;
    SArray2dC<UIntT> vx;
  };
  
  ////////////////////////////////////////
  
  inline 
  SobolSequenceC::SobolSequenceC(int nn,int nbits)
    : n(nn),
      s(0),
      bits(nbits)
  { Init(); }
}
  

#endif
