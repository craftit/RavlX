// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////
//! lib=RavlNet
//! file="Ravl/OS/Network/NetEndPoint.cc"

#include "Ravl/config.h"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/OS/NetEndPoint.hh"
#include "Ravl/OS/NetMsgCall.hh"
#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/NetByteStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#define RAVL_USE_DECODE_THREAD 0

namespace RavlN {

  enum NEPMsgTypeT { NEPMsgInit = 1, NEPMsgPing };
  
  const SizeT g_netEndPointDefaultPacketSizeLimit = 100000000;

  const SizeT g_stackSizeReceiveThread = 4 * 1024 * 1024; // 4Mb should be half of system default.
  const SizeT g_stackSizeTransmitThread = 100000; // Transmit thread only needs a small stack.
  
  //: Global count of open net end points.

#ifdef RAVL_USE_STD_ATOMIC
  static ravl_atomic_t openNetEndPointCount(0);
#else
  static ravl_atomic_t openNetEndPointCount = RAVL_ATOMIC_INIT(0);
#endif
  
  //: Constructor.
  
  NetEndPointBodyC::NetEndPointBodyC(const StringC &addr,bool nautoInit,bool _optimiseThroughput) 
    : transmitQ(15),
      receiveQ(5),
      shutdown(false),
      autoInit(nautoInit),
      sigConnectionBroken(true),
      peerInfo("Unknown","-"),
      useBigEndianBinStream(RAVL_BINSTREAM_ENDIAN_BIG),
      use32mode(true),
      pingSeqNo(1),
      optimiseThroughput(_optimiseThroughput),
      threadsStarted(false),
      m_maxPacketSize(g_netEndPointDefaultPacketSizeLimit)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::NetEndPointBodyC(address,autoInit,optimiseThroughput)");
    // Increment count of open connections.
    ravl_atomic_inc(&openNetEndPointCount);
    
    SocketC skt(addr,false);
    localInfo.appName = SysLogApplicationName();
    Init(skt); 
  }
  
  //:  Constructor.
  
  NetEndPointBodyC::NetEndPointBodyC(SocketC &nskt,bool nautoInit,bool _optimiseThroughput)
    : transmitQ(15),
      receiveQ(5),
      shutdown(false),
      autoInit(nautoInit),
      sigConnectionBroken(true),
      peerInfo("Unknown","-"),
      useBigEndianBinStream(RAVL_BINSTREAM_ENDIAN_BIG),
      use32mode(true),
      pingSeqNo(1),
      optimiseThroughput(_optimiseThroughput),
      threadsStarted(false),
      m_maxPacketSize(g_netEndPointDefaultPacketSizeLimit)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::NetEndPointBodyC(socket,autoInit,optimiseThroughput)");
    // Increment count of open connections.
    ravl_atomic_inc(&openNetEndPointCount);
    
    localInfo.appName = SysLogApplicationName();
    Init(nskt); 
  }
  
  //: Constructor.
  
  NetEndPointBodyC::NetEndPointBodyC(SocketC &socket,const StringC &protocolName,const StringC &protocolVersion,bool nautoInit,bool _optimiseThroughput )
    : transmitQ(15),
      receiveQ(5),
      shutdown(false),
      autoInit(nautoInit),
      sigConnectionBroken(true),
      localInfo(protocolName,protocolVersion),
      peerInfo("Unknown","-"),
      useBigEndianBinStream(RAVL_BINSTREAM_ENDIAN_BIG),
      use32mode(true),
      pingSeqNo(1),
      optimiseThroughput(_optimiseThroughput ),
      threadsStarted(false),
      m_maxPacketSize(g_netEndPointDefaultPacketSizeLimit)
  { 
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::NetEndPointBodyC(socket,protocolName,protocolVersion,autoInit,optimiseThroughput)");
    // Increment count of open connections.
    ravl_atomic_inc(&openNetEndPointCount);
    
    localInfo.appName = SysLogApplicationName();
    Init(socket); 
  }
  
  //: Constructor.
  
  NetEndPointBodyC::NetEndPointBodyC(const StringC &address,const StringC &protocolName,const StringC &protocolVersion,bool nautoInit,bool _optimiseThroughput) 
    : transmitQ(15),
      receiveQ(5),
      shutdown(false),
      autoInit(nautoInit),
      sigConnectionBroken(true),
      localInfo(protocolName,protocolVersion),
      peerInfo("Unknown","-"),
      useBigEndianBinStream(RAVL_BINSTREAM_ENDIAN_BIG),
      use32mode(true),
      pingSeqNo(1),
      optimiseThroughput(_optimiseThroughput),
      threadsStarted(false),
      m_maxPacketSize(g_netEndPointDefaultPacketSizeLimit),
      m_port(0)
  { 
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::NetEndPointBodyC(address,protocolName,protocolVersion,autoInit,optimiseThroughput) address=" << address);
    // Increment count of open connections.
    ravl_atomic_inc(&openNetEndPointCount);
    
    SocketC skt(address,false);
    localInfo.appName = SysLogApplicationName();
    Init(skt); 
  }
  
  //: Default constructor.
  
  NetEndPointBodyC::NetEndPointBodyC(bool _optimiseThroughput) 
    : transmitQ(15),
      receiveQ(5),
      shutdown(false),
      autoInit(true),
      sigConnectionBroken(true),
      peerInfo("Unknown","-"),
      useBigEndianBinStream(RAVL_BINSTREAM_ENDIAN_BIG),
      use32mode(true),
      pingSeqNo(1),
      optimiseThroughput(_optimiseThroughput),
      threadsStarted(false),
      m_maxPacketSize(g_netEndPointDefaultPacketSizeLimit),
      m_port(0)
  {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::NetEndPointBodyC(optimiseThroughput)");
    localInfo.appName = SysLogApplicationName();
    // Increment count of open connections.
    ravl_atomic_inc(&openNetEndPointCount);
  }
  
  //: Destructor.
  
  NetEndPointBodyC::~NetEndPointBodyC() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::~NetEndPointBodyC(), Called.");
    setupComplete.Post(); // Make sure nothings waiting for setup to complete.
    sigConnectionBroken.DisconnectAll(true);
    ravl_atomic_dec(&openNetEndPointCount);
  }

  //: Return a count of the number of the current number of open connections.
  
  IntT NetEndPointBodyC::CountOpenConnections() 
  {
    return ravl_atomic_read(&openNetEndPointCount);
  }
  
  //: Register new message handler.
  
  bool NetEndPointBodyC::Register(const NetMsgRegisterC &nmsg) {
    MutexLockC lock(accessMsgReg);
#if RAVL_CHECK
    if(msgReg.IsElm(nmsg.Id())) {
      NetMsgRegisterC oldMsg;
      msgReg.Lookup(nmsg.Id(),oldMsg);
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::Register(), WARNING: Overriding handling of message id:" << nmsg.Id() << " Name=" << nmsg.Name() << " Old name=" << oldMsg.Name();
    }
#endif
    msgReg[nmsg.Id()] = nmsg;
    return true;
  }
  
  //: Remove message handler for given id.
  
  bool NetEndPointBodyC::Deregister(UIntT id) {
    MutexLockC lock(accessMsgReg);
    return msgReg.Del(id);
  }
  
  //: Search for message decode/encode of type 'id'.
  
  NetMsgRegisterC NetEndPointBodyC::Find(UIntT msgTypeID) const {
    // Check local decode table.
    MutexLockC lock(accessMsgReg);
    NetMsgRegisterC *msg = const_cast<NetMsgRegisterC *>(msgReg.Lookup(msgTypeID));
    if(msg != 0) 
      return *msg;
    // Check global decode table.
    msg = NetMsgRegisterC::MsgTypes().Lookup(msgTypeID);
    if(msg != 0) 
      return *msg;
    return NetMsgRegisterC();
  }
  
  //: Send init message.
  
  void NetEndPointBodyC::SndInit(StringC &user) {
    const char *hostType = getenv("HOSTTYPE");
    if(hostType == 0)
      hostType = "*unknown*";
    localInfo.hostType = StringC(hostType);
    
    Send(NEPMsgInit,user,localInfo);
  }
  


  bool NetEndPointBodyC::Init(SocketC &nskt) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::Init(), Called.");
    if(!nskt.IsOpen()) {
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::Init(), Socket not opened. ");
      shutdown = true;
      return false;
    }
    
    // If stream doesn't support corking use NoDelay.
    if(!optimiseThroughput) 
      nskt.SetNoDelay();
    
    //nskt.SetNonBlocking(true);
    istrm = NetIByteStreamC(nskt);
    ostrm = NetOByteStreamC(nskt);
    
    RegisterR(NEPMsgInit,StringC("Init"),*this,&NetEndPointBodyC::MsgInit);
    RegisterR(NEPMsgPing,StringC("Ping"),*this,&NetEndPointBodyC::MsgPing);
    
    if(autoInit) {
      const char *un = getenv("USER"); // This isn't really secure!!
      if(un == 0)
        un = "*unknown*";
      StringC auser(un);
      SndInit(auser);
    }
    
    if(!threadsStarted && References() > 0)
      StartPacketProcessing();
    return true;
  }
  
  //: Call to start packet processing.
  
  void NetEndPointBodyC::StartPacketProcessing() {
    if(threadsStarted || shutdown)
      return ;
    threadsStarted = true;
    NetEndPointC me(*this);
    // If the following assert fails, it is because in order
    // to avoid race conditions with reference counting you need
    // setup the connection outside of the constructor of NetEndPointBodyC
    // derived classes. (There must be at least 1 handle to the object. )
    // Contact Charles Galambos if you need further information.
    RavlAssert(References() > 0); 
    LaunchThread(g_stackSizeReceiveThread, Trigger(me,&NetEndPointC::RunReceive));
    LaunchThread(g_stackSizeTransmitThread, Trigger(me, &NetEndPointC::RunTransmit));

#if RAVL_USE_DECODE_THREAD
    if(optimiseThroughput) 
      LaunchThread(me,&NetEndPointC::RunDecode);
#endif
  }
  
  //: Initialise link.
  
  bool NetEndPointBodyC::Ready() {
    if(References() > 0 && !threadsStarted)
      StartPacketProcessing(); // Start processing threads.
    // If the following assert fails, it is because in order
    // to avoid race conditions with reference counting you need
    // setup the connection outside of the constructor of NetEndPointBodyC
    // derived classes. (There must be at least 1 handle to the object. )
    // Contact Charles Galambos if you need further information.
    RavlAssert(threadsStarted); 
    
    if(autoInit)
      return true;
    autoInit = true;
    
    const char *un = getenv("USER"); // This isn't really secure!!
    if(un == 0)
      un = "*unknown*";
    StringC auser(un);
    
    SndInit(auser);
    
    return true;
  }
  
  //: Wait for setup to complete.
  
  bool NetEndPointBodyC::WaitSetupComplete(RealT timeOut) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::WaitSetupComplete(), Called. timeout:" << timeOut);
    RavlAssert(threadsStarted);
    if(!setupComplete.Wait(timeOut))
      return false;
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::WaitSetupComplete(), return IsOpen");
    return IsOpen();
  }
  
  //: Wait for the transmit queue to clear.
  
  bool NetEndPointBodyC::WaitTransmitQClear() {
    // Put an empty packet to ensure all is sent before we return.
    while(!transmitQ.TryPut(NetPacketC(),20)) {
      if(!IsOpen())
        return false;
    }
    
    while(IsOpen() && !transmitQ.IsEmpty())
      Sleep(0.1);
    return true;
  }

  //: Send a 0 parameter message.
  
  bool NetEndPointBodyC::Send(UIntT id) {
    BufOStreamC os;
    BinOStreamC bos(os);
    bos.UseBigEndian(useBigEndianBinStream);
    bos.SetCompatibilityMode32Bit(use32mode);
    bos << id ;
    Transmit(NetPacketC(os.Data()));
    return true;
  }
  
  //: Close connection.
  
  bool NetEndPointBodyC::Close() {
    ONDEBUG(RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::Close(), Called shutdown=" << shutdown << " RefCount=" << References());
    if(shutdown) 
      return true;
    MutexLockC lock(accessMsgReg);
    shutdown = true;
    msgReg.Empty();
    if(ostrm.IsValid())
      ostrm.PutEOS();
    
#if RAVL_USE_DECODE_THREAD
    // Put an empty packet to indicate shutdown.
    if(!receiveQ.TryPut(NetPacketC(),20))
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::Close(), Failed to notify recieveQ of shutdown ";
#endif
    
    // Put an empty packet to indicate shutdown.
    if(!transmitQ.TryPut(NetPacketC(),20))
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::Close(), Failed to notify transmitQ of shutdown ";
    
    return true;
  }
  
  //: Access the name of the connected Host 
  
  StringC NetEndPointBodyC::ConnectedHost(void) { 
    StringC hostName("Unknown");
    istrm.GetAttr("ConnectedHost",hostName);
    return hostName;
  }
  
  //: Access the name of the connected Port 
  
  IntT NetEndPointBodyC::ConnectedPort(void) { 
    IntT portNo = -1;
    istrm.GetAttr("ConnectedPort",portNo);
    return portNo;
  } 
  
  //: Queue a packet for transmission.
  
  bool NetEndPointBodyC::Transmit(const NetPacketC &pkt) { 
    if(shutdown) return false; // Don't Q new stuff if we're shutting down.
    if(pkt.Size() > m_maxPacketSize) {
      RavlSysLog(SYSLOG_ERR) << "Attempt to transmit packet that exceed size limit. Packet size=" << pkt.Size() << "  Limit=" << m_maxPacketSize;
      return false;
    }
    while(!transmitQ.TryPut(pkt,10)) {
      // Has the transmitted been shutdown ?
      if(shutdown)
        return false;
    }
    return true;
  }
  
#if RAVL_BINSTREAM_ENDIAN_LITTLE
  static const StringC streamHeaderLittleEndian ="<RBPS>\n";
#else
  static const StringC streamHeaderBigEndian ="<ABPS>\n";
#endif
  
  //: Handle packet transmission.
  
  bool NetEndPointBodyC::RunTransmit() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Started. ");
    if(!ostrm.IsValid() || !ostrm.IsPutReady()) {
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), ERROR: No connection. ";
      CloseTransmit();
      return false;       
    }
    //skt.SetNoDelay(); // Send packets asap.
#if RAVL_BINSTREAM_ENDIAN_LITTLE
    StringC streamHeader = streamHeaderLittleEndian;
#else
    StringC streamHeader = streamHeaderBigEndian;
#endif
    //ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC::RunTransmit(), Sending header '" << streamHeader << "' ");
    
    // Write stream header.
    if(ostrm.Write(streamHeader,streamHeader.Size()) != (IntT) streamHeader.Size()) {
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), ERROR: Failed to write header. ";
      CloseTransmit();
      return false;
    }
    if(shutdown) {
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Terminated. ";
      CloseTransmit();
      return false;
    }
    
    {
      StringC conHost,conPort;
      if(!ostrm.GetAttr("ConnectedHost",conHost))
        conHost = "Unknown";
      if(!ostrm.GetAttr("ConnectedPort",conPort))
        conPort = "Unknown";
      
      int maxRetry = 60; // Wait up to 60 seconds for connection.
      // Wait still we got a stream header from peer 
      bool issuedMessage = false;
      while(--maxRetry > 0 && !shutdown) {
        if(gotStreamType.Wait(1.0))
          break;
        if(!issuedMessage) // Avoid repeatedly displaying this message.
          RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Waiting for connect from " << conHost << ":" << conPort << " ";
        issuedMessage = true;
        UIntT zeroPacket = 0;
        if(ostrm.Write((char *) &zeroPacket,sizeof(UIntT)) != (IntT)sizeof(UIntT)) {
          RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), ERROR: Failed to write idle packet. ";
          CloseTransmit();
          return false;
        }
      }      
      if(maxRetry < 1) {
        RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), ERROR: Timeout waiting for stream header. ";
        CloseTransmit();
        return false;
      }
      if(issuedMessage) { // If we've complained is late, then let used know it was established ok in the end.
        RavlSysLog(SYSLOG_INFO) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Connection established to " << conHost << ":" << conPort << " ";
      }
    }
    if(shutdown) {
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Terminated. ";
      CloseTransmit();
      return false;
    }
    
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit() Stream mode:" << streamType );
    
#if RAVL_BINSTREAM_ENDIAN_LITTLE
    // Check peer protocol.
    if(streamType == "<ABPS>") { // Client using an old big endian protocol ?
#if RAVL_ENDIAN_COMPATILIBITY
      // We're in compatibility mode, send old header.
      // If so, write <ABPS> header.
      useBigEndianBinStream = true;
      if(ostrm.Write(streamHeaderBigEndian,streamHeader.Size()) < streamHeader.Size()) {
        RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), ERROR: Failed to write header. ";
        CloseTransmit();
        return false;
      }
#else
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), ERROR: Incompatible protocol. ";
      CloseTransmit();
      return false;
#endif
    }
#else
    if(streamType == "<RBPS>") { // Client using an old big endian protocol ?
#if RAVL_ENDIAN_COMPATILIBITY
      // We're in compatibility mode, send old header.
      // If so, write <ABPS> header.
      useBigEndianBinStream = false;
      if(ostrm.Write(streamHeaderBigEndian,streamHeader.Size()) < (IntT) streamHeader.Size()) {
        RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), ERROR: Failed to write header. ";
        CloseTransmit();
        return false;
      }
#else
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), ERROR: Incompatible protocol. ";
      CloseTransmit();
      return false;
#endif
    }
#endif
    const char *bufPtr[2];
    IntT bufLen[2];
    
    ONDEBUG(RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Starting transmit loop. ");
    try {
      IntT timeOutCount =0;
      while(!shutdown) {
        NetPacketC pkt;
        if(!transmitQ.Get(pkt,4)) {
          // Queue a ping packet for processing, this keeps some activity on 
          // the connection and will ensure that the link is actually open. The ping
          // is sent every 20 seconds.
          if(timeOutCount++ > 4) {
            Send(NEPMsgPing,pingSeqNo++,false);
            timeOutCount = 0;
          }
          continue;
        }
        timeOutCount = 0;
        if(shutdown) {
          if(optimiseThroughput)
            ostrm.Cork(false);
          break;
        }
        if(!pkt.IsValid()) {
          if(optimiseThroughput)
            ostrm.Cork(false);
          continue;
        }
        //ONDEBUG(cerr << "  Transmit packet:\n");
        //ONDEBUG(pkt.Dump(cerr));
        UIntT size = pkt.Size();
        if(optimiseThroughput)
          ostrm.Cork(true);
#if RAVL_LITTLEENDIAN
        if(useBigEndianBinStream)
          size = bswap_32(size);
#else
        if(!useBigEndianBinStream)
          size = bswap_32(size);
#endif
        // Write length of packet.
        bufPtr[0] = (char *)&size;
        bufLen[0] = sizeof(size);
        bufPtr[1] = &(pkt.Data()[0]);
        bufLen[1] = pkt.Size();
        if(ostrm.WriteV(bufPtr,bufLen,2) != (IntT) (sizeof(size) + pkt.Size())) {
          RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::WriteV() Failed.  ";
          break;
        }
        if(optimiseThroughput) {
          // Any writes pending ?
          if(transmitQ.IsEmpty())
            ostrm.Cork(false);
        }
        ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "  Sent packet. ");
      }
    } catch(ExceptionC &e) {
      RavlSysLog(SYSLOG_WARNING) << "RAVL Exception :'" << e.what() << "' ";
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Exception caught, terminating link. ";
    } catch(std::exception &e) {
      RavlSysLog(SYSLOG_WARNING) << "C++ Exception :'" << e.what() << "'";
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Exception caught, terminating link. ";
    } catch(...) {
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Exception caught, terminating link. ";
    }
    
    CloseTransmit();
    
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunTransmit(), Terminated. ");
    return true;
  }
  
  //: Close down for transmit thread.
  
  void NetEndPointBodyC::CloseTransmit() {
    // Close down.
    Close();
    
    // Drain transmit Q.
    NetPacketC pkt;
    while(transmitQ.TryGet(pkt)) ;
  }

  //: Decode packet and do call back.

  void NetEndPointBodyC::Dispatch(const NetPacketC &pkt) {
    NetEndPointC me(*this);
    // Direct dispatch
    BinIStreamC is(pkt.DecodeStream());
    is.SetArraySizeLimit(pkt.Size()); // Limit loaded array size to number of bytes in packet.
    is.UseBigEndian(useBigEndianBinStream);
    is.SetCompatibilityMode32Bit(use32mode);
    UIntT msgTypeID = 0;
    is >> msgTypeID;
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Incoming packet type id:" << msgTypeID );
    // Check local decode table.
    NetMsgRegisterC msg = Find(msgTypeID);
    if(!msg.IsValid()) {
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunDecode(), ERROR: Don't know how to decode message type " << msgTypeID << " Shutdown flag=" << shutdown;
      return;
    }
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "Decoding incoming packet. Type: '" << msg.Name() << "'");
    msg.Decode(me,is); 
  }
  
  //: Handle packet reception.
  
  bool NetEndPointBodyC::RunReceive() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Started. ");
    if(!istrm.IsValid() || !istrm.IsGetReady()) {
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC::RunReceive(), ERROR: No connection. ";
      return false;       
    }
    StringC recvStreamType;
    bool errorInHeader = false;
    {
      // Look for ABPS
      const char *str = "<ABPS>\n";
      char buff;
      int state = 0;
      streamType = "<ABPS>";
      do {
        if(istrm.Read(&buff,1) != 1) {
          RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Failed to read byte from input stream. " << errno << " ";
          errorInHeader = true;
          break;
        }
        //RavlSysLog(SYSLOG_DEBUG) << "State=" << state << " char='" << buff << "' ";
        if(str[state] != buff) {
          if(state != 1 || buff != 'R') {
            if(buff != 10) // Accept \n's, they expected.
            {
              RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Unexpected byte in header " << ((int) buff) << " ";
            }
            if(str[0] == buff)
              state = 1;
            else
              state = 0;
            continue;
          } else {
            if(buff == 'R' && state == 1)
              streamType = "<RBPS>";
          }
        }
        state++;
      } while(str[state] != 0) ;
    }
    gotStreamType.Post();
    if(errorInHeader) {
      Close();
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Terminated while reading header";
      return true;
    }
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Connection type confirmed. '" << streamType << "' ");
   
    try {
      while(!shutdown) {
        UIntT size;
        if(istrm.Read((char *) &size,sizeof(UIntT)) != (IntT) sizeof(UIntT)) {
          ONDEBUG(if(!shutdown) RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Read size failed. Assuming connection broken. ");
          break;
        }
        if(size == 0)
          continue;
#if RAVL_LITTLEENDIAN
        //RavlSysLog(SYSLOG_DEBUG) << "Little endian. \n";
        if(useBigEndianBinStream)
          size = bswap_32(size);
#else
        if(!useBigEndianBinStream)
          size = bswap_32(size);
#endif
        ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC::RunReceive(), Read " << size << " bytes. UseBigEndian:" << useBigEndianBinStream << " BigEdian:" << RAVL_ENDIAN_BIG << " ");
        if(size > m_maxPacketSize) {
          RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Very large packet received, assuming stream is corrupt, closing connection. Packet size=" << size << "  Limit=" << m_maxPacketSize;
          break;
        }
        SArray1dC<char> data(size);
        if(istrm.Read((char *) &(data[0]),size) != (IntT) size) {
          ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Read data failed. Assuming connection broken. ");
          break;
        }
        NetPacketC pkt(data);
#if RAVL_USE_DECODE_THREAD
        // Queue'd dispatch.
        if(!optimiseThroughput)
          Dispatch(pkt);
        else {
          while(!receiveQ.TryPut(pkt,10)) {
            // Have we shutdown ?
            if(shutdown)
              break;
          }
        }
#else
        // Direct
        Dispatch(pkt);
#endif
      }
    } catch(ExceptionC &e) {
      RavlSysLog(SYSLOG_ERR) << "RAVL Exception :'" << e.what() << "' ";
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Exception caught, terminating link. ";
    }
#if 0
    catch(exception &e) {
      RavlSysLog(SYSLOG_ERR) << "C++ Exception :'" << e.what() << "'";
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Exception caught, terminating link. ";
    } catch(...) {
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Exception caught, terminating link. ";
    }
#endif
 
#if 0
    if(!nis)
      RavlSysLog(SYSLOG_INFO) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Connection broken ";
#endif
    if(sigConnectionBroken.IsValid()) {
      ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), signalling connection broken.");
      sigConnectionBroken.Invoke();
      sigConnectionBroken.DisconnectAll(true);
    } else {
      RavlSysLog(SYSLOG_ERR) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Internal error: No sigBrokenConnection.";
    }
    
    // Legacy...
    MutexLockC lock(accessMsgReg);
    if(connectionBroken.IsValid()) { // Got a callback ?
      TriggerC call = connectionBroken; // Make copy of trigger in case its overwritten.
      connectionBroken.Invalidate(); // Not needed after this.
      lock.Unlock();
      call.Invoke();
      //      MutexLockC lock(accessMsgReg);
    } else
      lock.Unlock();
    Close();
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunReceive(), Terminated ");
    return true;
  }
  
  //: Decodes incoming packets.
  
  bool NetEndPointBodyC::RunDecode() {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC[" << (void *)this << "]::RunDecode(), Started. ");
    NetPacketC pkt;
    NetEndPointC me(*this);
    try {
      while(1) {
        NetPacketC pkt;
        if(!shutdown) {
          if(!receiveQ.Get(pkt,1.5))
            continue;
        } else {
        // We're shutting down, try and finish processing any
        // remaining packets, but exit when the Q is empty.
          if(!receiveQ.TryGet(pkt))
            break;
        }
        if(!pkt.IsValid())
          continue;
        Dispatch(pkt);
      }
    } catch(ExceptionOperationFailedC &) {
      // protocol error...
      Close();
    } catch(ExceptionC &e) {
      RavlSysLog(SYSLOG_WARNING) << "RAVL Exception :'" << e.what() << "'";
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::RunDecode(), Exception caught, terminating link. ";
    } 
#if 0
    catch(exception &e) {
      RavlSysLog(SYSLOG_WARNING) << "C++ Exception :'" << e.what() << "'";
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::RunDecode(), Exception caught, terminating link. ";
    } catch(...) {
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC[" << (void *)this << "]::RunDecode(), Exception caught, terminating link. ";
    }
#endif
    // Can't to much about receive...
    // Drain the receive Q.
     
    while(receiveQ.TryGet(pkt)) ;
    // Put an empty packet to indicate shutdown. 
    if(!transmitQ.TryPut(NetPacketC(),20)) {
      RavlSysLog(SYSLOG_WARNING) << "NetEndPointBodyC::RunDecode(), Failed to notift recieve queue of close.";
    }
    
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC::RunDecode(), Terminated. ");
    return true;
  }
  
  //: Handle ping message
  
  bool NetEndPointBodyC::MsgPing(IntT seqNo,bool reply) {
    if(reply) {
      //cerr << "NetEndPointBodyC::MsgPing, Got ping reply. \n";
      return true;
    }
    // Reply to ping.
    Send(NEPMsgPing,seqNo,true);
    return true;
  }
  
  //: Init message.
  
  bool NetEndPointBodyC::MsgInit(StringC &user,NetClientInfoC &nPeerInfo) {
    ONDEBUG(RavlSysLog(SYSLOG_DEBUG) << "NetEndPointBodyC::MsgInit(), Called. User:" << user);
    remoteUser = user;
    peerInfo = nPeerInfo;
    // Its up to the user to check the protocol.
    setupComplete.Post();
    return true;
  }
  
  //: Set new trigger to be called if connection broken.
  
  void NetEndPointBodyC::ConnectionBroken(const TriggerC &trigger) {
    MutexLockC lock(accessMsgReg);
    connectionBroken = trigger;
  }
  
  //:------------------------------------------------------------------------------

  //: Constructor.
  
  NetClientInfoC::NetClientInfoC(const StringC &nprotocolName,
				 const StringC &nprotocolVersion,
				 const StringC &nappName,
				 const StringC &nappVersion,
				 const StringC &nhostType
				 )
    : protocol(nprotocolName),
      protocolVersion(nprotocolVersion),
      appName(nappName),
      appVersion(nappVersion),
      hostType(nhostType)
  {}
  
  //: Default Constructor.
  
  NetClientInfoC::NetClientInfoC()
    : protocol("User Protocol"),
      protocolVersion("0.0"),
      appName("Default"),
      appVersion("0.0"),
      hostType("unknown")
  {}
  
  //: Write info to a stream.

  BinOStreamC &operator<<(BinOStreamC &strm,const NetClientInfoC &info) {
    IntT version = 0;
    strm << version;
    strm << info.protocol << info.protocolVersion;
    strm << info.appName << info.appVersion;
    return strm;
  }

  //: Read info from a stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,NetClientInfoC &info) {
    IntT version = 0;
    strm >> version;
    if(version != 0)
      throw ExceptionOutOfRangeC("Unexpected version number in NetClientInfoC io. ");
    strm >> info.protocol >> info.protocolVersion;
    strm >> info.appName >> info.appVersion;
    return strm;
  }

  

}
