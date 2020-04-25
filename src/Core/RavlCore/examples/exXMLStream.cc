// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.XML"
//! author="Charles Galambos"
//! file="Ravl/Core/System/exXMLStream.cc"

#include "Ravl/StrStream.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter.hh"

using namespace RavlN;

int exXMLStream(int nargs,char **argv)
{
  // Create some data.
  
  SArray1dC<RealT> arr(3);
  RealT c = 1.5;
  for(SArray1dIterC<RealT> it(arr);it;it++) {
    *it = c;
    c += 0.5;
  }
  
  StringC data;
  {
    // Write some data to an xml stream.
    
    StrOStreamC ostr;
    XMLOStreamC strm(ostr);
    
    strm << XMLStartTag("array") << XMLAttribute("size",arr.Size()) ;
      //<< XMLContent;
    for(SArray1dIterC<RealT> it(arr);it;it++) {
      strm << XMLStartTag("data") << XMLContent << *it;
      strm << XMLEndTag;
    }
    strm << XMLEndTag;
    
    data = ostr.String();
  }
  std::cerr << "Stream=" << data << "\n\n";
  
  {
    // Read some data from an xml stream.
    
    StrIStreamC istr(data);
    XMLIStreamC strm(istr);
    StringC tagName;
    if(strm.ReadTag(tagName) != XMLBeginTag)
      throw ExceptionInvalidStreamC("Unexpected tag when reading data. ");
    if(tagName != "array") // We're expecting an array tag here.
      throw ExceptionInvalidStreamC("Unexpected tag name when reading data. ");
    UIntT size = 0;
    strm >> XMLAttribute("size",size); // Read size attribute of last tag.
    
    SArray1dC<RealT> iarr(size);
    for(SArray1dIterC<RealT> it(iarr);it;it++) {
      // Read in next tag from stream.
      
      XMLTagOpsT tt = strm.ReadTag(tagName);
      if(tt == XMLEndTag)   // Got end tag ?
	break; // Then Finished here.
      
      if(tagName == "data") { // Got a tag called 'data'
	StringC val;
	strm >> val; // Read in tag contents
	*it = val.RealValue();
	strm >> XMLEndTag; // Skip end tag.
	continue;
      }
      
      // Handle unexpected tags.
      std::cerr << "WARNING: Ignoring tag '" << tagName << "'\n";
      strm.SkipElement();
    }
    
    std::cerr << "\nArray=" << iarr << "\n";
  }
  return 0;
}

RAVL_ENTRY_POINT(exXMLStream);
