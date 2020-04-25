// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HTTPRESPONSE_HEADER
#define RAVL_HTTPRESPONSE_HEADER 1
//////////////////////////////////////////////////////////////////
//! rcsid = "$Id$"
//! lib=RavlEHS
//! author = "Warren Moore"
//! file = "Ravl/Contrib/EHS/HTTPResponse.hh"
//! docentry = "Ravl.API.OS.HTTP.Server"

#include "Ravl/String.hh"
#include "Ravl/SArray1d.hh"



class HttpResponse;
// Pre-declare the EHS response implementation



namespace RavlN
{
  
  
  
  /////////////////////////////
  //! userlevel = Normal
  //: HTTP Server Response
  
  class HTTPResponseC
  {
  public:
    HTTPResponseC();
    //: Default constructor.
    // Creates an invalid handle.
    
    HTTPResponseC(HttpResponse *response);
    //: Constructor
    
    bool SetText(StringC &text);
    //: Set the response text
    
    bool SetData(SArray1dC<char> &data);
    //: Set the response data
    
    StringC GetText();
    //: Get the response text
    
    bool SetHeader(const StringC &header, const StringC &value);
    //: Set an HTTP response header, such as 'content-type'
    
    bool LookupHeader(const StringC &header,  StringC &value);
    //: Lookup an HTTP response header, such as 'content-type'
    
  protected:
    HttpResponse *m_response;
    //: EHS pimpl
  };
  
  
  
}

#endif
