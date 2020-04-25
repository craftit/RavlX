// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/XMLStream.cc"

#include "Ravl/XMLStream.hh"
#include "Ravl/HashIter.hh"
#include <stdlib.h>
#include <ctype.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {


  // Bit 0 - NameStartChar
  // Bit 1 - NameChar
  
  static UByteT  xmlCharacters[256] = 
  /*             0     1     2     3     4     5     6     7     8     9   A 10  B 11  C 12  D 13  E 14  F 15 */
  { /*0   0 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*1  16 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*2  32 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00,
    /*3  48 */ 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*4  64 */ 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    /*5  80 */ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x03,
    /*6  96 */ 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    /*7 112 */ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*8 128 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*9 144 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*A 160 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*B 176 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /*C 192 */ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    /*D 208 */ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    /*E 224 */ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    /*F 240 */ 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  
  static inline bool IsXMLIDChar(ByteT c) {
    return (xmlCharacters[c] & 0x02) != 0;
  }

  static inline bool IsXMLIDStartChar(ByteT c) {
    return (xmlCharacters[c] & 0x01) != 0;
  }
  
  ///// XMLBaseBodyC ////////////////////////////////////////////////
  
  //: Constructor.
  
  XMLBaseBodyC::XMLBaseBodyC()
    : strict(false),
      contents(false),
      autoIndent(true),
      pushed(false)
  {}
  
  ///// XMLBaseC ////////////////////////////////////////////////
  
  //: Convert a string into a form that will not confuse XML.
  
  StringC XMLBaseC::EncodeLitteral(const StringC &str) {
    StringC ret(str.Copy());
    // FIXME:- Encode efficiently.
    ret.gsub("&","&amp;");    // Has to go first!!
    ret.gsub("\"","&quot;");  // Needed for quoted text
    //ret.gsub("#","&num;");   
    //ret.gsub("$","&dollar;");
    ret.gsub("%","&percnt;"); // Needed for quoted text
    ret.gsub("\'","&apos;");  // Needed for quoted text
    ret.gsub("!","&excl;");
    ret.gsub("<","&lt;");     // Needed.
    ret.gsub(">","&gt;");
    //ret.gsub(" ","&#32;");
    return ret;
  }
  
  //: Decode a string from its XML encoding to the original string.
  
  StringC XMLBaseC::DecodeLitteral(const StringC &str) {
    StringC ret(str.Copy());
    // FIXME:- Decode efficently
    ret.gsub("&amp;","&");    // Has to go first!!
    ret.gsub("&quot;","\"");  // Needed for quoted text
    ret.gsub("&percnt;","%"); // Needed for quoted text
    ret.gsub("&apos;","\'");  // Needed for quoted text
    ret.gsub("&excl;","!");
    ret.gsub("&lt;","<");     // Needed.
    ret.gsub("&gt;",">");
    ret.gsub("&#10;","\n");
    ret.gsub("&#13;","\r");
    return ret;
  }

  //// XMLIStreamC //////////////////////////////////////////////////////////

  XMLIStreamC::XMLIStreamC(const StringC &fn)
   : IStreamC(fn),
     XMLBaseC(true)
  {}

  //: Construct from an ordinary stream.
  
  XMLIStreamC::XMLIStreamC(IStreamC &is)
    : IStreamC(is)
  {
    XMLIStreamC *ptr;
    if((ptr = dynamic_cast<XMLIStreamC *>(&is)) != 0) 
      ((XMLBaseC &)*this) = *ptr;
    else
      ((XMLBaseC &)*this) = XMLBaseC(true);
  }
  
  bool XMLIStreamC::ReadHeader() {
    // Check first two no
    streampos mark = Tell();
    char c = GetChar();
    if(c != '<') {
      is().putback(c);
      return false;
    }
    c = GetChar();
    if(c != '?') {
      is().putback('<');
      is().putback(c);
      return false;
    }
    StringC name = ReadID();
    if(name != "xml") {
      Seek(mark);
      return false;
    }
    XMLElementC elem(name);
    bool done = false;
    while(!done) {
      char c = SkipWhiteSpace();
      switch(c) {
      case '?':
	done = true;
	break;
      case '>': // Illegal charactor.
	Seek(mark);      
	return false;
      default:
	is().putback(c);
	ReadAttrib(elem);
	break;
      }
    }
    c = GetChar();
    if(c != '>') {
      Seek(mark);      
      return false;
    }
    return true;
  }
  
  StringC XMLIStreamC::ReadID() {
    StringC ret;
    char c = SkipWhiteSpace();
    if(IsXMLIDStartChar(c)) {
      ret += c;
      for(c = GetChar();IsXMLIDChar(c);c = GetChar())
        ret += c;
    } else {
      if(IsXMLIDChar(c)) {
        std::cerr << "WARNING: Found illegal start charactor " << (unsigned) c << " in name. Filename:" << Name() << " \n";
      }
    }

    is().putback(c);
    return ret;
  }

  //: Get the next tag from the stream without modifying the Context.
  
  XMLTagOpsT XMLIStreamC::GetTag(XMLElementC &elem,StringC &name) {
    char c;
    bool gotTag = false;
    bool endOfTag = false;
    bool emptyTag = false;
    bool foundEndTag = false;
    bool isPI = false;
    StringC id;
    try {
      while(*this && !endOfTag) {
	// Search for start of tag.
	gotTag = false;
	foundEndTag = false;
	c = GetChar();
	if(c != '<') {
	  Content() += c;
	  continue; // Skip non-tag characters.
	}
	gotTag = true;
	// Work out what sort of tag we've got.
	c = GetChar();
	switch(c) 
	  {
	  case '?': // Processing instruction.
	    isPI = true;
	    emptyTag = true;
	    ONDEBUG(std::cerr << "XMLIStreamC::ReadTag(), Found processing instruction. \n");
	    break;
	  case '!':  // Comment CDATA or DTD
	    {
              // CDATA ?
              if(this->SkipWord("[CDATA[")) {
                ONDEBUG(std::cerr << "XMLIStreamC::ReadTag(), Found CDATA \n");
                Content() += this->ClipTo("]]>");
                continue;
              }
	      if(this->SkipWord("--")) {
		ONDEBUG(std::cerr << "XMLIStreamC::ReadTag(), Found comment. \n");
		SkipTo("-->");
		continue; // Start searching for a tag again.
	      }
	      // This may have to be more clever.
	      ONDEBUG(std::cerr << "XMLIStreamC::ReadTag(), Found DTD info. \n");
	      SkipTo('>');
	      // Start searching for a tag again.
	      continue;
	    }
	    break;
	  case '>':
	    std::cerr << "WARNING: Found an empty tag (<>) in XML stream. Filename:" << Name() << "\n";
	    continue; // Empty <> probably illegal ??
	  case '/':
	    foundEndTag = true;
	    emptyTag = true;
	    break;
	  default:
	    is().putback(c);
	    break;
	  }
	
	id = ReadID();
	
	if(!emptyTag || isPI) { // Is an end tag, don't push a new context.
	  ONDEBUG(std::cerr << "XMLIStreamC::ReadTag(), Found tag start '" << id << "'\n");
	  elem = XMLElementC(id);
	  //StartContext(id);
	} else {
	  ONDEBUG(std::cerr << "XMLIStreamC::ReadTag(), Found tag end '" << id << "'\n");
	}
	
	// Check for attributes.
	while(!endOfTag) {
	  c = SkipWhiteSpace();
	  switch(c) 
	    {
	    case '?':
	      if(!isPI) { // Are we in a processing instruction ?
		std::cerr << "ERROR: Unexpected character '"  << c << "' in XML tag after id '" << id << "'. Filename:" << Name() << " \n";
		throw ExceptionInvalidStreamC("Unexpected end of XML tag. ");
	      }
	      /* no break */
	    case '/':
	      c = GetChar();
	      if(c != '>') {
		std::cerr << "ERROR: Unexpected character '"  << c << "' in XML tag after id '" << id << "' Filename:" << Name() << "\n";
		throw ExceptionInvalidStreamC("Unexpected end of XML tag. ");
	      }
	      emptyTag = true;
              /* no break */
	    case '>':
	      endOfTag = true;
	      continue;
	    default: // An attribute.
	      is().putback(c);
	      if(!elem.IsValid()) {
		cerr << "ERROR: Unexpected attribute. Filename:" << Name() << "\n";
		throw ExceptionInvalidStreamC("Unexpected attribute. ");
	      }
	      ReadAttrib(elem);
	      break;
	    }
	}
      }
    } catch(ExceptionEndOfStreamC &) {
      if(gotTag)
	throw ExceptionInvalidStreamC("Unexpected end of XML stream. ");
    }
    if(emptyTag) {
      name = id;
      if(isPI)
	return XML_PI;
      if(!foundEndTag)
	return XMLEmptyTag;
      return XMLEndTag;
    }
    if(!gotTag)
      return XMLContent;
    name = id;
    if(foundEndTag)
      return XMLEndTag;
    return XMLBeginTag;
  }
  
  //: Read a tag from a stream.
  // returns true if one is found or false if end of group found.
  
  XMLTagOpsT XMLIStreamC::ReadTag(StringC &name,RCHashC<StringC,StringC> &attr) {
    XMLTagOpsT ret;
    XMLElementC elem;    
    if(IsPushed())
      Pop(ret,elem,name);
    else
      ret = GetTag(elem,name);
    if(elem.IsValid())
      attr = elem.Attributes();
    if(ret == XMLEmptyTag) {
      // Push a fake end tag.
      XMLElementC elem;
      XMLTagOpsT pe = XMLEndTag;
      Push(pe ,elem,name);
      ret = XMLBeginTag;
    }
    if(ret == XMLBeginTag) 
      StartContext(elem);
    return ret;
  }

  XMLTagOpsT XMLIStreamC::PeekTag(StringC &name,RCHashC<StringC,StringC> &attr) {
    XMLTagOpsT ret;
    XMLElementC elem;
    if(IsPushed()) 
      GetPushed(ret,elem,name);
    else {
      ret = GetTag(elem,name);
      Push(ret,elem,name);
    }
    if(elem.IsValid())
      attr = elem.Attributes();
    if(ret == XMLEmptyTag) 
      ret = XMLBeginTag;
    return ret;
  }
  
  //: Skip to after the end of the current element.
  
  bool XMLIStreamC::SkipElement() {
    StringC curCtxt = Context().Name();
    StringC name;
    int level = 0;
    while(*this) {
      XMLTagOpsT tt = ReadTag(name); 
      if(tt == XMLEndTag) {
	// Found end tag.
	if(level == 0)
	  break;
	level--;
	continue;
      }
      if(tt == XMLBeginTag) {     // Found open tag.
	level++;
	continue;
      }
    }
    if(name != curCtxt) {
      std::cerr << "WARNING: End tag name mismatch, got '" << name << "' expected '" << curCtxt << "'\n";
      return false;
    }
    return true;
  }

  //: Skip to named element.
  // This will skip to the next tag of the given name.
  // if the Current context ends it will return XMLEndTag.

  XMLTagOpsT XMLIStreamC::SkipToElement(const StringC &elementName,RCHashC<StringC,StringC> &attr) {
    StringC name;
    RCHashC<StringC,StringC> tattr;
    while(*this) {
      XMLTagOpsT tt = ReadTag(name,tattr);
      if(name == elementName) {
	attr = tattr;
	return tt;
      } 
      if(tt == XMLBeginTag)
	SkipElement();
      if(tt == XMLEndTag)
	break;
    }
    return XMLEndTag;
  }

  //: Read attribute and add it to the current context.
  
  StringC XMLIStreamC::ReadAttrib(XMLElementC &elem) {
    StringC id = ReadID();
    char c = SkipWhiteSpace();
    if(c != '=') {
      std::cerr << "ERROR: Unexpected character '"  << c << "' in XML attribute. Filename:" << Name() << " \n";
      throw ExceptionInvalidStreamC("Unexpected character in XML attribute. ");
    }
    c = SkipWhiteSpace();
    StringC val;
    switch(c) {
    case '\"':
      val = ClipTo('\"');
      break;
    case '\'':
      val = ClipTo('\'');
      break;
    default:// Unescaped string ??
      is().putback(c);
      val += ReadID();
      break;
    }
    elem.Attributes()[id] = val;
    ONDEBUG(std::cerr << "XMLOStreamC::ReadAttrib(), Got '" << id << "' = '" << val << "' \n");
    return id;
  }
  
  //: Get an integer attribute for entity.
  // This can only be called between StartTag and StartContents.
  // This returns true if attribute is set.
  
  bool XMLIStreamC::GetAttrib(const StringC &name,IntT &val) {
    StringC tv;
    if(!XMLBaseC::GetAttrib(name,tv))
      return false;
    val = atoi(tv);
    return true;
  }
  
  //: Get an unsigned integer attribute for entity.
  // This can only be called between StartTag and StartContents.
  // This returns true if attribute is set.
  
  bool XMLIStreamC::GetAttrib(const StringC &name,UIntT &val) {
    StringC tv;
    if(!XMLBaseC::GetAttrib(name,tv))
      return false;
    StrIStreamC(tv) >> val;
    return true;
  }
  
  //: Get a real valued attribute for entity.
  // This can only be called between StartTag and StartContents.
  // This returns true if attribute is set.
  
  bool XMLIStreamC::GetAttrib(const StringC &name,RealT &val) {
    StringC tv;
    if(!XMLBaseC::GetAttrib(name,tv))
      return false;
    val = atof(tv);
    return true;
  }

  //: Get a boolean valued attribute for entity.
  // This can only be called between StartTag and StartContents.
  // This returns true if attribute is set.
  
  bool XMLIStreamC::GetAttrib(const StringC &name,bool &val) {
    StringC tv;
    if(!XMLBaseC::GetAttrib(name,tv))
      return false;
    val = (atoi(tv) != 0);
    return true;    
  }

  
  //// XMLOStreamC //////////////////////////////////////////////////////////

  //: Construct from an ordinary stream.
  
  XMLOStreamC::XMLOStreamC(OStreamC &os)
    : OStreamC(os)
  {
    XMLOStreamC *ptr;
    if((ptr = dynamic_cast<XMLOStreamC *>(&os)) != 0) 
      ((XMLBaseC &)*this) = *ptr;
    else
      ((XMLBaseC &)*this) = XMLBaseC(true);
  }
  
  //: Write XML header.
  
  bool XMLOStreamC::WriteHeader() {
    Stream() << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
    return true;
  }

  //: Write a Processing Instruction to the stream with the given name and attributes.
  
  bool XMLOStreamC::WritePI(const StringC &name,const RCHashC<StringC,StringC> &attribs) {
    Stream() << "<?" << name << ' ';
    for(HashIterC<StringC,StringC> it(attribs);it;it++)
      Stream() << ' ' << it.Key() << "=\"" << it.Data() << "\"";
    Stream() << " ?>";
    return true;
  }
  
  //: Begin writing a tag with the given attributes.
  
  void XMLOStreamC::StartTag(const StringC &name,const RCHashC<StringC,StringC> &attribs,bool emptyTag) {
    if(IsContext() && !IsContent())
      StartContents(); // Make sure we're in a content section.
    LastOpenTag() = name;
    StartContext(name,attribs,emptyTag);
  }
  
  //: Begin writing a tag with the given attributes.
  
  void XMLOStreamC::StartTag(const StringC &name,bool emptyTag) {
    RCHashC<StringC,StringC> none;
    //  LastOpenTag() = name;
    StartTag(name,none,emptyTag);
  }
  
  //: Call before writing contents of entity to stream.
  
  void XMLOStreamC::StartContents() {
    //ONDEBUG(std::cerr << "XMLOStreamC::StartContents(), Name:'" << Context().Name() << "' \n");
    if(!IsContext()) {
      std::cerr << "XMLOStreamC::StartContents(), ERROR: No tags around content. Filename:" << Name() << "\n";
      return ;
    }
    if(IsContent()) {
      std::cerr << "XMLOStreamC::StartContents(), ERROR: Contents already started in '" << Context().Name() << "' \n";
      return;
    }
    if(AutoIndent())
      Indent(-1); // -1 because we're already in the tag.
    Stream() << '<' << Context().Name();
    for(HashIterC<StringC,StringC> it((RavlN::HashC<RavlN::StringC,RavlN::StringC> &)Context().Attributes());it;it++)
      Stream() << ' ' << it.Key() << "=\"" << it.Data() << "\"";
    if(Context().IsEmptyTag()) 
      Stream() << " />";
    else
      Stream() << " >";
    SetContent(true);
  }
  
  //: End writing a tag.
  // if strict checking is enabled, name will be check against that of the open tag.
  
  void XMLOStreamC::EndTag(const StringC &name) {
    //ONDEBUG(std::cerr << "XMLOStreamC::EndTag(StringC) : '" << name << "'\n");
    if(!IsContent()) {
      Context().SetEmptyTag(true);
      StartContents();
    }
    if(!Context().IsEmptyTag() && LastOpenTag() != Context().Name()) {
      if(AutoIndent())
	Indent(-1);
      Stream() << "</" << name << ">";
    }
    if(IsStrict()) {
      if(!EndOfContext(name)) {
	ONDEBUG(std::cerr << "XMLOStreamC::EndTag()");
	cerr << "ERROR: Not end of named entity '" << name << "' Currently its '" << Context().Name() << "'\n";
        throw RavlN::ExceptionInvalidStreamC("Mismatched tags. ");
      }
    } else
      EndOfContext();
    LastOpenTag() = StringC();
  }
  
  //: End writing current entity.
  
  void XMLOStreamC::EndTag() {
    //ONDEBUG(std::cerr << "XMLOStreamC::EndTag() : '" << Context().Name() << "'\n");
    if(!IsContent()) {
      Context().SetEmptyTag(true);      
      StartContents();
    }
    if(!Context().IsEmptyTag()) {
      if(AutoIndent() && LastOpenTag() != Context().Name())
	Indent(-1);
      Stream() << "</" << Context().Name() << ">";
    }
    EndOfContext();
    LastOpenTag() = StringC();
  }

  //: Indent the following line appropriately.
  
  void XMLOStreamC::Indent(int off) {
    Stream() << '\n';
    IntT lvl = LevelsNested() + off;
    if(lvl < 0) {
      std::cerr << "XMLOStreamC::Indent(), Warning: Negative tab. \n";
    }
    // Could do something with tabs to make files smaller ?
    for(IntT i = 0;i < lvl;i++) 
      Stream() << ' ';
  }

  
  //: Set attribute for entity.
  // This can only be called between StartTag and StartContents.
  
  void XMLOStreamC::SetAttrib(const StringC &name,const IntT &val) {
    SetAttrib(name,StringC(val));
  }

  //: Set attribute for entity.
  // This can only be called between StartTag and StartContents.
  
  void XMLOStreamC::SetAttrib(const StringC &name,const UIntT &val) {
    SetAttrib(name,StringC(val));
  }
  
  //: Set attribute for entity.
  // This can only be called between StartTag and StartContents.
  
  void XMLOStreamC::SetAttrib(const StringC &name,const char * &val) {
    SetAttrib(name,StringC(val));
  }
  
  //: Set attribute for entity.
  // This can only be called between StartTag and StartContents.
  
  void XMLOStreamC::SetAttrib(const StringC &name,const RealT &val) {
    SetAttrib(name,StringC(val));
  }
  
  //// Globals //////////////////////////////////////////////////////////
  
  //: Do an output stream op.
  XMLOStreamC &operator<<(XMLOStreamC &strm,const XMLTagOpsT &elem) { 
    //cerr << "operator<<(XMLOStreamC &strm,const XMLTagOpsT &elem), Called with :" << ((int) elem) << "\n";
    switch(elem) 
      {
      case XMLEndTag:
	strm.EndTag();
	break;
      case XMLEmptyTag:
	strm.Context().SetEmptyTag(true);
	strm.StartContents();
	strm.EndOfContext();
	//strm.EndTag();
	break;
      case XMLContent:
	strm.StartContents(); 
	break;
      case XMLIndent: strm.Indent(); break;
      case XMLIndentDown: strm.Indent(-1); break;
	
      case XMLBeginTag:
      case XML_PI:
      case XMLComment:
	RavlAssertMsg(0,"XMLOStreamC & operator<<(XMLTagOpsT) Illegal tag op. ");
	break;
	
      }
    return strm;
  }

  //: Do an input stream op.
  XMLIStreamC &operator>>(XMLIStreamC &strm,const XMLTagOpsT &elem) {
    switch(elem) 
      {
      case XMLEndTag:
	{
	  StringC name;
	  for(;;) {
	    if(strm.ReadTag(name) == XMLEndTag)
	      break;
	    std::cerr << "Unexpected start tag '" << name << "' found, skipping. \n";
	    strm.SkipElement();
	  }
	}
	break;
	
      case XMLContent:
      case XMLIndent: 
      case XMLIndentDown: 
	break; // Ignore.
	
      case XML_PI:
      case XMLBeginTag:
      case XMLEmptyTag:
      case XMLComment: // Issue error.
	RavlAssertMsg(0,"XMLIStreamC & operator>>(XMLTagOpsT) Illegal tag op. ");
	break;
      }
    return strm;
  }

  XMLOStreamC &operator<<(XMLOStreamC &strm,const XMLElementC &elem) { 
    if(strm.IsContext() && !strm.IsContent())
      strm.StartContents(); // Make sure we're in a content section.
    strm.StartContext(elem);
    return strm;
  }
  //: Output a element, well start to.
  
  //: Write tag contents.
  
  XMLOStreamC &operator<<(XMLOStreamC &strm,const StringC &str) {   
    if(!strm.IsContent())
      strm << XMLContent; // End tag.
    ((OStreamC &) strm) << XMLBaseC::EncodeLitteral(str);
    return strm;
  }

  //: Write tag contents.
  
  XMLOStreamC &operator<<(XMLOStreamC &strm,const char *str) {
    if(!strm.IsContent())
      strm << XMLContent; // End tag.
    ((OStreamC &) strm) << XMLBaseC::EncodeLitteral(StringC(str));
    return strm;
  }
  
  //: Read tag contents as string.
  
  XMLIStreamC &operator>>(XMLIStreamC &strm,StringC &str) {
    StringC name;
    RCHashC<StringC,StringC> attr;
    strm.PeekTag(name,attr); // Make sure content is read.
    str = XMLBaseC::DecodeLitteral(strm.Content());
    strm.Content() = StringC(); // Reset string.
    return strm;
  }
  
}
