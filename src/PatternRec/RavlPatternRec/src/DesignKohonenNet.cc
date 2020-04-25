// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Cluster/DesignKohonenNet.cc"

#include "Ravl/PatternRec/DesignKohonenNet.hh"
#include "Ravl/PatternRec/ClassifierNearestNeighbour.hh"
#include "Ravl/StdMath.hh"
#include "Ravl/Random.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/PatternRec/SampleIter.hh"

namespace RavlN {
  
  DesignKohonenNetBodyC::DesignKohonenNetBodyC (UIntT Sx, UIntT Sy, UIntT cycles, const DistanceC &distance)
    : _distance(distance), 
      _labels(Sx*Sy),
      _neighbours(0),
      _rangeX(Sx*Sy), 
      _rangeY(Sx*Sy),
      _initialised(false)
  {
    _Sx = Sx;
    _Sy = Sy;
    _alpha = 1.0;
    _cycles = cycles;
    _cyclesSoFar = 0;
    
    // Set up the look up table
    for(UIntT i=0;i<Sy;i++)
      for(UIntT j=0;j<Sx;j++)
	_labels[(i*Sx)+j].Set(i, j);
    
    // Need to set a sensible value for the number of neighbours
    if(_Sx>_Sy) 
      _neighbours = _Sx/2 + 1;
    else _neighbours = _Sy/2 + 1;
    
    if(_neighbours>_cycles) 
      _neighbours = _cycles;
    
    NewNeighbourhood(_neighbours);
  }

  DesignKohonenNetBodyC::DesignKohonenNetBodyC (istream &in)
    : DesignClusterBodyC(in),
      _Sx(0),
      _Sy(0),
      _alpha(1.0),
      _neighbours(0),
      _cycles(0),
      _cyclesSoFar(0),
      _initialised(false)
  {
    in >> _weights >> _distance >> _labels >> _rangeX >> _rangeY >> _Sx >> _Sy >> _neighbours; 
  }

  DesignKohonenNetBodyC::DesignKohonenNetBodyC (const DesignKohonenNetBodyC &oth)
    : DesignClusterBodyC(oth),
      _weights(oth._weights.Copy()),
      _distance(oth._distance),
      _labels(oth._labels.Copy()),
      _neighbours(oth._neighbours),
      _rangeX(oth._rangeX.Copy()),
      _rangeY(oth._rangeY.Copy()),
      _initialised(false)
  {
    _Sx = oth._Sx;
    _Sy = oth._Sy;
    _alpha = oth._alpha;
    _labels = oth._labels;
    _cycles = oth._cycles;
    _cyclesSoFar = oth._cyclesSoFar;
  }
  
  RCBodyVC &DesignKohonenNetBodyC::Copy() const
  { return * (new DesignKohonenNetBodyC (*this)); }
  
  //  VectorC DesignKohonenNetBodyC::Evaluate (const VectorC &X) const
  //{ return LabelToVec (Classify (X),SizeY()-1); }
  
  UIntT DesignKohonenNetBodyC::Classify (const VectorC &X) const
  {
    UIntT label = 0;
    RealT minDistance = 99999.0;
    RealT thisDistance;
    for(Array2dIterC<VectorC> it(_weights);it;it++) {
      thisDistance = _distance.Magnitude (X-*it);
      if (thisDistance < minDistance) {
	minDistance = thisDistance;
	Index2dC at = it.Index();
	label = (UIntT)((at[0].V() * _Sx) + at[1].V());
      }
    }
    
    if(minDistance>=99999.0) {
      cerr << "Error: Min distance not found (DesignKohonenNetB::Classify)" << endl;
      cerr << "Try normalising data before using this algorithm" << endl;
      RavlAssert(0);
    }
    
    return label;
  }
  
  void DesignKohonenNetBodyC::Initialise (UIntT Sx, UIntT Sy, const SampleC<VectorC> &trainX)
  {
    // NumClassifyUnsupervisedBC::Initialise (Sx*Sy, trainX);
    _weights = Array2dC<VectorC> (Sy, Sx);
    SampleIterC<VectorC> it(trainX);
    VectorC mean(it.Data().Copy());
    for(;it;it++) 
      mean += *it;
    mean /= (RealT)trainX.Size();
    _cyclesSoFar = 0;
    _alpha = 1.0;
    UIntT size = mean.Size();
    // Set the initial values of the weights to random values.
    for(Array2dIterC<VectorC> it (_weights);it;it++)  
      *it = RandomVector(size,1/(RealT)RavlConstN::maxInt);
  }

  //: Train network.
  
  bool DesignKohonenNetBodyC::Train(const SampleC<VectorC> &trainX) {
    // Initialise if it has not already
    if (!_initialised) 
      Initialise (_Sx, _Sy, trainX);
    
    IntT label;
    IntT newn;
    IntT r, c;
    
    for(IntT cyc=_cycles;cyc>0;cyc--) {
    
      // Reduce the number of neighbours
      if(_neighbours!=0) {
	newn = _neighbours-1;
	NewNeighbourhood(newn);
      } else newn = 0;
      if(cyc<2) newn = 0;
      
      // Reduce the learning rate
      _alpha = 0.9 * ((RealT)cyc/(RealT)(_cycles+1));
    
      // Be verbose if you want
      //cout << "Cycle: " << cyc << "\tNeighbours: " << _neighbours << "\tLR: " << _alpha << endl;
      
      for(SampleIterC<VectorC> it(trainX);it;it++) {
	
	// First classify the sample
	label = Classify(it.Data());
	
	// Only updating winner in final stages of algorithm
	if(newn==0)
	  _weights[_labels[label]] += ((it.Data() - _weights[_labels[label]]) * _alpha);
	
	// Use smoothing kernel. i.e. update nearest neighbours as well
	else {
	  for(SizeT i=0;i<_labels.Size();i++) {
	    r = _labels[i].Row().V();
	    c = _labels[i].Col().V();
          
	    if((r>=_rangeY[label][0]) && (r<=_rangeY[label][1]))
	      if((c>=_rangeX[label][0]) && (c<=_rangeX[label][1]))
	        _weights[r][c] += ((it.Data() - _weights[r][c]) * _alpha);
	  }     
	} 
    
      } 
    }
    return true;
  }
  
  FunctionC DesignKohonenNetBodyC::Apply(const SampleC<VectorC> &trainX)
  {
    if(!Train(trainX))
      return FunctionC();
    
    // Build classifier
    
    UIntT totLabels = _weights.Frame().Area();
    SampleLabelC labs(totLabels);
    SampleVectorC vecs(totLabels);
    for(Array2dIterC<VectorC> it(_weights);it;it++) {
      vecs.Append(*it);
      Index2dC at = it.Index();
      labs.Append((UIntT)((at[0].V() * _Sx) + at[1].V()));
    }
    return ClassifierNearestNeighbourC(vecs,labs,_distance);
  }
  
  //: Compute cluster means.
  
  SArray1dC<MeanCovarianceC> DesignKohonenNetBodyC::Cluster(const SampleC<VectorC> &trainX) {
    SArray1dC<MeanCovarianceC> ret;
    if(!Train(trainX))
      return ret;
    Array2dIterC<VectorC> it(_weights); 
    if(!it)
      return ret;
    UIntT totLabels = _weights.Frame().Area();
    ret = SArray1dC<MeanCovarianceC>(totLabels);
    SArray1dIterC<MeanCovarianceC> ait(ret);
    MatrixC cov = MatrixC::Identity(it->Size());
    for(;it;it++,ait++)
      *ait = MeanCovarianceC(1,*it,cov);
    return ret;
  }
  
  bool DesignKohonenNetBodyC::Save (ostream &out) const {
    DesignClusterBodyC::Save(out);
    out << _weights << endl << _distance << endl;
    out << _labels << endl << _rangeX << endl;
    out << _rangeY << _Sx << endl << _Sy << endl << _neighbours;
    return true;
  }
  
  void
  DesignKohonenNetBodyC::NewNeighbourhood(UIntT Number)
  {
  
    _neighbours = Number;

    for(UIntT i=0;i<_Sy;i++)
      for(UIntT j=0;j<_Sx;j++) {
  
	IntT label =  (i*_Sx) + j;

	// Find the min and max range of neighbours to update
	IntT minX = j - _neighbours;
	IntT maxX = j + _neighbours;

	IntT minY = i - _neighbours;
	IntT maxY = i + _neighbours;

	_rangeX[label].Set(minX, maxX);
	_rangeY[label].Set(minY, maxY);

      }
 
    // cout << "X-RANGE" << endl << _rangeX << endl;
    // cout << "Y-RANGE" << endl << _rangeY << endl;
 
  }


}
