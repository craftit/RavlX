// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GPINSTRUCTION_HH
#define RAVL_GENETIC_GPINSTRUCTION_HH 1
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/SmartPtr.hh"
#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/GenomeMeta.hh"

namespace RavlN { namespace GeneticN {

  class GeneFactoryC;

  //! Context for execution.
  class GPExecutionContextC
   : public RCBodyVC
  {
  public:
    //! Constructor
    GPExecutionContextC();

    //! Test if program is terminated.
    bool IsTerminated() const
    { return m_terminate; }

    //! Terminate processing.
    void Terminate()
    { m_terminate = true; }

    void StartCall()
    { m_depth++; }

    void FinishCall()
    { m_depth--; }

    //! Is the call stack to deep ?
    bool IsTooDeep() const
    { return m_depth > m_maxDepth; }

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPExecutionContextC> RefT;

  protected:
    size_t m_depth;
    size_t m_maxDepth;
    bool m_terminate;
  };

  //! Helper class to make keeping track of the call stack depth easier

  class ScopedCallC
  {
  public:
     ScopedCallC(GPExecutionContextC &ctxt)
      : m_ctxt(&ctxt)
     { m_ctxt->StartCall(); }

     ~ScopedCallC()
     { m_ctxt->FinishCall(); }

  protected:
    GPExecutionContextC *m_ctxt;
  };

  //! Instruction base class.

  class GPInstructionC
   : public RCBodyVC
  {
  public:
    //! Construct from gene factory
    GPInstructionC(const GeneFactoryC &factory);

    //! Factory constructor
    GPInstructionC(const XMLFactoryContextC &factory);

    //! Constructor
    GPInstructionC();

    //! Access gene type for instructions
    static const GeneTypeC::RefT &GeneType();

    //! Execute instruction
    virtual bool Execute(GPExecutionContextC &context);

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPInstructionC> RefT;

  protected:
  };

  void AddToInstructionList(const GeneTypeC &instructionType);

  template<typename InstT,typename BaseInstT = GPInstructionC>
  class RegisterGPInstructionC
   : public RegisterGeneClassC<InstT>
  {
  public:
    static typename BaseInstT::RefT ConvertInst2Base(const typename InstT::RefT &val)
    { return val.BodyPtr(); }


    RegisterGPInstructionC(const char *nameOfType)
     : RegisterGeneClassC<InstT>(nameOfType)
    {
      RavlN::RegisterConversion(&ConvertInst2Base);
      AddToInstructionList(*InstT::GeneType());
    }

  protected:
  };

  //! This returns type containing list of all known instructions
  GeneTypeMetaC::RefT &InstructionMetaType();


  //! Access list of available instructions, this actually returns a proxy for an instruction list.
  GeneTypeC::RefT &InstructionGeneType();
}}

#endif
