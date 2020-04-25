// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Cluster/ClassifierNearestNeighbour.cc"

#include "Ravl/PatternRec/ClassifierNearestNeighbour.hh"
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
  
  ClassifierNearestNeighbourBodyC::ClassifierNearestNeighbourBodyC(const SampleVectorC &nvectors,
							       const SampleLabelC &nlabels,
							       const DistanceC &xdistanceMetric)
    : distanceMetric(xdistanceMetric),
      vectors(nvectors),
      vlabels(nlabels)
  {
    if(vlabels.Size() > 0)
      NoLabels(vlabels.MaxValue()+1);
    else
      NoLabels(nvectors.Size());
    ONDEBUG(cerr << "ClassifierNearestNeighbourBodyC::ClassifierNearestNeighbourBodyC(), Data=" << vectors.Size() <<" Labels=" << labels << "\n");
  }
  
  //: Constructor.
  
  ClassifierNearestNeighbourBodyC::ClassifierNearestNeighbourBodyC(const SampleVectorC &nvectors,
							       const DistanceC &xdistMetric)
    : distanceMetric(xdistMetric),
      vectors(nvectors)
  {
    NoLabels(nvectors.Size());
    ONDEBUG(cerr << "ClassifierNearestNeighbourBodyC::ClassifierNearestNeighbourBodyC(), Data=" << vectors.Size() <<" Labels=" << NoLabels() << " vlabels=" << vlabels.Size() << "\n");
  }
  
  //: Load from stream.
  
  ClassifierNearestNeighbourBodyC::ClassifierNearestNeighbourBodyC(istream &strm) 
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierNearestNeighbourBodyC(istream &), Unrecognised version number in stream. ");
    strm >> distanceMetric >> vectors >> vlabels;
  }
  
  //: Load from binary stream.
  
  ClassifierNearestNeighbourBodyC::ClassifierNearestNeighbourBodyC(BinIStreamC &strm) 
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierNearestNeighbourBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> distanceMetric >> vectors >> vlabels;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierNearestNeighbourBodyC::Save (ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << ' ' << version << ' ' << distanceMetric << ' ' << vectors << ' ' << vlabels;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierNearestNeighbourBodyC::Save (BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << distanceMetric << vectors << vlabels;
    return true;
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierNearestNeighbourBodyC::Classify(const VectorC &vec) const {
    // Find the k nearest neighbours.
    
    SampleIterC<VectorC> it(vectors);
    IndexC index = 0;
    RealT minDist = distanceMetric.Measure(*it,vec);
    for(it++;it;it++) {
      RealT dist = distanceMetric.Measure(*it,vec);
      if(dist < minDist) {
	minDist = dist;
	index = it.Index();
      }
    }
    if(vlabels.Size() == 0)
      return index.V();
    ONDEBUG(cerr << "Index=" << index << "\n");
    return vlabels[index];
  }

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierNearestNeighbourBodyC,ClassifierNearestNeighbourC,ClassifierC);
  
}
