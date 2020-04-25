// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GPInstAssign_HH
#define RAVL_GENETIC_GPInstAssign_HH 1
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstruction.hh"
#include "Ravl/Genetic/GPVariable.hh"
#include "Ravl/Genetic/GeneFactory.hh"

namespace RavlN { namespace GeneticN {

  //! Assign a value

  template<typename DataT>
  class GPInstAssignC
   : public GPInstructionC
  {
  public:
    //! Factory constructor
    GPInstAssignC(const GeneFactoryC &factory)
    {
      factory.Get("var1",m_variable1,*GPVariableC<DataT>::GeneType());
      factory.Get("result",m_result,*GPVariableC<DataT>::GeneType());
    }

    //! Constructor
    GPInstAssignC()
    {}

    //! Access gene type for instructions
    static GeneTypeC::RefT &GeneType() {
      static GeneTypeC::RefT x = new GeneTypeClassC(typeid(typename GPInstAssignC<DataT>::RefT));
      return x;
    }

    //! Execute instruction
    virtual bool Execute(GPExecutionContextC &context) {
      if(!m_variable1->IsSet()) {
        return false;
      }
      m_result->Set(m_variable1->Data());
      return true;
    }

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPInstAssignC<DataT> > RefT;

  protected:
    typename GPVariableC<DataT>::RefT m_variable1;
    typename GPVariableC<DataT>::RefT m_result;
  };


}}

#endif
