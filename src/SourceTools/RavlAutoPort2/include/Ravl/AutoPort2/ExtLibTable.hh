// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_EXTLIBTABLE_HEADER
#define RAVL_EXTLIBTABLE_HEADER 1
/////////////////////////////////////////////////////////
//! docentry="Ravl.API.Source Tools.AutoPort"
//! userlevel=Normal
//! file="Ravl/SourceTools/VisualCPP/ExtLibTable.hh"
//! rcsid="$Id: ExtLibTable.hh 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlAutoPort2

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/String.hh"
#include "Ravl/AutoPort2/ExtLib.hh"

namespace RavlN {

  
  //! userlevel="Normal"
  //: A RCHashC of StringC and ExtLibTableC with XML streams
  //: The string is the unique id of face, taken to be the leading
  class ExtLibTableC 
    : public RCHashC<StringC, ExtLibC> {

  public:

    ExtLibTableC()
      {}
    //: Default Constructor
    
  protected: 
    
    ExtLibTableC(XMLIStreamC& xml);
    //: Constructor from XML stream

    XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
    //: Save to xml stream
  
  public:    
 
  protected:

    friend XMLIStreamC &operator>>(XMLIStreamC &xml, ExtLibTableC &data);
    friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ExtLibTableC &data);
    
  };


  
  XMLIStreamC &operator>>(XMLIStreamC &xml, ExtLibTableC &data);
  //: XML Input
  // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
  // Use Load() instead.

  XMLOStreamC &operator<<(XMLOStreamC &xml, const ExtLibTableC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

  void InitExtLibTableFormat();
  
}


#endif
