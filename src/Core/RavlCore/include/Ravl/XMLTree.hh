// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_XMLTREE_HEADER
#define RAVL_XMLTREE_HEADER 1
////////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.XML"
//! example=exXMLTree.cc
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/XMLTree.hh"

#include "Ravl/RefCounter.hh"
#include "Ravl/XMLStream.hh"
#include "Ravl/HashTree.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/DList.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  class XMLTreeC;
  template<typename DataT> class HSetC;

  //! Helper methods for loading an tree.
  //! Used to change the default behaviour of the Read method.

  class XMLTreeLoadC {
  public:
    virtual ~XMLTreeLoadC();
    //! Destructor

    virtual bool NewNode(const StringC &name,
                         const RCHashC<StringC,StringC> &attr,
                         bool isPI,
                         XMLTreeC &node);
    //! Create a new node in the tree.
    
    virtual bool OpenFile(const StringC &name,
                          const StringC &parentFilename,
                          IStreamC &strm);
    //! Open file for reading.
  };

  //! userlevel=Develop
  //: XML parse tree.
  
  class XMLTreeBodyC
    : public HashTreeBodyC<StringC,RCHashC<StringC,StringC> >
  {
  public:
    XMLTreeBodyC();
    //: Default constructor.
    
    XMLTreeBodyC(const StringC &nname,const RCHashC<StringC,StringC> &attrs,bool _isPI = false);
    //: Construct from a name and an attribute table.
    
    XMLTreeBodyC(const StringC &nname);
    //: Construct from a name.
    
    XMLTreeBodyC(BinIStreamC &strm);
    //: Binary stream constructor.
    
    XMLTreeBodyC(std::istream &strm);
    //: Text stream constructor.
    
    virtual bool Save(BinOStreamC &strm) const;
    //: Save to binary stream.
    
    virtual bool Save(std::ostream &strm) const;
    //: Save to text stream.

    explicit XMLTreeBodyC(XMLIStreamC &in,XMLTreeLoadC *loader = 0);
    //: Construct from an XMLStream.

    bool ReadFile(const StringC &filename,XMLTreeLoadC *loader = 0);
    //: Read from a file

    bool Read(IStreamC &in,XMLTreeLoadC *loader = 0);
    //: Read from a stream

    bool Read(XMLIStreamC &in,XMLTreeLoadC *loader = 0);
    //: Read from an XML stream using this node as the root.
    
    bool Read(XMLIStreamC &in,HSetC<StringC> &includedFiles,XMLTreeLoadC *loader = 0);
    //: Read from an XML stream using this node as the root.

    bool ReadNode(XMLIStreamC &in,XMLTreeLoadC *loader = 0);
    //: Read a node from an XML stream.
    // It assumes that the stream is positioned immediately before a "begin" tag.<br>
    // <b>Note:</b> the node tag name <i> must </i> match the name used to construct the XMLTreeC.
    
    bool Write(OStreamC &out, int level=0) const;
    //: Write to a stream using this node as the root.
    // !param level - level of the XML tree for indenting / formatting purposes 
    
    const StringC &Name() const
    { return name; }
    //: Access name.

    std::ostream &Dump(std::ostream &out,int level= 0) const;
    //: Dump tree in a human readable format.
    
    bool IsPI() const
    { return isPI; }
    //: Is this a processing instruction.
    
    bool Add(const StringC &name,const XMLTreeC &subtree);
    //: Add subtree to node.
    // Obsolete; use Add(const XMLTreeC &subtree).
    
    bool Add(const XMLTreeC &subtree);
    //: Add subtree to node.
    
    bool AddContent(const StringC &content);
    //: Add some text to the tree
    
    DListC<XMLTreeC> &Children()
    { return children; }
    //: Access list of children.
    
    const DListC<XMLTreeC> &Children() const
    { return children; }
    //: Access list of children.
    
    bool Child(const StringC &key,XMLTreeC &child) const;
    //: lookup child in tree.
    // Returns true and updates parameter 'child' if child is found.
    
    bool ChildContent(const StringC &key,StringC &value) const;
    //: Get the content of a child if it exists.
    // Return true if value found.

    const RCHashC<StringC,StringC> & Attributes() const
    { return this->Data(); }
    //: Access table of attributes

    StringC AttributeString(const StringC &name,const StringC &defaultValue = StringC("")) const ;
    //: Access attribute.

    bool Attribute(const StringC &name,StringC &data,const StringC &defaultValue = StringC()) const;
    //: Access string attribute.
    // Return true if non default value has been specified.
    
    bool Attribute(const StringC &name,std::string &data,const std::string &defaultValue = std::string()) const;
    //: Access string attribute.
    // Return true if non default value has been specified.

    UIntT AttributeUInt(const StringC &name,UIntT defaultValue = 0) const;
    //: Access attribute.
    
    IntT AttributeInt(const StringC &name,IntT defaultValue = 0) const ;
    //: Access attribute.
    
    RealT AttributeReal(const StringC &name,RealT defaultValue = 0) const;
    //: Access attribute.

    UInt64T AttributeUInt64(const StringC &name,UInt64T defaultValue = 0) const;
    //: Access attribute.

    Int64T AttributeInt64(const StringC &name,Int64T defaultValue = 0) const ;
    //: Access attribute.
    
    bool Attribute(const StringC &name,bool &data,const bool &defaultValue = false) const;
    //: Access boolean attribute.

    bool AttributeBool(const StringC &name,bool defaultValue = false) const ;
    //: Access attribute.
    
    bool HasAttribute(const StringC &name) const;
    //: Test if attribute is set.

    template<typename DataT>
    bool Attribute(const StringC &name,DataT &data,const DataT &defaultValue = DataT()) const {
      const StringC *value = Data().Lookup(name);
      if(value == 0) { 
        data = defaultValue;
        return false;
      }
      StrIStreamC istrm(*value);
      istrm >> data;
      return true;
    }
    //: Access generic attribute.
    // Return true if non default value has been specified.


    StringC Content() const;
    //: Access content of tag (if any)
    
    bool FollowPath(const StringC &path,DListC<XMLTreeC> &nodes);
    //: Give list of nodes matching the given path.
    
    bool FollowPath(const DListC<StringC> &path,DListC<XMLTreeC> &nodes);
    //: Give list of nodes matching the given path.

  protected:
    bool ProcessInclude(XMLTreeC &subtree,
                        HSetC<StringC> &doneFiles,
                        const StringC &parentFilename,
                        XMLTreeLoadC *loader);
    //: Process xi:include directive
    
    bool ProcessIncludeFallback(XMLTreeC &subtree,
                                HSetC<StringC> &doneFiles,
                                const StringC &parentFilename,
                                XMLTreeLoadC *loader);
    //: Look for fallback
    
    static std::ostream &Indent(std::ostream &out,int level);
    bool isPI; // Is this a processing instruction ?
    StringC name;
    DListC<XMLTreeC> children; // Ordered list of children.
  };
  

  //! userlevel=Normal
  //: Parses XML and stores as a tree
  // <code>XMLTreeC</code> is a hash tree, templated on strings for key and data.
  // <ul>
  // <li> The top-level element is a container for the remaining XML - typically it has the name of the XML file.  The contents start at the next level down.
  // <li> The elements contained within an element (children) can be accessed either as a list, as a hash table, or individually by name.
  // <li> Attributes of an element can be accessed as a hash table, using the attribute name as the key.
  // </ul>
  // <p> To summarise: an XML tree node typically has a <code>Name()</code>, <code>Data()</code> (i.e. attributes) and <code>Children()</code> (i.e. contents).</p>
  
  // <p> Note: You can include files into the tree with the syntax from xinclude standard, see http://www.w3.org/TR/xinclude/ for more details though this is only a partial implementation. In particular the xpointer decoding only follows a path to a child. <p>
  // Include example: <code> &gt; xi:include href="other.xml" / &lt; </code>
  
  class XMLTreeC
    : public HashTreeC<StringC,RCHashC<StringC,StringC> >
  {
  public:
    XMLTreeC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    explicit XMLTreeC(bool)
      : HashTreeC<StringC,RCHashC<StringC,StringC> >(*new XMLTreeBodyC())
    {}
    //: Constructs a valid but empty tree, with empty name.
    // The bool argument value has no effect.
    
    explicit XMLTreeC(const char *name)
      : HashTreeC<StringC,RCHashC<StringC,StringC> >(*new XMLTreeBodyC(StringC(name)))
    {}
    //: Constructor a valid but empty tree.
    // The name is used as the "name" attribute for the top level.
    
    XMLTreeC(const StringC &name,const RCHashC<StringC,StringC> &attrs,bool IsPI = false)
      : HashTreeC<StringC,RCHashC<StringC,StringC> >(*new XMLTreeBodyC(name,attrs,IsPI))
    {}
    //: Constructor from a name and an attribute table.
    // If IsPI is true, a processing instruction is created.
    
    explicit XMLTreeC(const StringC &name)
      : HashTreeC<StringC,RCHashC<StringC,StringC> >(*new XMLTreeBodyC(name))
    {}
    //: Constructor from a name.
    
    explicit XMLTreeC(XMLIStreamC &in,XMLTreeLoadC *loader = 0)
      : HashTreeC<StringC,RCHashC<StringC,StringC> >(*new XMLTreeBodyC(in,loader))
    {}
    //: Constructor from an XML stream.

    XMLTreeC(HashTreeC<StringC,RCHashC<StringC,StringC> > &oth)
      : HashTreeC<StringC,RCHashC<StringC,StringC> >(dynamic_cast<const XMLTreeBodyC *>(BodyPtr(oth)))
    {}
    //: Construct from a base class.
    // An invalid handle is created if it fails.
    
  protected:
    XMLTreeC(XMLTreeBodyC &bod)
      : HashTreeC<StringC,RCHashC<StringC,StringC> >(bod)
    {}
    //: Body constructor.
    
    XMLTreeBodyC &Body()
    { return static_cast<XMLTreeBodyC &>(HashTreeNodeC<StringC,RCHashC<StringC,StringC> >::Body()); }
    //: Access body.

    const XMLTreeBodyC &Body() const
    { return static_cast<const XMLTreeBodyC &>(HashTreeNodeC<StringC,RCHashC<StringC,StringC> >::Body()); }
    //: Access body.
    
  public:

    bool ReadFile(const StringC &filename,XMLTreeLoadC *loader = 0)
    { return Body().ReadFile(filename,loader); }
    //: Read from a file
    
    bool Read(IStreamC &in,XMLTreeLoadC *loader = 0)
    { return Body().Read(in,loader); }
    //: Read from a stream using this node as the root.

    bool Read(XMLIStreamC &in,XMLTreeLoadC *loader = 0)
    { return Body().Read(in,loader); }
    //: Read from an XML stream using this node as the root.
    
    bool Read(XMLIStreamC &in,HSetC<StringC> &includedFiles,XMLTreeLoadC *loader = 0)
    { return Body().Read(in,includedFiles,loader); }
    //: Read from an XML stream using this node as the root.
    
    bool ReadNode(XMLIStreamC &in,XMLTreeLoadC *loader = 0)
    { return Body().ReadNode(in,loader); }
    //: Read a node from an XML stream.
    // It assumes that the stream is positioned immediately before a "begin" tag.<br>
    // <b>Note:</b> the node tag name <i> must </i> match the name used to construct the XMLTreeC.

    static XMLTreeC ReadNode(XMLIStreamC &in,const StringC &name,XMLTreeLoadC *loader = 0);
    //: Read a node from an XML stream.
    // It assumes that the stream is positioned immediately before a "begin" tag.<br>
    // <b>Note:</b> if <code>name</code> is not the empty string, it must match the tag name.

    bool Write(OStreamC &out, int level=0) const
    { return Body().Write(out, level); }
    //: Write to a stream using this node as the root.
    
    const StringC &Name() const
    { return Body().Name(); }
    //: Access name.
    
    bool IsPI() const
    { return Body().IsPI(); }
    //: Is this a processing instruction.

    bool Add(const StringC &name,const XMLTreeC &subtree)
    { return Body().Add(name,subtree); }
    //: Add subtree to node.
    // <font color="#880088"> Obsolete</font>; use Add(const XMLTreeC &subtree).
    
    bool Add(const XMLTreeC &subtree)
    { return Body().Add(subtree); }
    //: Add subtree to node.

    bool AddContent(const StringC &content)
    { return Body().AddContent(content); }
    //: Add some text to the tree
    
    DListC<XMLTreeC> &Children()
    { return Body().Children(); }
    //: Access list of children.
    
    const DListC<XMLTreeC> &Children() const
    { return Body().Children(); }
    //: Access list of children.
    
    bool Child(const StringC &key,XMLTreeC &child) const
    { return Body().Child(key,child); }
    //: lookup child in tree.
    // Returns true and updates parameter 'child' if child is found.
    
    bool ChildContent(const StringC &key,StringC &value) const
    { return Body().ChildContent(key,value); }
    //: Get the content of a child if it exists.
    // Return true if value found.

    const RCHashC<StringC,StringC> & Attributes() const
    { return Body().Attributes(); }
    //: Access table of attributes as strings
    // Attribute names &amp; values are RCHashC keys &amp; values respectively

    StringC AttributeString(const StringC &name,const StringC &defaultValue = StringC("")) const 
    { return Body().AttributeString(name,defaultValue); }
    //: Access attribute.
    
    UIntT AttributeUInt(const StringC &name,UIntT defaultValue = 0) const 
    { return Body().AttributeUInt(name,defaultValue); }
    //: Access attribute.
    
    IntT AttributeInt(const StringC &name,IntT defaultValue = 0) const 
    { return Body().AttributeInt(name,defaultValue); }
    //: Access attribute.
    
    RealT AttributeReal(const StringC &name,RealT defaultValue = 0) const 
    { return Body().AttributeReal(name,defaultValue); }
    //: Access attribute.
    
    UInt64T AttributeUInt64(const StringC &name,UInt64T defaultValue = 0) const 
    { return Body().AttributeUInt64(name,defaultValue); }
    //: Access attribute.
    
    Int64T AttributeInt64(const StringC &name,Int64T defaultValue = 0) const 
    { return Body().AttributeInt64(name,defaultValue); }
    //: Access attribute.
    
    bool AttributeBool(const StringC &name,bool defaultValue = false) const 
    { return Body().AttributeBool(name,defaultValue); }
    
    template<typename DataT>
    bool Attribute(const StringC &name,DataT &value,const DataT &defaultValue = DataT()) const
    { return Body().Attribute(name,value,defaultValue); }
    //: Access generic attribute.
    // Return true if non default value has been specified.

    bool HasAttribute(const StringC &name) const
    { return Body().HasAttribute(name); }
    //: Test if attribute is set.

    StringC Content() const
    { return Body().Content(); } 
    //: Access content of tag (if any)
    
    bool FollowPath(const StringC &path,DListC<XMLTreeC> &nodes)
    { return Body().FollowPath(path,nodes); }
    //: Give list of nodes matching the given path.
    // The path can span several layers of the tree: use "/" as separator.

    friend class XMLTreeBodyC;
  };


  
  inline StringC XMLTreeBodyC::Content() const
  {
    if(Children().IsEmpty())
      return StringC();
    return Children().First().Data()["."];
  }


}

#endif
