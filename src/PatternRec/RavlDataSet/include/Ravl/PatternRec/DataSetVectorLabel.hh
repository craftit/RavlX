// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASETVECTORLABEL_HEADER
#define RAVL_DATASETVECTORLABEL_HEADER 1
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSetVectorLabel.hh"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! author="Charles Galambos"

#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/SampleStreamVectorLabel.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {

  class DataSetVectorLabelC;

  //! userlevel=Develop
  //: Data set of labelled vectors.
  
  class DataSetVectorLabelBodyC 
    : public DataSet2BodyC<SampleVectorC,SampleLabelC>
  {
  public:
    DataSetVectorLabelBodyC(const SampleVectorC & vec,
			    const SampleLabelC & lab)
      : DataSet2BodyC<SampleVectorC,SampleLabelC>(vec,lab)
    {}
    //: Constructor

    DataSetVectorLabelBodyC(SampleStreamVectorLabelC & sampleStream);
    //: Construct from a sample stream. Note if dataset is large it will copy it all into memory

    DataSetVectorLabelBodyC(const SArray1dC<MeanCovarianceC> & stats);
    //: Construct a dataset using the statistics given for each class

    DataSetVectorLabelBodyC(const XMLFactoryContextC & factory);
    //: Construct a dataset

    SArray1dC<SampleVectorC> SeperateLabels() const;
    //: Create a separate sample for each label.

    VectorC GlobalMean() const;
    //: returns mean of the input vectors

    SArray1dC<VectorC> ClassMeans () const;
    //: Returns mean of input vectors for each label value
  
    DataSetVectorLabelC ClassMeansLabels () const;
    //: Returns mean of input vectors for each label along with the label

    SArray1dC<UIntT> ClassNums () const;
    //: Returns array containing the count of each label value
    
    SArray1dC<MeanCovarianceC> ClassStats (bool sampleStatistics = true) const;
    //: Returns mean and covariance of input vectors for each label value
    //!param: sampleStatistics - When true compute statistics as a sample of a random variable. (Normalise covariance by n-1 )
    
    MatrixC BetweenClassScatter () const;
    //: Returns between class scatter (covariance) matrix
    
    MatrixC WithinClassScatter (bool sampleStatistics = false) const;
    //: Returns within class scatter (covariance) matrix
    //!param: sampleStatistics - When true compute statistics as a sample of a random variable. (Normalise covariance by n-1 )
    
    DataSetVectorLabelC ExtractPerLabel(UIntT numSamples) const;
    //: Extracts numSamples samples per label
    
    StringC Info() const;
    //: Get a string containing size of the sample set and the number of samples with each label.

  };
  
  //! userlevel=Normal
  //: Data set of labelled vectors.
  
  class DataSetVectorLabelC 
    : public DataSet2C<SampleVectorC,SampleLabelC>
  {
  public:
    DataSetVectorLabelC()
    {}
    //: Default constructor.

    DataSetVectorLabelC(const DataSet2C<SampleVectorC,SampleLabelC> &dataSet)
     : DataSet2C<SampleVectorC,SampleLabelC>(dataSet)
    {}
    //: base class constructor

    DataSetVectorLabelC(UIntT sizeEstimate)
      : DataSet2C<SampleVectorC,SampleLabelC>(sizeEstimate)
    {}
    //: Construct from an estimate of the size.
    
    DataSetVectorLabelC(const SampleVectorC & vec,const SampleLabelC & lab)
      : DataSet2C<SampleVectorC,SampleLabelC>(*new DataSetVectorLabelBodyC(vec,lab))
    {}
    //: Constructor

    DataSetVectorLabelC(const SArray1dC<MeanCovarianceC> & stats)
      : DataSet2C<SampleVectorC,SampleLabelC>(*new DataSetVectorLabelBodyC(stats))
    {}
    //: Constructor

    DataSetVectorLabelC(const XMLFactoryContextC & factory)
      : DataSet2C<SampleVectorC,SampleLabelC>(*new DataSetVectorLabelBodyC(factory))
    {}
    //: Constructor


    
  protected:
    DataSetVectorLabelBodyC &Body()
    { return static_cast<DataSetVectorLabelBodyC &>(DataSetBaseC::Body()); }
    //: Access body.
    
    const DataSetVectorLabelBodyC &Body() const
    { return static_cast<const DataSetVectorLabelBodyC &>(DataSetBaseC::Body()); }
    //: Access body.
    
  public:
    SArray1dC<SampleVectorC> SeperateLabels() const
    { return Body().SeperateLabels(); }
    //: Create a separate sample for each label.

    VectorC GlobalMean() const
    { return Body().GlobalMean(); }     
    //: returns mean of the input vectors

    SArray1dC<VectorC> ClassMeans () const
    { return Body().ClassMeans(); }
    //: Returns mean of input vectors for each label value

    DataSetVectorLabelC ClassMeansLabels () const
    { return Body().ClassMeansLabels(); }
    //: Returns mean of input vectors for each label along with the label

    SArray1dC<UIntT> ClassNums () const
    { return Body().ClassNums(); }
    //: Returns array containing the count of each label value

    SArray1dC<MeanCovarianceC> ClassStats (bool sampleStatistics = true) const
    { return Body().ClassStats(sampleStatistics); }
    //: Returns mean and covariance of input vectors for each label value

    MatrixC BetweenClassScatter () const
    { return Body().BetweenClassScatter (); }
    //: Returns between class scatter (covariance) matrix
    // This assumes the prior probabilities of the classes are reflected in 
    // the number of samples in each set.
    
    MatrixC WithinClassScatter (bool sampleStatistics = false) const
    { return Body().WithinClassScatter (sampleStatistics); }
    //: Returns within class scatter (covariance) matrix
    // This assumes the prior probabilities of the classes are reflected in 
    // the number of samples in each set.
    
    DataSetVectorLabelC ExtractPerLabel(UIntT numSamples) const
    { return Body().ExtractPerLabel(numSamples); }
    //: Extracts numSamples samples per label

    StringC Info() const
    { return Body().Info(); }
    //: Get a string containing size of the sample set and the number of samples with each label.

  };

  DataSetVectorLabelC CreateDataSet(UIntT dimension = 2, UIntT classes = 2, UIntT samplesPerClass = 1000, RealT dist = 3.0);
  //: Create a data set containing random samples - useful for testing things

}


#endif
