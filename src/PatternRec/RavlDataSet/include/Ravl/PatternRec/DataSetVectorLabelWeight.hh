// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DATASETVECTORLABELWEIGHT_HEADER
#define RAVL_DATASETVECTORLABELWEIGHT_HEADER 1
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSetVectorLabelWeight.hh"
//! docentry="Ravl.API.Pattern Recognition.Data Set"
//! author="Charles Galambos"

#include "Ravl/PatternRec/DataSet3.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"

namespace RavlN {
  
  class DataSetVectorLabelWeightC;
  
  //! userlevel=Develop
  //: Data set of labeled vectors.
  
  class DataSetVectorLabelWeightBodyC 
    : public DataSet3BodyC<SampleVectorC,SampleLabelC,SampleC<RealT> >
  {
  public:
    DataSetVectorLabelWeightBodyC(const SampleVectorC & vec,
				  const SampleLabelC & lab,
				  const SampleC<RealT> &weight
				  )
      : DataSet3BodyC<SampleVectorC,SampleLabelC,SampleC<RealT> >(vec,lab,weight)
    {}
    //: Constructor
    
    SArray1dC<DataSet2C<SampleVectorC,SampleC<RealT> > > SeperateLabels() const;
    //: Create a seperate sample for each label.

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
    
    MatrixC BetweenClassScatter () const;
    //: Returns between class scatter (covariance) matrix
    
    MatrixC WithinClassScatter (bool sampleStatistics = false) const;
    //: Returns within class scatter (covariance) matrix
    
    DataSetVectorLabelWeightC ExtractPerLabel(UIntT numSamples) const;
    //: Extracts numSamples samples per label
    
  };
  
  //! userlevel=Normal
  //: Data set of labeled vectors.
  
  class DataSetVectorLabelWeightC 
    : public DataSet3C<SampleVectorC,SampleLabelC,SampleC<RealT> >
  {
  public:
    DataSetVectorLabelWeightC()
    {}
    //: Default constructor.

    DataSetVectorLabelWeightC(UIntT sizeEstimate)
      : DataSet3C<SampleVectorC,SampleLabelC,SampleC<RealT> >(sizeEstimate)
    {}
    //: Construct from an estimate of the size.
    
    DataSetVectorLabelWeightC(const SampleVectorC & vec,const SampleLabelC & lab,const SampleC<RealT> &weights)
      : DataSet3C<SampleVectorC,SampleLabelC,SampleC<RealT> >(*new DataSetVectorLabelWeightBodyC(vec,lab,weights))
    {}
    //: Constructor
    
  protected:
    DataSetVectorLabelWeightBodyC &Body()
    { return static_cast<DataSetVectorLabelWeightBodyC &>(DataSetBaseC::Body()); }
    //: Access body.
    
    const DataSetVectorLabelWeightBodyC &Body() const
    { return static_cast<const DataSetVectorLabelWeightBodyC &>(DataSetBaseC::Body()); }
    //: Access body.
    
  public:
    SArray1dC<DataSet2C<SampleVectorC,SampleC<RealT> > > SeperateLabels() const
    { return Body().SeperateLabels(); }
    //: Create a seperate sample for each label.
    
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

    MatrixC WithinClassScatter (bool sampleStatistics = false) const
    { return Body().WithinClassScatter (sampleStatistics ); }
    //: Returns within class scatter (covariance) matrix
    
    DataSetVectorLabelWeightC ExtractPerLabel(UIntT numSamples) const
    { return Body().ExtractPerLabel(numSamples); }
    //: Extracts numSamples samples per label

    
  };
}


#endif
