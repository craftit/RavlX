// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Cluster/DesignMeanShiftCluster.cc"

#include "Ravl/PatternRec/DesignMeanShiftCluster.hh"
#include "Ravl/HSet.hh"
#include "Ravl/Math.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/ClassifierNearestNeighbour.hh"
#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/VirtualConstructor.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  // k is the size of the kernel
  // ntermiter is the minimum distance the cluster centre is moved before terminating iteration.

  DesignMeanShiftClusterBodyC::DesignMeanShiftClusterBodyC(RealT nk,RealT ntermiter,const DistanceC &distanceMetric)
    : distance(distanceMetric),
      k(nk),
      termiter(ntermiter)
  {}


  //: Load from stream.
  
  DesignMeanShiftClusterBodyC::DesignMeanShiftClusterBodyC(istream &strm) 
    : DesignClusterBodyC(strm),
      k(0)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignMeanShiftClusterBodyC(istream &), Unrecognised version number in stream. ");
    strm >> distance >> k >> termiter;
  }
  
  //: Load from binary stream.
  
  DesignMeanShiftClusterBodyC::DesignMeanShiftClusterBodyC(BinIStreamC &strm) 
    : DesignClusterBodyC(strm),
      k(0),
      termiter(0)
  {
    IntT version;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("DesignMeanShiftClusterBodyC((BinIStreamC &), Unrecognised version number in stream. ");
    strm >> distance >> k >> termiter;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignMeanShiftClusterBodyC::Save (std::ostream &out) const
  {
    if(!DesignClusterBodyC::Save(out))
      return false;
    IntT version = 0;
    out << ' ' << version << ' ' << distance << ' ' << k << ' ' << termiter;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignMeanShiftClusterBodyC::Save (BinOStreamC &out) const {
    if(!DesignClusterBodyC::Save(out))
      return false;
    IntT version = 0;
    out << version << distance << k << termiter;
    return true;
  }
  
  //: Find means for 'in'.
  
  DListC<VectorC> DesignMeanShiftClusterBodyC::FindMeans(const SampleC<VectorC> &in,SampleC<UIntT> &labels) {
    DListC<VectorC> clusters;
    if(in.Size() == 0) {
      std::cerr << "DesignMeanShiftClusterBodyC::Apply(), WARNING: No data samples given. \n";
      return clusters;
    }
    labels = SampleC<UIntT>(SArray1dC<UIntT>(in.Size()));
    
    UIntT count;
    VectorC shift(in.First().Size());
    VectorC mean(shift.Size());
    VectorC diff(shift.Size());
    for(DataSet2IterC<SampleC<VectorC>,SampleC<UIntT> > sit(in,labels);sit;sit++) { // Got through all possible start points.
      for(BufferAccessIter2C<RealT,RealT> vit(mean,sit.Data1());vit;vit++)
	vit.Data1() = vit.Data2();
      do {
	count = 0;
	shift.Fill(0);
	for(SampleIterC<VectorC> it(in);it;it++) {
	  for(BufferAccessIter3C<RealT,RealT,RealT> vit(diff,it.Data(),mean);vit;vit++)
	    vit.Data1() = vit.Data2() - vit.Data3();
	  RealT mag = distance.Magnitude(diff);
	  if(mag > k)
	    continue;
	  shift += diff;
	  count++;
	}
	shift /= (RealT) count;
	if(distance.Magnitude(shift) < termiter)
	  break;
	mean += shift;
      } while(1);
      
      DLIterC<VectorC> cit(clusters);
      UIntT n = 0;
      for(;cit;cit++,n++) {
	if(distance.Measure(mean,*cit) < (k * 0.2))
	  break; // Already got cluster.
      }
      if(!cit) // Cluster not found.
	clusters.InsLast(mean.Copy()); // Create new cluster center
      sit.Data2() = n; // Record cluster id for this sample element
    }
    return clusters;
  }
  
  //: Find weighted means for 'in'.
  
  DListC<VectorC> DesignMeanShiftClusterBodyC::FindMeans(const SampleC<VectorC> &in,const SampleC<RealT> &weights,SampleC<UIntT> &labels) {
    DListC<VectorC> clusters;
    labels = SampleC<UIntT>(SArray1dC<UIntT>(in.Size()));
    if(in.Size() == 0) {
      std::cerr << "DesignMeanShiftClusterBodyC::Apply(), WARNING: No data samples given. \n";
      return clusters;
    }
    
    RealT count;
    VectorC shift(in.First().Size());
    VectorC mean(shift.Size());
    VectorC diff(shift.Size());
    for(DataSet2IterC<SampleC<VectorC>,SampleC<UIntT> > sit(in,labels);sit;sit++) { // Got through all possible start points.
      for(BufferAccessIter2C<RealT,RealT> vit(mean,sit.Data1());vit;vit++)
	vit.Data1() = vit.Data2();
      do {
	count = 0;
	shift.Fill(0);
	for(DataSet2IterC<SampleC<VectorC>,SampleC<RealT> > it(in,weights);it;it++) {
	  for(BufferAccessIter3C<RealT,RealT,RealT> vit(diff,it.Data1(),mean);vit;vit++)
	    vit.Data1() = vit.Data2() - vit.Data3();
	  RealT mag = distance.Magnitude(diff);
	  if(mag > k)
	    continue;
	  RealT weight = it.Data2();
	  for(BufferAccessIter2C<RealT,RealT> vit(shift,diff);vit;vit++)
	    vit.Data1() += vit.Data2() * weight;
	  count += weight;
	}
	shift /= count;

	if(distance.Magnitude(shift) < termiter)
	  break;
	mean += shift;
      } while(1);
      
      DLIterC<VectorC> cit(clusters);
      UIntT n = 0;
      for(;cit;cit++,n++) {
	if(distance.Measure(mean,*cit) < (k * 0.2))
	  break; // Already got cluster.
      }
      
      if(!cit) // Cluster not found.
	clusters.InsLast(mean.Copy()); // Create new cluster center
      sit.Data2() = n;  // Record cluster id for this sample element
    }
    return clusters;
  }
  
  //: Create a clasifier.
  
  FunctionC DesignMeanShiftClusterBodyC::Apply(const SampleC<VectorC> &in) {
    ONDEBUG(cerr << "DesignMeanShiftClusterBodyC::Apply(), Called with " << in.Size() << " vectors. K=" << k << "\n");
    SampleC<UIntT> labels;
    DListC<VectorC> clusters = FindMeans(in,labels);
    SampleC<VectorC> newMeans(clusters.Size());
    for(DLIterC<VectorC> cit(clusters);cit;cit++)
      newMeans.Append(*cit);
    return ClassifierNearestNeighbourC(newMeans,distance);
  }
  
  //: Compute cluster means.

  SArray1dC<MeanCovarianceC> DesignMeanShiftClusterBodyC::Cluster(const SampleC<VectorC> &in) {
    ONDEBUG(cerr << "DesignMeanShiftClusterBodyC::Cluster(), Called with " << in.Size() << " vectors. K=" << k << "\n");
    SampleC<UIntT> labels;
    DListC<VectorC> clusters = FindMeans(in,labels);
    SArray1dC<MeanCovarianceC> ret(clusters.Size());
    DLIterC<VectorC> lit(clusters);
    for(SArray1dIterC<MeanCovarianceC> ait(ret);ait;ait++,lit++) {
      *ait = MeanCovarianceC(1,*lit,MatrixC::Identity(lit->Size()) * k);
    }
    return ret;
  }

  //: Compute cluster means.

   SArray1dC<MeanCovarianceC> DesignMeanShiftClusterBodyC::Cluster(const SampleC<VectorC> &in, const SampleC<RealT> & weights) {
     ONDEBUG(cerr << "DesignMeanShiftClusterBodyC::Cluster(), Called with " << in.Size() << " vectors. K=" << k << "\n");
     SampleC<UIntT> labels;
     DListC<VectorC> clusters = FindMeans(in, weights, labels);
     SArray1dC<MeanCovarianceC> ret(clusters.Size());
     DLIterC<VectorC> lit(clusters);
     for(SArray1dIterC<MeanCovarianceC> ait(ret);ait;ait++,lit++) {
       *ait = MeanCovarianceC(1,*lit,MatrixC::Identity(lit->Size()) * k);
     }
     return ret;
   }


  //: Compute cluster means, and labels for all the samples
  
  SArray1dC<MeanCovarianceC> DesignMeanShiftClusterBodyC::Cluster(const SampleC<VectorC> &in,SampleC<UIntT> &labels) {
    DListC<VectorC> clusters = FindMeans(in,labels);
    SArray1dC<MeanCovarianceC> ret(clusters.Size());
    DLIterC<VectorC> lit(clusters);
    for(SArray1dIterC<MeanCovarianceC> ait(ret);ait;ait++,lit++) {
      *ait = MeanCovarianceC(1,*lit,MatrixC::Identity(lit->Size()) * k);
    }
    return ret;    
  }

  //: Compute cluster means, and labels for all the samples

   SArray1dC<MeanCovarianceC> DesignMeanShiftClusterBodyC::Cluster(const SampleC<VectorC> &in, const SampleC<RealT> & weights, SampleC<UIntT> &labels) {
     DListC<VectorC> clusters = FindMeans(in,weights,labels);
     SArray1dC<MeanCovarianceC> ret(clusters.Size());
     DLIterC<VectorC> lit(clusters);
     for(SArray1dIterC<MeanCovarianceC> ait(ret);ait;ait++,lit++) {
       *ait = MeanCovarianceC(1,*lit,MatrixC::Identity(lit->Size()) * k);
     }
     return ret;
   }

  //: Create function from the given data, and sample weights.
  
  FunctionC DesignMeanShiftClusterBodyC::Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weight) {
    ONDEBUG(cerr << "DesignMeanShiftClusterBodyC::Apply(), Called with " << in.Size() << " vectors. K=" << k << "\n");
    SampleC<UIntT> labels;
    DListC<VectorC> clusters = FindMeans(in,weight,labels);
    SampleC<VectorC> newMeans(clusters.Size());
    for(DLIterC<VectorC> cit(clusters);cit;cit++)
      newMeans.Append(*cit);
    return ClassifierNearestNeighbourC(newMeans,distance);
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(DesignMeanShiftClusterBodyC,DesignMeanShiftClusterC,DesignClusterC);

}

  

