// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_XMLSTREAM_HEADER
#define RAVL_XMLSTREAM_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! docentry="Ravl.API.Core.XML"
//! author="Charles Galambos"
//! file="Ravl/Core/System/XMLStream.hh"
//! example=exXMLStream.cc

#include "Ravl/Stream.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/String.hh"
#include "Ravl/Stack.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/StrStream.hh"

namespace RavlN {

  enum XMLTagOpsT 
    { XMLEndTag, XMLContent, XMLIndent,XMLIndentDown, XMLBeginTag, XMLEmptyTag, XMLComment, XML_PI };
    
  //! userlevel=Advanced
  //: Class for handling XML attributes.
  // SMALL object. <p>
  // Note: This only keeps a reference to the data, not an
  // actual copy.
  
  template<class DataT>
  class XMLAttributeC {
  public:
    XMLAttributeC(const StringC &nm,DataT &ndat)
      : name(nm),
	dat(ndat)
    {}
    //: Constructor.
    
    StringC &Name()
    { return name; }
    //: Access name of attribute.
    
    const StringC &Name() const
    { return name; }
    //: Access name of attribute.
    
    DataT &Data()
    { return dat; }
    //: Access data in attribute.
    
    const DataT &Data() const
    { return dat; }
    //: Access data in attribute.
    
  protected:
    StringC name;
    DataT &dat;
  };
  
  //! userlevel=Develop
  //: Information about the current element being processed.
  
  class XMLElementBodyC 
    : public RCBodyC
  {
  public:
    XMLElementBodyC()
    {}
    //: Constructor.

    XMLElementBodyC(const StringC &nm,bool anEmptyTag = false)
      : name(nm),
        emptyTag(anEmptyTag)
    {}
    //: Constructor.
    
    XMLElementBodyC(const StringC &nm,const RCHashC<StringC,StringC> &attrs,bool anEmptyTag = false)
      : name(nm),
        attribs(attrs),
        emptyTag(anEmptyTag)
    {}
    //: Constructor.
    
    StringC &Name()
    { return name; }
    //: Name of context.
    
    const StringC &Name() const
    { return name; }
    //: Name of context.
    
    RCHashC<StringC,StringC> &Attributes()
    { return attribs; }
    //: Get the current attributes.
    
    const RCHashC<StringC,StringC> &Attributes() const
    { return attribs; }
    //: Get the current attributes.
    
    StringC operator[] (const StringC &key) 
    { return attribs[key]; }
    //: Get attribute whose name is "key"

    const StringC operator[] (const StringC &key) const
    { return attribs[key]; }
    //: Get attribute whose name is "key"

    void SetEmptyTag(bool v)
    { emptyTag = v; }
    //: Set empty tag flag.
    
    bool IsEmptyTag() const
    { return emptyTag; }
    //: Is tag empty ?
    
    StringC &Content()
    { return content; }
    //: Access contents of element.
    
    const StringC &Content() const
    { return content; }
    //: Access contents of element.
  protected:
    StringC name;
    RCHashC<StringC,StringC> attribs;
    bool emptyTag;
    StringC content;
  };
  
  //! userlevel=Advanced
  //: Information about the current element being processed.
  
  class XMLElementC 
    : public RCHandleC<XMLElementBodyC>
  {
  public:
    XMLElementC()
    {}
    //: Constructor.

    XMLElementC(const StringC &nm,bool anEmptyTag = false)
      : RCHandleC<XMLElementBodyC>(*new XMLElementBodyC(nm,anEmptyTag))
    {}
    //: Constructor.
    
    XMLElementC(const StringC &nm,const RCHashC<StringC,StringC> &attrs,bool anEmptyTag = false)
      : RCHandleC<XMLElementBodyC>(*new XMLElementBodyC(nm,attrs,anEmptyTag))
    {}
    //: Constructor.
    
    StringC &Name()
    { return Body().Name(); }
    //: Name of context.
    
    const StringC &Name() const
    { return Body().Name(); }
    //: Name of context.
    
    RCHashC<StringC,StringC> &Attributes()
    { return Body().Attributes(); }
    //: Get the current attributes.
    
    const RCHashC<StringC,StringC> &Attributes() const
    { return Body().Attributes(); }
    //: Get the current attributes.
    
    StringC operator[] (const StringC &key) 
    { return Body()[key]; }
    //: Get attribute whose name is "key"

    const StringC operator[] (const StringC &key) const
    { return Body()[key]; }
    //: Get attribute whose name is "key"

    bool IsEmptyTag() const
    { return Body().IsEmptyTag(); }
    //: Is tag empty ?
    
    void SetEmptyTag(bool v)
    { Body().SetEmptyTag(v); }
    //: Set empty tag flag.
    
    StringC &Content()
    { return Body().Content(); }
    //: Access contents of element.
    
    const StringC &Content() const
    { return Body().Content(); }
    //: Access contents of element.
    
  };

  //! userlevel=Develop
  //: Common XML operations.
  // Stuff common to handling XML input and output.

  class XMLBaseBodyC 
    : public RCBodyVC
  {
  public:
    XMLBaseBodyC();
    //: Constructor.
    
    XMLElementC &Context()
    { return context.Top(); }
    //: Access current context.
    
    bool IsContext() const
    { return !context.IsEmpty(); }
    //: Are we in a valid context ? 
   
    const XMLElementC &Context() const
    { return context.Top(); }
    //: Access current context.
    
    bool EndOfContext(const StringC &nm) {
      // Check name here ?
      if(context.IsEmpty())
	return false;
      context.DelTop();
      return true;
    };
    //: End the current context.

    bool EndOfContext() {
      if(context.IsEmpty())
	return false;
      context.DelTop();
      return true;
    };
    //: End the current context.
    
    bool StartContext(const StringC &str,bool emptyTag = false) {
      context.Push(XMLElementC(str,emptyTag));
      SetContent(false);
      return true;
    };
    //: Start a context.
    
    bool StartContext(const StringC &str,const RCHashC<StringC,StringC> &attrs,bool emptyTag = false) {
      context.Push(XMLElementC(str,attrs,emptyTag));
      SetContent(false);
      return true;
    };
    //: Start a context.
    
    bool StartContext(const XMLElementC &elem) {
      context.Push(elem);
      SetContent(false);
      return true;
    };
    //: Start a context.
    
    void SetStrict(bool value)
    { strict = value; }
    //: Set strict parsing.
    
    bool IsStrict() const
    { return strict; }
    //: In strict mode ?
    // if true issue errors if XML consitancy check fails in reading or writing.
    
    bool IsContent() const
    { return contents; }
    //: Got content ?
    // True if content has been found.
    
    void SetContent(bool cont)
    { contents = cont; }
    //: Set got content.
    
    UIntT LevelsNested() const
    { return context.Size(); }
    //: Number of open elements in the stack.
    
    bool GetAttib(const StringC &name,StringC &val) {
      RavlAssert(!context.IsEmpty());
      StringC *sv = context.Top().Attributes().Lookup(name);
      if(sv == 0)
	return false;
      val = *sv;
      return true;
    }
    //: Set string attribute.
    
    void SetAttrib(const StringC &value,const StringC &val) { 
      RavlAssert(!context.IsEmpty());
      context.Top().Attributes()[value] = val; 
    }
    //: Set a string valued attribute.
    
    bool AutoIndent() const
    { return autoIndent; }
    //: Auto indenting activated.
    
    bool AutoIndent(bool val)
    { autoIndent = val; return val; }
    //: Auto indenting activated.
    
    void Push(XMLTagOpsT op,XMLElementC &elm,StringC &name) {
      RavlAssert(!pushed);
      pushed = true;
      pushedOp = op;
      pushedElem = elm;
      pushedName = name;
    }
    //: Push a context.
    
    void Pop(XMLTagOpsT &op,XMLElementC &elm,StringC &name) {
      RavlAssert(pushed);
      op = pushedOp;
      elm = pushedElem;
      name = pushedName;
      pushedElem.Invalidate();
      pushed = false;
    }
    //: Push a context.
    
    void GetPushed(XMLTagOpsT &op,XMLElementC &elm,StringC &name) {
      RavlAssert(pushed);
      op = pushedOp;
      elm = pushedElem;
      name = pushedName;      
    }
    //: Get pushed state.
    
    bool IsPushed() const
    { return pushed; }
    //: Is there a context pushed ?
    
    StringC &Content() { 
      if(context.IsEmpty())
        return globalContents;
      return context.Top().Content(); 
    }
    //: Content.
    
    StringC &LastOpenTag()
    { return lastOpenTag; }
    //: Get name of last tag opened.
    
  protected:
    bool strict; // Be strict about usage.
    StackC<XMLElementC> context;
    bool contents;
    bool autoIndent;
    StringC globalContents;

    // Pushed context info.
    bool pushed;
    XMLTagOpsT pushedOp;
    XMLElementC pushedElem;
    StringC pushedName;
    StringC lastOpenTag;
  };
  
  //! userlevel=Advanced
  //: Common XML operations.
  // Stuff common to handling XML input and output.
  
  class XMLBaseC 
    : public RCHandleC<XMLBaseBodyC>
  {
  public:
    XMLBaseC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    XMLBaseC(bool)
      : RCHandleC<XMLBaseBodyC>(*new XMLBaseBodyC())
    {}
    //: Constructor.
    
    static StringC EncodeLitteral(const StringC &str);
    //: Convert a string into a form that will not confuse XML.
    
    static StringC DecodeLitteral(const StringC &str);
    //: Decode a string from its XML encoding to the original string.
    
    XMLElementC &Context()
    { return Body().Context(); }
    //: Access current context.

    bool IsContext() const
    { return Body().IsContext(); }
    //: Are we in a valid context ? 

    StringC ContextName() const {
      if(!IsContext())
	return StringC();
      return Context().Name();
    }
    //: Get the name of the current context (the last open tag.)
    
    const XMLElementC &Context() const
    { return Body().Context(); }
    //: Access current context.

    bool StartContext(const StringC &str,bool emptyTag = false) 
    { return Body().StartContext(str,emptyTag); }
    //: Start a new element.
    
    bool StartContext(const StringC &str,const RCHashC<StringC,StringC> &attrs,bool emptyTag = false) 
    { return Body().StartContext(str,attrs,emptyTag); }
    //: Start a new element
    
    bool StartContext(const XMLElementC &elem) 
    { return Body().StartContext(elem); }
    //: Start a new element
    
    bool EndOfContext(const StringC &nm) 
    { return Body().EndOfContext(nm); }
    //: End the current context.
    
    bool EndOfContext() 
    { return Body().EndOfContext(); }
    //: End the current context.
    
    bool IsStrict() const
    { return Body().IsStrict(); }
    //: In strict mode ?
    // if true issue errors if XML consitancy check fails in reading or writing.

    void SetStrict(bool value)
    { Body().SetStrict(value); }
    //: Set strict parsing.

    bool IsContent() const
    { return Body().IsContent(); }
    //: Got content ?
    // True if content has been found.
    
    void SetContent(bool cont)
    { Body().SetContent(cont); }
    //: Set got content.

    UIntT LevelsNested() const
    { return Body().LevelsNested(); }
    //: Number of open elements in the stack.
    
    bool GetAttrib(const StringC &name,StringC &val) 
    { return Body().GetAttib(name,val); }
    //: Set string attribute.
    
    void SetAttrib(const StringC &name,const StringC &val) 
    { Body().SetAttrib(name,val); }
    //: Set a string valued attribute.
    
    bool AutoIndent() const
    { return Body().AutoIndent(); }
    //: Auto indenting activated.
    
    bool AutoIndent(bool val)
    { return Body().AutoIndent(val); }
    //: Auto indenting activated.

    StringC &Content()
    { return Body().Content(); }
    //: Content.

    StringC &LastOpenTag()
    { return Body().LastOpenTag(); }
    //: Get name of last tag opened.
    
  protected:
    void Push(XMLTagOpsT op,XMLElementC &elm,StringC &name) 
    { Body().Push(op,elm,name); }
    //: Push a context.
    
    void Pop(XMLTagOpsT &op,XMLElementC &elm,StringC &name) 
    { Body().Pop(op,elm,name); }
    //: Push a context.

    void GetPushed(XMLTagOpsT &op,XMLElementC &elm,StringC &name)
    { Body().GetPushed(op,elm,name); }
    //: Get pushed state.
    
    bool IsPushed() const
    { return Body().IsPushed(); }
    //: Is there a context pushed ?
  };

  //! userlevel=Normal
  //: XML Input Stream.
  // Provides extra functionality for parsing of XML streams.
    
  class XMLIStreamC 
    : public IStreamC,
      public XMLBaseC
  {
  public:
    XMLIStreamC(IStreamC &is);
    //: Construct from an ordinary stream.
    
    explicit XMLIStreamC(const StringC &fn);
    //: Construct from an ordinary stream.
    
    bool ReadHeader();
    //: Check XML header on stream.
    // Check the XML header, if the header is not found the stream
    // routine attempts to return the stream to the state is was in before
    // the call. If the header is found the stream is left on the
    // character after the xml header.
    
    XMLTagOpsT PeekTag(StringC &name,RCHashC<StringC,StringC> &attr);
    //: Have a look at the next tag that will be read.
    // This assumes the next element in the stream is a tag. If not the content
    // will be skipped. See 'ReadTag' for more information.
    
    XMLTagOpsT PeekTag(StringC &name) {
      RCHashC<StringC,StringC> attr;
      return PeekTag(name,attr);
    }
    //: Have a look at the next tag that will be read.
    // This assumes the next element in the stream is a tag. If not the content
    // will be skipped. See 'ReadTag' for more information.
    
    XMLTagOpsT ReadTag(StringC &name,RCHashC<StringC,StringC> &attr);
    //: Read a tag from a stream.
    // returns XMLBeginTag, XMLEndTag or XML_PI. 
    // XML Processing Instructions indicated by XML_PI are treated in the same way as empty tags.
    // This will skip comments and DTD info, and anything else it doesn't understand.
    
    XMLTagOpsT ReadTag(StringC &name) {
      RCHashC<StringC,StringC> attr;
      return ReadTag(name,attr);
    }
    //: Read a tag from a stream.
    // returns XMLBeginTag, XMLEndTag or XML_PI. 
    // XML Processing Instructions indicated by XML_PI are treated in the same way as empty tags.
    // This will skip comments and DTD info, and anything else it doesn't understand.
    
    bool SkipElement();
    //: Skip to after the end of the current element.
    
    XMLTagOpsT SkipToElement(const StringC &elementName,RCHashC<StringC,StringC> &attr);
    //: Skip to named element.
    // This will skip to the next tag of the given name.
    // if the Current context ends it will return XMLEndTag.
    
    StringC ReadID();
    //: Read an ID from the stream.
    // This will skip any initial white space, but will
    // return an empty string if an invalid characters is found.
    
    bool GetAttrib(const StringC &name,StringC &val)
    { return XMLBaseC::GetAttrib(name,val); }
    //: Get a string attribute for entity.
    // This can only be called between StartTag and StartContents.
    // This returns true if attribute is set.
    
    bool GetAttrib(const StringC &name,IntT &val);
    //: Get an integer attribute for entity.
    // This can only be called between StartTag and StartContents.
    // This returns true if attribute is set.
    
    bool GetAttrib(const StringC &name,UIntT &val);
    //: Get an unsigned integer attribute for entity.
    // This can only be called between StartTag and StartContents.
    // This returns true if attribute is set.
    
    bool GetAttrib(const StringC &name,RealT &val);
    //: Get a real valued attribute for entity.
    // This can only be called between StartTag and StartContents.
    // This returns true if attribute is set.
    
    bool GetAttrib(const StringC &name,bool &val);
    //: Get a boolean valued attribute for entity.
    // This can only be called between StartTag and StartContents.
    // This returns true if attribute is set.
    
    template<class DataT>
    bool GetAttrib(const StringC &name,DataT &val) {
      StringC strval;
      if(!XMLBaseC::GetAttrib(name,strval))
	return false;
      StrIStreamC istr(strval);
      istr >> val;
      return true;
    }
    //: Get an abitary valued attribute for entity.
    // This can only be called between StartTag and StartContents.
    // The attribute value is stored using its default streaming 
    // operators. (std::ostream and std::istream)
    
  protected:
    
    StringC ReadAttrib(XMLElementC &elem);
    //: Read attribute and add it to the current context.
    // returns the name of the attribute read.
    
    XMLTagOpsT GetTag(XMLElementC &elem,StringC &name);
    //: Get the next tag from the stream without modifying the Context.
    
  };
  
  //! userlevel=Normal
  //: XML Output Stream.
  // Provides extra functionality for generation of XML streams.
  
  class XMLOStreamC 
    : public OStreamC,
      public XMLBaseC
  {
  public:
    XMLOStreamC(OStreamC &os);
    //: Construct from an ordinary stream.
    
    XMLOStreamC(const StringC &fn)
      : OStreamC(fn),
	XMLBaseC(true)
    {}
    //: Construct from an ordinary stream.
    
    OStreamC &Stream()
    { return *this; }
    //: Stream.
    //Access to the underlying text stream
    
    bool WriteHeader();
    //: Write XML header.

    void Comment(const StringC& comment)
    { Stream() << "\n<!-- " << comment << " -->"; }
    //: Write an XML comment.
    
    bool WritePI(const StringC &name,const RCHashC<StringC,StringC> &attribs);
    //: Write a Processing Instruction to the stream with the given name and attributes.
    
    void StartTag(const StringC &name,
		  const RCHashC<StringC,StringC> &attribs,
		  bool emptyTag = false);
    //: Begin writing a tag with the given attributes.
    // If emptyTag = true, the closing tag is also added.
    
    void StartTag(const StringC &name,
		  bool emptyTag = false);
    //: Begin writing a tag with no attributes.
    // If emptyTag = true, the closing tag is also added.
    
    void StartContents();
    //: Call before writing contents of entity to stream.
    
    void EndTag(const StringC &name);
    //: End writing a tag.
    // if strict checking is enabled, name will be check against that of the open tag.
    
    void EndTag();
    //: End writing current entity.
    
    void Indent(int off = 0);
    //: Indent the following line appropriately.

    template<class DataT>
    void SetAttrib(const StringC &name,const DataT &val) {
      StrOStreamC ostr;
      ostr << val;
      XMLBaseC::SetAttrib(name,ostr.String());
    }
    //: Set an arbitrary valued attribute for entity.
    // This can only be called between StartTag and StartContents.
    // The attribute value is stored using its default streaming 
    // operators. (std::ostream and std::istream)

    void SetAttrib(const StringC &name,const StringC &val)
    { XMLBaseC::SetAttrib(name,val); }
    //: Set an integer attribute for entity.
    // This can only be called between StartTag and StartContents.
    
    void SetAttrib(const StringC &name,const IntT &val);
    //: Set an integer attribute for entity.
    // This can only be called between StartTag and StartContents.
    
    void SetAttrib(const StringC &name,const UIntT &val);
    //: Set an unsigned integer attribute for entity.
    // This can only be called between StartTag and StartContents.

    void SetAttrib(const StringC &name,const char * &val);
    //: Set a 'C' string attribute for entity.
    // This can only be called between StartTag and StartContents.
    
    void SetAttrib(const StringC &name,const RealT &val);
    //: Set a real valued attribute for entity.
    // This can only be called between StartTag and StartContents.

  };
  
  XMLOStreamC &operator<<(XMLOStreamC &strm,const XMLElementC &elem);
  //: Output a element, well start to.
  
  inline
  std::ostream &operator<<(std::ostream &strm,const XMLTagOpsT &elem) { 
    RavlAssertMsg(0,"XMLTagOpsT saved to non-xml stream. \n");
    return strm;
  }
  //: Catch silly mistakes.
  
  XMLOStreamC &operator<<(XMLOStreamC &strm,const StringC &str);
  //: Write tag contents.
  
  XMLOStreamC &operator<<(XMLOStreamC &strm,const char *str);
  //: Write tag contents.
  
  XMLIStreamC &operator>>(XMLIStreamC &strm,StringC &str);
  //: Read tag contents as string.

  template<class DataT>
  XMLOStreamC &operator<<(XMLOStreamC &strm,const DataT &data) {
    StrOStreamC strStrm;
    strStrm << data;
    strm << strStrm.String();
    return strm;
  }
  //: Handle writting of some arbitary data. 
  // Convert to a string and write into content section of the tags.
  
  inline
  XMLOStreamC &operator<<(XMLOStreamC &strm,int val)
  { strm << StringC(val); return strm; }
  //: write integer.
  
  inline
  XMLOStreamC &operator<<(XMLOStreamC &strm,RealT val)
  { strm << StringC(val); return strm; }
  //: write real value
  
  inline
  XMLOStreamC &operator<<(XMLOStreamC &strm,float val)
  { strm << StringC((RealT) val); return strm; }
  //: write float value
  
  XMLOStreamC &operator<<(XMLOStreamC &strm,const XMLTagOpsT &elem);
  //: Do an output stream op.
  
  XMLIStreamC &operator>>(XMLIStreamC &strm,const XMLTagOpsT &elem);
  //: Do an input stream op.
  
  template<class DataT>
  inline
  XMLOStreamC &operator<<(XMLOStreamC &strm,const XMLAttributeC<DataT> &elem) { 
    strm.SetAttrib(elem.Name(),elem.Data());
    return strm;
  }
  //: Write out an attribute.

  template<class DataT>
  inline
  XMLIStreamC &operator>>(XMLIStreamC &strm,const XMLAttributeC<DataT> &elem) { 
    strm.GetAttrib(elem.Name(),const_cast<XMLAttributeC<DataT> &>(elem).Data());
    return strm;
  }
  //: Read in an attribute.
  
  template<class DataT>
  XMLAttributeC<DataT> XMLAttribute(const StringC &nm,const DataT &dat)
  { return XMLAttributeC<DataT>(nm,const_cast<DataT &>(dat)); }
  //: Setup an XML attribute.
  
  inline
  XMLElementC XMLStartTag(const StringC &name)  
  { return XMLElementC(name); }
  //: Start writing a XML start tag.


  template<class DataT>
  XMLOStreamC &operator<<(XMLOStreamC &strm,const SArray1dC<DataT> &arr) {
    strm << XMLStartTag("array")  << XMLAttribute("size",arr.Size());
    for(SArray1dIterC<DataT> it(arr);it;it++) {
      strm << XMLStartTag("data") << XMLContent << *it;
      strm << XMLEndTag;
    }
    strm << XMLEndTag;
    return strm;
  }

  
  template<class DataT>
  XMLIStreamC &operator>>(XMLIStreamC &strm,SArray1dC<DataT> &arr) {
    UIntT size = 0;
    StringC name;
    if(strm.ReadTag(name) != XMLBeginTag)
      throw ExceptionInvalidStreamC("Unexpected tag when reading SArray1dC ");
    strm >> XMLAttribute("size",size);
    SArray1dC<DataT> ret(size);
    for(SArray1dIterC<DataT> it(ret);it;it++) {
      XMLTagOpsT tt = strm.ReadTag(name);
      if(tt == XMLEndTag) {
	//cerr << "Found end:" << name << "\n";
	break; // Found end of array tag.
      }
      if(name != "data") {
	cerr << "WARNING: Read array ignoring tag '" << name << "'\n";
	strm.SkipElement();
	continue;
      }
      strm >> *it;
      //cerr << "Got data " << *it << "\n";
      strm >> XMLEndTag;
    }
    //cerr << "Read of " << size << " items complete.\n";
    arr = ret;
    return strm;
  }


  template<class KeyT,class DataT>
  XMLOStreamC &operator<<(XMLOStreamC &strm,const HashC<KeyT,DataT> &elem) {
    strm << XMLStartTag("hashmap") 
	 << XMLAttribute("class",TypeName(typeid(HashC<KeyT,DataT>)));
    
    for(HashIterC<KeyT,DataT> it(elem);it;it++) {
      strm << XMLStartTag("map");
      strm << XMLStartTag("key") << XMLContent << it.Key();
      strm << XMLEndTag;
      strm << XMLStartTag("data")  << XMLContent << it.Data();
      strm << XMLEndTag;
      strm << XMLEndTag;
    }
    strm << XMLEndTag;
    return strm;
  } 
  //: Write out a hash table.
  // This also serves as an example.

/*
  template<class KeyT,class DataT>
  XMLIStreamC &operator>>(XMLIStreamC &strm,HashC<KeyT,DataT> &elem) {
    // Assume we've got the open tag.
    for(;;) {
      
    }
    return strm;
  } 
  //: Read in a hash table.
  // This also serves as an example.
*/  

}

#endif
