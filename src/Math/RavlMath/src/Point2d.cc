// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/Geometry/Euclidean/2D/Point2d.cc"

#include "Ravl/Point2d.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Matrix3d.hh"
#include "Ravl/Pair.hh"
#include "Ravl/Slice1d.hh"
#include "Ravl/Slice1dIter.hh"

namespace RavlN {
  
  template <class Container1T>
  bool TemplNormalise(const Container1T &raw, DListC<Point2dC> &norm,Matrix3dC &normMat) {
    if(raw.Size() == 0)
      return true;
    norm = DListC<Point2dC>();
    Point2dC mean(0,0);

    for(typename Container1T::IteratorT rawIter(raw);rawIter;rawIter++) {
      RavlAssert(!IsNan(rawIter.Data()[0]) && !IsNan(rawIter.Data()[1]));
      norm.Append(rawIter.Data());
      mean += rawIter.Data();
    
    }
    mean /= static_cast<RealT>(raw.Size());
    RealT d = 0;
    for(DListC<Point2dC>::IteratorT it(norm);it;it++) {
      (*it) -= mean;
      d += Hypot((*it)[0],(*it)[1]);
    }
    d = IsSmall(d) ? 1.0 : (static_cast<RealT>(raw.Size())/d);
    for(DListC<Point2dC>::IteratorT it(norm);it;it++)
      (*it) *= d;
    normMat = Matrix3dC(d,0,-mean[0] * d,
			0,d,-mean[1] * d,
			0,0,1);
    return true;    
  }
  //: Normalise an array of points.
  // This finds the mean and variation of euclidean point position. It corrects the mean to zero
  // and the average variation to 1.
  //!param: raw - Points to be normalised
  //!param: norm - Array to write normalised points into. A new array is always created as assigned to this handle. Note: If the array is of the required size it may be resused.
  //!param: normMat - Normalisation matrix. Multiplying the normalised points by this matrix will map them to the original space.
  //!return: true if normalisation found and applied.

  template <class Container1T, class Container2T> // Both Container types must be containers of Point2dC. Both must be iterable and Container2T must be able to be initialized with the size of the container.
  bool TemplNormalise(const Container1T &raw,Container2T &norm,Matrix3dC &normMat) {
    if(raw.Size() == 0)
      return true;
    norm = Container2T(raw.Size());

    typename Container1T::IteratorT rawIter(raw);
    typename Container2T::IteratorT normIter(norm);
    
    Point2dC mean(0,0);
    
    for(rawIter.First(), normIter.First();rawIter;rawIter++, normIter++) {
      RavlAssert(normIter);
      RavlAssert(!IsNan(rawIter.Data()[0]) && !IsNan(rawIter.Data()[1]));
      normIter.Data() = rawIter.Data();
      mean += rawIter.Data();
    }

    mean /= static_cast<RealT>(raw.Size());
    RealT d = 0;
    for(normIter.First();normIter;normIter++) {
      (*normIter) -= mean;
      d += Hypot((*normIter)[0],(*normIter)[1]);
    }
    d = IsSmall(d) ? 1.0 : (static_cast<RealT>(raw.Size())/d);
    for(normIter.First();normIter;normIter++)
      (*normIter) *= d;
    normMat = Matrix3dC(d,0,-mean[0] * d,
			0,d,-mean[1] * d,
			0,0,1);
    return true;   
  }
  //: Normalise an array of points.
  // This finds the mean and variation of euclidean point position. It corrects the mean to zero
  // and the average variation to 1.
  //!param: raw - Points to be normalised
  //!param: norm - Array to write normalised points into. A new array is always created as assigned to this handle. Note: If the array is of the required size it may be resused.
  //!param: normMat - Normalisation matrix. Multiplying the normalised points by this matrix will map them to the original space.
  //!return: true if normalisation found and applied.



// Any combination of most of RAVLs containers should work but just exposing these for now
bool Normalise(const SArray1dC<Point2dC> &raw,SArray1dC<Point2dC> &norm,Matrix3dC &normMat) 
{
  return TemplNormalise(raw,norm,normMat);
}

bool Normalise(const Slice1dC<Point2dC> &raw,Slice1dC<Point2dC> &norm,Matrix3dC &normMat) 
{
  return TemplNormalise(raw,norm,normMat);
}

bool Normalise(const DListC<Point2dC> &raw,DListC<Point2dC> &norm,Matrix3dC &normMat) 
{
  return TemplNormalise(raw,norm,normMat);
}


}
