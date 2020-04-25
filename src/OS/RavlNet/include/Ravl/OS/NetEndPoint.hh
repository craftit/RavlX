// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_NETENDPOINT_HEADER
#define RAVL_NETENDPOINT_HEADER 1
//////////////////////////////////////////////////////////////////
//! docentry="Basic Types.Network"
//! rcsid="$Id$"
//! example=exEndPoint.cc
//! file="Ravl/OS/Network/NetEndPoint.hh"
//! lib=RavlNet
//! userlevel=Normal
//! docentry="Ravl.API.OS.Network"
//! author="Charles Galambos"
//! date="13/12/2000"

#include "Ravl/OS/Packet.hh"
#include "Ravl/DP/ByteStream.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/Calls.hh"
#include "Ravl/Threads/MessageQueue.hh"
#include "Ravl/Threads/ThreadEvent.hh"
#include "Ravl/Threads/Mutex.hh"
#include "Ravl/Threads/Signal.hh"
#include "Ravl/OS/NetMessage.hh"
#include "Ravl/OS/NetMsgCall.hh"
#include "Ravl/CallMethodRefs.hh"
#include "Ravl/CallMethodPtrs.hh"
#include "Ravl/CallMethods.hh"
#include "Ravl/Threads/Thread.hh"

namespace RavlN {
  class SocketC;
  class NetEndPointC;
  class DateC;
  
  struct NetClientInfoC {
    NetClientInfoC();
    //: Default Constructor.
    
    NetClientInfoC(const StringC &protocolName,
		   const StringC &protocolVersion = StringC("0.0"),
		   const StringC &appName = StringC("default"),
		   const StringC &appVersion = StringC("0.0"),
		   const StringC &hostType = StringC("unknown")
		   );
    //: Constructor.

    void ProtocolName(const StringC &str)
    { protocol = str; }
    //: Set Name of protocol being used.
    
    void ProtocolVersion(const StringC & str)
    { protocolVersion = str; }
    //: Set Version of protocol being used.
    
    void AppName(const StringC &str)
    { appName = str; }
    //: Set Name of application
    
    void AppVersion(const StringC &str)
    { appVersion = str; }
    //: Set Version of application
    
    void HostType(const StringC &str)
    { hostType = str; }
    //: Set type of host.
    
    const StringC &ProtocolName() const
    { return protocol; }
    //: Name of protocol being used.
    
    const StringC &ProtocolVersion() const
    { return protocolVersion; }
    //: Version of protocol being used.
    
    const StringC &AppName() const
    { return appName; }
    //: Name of application
    
    const StringC &AppVersion() const
    { return appVersion; }
    //: Version of application
    
    const StringC &HostType() const
    { return hostType; }
    //: Access type of host.

    StringC protocol;    // Name of protocol being used.
    StringC protocolVersion; // Version of protocol.
    StringC appName;     // Name of application.
    StringC appVersion;  // Version of application.
    StringC hostType;
  };
  
  BinOStreamC &operator<<(BinOStreamC &strm,const NetClientInfoC &info);
  //: Write info to a stream.
  
  BinIStreamC &operator>>(BinIStreamC &strm,NetClientInfoC &info);
  //: Read info from a stream.
  
  //! userlevel=Develop
  //: An end point for a network packet base protocol.
  
  class NetEndPointBodyC 
    : public RCBodyVC
  {
  public:
    NetEndPointBodyC(SocketC &socket,bool autoInit = true,bool optimiseThroughput = false);
    //: Constructor.
    //!param: socket - connect to existing socket
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    //!param: optimiseThroughput - If true optimise for total throughput at the expense of communication latency. If false optimise for latency
    
    NetEndPointBodyC(const StringC &address,bool autoInit = true,bool optimiseThroughput = false);
    //: Constructor.
    //!param: address -  has the format  `host:port' where `host' may be a host name or its IP address (e.g. 122.277.96.255) and `port' is the number of the port to use.
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    //!param: optimiseThroughput - If true optimise for total throughput at the expense of communication latency. If false optimise for latency
    
    NetEndPointBodyC(SocketC &socket,const StringC &protocolName,const StringC &protocolVersion,bool autoInit = true,bool optimiseThroughput = false);
    //: Constructor.
    //!param: socket - connect to existing socket
    //!param: protocolName - Name of communication protocol being used.
    //!param: protocolVersion - Version of communication protocol being used.
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    //!param: optimiseThroughput - If true optimise for total throughput at the expense of communication latency. If false optimise for latency
    
    NetEndPointBodyC(const StringC &address,const StringC &protocolName,const StringC &protocolVersion,bool autoInit = true,bool optimiseThroughput = false);
    //: Constructor.
    //!param: address -  has the format  `host:port' where `host' may be a host name or its IP address (e.g. 122.277.96.255) and `port' is the number of the port to use.
    //!param: protocolName - Name of communication protocol being used.
    //!param: protocolVersion - Version of communication protocol being used.
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    //!param: optimiseThroughput - If true optimise for total throughput at the expense of communication latency. If false optimise for latency
    
    NetEndPointBodyC(const DPIByteStreamC &istrm,const DPOByteStreamC &ostrm,const StringC &protocolName,const StringC &protocolVersion,bool autoInit = true,bool optimiseThroughput = false);
    //: Constructor.
    //!param: istrm - Input communications stream
    //!param: ostrm - Output communications stream
    //!param: protocolName - Name of communication protocol being used.
    //!param: protocolVersion - Version of communication protocol being used.
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    //!param: optimiseThroughput - If true optimise for total throughput at the expense of communication latency. If false optimise for latency
    
    NetEndPointBodyC(bool _optimiseThroughput = false);
    //: Default constructor.
    
    ~NetEndPointBodyC();
    //: Destructor.
    
    void StartPacketProcessing();
    //: Call to start packet processing.
    // This should only be called by the thread that called the constructor in order 
    // to avoid race conditions. Only the first call will have any effect.
    
    static IntT CountOpenConnections();
    //: Return a count of the number of the current number of open connections.
    
    bool IsOpen() const { 
      if(!istrm.IsValid() || !ostrm.IsValid())
        return false;
      return !shutdown && !istrm.IsGetEOS() && ostrm.IsPutReady(); 
    }
    //: Is Connections open ?
    
    bool Init(SocketC &skt);
    //: Setup a connection.
    // This should only be used if net end point 
    // has been created by the default constructor.
    
    bool WaitSetupComplete(RealT timeOut = 60);
    //: Wait for setup to complete.
    
    bool WaitTransmitQClear();
    //: Wait for the transmit queue to clear.
    
    bool Close();
    //: Close connection.

#if 0
    inline SocketC &Socket(void) 
    { return skt ; }
    //: Access the socket 
#endif
    
    StringC ConnectedHost();
    //: Access the name of the connected Host 
    
    IntT ConnectedPort();
    //: Access the name of the connected Port 
    
    const StringC &RemoteUser() const
    { return remoteUser; }
    //: Access name of remote user.
    
    bool Transmit(const NetPacketC &pkt);
    //: Queue a packet for transmission.
    
    bool Ready();
    //: Initialise link.

    void SndInit(StringC &user);
    //: Send init message.
    
    bool Send(UIntT id);
    //: Send a 0 parameter message.
    
    template<class Data1T>
    bool Send(UIntT id,const Data1T &dat1) {
      BufOStreamC os;
      BinOStreamC bos(os);
      bos.UseBigEndian(useBigEndianBinStream);
      bos.SetCompatibilityMode32Bit(use32mode);
      bos << id << dat1;
      return Transmit(NetPacketC(os.Data()));
    }
    //: Send a 1 parameter message.
    
    template<class Data1T,class Data2T>
    bool Send(UIntT id,const Data1T &dat1,const Data2T &dat2) {
      BufOStreamC os;
      BinOStreamC bos(os);
      bos.UseBigEndian(useBigEndianBinStream);
      bos.SetCompatibilityMode32Bit(use32mode);
      bos << id << dat1 << dat2;
      return Transmit(NetPacketC(os.Data()));
    }
    //: Send a 2 parameter message.

    template<class Data1T,class Data2T,class Data3T>
    bool Send(UIntT id,const Data1T &dat1,const Data2T &dat2,const Data3T &dat3) {
      BufOStreamC os;
      BinOStreamC bos(os);
      bos.UseBigEndian(useBigEndianBinStream);
      bos.SetCompatibilityMode32Bit(use32mode);
      bos << id << dat1 << dat2  << dat3;
      return Transmit(NetPacketC(os.Data()));
    }
    //: Send a 3 parameter message.
    
    template<class Data1T,class Data2T,class Data3T,class Data4T>
    bool Send(UIntT id,const Data1T &dat1,const Data2T &dat2,const Data3T &dat3,const Data4T &dat4) {
      BufOStreamC os;
      BinOStreamC bos(os);
      bos.UseBigEndian(useBigEndianBinStream);
      bos.SetCompatibilityMode32Bit(use32mode);
      bos << id << dat1 << dat2  << dat3 << dat4;
      return Transmit(NetPacketC(os.Data()));
    }
    //: Send a 4 parameter message.
    
    bool Register(const NetMsgRegisterC &nmsg);
    //: Register new message handler.
    
    bool Deregister(UIntT id);
    //: Remove message handler for given id.
    
    NetMsgRegisterC Find(UIntT id) const;
    //: Search for message decode/encode of type 'id'.

    template<class ObjT>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)()) {
      return Register(NetMsgCall0C(mid,msgName,CallMethod0C<ObjT &,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class ObjT,class DataT>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(DataT)) {
      return Register(NetMsgCall1C<DataT>(mid,msgName,CallMethod1C<ObjT &,DataT,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.

    template<class ObjT,class Data1T,class Data2T>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T)) {
      return Register(NetMsgCall2C<Data1T,Data2T>(mid,msgName,CallMethod2C<ObjT &,Data1T,Data2T,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class ObjT,class Data1T,class Data2T,class Data3T>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T)) {
      return Register(NetMsgCall3C<Data1T,Data2T,Data3T>(mid,msgName,CallMethod3C<ObjT &,Data1T,Data2T,Data3T,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.

    template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T,Data4T)) {
      return Register(NetMsgCall4C<Data1T,Data2T,Data3T,Data4T>(mid,msgName,CallMethod4C<ObjT &,Data1T,Data2T,Data3T,Data4T,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.

    template<class ObjT>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)()) {
      return Register(NetMsgCall0C(mid,msgName,CallMethod0C<ObjT,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class ObjT,class DataT>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(DataT)) {
      return Register(NetMsgCall1C<DataT>(mid,msgName,CallMethod1C<ObjT,DataT,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.

    template<class ObjT,class Data1T,class Data2T>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T)) {
      return Register(NetMsgCall2C<Data1T,Data2T>(mid,msgName,CallMethod2C<ObjT,Data1T,Data2T,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.

    template<class ObjT,class Data1T,class Data2T,class Data3T>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T)) {
      return Register(NetMsgCall3C<Data1T,Data2T,Data3T>(mid,msgName,CallMethod3C<ObjT,Data1T,Data2T,Data3T,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T,Data4T)) {
      return Register(NetMsgCall4C<Data1T,Data2T,Data3T,Data4T>(mid,msgName,CallMethod4C<ObjT,Data1T,Data2T,Data3T,Data4T,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class HandleT,class ObjT>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)()) {
      return Register(NetMsgCall0C(mid,msgName,CallMethodPtr0C<HandleT,ObjT,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class HandleT,class ObjT,class DataT>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)(DataT)) {
      return Register(NetMsgCall1C<DataT>(mid,msgName,CallMethodPtr1C<HandleT,ObjT,DataT,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.

    template<class HandleT,class ObjT,class Data1T,class Data2T>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)(Data1T,Data2T)) {
      return Register(NetMsgCall2C<Data1T,Data2T>(mid,msgName,CallMethodPtr2C<HandleT,ObjT,Data1T,Data2T,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.

    template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T)) {
      return Register(NetMsgCall3C<Data1T,Data2T,Data3T>(mid,msgName,CallMethodPtr3C<HandleT,ObjT,Data1T,Data2T,Data3T,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class Data4T>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T,Data4T)) {
      return Register(NetMsgCall4C<Data1T,Data2T,Data3T,Data4T>(mid,msgName,CallMethodPtr4C<HandleT,ObjT,Data1T,Data2T,Data3T,Data4T,bool>(obj,func))); 
    }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    TriggerC &ConnectionBroken()
    { return connectionBroken; }
    //: Access trigger called if connection broken.
    // Obsolete, use SigConnectionBroken()
    
    void ConnectionBroken(const TriggerC &);
    //: Set new trigger to be called if connection broken.
    
    Signal0C &SigConnectionBroken()
    { return sigConnectionBroken; }
    //: Access connection broken signal.
    
    const StringC &ProtocolName() const
    { return localInfo.protocol; }
    //: Name of local protocol being used.
    
    const StringC &ProtocolVersion() const
    { return localInfo.protocolVersion; }
    //: Version of local protocol.
    
    const StringC &AppName() const
    { return localInfo.appName; }
    //: Name of local application.
    
    const StringC &AppVersion() const
    { return localInfo.appVersion; }
    //: Version of local application.
    
    NetClientInfoC &LocalInfo()
    { return localInfo; }
    // Info for this application.
    
    NetClientInfoC &PeerInfo()
    { return peerInfo; }
    // Info for remote application.
    
    bool UseBigEndianBinStream() const
    { return useBigEndianBinStream; }
    //: True if using big endian stream in packet composition 
    
    bool Use32BitMode() const
    { return use32mode; }
    //: Test if we're using 32 bit mode.

    void SetPacketSizeLimit(SizeT sizeLimit)
    { m_maxPacketSize = sizeLimit; }
    //: Set the maximum size of any packet that can be handled. 
    
    SizeT PacketSizeLimit() const
    { return m_maxPacketSize; }
    //: Access current packet size limit.
    
  protected:
    void Dispatch(const NetPacketC &pkt);
    //: Decode packet and do call back.
    
    bool MsgInit(StringC &user,NetClientInfoC &info);
    //: Init message.
    
    bool MsgPing(IntT seqNo,bool reply);
    //: Handle ping message
    
    bool RunTransmit();
    //: Handle packet transmission.
    
    bool RunReceive();
    //: Handle packet reception.
    
    bool RunDecode();
    //: Decodes incoming packets.
    
    void CloseTransmit();
    //: Close down for transmit thread.
    
    void CloseDecode();
    //: Close down for decode thread.
    
    DPIByteStreamC istrm; // Input channel 
    DPOByteStreamC ostrm; // Output channel
    
    MessageQueueC<NetPacketC> transmitQ; // Transmission Q.
    MessageQueueC<NetPacketC> receiveQ; // Receive Q.
    volatile bool shutdown;   // Shutdown system ?
    ThreadEventC setupComplete;
    ThreadEventC gotStreamType;
    StringC streamType;
    StringC remoteUser;
    MutexC accessMsgReg;
    HashC<UIntT,NetMsgRegisterC> msgReg;  // Local register of decoding routines.
    friend class NetEndPointC;
    bool autoInit;
    TriggerC connectionBroken; // Trigger called if connection broken.
    Signal0C sigConnectionBroken;
    
    NetClientInfoC localInfo; // Info for this application.
    NetClientInfoC peerInfo;  // Info for remote application.
    bool useBigEndianBinStream;
    bool use32mode;
    IntT pingSeqNo;          // Sequence number used for pings.
    bool optimiseThroughput; // Optimise through put at the expense of latency.
    bool threadsStarted;
    SizeT m_maxPacketSize;   // Maximum size of any packet that can be handled.
    IntT m_port;
  };
  
  //! userlevel=Normal
  //:  An end point for a network packet base protocol.

  // <p>This class provides a peer to peer communication mechanism for
  // operation over a network.  The user can register functions to be
  // called when messages (identified by an ID number) are sent over
  // the connection. Sending and receiving are handled asynchronously:
  // one thread handles sending messages from the transmit queue,
  // whilst the other handles decoding of messages and dispatching
  // them to the receiver functions.</p> 

  // <p>Note: There is only 1 receiver thread, if you wish to process
  // other incoming messages whilst handling a long request you should
  // use LaunchThread() to create a separate thread for handling
  // it.</p> 

  // <p>Note: The processing threads hold a reference to this class and
  // it will not be destroyed until the connection is closed down,
  // either with the 'Close()' method or by losing the connection.</p>
  
  class NetEndPointC 
    : public RCHandleC<NetEndPointBodyC>
  {
  public:
    NetEndPointC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    NetEndPointC(SocketC &socket,bool autoInit = true,bool optimiseThroughput = false)
      : RCHandleC<NetEndPointBodyC>(*new NetEndPointBodyC(socket,autoInit,optimiseThroughput))
    { StartPacketProcessing(); }
    //: Constructor.  
    //!param: socket - connect to existing socket
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    
    NetEndPointC(const StringC &address,bool autoInit = true,bool optimiseThroughput = false)
      : RCHandleC<NetEndPointBodyC>(*new NetEndPointBodyC(address,autoInit,optimiseThroughput))
    { StartPacketProcessing(); }
    //: Constructor.
    // This connects to the given port address. <p>
    //!param: address -  has the format  `host:port' where `host' may be a host name or its IP address (e.g. 122.277.96.255) and `port' is the number of the port to use.
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    
    NetEndPointC(const DPIByteStreamC &istrm,const DPOByteStreamC &ostrm,const StringC &protocolName,const StringC &protocolVersion,bool autoInit = true,bool optimiseThroughput = false)
      : RCHandleC<NetEndPointBodyC>(*new NetEndPointBodyC(istrm,ostrm,protocolName,protocolVersion,autoInit,optimiseThroughput))
    { StartPacketProcessing(); }
    //: Constructor.
    //!param: istrm - Input communications stream
    //!param: ostrm - Output communications stream
    //!param: protocolName - Name of communication protocol being used.
    //!param: protocolVersion - Version of communication protocol being used.
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    
    NetEndPointC(SocketC &socket,const StringC &protocolName,const StringC &protocolVersion,bool autoInit = true,bool optimiseThroughput = false)
      : RCHandleC<NetEndPointBodyC>(*new NetEndPointBodyC(socket,protocolName,protocolVersion,autoInit,optimiseThroughput))
    { StartPacketProcessing(); }
    //: Constructor.
    //!param: socket - connect to existing socket
    //!param: protocolName - Name of communication protocol being used.
    //!param: protocolVersion - Version of communication protocol being used.
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    //!param: optimiseThroughput - If true optimise for total throughput at the expense of communication latency. If false optimise for latency
    
    NetEndPointC(const StringC &address,const StringC &protocolName,const StringC &protocolVersion,bool autoInit = true,bool optimiseThroughput = false) 
      : RCHandleC<NetEndPointBodyC>(*new NetEndPointBodyC(address,protocolName,protocolVersion,autoInit,optimiseThroughput))
    { StartPacketProcessing(); }
    //: Constructor.
    //!param: address -  has the format  `host:port' where `host' may be a host name or its IP address (e.g. 122.277.96.255) and `port' is the number of the port to use.
    //!param: protocolName - Name of communication protocol being used.
    //!param: protocolVersion - Version of communication protocol being used.
    //!param: autoInit - If false, you must call the Ready() function when you are ready to start processing network messages. If true, messages will start being processed as soon as the connection is established.
    //!param: optimiseThroughput - If true optimise for total throughput at the expense of communication latency. If false optimise for latency

    explicit NetEndPointC(bool)
      : RCHandleC<NetEndPointBodyC>(*new NetEndPointBodyC())
    {}
    //: Create an uninitialised end point.
    
  protected:
    NetEndPointC(NetEndPointBodyC &bod)
      : RCHandleC<NetEndPointBodyC>(bod)
    {}
    //: Body constructor.
    
    NetEndPointC(NetEndPointBodyC *bod)
      : RCHandleC<NetEndPointBodyC>(bod)
    {}
    //: Body constructor.
    
    NetEndPointBodyC &Body() 
    { return RCHandleC<NetEndPointBodyC>::Body(); }
    //: Access body.

    const NetEndPointBodyC &Body() const
    { return RCHandleC<NetEndPointBodyC>::Body(); }
    //: Access body.

    bool RunTransmit()
    { return Body().RunTransmit(); }
    //: Handle packet transmission.
    
    bool RunReceive()
    { return Body().RunReceive(); }
    //: Handle packet reception.
    
    bool RunDecode()
    { return Body().RunDecode(); }
    //: Decodes incoming packets.
    
    void StartPacketProcessing()
    { return Body().StartPacketProcessing(); }
    //: Start processing packets.
  public:    
    bool IsOpen() const
    { return Body().IsOpen(); }
    //: Is Connections open ?
    
    static IntT CountOpenConnections()
    { return NetEndPointBodyC::CountOpenConnections(); }
    //: Return a count of the number of the current number of open connections.
    
#if 0
    inline SocketC Socket (void) 
    { return Body().Socket() ; } 
    //: Access the socket 
#endif

    inline StringC ConnectedHost (void) 
    { return Body().ConnectedHost() ; } 
    //: Access the connected host 

    inline IntT ConnectedPort (void) 
    { return Body().ConnectedPort() ; } 
    //: Access the connected port

    const StringC &RemoteUser() const
    { return Body().RemoteUser(); }
    //: Access name of remote user.
    
    bool Init(SocketC &skt)
    { return Body().Init(skt); }
    //: Setup a connection.
    // This should only be used if net end point 
    // has been created by the default constructor.
    
    bool Ready()
    { return Body().Ready(); }
    //: Call when your ready to received data from the network..
    
    bool WaitSetupComplete(RealT timeOut = 30)
    { return Body().WaitSetupComplete(timeOut); }
    //: Wait for setup to complete.
    // May fail if peer drops connection for some reason.
    
    bool WaitTransmitQClear()
    { return Body().WaitTransmitQClear(); }
    //: Wait for the transmit queue to clear.
    // Will return immediately if the NetEndPoint is closed.
    
    bool Close()
    { return Body().Close(); }
    //: Close connection.
    
    bool Transmit(const NetPacketC &pkt)
    { return Body().Transmit(pkt); }
    //: Queue a packet for transmission.
    
    bool MsgInit(StringC &user,NetClientInfoC &info)
    { return  Body().MsgInit(user,info); }
    //: Init message.

    bool Register(const NetMsgRegisterC &nmsg)
    { return Body().Register(nmsg); }
    //: Register new message handler.
    
    bool Deregister(UIntT id)
    { return Body().Deregister(id); }
    //: Remove message handler for given id.
    
    bool Send(UIntT id)
    { return Body().Send(id); }
    //: Send a 0 parameter message.
    
    template<class Data1T>
    bool Send(UIntT id,const Data1T &dat1)
    { return Body().Send(id,dat1); }
    //: Send a 1 parameter message.
    
    template<class Data1T,class Data2T>
    bool Send(UIntT id,const Data1T &dat1,const Data2T &dat2)
    { return Body().Send(id,dat1,dat2); }
    //: Send a 2 parameter message.

    template<class Data1T,class Data2T,class Data3T>
    bool Send(UIntT id,const Data1T &dat1,const Data2T &dat2,const Data3T &dat3)
    { return Body().Send(id,dat1,dat2,dat3); }
    //: Send a 3 parameter message.
    
    template<class Data1T,class Data2T,class Data3T,class Data4T>
    bool Send(UIntT id,const Data1T &dat1,const Data2T &dat2,const Data3T &dat3,const Data4T &dat4)
    { return Body().Send(id,dat1,dat2,dat3,dat4); }
    //: Send a 4 parameter message.
    
    template<class ObjT>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)())
    { return Body().RegisterR(mid,msgName,obj,func); }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class ObjT,class DataT>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(DataT ))
    { return Body().RegisterR(mid,msgName,obj,func); }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.

    template<class ObjT,class Data1T,class Data2T>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T ,Data2T ))
    { return Body().RegisterR(mid,msgName,obj,func); }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.

    template<class ObjT,class Data1T,class Data2T,class Data3T>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T ,Data2T ,Data3T ))
    { return Body().RegisterR(mid,msgName,obj,func); }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T>
    bool RegisterR(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T,Data4T))
    { return Body().RegisterR(mid,msgName,obj,func); }
    //: Register new message handler.
    // NB. This does not make a handle to 'obj', it is the users responsibility to 
    // ensure it is not deleted.
    
    template<class ObjT>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)())
    { return Body().Register(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.
    
    template<class ObjT,class DataT>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(DataT ))
    { return Body().Register(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.
    
    template<class ObjT,class Data1T,class Data2T>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T))
    { return Body().Register(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.

    template<class ObjT,class Data1T,class Data2T,class Data3T>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T))
    { return Body().Register(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.
    
    template<class ObjT,class Data1T,class Data2T,class Data3T,class Data4T>
    bool Register(UIntT mid,const StringC &msgName,ObjT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T,Data4T))
    { return Body().Register(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.
    
    template<class HandleT,class ObjT>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)())
    { return Body().RegisterPtr(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.
    
    template<class HandleT,class ObjT,class DataT>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)(DataT ))
    { return Body().RegisterPtr(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.
    
    template<class HandleT,class ObjT,class Data1T,class Data2T>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)(Data1T,Data2T))
    { return Body().RegisterPtr(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.

    template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T))
    { return Body().RegisterPtr(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.
    
    template<class HandleT,class ObjT,class Data1T,class Data2T,class Data3T,class Data4T>
    bool RegisterPtr(UIntT mid,const StringC &msgName,const HandleT &obj,bool (ObjT::*func)(Data1T,Data2T,Data3T,Data4T))
    { return Body().RegisterPtr(mid,msgName,obj,func); }
    //: Register new message handler.
    // Hold a handle to the object called.
    
    TriggerC &ConnectionBroken()
    { return Body().ConnectionBroken(); }
    //: Access trigger called if connection broken.
    // OBSOLETE: Use SigConnectionBroken().
    
    Signal0C &SigConnectionBroken()
    { return Body().SigConnectionBroken(); }
    //: Access connection broken signal.
    
    void ConnectionBroken(const TriggerC &trigger)
    { Body().ConnectionBroken(trigger); }
    //: Set new trigger to be called if connection broken.
    // OBSOLETE: Use SigConnectionBroken().
    
    const StringC &ProtocolName() const
    { return Body().ProtocolName(); }
    //: Name of protocol being used.
    
    const StringC &ProtocolVersion() const
    { return Body().ProtocolVersion(); }
    //: Version of protocol.
    
    const StringC &AppName() const
    { return Body().AppName(); }
    //: Name of application.
    
    const StringC &AppVersion() const
    { return Body().AppVersion(); }
    //: Version of application.
    
    NetClientInfoC &LocalInfo()
    { return Body().LocalInfo(); }
    // Info for this application.
    
    NetClientInfoC &PeerInfo()
    { return Body().PeerInfo(); }
    // Info for remote application.

    bool UseBigEndianBinStream() const
    { return Body().UseBigEndianBinStream(); }
    //: True if using big endian stream in packet composition 

    bool Use32BitMode() const
    { return Body().Use32BitMode(); }
    //: Test if we're using 32 bit mode.
    // Return true if we are.

    void SetPacketSizeLimit(SizeT sizeLimit)
    { Body().SetPacketSizeLimit(sizeLimit); }
    //: Set the maximum size of any packet that can be handled. 
    
    SizeT PacketSizeLimit() const
    { return Body().PacketSizeLimit(); }
    //: Access current packet size limit.
    
    friend class NetEndPointBodyC;
  };
  
  
}

#endif
