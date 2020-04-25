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
//! file = "Ravl/Contrib/EHS/HTTPRequest.cc"

#include "Ravl/HTTPRequest.hh"
#include "Ravl/Assert.hh"
#include "Ravl/StringList.hh"

#include <iostream>
#include <string>
#include <ehs.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  
  
  
  HTTPRequestC::HTTPRequestC() :
    m_request(NULL),
    m_method(EHTTPRequest_INVALID)
  {
  }


  
  HTTPRequestC::HTTPRequestC(HttpRequest *request) :
    m_request(request),
    m_method(EHTTPRequest_INVALID)
  {
    RavlAssertMsg(m_request != NULL, "HTTPRequestC::HTTPRequestC invalid request object");
    
    switch (m_request->nRequestMethod)
    {
      case REQUESTMETHOD_GET:
        m_method = EHTTPRequest_GET;
        break;
      case REQUESTMETHOD_POST:
        m_method = EHTTPRequest_POST;
        break;
      case REQUESTMETHOD_UNKNOWN:
      case REQUESTMETHOD_INVALID:
      default:
        m_method = EHTTPRequest_UNKNOWN;
        RavlIssueError("HTTPRequestC::HTTPRequestC unknown request method");
        break;
    }
  }


    
  StringC HTTPRequestC::URI()
  {
    RavlAssertMsg(m_request != NULL, "HTTPRequestC::URI invalid request object");
    RavlAssertMsg(m_method != EHTTPRequest_UNKNOWN && \
                  m_method != EHTTPRequest_INVALID, "HTTPRequestC::URI invalid request method");
                  
    return StringC(m_request->sUri.c_str());
  }
  
  
  
  StringC HTTPRequestC::OriginalURI()
  {
    RavlAssertMsg(m_request != NULL, "HTTPRequestC::OriginalURI invalid request object");
    RavlAssertMsg(m_method != EHTTPRequest_UNKNOWN && \
                  m_method != EHTTPRequest_INVALID, "HTTPRequestC::OriginalURI invalid request method");
                  
    return StringC(m_request->sOriginalUri.c_str());
  }
  
  
  
  void HTTPRequestC::ParseURI(const StringC &uri, StringC &pageName, HashC<StringC, StringC> &args)
  {
    // Process the request - separate into page and args
    pageName = uri;
    while (pageName.contains('/'))
    {
      pageName = pageName.after('/');
    }
    StringC argStr;
    if (pageName.contains('?'))
    {
      StringC tempPage = pageName;
      pageName = tempPage.before('?');
      argStr = tempPage.after('?'); 
    }
    StringListC argList(argStr, "&");
    for (DLIterC<StringC> it(argList); it; it++)
    {
      StringC arg, val;
      if (it->contains('='))
      {
        arg = it->before('=');
        val = it->after('=');
      }
      else
      {
        arg = *it;
      }
      args.Update(arg, val);
    }
  }



}

