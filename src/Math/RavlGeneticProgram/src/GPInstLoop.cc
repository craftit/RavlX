// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstLoop.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/GenomeConst.hh"
#include "Ravl/Genetic/GeneFactory.hh"

namespace RavlN { namespace GeneticN {

  //! Factory constructor
  GPInstLoopC::GPInstLoopC(const GeneFactoryC &factory)
   : GPInstructionC(factory)
  {
    static GeneTypeIntC::RefT rangeSizeType = new GeneTypeIntC("loopLimit",2,100);
    factory.Get("LoopLimit",m_loopLimit,*rangeSizeType);
    factory.Get("Test",m_test,*InstructionGeneType());
    factory.Get("Do",m_do,*InstructionGeneType());
  }

  //! Constructor
  GPInstLoopC::GPInstLoopC()
  {

  }

  //! Execute instruction
  bool GPInstLoopC::Execute(GPExecutionContextC &context)
  {
    bool ret = false;
    ScopedCallC scopedCall(context);
    if(!context.IsTooDeep()) {
      int n = 0;
      for(;n < m_loopLimit && !context.IsTerminated();n++) {
        if(!m_test->Execute(context))
          break;
        m_do->Execute(context);
      }
    } else {
      RavlSysLogf(SYSLOG_WARNING,"Call stack too deep.");
    }
    return ret;
  }

  //! Access gene type for instructions
  GeneTypeC::RefT &GPInstLoopC::GeneType()
  {
    static GeneTypeC::RefT x = new GeneTypeClassC(typeid(GPInstLoopC::RefT));
    return x;
  }

  static RegisterGPInstructionC<GPInstLoopC> g_regInst("RavlN::GeneticN::GPInstLoopC");

  void LinkGPInstLoop()
  {}

}}
