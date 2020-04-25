// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNKOHONENNET_HEADER
#define RAVL_DESIGNKOHONENNET_HEADER  1
////////////////////////////////////////////////////////////////////////////
//! author="Kieron Messer"
//! lib=RavlPatternRec
//! date="3/6/1998"
//! userlevel=Develop
//! file="Ravl/PatternRec/Cluster/DesignKohonenNet.hh"
//! docentry="Ravl.API.Pattern Recognition.Cluster"

#include "Ravl/PatternRec/DesignCluster.hh"
#include "Ravl/PatternRec/DistanceSqrEuclidean.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/Index2d.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Kohonen network clustering implementation class.
  // Implementation class for Kohonen network clustering algorithm.
  
  class DesignKohonenNetBodyC
    : public DesignClusterBodyC
  {
  public:
    DesignKohonenNetBodyC (UIntT Sx, UIntT Sy, UIntT Cycles, const DistanceC &distance);
    // Constructor
    //!param: Sx - specifies x-dimension of grid
    //!param: Sy - specifies y-dimension of grid
    //!param: Cycles - how iterations of the training algorithm to perform
    //!param: distance   - metric to use when measuring sample distance from class centres

    DesignKohonenNetBodyC (istream &in);
    //: Constructs from stream
    
    DesignKohonenNetBodyC (const DesignKohonenNetBodyC &oth);
    //: Copy Constructor
    
    virtual RCBodyVC & Copy() const; 
    //: Makes a deep copy and is virtual
    
    //virtual VectorC Evaluate (const VectorC &X) const;
    //: Evaluate Y=f(X) 
    
    virtual UIntT Classify (const VectorC &X) const;
    //: Evaluate c=f(X)
    
    virtual void Initialise (UIntT Sx, UIntT Sy, const SampleC<VectorC> &trainX);
    //: Initialises the clusters
    //!param: Sx - x-dimension of grid
    //!param: Sy - y-dimension of grid
    //!param: trainX - training samples used to determine initial centres

    virtual FunctionC Apply(const SampleC<VectorC> &trainX);
    //: Train the unsupervised classifier on a list of feature vectors
    
    virtual SArray1dC<MeanCovarianceC> Cluster(const SampleC<VectorC> &in);
    //: Compute cluster means.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor

    void NewNeighbourhood(UIntT neighbours);
    //: Set a new number of neighbours
    
    friend class DesignKohonenNetC;
    //: Handle class
    
  protected:
    bool Train(const SampleC<VectorC> &in);
    //: Train network.
    
    Array2dC<VectorC> _weights;
    //: Cluster centres
    
    DistanceC _distance;
    //: Metric for measuring distance between samples and cluster centres

    UIntT _Sx, _Sy;
    //: Size of the grid
  
    RealT _alpha;
    //: Learning parameter

    Array1dC<Index2dC> _labels;
    //: Look up table of grid points to get from table to grid

    UIntT _neighbours;
    //: Size of neighbourhood to update
    
    Array1dC<Index2dC> _rangeX;
    //: Minimum index of neighbourhood
  
    Array1dC<Index2dC> _rangeY;
    //: Maximum index of neighbourhood

    UIntT _cycles;
    //: Number of iterations through the complete dataset
  
    UIntT _cyclesSoFar;
    //: Internal counter of total number of steps  
    
    bool _initialised;
    //: Have we initialised yet?
  };


  //: Kohonen network clustering algorithm
  //
  // Unsupervised classification algorithm which performs Kohonen network clustering. It
  // is necessary to specify the number of clusters as well as a distance metric
  // to use. Initial class centres are randomly chosen from the training set.
  
  class DesignKohonenNetC
    : public DesignClusterC
  {
  public:
    DesignKohonenNetC (UIntT Sx, UIntT Sy, UIntT Cycles, const DistanceC &distance)
      : DesignClusterC(*(new DesignKohonenNetBodyC(Sx, Sy, Cycles, distance)))
    {}
    //: Constructor
    //!param: Sx - x-dimension of grid
    //!param: Sy - y-dimension of grid
    //!param: Cycles - number of iterations of clustering algorithm
    //!param: distance - metric to use when measuring distance to class centres
    
  protected:
    DesignKohonenNetC(DesignKohonenNetBodyC &bod)
      : DesignClusterC(bod)
    {}
    //: Body constructor.
    
    DesignKohonenNetBodyC &Body()
    { return static_cast<DesignKohonenNetBodyC &>(DesignerC::Body()); }
    //: Access body.
    
    const DesignKohonenNetBodyC &Body() const
    { return static_cast<const DesignKohonenNetBodyC &>(DesignerC::Body()); }
    //: Access body.
    
  public:
    //FunctionC Apply(const SampleC<VectorC> &trainX) 
    //{ return Body().Cluster(trainX);}
    //: Learn the dataset a specified number of cycles  
  };
  
}

#endif
