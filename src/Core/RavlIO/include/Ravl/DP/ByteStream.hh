// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BYTESTREAM_HEADER
#define RAVL_BYTESTREAM_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.IO" 
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/ByteStream.hh"

#include "Ravl/DP/Port.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Abstract class representing an output byte stream
  
  class DPOByteStreamBodyC
    : public DPOPortBodyC<ByteT>
  {
  public:
    DPOByteStreamBodyC()
    {}
    //: Default constructor.
    
    virtual bool Cork(bool enable);
    //: Cork stream.  
    // True indicates that there is going to
    // be several write operations immediatly following each
    // other and stops the transmition of fragmented packets.
    // If your not expecting to do any more writes immediatly
    // you must call 'Cork(false)' immediatly. <br>
    // False indicates that all the pending data has been written
    // This sends any partial packets still pending. <br>
    // Returns true if Corking is supported by stream.
    
    virtual IntT Write(const char *buffer,IntT len);
    //: Write data to stream
    //!param: buffer - Buffer to write data from.
    //!param: len - length of buffer.
    //!return: Number of bytes written, or -1 on error.
    
    virtual IntT WriteV(const char **buffer,IntT *len,int n);
    //: Write data to stream
    //!param: buffer - Array of buffers to write data from.
    //!param: len - Array of lengths of the buffers.
    //!param: n - Number of buffers involved.
    //!return: Number of bytes written, or -1 on error.
    
    virtual IntT WriteAll(const char *buffer,IntT len);
    //: Write data to stream
    // This routine works as Write(), but will handle temporary errors and
    // do its best to ensure the complete buffer is written.
    //!param: buffer - Buffer to write data from.
    //!param: len - length of buffer.
    //!return: Number of bytes written, or -1 on error.
    
    virtual IntT WriteAllV(const char **buffer,IntT *len,int n);
    //: Write data to stream
    // This routine works as Write(), but will handle temporary errors and
    // do its best to ensure the complete buffer is written.
    //!param: buffer - Array of buffers to write data from.
    //!param: len - Array of lengths of the buffers.
    //!param: n - Number of buffers involved.
    //!return: Number of bytes written, or -1 on error.
    
  protected:
  };
  
  //! userlevel=Normal
  //: Abstract class representing an output byte stream
  
  class DPOByteStreamC
    : public DPOPortC<ByteT>
  {
  public:
    DPOByteStreamC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DPOByteStreamC(DPOByteStreamBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPOByteStreamBodyC &Body()
    { return dynamic_cast<DPOByteStreamBodyC &>(DPEntityC::Body()); }
    //: Access body.

    const DPOByteStreamBodyC &Body() const
    { return dynamic_cast<const DPOByteStreamBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    
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
    // Returns true if Corking is supported by stream.
    
    
    IntT Write(const char *buffer,IntT len)
    { return Body().Write(buffer,len); }
    //: Write data to stream
    //!param: buffer - Buffer to write data from.
    //!param: len - length of buffer.
    //!return: Number of bytes written, or -1 on error.
    
    IntT WriteV(const char **buffer,IntT *len,int n)
    { return Body().WriteV(buffer,len,n); }
    //: Write data to stream
    //!param: buffer - Array of buffers to write data from.
    //!param: len - Array of lengths of the buffers.
    //!param: n - Number of buffers involved.
    //!return: Number of bytes written, or -1 on error.
    
    IntT WriteAll(const char *buffer,IntT len)
    { return Body().WriteAll(buffer,len); }
    //: Write data to stream
    // This routine works as Write(), but will handle temporary errors and
    // do its best to ensure the complete buffer is written.
    //!param: buffer - Buffer to write data from.
    //!param: len - length of buffer.
    //!return: Number of bytes written, or -1 on error.
    
    IntT WriteAllV(const char **buffer,IntT *len,int n)
    { return Body().WriteAllV(buffer,len,n); }
    //: Write data to stream
    // This routine works as Write(), but will handle temporary errors and
    // do its best to ensure the complete buffer is written.
    //!param: buffer - Array of buffers to write data from.
    //!param: len - Array of lengths of the buffers.
    //!param: n - Number of buffers involved.
    //!return: Number of bytes written, or -1 on error.

  };

  //! userlevel=Develop
  //: Abstract class representing an input byte stream
  
  class DPIByteStreamBodyC
    : public DPIPortBodyC<ByteT>
  {
  public:
    DPIByteStreamBodyC()
    {}
    //: Default constructor.
    
    virtual IntT Read(char *buffer,IntT len);
    //: Read data from stream info buffer.
    //!param: buffer - Buffer to read data into.
    //!param: len - length of buffer.
    //!return: Number of bytes read or -1 on error
    
    virtual IntT ReadV(char **buffer,IntT *len,int n);
    //: Read data to stream
    //!param: buffer - Array of buffers to read data into.
    //!param: len - Array of lengths of the buffers.
    //!param: n - Number of buffers involved.
    //!return: Number of bytes read or -1 on error
    
    virtual IntT ReadAll(char *buffer,IntT len);
    //: Read data from stream info buffer.
    // This routine works as Read(), but will handle temporary errors and do
    // its best to ensure a full buffer is read.
    //!param: buffer - Buffer to read data into.
    //!param: len - length of buffer.
    //!return: Number of bytes read or -1 on error
    
    virtual IntT ReadAllV(char **buffer,IntT *len,int n);
    //: Read data to stream
    // This routine works as Read(), but will handle temporary errors and do
    // its best to ensure a full buffer is read.
    //!param: buffer - Array of buffers to read data into.
    //!param: len - Array of lengths of the buffers.
    //!param: n - Number of buffers involved.
    //!return: Number of bytes read or -1 on error
    
  protected:
  };
  
  //! userlevel=Normal
  //: Abstract class representing an input byte stream
  
  class DPIByteStreamC
    : public DPIPortC<ByteT>
  {
  public:
    DPIByteStreamC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DPIByteStreamC(DPIByteStreamBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPIByteStreamBodyC &Body()
    { return dynamic_cast<DPIByteStreamBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIByteStreamBodyC &Body() const
    { return dynamic_cast<const DPIByteStreamBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:

    IntT Read(char *buffer,IntT len)
    { return Body().Read(buffer,len); }
    //: Read data from stream info buffer.
    //!param: buffer - Buffer to read data into.
    //!param: len - length of buffer.
    //!return: Number of bytes read or -1 on error
    
    IntT ReadV(char **buffer,IntT *len,int n)
    { return Body().ReadV(buffer,len,n); }
    //: Read data to stream
    //!param: buffer - Array of buffers to read data into.
    //!param: len - Array of lengths of the buffers.
    //!param: n - Number of buffers involved.
    //!return: Number of bytes read or -1 on error    

    IntT ReadAll(char *buffer,IntT len)
    { return Body().ReadAll(buffer,len); }
    //: Read data from stream info buffer.
    // This routine works as Read(), but will handle temporary errors and do
    // its best to ensure a full buffer is read.
    //!param: buffer - Buffer to read data into.
    //!param: len - length of buffer.
    //!return: Number of bytes read or -1 on error
    
    IntT ReadAllV(char **buffer,IntT *len,int n)
    { return Body().ReadAllV(buffer,len,n); }
    //: Read data to stream
    // This routine works as Read(), but will handle temporary errors and do
    // its best to ensure a full buffer is read.
    //!param: buffer - Array of buffers to read data into.
    //!param: len - Array of lengths of the buffers.
    //!param: n - Number of buffers involved.
    //!return: Number of bytes read or -1 on error
    
  };
  
  
}


#endif

