// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! author="Charles Galambos"
//! rcsid="$Id"
//! lib=RavlOS
//! file="Ravl/OS/FileSystem/testFileFormat.cc"
//! docentry="Ravl.API.Core.Data Processing" 
//! userlevel=Develop

//: Test file format handling
// This test really should be in the core, but we don't have
// the tools to deal with files there. 

#include "Ravl/Option.hh"
#include "Ravl/DP/FileFormatIO.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Stream.hh"
#include "Ravl/UnitTest.hh"

namespace RavlN {
  extern int IncludeFileFormatStream();
  extern void InitFileFormatBinStream();
}

using namespace RavlN;
///////////////

int TestStream(FilenameC &fn);
int TestBinStream(FilenameC &fn);
int TestFilename();

/////////////////
bool verb = false;

int main(int argc,char **argv) 
{
  IncludeFileFormatStream();
  InitFileFormatBinStream();
  
  OptionC option(argc,argv,true);
  FilenameC infile = option.String("i","/tmp/fsin","Input filename");
  verb = option.Boolean("v",false,"Verbose. \n");
  option.Check();
  int ret;

  RAVL_RUN_TEST(TestStream(infile));
  RAVL_RUN_TEST(TestBinStream(infile));
  RAVL_RUN_TEST(TestFilename());
  std::cout << "TEST PASSED. \n";
  return 0;
}

int TestFilename() {
  
  // Check name component.
  
  if(FilenameC("name").NameComponent() != "name")
    return __LINE__;
  
  if(FilenameC("path/name").NameComponent() != "name")
    return __LINE__;
  
  if(FilenameC("path/").NameComponent() != "")
    return __LINE__;
  
  if(FilenameC("/name").NameComponent() != "name")
    return __LINE__;

  if(FilenameC("/name/xyz").NameComponent() != "xyz")
    return __LINE__;
  
  if(FilenameC("/name/xyz/123").NameComponent() != "123")
    return __LINE__;
  
  if(FilenameC("/name/xyz/123/").NameComponent() != "")
    return __LINE__;
  

  // Check path component.
  
  if(FilenameC("name").PathComponent() != "")
    return __LINE__;
  
  if(FilenameC("path/name").PathComponent() != "path")
    return __LINE__;
  
  if(FilenameC("path/").PathComponent() != "path")
    return __LINE__;
  
  if(FilenameC("/name").PathComponent() != "")
    return __LINE__;
  
  if(FilenameC("/name/xyz").PathComponent() != "/name")
    return __LINE__;
  
  if(FilenameC("/name/xyz/123").PathComponent() != "/name/xyz")    
    return __LINE__;
  
  if(FilenameC("/name/xyz/123/").PathComponent() != "/name/xyz/123")
    return __LINE__;
  
  return 0;
}

int TestStream(FilenameC &infile)
{
  RealT val;
  val = 21;
  FilenameC fn = infile.MkTemp();
  
  if(!Save(fn,val,"stream",verb)) {
    std::cerr << "Failed to save data. \n";
    return __LINE__;
  }
  
  val = 0;
  if(!Load(fn,val,"",verb)) {
    std::cerr << "Failed to load data. \n";
    return __LINE__;
  }
  
  std::cout << "Loaded :" << val << std::endl;
  if(val != 21) {
    std::cerr << "ERROR: Test Failed ! \n";
    return __LINE__;
  }
  
  fn.Remove();// Remove test file.
  return 0;
}


int TestBinStream(FilenameC &infile)
{
  IntT val;
  val = 20;
  FilenameC fn = infile.MkTemp();
  if(!Save(fn,val,"abs",verb)) {
    std::cerr << "Failed to save data. \n";
    return __LINE__;
  }
  
  val = 0;
  if(!Load(fn,val,"",verb)) {
    std::cerr << "Failed to load data. \n";
    return __LINE__;
  }
  
  std::cout << "Loaded :" << val << std::endl;
  if(val != 20) {
    std::cerr << "ERROR: Test Failed ! \n";
    return __LINE__;
  }
  fn.Remove(); // Remove test file.
  return 0;
}
