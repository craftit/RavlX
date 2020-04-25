// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlNet
//! file="Ravl/OS/Network/NetByteStream.cc"

#include "Ravl/OS/NetByteStream.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/DP/AttributeValueTypes.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Default constructor.
  
  NetOByteStreamBodyC::NetOByteStreamBodyC()
  { Init(); }

  //: Constructor.
  
  NetOByteStreamBodyC::NetOByteStreamBodyC(const SocketC &_socket) 
    : socket(_socket)
  { Init(); }

  //: Construct from address string
  
  NetOByteStreamBodyC::NetOByteStreamBodyC(const StringC &address)
    : socket(address,false)
  { Init(); }

  //: Cork stream.  
  // This is used to indicate that there is going to
  // be several write operations immediatly following each
  // other and stops the transmition of fragmented packets.
  // If your not expecting to do any more writes immediatly
  // you must call 'Uncork()'.
  
  bool NetOByteStreamBodyC::Cork(bool enable) {
    return socket.Cork(enable);
  } 
  
  //: Build attribute lists etc.
  
  void NetOByteStreamBodyC::Init() {
    RegisterAttribute(AttributeTypeNumC<IntT>("ConnectedPort","Port number from connected host",true,false,0,RavlConstN::maxInt,1,0));
    RegisterAttribute(AttributeTypeStringC(   "ConnectedHost","Name of connected host"         ,true,false,"localhost"));
  }
  
  //: Put End Of Stream marker.
  
  void NetOByteStreamBodyC::PutEOS() 
  { socket.Close(); }
  
  //: Is port ready for data ?
  
  bool NetOByteStreamBodyC::IsPutReady() const { 
    ONDEBUG(cerr << "NetOByteStreamBodyC::IsPutReady() \n");
    if(!socket.IsValid()) return false;
    return socket.IsOpen(); 
  }
  
  //: Put data.
  
  bool NetOByteStreamBodyC::Put(const ByteT &data) 
  { return socket.Write(reinterpret_cast<const char *>(&data),1) == 1; }
  
  //: Put an array of data to stream.
  // returns number of elements processed.
  
  IntT NetOByteStreamBodyC::PutArray(const SArray1dC<ByteT> &data) {
    if(data.Size() == 0) return 0;
    return socket.Write(reinterpret_cast<const char *>(&data[0]),data.Size());
  }
  
  //: Write data to stream
  //!param: buffer - Buffer to write data from.
  //!param: len - length of buffer.
  //!return: true if write succeeded.
  
  IntT NetOByteStreamBodyC::Write(const char *buffer,IntT len) {
    return socket.Write(buffer,len);
  }

  //: Write data to stream
  // This routine works as Write(), but will handle temporary errors and
  // do its best to ensure the complete buffer is written.
  //!param: buffer - Buffer to write data from.
  //!param: len - length of buffer.
  //!return: Number of bytes written, or -1 on error.
  
  IntT NetOByteStreamBodyC::WriteAll(const char *buffer,IntT len) {
    return socket.Write(buffer,len);
  }
  
  //: Write data to stream
  //!param: buffer - Array of buffers to write data from.
  //!param: len - Array of lengths of the buffers.
  //!param: n - Number of buffers involved.
  //!return: Number of bytes written, or -1 on error.
  
  IntT NetOByteStreamBodyC::WriteV(const char **buffer,IntT *len,int n) {
    return socket.WriteV(buffer,len,n);
  }
    
  //: Write data to stream
  // This routine works as Write(), but will handle temporary errors and
  // do its best to ensure the complete buffer is written.
  //!param: buffer - Array of buffers to write data from.
  //!param: len - Array of lengths of the buffers.
  //!param: n - Number of buffers involved.
  //!return: Number of bytes written, or -1 on error.
  
  IntT NetOByteStreamBodyC::WriteAllV(const char **buffer,IntT *len,int n) {
    return socket.WriteV(buffer,len,n);
  }
  
  //: Get a attribute.
  // Returns false if the attribute name is unknown.
  
  bool NetOByteStreamBodyC::GetAttr(const StringC &attrName,StringC &attrValue) {
    if(attrName == "ConnectedHost") {
      attrValue = socket.ConnectedHost();
      return true;
    }
    return DPPortBodyC::GetAttr(attrName,attrValue);
  }
  
  //: Get a attribute.
  // Returns false if the attribute name is unknown.
  
  bool NetOByteStreamBodyC::GetAttr(const StringC &attrName,IntT &attrValue) {
    if(attrName == "ConnectedPort") {
      attrValue = socket.ConnectedPort();
      return true;
    }
    return DPPortBodyC::GetAttr(attrName,attrValue);    
  }
  
  //------------------------------------------------------------------------------
  
  //: Default constructor.
  
  NetIByteStreamBodyC::NetIByteStreamBodyC()
  { Init(); }
  
  //: Constructor.
  
  NetIByteStreamBodyC::NetIByteStreamBodyC(const SocketC &_socket) 
    : socket(_socket)
  { Init(); }
  
  //: Constructor from address string
  
  NetIByteStreamBodyC::NetIByteStreamBodyC(const StringC &address)
    : socket(address,false)
  { Init(); }

  //: Build attribute lists etc.
  
  void NetIByteStreamBodyC::Init() {
    RegisterAttribute(AttributeTypeNumC<IntT>("ConnectedPort","Port number from connected host",true,false,0,RavlConstN::maxInt,1,0));
    RegisterAttribute(AttributeTypeStringC(   "ConnectedHost","Name of connected host"         ,true,false,"localhost"));
  }
  
  //: Is some data ready ?
  // true = yes.
  // Defaults to !IsGetEOS().
  
  bool NetIByteStreamBodyC::IsGetReady() const { 
    if(!socket.IsValid()) return false;
    return socket.IsOpen(); 
  }
  
  //: Has the End Of Stream been reached ?
  // true = yes.
  
  bool NetIByteStreamBodyC::IsGetEOS() const { 
    if(!socket.IsValid()) return false;
    return !socket.IsOpen(); 
  }
  
  //: Get next piece of data.
  // May block if not ready, or it could throw an 
  // DataNotReadyC exception.
  
  ByteT NetIByteStreamBodyC::Get() {
    if(!socket.IsOpen()) throw DataNotReadyC("NetIByteStreamBodyC::Get(), Bad socket.");
    ByteT buff;
    if(socket.Read(reinterpret_cast<char *>(&buff),1) != 1)
      throw DataNotReadyC("NetIByteStreamBodyC::Get(), Read failed. ");
    return buff;
  }
  
  //: Try and get next piece of data.
  // This may not NOT block, if no data is ready
  // it will return false, and not set buff.
  
  bool NetIByteStreamBodyC::Get(ByteT &buff) {
    if(!socket.IsOpen()) return false;
    return socket.Read(reinterpret_cast<char *>(&buff),1) == 1;
  }
  
  //: Get an array of data from stream.
  // returns the number of elements succesfully processed.
  
  IntT NetIByteStreamBodyC::GetArray(SArray1dC<ByteT> &data) {
    if(!socket.IsOpen() || data.Size() == 0) return 0;
    return socket.Read(reinterpret_cast<char *>(&data[0]),data.Size());
  }
  
  //: Read data from stream info buffer.
  //!param: buffer - Buffer to read data into.
  //!param: len - length of buffer.
  //!return: Number of bytes read or -1 on error
  
  IntT NetIByteStreamBodyC::Read(char *buffer,IntT len) 
  { return socket.Read(buffer,len); }
  
  //: Read data from stream info buffer.
  // This routine works as Read(), but will handle temporary errors and do
  // its best to ensure a full buffer is read.
  //!param: buffer - Buffer to read data into.
  //!param: len - length of buffer.
  //!return: Number of bytes read or -1 on error
  
  IntT NetIByteStreamBodyC::ReadAll(char *buffer,IntT len) 
  { return socket.Read(buffer,len); }
  
  //: Read data to stream
  //!param: buffer - Array of buffers to read data into.
  //!param: len - Array of lengths of the buffers.
  //!param: n - Number of buffers involved.
  //!return: Number of bytes read or -1 on error
  
  IntT NetIByteStreamBodyC::ReadV(char **buffer,IntT *len,int n) 
  { return socket.ReadV(buffer,len,n); }
  
  //: Read data to stream
  // This routine works as Read(), but will handle temporary errors and do
  // its best to ensure a full buffer is read.
  //!param: buffer - Array of buffers to read data into.
  //!param: len - Array of lengths of the buffers.
  //!param: n - Number of buffers involved.
  //!return: Number of bytes read or -1 on error
  
  IntT NetIByteStreamBodyC::ReadAllV(char **buffer,IntT *len,int n) 
  { return socket.ReadV(buffer,len,n); }
  
  //: Get a attribute.
  // Returns false if the attribute name is unknown.
  
  bool NetIByteStreamBodyC::GetAttr(const StringC &attrName,StringC &attrValue) {
    if(attrName == "ConnectedHost") {
      attrValue = socket.ConnectedHost();
      return true;
    }
    return DPPortBodyC::GetAttr(attrName,attrValue);
  }
  
  //: Get a attribute.
  // Returns false if the attribute name is unknown.
  
  bool NetIByteStreamBodyC::GetAttr(const StringC &attrName,IntT &attrValue) {
    if(attrName == "ConnectedPort") {
      attrValue = socket.ConnectedPort();
      return true;
    }
    return DPPortBodyC::GetAttr(attrName,attrValue);    
  }
  
}
