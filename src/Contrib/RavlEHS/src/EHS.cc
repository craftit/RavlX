// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////////////
//! rcsid = "$Id$"
//! lib=RavlEHS
//! author = "Warren Moore"
//! file = "Ravl/Contrib/EHS/EHS.cc"

#include "Ravl/EHS.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  
  
  
  RavlEHSC::RavlEHSC() :
    EHS(),
    m_sigHandle(HTTPRequestC(), HTTPResponseC(), EHTTPResponseCode_Invalid)
  {
  }

  
  
  ResponseCode RavlEHSC::HandleRequest(HttpRequest *request, HttpResponse *response)
  {
    // Wrap the requests
    HTTPRequestC rrequest(request);
    HTTPResponseC rresponse(response);
    HTTPResponseCodeT rresponsecode = EHTTPResponseCode_Invalid;
    
    // Call the signal
    m_sigHandle(rrequest, rresponse, rresponsecode);

    // Complete the request
    return (ResponseCode)rresponsecode;
  }
  
  
  
}

