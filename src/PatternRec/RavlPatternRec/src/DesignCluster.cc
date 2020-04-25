// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Cluster/DesignCluster.cc"

#include "Ravl/PatternRec/DesignCluster.hh"
#include "Ravl/HSet.hh"
#include "Ravl/Math.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/PatternRec/ClassifierNearestNeighbour.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/VirtualConstructor.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //! Constructor

  DesignClusterBodyC::DesignClusterBodyC()
  {}

  //: Load from stream.
  
  DesignClusterBodyC::DesignClusterBodyC(istream &strm)
    : DesignClassifierUnsupervisedBodyC(strm)
  {}
  
  //: Load from binary stream.
  
  DesignClusterBodyC::DesignClusterBodyC(BinIStreamC &strm)
    : DesignClassifierUnsupervisedBodyC(strm)
  {}
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClusterBodyC::Save (ostream &out) const {
    return DesignClassifierUnsupervisedBodyC::Save(out);
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignClusterBodyC::Save (BinOStreamC &out) const {
    return DesignClassifierUnsupervisedBodyC::Save(out);
  }
  
  
  SArray1dC<MeanCovarianceC> DesignClusterBodyC::Cluster(const SampleC<VectorC> &in) {
    RavlAssertMsg(0,"DesignClusterBodyC::Cluster(), Abstract method called. ");
    return SArray1dC<MeanCovarianceC>();
  }
  
  SArray1dC<MeanCovarianceC> DesignClusterBodyC::Cluster(const SampleC<VectorC> &in, const SampleC<RealT> & weights) {
    RavlAssertMsg(0,"DesignClusterBodyC::Cluster(), Abstract method called. ");
    return SArray1dC<MeanCovarianceC>();
  }

  //: Load from stream.
  
  DesignClusterC::DesignClusterC(istream &strm)
    : DesignClassifierUnsupervisedC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignClusterBodyC))
  {}
  
  //: Load from binary stream.
  
  DesignClusterC::DesignClusterC(BinIStreamC &strm)
    : DesignClassifierUnsupervisedC(RAVL_VIRTUALCONSTRUCTOR(strm,DesignClusterBodyC))
  {}


  

  
}
