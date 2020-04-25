// Copyright (C) 2003, Omniperception Ltd.
// file-header-ends-here
#ifndef RAVLN_CPUID_HEADER
#define RAVLN_CPUID_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.Misc"
//! file="Ravl/Core/Base"
//! author="Kieron J Messer"

#include "Ravl/config.h"

#define CHECK_FOR_3DNOW 0

namespace RavlN {

  //! userlevel=Develop

  class CPUIDC {

  public:
    CPUIDC();
    //: Construct

    bool mmx() const
      { return m_mmx; }
    //: do we have mmx

    bool sse() const
      { return m_sse; }
    //: do we have sse

    bool sse2() const
      { return m_sse2; }
    //: do we have sse2

    bool htt() const
      { return m_htt; }
    //: do we have hyperthreading processor

#if CHECK_FOR_3DNOW
    bool ThreeDNow() const
      { return m_3dnow; }
    //: do we have 3d now

    bool ThreeDNowExt() const
      { return m_3dnow_p; }
    //: do we have 3d now with extensions
#endif

    void Info() const;
    //: Print info about processor out


  protected:
    bool m_mmx;
    //: do we have mmx

    bool m_sse;
    //: do we have sse

    bool m_sse2;
    //: do we have sse2

    bool m_htt;
    //: do we have hyper threading

#if CHECK_FOR_3DNOW
    bool m_3dnow;
    //: do we have 3d now

    bool m_3dnow_p;
    //: do we have 3d now extensions
#endif
  };

  //! userlevel=Advanced

  bool MMX();
  //: do we have mmx

  bool SSE();
  //: do we have sse

  bool SSE2();
  //: do we have sse2

  bool HTT();
  //: do we have hyperthreading processor
}


#endif
