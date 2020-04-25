// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include <iostream>
#include "Ravl/Types.hh"
#include "Ravl/OS/WinStreamIO.hh"
//! lib=RavlOS

namespace RavlN {

#if RAVL_OS_WIN32
#define ThrowLastError(_message) (cerr << _message << endl) 
  //: Constructor.
/*  
  WinStreamIOC::WinStreamIOC()
    :  m_rwTimeout(30.0),
       m_portHandle(INVALID_HANDLE_VALUE)
  {}
*/  
  //: Destructor.
  
  WinStreamIOC::~WinStreamIOC()
  { Close(); }
  
  //: Open a file.
  
  bool WinStreamIOC::Open(const char *fileName) 
  {
    m_portHandle = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE,
                              0, NULL, OPEN_EXISTING, 
                              FILE_FLAG_OVERLAPPED /*| FILE_FLAG_NO_BUFFERING*/, NULL);
    
    // Have we created a valid handle ?
    return m_portHandle == INVALID_HANDLE_VALUE;
  }
  
  //: Close the file handle.
  
  bool WinStreamIOC::Close() {
    if(IsOpen()) {
      if(!CloseHandle(m_portHandle)) 
        ThrowLastError("SerialPortC::Close: ");
      m_portHandle = INVALID_HANDLE_VALUE;
    }
    return true;
  }
  
  //: Read some bytes from a stream.
  
  IntT WinStreamIOC::Read(char *buffer,UIntT size) 
  {
    int bufferSize = size; // 2011-01-17 MS
    //printf("SerialPortC::Read:%i\n", MinBytesToRead);
    DWORD dwRead;
    OVERLAPPED osReader = {0};
    OVERLAPPED osCommEvent = {0};
    UIntT at = 0;
    
    // Create the overlapped event. Must be closed before exiting
    // to avoid a handle leak.
    osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    osCommEvent.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    
    if(osReader.hEvent == NULL || osCommEvent.hEvent == NULL)
      ThrowLastError("SerialPortC::Read: error creating overlapped event handle ");
    
    try {
      while(at < size) {
        //printf("Data len:%i of %i\n", m_data.length(), MinBytesToRead);
        
        //reset comm event
        if(!ResetEvent(osCommEvent.hEvent))
          ThrowLastError("SerialPortC::Read: Can't reset comm event ");
        
        //event is supposed to be reset by the ReadFile function, when no data is ready
        //if(!ResetEvent(osReader.hEvent))
        //  ThrowLastError("SerialPortC::Read: Can't reset read event ");
        
        // --- Issue read operation.
        if(!ReadFile(m_portHandle, &buffer[at], size-at, &dwRead, &osReader))  {
          // --- wait for data
          //printf("check error dwRead:%i\n", int(dwRead));
          if(GetLastError() != ERROR_IO_PENDING)     // read not delayed?
            ThrowLastError("SerialPortC::Read: error reading from com port");
          
          //printf("Waiting in read...");
          DWORD dwRes = WaitForSingleObject(osReader.hEvent, m_readTimeOut);
          switch(dwRes) {
            // Read completed.
          case WAIT_OBJECT_0:
            //printf("Ok\n");
            if(!GetOverlappedResult(m_portHandle, &osReader, &dwRead, FALSE))
              ThrowLastError("SerialPortC::Read: error in communication");
            //printf("read ok dwRead:%i\n", int(dwRead));
            break;
            
            // Operation isn't complete yet.
          case WAIT_TIMEOUT:
            //printf("SerialPortC::Read: read timeout\n");
            ThrowLastError("SerialPortC::Read: read timeout");
            break;                       
            
          default:
            //printf("SerialPortC::Read: Error in the WaitForSingleObject\n");
            ThrowLastError("SerialPortC::Read: Error in the WaitForSingleObject");
            break;
          }
        }
        
        if(dwRead == 0) {
          //printf("No bytes read\n");
          // wait for input
          DWORD dwCommEvent = 0;
          //printf("Waiting for input\n");
          if(!WaitCommEvent(m_portHandle, &dwCommEvent, &osCommEvent))  {
            //printf("dwCommEvent:%i\n", int(dwCommEvent));
            if(GetLastError() != ERROR_IO_PENDING)
              ThrowLastError("SerialPortC::Read: error in WaitCommEvent ");
            
            // Wait a little while for an event to occur.
            //printf("Waiting...");
            DWORD dwRes = WaitForSingleObject(osCommEvent.hEvent, m_readTimeOut);
            switch(dwRes) {
              // Event occurred.
            case WAIT_OBJECT_0: 
              //printf("Ok, trying to get overlapped result\n");
              if(!GetOverlappedResult(m_portHandle, &osCommEvent, &dwRead, FALSE))
                ThrowLastError("SerialPortC::Read: error in GetOverlappedResult ");
              //printf("dwRead:%i\n", int(dwRead));
              break;
              
            case WAIT_TIMEOUT: {
              //printf("m_rwTimeout:%i\n", m_rwTimeout);
              //printf("buffer len: %i  content:%s\n", m_data.size(), m_data.c_str());
              ThrowLastError("SerialPortC::Read: read timeout ");
            }
              
            default:
              ThrowLastError("SerialPortC::Read: Error in the WaitForSingleObject ");
            }
          }
        } 
        
        at += dwRead;
      }
      
      //read more data if possible
      //printf("trying to read rest of the buffer\n");
	  /* 2011-01-17
      while(ReadFile(m_portHandle, buffer, bufferSize, &dwRead, &osReader) && dwRead > 0) {
        m_data += buffer;
        //printf("dwRead %i buffer %i, m_data %s\n", int(dwRead), int(buffer[0]), m_data.c_str());
      }
	  */
    } catch( ... ) {
      if(!CancelIo(m_portHandle))
        printf("Can't cancel i/o\n");
      if(!PurgeComm(m_portHandle, PURGE_RXABORT | PURGE_RXCLEAR))
        printf("Can't purge com port\n");
      CloseHandle(osReader.hEvent);
      CloseHandle(osCommEvent.hEvent);
      throw;  
    }
    
    CloseHandle(osReader.hEvent);
    CloseHandle(osCommEvent.hEvent);
    return at;
  }
  
  //: Read some bytes from a stream.
  
  IntT WinStreamIOC::ReadV(char **buffer,IntT *len,int n)
  {
    IntT ret = 0;
    for(int i = 0;i < n;i++) {
      int val = Read(buffer[i],len[i]);
      // Negative values indicate an error.
      if(val < 0) return val; // Leave.
      ret += val;
    }
    return ret;
  }
  
  //: Write some bytes to a stream.
  
  IntT WinStreamIOC::Write(const char *buff,UIntT size) 
  {
    OVERLAPPED osWrite = {0};
    DWORD dwWritten;
    DWORD dwRes;
    UIntT at = 0;
    
    // Create this write operation's OVERLAPPED structure's hEvent.
    osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if(osWrite.hEvent == NULL)
      ThrowLastError("SerialPortC::Write: error creating overlapped event handle");
    
    // Issue write.
    try 
	{
	  while(at < size) {
		//printf("writing to port %s\n", Data.c_str());
		dwWritten = 0;
        
		if(!WriteFile(m_portHandle, &buff[at], size-at, &dwWritten, &osWrite)) {
		  //printf("Checking error code\n");
		  //printf("dwWritten:%i\n", int(dwWritten));
          
		  if(GetLastError() != ERROR_IO_PENDING)  {
			// WriteFile failed, but isn't delayed. Report error and abort.
			ThrowLastError("SerialPortC::Write: error writing to com port");
		  }
          
		  // Write is pending.
		  //printf("Waiting\n");
		  dwRes = WaitForSingleObject(osWrite.hEvent, m_writeTimeOut);
		  if(dwRes != WAIT_OBJECT_0) {
			if(!CancelIo(m_portHandle))
			  printf("Can't cancel i/o\n");
			ThrowLastError("SerialPortC::Write: writing to com port timeout");
		  }
          
		  if(!GetOverlappedResult(m_portHandle, &osWrite, &dwWritten, FALSE)) {
			ThrowLastError("SerialPortC::Write: writing to com port fail");
		  }
		  //printf("dwWritten:%i\n", int(dwWritten));
		}
        
		at += dwWritten;
	  }
	} 
	   
	catch( ... ) 
	{
        CloseHandle(osWrite.hEvent);
        throw;
    }
    
    CloseHandle(osWrite.hEvent);
    
    return at;
  }
  
  //: Write multiple buffer
  
  IntT WinStreamIOC::WriteV(const char **buffer,IntT *len,int n) 
  {
    IntT ret = 0;
    for(int i = 0;i < n;i++) {
      int val = Write(buffer[i],len[i]);
      // Negative values indicate an error.
      if(val < 0) return val; // Leave.
      ret += val;
    }
    return ret;
  }

#endif
}
