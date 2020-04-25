// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Cluster/DesignFuzzyCMeansCluster.cc"

#include "Ravl/PatternRec/DesignFuzzyCMeansCluster.hh"
#include "Ravl/PatternRec/ClassifierFuzzyCMeans.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/Random.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/PatternRec/SampleIter.hh"

namespace RavlN {
  
  DesignFuzzyCMeansClusterBodyC::DesignFuzzyCMeansClusterBodyC (UIntT numClasses, const DistanceC &distance, RealT m)
    : sizeY(numClasses),
      sizeX(0),
      _distance(distance),
      _m(m),
      _initialised(false)
  {}
  
  DesignFuzzyCMeansClusterBodyC::DesignFuzzyCMeansClusterBodyC (istream &in)
    : DesignClusterBodyC(in),
      sizeY(0),
      sizeX(0),
      _m(0),
      _initialised(false)
  { in >> _centres >> _distance >> _m; }

  DesignFuzzyCMeansClusterBodyC::DesignFuzzyCMeansClusterBodyC (const DesignFuzzyCMeansClusterBodyC &oth)
    : DesignClusterBodyC(oth),
      sizeY(oth.sizeY),
      sizeX(oth.sizeX),
      _centres(oth._centres.Copy()),
      _distance(oth._distance),
      _m(oth._m),
      _initialised(false)
  {}
  
  RCBodyVC & DesignFuzzyCMeansClusterBodyC::Copy () const
  { return *(new DesignFuzzyCMeansClusterBodyC (*this)); }
  
  void DesignFuzzyCMeansClusterBodyC::Initialise (UIntT numClasses, const SampleC<VectorC> &trainX)
  {
    {
      SampleIterC<VectorC> e1 (trainX);
      if(!e1) return ; // No samples.
      sizeX = e1->Size();
    }
    // First assign random class membership for each sample
    SArray1dC<VectorC> outU (trainX.Size ());
    
    for(SArray1dIterC<VectorC> i1(outU);i1;i1++) {
      *i1 = VectorC (sizeY); 
      RealT running = 1.0;                              // Random initial vals
      for (IndexC j = 1; j < i1->Size(); j++) {
	(*i1)[j] = Random1 () * running;
	running -= (*i1)[j];
      }
      (*i1)[0] = running;
    }
    // Next determine corresponding class centres
    _centres = SArray1dC<VectorC> (sizeY);
    for(SArray1dIterC<VectorC> i2(_centres);i2;i2++) {  // Calc class centres
      ;
      VectorC numerator (sizeX); 
      numerator.Fill(0);
      RealT denominator = 0.0;
      IndexC i3 = 0;
      for (SampleIterC<VectorC> e1 (trainX); e1; e1++,i3++) {
	numerator += e1.Data() * Pow (outU[i3][i2.Index()],_m);
	denominator += Pow (outU[i3][i2.Index()],_m);
      }
      *i2 = numerator * (1.0/denominator);
    }
  }

  FunctionC DesignFuzzyCMeansClusterBodyC::Apply (const SampleC<VectorC> &trainX)
  {
    if (_initialised != true) 
      Initialise (sizeY,trainX);
    RealT trainXsize = trainX.Size();
    SArray1dC<VectorC> outU ((SizeT) trainXsize);
    for(SArray1dIterC<VectorC> it(outU);it;it++)
      *it = VectorC (sizeY); 
    RealT cost = 0.0,oldcost;
    int counter = 0;
    do {
      oldcost = cost;
      cost = 0.0;
      IndexC i4 = 0;
      for (SampleIterC<VectorC> e2 (trainX); e2.IsElm(); e2.Next(),i4++) {
	for(Array1dIterC<VectorC> it1(_centres);it1;it1++) { // Calc class membership
	  RealT value = 0.0;
	  for(Array1dIterC<VectorC> it2(_centres);it2;it2++) 
	    value += Pow (_distance.Magnitude (e2.Data() - *it1) /
			  _distance.Magnitude (e2.Data() - *it2),2.0/(_m-1.0));
	  value = 1.0/value;
	  outU[i4][it1.Index()] = value;
	  cost += Pow (value,_m) * Pow(_distance.Magnitude (e2.Data() - *it1),2.0);
	}
      }
      cost /= trainXsize;
      for(Array1dIterC<VectorC> it3(_centres);it3;it3++) { // i2
	// Calc class centres
	VectorC numerator (sizeX); numerator.Fill (0);
	RealT denominator = 0.0;
	IndexC i3 = 0;
	for (SampleIterC<VectorC> e1 (trainX); e1.IsElm(); e1.Next(),i3++) {
	  numerator += e1.Data() * Pow (outU[i3][it3.Index()],_m);
	  denominator += Pow (outU[i3][it3.Index()],_m);
	}
	*it3 = numerator * (1.0/denominator);
      }
    } while (counter++ < 1 || (oldcost - cost > 1e-6 && counter < 100));
    
    return ClassifierFuzzyCMeansC(_centres,_m,_distance);
  }

  
  bool DesignFuzzyCMeansClusterBodyC::Save (ostream &out) const
  {
    if(!DesignClusterBodyC::Save (out))
      return false;
    out << _centres << endl << _distance << _m << endl;
    return true;
  }

}
