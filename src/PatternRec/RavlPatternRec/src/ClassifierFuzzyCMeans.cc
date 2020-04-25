// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Cluster/ClassifierFuzzyCMeans.cc"

#include "Ravl/PatternRec/ClassifierFuzzyCMeans.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Hash.hh"
#include "Ravl/RealC.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Default constructor.
  
  ClassifierFuzzyCMeansBodyC::ClassifierFuzzyCMeansBodyC(const SampleVectorC &nvectors,
							 RealT m,
							 const DistanceC &xdistanceMetric)
    : distanceMetric(xdistanceMetric),
      vectors(nvectors),
      _m(m)
  {
    NoLabels(nvectors.Size());
    ONDEBUG(cerr << "ClassifierFuzzyCMeansBodyC::ClassifierFuzzyCMeansBodyC(), Data=" << vectors.Size() <<" Labels=" << labels << "\n");
  }
  
  //: Load from stream.
  
  ClassifierFuzzyCMeansBodyC::ClassifierFuzzyCMeansBodyC(istream &strm) 
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierFuzzyCMeansBodyC(istream &), Unrecognised version number in stream. ");
    strm >> distanceMetric >> vectors >> _m;
  }
  
  //: Load from binary stream.
  
  ClassifierFuzzyCMeansBodyC::ClassifierFuzzyCMeansBodyC(BinIStreamC &strm) 
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierFuzzyCMeansBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> distanceMetric >> vectors >> _m;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierFuzzyCMeansBodyC::Save (ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << ' ' << version << ' ' << distanceMetric << ' ' << vectors << ' ' << _m;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierFuzzyCMeansBodyC::Save (BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << distanceMetric << vectors << _m;
    return true;
  }
  
  //: Classify vector.
  
  VectorC ClassifierFuzzyCMeansBodyC::Apply(const VectorC &X) const {
    VectorC Y (vectors.Size());
    SArray1dIterC<RealT> yit(Y);
    for (SampleIterC<VectorC> it(vectors);it;it++,yit++) {
      RealT value = 0.0;
      for (SampleIterC<VectorC> it2(vectors);it2;it2++) {
      	value += Pow (distanceMetric.Magnitude (X - *it) /
		      distanceMetric.Magnitude (X - *it2),2.0/(_m-1.0));
      }
      value = 1.0/value;
      *yit = value;
    }
    return Y;
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierFuzzyCMeansBodyC::Classify(const VectorC &Xvec) const {
    return Apply(Xvec).MaxIndex().V();
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierFuzzyCMeansBodyC,ClassifierFuzzyCMeansC,ClassifierC);
  
}
