// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlNet

#include "Ravl/OS/SocketAddr.hh"


#if RAVL_OS_SOLARIS
#define __EXTENSIONS__ 1
#include <string.h>
#include <netdir.h>
#endif

#ifdef __linux__
#define _REENTRANT 1
#define _BSD_SOURCE 1
#define _XOPEN_SOURCE_EXTENDED 1
#endif

#include "Ravl/OS/SocketAddr.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/SktError.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/OS/SysLog.hh"

#if RAVL_HAVE_NETDB_H
#include <netdb.h>
#endif
#include <sys/types.h>

#if !RAVL_USE_WINSOCK
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#endif
#if RAVL_HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#include <string.h>

#include <stdlib.h>
#include <errno.h>
#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#endif

#if !RAVL_USE_WINSOCK
#include <sys/uio.h>
#include <poll.h>
#endif
#include <fcntl.h>



#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#if !RAVL_HAVE_SOCKLEN_T
#define socklen_t int
#endif

namespace RavlN {
  //: Attempt to get info about named host.
  // returns true on success.

  bool GetHostByName(const char *name,struct sockaddr_in &sin) {
#if RAVL_USE_WINSOCK
    RavlAssertMsg(0,"Not implemented. ");
	return false;
#else
    int opErrno = 0;
    int buffSize = 1024;
    char *hostentData = new char [buffSize];
    struct hostent ent;
    struct hostent *result = 0;
    if(*name == 0)
      name = "localhost";
    ent.h_addrtype = 0;
    while(1) {
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << " Looking for '" << name << "'\n");
      opErrno = 0;
#if RAVL_OS_LINUX || RAVL_OS_LINUX64
      if(gethostbyname_r(name,&ent,hostentData,buffSize,&result, &opErrno) == 0 && result != 0)
	break;
#elif RAVL_OS_OSF
      if((gethostbyname_r(name,&ent,(struct hostent_data *) hostentData)) != 0) {
        result = &ent;
        break;
      }
      opErrno = h_errno;
#elif RAVL_OS_MACOSX || RAVL_OS_FREEBSD
	  if((result = gethostbyname(name)) != 0)
	break;
#else
      if((result = gethostbyname_r(name,&ent,hostentData,buffSize,&opErrno)) != 0)
	break;
#endif
      if(opErrno == ERANGE) {
        delete [] hostentData;
        buffSize *= 2;
        if(buffSize > 100000) {
          delete [] hostentData;
          throw ExceptionNetC("GetHostByName(),ERROR: Buffer requested too large. Failing.\n");
        }
        hostentData = new char [buffSize];
        continue;
      }
      if(opErrno < 0) {
        RavlSysLog(SYSLOG_WARNING) << "Can't understand hostname '" << name  << "', Malformed address?";
        delete [] hostentData;
        throw ExceptionNetC("Failed to unstanderstand find host name.\n");
      }
#if 0
      if(opErrno == TRY_AGAIN) {
        ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Failed to get hostname, retrying. \n");
        DateC::Sleep(0.5);  // Thread safe sleep.
        continue;
      }
#endif
      if(opErrno == HOST_NOT_FOUND) {
        RavlSysLog(SYSLOG_WARNING) << "Can't find host '" << name << "' .";
        delete [] hostentData;
        throw ExceptionNetC("Can't find host name.\n");
      }
      RavlSysLog(SYSLOG_WARNING) << "Can't find host '" << name  << "' for some reason. Errno:" << opErrno << " '"
#if RAVL_HAVE_HSTRERROR
        << hstrerror(opErrno) << "'";
#else
        << strerror(opErrno) << "'";
#endif
      delete [] hostentData;
      throw ExceptionNetC("Can't find host name for some reason.");

    }

    RavlAssert(result != 0);
    //  char *addr = inet_ntoa(*((struct in_addr *)(result->h_addr_list[0])));
    //sin.sin_addr.s_addr = inet_addr(addr);

#if DODEBUG
    RavlSysLog(SYSLOG_DEBUG) << "Offical hostname: '" << result->h_name << "' h_length: '" << result->h_length << "' ";
    //RavlSysLog(SYSLOG_DEBUG) << "h_addr_list: '" << result->h_addr_list[0] << "' ";
#endif
    sin.sin_addr.s_addr = ((struct in_addr *)result->h_addr_list[0])->s_addr;
    sin.sin_family = ent.h_addrtype;

    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Got host data..  Addr:'" << inet_ntoa(sin.sin_addr) << "' ");

    delete [] hostentData;
    return true;
 #endif
  } 
  //: Attempt to find hostname by the address.
  // returns true on success and assignes the hostname to name.
  
  bool GetHostByAddr(struct sockaddr &sin,int sinLen,StringC &name) {
#if  RAVL_USE_WINSOCK
#if 0
    char strHostName[1024];
    if(getnameinfo(&sin,1024, strHostName, 1024, 0,0,NI_NUMERICSERV) == SOCKET_ERROR)
      return false;
    strRet = StringC(strHostName);
    return true;
#else
    RavlAssertMsg(0,"Not implemetned. ");
    return false;
#endif
#else
    int buffSize = 1024;
    char *hostentData = new char [buffSize];
    struct hostent ent;
    ent.h_name = 0;

    int error = 0;
    while(1) {
#if RAVL_OS_LINUX || RAVL_OS_LINUX64
      struct hostent *result = 0;
      int retcode;
      // Hack to detect the difference between red-hat 6.x  and 7.x boxes.
#if (_XOPEN_SOURCE - 0) <= 500
      // For red-hat 6.2
      if((retcode = gethostbyaddr_r((char *) &(((sockaddr_in &)sin).sin_addr),sinLen,AF_INET,&ent,hostentData,buffSize,&result,&error)) != 0)
	break;
#else
      // For red-hat 7.2
      if((retcode = gethostbyaddr_r(&(((sockaddr_in &)sin).sin_addr),sinLen,AF_INET,&ent,hostentData,buffSize,&result,&error)) != 0)
	break;
#endif
#elif RAVL_OS_OSF
      if(gethostbyaddr_r((const char *) &((sockaddr_in &)sin).sin_addr,sinLen,AF_INET,&ent,(struct hostent_data *)hostentData) == 0)
	break;
      error = h_errno;
#elif RAVL_OS_IRIX
      ulong tmp_addr = ((sockaddr_in &)sin).sin_addr.s_addr ;
      gethostbyaddr_r ((const char *) & tmp_addr, sizeof(tmp_addr), AF_INET, &ent, hostentData, buffSize, &error) ;
#elif RAVL_OS_MACOSX || RAVL_OS_FREEBSD
      {
      	struct hostent *pHostent = gethostbyaddr((const char *) &((sockaddr_in &)sin).sin_addr,sinLen,AF_INET);
      	if (pHostent == 0)
      	  break;
      }
#else
      gethostbyaddr_r((const char *) &((sockaddr_in &)sin).sin_addr,sinLen,AF_INET,&ent,hostentData,buffSize,&error);
#endif
      if(error == 0)
	break;
      if(error == ERANGE) { // Buffer not big enough.
	delete [] hostentData;
	buffSize *= 2;
	if(buffSize > 100000) {
	  delete [] hostentData;
	  throw ExceptionNetC("GetHostByName(),ERROR: Buffer requested too large. Failing. ");
	}
	hostentData = new char [buffSize];
	continue;
      }
#if 0
      char msgBuff[1024];
      msgBuff[0] = 0;
      strerror_r(error,msgBuff,1024);
      RavlSysLog(SYSLOG_WARNING) << "Failed to find hostname from ip address. Error=" << msgBuff << " ("<< error << ") ";
#endif
      break; // Unknown error.
    }
    if(error == 0) { // If we got the name ok.
      name = StringC(ent.h_name);
      delete [] hostentData;
      return true;
    }
    delete [] hostentData;
    MTWriteLockC hold; // this call isn't MT safe.
    name = inet_ntoa(((sockaddr_in &)sin).sin_addr); // Convert to a dot notation string.
    return true;
#endif
  }
  
}
