// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GPInstLoadConst_HH
#define RAVL_GENETIC_GPInstLoadConst_HH 1
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstruction.hh"
#include "Ravl/Genetic/GPVariable.hh"
#include "Ravl/Genetic/GeneFactory.hh"

namespace RavlN { namespace GeneticN {

  //! An environment for an agent.

  template<typename DataT>
  class GPInstLoadConstC
   : public GPInstructionC
  {
  public:
    //! Factory constructor
    GPInstLoadConstC(const GeneFactoryC &factory)
    {
      factory.Get("value",m_value);
      factory.Get("variable",m_variable,*GPVariableC<DataT>::GeneType());
    }

    //! Constructor
    GPInstLoadConstC()
    {}

    //! Access gene type for instructions
    static GeneTypeC::RefT &GeneType() {
      static GeneTypeC::RefT x = new GeneTypeClassC(typeid(typename GPInstLoadConstC<DataT>::RefT));
      return x;
    }

    //! Execute instruction
    virtual bool Execute(GPExecutionContextC &context) {
      m_variable->Set(m_value);
      return true;
    }

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPInstLoadConstC<DataT> > RefT;

  protected:
    DataT m_value;
    typename GPVariableC<DataT>::RefT m_variable;
  };


}}

#endif
