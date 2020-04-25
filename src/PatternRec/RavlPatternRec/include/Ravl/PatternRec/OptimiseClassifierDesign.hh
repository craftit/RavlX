#ifndef RAVL_OPTIMISECLASSIFIERDESIGN_HEADER
#define RAVL_OPTIMISECLASSIFIERDESIGN_HEADER 1

#include "Ravl/PatternRec/DesignClassifierSupervised.hh"
#include "Ravl/PatternRec/Error.hh"
#include "Ravl/PatternRec/Optimise.hh"

namespace RavlN {

  //: Optimise classifier parameters for generalisation

  class OptimiseClassifierDesignBodyC
   : public DesignFunctionSupervisedBodyC
  {
  public:
    //! Constructor
    OptimiseClassifierDesignBodyC();

    //! Constructor
    OptimiseClassifierDesignBodyC(RealT designFraction);

    //! XML factory constructor
    OptimiseClassifierDesignBodyC(const XMLFactoryContextC &factory);

    using DesignFunctionSupervisedBodyC::Apply;

    //! Optimise a the design for a classifier given some data and a designer
    virtual bool Apply(DesignClassifierSupervisedC &designer,
                       const SampleVectorC & x,
                       const SampleLabelC & labels,
                       ClassifierC &bestClassifier,
                       VectorC &bestClassifierParams,
                       RealT &finalResult
                       ) ;

    typedef RavlN::SmartPtrC<OptimiseClassifierDesignBodyC> RefT;
  protected:
    OptimiseC m_optimiser;
    ErrorC m_errorMeasure;  //!<
    RealT m_designFraction; // Fraction of the training set used for design.
    RealT m_crossValidateFraction; // Fraction used for cross validation, the remaining fraction is used for testing.
  };

  //: Optimise classifier parameters for generalisation

  class OptimiseClassifierDesignC
  : public DesignFunctionSupervisedC
  {
  public:
    OptimiseClassifierDesignC()
    {}

    //: Create a designer
    OptimiseClassifierDesignC(RealT designFraction)
      : DesignFunctionSupervisedC(new OptimiseClassifierDesignBodyC(designFraction))
    {}

    //: Create a designer
    OptimiseClassifierDesignC(const XMLFactoryContextC &factory)
      : DesignFunctionSupervisedC(new OptimiseClassifierDesignBodyC(factory))
    {}

  protected:
    OptimiseClassifierDesignBodyC &Body()
    { return static_cast<OptimiseClassifierDesignBodyC &>(DesignerC::Body()); }
    //: Access body.

    const OptimiseClassifierDesignBodyC &Body() const
    { return static_cast<const OptimiseClassifierDesignBodyC &>(DesignerC::Body()); }
    //: Access body.

  public:

    bool Apply(DesignClassifierSupervisedC &designer,
                       const SampleVectorC & x,
                       const SampleLabelC & labels,
                       ClassifierC &bestClassifier,
                       VectorC &bestClassifierParams,
                       RealT &finalResult
                       )
    { return Body().Apply(designer,x,labels,bestClassifier,bestClassifierParams,finalResult); }

  };

}

#endif
