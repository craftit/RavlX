// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/System/XMLTree.cc"

#include "Ravl/XMLTree.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/HSet.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Resource.hh"
#include "Ravl/SysLog.hh"
#include <stdlib.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x) 
#endif

namespace RavlN {

  static StringC FilePathComponent(const StringC &filename) {
    if(filename.IsEmpty())
      return filename;
#ifdef VISUAL_CPP
    // paths can contain / and \ which confuse this routine, so lets sort that out first!
    StringC str = filename.Copy();
    str.gsub("/", "\\");
    str.gsub("\\\\", "\\");
    return str.before("\\",-1);
#else
    return const_cast<StringC &>(filename).before("/",-1);
#endif
  }

  XMLTreeLoadC::~XMLTreeLoadC()
  {}

  //! Create a new node in the tree.

  bool XMLTreeLoadC::NewNode(const StringC &name,
                        const RCHashC<StringC,StringC> &attr,
                        bool isPI,
                        XMLTreeC &node)
  {
    node = XMLTreeC(name,attr,isPI);
    return true;
  }


  //! Open file for reading.
  bool XMLTreeLoadC::OpenFile(const StringC &theFilename,
                              const StringC &parentFilename,
                              IStreamC &newIStream)
  {
    ONDEBUG(std::cerr << "XMLTreeLoadC::OpenFile '" << theFilename << "' from '" << parentFilename << "'\n");
    if(theFilename.IsEmpty())
      return false;
    StringC filename(theFilename);

    // Look in the directory of the current file first, Unless we've been given an absolute path.
    if(filename.firstchar() == '~') {
      StringC homeDir(getenv("HOME"));
      if(!homeDir.IsEmpty()) {
        StringC newFn = homeDir + filename.after('~');
        ONDEBUG(std::cerr << "Trying '" << newFn << "' from '" << homeDir << "'\n");
        newIStream = IStreamC(newFn);
        if(newIStream.good()) {
          ONDEBUG(std::cerr << "Found! \n");
          return true;
        }
      }
    }

    // Try in the same directory as the parent
    if(filename.firstchar() != '/') {
      StringC parentDir = FilePathComponent(parentFilename);
      if(!parentDir.IsEmpty()) {
        StringC newFn = parentDir + '/' + filename;
        ONDEBUG(std::cerr << "Trying '" << newFn << "' from '" << parentDir << "'\n");
        newIStream = IStreamC(newFn);
        if(newIStream.good()) {
          ONDEBUG(std::cerr << "Found! \n");
          return true;
        }
      }

      // Try from the resource root.
      
      StringC resourceDir = ResourceRoot();
      if(!resourceDir.IsEmpty()) {
        StringC newFn = resourceDir + '/' + filename;
        ONDEBUG(std::cerr << "Trying '" << newFn << "' from '" << resourceDir << "'\n");
        newIStream = IStreamC(newFn);
        if(newIStream.good()) {
          ONDEBUG(std::cerr << "Found! \n");
          return true;
        }
      }
    }

    // Try opening from the current directory.
    ONDEBUG(std::cerr << "Trying '" << filename << "'\n");
    newIStream = IStreamC(filename);
    
    if(newIStream.good()) {
      ONDEBUG(std::cerr << "Found! \n");
      return true;
    }
    //RavlDebug("Failed to find xml file '%s' ",theFilename.c_str());
    ONDEBUG(std::cerr << "Not found! \n");
    return false;
  }

  // --------------------------------------------------------------------


  static StringC xmlContentKey("?content");
  static StringC xmlContentAttrib(".");
  //: Construct from an XMLStream.

  XMLTreeBodyC::XMLTreeBodyC()
  : HashTreeBodyC<StringC, RCHashC<StringC, StringC> >(),
    isPI(false)
  { }
  //: Default constructor.

  XMLTreeBodyC::XMLTreeBodyC(const StringC &nname, const RCHashC<StringC, StringC> &attrs, bool _isPI)
  : HashTreeBodyC<StringC, RCHashC<StringC, StringC> >(attrs),
    isPI(_isPI),
    name(nname)
  { }
  //: Construct from a name and an attribute table.

  XMLTreeBodyC::XMLTreeBodyC(const StringC &nname)
  : HashTreeBodyC<StringC, RCHashC<StringC, StringC> >(),
    isPI(false),
    name(nname)
  { }
  //: Construct from a name and an attribute table.

  XMLTreeBodyC::XMLTreeBodyC(BinIStreamC &strm)
  : HashTreeBodyC<StringC, RCHashC<StringC, StringC> >(strm),
    isPI(false)
  { strm >> isPI >> name >> children; }
  //: Binary stream constructor.

  XMLTreeBodyC::XMLTreeBodyC(std::istream &strm)
  : HashTreeBodyC<StringC, RCHashC<StringC, StringC> >(strm),
    isPI(false)
  { strm >> isPI >> name >> children; }
  //: Text stream constructor.

  bool XMLTreeBodyC::Save(BinOStreamC &strm) const
  {
    if (!HashTreeBodyC<StringC, RCHashC<StringC, StringC> >::Save(strm))
      return false;
    strm << isPI << name << children;
    return true;
  }
  //: Save to binary stream.

  bool XMLTreeBodyC::Save(std::ostream &strm) const
  {
    if (!HashTreeBodyC<StringC, RCHashC<StringC, StringC> >::Save(strm))
      return false;
    strm << ' ' << isPI << ' ' << name << ' ' << children;
    return true;
  }
  //: Save to text stream.

  XMLTreeBodyC::XMLTreeBodyC(XMLIStreamC &in,XMLTreeLoadC *loader)
  : isPI(false)
  { Read(in,loader); }

  //: Read from a file

  bool XMLTreeBodyC::ReadFile(const StringC &filename,XMLTreeLoadC *loader)
  {
    if(loader == 0) {
      static XMLTreeLoadC defaultLoader;
      loader = &defaultLoader;
    }
    IStreamC strm;
    if(!loader->OpenFile(filename,"",strm)) {
      ONDEBUG(std::cerr << "Failed to open input file " << filename << " \n");
      return false;
    }
    return Read(strm,loader);
  }


  //: Read from a stream

  bool XMLTreeBodyC::Read(IStreamC &in,XMLTreeLoadC *loader)
  {
    XMLIStreamC ins(in);
    //ins.is().clear(std::ios_base::badbit);
    ONDEBUG(std::cerr << "XML Bad:" << (int) (!ins) << "\n");
    return Read(ins,loader);
  }

  //: Read from an XML stream using this node as the root.
  
  bool XMLTreeBodyC::Read(XMLIStreamC &in,XMLTreeLoadC *loader) {
    HSetC<StringC> includedFiles;
    in.SetStrict(true);
    includedFiles += in.Name();
    return Read(in,includedFiles,loader);
  }
  
  //: Read from an XML stream using this node as the root.
  
  bool XMLTreeBodyC::Read(XMLIStreamC &in,
                          HSetC<StringC> &includedFiles,
                          XMLTreeLoadC *loader)
  {
    if(!in) {
      ONDEBUG(std::cerr << "Bad input stream. \n");
      return false;
    }
    if(loader == 0) {
      static XMLTreeLoadC defaultLoader;
      loader = &defaultLoader;
    }
#if 0
    XMLTagOpsT thisTag = in.ReadTag(name);
    if(thisTag == XML_PI)
      isPI = true;
#endif
    while(in) {
      StringC name;
      RCHashC<StringC,StringC> attr;
      
      StringC content;
      in >> content;
      // FIXME:- Horrible hack!
      // Only store non-whitespace content.
      StringC actualContent = content.TopAndTail();
      
      if(!actualContent.IsEmpty()) {
	XMLTreeC cn(xmlContentKey);
	cn.Data()[xmlContentAttrib] = content;
	Add(xmlContentKey,cn);
      }
      
      XMLTagOpsT tt = in.ReadTag(name,attr);
      
      if(tt == XMLContent) {
	ONDEBUG(std::cerr << "Found tag '" << name << "' XMLContent \n");
	continue;
      }
      if(tt == XMLEndTag ) {
        if(Name() != name) {
          std::cerr << "Tag mismatch in XML, opened with '" << Name() << "' and closed with '" << name << "' in '" << in.Name() << "'\n";
          throw RavlN::ExceptionBadConfigC("Mismatched tags in XML file. ");
          return false;
        }
	ONDEBUG(std::cerr << "Found end tag '" << name << "' \n");
	break;
      }
      // Process includes using XInclude syntax.
      XMLTreeC subtree(name,attr,tt == XML_PI);
      if(tt == XMLBeginTag) {
	ONDEBUG(std::cerr << "Found begin tag '" << name << "' \n");
	if(!subtree.Read(in,includedFiles,loader)) {
          ONDEBUG(std::cerr << "Failed to read sub node. \n");
	  return false;
        }
      }
      
      if(name == "xi:include") {
	if(!ProcessInclude(subtree,includedFiles,in.Name(),loader)) {
          ONDEBUG(std::cerr << "Failed to process include. \n");
	  return false;
        }
	continue;
      }
      
      Add(subtree.Name(),subtree);
    }
    
    return true;
  }

  

  //: Read a node from an XML stream.
  bool XMLTreeBodyC::ReadNode(XMLIStreamC &in,XMLTreeLoadC *loader)
  {
    StringC s;
    in.ReadTag(s);
    RavlAssertMsg(s == name, "XMLTreeBodyC::ReadNode(): tag name of node does not match XMLTreeBodyC name");
    return this->Read(in, loader);
  }



  //: Read a node from an XML stream.
  XMLTreeC XMLTreeC::ReadNode(XMLIStreamC &in,const StringC &name,XMLTreeLoadC *loader)
  {
    StringC s;
    in.ReadTag(s);
    XMLTreeC xml(name.IsEmpty() ? s : name);
    xml.Read(in, loader);
    return xml;
  }


  //: Process xi:xinclude directive.
  
  // TODO:
  //  Support xpointer.
  
  bool XMLTreeBodyC::ProcessInclude(XMLTreeC &subtree,
                                    HSetC<StringC> &doneFiles,
                                    const StringC &parentFilename,
                                    XMLTreeLoadC *loader)
  {
    StringC xi_href;
    
    if(!subtree.Data().Lookup("href",xi_href) || xi_href.IsEmpty()) {
      // Include nothing, this is used where the fallback is to include nothing.
      ONDEBUG(std::cerr << "Including fallback contents. \n");
      for(DLIterC<XMLTreeC> it(subtree.Children());it;it++) {
        if(!it->IsPI())
          Add(it->Name(),*it);
      }
      subtree.Invalidate();
      return true;
    }
    
    // Check for recursive includes.
    if(doneFiles[xi_href]) {	
      RavlWarning(StringC("Recursive include of file '" + xi_href +"'"));
      return false;
    }
    doneFiles += xi_href;
    
    // Check if we're being asked to parse the file as text.
    StringC xi_parse;
    if(subtree.Data().Lookup("parse",xi_parse)) {
      xi_parse = downcase(xi_parse);
      if(xi_parse == "text") {
	// Load file as simple text.
	StrOStreamC strOut;
	IStreamC inFile;
        if(!loader->OpenFile(xi_href,parentFilename,inFile)) {
	  if(!ProcessIncludeFallback(subtree,doneFiles,parentFilename,loader)) {
	    RavlWarning(StringC("Failed to open file '" + xi_href +"'"));
	    return false;
	  }
	  return true;
	}
	inFile.CopyTo(strOut);
	AddContent(strOut.String());
	subtree.Invalidate();
	return true;
      }
      
      if(xi_parse!="xml") {
	RavlIssueWarning(StringC("Unexpected value for parse '" + xi_parse +"'"));
	return false;
      }
    }
    
    // Load the file as XML.

    // Look in the directory of the current file first, Unless we've been given an absolute path.
    IStreamC newIStream;
    if(!loader->OpenFile(xi_href,parentFilename,newIStream)) {
      ONDEBUG(std::cerr << "Load of include file failed, falling back. ");
      
      if(!ProcessIncludeFallback(subtree,doneFiles,parentFilename,loader)) {
	RavlWarning(StringC("Failed to open file '" + xi_href +"' from '" + parentFilename + "' "));
        doneFiles -= xi_href;
	return false;
      }
      doneFiles -= xi_href;
      return true;
    }
    XMLIStreamC newStream(newIStream);
    XMLTreeC newTree(true);
    if(!newTree.Read(newStream,doneFiles,loader)) {
      RavlWarning(StringC("Failed to open file '" + newStream.Name() +"'. "));
      // Assume error has already been reported.
      return false;
    }
    
    // There can't be a recursive include anymore.
    doneFiles -= xi_href;
    
    // Basic xpointer support.
    StringC xi_xpointer;
    if(subtree.Data().Lookup("xpointer",xi_xpointer)) {
      DListC<XMLTreeC> children;
      if(!newTree.FollowPath(xi_xpointer,children)) {
        RavlWarning(StringC("Failed to follow xpointer in file '" + xi_href +"' with path '" + xi_xpointer + "' "));
	return false;
      }
      for(DLIterC<XMLTreeC> it(children);it;it++) 
	Add(it->Name(),*it);
      return true;
    }
    
    // Look for first non processing directive.
    subtree.Invalidate();
    for(DLIterC<XMLTreeC> it(newTree.Children());it;it++) {
      if(!it->IsPI())
	Add(it->Name(),*it);
    }
    
    return true;
  }

  //: Look for fall back
  
  bool XMLTreeBodyC::ProcessIncludeFallback(XMLTreeC &subtree,
                                            HSetC<StringC> &doneFiles,
                                            const StringC &parentFilename,
                                            XMLTreeLoadC *loader)
  {
    if(subtree.Children().IsEmpty())
      return false;
    XMLTreeC childTree = subtree.Children().First();
    if(childTree.Name() != "xi:fallback") {
      RavlWarning(StringC("Unexpected xi:include child, '" + childTree.Name() +"'"));
      return false;
    }
    if(!ProcessInclude(childTree,doneFiles,parentFilename,loader)) {
      return false;
    }
    subtree = childTree;
    return true;
  }
  
  //: Give list of nodes matching the given path.
  
  bool XMLTreeBodyC::FollowPath(const StringC &path,DListC<XMLTreeC> &nodes) {
    StringListC steps(path,"/");
    return FollowPath(steps,nodes);
  }
  
  //: Give list of nodes matching the given path.
  
  bool XMLTreeBodyC::FollowPath(const DListC<StringC> &path,DListC<XMLTreeC> &nodes) {
    DListC<XMLTreeC> current;
    DListC<XMLTreeC> newNodes;
    newNodes += XMLTreeC(*this);
    
    for(DLIterC<StringC> sit(path);sit;sit++) {
      current = newNodes;
      newNodes = DListC<XMLTreeC>();
      HSetC<XMLTreeC> done;
      
      int axisAt = sit->index("::");
      StringC axis = "child";
      StringC spec = *sit;
      if(axisAt >= 0) {
	axis = sit->before(axisAt);
	spec = sit->after(axisAt+1);
	if(spec.IsEmpty())
	  spec = "*";
      }
      
      if(axis == "child") {
	if(spec == "*") {
	  // Just include all children.
	  for(DLIterC<XMLTreeC> cit(current);cit;cit++) {
	    for(DLIterC<XMLTreeC> it(cit->Children());it;it++) {
              if(done.Insert(*it))
                newNodes += *it;
	    }
	  }
	  continue;
	}
	
	// Include children with matching names.
	for(DLIterC<XMLTreeC> cit(current);cit;cit++) {
	  for(DLIterC<XMLTreeC> it(cit->Children());it;it++) {
	    if(it->Name() == spec) {
              if(done.Insert(*it))
                newNodes += *it;
            }
	  }
	}
	continue;
      }
      
      RavlWarning(StringC("Unsupported axis specifier='" + axis +"'"));
      
      return false;
    }
    
    nodes =  newNodes;
    return true;
  }
  

  //: Add subtree to node.
  
  bool XMLTreeBodyC::Add(const StringC &name,const XMLTreeC &subtree) {
    children.InsLast(subtree);
    return HashTreeBodyC<StringC,RCHashC<StringC,StringC> >::Add(name,subtree,true);
  }

  
  //: Add subtree to node.
  
  bool XMLTreeBodyC::Add(const XMLTreeC &subtree) {
    children.InsLast(subtree);
    return HashTreeBodyC<StringC,RCHashC<StringC,StringC> >::Add(subtree.Name(),subtree,true);
  }
  
  //: Add content to node.

  bool XMLTreeBodyC::AddContent(const StringC &content){
    RCHashC<StringC,StringC> attr;
    attr["."] = content;
    XMLTreeC item("?content", attr);
    children.InsLast(item);
    return HashTreeBodyC<StringC,RCHashC<StringC,StringC> >::Add(item.Name(),item,true);
  }
  
  //: lookup child in tree.
  // Returns true and updates parameter 'child' if child is found.
  
  bool XMLTreeBodyC::Child(const StringC &key,XMLTreeC &child) const {
    return const_cast<XMLTreeBodyC*>(this)->HashTreeBodyC<StringC,RCHashC<StringC,StringC> >::Child(key,child);
  }

  //: Get the content of a child if it exists.
  // Return true if value found.

  bool XMLTreeBodyC::ChildContent(const StringC &key,StringC &value) const {
    XMLTreeC child;
    if(!Child(key,child))
      return false;
    value = child.Content();
    return true;
  }


  //: Indents the XML listing

  std::ostream &XMLTreeBodyC::Indent(std::ostream &out,int level) {
    for(int i = 0;i < level;i++)
      out << ' ';
    return out;
  }
  
  // : Write (sub)tree as valid XML file
  bool XMLTreeBodyC::Write(OStreamC &out,int level) const {
    XMLTreeC me(const_cast<XMLTreeBodyC &>(*this));
    if (level == 0) {  // no tag at top level
      for(DLIterC<XMLTreeC> it(me.Children());it;it++) {
        it.Data().Write(out,level+1);
      }
    }
    else if (Name() == "?content") Indent(out,level) << Data()["."] << "\n";
    else {
      Indent(out,level) << '<';
      if (isPI) {
        out << '?';
      }
      out << Name();
      for(HashIterC<StringC,StringC> ita(Data());ita;ita++) {
        out << ' ' << ita.Key() << "=\"" << ita.Data() << "\"";
      }
      if(isPI) {
      out << "?>\n";
    }
    else if(me.Children().IsEmpty()) {
        out << "/>\n";
      }
      else {
        out << ">\n";
        for(DLIterC<XMLTreeC> it(me.Children());it;it++) {
          it.Data().Write(out,level+1);
        }
        Indent(out,level) << "</" << Name() << ">\n";
      }
    }
    return true;
  }

  //: Dump tree in a human readable format.
  
  std::ostream &XMLTreeBodyC::Dump(std::ostream &out,int level) const {
    XMLTreeC me(const_cast<XMLTreeBodyC &>(*this));
    Indent(out,level) << '<';
    if (isPI) {
      out << '?';
    }
    out << Name();
    for(HashIterC<StringC,StringC> ita(Data());ita;ita++) {
      out << ' ' << ita.Key() << "=\"" << ita.Data() << "\"";
    }
    if(isPI) {
      out << "?>\n";
    }
    else if(me.Children().IsEmpty()) {
      out << "/>\n";
    } 
    else {
      out << ">\n";
      for(DLIterC<XMLTreeC> it(me.Children());it;it++) {
	it.Data().Dump(out,level+1);
      }
      Indent(out,level) << "</" << Name() << ">\n";
    }
    return out;
  }


  StringC XMLTreeBodyC::AttributeString(const StringC &name,const StringC &defaultValue) const
  {
    const StringC *value = Data().Lookup(name);
    if(value == 0) return defaultValue;
    return *value;
  }
  //: Access attribute.

  bool XMLTreeBodyC::Attribute(const StringC &name,StringC &data,const StringC &defaultValue) const
  {
    const StringC *value = Data().Lookup(name);
    if(value == 0) {
      data = defaultValue;
      return false;
    }
    data = *value;
    return true;
  }
  //: Access string attribute.
  // Return true if non default value has been specified.

  bool XMLTreeBodyC::Attribute(const StringC &name,std::string &data,const std::string &defaultValue) const
  {
    const StringC *value = Data().Lookup(name);
    if(value == 0) {
      data = defaultValue;
      return false;
    }
    data = value->data();
    return true;
  }
  //: Access string attribute.
  // Return true if non default value has been specified.


  UIntT XMLTreeBodyC::AttributeUInt(const StringC &name,UIntT defaultValue) const
  {
    const StringC *value = Data().Lookup(name);
    if(value == 0) return defaultValue;
    return value->UIntValue();
  }
  //: Access attribute.

  IntT XMLTreeBodyC::AttributeInt(const StringC &name,IntT defaultValue) const
  {
    const StringC *value = Data().Lookup(name);
    if(value == 0) return defaultValue;
    return value->IntValue();
  }
  //: Access attribute.

  RealT XMLTreeBodyC::AttributeReal(const StringC &name,RealT defaultValue) const
  {
    const StringC *value = Data().Lookup(name);
    if(value == 0) return defaultValue;
    return value->RealValue();
  }
  //: Access attribute.

  UInt64T XMLTreeBodyC::AttributeUInt64(const StringC &name,UInt64T defaultValue) const
  {
    const StringC *value = Data().Lookup(name);
    if(value == 0) return defaultValue;
    return value->UInt64Value();
  }
  //: Access attribute.

  Int64T XMLTreeBodyC::AttributeInt64(const StringC &name,Int64T defaultValue) const
  {
    const StringC *value = Data().Lookup(name);
    if(value == 0) return defaultValue;
    return value->Int64Value();
  }
  //: Access attribute.

  bool XMLTreeBodyC::Attribute(const StringC &name,bool &data,const bool &defaultValue) const
  {
    const StringC *value = Data().Lookup(name);
    if(value == 0) {
      data = defaultValue;
      return false;
    }
    StringC tmp = RavlN::downcase(*value);
    if(tmp == "1" || tmp == "t" || tmp == "true" || tmp == "yes") {
      data = true;
      return true;
    }
    if(tmp == "0" || tmp == "f" || tmp == "false" || tmp == "no") {
      data = false;
      return true;
    }
    RavlIssueWarning(StringC("Expected boolean value, got '") + tmp + "' for attribute " + name + " in node '" + Name() + "' ");
    data = defaultValue;
    return false;
  }

  bool XMLTreeBodyC::AttributeBool(const StringC &name,bool defaultValue) const
  {
    bool ret;
    Attribute(name,ret,defaultValue);
    return ret;
  }
  //: Access attribute.

  bool XMLTreeBodyC::HasAttribute(const StringC &name) const
  { return Data().Lookup(name) != 0; }
  //: Test if attribute is set.
  
  static TypeNameC type0(typeid(XMLTreeC),"RavlN::XMLTreeC");
  
  
}
