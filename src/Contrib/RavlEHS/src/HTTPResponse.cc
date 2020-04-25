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
//! file = "Ravl/Contrib/EHS/HTTPResponse.cc"

#include "Ravl/HTTPResponse.hh"

#include <ehs.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  
  
  
  HTTPResponseC::HTTPResponseC() :
    m_response(NULL)
  {
  }


  
  HTTPResponseC::HTTPResponseC(HttpResponse *response) :
    m_response(response)
  {
  }
  

#ifdef EHS_HAVE_SETBODYNOCOPY
  static void DestoryStringBuffer(char *data,void *userData) {
    StringC *buff = static_cast<StringC *>(userData);
    delete buff;
  }
#endif
  
  
  bool HTTPResponseC::SetText(StringC &text)
  {
    RavlAssertMsg(m_response, "HTTPResponseC::SetText no valid response");
    
#ifdef EHS_HAVE_SETBODYNOCOPY
    StringC *handle = new StringC(text);
    m_response->SetBodyNoCopy(const_cast<char *>(text.chars()), text.length(),DestoryStringBuffer,handle);
#else
    m_response->SetBody(text.chars(), text.length());    
#endif
    
    return true;
  }
  
#ifdef EHS_HAVE_SETBODYNOCOPY
  static void DestorySArrayBuffer(char *data,void *userData) {
    SArray1dC<char> *buff = static_cast<SArray1dC<char> *>(userData);
    delete buff;
  }
#endif
  
  bool HTTPResponseC::SetData(SArray1dC<char> &data)
  {
    RavlAssertMsg(m_response, "HTTPResponseC::SetData no valid response");
    
#ifdef EHS_HAVE_SETBODYNOCOPY
    SArray1dC<char> *handle = new SArray1dC<char>(data);
    m_response->SetBodyNoCopy(data.DataStart(), data.Size(),DestorySArrayBuffer,handle);
#else
    m_response->SetBody(data.DataStart(), data.Size());
#endif
    
    return true;
  }
  
  
  
  StringC HTTPResponseC::GetText()
  {
    RavlAssertMsg(m_response, "HTTPResponseC::GetText no valid response");
    
    return StringC(m_response->GetBody());
  }
  
  
  
  bool HTTPResponseC::SetHeader(const StringC &header, const StringC &value)
  {
    RavlAssertMsg(m_response, "HTTPResponseC::SetHeader no valid response");
    
    m_response->oResponseHeaders[header.chars()] = value.chars();
    
    return true;
  }
  
  
  
  bool HTTPResponseC::LookupHeader(const StringC &header,  StringC &value)
  {
    RavlAssertMsg(m_response, "HTTPResponseC::LookupHeader no valid response");
    
    std::map<std::string, std::string>::const_iterator it = m_response->oResponseHeaders.find(header.chars());
    if (it != m_response->oResponseHeaders.end())
    {
      value = StringC(it->second.c_str());
      return true;
    }
    
    return false;
  }
  
  
  
}

