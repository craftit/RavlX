// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! lib=RavlCore
//! file="Ravl/Core/Base/RefCounter.cc"

#include "Ravl/RefCounter.hh"
#include "Ravl/Assert.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/Hash.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/TypeName.hh"

#if QMAKE_PARANOID && RAVL_HAVE_POSIX_THREADS
#if RAVL_HAVE_POSIX_THREADS
#include <pthread.h>
#endif
#endif

namespace RavlN {
  

  static bool g_doInstanceStatCollection = false;

#if QMAKE_PARANOID && RAVL_HAVE_POSIX_THREADS
  class InstanceEntryC
  {
  public:
    InstanceEntryC()
     : m_count(0),
       m_peak(0)
    {}

    void Increment()
    {
      m_count++;
      if(m_count > m_peak)
        m_peak = m_count;
    }

    void Decrement()
    {
      if(m_count > 0)
        m_count--;
    }

    size_t Count() const
    { return m_count; }

    size_t Peak() const
    { return m_peak; }

    size_t m_count;
    size_t m_peak;
  };

  static pthread_mutex_t g_statsMutex = PTHREAD_MUTEX_INITIALIZER;


  static bool g_disableStats = false;

  static RavlN::HashC<const char *,InstanceEntryC> &InstCounts()
  {
    static RavlN::HashC<const char *,InstanceEntryC> instCounts;
    return instCounts;
  }

  //: Write out class instance counts to 'strm'.
  // This only works in debug builds.

  void DumpInstanceCounts(std::ostream &strm,size_t countThreshold) {
    if(pthread_mutex_lock(&g_statsMutex) != 0) {
      RavlIssueError("Failed to lock mutex.");
    }
    for(HashIterC<const char *,InstanceEntryC> it(InstCounts());it;it++) {
      if(it.Data().Peak() > countThreshold)
        strm << it.Key() << ","<< TypeName(it.Key()) << "," << it.Data().Count() << "," << it.Data().Peak() << "\n";
    }
    pthread_mutex_unlock(&g_statsMutex);
  }
  //: Register a value has been created

  void RCBodyC::RavlRegisterInstance(const std::type_info &type) const
  {
    if(!g_doInstanceStatCollection || g_disableStats)
      return;
    if(pthread_mutex_lock(&g_statsMutex) != 0) {
      RavlIssueError("Failed to lock mutex.");
    }
    g_disableStats = true;
    InstCounts()[type.name()].Increment();
    g_disableStats = false;
    pthread_mutex_unlock(&g_statsMutex);
  }

  //: Unregister a value

  void RCBodyC::RavlUnregisterInstance(const std::type_info &type) const
  {
    if(!g_doInstanceStatCollection || g_disableStats)
      return;
    if(pthread_mutex_lock(&g_statsMutex) != 0) {
      RavlIssueError("Failed to lock mutex.");
    }
    g_disableStats = true;
    InstCounts()[type.name()].Decrement();
    g_disableStats = false;
    pthread_mutex_unlock(&g_statsMutex);
  }
#else
  void RCBodyC::RavlRegisterInstance(const std::type_info &type) const
  {
  }

  void RCBodyC::RavlUnregisterInstance(const std::type_info &type) const
  {
  }

  void DumpInstanceCounts(std::ostream &strm,size_t countThreshold)
  {
    strm << "Instance counts not enabled. \n";
  }

#endif
  void EnableClassInstanceStats(bool flag) {
    g_doInstanceStatCollection = flag;
  }

  //: Test if class stats are enabled

  bool IsClassInstanceStatsEnabled() {
#if QMAKE_PARANOID && RAVL_HAVE_POSIX_THREADS
    return g_doInstanceStatCollection;
#else
    return false;
#endif
  }

  /// RCBodyC //////////////////////////////////////////

  //: Destructor.

  RCBodyC::~RCBodyC() {
    RavlAssert(ravl_atomic_read(&counter) == 0);
#if RAVL_CHECK
    ravl_atomic_set(&counter,-100);
#endif
  }


  //: Make copy of body.
  // This should be provided in derived classes.
  // this function will issue an assertion failure if called.

  RCBodyC &RCBodyC::Copy() const {
    RavlAssert(0);
    return *new RCBodyC();
  }

  RCBodyC &RCBodyC::DeepCopy(UIntT level) const {
    RavlAssert(0);
    return *new RCBodyC();
  }

  
  std::istream &operator>>(std::istream &strm,RCBodyC &obj) {
    return strm;
  }
  //: Input body.
  // No-op.
  
  std::ostream &operator<<(std::ostream &strm,const RCBodyC &obj) {
    return strm;
  }
  //: Output body.
  // No-op.

  BinIStreamC &operator>>(BinIStreamC &strm,RCBodyC &obj) {
    return strm;
  }
  //: Input body.
  // No-op.
  
  BinOStreamC &operator<<(BinOStreamC &strm,const RCBodyC &obj) {
    return strm;
  }
  //: Output body.
  // No-op.


}
