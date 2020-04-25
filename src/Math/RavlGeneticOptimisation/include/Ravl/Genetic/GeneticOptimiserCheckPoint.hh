// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENETICOPTIMISERCHECKPOINT_HH
#define RAVL_GENETIC_GENETICOPTIMISERCHECKPOINT_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneticOptimiser.hh"
#include "Ravl/Threads/TimedTriggerQueue.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Service.hh"
#include "Ravl/Threads/Mutex.hh"

namespace RavlN { namespace GeneticN {

  //! Save state of optimiser at regular intervals

  class GeneticOptimiserCheckPointC
    : public ServiceC
  {
  public:
    //! Default constructor
    GeneticOptimiserCheckPointC();

    //! Factory constructor
    GeneticOptimiserCheckPointC(const XMLFactoryContextC &factory);

    //! Start service.
    virtual bool Start();

    //! Shutdown service
    virtual bool Shutdown();

    //! Generate filename from count.
    virtual bool GenerateFilename(Int64T count,StringC &filename) const;

    //! Generate a new checkpoint.
    bool DoCheckPoint();

    //! Owner reference counted ptr to class
    typedef RavlN::SmartOwnerPtrC<GeneticOptimiserCheckPointC> RefT;

    //! Callback reference counter ptr to class
    typedef RavlN::SmartCallbackPtrC<GeneticOptimiserCheckPointC> CBRefT;

  protected:
    MutexC m_mutexWriteCheckpoint;
    GeneticOptimiserC::RefT m_optimiser;
    float m_interval;
    FilenameC m_filename;
    IntT m_numberToKeep;
    Int64T m_count;
    TimedTriggerQueueBodyC::RefT m_timer;
    UIntT m_checkTaskId;
  };

}}

#endif
