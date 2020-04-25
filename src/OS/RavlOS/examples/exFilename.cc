// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.File System"
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/exFilename.cc"

#include "Ravl/OS/Filename.hh"
#include "Ravl/Option.hh"

using namespace RavlN;

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  FilenameC filename = opt.String("",".","Filename");
  opt.Check();
  
  cout << "Exists=" << filename.Exists() << "\n";
  cout << "Filesize=" << filename.FileSize() << "\n";
  return 0;
}
