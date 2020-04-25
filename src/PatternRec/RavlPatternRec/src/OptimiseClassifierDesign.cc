/*
 * OptimiseClassifierDesign.cc
 *
 *  Created on: 25 Jun 2012
 *      Author: charles
 */

#include "Ravl/PatternRec/OptimiseClassifierDesign.hh"
#include "Ravl/PatternRec/OptimisePowell.hh"
#include "Ravl/PatternRec/ClassifierPreprocess.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/PatternRec/FuncMeanProjection.hh"

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //! Cost function for logistic regression.

  class CostClassifierDesignC
   : public CostBodyC
  {
  public:
     //! Constructor
     CostClassifierDesignC(DesignClassifierSupervisedC &designer,
                           ErrorC &errorMeasure,
                           DataSetVectorLabelC &trainSample,
                           DataSetVectorLabelC &crossValidateSample)
     : m_designer(designer),
       m_errorMeasure(errorMeasure),
       m_trainSample(trainSample),
       m_crossValidateSample(crossValidateSample)
    {
      VectorC min;
      VectorC max;
      VectorC defaultValues;
      SArray1dC<StringC> names;

      designer.ParameterLimits(defaultValues,min,max,names);

      ParametersC parameters(defaultValues.Size(),true);

      for(unsigned i = 0;i < defaultValues.Size();i++) {
        RavlDebug("Param: %d Min=%f Max=%f Default=%f",min[i],max[i],defaultValues[i]);
        parameters.Setup(i,min[i],max[i],1000,defaultValues[i]);
      }

      SetParameters(parameters);
    }

    //! Compute the cost of a solution.
    virtual RealT Cost (const VectorC &X) const;

  protected:
    mutable DesignClassifierSupervisedC m_designer;
    mutable ErrorC m_errorMeasure;  //!<
    DataSetVectorLabelC m_trainSample;
    DataSetVectorLabelC m_crossValidateSample;


  };


  //! Compute the cost of a solution.
  RealT CostClassifierDesignC::Cost (const VectorC &X) const {
    m_designer.SetParameters(X);
    ClassifierC classifier = m_designer.Apply(m_trainSample.Sample1(),m_trainSample.Sample2());
    RealT cost = m_errorMeasure.Error(classifier,m_crossValidateSample);
    ONDEBUG(RavlDebug(" Cost:%f  At:%s  ",cost,RavlN::StringOf(X).c_str()));
    return cost;
  }

  // ---------------------------------------------------------------------------

  //! Constructor
  OptimiseClassifierDesignBodyC::OptimiseClassifierDesignBodyC()
   : m_designFraction(0.6),
     m_crossValidateFraction(0.2)
  {
    m_errorMeasure = ErrorC();
    m_optimiser = OptimisePowellC(100,-1,false);
  }

  //! Constructor
  OptimiseClassifierDesignBodyC::OptimiseClassifierDesignBodyC(RealT designFraction)
   : m_designFraction(designFraction),
     m_crossValidateFraction(0.2)
  {
    m_errorMeasure = ErrorC();
    OptimisePowellC popt(100,-1,false);
    popt.SetVerbose(true);
    m_optimiser = popt;
  }

  //! XML factory constructor
  OptimiseClassifierDesignBodyC::OptimiseClassifierDesignBodyC(const XMLFactoryContextC &factory)
   : m_designFraction(factory.AttributeReal("designFraction",0.6)),
     m_crossValidateFraction(factory.AttributeReal("crossValidateFraction",0.2))
  {
    if(!factory.UseChildComponent("ErrorMeasure",m_errorMeasure,true,typeid(ErrorC)))
      m_errorMeasure = ErrorC();

    if(!factory.UseChildComponent("Optimiser",m_optimiser,true,typeid(OptimisePowellC))) {
      m_optimiser = OptimisePowellC(-1,100,true);
    }

  }

  //! Optimise a the design for a classifier given some data and a designer
  bool OptimiseClassifierDesignBodyC::Apply(DesignClassifierSupervisedC &designer,
                                             const SampleVectorC & x,
                                             const SampleLabelC & labels,
                                             ClassifierC &bestClassifier,
                                             VectorC &bestClassifierParams,
                                             RealT &finalResult
                                             )
  {
    DataSetVectorLabelC trainSample(x,labels);
    DataSetVectorLabelC crossValidateSample(trainSample.ExtractSample(1 - m_designFraction));

    // We only need to do normalisation once, so get that out of the way.

    MeanCovarianceC meanCovariance = trainSample.Sample1().MeanCovariance();
    FunctionC normaliseFunc = trainSample.Sample1().NormalisationFunction(meanCovariance);

    trainSample.Sample1().Normalise(meanCovariance);
    crossValidateSample.Sample1().Normalise(meanCovariance);


    RealT minimumCost;

    CostC costFunc(new CostClassifierDesignC(designer,
                              m_errorMeasure,
                              trainSample,
                              crossValidateSample));

    ONDEBUG(RavlDebug("Start at:%s",RavlN::StringOf(costFunc.StartX()).c_str()));

    bestClassifierParams = m_optimiser.MinimalX (costFunc,minimumCost);
    ONDEBUG(RavlInfo("Minimum error found:%f Parameters:%s ",minimumCost,RavlN::StringOf(bestClassifierParams).c_str()));

    //! Use the score before retraining, as it'll be less bias. Though still not perfect.
    finalResult = minimumCost;

    // Retrain on all the data.
    ClassifierC classifier = designer.Apply(x,labels);

    // Add normalisation in to make the final classifier.
    bestClassifier = ClassifierPreprocessC(normaliseFunc, classifier);

    return true;
  }

  void LinkOptimiseClassifierDesign()
  {}

  RavlN::XMLFactoryRegisterHandleConvertC<OptimiseClassifierDesignC,DesignFunctionSupervisedC>
      g_registerXMLFactoryOptimiseClassifierDesign("RavlN::OptimiseClassifierDesignC");

}

