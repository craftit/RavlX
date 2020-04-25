// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlOS

#include "Ravl/OS/UnixStreamIO.hh"

#if RAVL_HAVE_INTFILEDESCRIPTORS

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

#include "Ravl/OS/Date.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/MTLocks.hh"

#if RAVL_HAVE_NETDB_H
#include <netdb.h>
#endif
#include <sys/types.h>

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

namespace RavlN {
  
#if RAVL_HAVE_INTFILEDESCRIPTORS

  //: Destructor.
  // Closes the file descriptor.
  
  UnixStreamIOC::~UnixStreamIOC()
  { Close(); }

  //: Close the socket.
  
  void UnixStreamIOC::Close() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::Close, Closing fd " << m_fd << " ");
    if(!m_dontClose) {
      if(m_fd >= 0)
        close(m_fd);
    }
    m_fd = -1;
  }
  
  //: Wait for read to be ready.
  // Returns false on error.

  bool UnixStreamIOC::WaitForRead() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Waiting for read. Timeout=" << m_readTimeOut);
#if !RAVL_OS_MACOSX
    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    
    while(m_fd >= 0) {
      int checkFd = m_fd;
      if(checkFd < 0)
        break;
      FD_SET(checkFd,&rfds);
      int rn;
      if(m_readTimeOut >= 0) {
        tv.tv_sec = Floor(m_readTimeOut);
        tv.tv_usec = Round((m_readTimeOut - floor(m_readTimeOut)) * 1000000);
        rn = select(checkFd+1,&rfds,0,0,&tv);
      } else {
        rn = select(checkFd+1,&rfds,0,0,0);
      }
      if(rn == 0) {
	ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForRead(), Timeout. Err=" << errno);
        if(m_failOnReadTimeout)
          return false;
	continue; // Timeout retry, check if sockets been closed.
      }
      if(rn > 0) return m_fd >= 0; // Some data's ready!
      if(!CheckErrors("UnixStreamIOC::WaitForRead()"))
	break;
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForRead(), Temp error, retry! " << errno);
      // Temporary error, retry.
    }
#else
    struct pollfd ufds[2];
    ufds[0].fd = m_fd;
    ufds[0].events = POLLIN | POLLERR |  POLLHUP | POLLNVAL;
    while(m_fd >= 0) {
      ufds[0].revents = 0;
      int rn = poll(ufds,1,Round(m_readTimeOut * 1000.0));
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForRead(), Poll=" << rn << " Errno= " << errno << " Events=" << ufds[0].revents);
      if(rn == 0) {
        if(m_failOnReadTimeout)
          return false;        
        continue; // Timeout.
      }
      if(rn < 0) { // Error.
	if(!CheckErrors("UnixStreamIOC::WaitForRead()"))
	  break;
      }
      if(ufds[0].revents & POLLIN) // Data ready ?
	return true;
      if(ufds[0].revents & POLLERR) { // Error ?
	RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForRead(), Error on stream " << errno;
	return false;
      }

      if(ufds[0].revents & POLLHUP) { // Hangup ?
	RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForRead(), Hangup on stream " << errno;
	return false;
      }
      if(ufds[0].revents & POLLNVAL) { // Invalid fd ?
	RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForRead(), Invalid fd (" << m_fd << ") for stream." << errno;
	return false;
      }
      RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForRead(), Unexpected condition " << ufds[0].revents;
    }
#endif
    return false;
  }

  //: Wait for write to be ready.
  // Returns false on error.

  bool UnixStreamIOC::WaitForWrite() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Waiting for write. Timeout=" << m_writeTimeOut);
#if !RAVL_OS_MACOSX
    struct timeval timeout;
    fd_set wfds;
    FD_ZERO(&wfds);
    DateC startTime = DateC::NowUTC();
    while(m_fd >= 0) {
      int checkFd = m_fd; // Read it from the class, it might be set to -1 at any time.
      if(checkFd < 0)
        break;
      FD_SET(checkFd,&wfds);
      int rn;
      
      if(m_writeTimeOut >= 0) { // Do we have a finite timeout ?
        RealT timeToGo = (RealT) m_writeTimeOut - (DateC::NowUTC() - startTime).Double(); // Compute time remaining.
        
        if(timeToGo <= 0) {
          RavlSysLog(SYSLOG_WARNING) << "UnixStreamIOC::WaitForWrite(), Timeout writing to file descriptor : " << errno;
          if(m_failOnWriteTimeout)
            break;
          // Reset timer.
          startTime = DateC::NowUTC();
        }
        timeout.tv_sec = Floor(timeToGo);
        timeout.tv_usec = Round((timeToGo - floor(timeToGo)) * 1000000);
        rn = select(checkFd+1,0,&wfds,0,&timeout);
      } else
        rn = select(checkFd+1,0,&wfds,0,0);
      
      if(rn == 0) continue; // Go check if timeout has expired.
      if(rn > 0) return m_fd >= 0; // Ready to write.
      if(!CheckErrors("UnixStreamIOC::WaitForWrite()"))
	break;
      // Retry.
    }
#else
    struct pollfd ufds[2];
    ufds[0].fd = m_fd;
    ufds[0].events = POLLOUT | POLLERR |  POLLHUP | POLLNVAL;
    while(m_fd >= 0) {
      ufds[0].revents = 0;
      int rn = poll(ufds,1,Round(m_writeTimeOut * 1000.0));
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForWrite(), Poll=" << rn << " Errno= " << errno << " Events=" << ufds[0].revents);
      if(rn == 0) continue; // Timeout.
      if(rn < 0) { // Error.
	if(!CheckErrors("UnixStreamIOC::WaitForWrite()"))
	  break;
      }
      if(ufds[0].revents & POLLOUT) // Data ready ?
	return true;
      if(ufds[0].revents & POLLERR) { // Error ?
	RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForWrite(), Error on stream " << errno;
	return false;
      }
      if(ufds[0].revents & POLLHUP) { // Hangup ?
	RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForWrite(), Hangup on stream " << errno;
	return false;
      }
      if(ufds[0].revents & POLLNVAL) { // Invalid fd ?
	RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForWrite(), Invalid fd for stream " << errno;
	return false;
      }
      RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::WaitForWrite(), Unexpected condition " << ufds[0].revents;
    }
#endif
    return false;
  }
  
  
  bool UnixStreamIOC::CheckErrors(const char *opName) {
    if(errno == EINTR || errno == EAGAIN)
      return true; // Temporary error, try again.
#if RAVL_OS_LINUX || RAVL_OS_LINUX64
    char buff[256];
    RavlSysLog(SYSLOG_WARNING) << opName << " Error :" << errno << " '" << strerror_r(errno,buff,256) << "' ";
#else
    RavlSysLog(SYSLOG_WARNING) << opName << " Error :" << errno;
#endif
    return false;
  }

  //: Read some bytes from a stream.

  IntT UnixStreamIOC::Read(char *buff,UIntT size) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::Read(), Buff=" << ((void *) buff) << " Size=" << size << "\n");
    UIntT at = 0;
    while(at < size && m_fd >= 0) {
      if(!WaitForRead())
	break;
      int n = read(m_fd,&(buff[at]),size - at);
      if(n == 0) { // Linux indicates a close by returning 0 bytes read.  Is this portable ??
	ONDEBUG(RavlSysLog(SYSLOG_INFO) << "UnixStreamIOC::Read(). Socket closed ? fd=" << m_fd);
	break;
      }
      if(n < 0) {
	if(!CheckErrors("UnixStreamIOC::Read()"))
	  break;
	RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::Read(), Temp error, retry! " << errno;
	// Temporary error, retry
	n = 0;
      }
      at += n;
      if (!m_fillBufferOnRead)
        break;
    }
    return at;
  }

  //: Read some bytes from a stream.

  IntT UnixStreamIOC::ReadV(char **buffer,IntT *len,int n) {
    if(n == 0) return 0;
    struct iovec vecBuf[101];
    struct iovec *vecp = vecBuf;
    IntT at = 0,total = 0;
    SArray1dC<iovec> vecArray;
    if(n > 100) {
      vecArray = SArray1dC<iovec>(n);
      vecp = &(vecArray[0]);
    }
    for(int i = 0;i < n;i++) {
      vecp[i].iov_base = (char *) buffer[i];
      vecp[i].iov_len = len[i];
      total += len[i];
    }

    while(m_fd >= 0) {
      if(!WaitForRead())
	return 0;
      at = readv(m_fd,vecp,n);
      if(at == 0) {
	RavlSysLog(SYSLOG_INFO) << "UnixStreamIOC::ReadV(). Socket closed. fd=" << m_fd;
	break;
      }
      if(at > 0)
	break;
      if(!CheckErrors("UnixStreamIOC::ReadV()"))
	return 0;
      // Temporary failure, try again.
    }
    if(at == total)
      return at; // All done ?
    if (!m_fillBufferOnRead)
      return at;

    ONDEBUG(RavlSysLog(SYSLOG_WARNING) << "UnixStreamIOC::ReadV(), Socket read interrupted, at=" << at << " Blocks=" << n << " attempting to recover. \n");

    // Read in 1 lump failed, break it up.
    int b = 0,xat = 0;
    do {
      for(;b < n;b++) {
	ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "len[" << b << "]=" << len[b] << " xat=" << xat << " ");
	xat += len[b];
	if(at <= xat) //In this block ?
	  break;
      }
      if(at < xat){
	IntT toGo = xat - at;
	IntT done = len[b] - toGo;
	ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Reading " << toGo << " bytes to complete the block " << b << " xat=" << xat << " done=" << done << " toGo=" << toGo << " ");
	IntT x = Read(&(buffer[b][done]),toGo);
	if(x < 0)
	  return at;
	at += x;
	if(x < toGo)
	  return at; // Some serious error must have occurred to stop 'Read'
      }
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Reading vector b=" << b << " n=" << n << " ");
      RavlAssert(xat == at);
      b++;
      int blocksToGo = n - b;
      if(blocksToGo == 0)
	break; // We're done!

      if(!WaitForRead())
	return at;

      int x = readv(m_fd,&(vecp[b]),blocksToGo);
      if(x == 0) {
	RavlSysLog(SYSLOG_INFO) << "UnixStreamIOC::ReadV(). Socket closed. fd=" << m_fd;
	break;
      }
      if(x < 0) {
	if(!CheckErrors("UnixStreamIOC::ReadV()"))
	  return at;
	// Temporary error. Retry.
	x = 0;
      }
      at += x;
    } while(at < total) ;
    return at;
  }

  //: Write multiple buffers

  IntT UnixStreamIOC::WriteV(const char **buffer,IntT *len,int n) {
    if(n == 0) return 0;
    struct iovec vecBuf[101];
    struct iovec *vecp = vecBuf;
    SArray1dC<iovec> vecArray;
    IntT at = 0,total = 0;
    if(n > 100) {
      vecArray = SArray1dC<iovec>(n);
      vecp = &(vecArray[0]);
    }
    for(int i = 0;i < n;i++) {
      vecp[i].iov_base = (char *) buffer[i];
      vecp[i].iov_len = len[i];
      total += len[i];
    }

    while(m_fd >= 0) {
      if(!WaitForWrite())
	return at;

      //RavlSysLog(SYSLOG_DEBUG) << "writev(" << fd << "," << (void *) vecp << "," << n << "); ";
      if((at = writev(m_fd,vecp,n)) > 0)
	break;
      if(!CheckErrors("UnixStreamIOC::WriteV()"))
	return 0;
      // Temporary failure, try again.
    }

    if(at == total || m_fd < 0)
      return at; // All done ?

    ONDEBUG(RavlSysLog(SYSLOG_WARNING) << "UnixStreamIOC::WriteV(), Socket write interrupted, attempting to recover. (Relatively untested code.) ");

    // Write in 1 lump failed, break it up.
    int b = 0,xat = 0;
    do {
      for(;b < n;b++) {
	xat += len[b];
	if(at <= xat) //In this block ?
	  break;
      }
      if(at < xat){
	IntT toGo = xat - at;
	IntT done = len[b] - toGo;
	ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Reading " << toGo << " bytes to complete the block " << b << " xat=" << xat << " done=" << done << " toGo=" << toGo << "\n");
	IntT x = Write(&(buffer[b][done]),toGo);
	if(x <= 0) return at;
	at += x;
	if(x < toGo) return at; // Some serious error must have occurred to stop 'Write'
      }
      RavlAssert(xat == at);
      b++;
      int blocksToGo = n - b;
      if(blocksToGo == 0)
	break; // We're done!

      if(!WaitForWrite())
	return at;
      int x = writev(m_fd,&(vecp[b]),blocksToGo);
      if(x < 0) {
	if(!CheckErrors("UnixStreamIOC::WriteV()"))
	  return at;
	x = 0;
      }
      at += x;
    } while(at < total) ;
    return at;
  }

  //: Write some bytes to a stream.

  IntT UnixStreamIOC::Write(const char *buff,UIntT size) {
    UIntT at = 0;
    while(at < size && m_fd >= 0) {
      if(!WaitForWrite())
	return at;
      int n = write(m_fd,&(buff[at]),size - at);
      if(n < 0) {
	if(!CheckErrors("UnixStreamIOC::Write()"))
	  return at;
	n = 0;
      }
      at += n;
    }
    return at;
  }

  //: Enable non-blocking use of read and write.
  // true= read and write's won't do blocking waits.
  
  bool UnixStreamIOC::SetNonBlocking(bool block) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "UnixStreamIOC::SetNonBlocking(), Block=" << block << " ");
    long flags = fcntl(m_fd,F_GETFL);
    if(flags < 0) {
      RavlSysLog(SYSLOG_WARNING) << "UnixStreamIOC::SetNonBlocking(), WARNING: Get flags failed. errno=" << errno << " fd=" << m_fd << " ";
      return false;
    }
    long newFlags;
    if(block)
      newFlags = flags | O_NONBLOCK;
    else
      newFlags = flags & ~O_NONBLOCK;
    flags = fcntl(m_fd,F_SETFL,newFlags);
    if(flags < 0) {
      RavlSysLog(SYSLOG_WARNING) << "UnixStreamIOC::SetNonBlocking(), WARNING: Set flags failed. ";
      return false;
    }
    return false;
  }
  
#endif
#else
namespace RavlN {
  //: Destructor.
  // Closes the file descriptor.
  
  UnixStreamIOC::~UnixStreamIOC()
  {}
  
  //: Read some bytes from a stream.
  
  IntT UnixStreamIOC::Read(char *buff,UIntT size) {
    return 0;
  }
  
  //: Read some bytes from a stream.
  IntT UnixStreamIOC::ReadV(char **buffer,IntT *len,int n) {
    return 0;
  }
  
  //: Write some bytes to a stream.
  IntT UnixStreamIOC::Write(const char *buff,UIntT size) {
    return 0;    
  }
  
  //: Write multiple buffers
  IntT UnixStreamIOC::WriteV(const char **buffer,IntT *len,int n) {
    return 0;
  }
  
  //: Enable non-blocking use of read and write.
  // true= read and write's won't do blocking waits.
  bool UnixStreamIOC::SetNonBlocking(bool block) {
    return 0;
  }
  
  //: Close the socket.
  // Note this will only actually close the socket if
  // m_dontClose is false.  
  void UnixStreamIOC::Close() {
  }
  
  //: Wait for read to be ready.
  // Returns false on error.
  bool UnixStreamIOC::WaitForRead() {
    return false;
  }
  
  //: Wait for write to be ready.
  // Returns false on error.
  bool UnixStreamIOC::WaitForWrite() {
    return false;
  }
  
  //: Check for recoverable errors.
  bool UnixStreamIOC::CheckErrors(const char *opName) {
    return false;
  }
  
  
#endif
}
