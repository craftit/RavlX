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
//! file = "Ravl/Contrib/EHS/HTTPServer.cc"

#include "Ravl/HTTPServer.hh"
#include "Ravl/EHS.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/Exception.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN
{
  
  
  HTTPServerBodyC::HTTPServerBodyC(UIntT port,UIntT threadPoolSize) :
    m_port(port),
    m_threadPoolSize(threadPoolSize),
    m_ehs(NULL),
    m_sigHandle(HTTPRequestC(), HTTPResponseC(), EHTTPResponseCode_Invalid)
  {
    // Create the server
    m_ehs = new RavlEHSC;
  }
  
  
  
  HTTPServerBodyC::~HTTPServerBodyC()
  {
    // Delete child servers.
    for(HashIterC<StringC,RavlEHSC *> it(m_ehsChildren);it;it++)
      delete it.Data();
    
    // Delete master server.
    if (m_ehs)
      delete m_ehs;
  }
  
  
  
  bool HTTPServerBodyC::Start()
  {
    MutexLockC lock(m_accessMutex);
    
    // Build the parameters
    EHSServerParameters params;
    params["port"] = (IntT)m_port;
    params["mode"] = "threadpool";
    params["threadcount"] = (IntT)m_threadPoolSize;
    
    Connect(m_ehs->SigHandle(), m_sigHandle);

    // Start the server
    m_ehs->StartServer(params);
    
    return true;
  }
  
  
  
  bool HTTPServerBodyC::Stop()
  {
    RavlAssertMsg(m_ehs != NULL, "HTTPServerBodyC::Stop no server");
    MutexLockC lock(m_accessMutex);
    
    m_ehs->StopServer();
    
    return true;
  }
  

  Signal3C< HTTPRequestC, HTTPResponseC, HTTPResponseCodeT > &HTTPServerBodyC::SigHandlePath(const StringC &serverPath) {
    MutexLockC lock(m_accessMutex);
    RavlEHSC *childServer;
    if(m_ehsChildren.Lookup(serverPath,childServer))
      return childServer->SigHandle();
    
    childServer = new RavlEHSC;
    
    if(childServer == 0) {
      // Out of memory, should
      throw ExceptionOperationFailedC("Out of memory.");
    }
    
    m_ehsChildren[serverPath] = childServer;
    m_ehs->RegisterEHS(childServer,serverPath.chars());
    childServer->SetParent (m_ehs,serverPath.chars());
    
    return childServer->SigHandle();
  }
  
  //: Remove path from sever.
  // Returns true if server found and removed
  
  bool  HTTPServerBodyC::RemovePath(const StringC &serverPath) {
    MutexLockC lock(m_accessMutex);
    RavlEHSC *childServer;
    if(!m_ehsChildren.Lookup(serverPath,childServer))
      return false;
    m_ehsChildren.Del(serverPath);
    delete childServer;
    return true;
  }

  
}

