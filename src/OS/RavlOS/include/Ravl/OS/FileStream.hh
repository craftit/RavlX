// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILESTREAM_HEADER
#define RAVL_FILESTREAM_HEADER 1
//! docentry="Ravl.API.OS.File System"
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/FileStream.hh"

#include "Ravl/Types.hh"
#include "Ravl/String.hh"

#if !RAVL_HAVE_INTFILEDESCRIPTORS 
#include "Ravl/Stream.hh"
#endif


namespace RavlN {
  
  //! userlevel=Normal
  //: Class for handling 64 bit file IO.
  // Unfortunately C++ streams don't reliably support 64 bit file IO. This class encapsulates
  // the IO calls needed to work with large files.
  
  class FileStreamC {
  public:
    FileStreamC();
    //: Default constructor.
    
#if RAVL_HAVE_INTFILEDESCRIPTORS 
    FileStreamC(int fd);
    //: Construct from a file descriptor.
#endif
    
    FileStreamC(const StringC &filename,bool forRead,bool forWrite = false);
    //: Construct from a file descriptor.

    ~FileStreamC();
    //: Destructor
    
    bool Open(const StringC &filename,bool forRead,bool forWrite = false);
    //: Open stream

    bool Seek(StreamOffsetT offset);
    //: Seek to offset in a file.
    
    IntT Write(const void *data,IntT length);
    //: Write data to file descriptor.
    // Returns the number of characters written, or -1 on error.
    
    IntT Read(void *data,IntT length);
    //: Read data from file descriptor.
    // Returns the number of characters read, or -1 on error.
    
    IntT WriteAll(const void *data,IntT length);
    //: Write all data to file descriptor.
    // This routine works as Write(), but will handle temporary errors.
    // Returns the number of characters read, or -1 on error.
    
    IntT ReadAll(void *data,IntT length);
    //: Read all data from file descriptor.
    // This routine works as Read(), but will handle temporary errors.
    // Returns the number of characters written, or -1 on error.
    
    bool Good() const
#if RAVL_HAVE_INTFILEDESCRIPTORS 
    { return fd >= 0; }
#else
    { return is.good() && os.good(); }
#endif
    //: Is stream good ?
    
    StreamSizeT Size() const;
    //: Find the size of the file.
    
    bool Sync(bool metaDataToo = true);
    //: Ensure all buffers are flushed and data is written to disk.
    
  protected:
#if RAVL_HAVE_INTFILEDESCRIPTORS 
    int fd;
#else
    IStreamC is;
    OStreamC os;
    bool forRead;
    bool forWrite;
    StringC filename;
#endif
    int error;
    
  private:
    FileStreamC(const FileStreamC &)
      : fd(0),
        error(0)
    {}
    //: Hide copy constructor.
  };
}



#endif
