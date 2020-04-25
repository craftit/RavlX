// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id: Filename.cc 7951 2011-01-05 17:10:46Z robowaz $"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/Filename.cc"

#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Random.hh"
#include "Ravl/Stream.hh"
//#include "Ravl/UserInfo.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <fstream>
#else
#include <fstream.h>
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Ravl/Assert.hh"
#include <string.h>
#include <errno.h>

#if RAVL_COMPILER_VISUALCPP
#include <direct.h>
#include <memory.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif




namespace RavlN {
  StringC FilenameC::AMDPrefix("/a/");

  // Make a directory with this name.
  
  bool FilenameC::MakeDir(const FilePermissionC &Acc) const  {
    if(IsEmpty()) // Empty string ?
      return false; 

    // Need to make nested directories ?
    FilenameC comp = PathComponent();
    if(!comp.IsEmpty()) {
      if(!comp.Exists()) {
	if(!comp.MakeDir(Acc))
	  return false;
      }
    }
#if !RAVL_COMPILER_VISUALCPP
    if(mkdir(chars(),Acc.Mode()) == 0)
#else
      if(_mkdir(chars()) == 0)
#endif
	return true;
    // If it exists when we exit, return true.
    // This can catch problems with relative paths, particularly
    // ones ending in '.'
    return Exists(); 
  }
  
// Test if the file/directory given in this path exists.

  bool FilenameC::Exists() const  {
    if(IsEmpty()) // Empty string ?
      return false; 
#if RAVL_USE_LARGEFILESUPPORT
    // In case the files large, use stat64.
    struct stat64 buff;
    if(stat64(chars(),&buff) < 0) {
      ONDEBUG(cerr << "FilenameC::Exists(), '" << (*this) << "' errno=" << errno << " \n");
      return false;
    }
#else
    struct stat buff;
    if(stat(chars(),&buff) < 0) {
      ONDEBUG(cerr << "FilenameC::Exists(), '" << (*this) << "' errno=" << errno << " \n");
      return false;
    }
#endif
    return true;
  }

  // Rename file.
  // Ret true=Succeeded.
  
  bool FilenameC::Rename(const StringC &newname) const {
    return (rename(this->chars(),newname.chars()) == 0);
  }
  
  // Creates hard link of this file

  bool FilenameC::Link(const StringC &linkname) const {
#if RAVL_COMPILER_VISUALCPP
    return 0;
#else
    return (link(this->chars(),linkname.chars()) == 0);
#endif 
  }

  // Get the access permissions for this file.
  
  FilePermissionC FilenameC::Permissions() const {
#if RAVL_USE_LARGEFILESUPPORT
    struct stat64 buff;
    if(stat64(chars(),&buff) < 0) {
      FilePermissionC Failed;
      Failed.SetNone();
      return Failed;
    }
#else
    struct stat buff;
    if(stat(chars(),&buff) < 0) {
      FilePermissionC Failed;
      Failed.SetNone();
      return Failed;
    }
#endif
    return FilePermissionC(buff.st_mode,buff.st_uid,buff.st_gid);
  }
  
  //: Set the access permissions for this file.
  // Returns false if failed.
  
  bool FilenameC::SetPermissions(const FilePermissionC &perm)
  { 
#if RAVL_COMPILER_VISUALCPP
    return _chmod(chars(),perm.Mode()) == 0; 
#else
    return chmod(chars(),perm.Mode()) == 0; 
#endif
  }
  
  
  // Return the path component of a Filename, ie. upto last /
  
  FilenameC FilenameC::PathComponent() const  {
    
    if(IsEmpty())
      return FilenameC();
#ifdef VISUAL_CPP
    // paths can contain / and \ which confuse this routine, so lets sort that out first!
    FilenameC str = Copy();
    str.gsub("/", "\\");
    str.gsub("\\\\", "\\");
    return str.before(filenameSeperator,-1);
#else
    return const_cast<FilenameC &>(*this).before(filenameSeperator,-1);
#endif
  }
  
  // Return the name component of a Filename, ie. after last /
  
  FilenameC FilenameC::NameComponent() const  {
    if(IsEmpty())
      return FilenameC();
#ifdef VISUAL_CPP
    // paths can contain / and \ which confuse this routine, so lets sort that out first!
    FilenameC str = Copy();
    str.gsub("/", "\\");
    str.gsub("\\\\", "\\");
    int ind = str.index(filenameSeperator,-1);
    if(ind < 0)
      return str;
    return str.after(ind);
#else
    int ind = index(filenameSeperator,-1);
    if(ind < 0)
      return *this;
    return const_cast<FilenameC &>(*this).after(ind);
#endif
  }

  //: Return the base name component of a Filename, 
  // ie. after last / and before the extension.
  // (the last '.' in the NameComponent()).
  
  FilenameC FilenameC::BaseNameComponent() const {
    if (contains('.'))
      return NameComponent().before('.',-1);
    return NameComponent();
  }
  
  //////////////////////////////////////
  //: Make this filename unique by appending a unique string
  //: and checking if it doesn't exist already. 
  // If it does try another, if process fails more than
  // maxretry times, then return false. If maxretry is negative
  // infinite retries are allowed.
  
  FilenameC FilenameC::MkTemp(IntT Digits,IntT maxretry) const RAVL_THROW2(ExceptionOperationFailedC,ExceptionOutOfRangeC) {
    if(Digits < 1)
      maxretry = 0; // If we can't change the name, no point retrying.
    if(Digits > 6 || Digits < 0) {
#ifdef RAVL_CHECK
      cerr << "FilenameC::MkTemp(), Value out of range Digits=" << Digits << "  should be between 0 and 6 \n";
      RavlAssert(0);
#endif
      throw ExceptionOutOfRangeC("FilenameC::MkTemp(), Digits must be between 0 and 6. \n");
    }
    StringC ext = Extension(); // Find extension.
    if(!ext.IsEmpty())
      ext = StringC('.') + ext;
    StringC base = const_cast<FilenameC &>(*this).before((int) Size() - (int) ext.Size()); // We know it won't modify the string.
    do {
      FilenameC newun = base + StringC((UIntT) RandomInt()).before(Digits) + ext;
      if(!newun.Exists())
	return newun;
      if(maxretry > 0)
	maxretry--;
    } while(maxretry != 0);  
    throw ExceptionOperationFailedC(StringC("Can't create temporary file : '") + (*this) + "'  (Digits:" + StringC(Digits) + ")", true);
    //  return FilenameC();
  }
  
  ///////////////////////////////////
  //: Delete file by this name.
  
  bool FilenameC::Remove() const  {
    if(IsEmpty())
      return false;
#if !RAVL_COMPILER_VISUALCPP
    if(IsDirectory())
      return (rmdir(*this) == 0);
    return (unlink(*this) == 0);
#else
    if(IsDirectory())
      return (_rmdir(*this) == 0);
    return (_unlink(*this) == 0);
#endif
  }
  
  
  //: Get last file access.
  
  DateC FilenameC::LastAccessTime() const {
#if RAVL_USE_LARGEFILESUPPORT
    struct stat64 buff;
    if(stat64(chars(),&buff) < 0)
      return DateC(false);
#else
    struct stat buff;
    if(stat(chars(),&buff) < 0)
      return DateC(false);
#endif
#if RAVL_TIMET_IS_INT
    return DateC((DateC::SecondT) buff.st_atime,0);
#else
    return DateC((DateC::SecondT) buff.st_atim.tv_sec,(long) buff.st_atim.tv_nsec / 1000);
#endif
  }
  
  //: Get last modification time.
  
  DateC FilenameC::LastModTime() const  {
    if(IsEmpty())
      return DateC(false);
#if RAVL_USE_LARGEFILESUPPORT
    struct stat64 buff;
    if(stat64(chars(),&buff) < 0)
      return DateC(false);
#else
    struct stat buff;
    if(stat(chars(),&buff) < 0)
      return DateC(false);
#endif
#if RAVL_TIMET_IS_INT
    return DateC((DateC::SecondT) buff.st_mtime,0);
#else
    return DateC((DateC::SecondT) buff.st_mtim.tv_sec,(long) buff.st_mtim.tv_nsec / 1000);
#endif
  }
  
  //: Get last status modification time.
  
  DateC FilenameC::LastStatusModTime() const  {
    if(IsEmpty())
      return DateC(false);
#if RAVL_USE_LARGEFILESUPPORT
    struct stat64 buff;
    if(stat64(chars(),&buff) < 0)
      return DateC(false);
#else
    struct stat buff;
    if(stat(chars(),&buff) < 0)
      return DateC(false);
#endif
#if RAVL_TIMET_IS_INT
    return DateC((DateC::SecondT) buff.st_ctime,0);
#else
    return DateC((DateC::SecondT) buff.st_ctim.tv_sec,(long) buff.st_ctim.tv_nsec / 1000);
#endif
  }

  //: Get size of file.
  
  StreamSizeT FilenameC::FileSize() const  {
    if(IsEmpty())
      return 0;
#if RAVL_USE_LARGEFILESUPPORT
    struct stat64 buff;
    if(stat64(chars(),&buff) < 0)
      return 0;
#else
    struct stat buff;
    if(stat(chars(),&buff) < 0)
      return 0;
#endif
    return (StreamSizeT) buff.st_size;
  }
  
  //: Copy this file to somewhere else.
  
  bool FilenameC::CopyTo(const FilenameC &oth) const  {
    if((*this) == oth)
      return false; // Can't copy a file to itself.
    if(IsEmpty() || oth.IsEmpty())
      return false;
    ifstream in1(this->chars());
    ofstream in2(oth.chars());
    if(!in1) {
      cerr << "FilenameC::CopyTo(), Failed to open file :" << (*this) << endl;
      return false;
    }
    if(!in2) {
      cerr << "FilenameC::CopyTo(), Failed to open file :" << oth << endl;
      return false;
    }
    const int buffsize = 4096;
    char buff[buffsize];
    do {
      in1.read(buff,buffsize);
      std::streamsize n = in1.gcount();
      in2.write(buff,n);
      //RavlAssert(n == in2.gcount());
    } while(in1) ;
    // FIXME :- Copy access permissions !!
#if RAVL_USE_LARGEFILESUPPORT
    struct stat64 sbuff;
    if(stat64(chars(),&sbuff) != 0) {
      cerr << "FilenameC::CopyTo(), Error reading file mode for:" << (*this) << endl;
      return false; // Copy failed. Error message ??
    }
#else
    struct stat sbuff;
    if(stat(chars(),&sbuff) != 0) {
      cerr << "FilenameC::CopyTo(), Error reading file mode for:" << (*this) << endl;
      return false; // Copy failed. Error message ??
    }
#endif
#if RAVL_COMPILER_VISUALCPP
    if(_chmod(oth.chars(),sbuff.st_mode) != 0) {
      cerr << "FilenameC::CopyTo(), Error changing file mode for:" << oth << endl;
      return false; // Write stats failed.
    }
#else
    if(chmod(oth.chars(),sbuff.st_mode) != 0) {
      cerr << "FilenameC::CopyTo(), Error changing file mode for:" << oth << endl;
      return false; // Write stats failed.
    }
#endif
    return true;
  }
  
  /////////////////////////////////
  //: Are two files identical. byte wise comparison.
  
  bool FilenameC::BinCompare(const FilenameC &oth) const  {
    if(FileSize() != oth.FileSize())
      return false; // That was easy.  
    if(IsEmpty() || oth.IsEmpty())
      return false; // Can't compare non-existent files.
    ifstream in1(this->chars()),in2(oth.chars());
    if(!in1) {
      cerr << "FilenameC::BinCompare(), Failed to open file :" << (*this) << endl;
      return false;
    }
    if(!in2) {
      cerr << "FilenameC::BinCompare(), Failed to open file :" << oth << endl;
      return false;
    }
    const int buffsize = 4096;
    char buff1[buffsize];
    char buff2[buffsize];
    do {
      in1.read(buff1,buffsize);
      in2.read(buff2,buffsize);
      int n1 = (int) in1.gcount();
      RavlAssert(n1 == ((int) in2.gcount()));
      if(memcmp(buff1,buff2,n1) != 0)
	return false;
    } while(in1 && in2) ;
    RavlAssert(!(in1 || in2)); // Both streams should be finished
    return true;
  }
  
  /////////////////////////////
  //: Set the AMD prefix.
  
  void FilenameC::SetAMDPrefix(StringC prefix) 
  { AMDPrefix = prefix; }  
  
  ///////////////////////////////
  //: Get the extension on the file name.
  // That is the text after the last '.' in the name component.
  // Returns empty string if none found.
  
  StringC FilenameC::Extension() const {
    StringC fn = NameComponent(); // Only interested in the name.
    int i;
    for(i = fn.length()-1;i >= 0;i--)
      if(fn[i] == '.')
	break;
    if(i < 0)
      return StringC(); // No extension found.
    return fn.after(i);
  }

  /////////////////////////////
  //: Test if filename has the given extension.
  // caseSens = false means case is ignored.
  
  bool FilenameC::HasExtension(StringC ext,bool caseSens) const  {
    if(IsEmpty())
      return false;
    if(length() < ext.length())
      return false; // File name can't contain the extension.
    // This doesn't actual do anything non-const, so.
    StringC aExt = const_cast<FilenameC &>(*this).from((int) ((int) length() - (int) ext.length()));
    if(caseSens) 
      return (aExt == ext);
    return (aExt.CaseCmp(ext) == 0);
  }
  
}
