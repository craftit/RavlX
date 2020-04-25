// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/ByteStream.cc"

#include "Ravl/DP/ByteStream.hh"

namespace RavlN {

  //: Cork stream.  
  // True indicates that there is going to
  // be several write operations immediately following each
  // other and stops the transmition of fragmented packets.
  // If your not expecting to do any more writes immediately
  // you must call 'Cork(false)' immediately. <br>
  // False indicates that all the pending data has been written
  // This sends any partial packets still pending. <br>
  // Returns true if Corking is supported by stream.
  
  bool DPOByteStreamBodyC::Cork(bool enable) {
    return false;
  }

  //: Write data to stream
  //!param: buffer - Buffer to write data from.
  //!param: len - length of buffer.
  //!return: true if write succeeded.
  
  IntT DPOByteStreamBodyC::Write(const char *buffer,IntT len) {
    SArray1dC<ByteT> tmp(reinterpret_cast<ByteT *>(const_cast<char *>(buffer)),len,false);
    return PutArray(tmp);
  }
  
  //: Write data to stream
  //!param: buffer - Array of buffers to write data from.
  //!param: len - Array of lengths of the buffers.
  //!param: n - Number of buffers involved.
  //!return: true if write succeeded.

  IntT DPOByteStreamBodyC::WriteV(const char **buffer,IntT *len,int n) {
    IntT count = 0;
    for(int i = 0;i < n;i++) {
      int x = Write(buffer[i],len[i]);
      if(x < 0)
	return count;
      count += x;
      if(x < len[i])
	break;
    }
    return count;
  }
  
  //: Write data to stream
  // This routine works as Write(), but will handle temporary errors and
  // do its best to ensure the complete buffer is written.
  //!param: buffer - Buffer to write data from.
  //!param: len - length of buffer.
  //!return: Number of bytes written, or -1 on error.
  
  IntT DPOByteStreamBodyC::WriteAll(const char *buffer,IntT len) {
    SArray1dC<ByteT> tmp(reinterpret_cast<ByteT *>(const_cast<char *>(buffer)),len,false);
    IntT done = PutArray(tmp);
    while(done < len && IsPutReady()) {
      SArray1dC<ByteT> theRest(tmp,len-done,done);
      done += PutArray(theRest);
    }
    return done;
  }
  
  //: Write data to stream
  // This routine works as Write(), but will handle temporary errors and
  // do its best to ensure the complete buffer is written.
  //!param: buffer - Array of buffers to write data from.
  //!param: len - Array of lengths of the buffers.
  //!param: n - Number of buffers involved.
  //!return: Number of bytes written, or -1 on error.
  
  IntT DPOByteStreamBodyC::WriteAllV(const char **buffer,IntT *len,int n) {
    IntT count = 0;
    for(int i = 0;i < n;i++) {
      int x = WriteAll(buffer[i],len[i]); // If WriteAll fails then something must be very wrong, so exit.
      if(x < 0)
	return count;
      count += x;
      if(x < len[i])
	break;
    }
    return count;    
  }
  

  //----------------------------------------------------------------
  
  //: Read data from stream info buffer.
  //!param: buffer - Buffer to read data into.
  //!param: len - length of buffer.
  //!return: Number of bytes read or -1 on error
  
  IntT DPIByteStreamBodyC::Read(char *buffer,IntT len) {
    SArray1dC<ByteT> org(reinterpret_cast<ByteT *>(buffer),len,false);
    SArray1dC<ByteT> buf = org;
    IntT n = GetArray(buf);
    RavlAssert(buf.Buffer() == org.Buffer());
    return n;
  }

  //: Read data to stream
  //!param: buffer - Array of buffers to read data into.
  //!param: len - Array of lengths of the buffers.
  //!param: n - Number of buffers involved.
  //!return: Number of bytes read or -1 on error
  
  IntT DPIByteStreamBodyC::ReadV(char **buffer,IntT *len,int n) {
    IntT count = 0;
    for(int i = 0;i < n;i++) {
      int x = Read(buffer[i],len[i]);
      if(x < 0)
	return count;
      count += x;
      if(x < len[i])
	break;
    }
    return count;    
  }

  
  //: Read data from stream info buffer.
  // This routine works as Read(), but will handle temporary errors and do
  // its best to ensure a full buffer is read.
  //!param: buffer - Buffer to read data into.
  //!param: len - length of buffer.
  //!return: Number of bytes read or -1 on error
  
  IntT DPIByteStreamBodyC::ReadAll(char *buffer,IntT len) {
    SArray1dC<ByteT> tmp(reinterpret_cast<ByteT *>(buffer),len,false);
    IntT done = GetArray(tmp);
    while(done < len && !IsGetEOS()) {
      SArray1dC<ByteT> theRest(tmp,len-done,done);
      done += GetArray(theRest);
    }
    return done;    
  }
    
  //: Read data to stream
  // This routine works as Read(), but will handle temporary errors and do
  // its best to ensure a full buffer is read.
  //!param: buffer - Array of buffers to read data into.
  //!param: len - Array of lengths of the buffers.
  //!param: n - Number of buffers involved.
  //!return: Number of bytes read or -1 on error
  
  IntT DPIByteStreamBodyC::ReadAllV(char **buffer,IntT *len,int n) {
    IntT count = 0;
    for(int i = 0;i < n;i++) {
      int x = ReadAll(buffer[i],len[i]); // If ReadAll fails then we're forced to exit anyway
      if(x < 0)
	return count;
      count += x;
      if(x < len[i])
	break;
    }
    return count;        
  }
  
}
