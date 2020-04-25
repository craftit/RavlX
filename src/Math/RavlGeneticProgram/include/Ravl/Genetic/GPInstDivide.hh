// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GPInstDivide_HH
#define RAVL_GENETIC_GPInstDivide_HH 1
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstruction.hh"
#include "Ravl/Genetic/GPVariable.hh"
#include "Ravl/Genetic/GeneFactory.hh"

namespace RavlN { namespace GeneticN {

  //! Add two values.

  template<typename DataT>
  class GPInstDivideC
   : public GPInstructionC
  {
  public:
    //! Factory constructor
    GPInstDivideC(const GeneFactoryC &factory)
    {
      factory.Get("var1",m_variable1,*GPVariableC<DataT>::GeneType());
      factory.Get("var2",m_variable2,*GPVariableC<DataT>::GeneType());
      factory.Get("result",m_result,*GPVariableC<DataT>::GeneType());
    }

    //! Constructor
    GPInstDivideC()
    {}

    //! Access gene type for instructions
    static GeneTypeC::RefT &GeneType() {
      static GeneTypeC::RefT x = new GeneTypeClassC(typeid(typename GPInstDivideC<DataT>::RefT));
      return x;
    }

    //! Execute instruction
    virtual bool Execute(GPExecutionContextC &context) {
      if(!m_variable1->IsSet() && !m_variable2->IsSet())
        return false;
      if(!m_variable2->IsSet()) {
        m_result->Set(m_variable1->Data());
        return m_variable1->Data() > 0;
      }
      if(!m_variable1->IsSet()) {
        m_result->Set(m_variable2->Data());
        return m_variable2->Data() > 0;
      }
      // Avoid divide by zero
      if(m_variable2->Data() == 0) {
        m_result->Set(0.0);
        return 0;
      }
      DataT value = m_variable1->Data() / m_variable2->Data();
      m_result->Set(value);
      return value > 0;
    }

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPInstDivideC<DataT> > RefT;

  protected:
    typename GPVariableC<DataT>::RefT m_variable1;
    typename GPVariableC<DataT>::RefT m_variable2;
    typename GPVariableC<DataT>::RefT m_result;
  };


}}

#endif
