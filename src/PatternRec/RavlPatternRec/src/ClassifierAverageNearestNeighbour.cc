// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierAverageNearestNeighbour.cc"

#include "Ravl/PatternRec/ClassifierAverageNearestNeighbour.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Hash.hh"
#include "Ravl/RealC.hh"
#include "Ravl/Mean.hh"
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
  
  ClassifierAverageNearestNeighbourBodyC::ClassifierAverageNearestNeighbourBodyC(const DataSet2C<SampleVectorC,SampleLabelC> &ndata,
						 UIntT defK,
						 const DistanceC &xdistanceMetric)
    : ClassifierKNearestNeighbourBodyC(ndata,defK,xdistanceMetric)
  {
    ONDEBUG(cerr << "ClassifierAverageNearestNeighbourBodyC::ClassifierAverageNearestNeighbourBodyC(), Data=" << data.Size() <<" Labels=" << NoLabels() << "\n");
  }
  
  //: Load from stream.
  
  ClassifierAverageNearestNeighbourBodyC::ClassifierAverageNearestNeighbourBodyC(std::istream &strm) 
    : ClassifierKNearestNeighbourBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierAverageNearestNeighbourBodyC(std::istream &), Unrecognised version number in stream. ");
  }
  
  //: Load from binary stream.
  
  ClassifierAverageNearestNeighbourBodyC::ClassifierAverageNearestNeighbourBodyC(BinIStreamC &strm) 
    : ClassifierKNearestNeighbourBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierAverageNearestNeighbourBodyC(BinIStreamC &), Unrecognised version number in stream. ");
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierAverageNearestNeighbourBodyC::Save (std::ostream &out) const {
    if(!ClassifierKNearestNeighbourBodyC::Save(out))
      return false;
    IntT version = 0;
    out << ' ' << version;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierAverageNearestNeighbourBodyC::Save (BinOStreamC &out) const {
    if(!ClassifierKNearestNeighbourBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version;
    return true;
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierAverageNearestNeighbourBodyC::Classify(const VectorC &data) const {
    // Find the k nearest neighbours.
    
    SArray1dC<Tuple2C<UIntT,RealT> > res = Search(data,defaultK);
    
    // Find average distance to each label.
    
    HashC<UIntT,MeanC > tab;
    for(SArray1dIterC<Tuple2C<UIntT,RealT> > it(res);it;it++)
      tab[it->Data1()] += it->Data2();
    
    // Find the smallest mean distance.
    
    HashIterC<UIntT,MeanC> hit(tab);
    RavlAssertMsg(hit,"Need some data to do Nearest Neighbour.");
    UIntT minLab = hit.Key();
    RealT minVal = hit.Data().Mean();
    for(hit++;hit;hit++) {
      if(hit.Data().Mean() < minVal) {
	minLab = hit.Key();
	minVal = hit.Data().Mean();
      }
    }
    
    return minLab;
  }
  
  VectorC ClassifierAverageNearestNeighbourBodyC::Confidence(const VectorC &data) const {
    SArray1dC<Tuple2C<UIntT,RealT> > res = Search(data,defaultK);
    VectorC ret(NoLabels());
    ret.Fill(0);
    // Generate normalised vector.
    HashC<UIntT,MeanC > tab;
    for(SArray1dIterC<Tuple2C<UIntT,RealT> > it(res);it;it++) 
      tab[it->Data1()] += it->Data2();

    RealT sum = 0;
    HashIterC<UIntT,MeanC> hit(tab);
    for(;hit;hit++)
      sum += 1/hit.Data().Mean();
    for(hit.First();hit;hit++)
      ret[hit.Key()] = 1/(hit.Data().Mean() * sum);
    return ret;
  }
  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.
  
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierAverageNearestNeighbourBodyC,ClassifierAverageNearestNeighbourC,ClassifierKNearestNeighbourC);
  

}
