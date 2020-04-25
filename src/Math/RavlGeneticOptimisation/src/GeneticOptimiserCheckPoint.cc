// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneticOptimiserCheckPoint.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/CallMethodPtrs.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Directory.hh"

namespace RavlN { namespace GeneticN {

  //! Default constructor
  GeneticOptimiserCheckPointC::GeneticOptimiserCheckPointC()
   : m_interval(3600),
     m_filename("checkpoint"),
     m_numberToKeep(3),
     m_count(0),
     m_checkTaskId(0)
  {}

  //! Factory constructor
  GeneticOptimiserCheckPointC::GeneticOptimiserCheckPointC(const XMLFactoryContextC &factory)
   : m_interval(factory.AttributeReal("interval",3600/4)),
     m_filename(factory.AttributeString("filename","checkpoint")),
     m_numberToKeep(factory.AttributeInt("numberToKeep",3)),
     m_count(0),
     m_checkTaskId(0)
  {
    factory.UseChildComponent("Optimiser",m_optimiser);
    m_timer = GlobalTriggerQueue().BodyPtr();
  }

  //! Start service.

  bool GeneticOptimiserCheckPointC::Start() {

    RavlDebug("Starting checkpointer. ");

    // First check if we're restoring from a previous checkpoint.
    StringC prefix = m_filename.NameComponent();
    DirectoryC dir = m_filename.PathComponent();
    if(dir.IsEmpty())
      dir = StringC(".");

    DListC<StringC> files = dir.List(prefix,".abs");
    Int64T maxCount = -1;
    StringC maxFilename;
    RavlDebug("Found %u matching files. Prefix=%s Directory=%s ",files.Size().V(),prefix.data(),dir.data());
    for(DLIterC<StringC> it(files);it;it++) {
      StringC number = it.Data().after(prefix);
      Int64T num = number.Int64Value();
      if(num > maxCount) {
        maxCount = num;
        maxFilename = it.Data();
      }
    }
    if(maxCount >= 0) {
      RavlInfo("Found checkpoint file '%s', loading it into the optimiser. ",maxFilename.data());
      m_count = maxCount + 1;
      StringC theFilename;
      if(dir.IsEmpty())
        theFilename = maxFilename;
      else
        theFilename = dir + '/' + maxFilename;
      if(!m_optimiser->LoadPopulation(theFilename)) {
        RavlError("Failed to load previous checkpoint '%s' ",maxFilename.data());
        throw RavlN::ExceptionOperationFailedC("Failed to load previous checkpoint. ");
      }
    } else {
      RavlInfo("No checkpoint file found. ");
    }

    m_checkTaskId = m_timer->Schedule(m_interval,TriggerPtr(CBRefT(this),&GeneticOptimiserCheckPointC::DoCheckPoint),m_interval);
    return true;
  }

  //! Generate filename from count.
  bool GeneticOptimiserCheckPointC::GenerateFilename(Int64T count,StringC &filename) const {
    filename = m_filename + StringC(count) + ".abs";
    return true;
  }


  //! Shutdown service

  bool GeneticOptimiserCheckPointC::Shutdown() {
    if(m_checkTaskId > 0) {
      m_timer->Cancel(m_checkTaskId);
      m_checkTaskId = 0;
    }
    return true;
  }

  //! Generate a new checkpoint.
  bool GeneticOptimiserCheckPointC::DoCheckPoint() {
    RavlDebug("DoCheckPoint.");

    // We only want to write them one at a time.
    MutexLockC lock(m_mutexWriteCheckpoint);

    FilenameC newFilename;
    GenerateFilename(m_count+1,newFilename);
    if(newFilename.Exists()) {
      RavlError("Next checkpoint file '%s' already exists! ",newFilename.data());
      return false;
    }
    FilenameC tmpFilename = newFilename + '~';
    if(tmpFilename.Exists()) {
      RavlWarning("Checkpoint temporary file '%s' already exists, most likely old. Removing it." ,newFilename.data());
      tmpFilename.Remove();
    }
    if(!m_optimiser->SavePopulation(tmpFilename)) {
      RavlError("Failed to generate checkpoint '%s' ",newFilename.data());
      return false;
    }
    if(!tmpFilename.Exists()) {
      RavlError("I've lost the checkpoint file '%s' ",tmpFilename.data());
      return false;
    }
    if(tmpFilename.Size() <= 0) {
      RavlError("Checkpoint file '%s' is empty, aborting. ",tmpFilename.data());
      return false;
    }
    if(!tmpFilename.Rename(newFilename)) {
      RavlError("Failed to rename checkpoint file from '%s' to '%s' ",tmpFilename.data(),newFilename.data());
      return false;
    }

    m_count++;

    // Only if things went to plan to we delete the old ones.
    RavlDebug("Keeping %d old files. ",m_numberToKeep);
    if(m_numberToKeep >= 0) {
      Int64T oldCount = m_count - (Int64T) m_numberToKeep;
      if(oldCount >= 0) {
        FilenameC toDelete;
        GenerateFilename(oldCount,toDelete);
        RavlDebug("Deleting old checkpoint file '%s' ",toDelete.data());
        if(toDelete.Exists())
          toDelete.Remove();
      }
    }

    return true;
  }

  void LinkGeneticOptimiserCheckPoint()
  {}


  static XMLFactoryRegisterC<GeneticOptimiserCheckPointC> g_registerGeneticOptimiserCheckPoint("RavlN::GeneticN::GeneticOptimiserCheckPointC");



}}
