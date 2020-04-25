// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/config.h"
#if RAVL_USE_WINSOCK
//! lib=RavlNet

#include "Ravl/OS/SysLog.hh"

#include "Ravl/OS/WinSocket.hh"
#include "Ravl/SArray1d.hh"

#include <time.h>
#include <stdio.h>
#include <winsock2.h>
#include "ws2tcpip.h"


#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace 
{

  const int g_maxWriteVSize = 1500;

}

namespace RavlN 
{
  CRITICAL_SECTION g_critSection;
  bool g_critSectionInitialised = false;

  //: Open socket.  
  SocketBodyC::SocketBodyC(StringC strAddress, bool bServer) :
    m_nSocket(0), 
    m_bServer(bServer), 
    m_nTimeout(-1)
  {
    if(!g_critSectionInitialised)
      {
        InitializeCriticalSection(&g_critSection);
      }
    EnterCriticalSection(&g_critSection);
	  
    int nPos = strAddress.index(':'); //Get the position of the ":"
    if(nPos < 0) //No ":" in the IP address
      {
        //cerr << "SocketBodyC::SocketBodyC(), No port no. in '" << strAddress << "' \n";
        LeaveCriticalSection(&g_critSection);
        return ;
      }

    StringC strHost = strAddress.before(nPos);
    StringC strPort(strAddress.after(nPos));
    int nPort = atoi(strPort);

    ONDEBUG(cerr << "SocketBodyC, opening connection '" << strHost << "' port " << nPort << " \n");
    if(bServer)
      {
        OpenServer(strHost,nPort);
      }
    else
      {
        OpenClient(strHost,nPort);  
      }
    LeaveCriticalSection(&g_critSection);
  }

  SocketBodyC::SocketBodyC(StringC name,UIntT portno, bool bServer) :
    m_nSocket(0), 
    m_bServer(bServer),
    m_nTimeout(-1)
  {
    if(!g_critSectionInitialised)
      {
        InitializeCriticalSection(&g_critSection);
      }
    EnterCriticalSection(&g_critSection);
		
    cerr << "SocketBodyC::SocketBodyC, second constructor\n";
    ONDEBUG(cerr << "Opening connection '" << name << "' port " << portno << "\n");
    if(bServer)
      {
        OpenServer(name,portno);
      }
    else
      {
        OpenClient(name,portno);
      }
    LeaveCriticalSection(&g_critSection);
  }
  
  //: Open socket.
  SocketBodyC::SocketBodyC(struct sockaddr *naddr,int nSocket, bool bServer)
    : m_nSocket(nSocket),
      m_bServer(bServer),
      m_nTimeout(-1)
  { 
    if(!g_critSectionInitialised)
      {
        InitializeCriticalSection(&g_critSection);
      }
    InitialiseWinsock();
  }
	
  void SocketBodyC::InitialiseWinsock()
  {
    WSADATA wsaData;	
    WORD sockVersion = MAKEWORD(2, 3);			// Winsock version 2.2

    // We begin by initializing Winsock
    int ret = WSAStartup(sockVersion, &wsaData);
    if(ret != 0) {
      cerr << "Failed to setup socket layer. Error=" << ret << "\n";
    }
  }

  bool SocketBodyC::OpenClient(StringC strHost, int nPort)
  {
    InitialiseWinsock();

    StringC strIPAddress;
    GetIPAddressFromName(strHost, strIPAddress);
    //Fill out the information needed to initialize a socket�
    SOCKADDR_IN target; //Socket address information
    target.sin_family = AF_INET; // address family Internet
    target.sin_port = htons (nPort); //Port to connect on
    target.sin_addr.s_addr = inet_addr (strIPAddress); //Target IP

    m_nSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
    if (m_nSocket == INVALID_SOCKET)
      {
        int nRet = WSAGetLastError();
        cerr << "SocketBodyC, failed to create socket " << strHost;
        cerr << " port: " << nPort << " error number: " << nRet << "\n";
        WSACleanup();
        return false; //Couldn�t create the socket
      }
    //Try connecting...
    if (connect(m_nSocket, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
      {
        cerr << "SocketBodyC, unable to connect to server, error: " << WSAGetLastError() << "\n";
        return false; //Couldn�t connect
      }
    
    return true; //Success
  }

  void SocketBodyC::Close()
  {
    if(m_nSocket > 0)
      {
        closesocket(m_nSocket);
        if(WSACleanup() == SOCKET_ERROR )
          {
            cerr << "SocketBodyC, failed to close socket, error number: " << WSAGetLastError() << "\n";
            return;
          }
        m_nSocket = 0;
      }
  }

  bool SocketBodyC::OpenServer(StringC strHost, int nPort)
  {
    InitialiseWinsock();

    m_nSocket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket
    if (m_nSocket == INVALID_SOCKET)
      {
        cerr << "SocketBodyC, failed to open server socket\n";
        WSACleanup();
        return false; //Don�t continue if we couldn�t create a socket!!
      }
    StringC strIPAddress;
    GetIPAddressFromName(strHost, strIPAddress);

    SOCKADDR_IN addr; // The address structure for a TCP socket
    addr.sin_family = AF_INET;      // Address family
    addr.sin_port = htons (nPort);   // Assign port to this socket

    addr.sin_addr.s_addr = inet_addr(strIPAddress);

    //Allow socket to be reused
    bool bReusePermitted = true;
    if(setsockopt(m_nSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &bReusePermitted ,sizeof(bool)) == SOCKET_ERROR)
      {
        cerr << "SocketBodyC, failed to set socket option, error no: " << WSAGetLastError() << "\n";
        return false;
      }
    //Attempt to bind to a socket
    if(bind(m_nSocket, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
      {
        cerr << "SocketBodyC, failed to bind socket, error: " << WSAGetLastError() << "\n";
        return false;
      }
    return true;
  }

  SocketC SocketBodyC::Listen(bool block /*= true*/, int nBackLog/* = 1*/)
  {
    if(!m_bServer || m_nSocket <= 0)
      {
        return SocketC(); //Either this isn't a server or the socket is closed so return an empty socket
      }

    //Blocking call 
    bool bContinueBlocking = true;
    while(bContinueBlocking)
      {
        if(!block)
          {
            bContinueBlocking = false;
          }
        if(listen(m_nSocket, nBackLog) == SOCKET_ERROR)
          {
            cerr << "SocketBodyC, listen failed on server\n";
            return SocketC();
          }	  

        SOCKET AcceptSocket = SOCKET_ERROR;
        while( AcceptSocket == SOCKET_ERROR || AcceptSocket == WSAEINTR || AcceptSocket == WSAEWOULDBLOCK) 
          {
            AcceptSocket = accept( m_nSocket, NULL, NULL);
          }
        if(AcceptSocket != INVALID_SOCKET)
          {
            return SocketC(NULL, AcceptSocket, false);
          }
        else
          {
            cerr << "SocketBodyC, failed to accept socket, error: " << WSAGetLastError() << "\n";
          }
      }
    return SocketC();
  }

  //: Get host name 
  StringC SocketBodyC::ConnectedHost() 
  {
    StringC strRet("-failed-");
    if(m_nSocket <= 0)
      {
        return strRet;
      }
		
    struct sockaddr Addr;
    int nameBuffSize = sizeof(Addr);

    if(getpeername(m_nSocket, &Addr, &nameBuffSize) == SOCKET_ERROR) 
      {
        cerr << "SocketBodyC::ConnectedHost(), getpeername failed. Error=" << WSAGetLastError() << "\n";
        return StringC("unknown");
      }
		
    GetHostByAddr(Addr,nameBuffSize,strRet);
    ONDEBUG(cerr << "Hostname=" << strRet << "\n");
    return strRet;
  }

  bool SocketBodyC::GetHostByAddr(struct sockaddr &addr, int namelen, StringC& strRet)
  {
    char strHostName[1024];
		
    if(getnameinfo(&addr,namelen, strHostName, 1024, 0,0,NI_NUMERICSERV) != SOCKET_ERROR)
      {
        strRet = StringC(strHostName);
        return true;
      }
		
    return false;
  }

  bool SocketBodyC::GetIPAddressFromName(StringC &strHost, StringC &strIP)
  {
    //Get the addrinfo of the host
    addrinfo* pHostInfo = NULL;
    if(getaddrinfo(strHost, 0, 0, &pHostInfo) != 0)
      {
        cerr << "Failed to get server address info "<< WSAGetLastError() << "\n";
        return false;
      }

    //Get the ip address
    char strIPAddress[256];
    if(getnameinfo(pHostInfo->ai_addr, pHostInfo->ai_addrlen, strIPAddress, 256, NULL, 0, NI_NUMERICHOST) != 0)
      {
        cerr << "SocketC failed to get server name info "<< WSAGetLastError() << "\n";
        return false;
      }
    freeaddrinfo(pHostInfo);
    strIP = strIPAddress;
    return true;
  }

  //Get the port number at the other end of the connection
  IntT SocketBodyC::ConnectedPort()
  {
    if(m_nSocket <= 0)
      {
        return 0;
      }
    struct sockaddr_in peerName;
    socklen_t namelen = sizeof(peerName); 
    getpeername( m_nSocket, (sockaddr*) &peerName, &namelen );
    IntT nPortNo = ntohs( peerName.sin_port );
    ONDEBUG(cerr << "Port number: " << nPortNo << "\n");
    return nPortNo;
  }


  //Send data as soon as possible
  void SocketBodyC::SetNoDelay()
  {
    if(m_nSocket <= 0)
      {
        return;
      }
    // Disable delays.
    bool bNoDelay = TRUE;
    SetDelay(false);
    //static int tcpprotocolno = GetTCPProtocolNumber();
    if(setsockopt(m_nSocket, IPPROTO_TCP, TCP_NODELAY,(char*) &bNoDelay,sizeof(bool)) != 0) 
      {
        cerr << "SocketBodyC::SetNoDelay(), Failed. errno=" << errno <<"\n";
      }
  }

  bool SocketBodyC::SetDelay(bool bDelay)
  {
    if(m_nSocket <= 0)
      {
        return false;
      }
    bool bNoDelay = !bDelay;
    if(setsockopt(m_nSocket, IPPROTO_TCP, TCP_NODELAY,(char*) &bNoDelay,sizeof(bool)) != 0) 
      {
        cerr << "SocketBodyC::SetNoDelay(), Failed. errno=" << errno <<"\n";
        return false;
      }
    return true;
  }

  bool SocketBodyC::SetNonBlocking(bool bDontBlock)
  {
    if(m_nSocket <= 0)
      {
        return false;
      }
    bool bBlock = !bDontBlock;
    // If iMode = 0, blocking is enabled; 
    // If iMode != 0, non-blocking mode is enabled.
    int iMode = (bBlock ? 0 : 1); //Set to zero if blocking enabled, non-zero otherwise
    if(ioctlsocket(m_nSocket, FIONBIO, (u_long FAR*) &iMode) != 0)
      {
        cerr << "SocketBodyC::SetNonBlocking, failed to state state\n";
        return false;
      }
    return true;
  }

  //Set corking to true if you know that lots of packets are about to be transmitted one 
  //after the other. Set corking to false after the transmission.
  //If enable corking then set a delay to the transmission so get a big packet before transmit,
  //if disable corking then do the opposite.
  bool SocketBodyC::Cork(bool bEnableCorking)
  {
    bool bSetDelay = bEnableCorking;
    return SetDelay(bSetDelay);
  }


  //: Read some bytes from a stream.
  IntT SocketBodyC::Read(char *buff, UIntT buffSize) 
  {
    if(m_nSocket <= 0)
      {
        ONDEBUG(cerr <<  "SocketBodyC::Read invalid socket\n");
        return 0;
      }

    UIntT nTotBytes = 0;
    while( nTotBytes < buffSize)
      {
        int bytesRecv = recv( m_nSocket, &(buff[nTotBytes]), buffSize - nTotBytes, 0 );
        if(bytesRecv != SOCKET_ERROR)
          {
            nTotBytes += bytesRecv;
            if ( bytesRecv == 0) 
              {
                ONDEBUG(cerr <<  "SocketBodyC::Read closed gracefully.\n");
                break;
              }
          }
        else
          {
            int nError = WSAGetLastError();
	
            switch(nError) {
              case WSAECONNRESET: {
                ONDEBUG(cerr << "SocketBodyC::Read closed disgracefully.\n");
                return (IntT) nTotBytes;
              }
              case WSAEINPROGRESS: // Some other operation is waiting.
                ONDEBUG(cerr << "SocketBodyC::Read error: WSAEINPROGRESS\n");
                ::Sleep(10);
                break;
              case WSAEINTR:
              case WSAEWOULDBLOCK: // We don't care about blocking errors
                ONDEBUG(cerr << "SocketBodyC::Read error: WSAEINTR | WSAEWOULDBLOCK\n");
                break;
              case WSAECONNABORTED:
              //case WSAENOTSOCK:
                // Happens when socket is closed.
                ONDEBUG(cerr << "SocketBodyC::Read error: WSAECONNABORTED\n");
                return (IntT) nTotBytes;
              default: {
                cerr << "Receive failed with error code: " << nError << "\n";
                return (IntT) nTotBytes;
              }
            }
          }
      }	
    return (IntT) nTotBytes;
  }

  //Read an array of bytes from a stream
  IntT SocketBodyC::ReadV(char **buffer,IntT *len,int numEntries) 
  {
    if(m_nSocket <= 0)
      {
        return 0;
      }

    int nTotBytes = 0;
    for(int n = 0; n < numEntries; n++)
      {
        nTotBytes += Read(&(*buffer[n]), len[n]);
      }
    return nTotBytes;
  }

  //: Write some bytes from a stream.
  IntT SocketBodyC::Write(const char *buff,UIntT size)
  {
    if(m_nSocket <= 0)
      {
        return 0;
      }
    int nError = 0;
    int nTotBytesSent = 0;
    int nCounter = 0;
    int nPoll = 100;
		
    time_t startTime;
    time(&startTime);
    while( (UIntT) nTotBytesSent < size)
      {
        int numBytesSent = 0;
        numBytesSent = send(m_nSocket, &(buff[numBytesSent]), (size - numBytesSent), 0);
        nError = WSAGetLastError();
        if(numBytesSent == SOCKET_ERROR) {
          switch(nError) {
            case WSAEINTR:
            case WSAEWOULDBLOCK: // Just ignore.
              break;
            case WSAEINPROGRESS:
              ::Sleep(10);
              break;
            default: {
                cerr << "SocketBodyC failed to transmit data, error: " << nError << "\n";
                return nTotBytesSent;
              }
          }
        } else {
          nTotBytesSent += numBytesSent;
        }
        //This read has succeeded so reset the time out data
        if(numBytesSent > 0)
          {
            nCounter = 0;
            time(&startTime);
          }
        //Check whether the time taken has exceeded the time out value
        if(m_nTimeout > -1 && nCounter++ >= nPoll)
          {				
            Sleep(100); 
            time_t currentTime;
            time(&currentTime);
            int nElapsedSeconds = (int) currentTime - (int)startTime;
			
            if( nElapsedSeconds > m_nTimeout)
              {
                cerr << "Timeout on writing data\n";
                cerr << "Timeout: " << m_nTimeout << ", seconds: " << nElapsedSeconds << "\n";
                break;
              }
            nCounter = 0;
          }
      }
    return nTotBytesSent;
  }

  //: Write an array of bytes to a stream.
  IntT SocketBodyC::WriteV(const char **buffer,IntT *len,int numEntries)
  {
    if(m_nSocket <= 0) {
      return 0;
    }
    int nTotBytesSent = 0;
    int firstEntry = 0;
    while (firstEntry < numEntries)
    {
      int lastEntry = firstEntry + 1;
      int dataSent = 0;
      int dataSize = len[firstEntry];

      if (dataSize > g_maxWriteVSize)
      {
//        ONDEBUG(cerr << "Writing entry [" << lastEntry << "] " << dataSize << " bytes" << endl);
        dataSent = Write(buffer[firstEntry], dataSize);
      }
      else
      {
        for (; lastEntry < numEntries; lastEntry++)
        {
          if (dataSize + len[lastEntry] > g_maxWriteVSize)
            break;
          dataSize += len[lastEntry];
//          ONDEBUG(cerr << "Merging entry [" << lastEntry << "] " << dataSize << " bytes" << endl);
        }
//        ONDEBUG(cerr << "Copying " << lastEntry - firstEntry << " entries [" << firstEntry << " - " << lastEntry << "] " << dataSize << " bytes" << endl);

        SArray1dC<char> data(dataSize);
        int dataOffset = 0;
        for (int index = firstEntry; index < lastEntry; index++)
        {
//          ONDEBUG(cerr << "Copying entry [" << index << "] " << len[index] << " bytes @ " << dataOffset << " offset" << endl);
          memcpy(data.DataStart() + dataOffset, buffer[index], len[index]);
          dataOffset += len[index];
        }

//        ONDEBUG(cerr << "Writing " << dataSize << " bytes" << endl);
        dataSent = Write(data.DataStart(), dataSize);
      }

      nTotBytesSent += dataSent;
      firstEntry = lastEntry;

      if (dataSent != dataSize)
      {
        cerr << "SocketBodyC::WriteV error (" << dataSize << " size != " << dataSent << " sent)" << endl;
        break;
      }
    }

    return nTotBytesSent;
  }	
	
}
		


#endif
