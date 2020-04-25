// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HTTPREQUEST_HEADER
#define RAVL_HTTPREQUEST_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid = "$Id$"
//! lib=RavlEHS
//! author = "Warren Moore"
//! file = "Ravl/Contrib/EHS/HTTPRequest.hh"
//! docentry = "Ravl.API.OS.HTTP.Server"

#include "Ravl/String.hh"
#include "Ravl/Hash.hh"



class HttpRequest;
// Pre-declare the EHS request implementation

  
  
namespace RavlN
{
  
  
  
  enum HTTPRequestMethodT
  {
    EHTTPRequest_GET = 0,
    EHTTPRequest_POST,
    EHTTPRequest_UNKNOWN,
    EHTTPRequest_INVALID
  };
  //: Request method enumeration



  /////////////////////////////
  //! userlevel = Normal
  //: HTTP Server Request
  
  class HTTPRequestC
  {
  public:
    HTTPRequestC();
    //: Default constructor.
    // Creates an invalid handle.
    
    HTTPRequestC(HttpRequest *request);
    //: Constructor
    
    HTTPRequestMethodT Method() const
    { return m_method; }
    //: Get the request method
    
    StringC URI();
    //: Get the request URI
    
    StringC OriginalURI();
    //: Get the original request URI
    
    static void ParseURI(const StringC &uri, StringC &pageName, HashC<StringC, StringC> &args);
    //: Parse the URI
    // Sets the page name and arg hash e.g. /index.html?id=256&test
    // pageName = index.html
    // args {
    //   "id" => "256"
    //   "test" => ""
    // }
  
  protected:
    HttpRequest *m_request;
    //: EHS pimpl
    
    HTTPRequestMethodT m_method;
    //: Request method
  };
  
  
  
}

#endif
