// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_EXTLIB_HEADER
#define RAVL_EXTLIB_HEADER 1
/////////////////////////////////////////////////////////
//! docentry="Ravl.API.Source Tools.AutoPort"
//! userlevel=Normal
//! file="Ravl/SourceTools/VisualCPP/ExtLib.hh"
//! rcsid="$Id: ExtLib.hh 5240 2005-12-06 17:16:50Z plugger $"
//! lib=RavlAutoPort2

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Information about a face for XML

  class ExtLibBodyC : public RCBodyC {

  public:
    ExtLibBodyC()
    {}
    
    ExtLibBodyC(const StringC & name, const DListC<StringC> & includePaths, DListC<StringC> & libPaths, DListC<StringC> & libs);
    //: Construct from actual id and path to original image
    
  protected: 
   
    ExtLibBodyC(XMLIStreamC& xml);
    //: Constructor from XML stream

    XMLOStreamC& SaveXML(XMLOStreamC &xml) const;
    //: Save to xml stream
  
  public:

    StringC Name(void) const 
    { return name; }
    //: The actual id of client
    
    DListC<StringC> IncludePaths() const
      { return includePaths; }
    //: The include paths

    DListC<StringC> LibPaths() const
      { return libPaths; }
    //: The lib paths

    DListC<StringC> Libs() const
      { return libs; }
    //: The libs

    unsigned int Hash(void) const
    { return name.Hash(); } 
    //: Create a hash value
        
  protected:

    StringC name;
    //: The name of the library

    DListC<StringC>includePaths;
    //: The include paths
    
    DListC<StringC>libPaths;
    //: The library paths
        
    DListC<StringC>libs;
    //: The libs


    friend class ExtLibC;

  };

  //! userlevel="Normal"
  //: Information about a face for XML
  
  class ExtLibC 
    : public RCHandleC<ExtLibBodyC> 
  {
  public:
    ExtLibC() 
    {}
    //: Default constructor
    // Creates invalid handle
    
    explicit ExtLibC(const StringC & name, const DListC<StringC> & includePaths, DListC<StringC> & libPaths, DListC<StringC> & libs) 
      : RCHandleC<ExtLibBodyC>(*new ExtLibBodyC(name, includePaths, libPaths, libs))
    {}
    //: Construct from actual id and path to the original image
    //protected:

    ExtLibC(XMLIStreamC& xml) 
      : RCHandleC<ExtLibBodyC>(*new ExtLibBodyC(xml))
    {}
    //: Constructor
    // Creates faceinfo from an XML stream

    XMLOStreamC& SaveXML(XMLOStreamC &xml) const 
    { return Body().SaveXML(xml); }
    //: Save to xml stream

  protected:
    
    ExtLibC(ExtLibBodyC &bod)
      : RCHandleC<ExtLibBodyC>(bod)
    {}
    //: Body constructor.

    ExtLibBodyC &Body()
    { return RCHandleC<ExtLibBodyC>::Body(); }
    //: Access body.
    
    const ExtLibBodyC &Body() const
    { return RCHandleC<ExtLibBodyC>::Body(); }
    //: Access body (const)

  public:
    StringC Name(void) const 
    { return Body().Name(); }
    //: The actual id of client
    
    DListC<StringC> IncludePaths() const
      { return Body().IncludePaths(); }
    //: The include paths

    DListC<StringC> LibPaths() const
      { return Body().LibPaths(); }
    //: The lib paths

    DListC<StringC> Libs() const
      { return Body().Libs(); }
    //: The libs

    unsigned int Hash(void) const
    { return Body().Hash(); } 
    //: Create a hash value
    
    friend XMLIStreamC &operator>>(XMLIStreamC &xml, ExtLibC &data);
    friend XMLOStreamC &operator<<(XMLOStreamC &xml, const ExtLibC &data);
    
  };

  BinOStreamC &operator<<(BinOStreamC &strm,const ExtLibC &data);
  //: Output to binary stream.
  // Not implemented.
  
  BinIStreamC &operator>>(BinIStreamC &xml,ExtLibC &data);
  //: Read from binary stream.
  // Not implemented.

  std::ostream &operator<<(std::ostream &strm,const ExtLibC &data);
  //: Output to binary stream.
  // Not implemented.
  
  std::istream &operator>>(std::istream &xml,ExtLibC &data);
  //: Read from binary stream.
  // Not implemented.
  
  XMLIStreamC &operator>>(XMLIStreamC &xml,const ExtLibC &data);
  //: XML Input
  // DO NOT USE DIRECTLY unless you're using this inside another XML loader and you know what you're doing.
  // Use Load() instead.

  XMLOStreamC &operator<<(XMLOStreamC &xml, const ExtLibC &data);
  //: XML Output
  // DO NOT USE DIRECTLY unless you're using this inside another XML output func and you know what you're doing.
  // Use Save() instead.

  void InitExtLibFormat();

};


#endif
