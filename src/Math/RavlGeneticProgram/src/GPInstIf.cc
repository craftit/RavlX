// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstIf.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/GeneFactory.hh"

namespace RavlN { namespace GeneticN {

  //! Factory constructor
  GPInstIfC::GPInstIfC(const GeneFactoryC &factory)
   : GPInstructionC(factory)
  {
    factory.Get("Test",m_test,*InstructionGeneType());
    factory.Get("OnTrue",m_onTrue,*InstructionGeneType());
    factory.Get("OnFalse",m_onFalse,*InstructionGeneType());
  }

  //! Constructor
  GPInstIfC::GPInstIfC()
  {

  }

  //! Execute instruction
  bool GPInstIfC::Execute(GPExecutionContextC &context)
  {
    if(context.IsTerminated())
      return false;
    ScopedCallC scopedCall(context);
    if(context.IsTooDeep()) {
      RavlSysLogf(SYSLOG_WARNING,"Call stack too deep.");
      return false;
    }
    bool ret = false;
    if(m_test->Execute(context)) {
      ret = m_onTrue->Execute(context);
    } else {
      ret = m_onFalse->Execute(context);
    }
    return ret;
  }

  //! Access gene type for instructions
  GeneTypeC::RefT &GPInstIfC::GeneType()
  {
    static GeneTypeC::RefT x = new GeneTypeClassC(typeid(GPInstIfC::RefT));
    return x;
  }

  static RegisterGPInstructionC<GPInstIfC> g_regInst("RavlN::GeneticN::GPInstIfC");

  void LinkGPInstIf()
  {}

}}
