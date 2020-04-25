// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/Socket.cc"
#include "Ravl/config.h"
#if !RAVL_USE_WINSOCK

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

#include "Ravl/OS/Socket.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/SktError.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/SArray1d.hh"

#if RAVL_HAVE_NETDB_H
#include <netdb.h>
#endif
#include <sys/types.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#if RAVL_HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#include <string.h>

#include <stdlib.h>
#include <errno.h>
#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sys/uio.h>
#include <fcntl.h>

#include <poll.h>

#include "Ravl/OS/SysLog.hh"
#include "Ravl/OS/Date.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#if !RAVL_HAVE_SOCKLEN_T
#define socklen_t int
#endif

static const float g_defaultTimeout = 180;

namespace RavlN {

  //: Open socket.

  SocketBodyC::SocketBodyC(StringC name,bool nserver)
    : UnixStreamIOC(-1,g_defaultTimeout,g_defaultTimeout,false),
      server(nserver),
      addr(0),
      m_addrReuse(false),
      m_quiet(false)
  {
    int at = name.index(':');
    if(at < 0) {
      RavlSysLog(SYSLOG_ERR) << "SocketBodyC::SocketBodyC(), No port no in '" << name << "' ";
      return ;
    }
    StringC host = name.before(at);
    StringC pn(name.after(at));
    int portno = atoi(pn);
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Opening connection '" << host << "' port " << portno << " ");
    if(server)
      OpenServer(host,portno);
    else
      OpenClient(host,portno);
  }

  //: Open socket.

  SocketBodyC::SocketBodyC(StringC name,UIntT portno,bool nserver,bool quiet)
    : UnixStreamIOC(-1,g_defaultTimeout,g_defaultTimeout),
      server(nserver),
      addr(0),
      m_addrReuse(false),
      m_quiet(quiet)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Opening connection '" << name << "' port " << portno << " ");
    if(server)
      OpenServer(name,portno);
    else
      OpenClient(name,portno);
  }


  //: Open socket.

  SocketBodyC::SocketBodyC(struct sockaddr *naddr,int nfd,bool nserver)
    : UnixStreamIOC(nfd,g_defaultTimeout,g_defaultTimeout),
      server(nserver),
      addr(naddr),
      m_addrReuse(false),
      m_quiet(false)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "SocketBodyC::SocketBodyC(), fd = " << m_fd );
  }

  //: Open socket.

  SocketBodyC::~SocketBodyC()  {
    Close(); // Tidy up.
  }

  //: Close the socket.

  void SocketBodyC::Close() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "SocketBodyC::Close() dontclose=" << m_dontClose << " fd=" << m_fd << " ");
    UnixStreamIOC::Close();
    if(addr != 0)
      delete [] (char *) addr;
    addr = 0;
  }

  int GetTCPProtocolNumber() {
#if RAVL_OS_LINUX || RAVL_OS_LINUX64
    return SOL_TCP;
#elif RAVL_OS_MACOSX || RAVL_OS_FREEBSD
    return getprotobyname("tcp")->p_proto;
#else
    struct protoent entry;
    char buffer[1024];
    if(getprotobyname_r("tcp",&entry,buffer,1024) != 0) {
      RavlSysLog(SYSLOG_WARNING) << "WARNING: Failed to get tcp protocol number. Guessing a value of 6. \n";
      return 6;
    }
    return entry.p_proto;
#endif
  }

  //: Cork stream.
  // This is used to indicate that there is going to
  // be several write operations immediatly following each
  // other and stops the transmition of fragmented packets.
  // If your not expecting to do any more writes immediatly
  // you must call 'Uncork()'.
  // Returns true if Corking is supported by stream.

  bool SocketBodyC::Cork(bool enable) {
    // Enable delays.
    int n;
    if(enable)
      n = 1;
    else
      n = 0;
    static int tcpprotocolno = GetTCPProtocolNumber();
#ifdef TCP_CORK
    // Linux specific call.
    if(setsockopt(m_fd,tcpprotocolno,TCP_CORK,&n,sizeof(int)) != 0) {
      RavlSysLog(SYSLOG_WARNING) << "SocketBodyC::Cork(), Failed. errno=" << errno <<"\n";
    }
#else
    if(setsockopt(m_fd,tcpprotocolno,TCP_NODELAY,&n,sizeof(int)) != 0) {
      RavlSysLog(SYSLOG_WARNING) << "SocketBodyC::Cork(), Failed. errno=" << errno <<"\n";
    }
#endif
    return true;
  }

  //: Send data as soon as possible.

  void SocketBodyC::SetNoDelay() {
    // Disable delays.
    int n = 1;
    static int tcpprotocolno = GetTCPProtocolNumber();
    if(setsockopt(m_fd,tcpprotocolno,TCP_NODELAY,&n,sizeof(int)) != 0) {
      RavlSysLog(SYSLOG_WARNING) << "SocketBodyC::SetNoDelay(), Failed. errno=" << errno <<" '" << strerror(errno) << "' on fd= " << m_fd << "\n";
    }
  }
  
  //: Open a socket to the given address / port no.

  int SocketBodyC::OpenSocket(struct sockaddr_in &sin,IntT portNo)  {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
      RavlSysLog(SYSLOG_ERR) << "Failed to create socket. " << errno << " ";
      return -1;
    }
    return sock;
  }


  int SocketBodyC::OpenClient(const char *name,IntT portNo) {
    struct sockaddr_in sin = {PF_INET};
    if(!GetHostByName(name,sin)) {
      RavlSysLog(SYSLOG_ERR) << "Failed to lookup hostname '" << name << "' ";
      return -1;
    }
    if((m_fd = OpenSocket(sin,portNo)) < 0)
      return -1;
    sin.sin_port = htons(portNo);
    int retryLimit = 10;
    while(connect(m_fd, (sockaddr*)&sin, sizeof(sin)) < 0) {
      // Sometimes its worth trying a again a few times.
      if((errno == EAGAIN || errno == EINTR || errno==ETIMEDOUT) && retryLimit-- > 0) {
        ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Connect failed, EAGAIN. errno=" << errno << " ");
        Sleep(0.1);
        continue;
      }
      Close();
#if DODEBUG
      if(errno == EADDRINUSE) {
        RavlSysLog(SYSLOG_DEBUG) << "Address in use. " << m_fd ;
        return -1;
      }
      if(errno == ECONNREFUSED) {
        RavlSysLog(SYSLOG_DEBUG) << "Connection refused. " << m_fd;
        return -1;
      }
      RavlSysLog(SYSLOG_DEBUG) << "Connect failed. " << m_fd;
#endif
      return -1;
    }

    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "SocketBodyC::OpenClient(), Connected to '" << name  << "' ");
    if(addr != 0)
      delete [] (char *) addr;
    addr = (struct sockaddr *) new char [sizeof(struct sockaddr)];
    memcpy((char *) addr,(sockaddr*)&sin,sizeof(struct sockaddr));
    //SetNonBlocking(true);
    return m_fd;
  }


  //: Open a server socket.
  // Its then ready for listening.

  int SocketBodyC::OpenServer(const char *name,IntT portNo) {
    struct sockaddr_in sin = {PF_INET};
    if(*name != 0) {
      if(!GetHostByName(name,sin))
        return -1;
    } else {
      sin.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    if((m_fd = OpenSocket(sin,portNo)) < 0)
      return -1;
    sin.sin_port = htons(portNo);
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Binding name. ");
    if(::bind(m_fd,(struct sockaddr*)&sin, sizeof(sockaddr)) < 0) {
      if(!m_quiet) {
        RavlSysLog(SYSLOG_DEBUG) << "Bind failed. errno=" << errno << " ";
        RavlSysLog(SYSLOG_DEBUG) << "Error:" << strerror(errno);
      }
      Close();
      return -1;
    }
    if(m_addrReuse) {
      int one = 1;
      setsockopt(m_fd,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    }
    return m_fd;
  }

  //: Set address reuse flag
  bool SocketBodyC::SetAddressReuse(bool state)
  {
    m_addrReuse = true;
    if(m_fd < 0)
      return false;
    /*
     This socket option tells the kernel that even if this port is busy (in
     the TIME_WAIT state), go ahead and reuse it anyway.  If it is busy,
     but with another state, you will still get an address already in use
     error.  It is useful if your server has been shut down, and then
     restarted right away while sockets are still active on its port.  You
     should be aware that if any unexpected data comes in, it may confuse
     your server, but while this is possible, it is not likely.
    */
    int one = 1;
    setsockopt(m_fd,SOL_SOCKET,SO_REUSEADDR,&one,sizeof(one));
    return true;
  }

  //: Listen for a connection from a client.
  // Can only be used on server sockets.

  SocketC SocketBodyC::Listen(bool block,int backLog) {
    RavlAssert(server); // Must be a server socket.
    if(m_fd < 0)
      return SocketC(); // Not open!
    do {
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Listening. ");
      if(listen(m_fd, backLog) < 0) {
	RavlSysLog(SYSLOG_ERR) << "Listen failed." << errno << " ";
	return SocketC(); // Failed.
      }
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Accepting. ");
      socklen_t addrBuffSize = sizeof(sockaddr);
      struct sockaddr *cn_addr = (struct sockaddr *) new char [addrBuffSize];
      do {
	int nfd = accept(m_fd,cn_addr, &addrBuffSize);
	ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Got connection. Server fd=" << m_fd << "  Client fd=" << nfd);
	if(nfd >= 0)
	  return SocketC(cn_addr,nfd); // Socket accepted ok.
	// Recoverable error ?
      } while(errno == EAGAIN || errno == EINTR) ;
      char errStrBuff[256];
#if RAVL_OS_MACOSX
      strerror_r(errno,errStrBuff,256);
      const char *errStrPtr = errStrBuff;
#else
      const char *errStrPtr = strerror_r(errno,errStrBuff,256);
#endif
      RavlError("ERROR: Failed to accept connection. errno=%d  %s ",errno,errStrPtr);
      delete [] (char *) cn_addr;
    } while(block);
    return SocketC();
  }

  //: Get host name

  StringC SocketBodyC::ConnectedHost() {
    StringC ret("-failed-");
    if(m_fd == 0)
      return ret;
    socklen_t namelen = sizeof(sockaddr) + 256;
    struct sockaddr *name = (struct sockaddr *) new char[namelen];
    if(getpeername(m_fd,name,&namelen) != 0)
    {
      delete [] name;
      RavlSysLog(SYSLOG_ERR) << "SocketBodyC::ConnectedHost(), gerpeername failed. Error=" << errno << " ";
      return StringC("unknown");
    }
    GetHostByAddr(*name,namelen,ret);
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Hostname=" << ret << " ");
    delete [] (char*) name;
    return ret;
  }

  //: Get other port number.

  IntT SocketBodyC::ConnectedPort() {
    if(addr == 0)
      return 0;
    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    getpeername(m_fd, (sockaddr*) &name, &namelen );
    IntT ret = ntohs( name.sin_port );
    return ret;
  }


}
#endif
