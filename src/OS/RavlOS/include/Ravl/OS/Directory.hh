// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILEDIRECTORY_HEADER
#define RAVL_FILEDIRECTORY_HEADER
///////////////////////////////////////////////////////////////////////
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.File System"
//! lib=RavlOS
//! rcsid="$Id$"
//! file="Ravl/OS/FileSystem/Directory.hh"
//! date="07/05/98"

#include "Ravl/OS/Filename.hh"
#include "Ravl/DList.hh"

namespace RavlN {

  const static StringC imageExtensions("jpg JPG jpeg JPEG png PNG ppm pgm");

  //: Directory handling.
  // Used for querying and changing the contents of directories. <p>
  // The class is designed to be thread safe where different
  // instances of DirectoryC are used simulatiously and between
  // 'const' methods of this class.
  
  class DirectoryC : public FilenameC {
  public:    
    inline DirectoryC(const StringC &name = StringC("."));
    //: Construct from a string.
    
    inline DirectoryC(const DirectoryC &name);
    //: Copy constructor.
    
    inline bool Create(const FilePermissionC &Acc = FilePermissionC()) const
      { return FilenameC::MakeDir(Acc); }
    //: Create a directory by this name.
    
    inline bool IsValid() const
      { return FilenameC::IsDirectory(); }
    //: Is this really a directory ?
    
    DListC<StringC> List() const;
    //: List contents of directory.
    // Will return an empty list if anything goes wrong.
    // Gives names of contents relative to this directory
    // , without any leading or trailing "/" <p>
    // '..' and '.' are not returned in the list.
    
    DListC<StringC> List(const StringC &pre,const StringC &post) const;
    //: List contents of directory, returning only files with the given
    //: prefix and postfix.
    // Either (or both) pre and post maybe empty strings..
    // '..' and '.' are not returned in the list. <p>
    // Will return an empty list if anything goes wrong. 
    
    DListC<StringC> FiltList(const StringC &filter) const;
    //: List contents of directory, returning only files matching
    //: the given filter.
    // The wild cards are the usual UNIX ones: '*','?' and '\' to escape.  <br>
    // '*' Matches any string. '?' matches any single charactor. <br>
    // '..' and '.' are not returned in the list. 

    DListC<StringC> SuffixList(const StringC &suffices=imageExtensions) const;
    //: List contents of directory, returning only files with suffices in string
    // Returns a list of absolute paths to matching files. <br>
    // Default suffices are for image files.  See header file for list.
    
    DListC<StringC> SearchTree(const StringC &filter) const;
    //: Search a directory tree for files matching 'filter'
    // Returns a list of absolute paths to matching files. The filter
    // is the same as 'FiltList'
    
    bool Copy(const FilenameC &othDir,bool rec = true,bool lazy = false,bool sync = false,bool verb = false);
    //: Copy all files in this directory into 'othDir'.
    // This will make 'othDir' if necessary. When 'rec' is true
    // it will do so recursively, if false subdirectories will be
    // ignored.  If othDir is an empty string the currect directory 
    // is used. <p>
    // if lazy is true, files are only updated if they have a later 
    // date than the target. <p>
    // if sync is true, all files that are not in the source directory 
    // are removed. <p>
    
    bool Sync(bool metaDataToo = true);
    //: Sync contents of directory onto the disk.
    
  protected:
    static bool MatchFilt(const char *filter,const char *str);
    //: Simple test if 'x' matches filter.
    // Wild cards are '*','?' and '\' escapes. 
  
  };
  
  ////////////////////////////////////////////////////////////
    
  inline 
  DirectoryC::DirectoryC(const StringC &name)
    : FilenameC(name)
  {}
  
  inline 
  DirectoryC::DirectoryC(const DirectoryC &name)
    : FilenameC(name)
  {}
}

#endif
