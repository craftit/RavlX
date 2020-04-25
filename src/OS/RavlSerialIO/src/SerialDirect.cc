// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010-12, University of Surrey.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

/////////////////////////////////////////////////////////////
//! userlevel=Normal
//! date="16/09/2010"
//! author="Alex Kostin"
//! docentry="Ravl.API.OS.Hardware"
//! rcsid="$Id$"
//! lib=RavlSerialIO
//! file="Ravl/OS/Misc/SerialAbstract.cc"


#include "Ravl/OS/SerialDirect.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Exception.hh"

#if RAVL_OS_SOLARIS
#include <sys/open.h>
#endif

#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#if RAVL_HAVE_TERMIOS
#include <sys/termios.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#if RAVL_HAVE_ANSICPPHEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

#define ISSUE_FATAL_ERROR(message) \
  RavlAssertMsg(0,message); \
  throw ExceptionOperationFailedC(message);


namespace RavlN {
  
//: Get setting to use for a bit rate.
// returns -1 for illegal values.
static int SpeedSetting(int bitrate) {
  int speedVal = -1;
#if RAVL_HAVE_TERMIOS
  switch(bitrate)
    {
    case 0:  speedVal = B0; break;
    case 50: speedVal = B50; break;
    case 75: speedVal = B75; break;
    case 110: speedVal = B110; break;
    case 134: speedVal = B134; break;
    case 150: speedVal = B150; break;
    case 200: speedVal = B200; break;
    case 300: speedVal = B300; break;
    case 600: speedVal = B600; break;
    case 1200: speedVal = B1200; break;
    case 1800: speedVal = B1800; break;
    case 2400: speedVal = B2400; break;
    case 4800: speedVal = B4800; break;
    case 9600: speedVal = B9600; break;
    case 19200: speedVal = B19200; break;
    case 38400: speedVal = B38400; break;
#ifdef B57600
    case 57600: speedVal = B57600; break;
#endif
#ifdef B115200
    case 115200: speedVal = B115200; break;
#endif
#ifdef B153600
    case 153600: speedVal = B153600; break;
#endif
#ifdef B230400
    case 230400: speedVal = B230400; break;
#endif
#ifdef B307200
    case 307200: speedVal = B307200; break;
#endif
#ifdef B460800
    case 460800: speedVal = B460800; break;
#endif
#ifdef B500000
    case 500000: speedVal = B500000; break;
#endif
#ifdef B576000
    case 576000: speedVal = B576000; break;
#endif
#ifdef B921600
    case 921600: speedVal = B921600; break;
#endif
#ifdef  B1000000
    case 1000000: speedVal = B1000000; break;
#endif
#ifdef  B1152000
    case 1152000: speedVal = B1152000; break;
#endif
#ifdef  B1500000
    case 1500000: speedVal = B1500000; break;
#endif
#ifdef  B2000000
    case 2000000: speedVal = B2000000; break;
#endif
#ifdef  B2500000
    case 2500000: speedVal = B2500000; break;
#endif
#ifdef  B3000000
    case 3000000: speedVal = B3000000; break;
#endif
#ifdef  B3500000
    case 3500000: speedVal = B3500000; break;
#endif
#ifdef  B4000000
    case 4000000: speedVal = B4000000; break;
#endif
    }
#endif
  return speedVal;
}


//: Base class to control serial ports.

//! default constructor;
SerialDirectC::SerialDirectC() {
}

//! open a device for initialize;
SerialDirectC::SerialDirectC(const char *dev, const char *perm, bool nonBlocking) {
  Open(dev, perm, nonBlocking);
}

//! Open device.
bool SerialDirectC::Open(const char *dev, const char * perm,bool nonBlocking) {
  ONDEBUG(cerr << "SerialDirectC::Open:" << dev << " perm:" << perm << endl;)
#if RAVL_HAVE_TERMIOS
  int openFlags = O_NOCTTY;
  if(nonBlocking) {
    openFlags = openFlags | O_NONBLOCK;
#ifdef O_NDELAY
    openFlags = openFlags | O_NDELAY;
#endif
  }
  if(strcmp(perm, "RDONLY")==0)
    openFlags |= O_RDONLY;
  else if ( strcmp(perm, "WRONLY")==0)
    openFlags |=  O_WRONLY;
  else if(strcmp(perm,"RDWR")==0)
    openFlags |=  O_RDWR;
  else {
    cerr<<"SerialCtrlC::SerialCtrlC(), ERROR: Unknown permission '" << perm << "' should be one of 'RDONLY', 'WRONLY' or 'RDWR'. "<<endl;
    openFlags |=  O_RDWR;
  }
  m_streamIo.Fd() = open(dev,openFlags);

  if(m_streamIo.Fd() < 0) {
    cout << "ERROR: Failed to open serial port '" << dev << "' "<<endl;
    return false;
  }
#else
  cerr << "WARNING: Control of serial ports is not currently supported on this platform. ";
#endif
  return true;
}

//! Close the socket.
// Note this will only actually close the socket if m_dontClose is false.
void SerialDirectC::Close() {
  m_streamIo.Close();
}

//! Setup the port.
// with the given parameters: Input speed, Output speed,
// stop bit(1,2), parity(exist or not exist), parity type(odd or even) and character size(5,6,7,8)
bool SerialDirectC::Setup(IntT i_speed, IntT o_speed, IntT stop_bits, ParityT par, IntT char_size) {
  ONDEBUG(cerr << "SerialDirectC::Setup:" << i_speed << "  " << o_speed << "  " << stop_bits << "  " << int(par) << "  " << char_size << endl);
#if RAVL_HAVE_TERMIOS
  if(!m_streamIo.IsOpen()) {
    cerr << "SerialCtrlC::Setup(), No file descriptor for port, can't configure. \n";
    return false;
  }
  termios termios_p;
  if (tcgetattr(m_streamIo.Fd(),&termios_p) < 0) {
    cerr << "SerialCtrlC::Setup(), Failed to read port parameters. \n";
    return false;
  }

  termios_p.c_iflag = IGNBRK;
  termios_p.c_oflag = 0;
  termios_p.c_cflag = CLOCAL | CREAD;
  termios_p.c_lflag = 0;
  termios_p.c_cc[VMIN] = 1;
  termios_p.c_cc[VTIME] = 0;

  bool ok = true;

  ok &= SetISpeed(termios_p,i_speed);
  ok &= SetOSpeed(termios_p,o_speed);
  ok &= SetCharSize(termios_p,char_size);
  ok &= SetStopBits(termios_p,stop_bits);
  ok &= SetParity(termios_p,par);
  // Bit of a cheat, should give better diagnostics.
  if(!ok) {
    cerr << "SerialCtrlC::Init(), Invalid parameter. \n";
    return false;
  }

  //tcflush(m_fd, TCSAFLUSH);

  if(tcsetattr(m_streamIo.Fd(), TCSANOW, &termios_p ) < 0)  {
    cerr << "SerialCtrlC::Init(), Failed to configure serial port \n";
    return false;
  }
  return true;
#else
  return false;
#endif
}

//! Reset interface.
void SerialDirectC::Reset() {
#if RAVL_HAVE_TERMIOS
  tcsendbreak(m_streamIo.Fd(), 0);
#else
  RavlAssert(0);
#endif
}

//! Flush buffer.
void SerialDirectC::Flush() {
#if RAVL_HAVE_TERMIOS
  tcflush(m_streamIo.Fd(),TCIOFLUSH);
#else
  RavlAssert(0);
#endif
}

//! Is stream open ?
bool SerialDirectC::IsOpen() const {
  return m_streamIo.IsOpen();
}

//! Read some bytes from a stream.
IntT SerialDirectC::Read(char *buff,UIntT size) {
  return m_streamIo.Read(buff, size);
}

//! Read some bytes from a stream.
IntT SerialDirectC::ReadV(char **buffer,IntT *len,int n) {
  return m_streamIo.ReadV(buffer, len, n);
}

//! Write some bytes to a stream.
IntT SerialDirectC::Write(const char *buff,UIntT size) {
  ONDEBUG(cerr << "SerialDirectC::Write:" << buff << "  " << size << endl);
  return m_streamIo.Write(buff, size);
}

//! Write multiple buffers
IntT SerialDirectC::WriteV(const char **buffer,IntT *len,int n) {
  return m_streamIo.WriteV(buffer, len, n);
}

//! set the input speed of the port;
bool SerialDirectC::SetISpeed(const IntT i_speed) {
#if RAVL_HAVE_TERMIOS
  termios pb;
  if (tcgetattr(m_streamIo.Fd(),&pb) < 0) {
    cerr << "SerialCtrlC::SetISpeed(), Failed to read port parameters. \n";
    return false;
  }
  if(!SetISpeed(pb,i_speed))
    return false;
  return tcsetattr(m_streamIo.Fd(), TCSANOW, &pb ) >= 0;
#else
  return false;
#endif
}

//! set the output speed of the port;
bool SerialDirectC::SetOSpeed(const IntT o_speed) {
#if RAVL_HAVE_TERMIOS
  termios pb;
  if (tcgetattr(m_streamIo.Fd(),&pb) < 0) {
    cerr << "SerialCtrlC::SetOSpeed(), Failed to read port parameters. \n";
    return false;
  }
  if(!SetOSpeed(pb,o_speed))
    return false;
  return tcsetattr(m_streamIo.Fd(), TCSANOW, &pb ) >= 0;
#else
  return false;
#endif
}

//! set the number of stop bits : 1 or 2;
bool SerialDirectC::SetStopBits(const IntT stop_bit) {
#if RAVL_HAVE_TERMIOS
  termios pb;
  if (tcgetattr(m_streamIo.Fd(), &pb) < 0) {
    cerr << "SerialCtrlC::SetStopBits(), Failed to read port parameters. \n";
    return false;
  }
  if(!SetStopBits(pb,stop_bit))
    return false;
  return tcsetattr(m_streamIo.Fd(), TCSANOW, &pb) >= 0;
#else
  return false;
#endif
}

//! set the character size: 5,6,7,8;
bool SerialDirectC::SetCharSize(const IntT char_size) {
#if RAVL_HAVE_TERMIOS
  termios pb;
  if (tcgetattr(m_streamIo.Fd(), &pb) < 0) {
    cerr << "SerialCtrlC::SetCharSize(), Failed to read port parameters. \n";
    return false;
  }
  if(!SetCharSize(pb,char_size))
    return false;
  return tcsetattr(m_streamIo.Fd(), TCSANOW, &pb) >= 0;
#else
  return false;
#endif
}

//! parity type: Odd or Even or None or 1
bool SerialDirectC::SetParity(ParityT par) {
#if RAVL_HAVE_TERMIOS
  termios pb;
  if (tcgetattr(m_streamIo.Fd(),&pb) < 0) {
    cerr << "SerialCtrlC::SetParity(), Failed to read port parameters. \n";
    return false;
  }
  if(!SetParity(pb,par))
    return false;
  return tcsetattr(m_streamIo.Fd(), TCSANOW, &pb) >= 0;
#else
  return false;
#endif
}

#if (!RAVL_OS_WIN32)
//! Access file descriptor.
IntT SerialDirectC::Fd() const {
  return m_streamIo.Fd();
}
#endif
//! Set the amount of time you should attempt to read from a file descriptor.
// This limits the time spent attempting to write to a socket
// without reading a single byte. The default is 120 seconds.
void SerialDirectC::SetReadTimeout(float timeout) {
  m_streamIo.SetReadTimeout(timeout);
}

//! Set the amount of time you should attempt to write to a file descriptor.
// This limits the time spent attempting to write to a socket
// without sending a single byte. The default is 120 seconds.
void SerialDirectC::SetWriteTimeout(float timeout) {
  m_streamIo.SetWriteTimeout(timeout);
}

//! Enable non-blocking use of read and write.
// true= read and write's won't do blocking waits.

#ifndef VISUAL_CPP
bool SerialDirectC::SetNonBlocking(bool block) {
  return m_streamIo.SetNonBlocking(block);
}
#endif

//! Setup don't close flag.
#if (!RAVL_OS_WIN32)
void SerialDirectC::SetDontClose(bool ndontClose) {
  m_streamIo.SetDontClose(ndontClose);
}
#endif
//! Should read's fail on timeout ?
// If false, the socket will be checked its
// open and valid, if it is the read will be retried.
#ifndef VISUAL_CPP
void SerialDirectC::SetFailOnReadTimeout(bool val) {
  m_streamIo.SetFailOnReadTimeout(val);
}
#endif
//! Should write's fail on timeout ?
// If false, the socket will be checked its
// open and valid, if it is the write will be retried.
#ifndef VISUAL_CPP
void SerialDirectC::SetFailOnWriteTimeout(bool val) {
  m_streamIo.SetFailOnWriteTimeout(val);
}
#endif

//! Should read functions keep reading until the buffer is full?
// If not the read will return immediately once any has been read.
#ifndef VISUAL_CPP
void SerialDirectC::SetFillBufferOnRead(bool value) {
  m_streamIo.SetFillBufferOnRead(value);
}
#endif

//: Set input bit rate.
bool SerialDirectC::SetISpeed(termios &pb,IntT bitrate) {
#if RAVL_HAVE_TERMIOS
  return cfsetispeed(&pb, SpeedSetting(bitrate)) >= 0;
#else
  return false;
#endif
}

//: Set output bit rate.
bool SerialDirectC::SetOSpeed(termios &pb,IntT bitrate) {
#if RAVL_HAVE_TERMIOS
  return cfsetospeed(&pb, SpeedSetting(bitrate)) >= 0;
#else
  return false;
#endif
}

//: Set char size.
bool SerialDirectC::SetCharSize(termios &pb,IntT bits) {
#if RAVL_HAVE_TERMIOS
  pb.c_cflag &= ~(CSIZE);
  switch(bits)
  {
    case 5: pb.c_cflag |= CS5; break;
    case 6: pb.c_cflag |= CS6; break;
    case 7: pb.c_cflag |= CS7; break;
    case 8: pb.c_cflag |= CS8; break;
    default:
      cerr << "SerialCtrlC::SetCharSize(), WARNING: Unsupported char size " << bits << ", Using 8 bit.\n";
      pb.c_cflag |= CS8;
      return false;
  }
  return true;
#else
  return false;
#endif
}

//: Set number of stop bits.
bool SerialDirectC::SetStopBits(termios &pb,IntT bits) {
#if RAVL_HAVE_TERMIOS
  switch(bits)
  {
    case 1: pb.c_cflag &= ~(CSTOPB); break;
    case 2: pb.c_cflag |= CSTOPB; break;
    default:
      cerr<<"SerialCtrlC::SetStopBits(), ERROR: Illegal stopbit input " << bits << "\n";
      return false;
  }
  return true;
#else
  return false;
#endif
}

//: parity type: Odd or Even or None or 1
bool SerialDirectC::SetParity(termios &pb,ParityT parity_type) {
#if RAVL_HAVE_TERMIOS
  switch(parity_type)
  {
    case SERIAL_PARITY_ODD:
      pb.c_cflag |=(PARODD|PARENB);
      pb.c_iflag |= INPCK;
      break;
    case SERIAL_PARITY_EVEN:
      pb.c_cflag |=(PARENB);
      pb.c_cflag &= ~(PARODD);
      pb.c_iflag |= INPCK;
      break;
    case SERIAL_PARITY_SET:
      pb.c_cflag &=~(PARENB);
      pb.c_iflag &= ~(INPCK);
      break;
    case SERIAL_PARITY_NONE:
      pb.c_cflag &=~(PARENB);
      pb.c_iflag &= ~(INPCK);
      break;
    }
  return true;
#else
  return false;
#endif
}


}

