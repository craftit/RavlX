// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! docentry="Ravl.API.Core.Strings"
//! file="Ravl/Core/Base/String.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/05/2012"
#ifndef RAVL_CURL_HEADER
#define RAVL_CURL_HEADER 1

#include "Ravl/String.hh"
#include <curl/curl.h>
#include <curl/easy.h>

namespace RavlN {

  //! Class to ensure curl handle is deleted when it goes out of scope.

  class CurlC
  {
  public:
    CurlC();
    //: Constructor.

    CurlC(const StringC &url);
    //: Constructor.

    ~CurlC();
    //: Destructor

    CURL *Curl()
    { return m_curl; }
    //: Access curl handle.

    operator CURL *()
    { return m_curl; }
    //: Cast to a curl ptr.
  protected:
    CURL *m_curl;
  };
}

#endif
