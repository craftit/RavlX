// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstRoutine.hh"
#include "Ravl/Genetic/GeneFactory.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/GenomeList.hh"
#include "Ravl/Genetic/GenomeShare.hh"
#include "Ravl/VirtualConstructor.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {

  static GeneTypeC::RefT &InstructionListType() {
    static GeneTypeC::RefT retType = new GeneTypeListC<GPInstructionC::RefT>("GPInstRoutine",*InstructionGeneType(),10);
    return retType;
  }

  static RavlN::VirtualConstructorInstC<GeneTypeListC<GPInstructionC::RefT> > vc_function_GeneTypeListC_GPInstructionC_RefT("RavlN::GeneticN::GeneTypeListC<RavlN::SmartPtrC<GPInstructionC>>");

  //! Factory constructor
  GPInstRoutineC::GPInstRoutineC(const GeneFactoryC &factory)
  {
    factory.Get("Instructions",m_routine,*InstructionListType());
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Routine has %zu instructions.",(size_t) m_routine.Size().V()));
  }

  //! Constructor
  GPInstRoutineC::GPInstRoutineC()
  {

  }

  //! Execute instruction
  bool GPInstRoutineC::Execute(GPExecutionContextC &context)
  {
    bool ret = false;
    if(context.IsTerminated())
      return false;
    ScopedCallC scopedCall(context);
    if(!context.IsTooDeep()) {
      for(unsigned i = 0;i < m_routine.Size();i++) {
        ret = m_routine[i]->Execute(context);
      }
    } else {
      RavlSysLogf(SYSLOG_WARNING,"Call stack too deep.");
    }
    return ret;
  }

  //! Create the type
  static GeneTypeC::RefT CreateType() {
    GeneTypeC::RefT x = new GeneTypeClassShareC(typeid(GPInstRoutineC::RefT));
    x->SetDefaultWeight(1.6f);
    return x;
  }

  //! Access gene type for instructions
  GeneTypeC::RefT &GPInstRoutineC::GeneType()
  {
    static GeneTypeC::RefT x = CreateType();
    return x;
  }

  static RegisterGPInstructionC<GPInstRoutineC> g_regInst("RavlN::GeneticN::GPInstRoutineC");

  void LinkGPInstRoutine()
  {}

}}
