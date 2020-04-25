// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/Directory.cc"

// This config stuff should be moved else where.

#include "Ravl/config.h"
#if RAVL_OS_MACOSX
#define _DARWIN_C_SOURCE 1
#endif

#if RAVL_OS_SOLARIS
#define _POSIX_PTHREAD_SEMANTICS 1
#endif

#include "Ravl/OS/Directory.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Exception.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HSet.hh"
#include "Ravl/StringList.hh"

#include <stdlib.h>

#ifdef WIN32
#include <windows.h>
#include <wchar.h>
#else

#if RAVL_HAVE_UNIXDIRFUNCS
#include <dirent.h>
#endif
#if RAVL_HAVE_UNISTD_H
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif
#if !RAVL_HAVE_REENTRANT_UNIXDIRFUNCS
#include "Ravl/MTLocks.hh"
#endif
#endif

#define DPDEBUG 0

#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  static inline bool ReturnInList(const char *nm) {
    if(nm[0] != '.')
      return true;
    if(nm[1] == 0)
      return false; // skip "."
    if(nm[1] == '.')
      if(nm[2] == 0)
	return false;
    return true;
  }
  
  //////////////////////////////////////////////////////
  //: List contents of directory.
  
  DListC<StringC> DirectoryC::List() const  {
#ifdef WIN32
    DListC<StringC> ret;
    WIN32_FIND_DATA dataFind;
    memset(&dataFind, 0, sizeof(WIN32_FIND_DATA));

    StringC strSearch = StringC(chars()) + RavlN::filenameSeperator + '*';
#if !RAVL_COMPILER_VISUALCPPNET_2005
    HANDLE hFindFile = FindFirstFile(strSearch.chars(), &dataFind);
    BOOL bFoundNext = hFindFile ? true : false;
    while (bFoundNext) {
      const char *name = dataFind.cFileName;
      if(name[0] != 0 && (name[0] != '.' || (name[1] != 0 && (name[1] != '.' || name[2] != 0))))
        ret.InsLast(StringC(name));
      bFoundNext = FindNextFile(hFindFile, &dataFind);
    }
#else
    //char wsbuff[1025];
    size_t size = 0;
    //mbstowcs_s(&size,wsbuff, 1024,strSearch.chars(), strSearch.length());

#ifdef UNICODE

    // CP_UTF8
    int sz = MultiByteToWideChar(CP_ACP, 0, strSearch.chars(), -1, NULL, 0);
    wchar_t* wbuf = new wchar_t[sz*sizeof(wchar_t)];
    MultiByteToWideChar(CP_ACP, 0, strSearch.chars(), -1, wbuf, sz*sizeof(wchar_t));
    HANDLE hFindFile = FindFirstFile(wbuf, &dataFind);
    delete[] wbuf;

    //HANDLE hFindFile = FindFirstFile((LPCWSTR)(strSearch.chars()), &dataFind);
#else
    HANDLE hFindFile = FindFirstFile(strSearch.chars(), &dataFind);
#endif

    BOOL bFoundNext = hFindFile ? true : false;
    while (bFoundNext) {
          mbstate_t state;
          //char cbuff[1025];
          size_t csize = 0;

          char *wname, *buf = NULL;

#ifdef UNICODE

        // CP_UTF8
        sz = WideCharToMultiByte(CP_ACP, 0, dataFind.cFileName, -1, NULL, 0, NULL, NULL);
        buf = new char[sz];
        WideCharToMultiByte(CP_ACP, 0, dataFind.cFileName, -1, buf, sz, NULL, NULL);
        wname = buf;
#else
        wname = (char*)dataFind.cFileName;
#endif

          //wcsrtombs_s(&csize,cbuff,1024,(const char **) &wname,1024,&state);
        ret.InsLast(StringC(wname));
        if(buf != NULL) delete[] buf;
        bFoundNext = FindNextFile(hFindFile, &dataFind);
    }

#endif
    return ret;
#else
#if RAVL_HAVE_UNIXDIRFUNCS
    DListC<StringC> ret;
    struct dirent *entry;
    DIR *dinf = opendir(chars());
    if(dinf == NULL)
      return ret; // Empty list, open failed.
#if RAVL_HAVE_REENTRANT_UNIXDIRFUNCS
    struct dirent *buff=(dirent *) malloc(sizeof(struct dirent) + pathconf(".",_PC_NAME_MAX)+1); 
    if(buff == 0) {
      closedir(dinf);
      throw ExceptionC("DirectoryC::List(), Failed to allocate buffer.");
    }
    // This seems to work everywhere.
    entry = 0;
    while(1) {
#if RAVL_READDIR_R_NOBUFF
      readdir_r(dinf,buff);
      entry = buff;
#else
      readdir_r(dinf,buff,&entry);
#endif
      if(entry!=buff)
	break;
      if(!ReturnInList(entry->d_name))
	continue;
      ret.InsLast(FilenameC(entry->d_name));
    }
    free(buff);
#else
    // This is non-reentrant, but portable.
    // so we have to do some locking.
    MTWriteLockC lock(2); // It involves modifying global buffers !
    while((entry = readdir(dinf)) != NULL) { 
      if(!ReturnInList(entry->d_name))
	continue;
      ret.InsLast(FilenameC(entry->d_name));
    }
    lock.Unlock(); // Unlock here, we're done.
#endif
    closedir(dinf);
    return ret;
#else
    throw ExceptionC("DirectoryC::List(), Not implemented. ");
#endif

#endif
  }
  
  //////////////////////////////////////////////////////
  //: List contents of directory, returning only files with the given
  //: prefix and postfix.
  // Either (or both) pre and post maybe empty strings..
  // Will return an empty list if anything goes wrong. 
  // This could be faster, we could filter as we build the list. Later.
  
  DListC<StringC> DirectoryC::List(const StringC &pre,const StringC &post) const { 
    DListC<StringC> ret = DirectoryC::List();
    for(DLIterC<StringC> it(ret);it.IsElm();it.Next()) {
      // Filter out files we're not interested in..
      ONDEBUG(cerr << "DirectoryC::List() Testing:" << it.Data() << "\n");
      if(!pre.IsEmpty()) {
	if(!it.Data().matches(pre,0,false)) {
	  it.Del();
	  continue;
	}
      }
      if(!post.IsEmpty()) {
	if(!it.Data().matches(post,it.Data().length() - post.length(), false)) {
	  it.Del();
	  continue;
	}
      }
    }
    return ret;
  }
  
  //: List contents of directory, returning only files matching
  //: the given filter.
  
  DListC<StringC> DirectoryC::FiltList(const StringC &filter) const {
    DListC<StringC> ret = DirectoryC::List();
    for(DLIterC<StringC> it(ret);it.IsElm();it.Next()) {
      ONDEBUG(cerr << "DirectoryC::FiltList() Testing:" << it.Data() << "\n");
      if(!MatchFilt(filter.chars(),it.Data().chars()))
	it.Del();
    }
    return ret;
  }
  
  //: Simple test if 'x' matches filter.
  // Wild cards are '*','?' and '\' escapes. 
  
  bool DirectoryC::MatchFilt(const char *filt,const char *str) {
    char nextf;
    while(*str != 0) {
      switch(*filt)
	{
	case '*': // Match any sub-string.
	  filt++;
	  nextf = *filt;
	  // Find the next real character in filter.
	  // Could check for '*' or '?' here, but we're but
	  // this routine isn't intended to be that clever.
	  if(nextf == '\\')
	    nextf = *(++filt);
	  // Look for end of substring...
	  for(;*str != 0;str++) {
	    if(*str == nextf && MatchFilt(filt,str))
	      return true; // Found !
	  }
	  return *str == nextf; // Maybe end of filter and string.
	case 0: // End of string ?
	  return false; // If it was end of input we'd never have got here.
	default: // Require exact match.
	  if(*str != *filt)
	    return false;
	    /* no break */
	case '?': // Match any character.
	  str++;
	  filt++;
	  break;
	}
    }
    return *filt == 0; // Must have found end of input, is filter there as well ?
  }
  
  //: List contents of directory, returning only files with suffices in string
  DListC<StringC> DirectoryC::SuffixList(const StringC &suffices) const {
    StringListC lst(suffices);
    DListC<StringC>names;
    for(DLIterC<StringC>it(lst);it;it++) {
      names += this->FiltList("*" + *it);
    }
    DListC<StringC> result;
    for(DLIterC<StringC>it(names);it;it++)
      result += (FilenameC)(*this) + "/" + *it;
    return result;
  }

  //: Search a directory tree for files matching 'filter'
  // Returns a list of absolute paths to matching files. The filter
  // is the same as 'FiltList'
  
  DListC<StringC> DirectoryC::SearchTree(const StringC &filter) const {
    DListC<StringC> ret;
    DListC<StringC> list = DirectoryC::List();
    DirectoryC file;  
    for(DLIterC<StringC> it(list);it;it++) {
      ONDEBUG(cerr << "DirectoryC::SearchTree() Testing:" << it.Data() << "\n");
      if(it->lastchar() != RavlN::filenameSeperator)
      	file = (*this) + RavlN::filenameSeperator + *it;
      else
	      file = (*this) + *it;
      if(file.IsDirectory()) {
	      DListC<StringC> lst = file.SearchTree(filter);
      	ret.MoveLast(lst);
      }
      else { // Don't match directories...
	      if(MatchFilt(filter.chars(),it.Data().chars()))
      	  ret.InsLast(file);
      }
    }
    return ret;
  }

  /////////////////////////////////////
  //: Copy all files in this directory into 'othDir'.
  // This will make 'othDir' if necissary. When 'rec' is true
  // it will do so recursively, if false sub directories will be
  // ignored.
  
  bool DirectoryC::Copy(const FilenameC &othDir,bool rec,bool lazy,bool sync,bool verb) {
    if(verb)
      cerr << "Copying directory '" << (*this) << "' to '" << othDir << "'\n";
    if(!othDir.Exists()) 
      othDir.MakeDir();
    if(!othDir.IsDirectory()) {
      cerr << "DirectoryC::Copy(), Failed to create '" << othDir << "' \n";
      return false; // Failed.
    }
    bool ret = true;
    HSetC<FilenameC> fileset;
    DListC<StringC> lst(List());
    for(DLIterC<StringC> it(lst);it.IsElm();it.Next()) {
      FilenameC lookat((*this) + RavlN::filenameSeperator + it.Data());
      FilenameC target(othDir + RavlN::filenameSeperator + it.Data());
      if(lookat.IsDirectory()) {
	if(!rec) // Recursive copy ?
	  continue; // Ignore sub-directories
	DirectoryC sub(lookat);
	if(!sub.Copy(othDir + RavlN::filenameSeperator + it.Data(),rec,lazy,sync,verb))
	  ret = false;
	continue;
      }
      if(!lookat.IsRegular()) { // Is it a normal file ??
	cerr << "DirectoryC::Copy(), Ignoring special file : '" << lookat << "'\n";
	continue;
      }
      if(sync)
	fileset.Insert(it.Data());
      // Lazy copy, look at mod times before doing any work.
      if(lazy) {
	if(target.LastModTime() > lookat.LastModTime())
	  continue;
      }
      if(verb)
	cerr << "Copying file '" << lookat << "' to '" << target << "'\n";
      if(!lookat.CopyTo(target)) { // Copy the file.
	cerr << "DirectoryC::Copy(), Failed to copy '" << lookat << "' to '" << target << "' \n";
	return false; // Failed to copy file.
      }
    }
    if(!sync) // Remove old files ?
      return ret;
    // Check all files in target directory should actual be there....
    DirectoryC odir(othDir);
    DListC<StringC> tLst(List());
    for(DLIterC<StringC> it2(tLst);it2.IsElm();it2.Next()) {
      if(fileset.IsMember(it2.Data()))
	continue;
      FilenameC target(othDir + RavlN::filenameSeperator + it2.Data());
      if(verb)
	cerr <<"Deleting old file '" << target << "' \n";
      if(!target.Remove())
	cerr << "DirectoryC::Copy(), Failed to delete '" << target << "' \n";
    }
    return true;
  }
  
    
  //: Sync contents of directory onto the disk.
  
  bool DirectoryC::Sync(bool metaDataToo) {
#if RAVL_HAVE_INTFILEDESCRIPTORS && RAVL_HAVE_UNIXDIRFUNCS
    DIR *dirInfo = opendir(this->chars());
    if(dirInfo == 0) {
      cerr << "DirectoryC::Sync, Open failed error=" << errno << "\n";
      return false;
    }
    bool ret = false;
    int fd = dirfd(dirInfo);
    if(fd < 0) { // Check we've got a file descriptor.
      cerr << "DirectoryC::Sync, failed to find file descriptor\n";
    } else {
#if RAVL_HAVE_FDATASYNC
      if(metaDataToo)
        ret = (fsync(fd) == 0);
      else
        ret = (fdatasync(fd) == 0);
#else
      ret = (fsync(fd) == 0);
#endif
      if(!ret) {
        cerr << "DirectoryC::Sync, Sync failed error=" << errno << "\n";
      }
    }
    closedir(dirInfo);
    return ret;
#else
    return true;
#endif    
  }

}
