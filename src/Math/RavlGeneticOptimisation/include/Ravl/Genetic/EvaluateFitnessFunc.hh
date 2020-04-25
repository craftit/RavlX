// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author=Charles Galambos
//! lib=RavlGeneticOptimisation
//! docentry=Ravl.API.Math.Genetic.Optimisation

#ifndef RAVL_GENETIC_EVALUATEFITNESSFUNC_HH_
#define RAVL_GENETIC_EVALUATEFITNESSFUNC_HH_

#include "Ravl/Genetic/EvaluateFitness.hh"

namespace RavlN {  namespace GeneticN {

  //! Abstract class for evaluating the fitness of an object.

  template<typename DataT>
  class EvaluateFitnessFuncC
   : public EvaluateFitnessC
  {
  public:
    //! Constructor
    EvaluateFitnessFuncC()
     : m_func(0)
    {}

    //! Constructor
    EvaluateFitnessFuncC(float (*func)(DataT &data))
     : m_func(func)
    {}

    //! Is this fitness IO capable ?
    virtual bool CanSave() const
    { return false; }

    //! Copy me.
    virtual RavlN::RCBodyVC &Copy() const
    { return *new EvaluateFitnessFuncC(*this); }

    //! Access type of object evaluated for fitness.
    virtual const std::type_info &ObjectType() const
    { return typeid(DataT); }

    //! Evaluate the fit
    virtual bool Evaluate(int generation,RCWrapAbstractC &obj,float &score)
    {
      RavlN::RCWrapC<DataT> wrap(obj,true);
      if(!wrap.IsValid()) {
        return false;
      }
      score = m_func(wrap.Data());
      return true;
    }

    //! Handle to this class.
    typedef RavlN::SmartOwnerPtrC<EvaluateFitnessFuncC<DataT> > RefT;

    //! Handle to class
    typedef RavlN::SmartCallbackPtrC<EvaluateFitnessFuncC<DataT> > CBRefT;

  protected:
    virtual void ZeroOwners()
    { EvaluateFitnessC::ZeroOwners(); }
    //: Called when owner handles drop to zero.

    float (*m_func)(DataT &);
  };
}}

#endif /* EVALUATOR_HH_ */
