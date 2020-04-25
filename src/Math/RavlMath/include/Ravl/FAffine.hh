// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FAFFINE_HEADER
#define RAVL_FAFFINE_HEADER 1
//////////////////////////////////////////////////////
//! author="Charles Galambos"
//! date="17/3/1997"
//! docentry="Ravl.API.Math.Geometry"
//! rcsid="$Id$"
//! example=exFAffine.cc
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/FAffine.hh"

#include "Ravl/FMatrix.hh"
#include "Ravl/FVector.hh"

namespace RavlN {
  class BinOStreamC;
  class BinIStreamC;
  
  //! userlevel=Normal
  //: General affine transformation.
  
  template<unsigned int N>
  class FAffineC {
  public:
    inline FAffineC();
    //: Construct no-change transform.
    
    inline FAffineC(const FAffineC &Oth);
    //: Copy constructor.
    
    inline FAffineC(const FMatrixC<N,N> &SR, const FVectorC<N> &T);
    //: Construct from Scale/Rotate matrix and a translation vector.
    
    inline FVectorC<N> &Translation() { return T; }
    //: Access the translation component of the transformation.
    
    inline const FVectorC<N> &Translation() const { return T; }
    //: Constant access to the translation component of the transformation.
    
    inline void Scale(const FVectorC<N> &xy);
    //: In place Scaling along the X & Y axis by value given in the vector.
    // If all values 1, then no effect.
    
    inline void Translate(const FVectorC<N> &T);
    //: Add a translation in direction T.
    
    inline FVectorC<N> operator*(const FVectorC<N> &In) const;
    //: Transform Vector,  Scale, Rotate, Translate.
    // Take a vector and put it though the transformation.
    
    inline FAffineC<N> operator*(const FAffineC &In) const;
    //: Compose this transform with 'In'
    
    inline FAffineC<N> operator/(const FAffineC &In) const;
    //: 'In' / 'Out' = this; 
    
    FAffineC<N> I() const
    { return Inverse(); }
    //: Generate an inverse transformation.
    // This is obsolete, use Inverse().
    
    FAffineC<N> Inverse() const;
    //: Generate an inverse transformation.
    
    FMatrixC<N,N> &SRMatrix() { return SR; }
    //: Get Scale/Rotate matrix.
    
    const FMatrixC<N,N> &SRMatrix() const { return SR; }
    //: Get Scale/Rotate matrix.
    
    inline const FAffineC<N> &operator=(const FAffineC &Oth);
    //: Assignment.
    
    bool IsReal() const;
    //: Check all components of transform are real.

  protected:
    FMatrixC<N,N> SR; // Scale/rotate.
    FVectorC<N> T;   // Translate.
    
  };

  template<unsigned int N>
  BinOStreamC & operator<< (BinOStreamC & outS, const FAffineC<N> & vector) {
    outS << vector.SRMatrix() << vector.Translation();
    return outS;
  }
  
  template<unsigned int N>
  BinIStreamC & operator>> (BinIStreamC & inS, FAffineC<N> & vector) {
    inS >> vector.SRMatrix() >> vector.Translation();
    return inS;
  }
  
  /////////////////////////////////////////////////
  
  template<unsigned int N>
  inline FAffineC<N>::FAffineC()
    : SR()
  {
    T.Fill(0);
    SR.Fill(0.0);
    for(unsigned int i = 0;i < N;i++)
      SR[i][i] = 1.0;
  }
  
  template<unsigned int N>
  inline FAffineC<N>::FAffineC(const FAffineC &Oth) 
    : SR(Oth.SR),
      T(Oth.T)
  {}
  
  template<unsigned int N>
  inline FAffineC<N>::FAffineC(const FMatrixC<N,N> &nSR, const FVectorC<N> &nT) 
    : SR(nSR),
      T(nT)
  {}
  
  template<unsigned int N>
  void FAffineC<N>::Scale(const FVectorC<N> &xy) {
    for(UIntT i = 0;i < N;i++)
      for(UIntT j = 0;j < N;j++)
	SR[i][j] *= xy[j];
  }
  
  template<unsigned int N>
  inline void FAffineC<N>::Translate(const FVectorC<N> &DT) {
    T += DT;
  }
  
  template<unsigned int N>
  FAffineC<N> FAffineC<N>::Inverse(void) const {
    FAffineC<N> ret;
    ret.SR = SR.Inverse();
    Mul(ret.SR,T,ret.T);
    ret.T *= -1;
    return ret;
  }
  
  template<unsigned int N>
  FVectorC<N> FAffineC<N>::operator*(const FVectorC<N> &In) const {
    return (SR * In) + T;
  }
  
  template<unsigned int N>
  FAffineC<N> FAffineC<N>::operator*(const FAffineC &In) const{
    return FAffineC(SR*In.SRMatrix(), SR*In.Translation() + T);
  }
  
  template<unsigned int N>
  FAffineC<N> FAffineC<N>::operator/(const FAffineC &In) const{
    FMatrixC<N,N> inverse = In.SRMatrix().Inverse();
    return FAffineC(SR*inverse, inverse*(T-In.Translation()));
  }
  
  template<unsigned int N>
  inline const FAffineC<N> &FAffineC<N>::operator=(const FAffineC &Oth) {
    SR = Oth.SR;
    T = Oth.T;
    return *this;
  }
  
  template<unsigned int N>
  bool FAffineC<N>::IsReal() const {
    for(UIntT i = 0;i < N;i++) {
      for(UIntT j = 0;j < N;j++) {
        if(IsNan(SR[i][j]) || IsInf(SR[i][j]))
          return false;
      }
      if(IsNan(T[i]) || IsInf(T[i]))
        return false;
    }
    return true;
  }

  
  template<unsigned int N>
  ostream &operator<<(ostream & outS, const FAffineC<N> & vector) {
    outS << vector.SRMatrix() << "\t" << vector.Translation();
    return outS;
  }
  
  template<unsigned int N>
  istream &operator>>(istream & inS, FAffineC<N> & vector) {
    inS >> vector.SRMatrix() >> vector.Translation();
    return inS;
  }
   
}
#endif
  
