// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLGUI_GLADEXML_HEADER
#define RAVLGUI_GLADEXML_HEADER 1
//! rcsid="$Id$"
//! docentry="Ravl.API.Graphics.GTK.LibGlade"
//! lib=RavlLibGlade
//! file="Ravl/GUI/LibGlade/GladeXML.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/GUI/Widget.hh"
#include "Ravl/String.hh"

extern "C" {
  typedef struct _GladeXML GladeXML;
}

namespace RavlN {
  class XMLFactoryContextC;
}

namespace RavlGUIN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: Glade XML file
  
  class GladeXMLBodyC
    : public RCBodyC
  {
  public:
    GladeXMLBodyC();
    //: Default constructor.
    
    GladeXMLBodyC(const StringC &filename);
    //: Create whole interface from a file.
    
    GladeXMLBodyC(const StringC &filename,const StringC &rootWidgetName,const StringC &moduleName = StringC("Glade"));
    //: Create part of interface from a file.
    
    GladeXMLBodyC(const XMLFactoryContextC &factory);
    //: Factory constructor.

    GladeXML *XML();
    //: Access xml info for whole interface.
    
    GtkWidget *Widget(const StringC &widgetName);
    //: Find named widget.
    
    const StringC &Filename() const
    { return filename; }
    //: Access filename.
    
    const StringC &ModuleName() const
    { return m_moduleName; }
    //: Name of module to use.

    const StringC &RootWidgetName() const
    { return rootWidgetName; }
    //: Get name of root widget.
    
  protected:
    bool Create();
    //: Create interface.
    
    GladeXML *xml;
    StringC filename;
    StringC m_moduleName;
    StringC rootWidgetName;
  };
  
  //! userlevel=Normal
  //: Glade XML file
  
  class GladeXMLC
    : public RCHandleC<GladeXMLBodyC>
  {
  public:
    GladeXMLC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    GladeXMLC(const StringC &filename)
      : RCHandleC<GladeXMLBodyC>(*new GladeXMLBodyC(filename))
    {}
    //: Create whole interface from a file.
    
    GladeXMLC(const StringC &filename,const StringC &rootWidgetName,const StringC &moduleName = StringC("Glade"))
      : RCHandleC<GladeXMLBodyC>(*new GladeXMLBodyC(filename,rootWidgetName,moduleName))
    {}
    //: Create part of interface from a file.

    GladeXMLC(const XMLFactoryContextC &factory)
     : RCHandleC<GladeXMLBodyC>(*new GladeXMLBodyC(factory))
    {}
    //: Factory constructor.

  protected:
    GladeXMLBodyC &Body()
    { return RCHandleC<GladeXMLBodyC>::Body(); }
    //: Access body.
    
    const GladeXMLBodyC &Body() const
    { return RCHandleC<GladeXMLBodyC>::Body(); }
    //: Access body.
    
  public:
    GladeXML *XML()
    { return Body().XML(); }
    //: Access xml info for whole interface.
    
    GtkWidget *Widget(const StringC &widgetName)
    { return Body().Widget(widgetName); }
    //: Find named widget.
    
    const StringC &Filename() const
    { return Body().Filename(); }
    //: Access filename.
    
    const StringC &ModuleName() const
    { return Body().ModuleName(); }
    //: Access module name.

    const StringC &RootWidgetName() const
    { return Body().RootWidgetName(); }
    //: Get name of root widget.
    
  };

  inline std::istream &operator>>(std::istream &strm,GladeXMLC &obj) {
    RavlAssertMsg(0,"Not implemented. ");
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.

  inline std::ostream &operator<<(std::ostream &out,const GladeXMLC &obj) {
    RavlAssertMsg(0,"Not implemented. ");
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  inline BinIStreamC &operator>>(BinIStreamC &strm,GladeXMLC &obj) {
    RavlAssertMsg(0,"Not implemented. ");
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.

  inline BinOStreamC &operator<<(BinOStreamC &out,const GladeXMLC &obj) {
    RavlAssertMsg(0,"Not implemented. ");
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.
}

#endif
