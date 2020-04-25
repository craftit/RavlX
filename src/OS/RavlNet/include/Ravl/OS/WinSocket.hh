// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/config.h"
#if RAVL_USE_WINSOCK
//! lib=RavlNet

#ifndef RAVL_WINSOCKET_HEADER
#define RAVL_WINSOCKET_HEADER 1

#include "Ravl/String.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/OS/SktError.hh"

// Some forward declarations to avoid including extra header
// files.

struct sockaddr_in;
struct sockaddr;

namespace RavlN 
{

  class SocketC;

  class SocketBodyC 
    : public RCBodyVC
  {
  public:
	  
	//: Open socket.
    // The 'name' has the format  'host:port' where port may be a
    // host name or its ip (dotted numbers) address and port is the 
    // number of the port to use.	
    SocketBodyC(StringC name,bool server = false);
	SocketBodyC(StringC name, UIntT portno, bool bServer = false);
	SocketBodyC(struct sockaddr *addr, int nSocket, bool server = false);  

	~SocketBodyC() 
	{
		Close();
	};	

  private:
	  //Member functions
	  bool OpenClient(StringC strHost, int nPort);
	  bool OpenServer(StringC strHost, int nPort);
	  bool GetIPAddressFromName(StringC &strHost, StringC &strIP);

	  void InitialiseWinsock();

	  //Member variables
	  //static bool m_sbWinSockInitialised;
	//  static CRITICAL_SECTION m_sCritSec;
 
	  bool m_bServer;	//Is this a server socket
	  bool m_bCanClose; //Are we allowed to close this socket
	  int m_nSocket;	//Socket identifier
	  RealT m_nTimeout;	//Time (in seconds) to attempt a write to a socket before giving up (-1 if not set)
    
  public:
	
	//: Set the amount of time you should attempt to write to socket.
    // This limits the time spent attempting to write to a socket
    // without sending a single byte. The default is 120 seconds.
    void SetWriteTimeout(IntT nTimeOut)
    {
		m_nTimeout = (RealT) nTimeOut;
	}
    
	//: Access file descriptor.
    int Fd() const
    { 
		return m_nSocket; 
	}
    
	//: Is the socket open
    bool IsOpen() const
    { 
		return (m_nSocket > 0); 
	}

    //: Listen for a connection from a client.
    // Can only be used on server sockets.
    // If block is true, the call will not return until there
    // is a valid client.
    SocketC Listen(bool block = true,int backLog = 1);
	
	//: Close the socket.
    void Close();
    
	//: Setup don't close flag.
    void SetDontClose(bool bDontClose)
    { m_bCanClose = !bDontClose;	//Avoid double negative issues
    }
    
    StringC ConnectedHost(); //: Get the name of the host at the other end of the connection.
    
    IntT ConnectedPort();    //: Get the port number at the other end of the connection.
    
	void SetNoDelay(); //Disable Nagle algorithm and send data asap. Faster transactions but more packets sent 

	bool SetNonBlocking(bool bDontBlock); //Enable \ disable blocking
    
	//Set corking to true if you know that lots of packets are about to be written one 
	//after the other. This prevents transmission of fragmented packets.
	//Set corking to false immediately after the transmission.
	bool Cork(bool bEnableCorking);
    IntT Read(char *buff,UIntT size); //: Read some bytes from a stream.
    IntT ReadV(char **buffer,IntT *len,int n);	//Read an array of bytes from a stream
    IntT Write(const char *buff,UIntT size);	//: Write some bytes from a stream.
    IntT WriteV(const char **buffer,IntT *len,int n);	//: Write an array of bytes to a stream.
	
    
    
  protected:
    
    bool GetHostByAddr(struct sockaddr &sin,int sinLen,StringC &name);
    //: Attempt to find hostname by the address.
    // returns true on success and assignes the hostname to name.

	bool SetDelay(bool bDelay);
  };
  
  
  //! userlevel=Normal
  // For dealing with low level stream sockets.
  
  class SocketC 
    : public RCHandleC<SocketBodyC>
  {
  public:
    SocketC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    SocketC(StringC address,bool server = false)
      : RCHandleC<SocketBodyC>(*new SocketBodyC(address,server))
    {}
    //: Open socket.
    // The 'address' has the format  'host:port' where port may be a
    // host name or its ip (dotted numbers) address and port is the 
    // number of the port to use.
    
    SocketC(StringC host,UIntT portno,bool server = false)
      : RCHandleC<SocketBodyC>(*new SocketBodyC(host,portno,server))
    {}
    //: Open socket.
    // 'host' is the host name to connect to or its ip address (dotted numbers)
    // 'port' is the port number to use.
    
  protected:
    SocketC(struct sockaddr *addr,int nfd,bool server = false)
      : RCHandleC<SocketBodyC>(*new SocketBodyC(addr,nfd,server))
    {}
    //: Open socket.
    
    SocketBodyC &Body()
    { return RCHandleC<SocketBodyC>::Body(); }
    //: Access body.
    
    const SocketBodyC &Body() const
    { return RCHandleC<SocketBodyC>::Body(); }
    //: Access body.
    
  public:
    void SetWriteTimeout(IntT value)
    { Body().SetWriteTimeout(value); }
    //: Set the amount of time you should attempt to write to socket.
    // This limits the maximum time spent attempting to write to a socket
    // without sending a single byte. The default is 120 seconds.
    
    int Fd() const
    { return Body().Fd(); }
    //: Access file descriptor.
    
    bool IsOpen() const
    { return Body().IsOpen(); }
    //: Test if socket is open.
    
    inline SocketC Listen(bool block = true,int backLog = 1)
    { return Body().Listen(block,backLog); }
    //: Listen for a connection from a client.
    // Can only be used on server sockets. <br>
    // If block is true, the call will not return until there
    // is a valid client. <br>
    // 'backLog' is the maximum number of connects that will be kept pending, between calls
    // to Listen().
    
    void Close()
    { Body().Close(); }
    //: Close the socket.
    
    void SetDontClose(bool ndontClose)
    { Body().SetDontClose(ndontClose); }
    //: Setup don't close flag.
    
    StringC ConnectedHost()
    { return Body().ConnectedHost(); }
    //: Get host name 
    
    IntT ConnectedPort()
    { return Body().ConnectedPort(); }
    //: Get other port number.
    
    bool Cork(bool enable)
    { return Body().Cork(enable); }
    //: Cork stream.  
    // True indicates that there is going to
    // be several write operations immediatly following each
    // other and stops the transmition of fragmented packets.
    // If your not expecting to do any more writes immediatly
    // you must call 'Cork(false)' immediatly. <br>
    // False indicates that all the pending data has been written
    // This sends any partial packets still pending. <br>
    // NB: This is not compatible with SetNoDelay. <br>
    // Returns true if Corking is supported by stream.
    
    
    void SetNoDelay()
    { Body().SetNoDelay(); }
    //: Send data as soon as possible. 
    // Don't gather data into larger packets. 
    // This should make transactions faster at the expense of sending more 
    // packets over the network.
    
    bool SetNonBlocking(bool block)   
    { return Body().SetNonBlocking(block); }
    //: Enable non-blocking use of read and write.
    // true= read and write's won't do blocking waits.
    
    IntT Read(char *buff,UIntT size)
    { return Body().Read(buff,size); }
    //: Read some bytes from a stream.
    
    IntT ReadV(char **buffer,IntT *len,int n)
    { return Body().ReadV(buffer,len,n); }
    //: Read an array of bytes from a stream.
    
    IntT Write(const char *buff,UIntT size)
    { return Body().Write(buff,size); }
    //: Write some bytes to a stream.
    
    IntT WriteV(const char **buffer,IntT *len,int n)
    { return Body().WriteV(buffer,len,n); }
    //: Write multiple buffers
    
    friend class SocketBodyC;
  };
  
}
#endif
#endif
