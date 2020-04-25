// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GPINSTRUCTIONSENDSIGNAL_HH
#define RAVL_GENETIC_GPINSTRUCTIONSENDSIGNAL_HH 1
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/SmartPtr.hh"
#include "Ravl/Genetic/GPInstruction.hh"
#include "Ravl/Genetic/GPVariable.hh"
#include "Ravl/Threads/Signal1.hh"
#include "Ravl/Calls.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Genetic/GeneFactory.hh"


namespace RavlN { namespace GeneticN {

  //! An environment for an agent.

  template<typename VarDataT,typename DataT = VarDataT>
  class GPInstSendSignalC
   : public GPInstructionC
  {
  public:

    //! Factory constructor
    GPInstSendSignalC(const GeneFactoryC &factory)
    {
      //RavlSysLogf(SYSLOG_DEBUG,"new GPInstSentSignal %s. ",TypeName(typeid(DataT)));
      factory.Get("Data",m_data,*GPVariableC<VarDataT>::GeneType());
      factory.Get("Signal",m_sig,*GPVariableC<RavlN::CallFunc1C<DataT> >::GeneType());
    }

    //! Constructor
    GPInstSendSignalC();

    //! Access gene type for instructions
    static GeneTypeC::RefT &GeneType() {
      static GeneTypeC::RefT x = new GeneTypeClassC(typeid(typename GPInstSendSignalC<VarDataT,DataT>::RefT));
      return x;
    }

    //! Execute instruction
    virtual bool Execute(GPExecutionContextC &context) {
      //RavlDebug("Do GPInstSentSignal %s. Sig:%d Data:%d ",TypeName(typeid(DataT)),(int) m_sig->IsSet(),(int) m_data->IsSet());
      if(!m_sig->IsSet())
        return false;
      if(!m_sig->Data().IsValid()) {
        //RavlDebug("Nowhere to put data. ");
        return false;
      }
      if(!m_data->IsSet()) {
        //RavlDebug("Data not set. ");
        return false;
      }
      //RavlDebug("Sending data! ");
      m_sig->Data().Call(m_data->Data());
      return true;
    }

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPInstSendSignalC<VarDataT,DataT> > RefT;

  protected:
    typename GPVariableC<VarDataT>::RefT m_data;
    typename GPVariableC<RavlN::CallFunc1C<DataT> >::RefT m_sig;
  };

}}

#endif
