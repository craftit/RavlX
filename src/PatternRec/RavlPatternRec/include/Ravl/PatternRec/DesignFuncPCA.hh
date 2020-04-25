// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DESIGNFUNCPCA_HEADER
#define RAVL_DESIGNFUNCPCA_HEADER 1
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Dimension Reduction"
//! file="Ravl/PatternRec/Modeling/DimensionReduction/DesignFuncPCA.hh"

#include "Ravl/PatternRec/DesignFuncReduce.hh"
#include "Ravl/Matrix.hh"

namespace RavlN {
  class MeanCovarianceC;
  
  //! userlevel=Develop
  //: Design a dimension reducing function using Principal Component Analysis (PCA)
  class DesignFuncPCABodyC
    : public DesignFuncReduceBodyC
  {
  public:
    DesignFuncPCABodyC(RealT variationPreserved = 0.98);
    //: Default constructor.
    // "variationPreserved" is amount of variation to attempt to preserve in reduced set.
    // 0 -> None; 1 -> All; >1 (truncated to int) -> Size of set preserved.
    
    DesignFuncPCABodyC(istream &strm);
    //: Load from stream.
    
    DesignFuncPCABodyC(BinIStreamC &strm);
    //: Load from binary stream.
    
    virtual bool Save (ostream &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor
    
    virtual FunctionC Apply(const SampleC<VectorC> &in);
    //: Create function from the given data.
    // The actual class returned is <a href="RavlN.FuncMeanProjectionC.html">FuncMeanProjectionC</a>

    virtual FunctionC Apply(SampleStreamC<VectorC> &in);
    //: Create function from the given data.
    // The actual class returned is <a href="RavlN.FuncMeanProjectionC.html">FuncMeanProjectionC</a>
    
    virtual FunctionC Apply(const SampleC<VectorC> &in,const SampleC<RealT> &weight);
    //: Create function from the given data, and sample weights.
    
    VectorMatrixC &Pca()
    { return pca; }
    //: Access eigen vectors and values.

    const VectorMatrixC &Pca() const
    { return pca; }
    //: Access eigen vectors and values.
    
    VectorC &Mean()
    { return mean; }
    //: Access mean vector.

    const VectorC &Mean() const
    { return mean; }
    //: Access mean vector.
    
  protected:
    
    FunctionC DesignHighDim(const SampleC<VectorC> &sample,RealT variation);
    //: Design the transform.
    
    FunctionC DesignLowDim(const SampleC<VectorC> &sample,RealT variation);
    //: Design the transform.
    
    FunctionC DesignLowDim(const MeanCovarianceC &meanCovar);
    //: Build pca from mean and covariance.
    
    VectorC mean; // Last mean vector.
    VectorMatrixC pca; // Last set of eigen values and vectors.
    
    bool forceHimDim;
  };
  
  //! userlevel=Normal
  //: Design a dimension reducing function using Principal Component Analysis (PCA) <br>
  // Use the <code>variationPreserved</code> argument in the constructor to control the proportion of dimensions to be kept / discarded.<br>
  // The actual class returned by the <code>Apply()</code> methods is <a href="RavlN.FuncMeanProjectionC.html">FuncMeanProjectionC</a>, which can be used to project data from the original space into the subspace. It is possible
  // to obtain the projection matrix from this class if needed.
  
  class DesignFuncPCAC
    : public DesignFuncReduceC
  {
  public:
    DesignFuncPCAC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DesignFuncPCAC(RealT variationPreserved)
      : DesignFuncReduceC(*new DesignFuncPCABodyC(variationPreserved))
    {}
    //: Constructor 
    // "variationPreserved" is amount of variation to attempt to preserve in reduced set:<br>
    // 0 -> None; 1 -> All; >1 (truncated to int) -> Size of set preserved.
    
  protected:
    DesignFuncPCAC(DesignFuncPCABodyC &bod)
      : DesignFuncReduceC(bod)
    {}
    //: Body constructor.
    
    DesignFuncPCABodyC &Body()
    { return static_cast<DesignFuncPCABodyC &>(DesignerC::Body()); }
    //: Access body.
    
    const DesignFuncPCABodyC &Body() const
    { return static_cast<const DesignFuncPCABodyC &>(DesignerC::Body()); }
    //: Access body.
    
  public:    
    VectorMatrixC &Pca()
    { return Body().Pca(); }
    //: Access eigen vectors and values.

    const VectorMatrixC &Pca() const
    { return Body().Pca(); }
    //: Access eigen vectors and values.
    
    VectorC &Mean()
    { return Body().Mean(); }
    //: Access mean of input vectors.
    
    const VectorC &Mean() const
    { return Body().Mean(); }
    //: Access mean of input vectors.
  };

}



#endif
