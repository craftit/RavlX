// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_EHS_HEADER
#define RAVL_EHS_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid = "$Id$"
//! lib=RavlEHS
//! author = "Warren Moore"
//! file = "Ravl/Contrib/EHS/EHS.hh"
//! docentry = "Ravl.API.OS.HTTP.Server"

#include "Ravl/Threads/Signal3.hh"
#include "Ravl/HTTPServer.hh"
#include "Ravl/HTTPRequest.hh"
#include "Ravl/HTTPResponse.hh"

#include <stdio.h>
#include <stdlib.h>
#include <ehs.h>

namespace RavlN
{
  
  //! userlevel=Develop
  //: Dervied class to overrite HandleRequest method in EHS.
  // This class is not intended for direct use.
  
  class RavlEHSC :
    public EHS
  {
  public:
    RavlEHSC();
    //: Constructor
    
    ResponseCode HandleRequest(HttpRequest *request, HttpResponse *response);
    //: Custom request handler
    
    Signal3C< HTTPRequestC, HTTPResponseC, HTTPResponseCodeT > &SigHandle()
    { return m_sigHandle; }
    //: Get the signal handle
    
  protected:
    Signal3C< HTTPRequestC, HTTPResponseC, HTTPResponseCodeT > m_sigHandle;
  };
  
  
  
}

#endif
