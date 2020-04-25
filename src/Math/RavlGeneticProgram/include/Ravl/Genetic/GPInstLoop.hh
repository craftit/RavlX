// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GPInstLoop_HH
#define RAVL_GENETIC_GPInstLoop_HH 1
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstruction.hh"
#include "Ravl/Genetic/GPVariable.hh"

namespace RavlN { namespace GeneticN {

  //! An environment for an agent.

  class GPInstLoopC
   : public GPInstructionC
  {
  public:
    //! Factory constructor
    GPInstLoopC(const GeneFactoryC &factory);

    //! Constructor
    GPInstLoopC();

    //! Access gene type for instructions
    static GeneTypeC::RefT &GeneType();

    //! Execute instruction
    virtual bool Execute(GPExecutionContextC &context);

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPInstLoopC> RefT;

  protected:
    GPInstructionC::RefT m_test;
    GPInstructionC::RefT m_do;
    IntT m_loopLimit;
  };

}}

#endif
