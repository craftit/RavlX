// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/testXMLStream.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.XML"

#include "Ravl/XMLStream.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Option.hh"

using namespace RavlN;

int testXML(int argc, char* argv[]) {
  OptionC option(argc,argv,true);
  StringC infile = option.String("i","","Input filename");
  StringC outfile = option.String("o","","Output filename");
  option.Check();
  
  if(outfile != "") {
    XMLOStreamC os(outfile);
#if 1
    os.Comment("A comment");
    os << XMLIndent << XMLStartTag("hello") << XMLEmptyTag;
    int i = 1;
    i++;
    os << XMLIndent << XMLStartTag("test") << XMLAttribute("Fred",i) << XMLAttribute("Jim",i) << XMLContent 
       << XMLIndent <<  "Some data... ";
    os << XMLIndentDown << XMLEndTag << "\n";
#endif    
#if 0
#if 0
    HashC<StringC,StringC> testTab;
    testTab["Hello"] = "one";
    testTab["Boo"] = "two";
    os << testTab;
#else
    SArray1dC<int> arr(3);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    os << arr;
#endif
#endif

  }
  
  if(infile != "") {
    XMLIStreamC is(infile);
#if 0
    RCHashC<StringC,StringC> attr;
    StringC str;
    while(is) {
      std::cerr << "Looking for tag.\n";
      if(is.ReadTag(str,attr)) {
	cerr << "Got tag '" << str << "' with " << attr.Size() << " attributes \n";
      }
    }
#endif
    SArray1dC<int> arr;
    is >> arr;
    std::cerr << arr;
  }
  return 0;
}

RAVL_ENTRY_POINT(testXML);
