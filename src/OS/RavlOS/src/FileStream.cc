// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! docentry="Ravl.OS.File System"
//! author="Charles Galambos"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/FileStream.cc"

#if VISUAL_CPP
#include <direct.h>
#include <memory.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#endif

#include "Ravl/OS/FileStream.hh"
#include "Ravl/OS/Filename.hh"

namespace RavlN {
  
  //: Default constructor.
  
  FileStreamC::FileStreamC()
#if RAVL_HAVE_INTFILEDESCRIPTORS 
    : fd(-1)
#endif
  {}
  
#if RAVL_HAVE_INTFILEDESCRIPTORS 
  //: Construct from a file descriptor.
  
  FileStreamC::FileStreamC(int nfd)
    : fd(nfd)
  {}
#endif
  
  //: Construct from a file descriptor.
  
#if RAVL_HAVE_INTFILEDESCRIPTORS 
  FileStreamC::FileStreamC(const StringC &filename,bool forRead,bool forWrite)
    : fd(-1)
  {
    Open(filename,forRead,forWrite);
  }
#else
  FileStreamC::FileStreamC(const StringC &filename,bool _forRead,bool _forWrite)
    :  forRead(_forRead),
       forWrite(_forWrite),
       filename(filename)
  {
    Open(filename,forRead,forWrite);
  }
#endif
  
  //: Open stream
  bool FileStreamC::Open(const StringC &filename,bool forRead,bool forWrite)
  {
#if RAVL_HAVE_INTFILEDESCRIPTORS
    if(fd >= 0)
      close(fd);
    fd = -1;
    int flags = 0;
    if(forRead) {
      if(forWrite)
        flags =  O_RDWR;
      else
        flags =  O_RDONLY;
    } else
      flags = O_WRONLY | O_CREAT | O_TRUNC;
#ifdef O_LARGEFILE
    flags |= O_LARGEFILE;
#endif
    fd = open(filename.chars(),flags,0666);
#else
    if(forRead)
      is = IStreamC(filename);
    if(forWrite)
      os = OStreamC(filename);
#endif
    return true;
  }

  //: Destructor
  
  FileStreamC::~FileStreamC() {
#if RAVL_HAVE_INTFILEDESCRIPTORS 
    if(fd >= 0)
      close(fd);
#endif
  }
  
  //: Seek to offset in a file.
  
  bool FileStreamC::Seek(StreamOffsetT offset) { 
#if RAVL_HAVE_INTFILEDESCRIPTORS 
#if RAVL_HAVE_LSEEK64
    return lseek64(fd,offset,SEEK_SET) >= 0; 
#else
    RavlAssert(sizeof(off_t) == 8); // Check off_t is 64 bit.
    return lseek(fd,offset,SEEK_SET) >= 0;     
#endif
#else
    if(forRead)
      is.Seek(offset);
    if(forWrite)
      os.Seek(offset);
    return true;
#endif
  }
  
  //: Write data to file descriptor.
  // Returns the number of characters read, or -1 on error.
  
  IntT FileStreamC::Write(const void *data,IntT length) { 
#if RAVL_HAVE_INTFILEDESCRIPTORS     
    return write(fd,data,length); 
#else
    os.write(static_cast<const char *>(data),length);
    return os.good() ? length : 0;
#endif
  }
  
  //: Read data from file descriptor.
  
  IntT FileStreamC::Read(void *data,IntT length) { 
#if RAVL_HAVE_INTFILEDESCRIPTORS    
    return read(fd,data,length); 
#else
    is.read(static_cast<char *>(data),length);
	// FIXME:- Should return a std::streamsize
    return static_cast<IntT>(is.is().gcount());    
#endif
  }

  
  //: Write all data to file descriptor.
  // This routine works as Write(), but will handle temporary errors.
  
  IntT FileStreamC::WriteAll(const void *tdata,IntT length) {
#if RAVL_HAVE_INTFILEDESCRIPTORS    
    const char *data = (const char *) tdata;
    IntT done = 0;
    do {
      int n = Write(&(data[done]),length - done);
      if(n < 0) {
	if(errno == EAGAIN || errno == EINTR) // Recoverable error?
	  continue;
	return -1;
      }
      done += n;
    } while(done < length);
    return done;
#else
    return Write(tdata,length);
#endif
  }
  
  //: Read all data from file descriptor.
  // This routine works as Write(), but will handle temporary errors.
  
  IntT FileStreamC::ReadAll(void *tdata,IntT length) {
#if RAVL_HAVE_INTFILEDESCRIPTORS    
    char *data = (char *) tdata;
    IntT done = 0;
    do {
      int n = Read(&(data[done]),length - done);
      if(n == 0) {
	// Indicates end of file on linux, what about other platforms ?
	return 0;
      }
      if(n < 0) {
	if(errno == EAGAIN || errno == EINTR) // Recoverable error?
	  continue;
	cerr << "Read error =" << errno << "\n";
	return -1;
      }
      done += n;
    } while(done < length);
    return done;
#else
    return Read(tdata,length);
#endif
  }
  
  //: Find the size of the file.
  
  StreamSizeT FileStreamC::Size() const {
#if RAVL_HAVE_INTFILEDESCRIPTORS    
    if(!Good())
      return 0;
#if RAVL_USE_LARGEFILESUPPORT
    struct stat64 buff;
    if(fstat64(fd,&buff) < 0)
      return 0;
#else
    struct stat buff;
    if(fstat(fd,&buff) < 0)
      return 0;
#endif
    return (StreamSizeT) buff.st_size;
#else
    FilenameC lf(filename);
    return lf.FileSize();
#endif
  }

  //: Ensure all data is written to disk.
  
  bool FileStreamC::Sync(bool metaDataToo) {
#if RAVL_HAVE_INTFILEDESCRIPTORS 
    if(fd < 0)
      return false;
#if RAVL_HAVE_FDATASYNC
    if(metaDataToo)
      return ::fsync(fd) == 0;
    else
      return fdatasync(fd) == 0;
#else
    return fsync(fd) == 0;    
#endif
#endif    
    return true;
  }
  
}
