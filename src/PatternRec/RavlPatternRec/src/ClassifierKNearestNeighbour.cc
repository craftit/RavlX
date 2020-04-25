// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Classify/ClassifierKNearestNeighbour.cc"

#include "Ravl/PatternRec/ClassifierKNearestNeighbour.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
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
  
  ClassifierKNearestNeighbourBodyC::ClassifierKNearestNeighbourBodyC(const DataSet2C<SampleVectorC,SampleLabelC> &ndata,
								     UIntT defK,
								     const DistanceC &xdistanceMetric)
    : defaultK(defK),
      distanceMetric(xdistanceMetric),
      data(ndata)
  {
    if(data.Size() > 0)
      NoLabels(ndata.Sample2().MaxValue()+1);
    ONDEBUG(cerr << "ClassifierKNearestNeighbourBodyC::ClassifierKNearestNeighbourBodyC(), Data=" << data.Size() <<" Labels=" << NoLabels() << "\n");
  }
  
  //: Load from stream.
  
  ClassifierKNearestNeighbourBodyC::ClassifierKNearestNeighbourBodyC(std::istream &strm) 
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierKNearestNeighbourBodyC(std::istream &), Unrecognised version number in stream. ");
    strm >> defaultK >> distanceMetric >> data;
  }
  
  //: Load from binary stream.
  
  ClassifierKNearestNeighbourBodyC::ClassifierKNearestNeighbourBodyC(BinIStreamC &strm) 
    : ClassifierBodyC(strm)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("ClassifierKNearestNeighbourBodyC(BinIStreamC &), Unrecognised version number in stream. ");
    strm >> defaultK >> distanceMetric >> data;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierKNearestNeighbourBodyC::Save (std::ostream &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << ' ' << version << ' ' << defaultK << ' ' << distanceMetric << ' ' << data;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool ClassifierKNearestNeighbourBodyC::Save (BinOStreamC &out) const {
    if(!ClassifierBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << defaultK << distanceMetric << data;
    return true;
  }
  
  //: Classifier vector 'data' return the most likely label.
  
  UIntT ClassifierKNearestNeighbourBodyC::Classify(const VectorC &data) const {
    // Find the k nearest neighbours.
    
    SArray1dC<Tuple2C<UIntT,RealT> > res = Search(data,defaultK);
    
    // Cound how many of with each label.
    
    HashC<UIntT,IntC> tab;
    for(SArray1dIterC<Tuple2C<UIntT,RealT> > it(res);it;it++) 
      tab[it->Data1()]++; 

    // Find the most common neighbour, checking for a conflict.
    
    HashIterC<UIntT,IntC> hit(tab);
    RavlAssertMsg(hit,"Need some data to do Nearest Neighbour.");
    UIntT maxLab = hit.Key();
    IntT maxVal = hit.Data();
    bool conflict = false;
    for(hit++;hit;hit++) {
      if(hit.Data() > maxVal) {
	maxLab = hit.Key();
	maxVal = hit.Data();
	conflict = false;
	continue;
      } 
      if(hit.Data() == maxVal)
	conflict = true;
    }
    
    if(!conflict)
      return maxLab; // No conflict, just return label.
    
    // Conflict resolution - Work out average distances, take smallest.
    
    HashC<UIntT,RealC> tab2;
    for(SArray1dIterC<Tuple2C<UIntT,RealT> > ita(res);ita;ita++)
      tab2[ita->Data1()] += ita->Data2();
    
    HashIterC<UIntT,RealC> itd(tab2);
    RavlAssert(itd.IsElm());
    RealT minD = itd.Data();
    maxLab = itd.Key();
    for(;itd;itd++) {
      if(minD < itd.Data()) {
	minD = itd.Data();
	maxLab = itd.Key();
      }
    }
    
    return maxLab;
  }
  
  VectorC ClassifierKNearestNeighbourBodyC::Confidence(const VectorC &data) const {
    SArray1dC<Tuple2C<UIntT,RealT> > res = Search(data,defaultK);
    VectorC ret(NoLabels());
    ret.Fill(0);
    RealT sumDist = res.Size();
    // Generate normalised vector.
    for(SArray1dIterC<Tuple2C<UIntT,RealT> > it(res);it;it++) 
      ret[it->Data1()] += 1/sumDist; 
    return ret;
  }
  //: Estimate the confidence for each label.
  // The meaning of the confidence assigned to each label depends
  // on the classifier used. The higher the confidence the more likely
  // it is the label is correct.
  
  
  //: Find the 'k' nearest neighbours.
  
  SArray1dC<Tuple2C<UIntT,RealT> > ClassifierKNearestNeighbourBodyC::Search(const VectorC &ex,UIntT k) const {
    RavlAssert(k > 0);
    SArray1dC<Tuple2C<UIntT,RealT> > ret(k);
    ONDEBUG(cerr << "ClassifierKNearestNeighbourBodyC::Search(), ex=" << ex << " k=" << k << "\n");
    RavlAssertMsg(ex.Size() != data.Sample1().Size(),"vector size mismatch.");
    DataSet2IterC<SampleVectorC,SampleLabelC> it(data);
    // Initaly use the first 'k' examples with the first entries from 'data'.
    if(!it)
      return SArray1dC<Tuple2C<UIntT,RealT> >(); // No examples.
    // Initalise max with first element.
    UIntT maxInd = 0;
    SArray1dIterC<Tuple2C<UIntT,RealT> > xit(ret);
    RealT max = distanceMetric.Measure(ex,it.Data1());
    *xit =  Tuple2C<UIntT,RealT>(it.Data2(),max);
    xit++;
    it++;
    // Then do the rest.
    for(;xit && it;xit++,it++) {
      ONDEBUG(cerr << "ClassifierKNearestNeighbourBodyC::Search(),1 Data1=" << it.Data1() << " ex=" << ex << "\n");
      RealT dist = distanceMetric.Measure(it.Data1(),ex);
      *xit =  Tuple2C<UIntT,RealT>(it.Data2(),dist);
      if(dist > max) {
	max = dist;
	maxInd= xit.Index().V();
      }
    }
    if(!it) // In the unlikely event we've got less than 'k' examples to choose from.
      return SArray1dC<Tuple2C<UIntT,RealT> >(ret,(SizeT) xit.Index().V()); // Just return what we have.
    // Ok... get on with the hard work.
    for(;it;it++) {
      ONDEBUG(cerr << "ClassifierKNearestNeighbourBodyC::Search(),2 Data1=" << it.Data1() << " ex=" << ex << "\n");
      RealT dist = distanceMetric.Measure(ex,it.Data1());
      if(dist > max)
	continue; // Its bigger than all the examples we have.
      // Update out 'k' best so far.
      // First replace entry with biggest distance.
      ret[maxInd] = Tuple2C<UIntT,RealT>(it.Data2(),dist);
      max = dist;
      // Then go through and find which entry now has the largest distance.
      for(xit.First();xit;xit++) {
	if(xit->Data2() <= max) 
	  continue;
	max = xit->Data2();
	maxInd = xit.Index().V();
      }
    }
    return ret;
  }
    
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(ClassifierKNearestNeighbourBodyC,ClassifierKNearestNeighbourC,ClassifierC);


}
