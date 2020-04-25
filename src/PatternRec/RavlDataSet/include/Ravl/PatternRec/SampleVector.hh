// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SAMPLEVECTOR_HEADER
#define RAVL_SAMPLEVECTOR_HEADER 1
//! author="Kieron Messer"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleVector.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/Vector.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/PatternRec/FieldInfo.hh"

namespace RavlN {

  class FuncMeanProjectionC;
  class FuncLinearC;

  enum DataSetNormaliseT {
    DATASET_NORMALISE_NONE=0,
    DATASET_NORMALISE_MEAN=1,
    DATASET_NORMALISE_SCALE=2
  };

  class MatrixRUTC;
  class MeanCovarianceC;  
  
  //! userlevel=Normal
  //: Sample of vectors.
  // This class expects all the vectors in the sample to be of the same
  // size.
  
  class SampleVectorC
    : public SampleC<VectorC>
  {
    
  public:
    SampleVectorC(SizeT maxSize=10, const SArray1dC<FieldInfoC> & fieldInfo = SArray1dC<FieldInfoC>())
      : SampleC<VectorC>(maxSize),
        m_fieldInfo(fieldInfo)
    {}
    //: Create a sample of data with a maximum size
    
    SampleVectorC(const SArray1dC<VectorC> & dat, const SArray1dC<FieldInfoC> & fieldInfo = SArray1dC<FieldInfoC>())
      : SampleC<VectorC>(dat),
        m_fieldInfo(fieldInfo)
    {}
    //: Create a sample of data from an array
    
    SampleVectorC(const SampleC<VectorC> &svec, const SArray1dC<FieldInfoC> & fieldInfo = SArray1dC<FieldInfoC>())
      : SampleC<VectorC>(svec),
        m_fieldInfo(fieldInfo)
    {}
    //: Construct from base class.

    SampleVectorC(const SampleC<TVectorC<float> > &svec, const SArray1dC<FieldInfoC> & fieldInfo = SArray1dC<FieldInfoC>());
    //: Construct from a sample of floats.

    SampleVectorC(const SampleC<VectorC> &svec, const SArray1dC<IndexC> &featureSet, const SArray1dC<FieldInfoC> & fieldInfo = SArray1dC<FieldInfoC>());
    //: Construct a sample set with a reduced set of features
    //!param: svec       - a sample of vectors
    //!param: featureSet - the indexes of features to keep

    SampleVectorC(const MeanCovarianceC & meanCovariance);
    //: Construct a dataset with random values using the statistics and number of samples specified.  Note the actual mean and covariance will differ slighty
    //!param meanCovariance The desired mean and covariance to use to generate the dataset
    
    SampleVectorC(const XMLFactoryContextC & factory);
    //: Construct a dataset from a factory
    //!param mean the mean vector
    //!param covariance the covariance matrix of the data
    //!param samples the number of samples to generate

    SampleVectorC Copy (void) const
    { return SampleVectorC(static_cast<DArray1dC<VectorC> >(*this).Copy(),FieldInfo()); }
    //: Make a copy of this object

    UIntT VectorSize() const;
    //: Get the size of vectors in this sample.
    
    VectorC Mean() const;
    //: Find the mean vector of the sample.
    
    VectorC Mean(const SampleC<RealT> &weights) const;
    //: Find the weighted mean vector of the sample.
    
    MeanCovarianceC MeanCovariance(bool sampleStatistics = true) const;
    //: Find the mean and covariance of the sample
    
    MeanCovarianceC MeanCovariance(const SampleC<RealT> &weights,bool sampleStatistics = true) const;
    //: Find the mean and covariance of a weighted sample
    
    MatrixRUTC SumOuterProducts() const;
    //: Compute the sum of the outerproducts.
    
    MatrixC TMul(const SampleC<VectorC> &sam2) const;
    //: Compute the sum of the outerproducts.
    // sam2 must have the same size as this sample vector.
    
    MatrixRUTC SumOuterProducts(const SampleC<RealT> &w) const;
    //: Compute the sum of the outerproducts weighting each with the corresponding value from 'w'.
    
    MatrixC TMul(const SampleC<VectorC> &sam2,const SampleC<RealT> &w) const;
    //: Compute the sum of the outerproducts weighting each with the corresponding value from 'w'.
    // sam2 must have the same size as this sample vector.
    
    FunctionC Normalise(DataSetNormaliseT normType);
    //: Compute the normalisation function using the current data, normalise the data IP and return function

    void Normalise(const MeanCovarianceC & stats);
    //: Normalises the input vectors using given stats, in place
    // In order to achieve zero mean and unity variance this function should be
    // called with the return value from MeanCovariance. Subsequent data sets can
    // then be normalised the same way by recording the MeanCovarianceC returned by
    // MeanCovariance.
    
    void Normalise(const FunctionC & func);
    //: Apply function in place.  Typically used for normalising data.

    void UndoNormalisation(const MeanCovarianceC & stats);
    //: Undo the normalisation done by 'Normalise()', in place.

    void Normalise(const MeanCovarianceC & stats,SampleVectorC &sampleVector,bool addBiasElement = false) const;
    //: Normalises the input vectors using given stats, append results to sampleVector
    // In order to achieve zero mean and unity variance this function should be
    // called with the return value from MeanCovariance. Subsequent data sets can
    // then be normalised the same way by recording the MeanCovarianceC returned by
    // MeanCovariance.
    
    void UndoNormalisation(const MeanCovarianceC & stats,SampleVectorC &sampleVector,bool removeBiasElement = false) const;
    //: Undo the normalisation done by 'Normalise()', append results to sampleVector

    FuncMeanProjectionC NormalisationFunction(const MeanCovarianceC & stats) const;
    //: Get the function used for the normalisation

    FuncLinearC UndoNormalisationFunction(const MeanCovarianceC & stats) const;
     //: Get the function used to un-normalise the data


    void Scale(FuncLinearC & func);
    //: Compute function that scales each dimension between 0 and 1 and return function created to do this
    //: !param: func The function that performs the scaling

    const SArray1dC<FieldInfoC> & FieldInfo() const {
      return m_fieldInfo;
    }
    //: Access information about the fields

    bool SetFieldInfo(const SArray1dC<FieldInfoC> & fieldInfo);
    //: Set the field information

  protected:
    SArray1dC<FieldInfoC>m_fieldInfo;
    //: The information about each column in the sample set

  }; 
  
}

#endif
