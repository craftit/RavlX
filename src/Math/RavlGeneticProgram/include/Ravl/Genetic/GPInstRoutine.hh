// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GPInstRoutine_HH
#define RAVL_GENETIC_GPInstRoutine_HH 1
//! lib=RavlGeneticProgram
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Programming

#include "Ravl/Genetic/GPInstruction.hh"
#include "Ravl/Collection.hh"

namespace RavlN { namespace GeneticN {

  //! Callable routine

  class GPInstRoutineC
   : public GPInstructionC
  {
  public:
    //! Factory constructor
    GPInstRoutineC(const GeneFactoryC &factory);

    //! Constructor
    GPInstRoutineC();

    //! Access gene type for instructions
    static GeneTypeC::RefT &GeneType();

    //! Execute instruction
    virtual bool Execute(GPExecutionContextC &context);

    //! Handle to this class.
    typedef RavlN::SmartPtrC<GPInstRoutineC> RefT;

  protected:
    RavlN::CollectionC<GPInstructionC::RefT> m_routine;
  };

}}

#endif
