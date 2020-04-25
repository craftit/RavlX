// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

/////////////////////////////////////////////////////////////
//! userlevel=Normal
//! date="15/09/2010"
//! author="Alex Kostin"
//! docentry="Ravl.API.OS.Hardware"
//! rcsid="$Id$"
//! lib=RavlSerialIO
//! file="Ravl/OS/Misc/SerialAbstract.cc"

#include "Ravl/OS/SerialAbstract.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Exception.hh"

#define ISSUE_ABSTRACT_ERROR(message) \
  RavlAssertMsg(0,message); \
  throw ExceptionOperationFailedC(message);

namespace RavlN {
  
  //: Base class to control serial ports.

  //! default constructor;
  SerialAbstractC::SerialAbstractC() {
  }

  //! open a device for initialize;
  SerialAbstractC::SerialAbstractC(const char *dev, const char * perm, bool nonBlocking) {
  }

  //! Open device.
  bool SerialAbstractC::Open(const char *dev, const char * perm,bool nonBlocking) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::Open: Abstract method called.");
    return false;
  }

  //! Close the socket.
  // Note this will only actually close the socket if
  // m_dontClose is false.
  void SerialAbstractC::Close() {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::Close: Abstract method called.");
  }

  //! Setup the port.
  // with the given parameters: Input speed, Output speed,
  // stop bit(1,2), parity(exist or not exist), parity type(odd or even) and character size(5,6,7,8)
  bool SerialAbstractC::Setup(IntT i_speed, IntT o_speed, IntT stop_bit, ParityT par, IntT char_size) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::Setup: Abstract method called.");
    return false;
  }

  //! Reset interface.
  void SerialAbstractC::Reset() {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::Reset: Abstract method called.");
  }

  //! Flush buffer.
  void SerialAbstractC::Flush() {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::Flush: Abstract method called.");
  }

  //! Is stream open ?
  bool SerialAbstractC::IsOpen() const {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::IsOpen: Abstract method called.");
    return false;
  }

  //! Read some bytes from a stream.
  IntT SerialAbstractC::Read(char *buff,UIntT size) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::Read: Abstract method called.");
    return 0;
  }

  //! Read some bytes from a stream.
  IntT SerialAbstractC::ReadV(char **buffer,IntT *len,int n) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::ReadV: Abstract method called.");
    return 0;
  }

  //! Write some bytes to a stream.
  IntT SerialAbstractC::Write(const char *buff,UIntT size) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::Write: Abstract method called.");
    return 0;
  }

  //! Write multiple buffers
  IntT SerialAbstractC::WriteV(const char **buffer,IntT *len,int n) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::WriteV: Abstract method called.");
    return 0;
  }

  //! set the input speed of the port;
  bool SerialAbstractC::SetISpeed(const IntT i_speed) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetISpeed: Abstract method called.");
    return false;
  }

  //! set the output speed of the port;
  bool SerialAbstractC::SetOSpeed(const IntT o_speed) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetOSpeed: Abstract method called.");
    return false;
  }

  //! set the number of stop bits : 1 or 2;
  bool SerialAbstractC::SetStopBits(const IntT stop_bit) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetStopBits: Abstract method called.");
    return false;
  }

  //! set the character size: 5,6,7,8;
  bool SerialAbstractC::SetCharSize(const IntT char_size) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetCharSize: Abstract method called.");
    return false;
  }

  //! parity type: Odd or Even or None or 1
  bool SerialAbstractC::SetParity(ParityT par) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetParity: Abstract method called.");
    return false;
  }
    
  //! Access file descriptor.
  IntT SerialAbstractC::Fd() const {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::Fd: Abstract method called.");
    return 0;
  }

  //! Set the amount of time you should attempt to read from a file descriptor.
  // This limits the time spent attempting to write to a socket
  // without reading a single byte. The default is 120 seconds.
  void SerialAbstractC::SetReadTimeout(float timeOut) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetReadTimeout: Abstract method called.");
  }

  //! Set the amount of time you should attempt to write to a file descriptor.
  // This limits the time spent attempting to write to a socket
  // without sending a single byte. The default is 120 seconds.
  void SerialAbstractC::SetWriteTimeout(float timeOut) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetWriteTimeout: Abstract method called.");
  }

  //! Enable non-blocking use of read and write.
  // true= read and write's won't do blocking waits.
  bool SerialAbstractC::SetNonBlocking(bool block) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetNonBlocking: Abstract method called.");
    return false;
  }
    
  //! Setup don't close flag.
  void SerialAbstractC::SetDontClose(bool ndontClose) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetDontClose: Abstract method called.");
  }
    
  //! Should read's fail on timeout ?
  // If false, the socket will be checked its
  // open and valid, if it is the read will be retried.
  void SerialAbstractC::SetFailOnReadTimeout(bool val) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetFailOnReadTimeout: Abstract method called.");
  }

  //! Should write's fail on timeout ?
  // If false, the socket will be checked its
  // open and valid, if it is the write will be retried.
  void SerialAbstractC::SetFailOnWriteTimeout(bool val) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetFailOnWriteTimeout: Abstract method called.");
  }
    
  //! Should read functions keep reading until the buffer is full?
  // If not the read will return immediately once any has been read.
  void SerialAbstractC::SetFillBufferOnRead(bool value) {
    ISSUE_ABSTRACT_ERROR("SerialAbstractC::SetFillBufferOnRead: Abstract method called.");
  }

}

