// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/Document.cc"

#include "Ravl/CxxDoc/Document.hh"
#include "Ravl/CxxDoc/CxxElements.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/StrStream.hh"
#include <ctype.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlCxxDocN {

  static HSetC<StringC> ListNativeTypes() {
    HSetC<StringC> ret;
    ret += "void";
    ret += "bool";
    ret += "char";
    ret += "short";
    ret += "int";
    ret += "long";
    ret += "signed";
    ret += "unsigned";
    ret += "float";
    ret += "double";
    ret += "wchar_t";
    return ret;
  }
  
  static HSetC<StringC> ListWordsToIgnore() {
    HSetC<StringC> ret = ListNativeTypes();
    ret += "a";
    ret += "is"; 
    ret += "in";
    ret += "on";
    ret += "it";
    ret += "an";
    ret += "of";
    ret += "to";
    ret += "the";
    ret += "The";
    ret += "use";
    ret += "are";
    ret += "off";
    ret += "one";
    ret += "two";
    ret += "six";
    ret += "ten";
    ret += "nine";
    ret += "from";
    ret += "have";
    ret += "take";
    ret += "some";
    ret += "into";
    ret += "been";
    ret += "with";
    ret += "used";
    ret += "page";
    ret += "four";
    ret += "five";
    ret += "three";
    ret += "pages";
    ret += "there";
    ret += "these";
    ret += "other";
    ret += "maybe";
    ret += "input";
    ret += "ready";
    ret += "eight";
    ret += "seven";
    ret += "stream";
    ret += "output";
    ret += "appear";
    ret += "method";
    ret += "simple";
    ret += "methods";
    ret += "complex";
    ret += "returns";
    ret += "standard";
    ret += "function";
    ret += "functions";
    ret += "interface";
    
    ret += "and";
    ret += "and_eq";
    ret += "asm";
    ret += "auto";
    ret += "bitand";
    ret += "bitor";
    ret += "break";
    ret += "case";
    ret += "catch";
    ret += "class";
    ret += "compl";
    ret += "const";
    ret += "const_cast";
    ret += "continue";
    ret += "default";
    ret += "delete";
    ret += "delete";
    ret += "do";
    ret += "dynamic_cast";
    ret += "else";
    ret += "enum";
    ret += "explicit";
    ret += "export";
    ret += "extern";
    ret += "false";
    ret += "false";    
    ret += "for";
    ret += "friend";
    ret += "goto";    
    ret += "if";
    ret += "inline";
    ret += "mutable";
    ret += "namespace";
    ret += "new";
    ret += "not";
    ret += "not_eq";
    ret += "operator";
    ret += "or";
    ret += "or_eq";
    ret += "private";

    ret += "protected";
    ret += "public";
    ret += "register";
    ret += "reinterpret_cast";
    ret += "return";
    ret += "sizeof";
    ret += "static";
    ret += "static_cast";
    ret += "struct";
    ret += "switch";
    ret += "template";
    ret += "this";
    ret += "throw";
    ret += "true";
    ret += "try";
    ret += "typedef";
    ret += "typeid";
    ret += "typename";
    ret += "union";
    ret += "using";
    ret += "virtual";
    ret += "volatile";    
    ret += "while";
    ret += "xor";
    ret += "xor_eq";
    
    ret += "root";

    return ret;
  }
  
  static HSetC<StringC> nativeTypes = ListNativeTypes();
  static HSetC<StringC> ignoreWords = ListWordsToIgnore();
  
  //: Default contructor.

  DocumentBodyC::DocumentBodyC(const FilenameC &tmplName,const StringC &outDir,const DocTreeC &ndocTree,const StringC &projName,const StringC &projDesc)
    : TemplateComplexBodyC(tmplName),
      outputDir(outDir),
      fileObject("class"),
      filePattern("%.html"),
      docTree(ndocTree),
      verbose(true)
  {
    templArgSub.Push(RCHashC<StringC,ObjectC>());
    if(!tmplName.Exists())
      std::cerr << "WARNING: Can't open template file '" << tmplName << "' \n";
    Init();
    SetVar("projectName",projName);
    SetVar("projectDesc",projDesc);
  }

  //: If there's a substituation use it.
  inline
  StringC DocumentBodyC::TemplateNameSubst(const StringC &x) { 
    ObjectC *el = templArgSub.Top().Lookup(x);
    if(el == 0)
      return x;
    if(DataTypeC::IsA(*el))
      return el->FullName(templArgSub.Top());
    return el->Name();
  }
  
  //: Get initalise information from template file.
  
  void DocumentBodyC::Init() {
    InitVars();
    
    if(outputDir != "") {
      if(outputDir.lastchar() != '/')
	outputDir += '/';
    }
    
    // Get preset variables.
    
    HSetC<StringC> npresets;
    npresets += StringC("FilePattern");
    npresets += StringC("FileObject");
    SetupPresets(npresets);
    Scan(templFile);
    filePattern = GetVar("FilePattern");
    fileObject = GetVar("FileObject");    
    
#if 1
    if(verbose) {
      std::cerr << "Found file object '" << fileObject << "'\n";
    }
#endif
    
    // Setup commands.
    SetupCommand("forall",*this,&DocumentBodyC::Forall);
    SetupCommand("ifany",*this,&DocumentBodyC::IfAny);
    SetupCommand("MakeFilename",*this,&DocumentBodyC::MakeFilenameCmd);
    SetupCommand("GotoInherit",*this,&DocumentBodyC::GotoInherit);
    SetupCommand("HtmlMethodName",*this,&DocumentBodyC::HtmlMethodName);
    SetupCommand("HtmlTypeName",*this,&DocumentBodyC::HtmlTypeName);
    SetupCommand("MakeDocNode",*this,&DocumentBodyC::InsertDocNode);
    SetupCommand("AutoLink",*this,&DocumentBodyC::AutoLink);
    SetupCommand("Exec",*this,&DocumentBodyC::Exec);
    SetupCommand("ForDir",*this,&DocumentBodyC::ForDir);
  }
  
  //: Init known vars list.
  
  void DocumentBodyC::InitVars() {
    knownVars += StringC("ObjectName");
    knownVars += StringC("FullName");
    knownVars += StringC("brief");
    knownVars += StringC("detail");
    knownVars += StringC("userlevel");
    knownVars += StringC("dectype");
    knownVars += StringC("static");
    knownVars += StringC("array");
    knownVars += StringC("virtual");
    knownVars += StringC("body");    
  }
  
  //: Document list of files.
  
  void DocumentBodyC::Document(ObjectListC &ol) {
    if(!root.IsValid())
      root = ol;
    
    //cerr << "FileObject : '" << fileObject << "'\n";
    if(fileObject == "none")  // Generate nothing!
      return ;
    
    // Only generating one file ?
    
    if(fileObject == "one") {
      FilenameC fn(outputDir + filePattern);
      obj.Push(ol);
      Build(fn);
      obj.DelTop();
      return ;
    }
    
    if(fileObject == "namespace") {
      if(!ol.HasParentScope()) {
	if(!ClassC::IsA(ol)) { // NOT classes!!
	  FilenameC fn = MakeFilename(ol);
	  obj.Push(ol);
	  Build(fn);
	  obj.DelTop();
	}
      }
    }

    // For each input file...
    
    if(fileObject == "class" || fileObject == "namespace") {
      for(DLIterC<ObjectC> dit(ol.List());dit.IsElm();dit.Next()) {
	// If its a scope, recurse...
	if(ScopeC::IsA(dit.Data())) { // Do child namespaces as well.
	  ObjectListC sol(dit.Data());
	  Document(sol);
	}
	// Do a quick filter on type.
	if(fileObject != dit.Data().TypeName())
	  continue;
	// Ignore everything that isn't a proper definition...
	StringC decType = dit.Data().Var("dectype");
	if(decType == "forward")
	  continue;
	//cerr << "Document() " << fileObject << " " << dit.Data().TypeName() << "  " << decType <<"\n";
	// Generate file.
	FilenameC fn = MakeFilename(dit.Data());
	cerr << "******* GENERATING FILE " << fn << " ********** \n";
	obj.Push(dit.Data());	
	Build(fn);
	obj.DelTop();
      }
      return;
    }

    // For every global function.
    if(fileObject == "function") {
      for(DLIterC<ObjectC> dit(ol.List());dit.IsElm();dit.Next()) {
	// If its a scope, recurse...
	if(ScopeC::IsA(dit.Data()) && !ClassC::IsA(dit.Data())) { 
	  // Do child namespaces as well, but not classes
	  ObjectListC sol(dit.Data());
	  Document(sol);
	  continue;
	}
	if(!MethodC::IsA(dit.Data()))
	  continue;
	FilenameC fn = MakeFilename(dit.Data());
	cerr << "******* GENERATING FILE " << fn << " (Method) ********** \n";
	obj.Push(dit.Data());	
	Build(fn);
	obj.DelTop();	
      }
      return ;
    }
    
    if(fileObject == "docnode") {
      if(fileObject == ol.TypeName()) {
	FilenameC fn = MakeFilename(ol);
	if(verbose) 
	  std::cerr << "Writing docnode file '" << fn << "' \n";
	obj.Push(ol);
	Build(fn);
	obj.DelTop();
      }
      // Do child nodes as well.
      // NB. This will NOT search the whole tree for document
      // nodes. The root document node should be in the root
      // parse node.
      for(DLIterC<ObjectC> dit(ol.List());dit.IsElm();dit.Next()) {
	if(fileObject != dit->TypeName())
	  continue;
	if(!ObjectListC::IsA(*dit))
	  continue;
	ObjectListC sol(dit.Data());
	Document(sol);
      }
      return ;
    }
    if(fileObject == "exe") {
      for(DLIterC<ObjectC> dit(ol.List());dit.IsElm();dit.Next()) {
	// If its a scope, recurse...
	if(fileObject != dit->TypeName())
	  continue;
	FilenameC fn = MakeFilename(dit.Data());
	cerr << "******* GENERATING FILE " << fn << " (executable) ********** \n";
	obj.Push(dit.Data());
	Build(fn);
	obj.DelTop();	
      }
      return ;
      
    }
    int param = fileObject.index(':');
    if(param >= 0) {
      SubStringC objType = fileObject.before(param);
      if(objType == "file") {
	StringC theRest = fileObject.after(param);
	StringC searchDir = outputDir + theRest.before('/',-1);
	DirectoryC dir(searchDir);
	StringC filterStr = theRest.after('/',-1);
	DListC<StringC> files = dir.FiltList(filterStr);
	cerr << "Found " << files.Size() << " files in " << searchDir << "/" << theRest.after('/',-1) << "\n";
	for(DLIterC<StringC> it(files);it;it++) {
	  if(it->firstchar() == '.' && filterStr.firstchar() != '.')
	    continue; // Ignore dot files if its not in the filter.
	  FilenameC fn(outputDir + filePattern);
	  StringC cfn = FilenameC(*it).NameComponent();
	  fn.gsub("%",cfn);
	  std::cerr << "******* GENERATING FILE " << fn << " for '" << *it << "' ********** \n";
	  DocNodeC docNode(cfn,"Examples","Default","",*it);
	  docNode.SetVar("dir",searchDir);
	  docNode.SetVar("file",*it);
	  obj.Push(docNode);
	  Build(fn);
	  obj.DelTop();
	}
	return ;
      }
    }
    
    std::cerr << "Unknown file object: '" << fileObject << "' in template " << templFile.Filename() << "\n";
  }
  
  //: Make string suitable for use in plain html.
  //: Make a linked version of the method name.
  
  bool DocumentBodyC::HtmlMethodName(StringC &x) {
    ONDEBUG(std::cerr << "DocumentBodyC::HtmlMethodName(), Processing: '" << obj.Top().Name() << "'\n");
#if RAVL_CHECK
    if(!MethodC::IsA(obj.Top())) {
      ONDEBUG(std::cerr << "DocumentBodyC::HtmlMethodName(), ");
      std::cerr << "WARNING: HtmlMethodName called on non method object '" << obj.Top().Name() << "' \n";
    }
#endif
    Output() << obj.Top().FullName(templArgSub.Top(),*this,60);
    return true;
  }
    
  //: Make a linked version of the method name.
  
  bool DocumentBodyC::HtmlTypeName(StringC &arg) {
    OStreamC &out = Output();
    if(obj.Top().IsTemplate()) {
      ONDEBUG(std::cerr << "DocumentBodyC::HtmlTypeName(), Processing :" << obj.Top().Name() << "\n");
      if(arg == "link") {
	out << obj.Top().FullName(templArgSub.Top(),*this);
	return true;
      }
      out << MakeHtml(obj.Top().FullName(templArgSub.Top()));
      return true;
    }
    if(DataTypeC::IsA(obj.Top())) {
      DataTypeC atype(obj.Top());
      out << GetHtmlTypeName(atype);
      return true;
    }
    if(InheritC::IsA(obj.Top())) {
      if(arg == "link") {
	out << obj.Top().FullName(templArgSub.Top(),*this);
	return true;
      }
      out << MakeHtml(obj.Top().FullName(templArgSub.Top()));
      return true;
    }
    out << obj.Top().Name();
    return true;
  }

  //: How to render an char into html.
  
  StringC DocumentBodyC::TextFor(char let) {
    // Put the common characters in pre-built strings.
    static StringC amp("&amp;");
    static StringC lt("&lt;");
    static StringC gt("&gt;");
    static StringC comma(",");
    static StringC lb("(");
    static StringC rb(")");
    switch(let) 
      {
      case '&': return amp;
      case '<': return lt;
      case '>': return gt;
      case ',': return comma;
      case '(': return lb;
      case ')': return rb;
      }
    // Handle anything else.
    char arr[2];
    arr[0] = let;
    arr[1] = 0;
    return StringC(arr);
  }

  //: How to render a string in html.
  
  StringC DocumentBodyC::TextFor(const StringC &obj) {
    return MakeHtml(obj);
  }
  
  //: Render a method name appropriatly. (e.g. bold.)
  
  StringC DocumentBodyC::MethodNameText(const StringC &obj) {
    // The markup should really come from the variables set in the template file.
    return StringC("<B>") + MakeHtml(obj) + StringC("</B>");
  }

  StringC DocumentBodyC::TextFor(const ObjectC &xObj) {
    ONDEBUG(std::cerr << "DocumentBodyC::TextFor() '" << xObj.Name() << "' type:'" << xObj.TypeName() << "' called. Template:" << xObj.IsTemplate() << "\n");
    ObjectC anObj(xObj);
    DListC<ObjectC> ss;
    if(!obj.Top().IsValid())
      return anObj.Name();
    if(!obj.Top().HasParentScope())
      return anObj.Name();
    ScopeC scope = obj.Top().ParentScope();
    if(DataTypeC::IsA(anObj)) {
      DataTypeC tn(anObj);
      anObj.Invalidate();
      ONDEBUG(std::cerr << "DocumentBodyC::TextFor() Attempting to generate link for '" << xObj.Name() << "' \n");
      ObjectListC scopePath = const_cast<DataTypeC &>(tn).ScopePath();
      if(scopePath.IsValid()) {
	// Do a template substitution ?
	if(scopePath.List().Size() == 1) {
	  ObjectC &obj = scopePath.List().Last();
	  ObjectC *lu = templArgSub.Top().Lookup(obj.Name());
	  if(lu != 0) {
	    if(DataTypeC::IsA(*lu)) {
	      DataTypeC dt(*lu);
	      scopePath = dt.ScopePath();
	    }
	  }
	}
	ss = scopePath.List();
	RCHashC<StringC,ObjectC> templSub;
	anObj = scope.ResolveName(ss,templSub);
	// Should do something with templSub here ??
      }
    } else {
      ONDEBUG(std::cerr << "DocumentBodyC::TextFor() Looking up name '" << xObj.Name() << "' in scope :'" << scope.Name() <<"'\n");
      anObj = scope.ResolveName(xObj.Name());
      if(anObj.IsValid()) {
        ONDEBUG(std::cerr << "DocumentBodyC::TextFor() Found '" << anObj.Name() << "'\n");
	ss.InsLast(xObj);
      }
    }
    
    if(!anObj.IsValid()) { // Assume its not documented...
      ONDEBUG(std::cerr << "DocumentBodyC::TextFor() Undocumented" << xObj.Name() << "\n\r");
      if(nativeTypes.IsMember(xObj.Name()))
	return xObj.Name(); // All native types are fine as plain text.
      ONDEBUG(std::cerr << "WARNING: DocumentBodyC::TextFor() Can't resolve name, for HtmlTypeName. '" << xObj.Name() << "' -> '" << xObj.Name() << "'\n");
      return MakeHtml(xObj.Name());
    }
    
    if ( anObj.ActualPath().TopAndTail().index("std::") == 0 )
        return xObj.Name(); // We don't document things from the standard namespace

    // Put a link in around the type.
    StringC ret = "<a href=\"";
    StringC refPattern;
    if(!Lookup("refPattern",refPattern))
      refPattern = filePattern;
    if(!TypedefC::IsA(anObj) && !EnumC::IsA(anObj)) // FIXME:- This should be set in the template somewhere.
      ret += MakeFilename(refPattern,anObj,true);
    else { // Take care of typedef's
      if(anObj.HasParentScope()) {
	ScopeC ps(anObj.ParentScope());
	ret += MakeFilename(refPattern,ps,true);
      } else
	cerr << "WARNING: Can't resolve typedef name, no parent scope for :" << obj.Top().Name() << "\n";
      ret += '#';
      ret += MakeHRef(anObj.Name());
    }
    ONDEBUG(std::cerr << "DocumentBodyC::TextFor() Linking  '" << MakeHtml(xObj.Name()) << "' to '" << ret.after(7) << "'\n");
    ret += StringC("\">") + MakeHtml(xObj.Name()) + "</a>";
    return ret;
  }
  
  //: Make a linked version of the method name.
  
  StringC DocumentBodyC::GetHtmlTypeName(const DataTypeC &tn) {    
    ONDEBUG(std::cerr << "DocumentBodyC::GetHtmlTypeName(), Processing: '" << tn.Name() << "' Template:" << tn.IsTemplated() << " FN:" << tn.FullName(templArgSub.Top()) <<  "\n");
    if(!obj.Top().HasParentScope()) {
      ONDEBUG(std::cerr << "DocumentBodyC::GetHtmlTypeName() ");
      std::cerr << "WARNING: Can't resolve name, no parent scope for :" << obj.Top().Name() << "\n";
      ONDEBUG(std::cerr << " Doc:" << tn.Name() << "\n");
      return MakeHtml(tn.Name());
    }
    
    StringC ret = tn.FullName(templArgSub.Top(),*this);
    ONDEBUG(std::cerr << "DocumentBodyC::GetHtmlTypeName(), Fullname:" << ret << "\n");
    // Need to add template args ?
    return ret;
  }
  
  //: Goto an inherited class.
  
  bool DocumentBodyC::GotoInherit(StringC &param) {
    //cerr << "DocumentBodyC::GotoInherit(), Called for " << obj.Top() << "\n";
    ObjectC &anobj = obj.Top();
    ObjectC newObj;    
    bool found = false;
    if(InheritC::IsA(anobj)) {
      found = true;
      InheritC ihf(anobj);
      newObj = ihf.From();
      if(!newObj.IsValid())
	newObj = ObjectC(ihf.ScopeDef());
    }
    if(DerivedC::IsA(anobj)) {
      found = true;
      DerivedC der(anobj);
      newObj=der.DerivedObj();
    }
    
    if(!found) {
      std::cerr << "WARNING: GotoInherit used outside inherit. In:'" << anobj.Name() << "'\n";
      Output() << "***Invalid GotoInherit***";
      return false;
    }
    if(!newObj.IsValid()) {
      std::cerr << "ERROR: Invalid object from inherit. " << anobj.Name() << " Type:" << anobj.TypeName() << "\n";
      return false;
    }
    obj.Push(newObj);
    TextFileC subTextBuff(param,true,true);
    BuildSub(subTextBuff);
    obj.DelTop();
    return true;
  }
  
  //: Makefile name
  
  FilenameC DocumentBodyC::MakeFilename(ObjectC &anobj,bool relative) {
    return MakeFilename(filePattern,anobj,relative);
  }
  
  //: Makefile name, send it to output.
  
  bool DocumentBodyC::MakeFilenameCmd(StringC &pattern) {
    Output() << MakeFilename(pattern,obj.Top(),true);
    return true;
  }
  
  //: Make a filename from a pattern and a full object name.
  
  FilenameC DocumentBodyC::MakeFilename(StringC pattern,ObjectC &anobj,bool relative) {
    StringC ret;
    if(!relative)
      ret = outputDir.Copy();
    if(!anobj.IsValid()) {
      ret += "unknown";
      std::cerr << "DocumentBodyC::MakeFilename(), Called with an invalid object. (Returning '" << ret << "')\n";
      return ret;
    }
    ret += pattern;
    //cerr << "FullPath:" << obj.FullPath() <<"\n";
    StringC objName = anobj.ActualPath().TopAndTail() ; 
    objName.gsub("/","Slash") ; 
    ret.gsub("%", objName);
    ret.gsub("<","Lt");
    ret.gsub(">","Gt");
    ret.gsub("(","Ob"); // These may appear in pages on functions.
    ret.gsub(")","Cb");
    ret.gsub("&","Amp");
    ret.gsub(",","_");
    ret.gsub(" ","_");
    ret.gsub("::",".");
    return ret;    
  }

  //: Lookup variable.
  // if found put value into 'buff' and return true.
  // otherwise return false.
  
  bool DocumentBodyC::Lookup(const StringC &varname,StringC &buff) {
    // Retrieve value from current object ?
    //ONDEBUG(std::cerr << "DocumentBodyC::Lookup(), " << obj.Top().Name() << " '" << varname << "' \n");
    if(!obj.Top().IsValid()) {
      std::cerr << "DocumentBodyC::Lookup(), ERROR: Called on invalid object. Stackdepth:" << obj.Size() << "\n";
      RavlAssert(0);
      return false;
    }
    if(obj.Top().Comment().Vars().Lookup(varname,buff)) {
      //ONDEBUG(std::cerr << "DocumentBodyC::Lookup(), ObjectVar: " << varname << " = " << buff << "\n");
      return !buff.IsEmpty();
    }
    if(obj.Top().Comment().Locals().Lookup(varname,buff))
      return !buff.IsEmpty();
    
    const StringC &ip = varname;
    
    if(ip == "ObjectName") {
      buff = obj.Top().Name();
      return true;
    }
    
    if(ip == "Name") {
      buff = obj.Top().Name();
      //obj.Top().SetVar(ip,buff);
      return true;
    }
    
    if(ip == "NodeType") {
      buff = obj.Top().TypeName();
      //obj.Top().SetVar(ip,buff);
      return true;
    }
    
    if(ip == "href") { // Generate a href sutable name.
      buff = MakeHRef(obj.Top().Name());
      obj.Top().SetVar(ip,buff);
      return true;
    }
    
    if(ip == "storageType") { // Setup default
      buff = "class";
      obj.Top().SetVar(ip,buff);
      return true;
    }
    
    if(ip == "userlevel") { // Setup default userlevel
      buff = "Default";
      return true;
    }
    
    if(ip == "FullName") { // Setup defau
      buff = MakeHtml(obj.Top().FullName());
      obj.Top().SetVar(ip,buff);
      return true;
    }
    
    if(ip == "PathName") { // Setup defau
      buff = obj.Top().FullPath();
      obj.Top().SetVar(ip,buff);
      return true;
    }
    
    if(ip == "filename") { // Search out filename. 
      buff = "unknown";
      if(obj.Top().HasParentScope()) {
	ScopeC place(obj.Top().ParentScope());
	while(place.HasParentScope()) {
	  if(place.IsVar("filename")) {
	    buff = place.Var("filename");
	    break;
	  }
	  place = place.ParentScope();
	}
      }
      obj.Top().SetVar("filename",buff);
      return true;
    }
    
    if(ip == "ClassName") {
      if(ClassC::IsA(obj.Top())) {
	buff = obj.Top().Name();
	return true;
      }
      buff = "*** NOT A CLASS ***";
      std::cerr << "WARNING: ClassName used outside class. In:'" << obj.Top().Name() << "'\n";
      return true;
    }
    
    if(ip == "InheritFrom") {
      if(!InheritC::IsA(obj.Top())) {
	cerr << "WARNING: InheritFrom used outside inherit. In:'" << obj.Top().Name() << "'\n";
	buff = "***Invalid InheritFrom***";
	return true;
      }
      InheritC ihf(obj.Top());
      buff = ihf.ScopeDef();
      return true;
    }

    if(ip == "FullParentScope") {
      if(obj.Top().HasParentScope()) { // Is it a member of a class ?
       ScopeC ps(obj.Top().ParentScope());
       buff = ps.Name().Copy();
       obj.Top().SetVar("ClassName",ps.Name());
        while (ps.HasParentScope())
        {  ps=ps.ParentScope();
           if (ps.Name() == "std" )
              break;
           if (buff.matches("::") == 0)
              buff.prepend("::");
           buff.prepend(ps.Name());
        }
      } else {
       obj.Top().SetVar("ClassName","std");
       Output() << "std";
      }

      return true;
    }

    if(ip == "ParentScope") {
      if(obj.Top().HasParentScope()) { // Is it a member of a class ?
	ScopeC ps(obj.Top().ParentScope());
	buff = ps.Name();
	obj.Top().SetVar("ClassName",ps.Name());
      } else {
	obj.Top().SetVar("ClassName","std");
	Output() << "std";
      }
	
      return true;
    }
    
    if(ip == "variable_type") {
      if(!VariableC::IsA(obj.Top())) {
	buff = "*** Invaid variable_type ****"; 
	cerr << "WARNING: variable_type used in an incorrect context. In:'" << obj.Top().Name() << "'\n";
	return true;
      }
      VariableC atd(obj.Top());
      buff = GetHtmlTypeName(atd.DataType());
      obj.Top().SetVar("typedef_type",buff);
      return true;
    }
    
    if(ip == "typedef_type") {
      if(!TypedefC::IsA(obj.Top())) {
	buff = "*** Invaid typedef_type ****"; 
	cerr << "WARNING: typedef_type used outside typedef. In:'" << obj.Top().Name() << "'\n";
	return true;
      }
      TypedefC atd(obj.Top());
      buff = GetHtmlTypeName(atd.DataType());
      obj.Top().SetVar("typedef_type",buff);
      return true;
    }
    
    // Retrieve from other vars ?
    
    if(TemplateComplexBodyC::Lookup(varname,buff))
      return true;

    if(ip == "brief") {
      buff = obj.Top().Comment().Header();
      return true;
    }
    
    // Setup some default values.
    
    if(ip == "buildtime") { // Setup default
      DateC now(true);
#ifdef CXXDOC_FALSE_TIME
      buff = "Wed May 25 22:07:00 2005";
#else
      buff = now.CTime();
#endif
      SetVar(ip,buff);
      return true;
    }
    
#if 0
    if(knownVars.IsMember(ip)) {
      buff = StringC("");
      return true;
    }
#endif
    return false;
  }
  
  //: Make a string suitable for an href.
  
  StringC DocumentBodyC::MakeHRef(StringC name) {
    StringC newRef(name.Copy());
    newRef.gsub("<","St");
    newRef.gsub(">","Bt");
    newRef.gsub(".","_");
    newRef.gsub(",","_");
    newRef.gsub(" ","_");
    newRef.gsub("&","Ref");
    newRef.gsub("*","Ptr");
    newRef.gsub("(","Ob");
    newRef.gsub(")","Cb");
    return newRef;
  }
  
  //: Do a forall build.

  
  bool DocumentBodyC::Foralli(StringC &data,bool ifAny,bool inClassScope) {
    //ONDEBUG(std::cerr << "For all in '"<< obj.Top().Name() << "' do '" << dat << "'\n");
    
    int templStart = data.index(':');
    if(ifAny)
      lastIf = false;
    if(templStart < 1) {
      std::cerr << "Malformed 'forall' in template. '" << data << "' ignoring \n";
      return false;
    }    
    StringC typedata = data.before(templStart);
    
    // Check for a condition in the for loop.
    StringC condition;
    IntT condInd =typedata.index(',');
    if(condInd >= 0) {
      condition = typedata.after(condInd);
      //cerr << "Found condition '" << condition << "'\n";
      typedata = typedata.before(condInd);
      if(typedata == "") {
	cerr << "Malformed 'forall' in template. '" << data << "' ignoring \n";
	return false;
      }
    }
    
    // Variable in the element type ?
    if(typedata[0] == '$') {
      typedata = typedata.after(0);
      StringC buff;
      if(!Lookup(typedata,buff)) {
	cerr << "Unknown variable. '" << typedata << "' in forall. \n";
	return false;
      }
      typedata = buff;
      //cerr << "Got variable in  '" << typedata << "' in forall. \n";
    }

#ifndef NDEBUG
    if(typedata != "method"      && 
       typedata != "method_user" && 
       typedata != "namespace" && 
       typedata != "class" && 
       typedata != "inherit" && 
       typedata != "scope" && 
       typedata != "scope_user" && 
       typedata != "derived" && 
       typedata != "friend" && 
       typedata != "enum" && 
       typedata != "typedef" && 
       typedata != "variable" &&
       typedata != "all" &&
       typedata != "docnode" &&
       typedata != "docleaf") {
      std::cerr << "WARNING: Unknown forall type :" << typedata << "\n";
    }
#endif
    StringC subtempltxt = data.after(templStart);
    
    TextFileC subTextBuff(subtempltxt,true,true);
    //subTextBuff.Dump(std::cerr);
    //cerr << "For all dump. \n";
    if(!ObjectListC::IsA(obj.Top())) {
      std::cerr << "ERROR: Forall " << obj.Top().Name() << ", is not an object list! \n";
      return false;
    }
    ObjectListC ol(obj.Top());
    bool usetypedata = true;
    if(typedata == "scope" || typedata == "scope_user" ||typedata == "all")
      usetypedata = false;
      
    if(typedata == "method" || typedata == "method_user" || 
       typedata == "scope" || typedata == "scope_user") { // Go through all methods ?
      bool localOnly = false;
      StringC tmp;
      if(Lookup("localScopeOnly",tmp)) {
	if(tmp == "1")
	  localOnly = true;
      }
      //cerr << "Doing inherit iter...  for " << ol.Name() << "\n";
      // Which methods are we interested in ?
      ScopeAccessT useAccess = SAProtected;
      if(typedata == "method_user" || typedata == "scope_user")
	useAccess = SAPublic;
      StringC methodtxt("method");
      // Got through objects.
      for(InheritIterC it(ol,useAccess,localOnly);it;it++) {
	if(usetypedata)
	  if(methodtxt != it->TypeName())
	    continue;    // If not a method, continue on.
	obj.Push(*it); // Change context to method.
	if(!condition.IsEmpty()) {
	  if(!CheckCondition(condition)) {
	    obj.DelTop(); 
	    continue; // Condition test failed.
	  }
	}
	if(MethodC::IsA(*it)) {
	  MethodC meth(*it);
	  if(meth.IsConstructor()) {
	    if(it.CurrentScope() != ol) {
	      obj.DelTop(); 
	      continue; // Ignore constructors from other scopes.
	    }
	  }
	}
	templArgSub.Push(it.TemplateSubs());
	if(!ifAny) {
	  // Process template for method.
	  BuildSub(subTextBuff);
	  templArgSub.DelTop();
	} else {
	  obj.DelTop(); // Only needed for if condition.
	  // Found an element so execute ifany..
	  BuildSub(subTextBuff);
	  templArgSub.DelTop();
	  lastIf = true;
	  return true;
	}
	obj.DelTop();
      }
    } else {
      bool isClass = (typedata == "class");
      bool isFirst = true;
      for(DLIterC<ObjectC> it(ol.List());it.IsElm();it.Next()) {
	// Filter out the wanted objects...
	if(ScopeC::IsA(it.Data())) {
	  if(isClass && !inClassScope) { // Don't go into nested classes.
	    // Ignore everything that isn't a proper definition...
	    StringC decType = it.Data().Var("dectype");
	    if(decType == "forward")
	      continue;
	    // Deal with namespace. Could be class or namespace.
	    obj.Push(it.Data());
	    bool ret = Foralli(data,ifAny,true);
	    obj.DelTop();
	    if(ifAny && ret)
	      return true;
	  }
	}
	if(usetypedata)
	  if(typedata != it.Data().TypeName())
	    continue;
	obj.Push(it.Data());
	if(!condition.IsEmpty()) {
	  if(!CheckCondition(condition)) {
	    obj.DelTop();
	    continue; // Condition test failed.
	  }
	}
	if(!ifAny) {
	  // Flag is its the first in the list.
	  // Is there a faster way ?
	  if(isFirst) {
	    it.Data().SetVar("isfirst","1");
	    isFirst = false;
	  } else
	    it.Data().SetVar("isfirst","0");
	  BuildSub(subTextBuff);
	} else {
	  obj.DelTop(); // Just need the context for 'CheckCondition'
	  // Found an element so execute ifany..
	  BuildSub(subTextBuff);
	  lastIf=true;
	  return true;
	}
	obj.DelTop();

      }
    }
    if(ifAny)
      return false;
    return true;
  }
  
  //: Insert a DocNodeC into the documentation tree.
  
  bool DocumentBodyC::InsertDocNode(StringC &node) {
    if(!docTree.IsValid())
      return false;
    node = Interpret(node);
    StringC userlevel ,brief,thisFilename,nodeType;
    Lookup("userlevel",userlevel);
    Lookup("thisFilename",thisFilename);
    Lookup("NodeType",nodeType);
    
    brief = obj.Top().Comment().Header();
    ONDEBUG(std::cerr << "DocumentBodyC::InsertDocNode(), Called for : '" << node << "' File:'" << thisFilename <<"'\n");
    docTree.InsertDocLeaf(node,obj.Top().Name(),userlevel,brief,thisFilename,nodeType.Copy());
    
    return true;
  }
  
  //: Automaticly put links in some text.
  
  bool DocumentBodyC::AutoLink(StringC &rawtext) {    
    StringC text = Interpret(rawtext);    
    StringC linkModeTxt,linkContext;
    int linkMode = -1;
    ScopeC parentScope;
    
    // See if the context is set explicity.
    
    if(Lookup("autoLinkContext",linkContext)) {
      std::cerr << "Found AutoLink Context '" << linkContext <<"' specification. \n";
      RavlAssert(root.IsValid());
      RavlAssert(ScopeC::IsA(root));
      parentScope = ScopeC(root);
      ObjectC nscope = parentScope.ResolveName(linkContext);
      if(nscope.IsValid()) {
	if(ScopeC::IsA(nscope)) {
	  parentScope = ScopeC(nscope);
	  std::cerr << "Found AutoLink scope " << parentScope.Name() << "\n";
	}
      }
    }
    
    // Setup a default context.
    
    if(!parentScope.IsValid()) {
      if(obj.Top().HasParentScope())
	parentScope = ScopeC(obj.Top().ParentScope());
      else {
	if(root.IsValid() && ScopeC::IsA(root))
	  parentScope = ScopeC(root);
      }
    }
    
    if(!parentScope.IsValid()) {
      std::cerr << "DocumentBodyC::AutoLink(), No parent scope for object. Root=" << root.Name() << "\n";
      Output() << text;
      return false;
    }
    
    if(Lookup("autoLink",linkModeTxt)) {
      if(linkModeTxt == "on")
	linkMode = 1;
      if(linkModeTxt == "marked")
	linkMode = 2;
      if(linkModeTxt == "off")
	linkMode = 0;
      if(linkMode == -1)
	cerr << "DocumentBodyC::AutoLink(), Unknown link mode '" << linkModeTxt << "' \n";
    } else
      linkMode = 2;
    
    ONDEBUG(std::cerr << "DocumentBodyC::AutoLink(), Called, Mode: " << linkMode << " Data:'" << text <<"' \n");
    int namespaceState = 0;
    if(linkMode == 1) {
      // Full auto link.
      const char *place = text.chars();
      const char *eos = &place[text.length()];
      //eos++;
      const char *lw;
      bool inAnchor = false;
      for(;place != eos;) {
	// Sperl -pi -e 's/bar/baz/' fileAkip spaces and other punctuation.
	lw = place;
	for(;!(isalnum(*place) || *place == '_' || *place == ':' || *place == '<') && place != eos;place++) ;
	int len = (place - lw);
	Output().write(lw,len);
	if(place == eos)
	  break; // got to end of text.
	// Find a word.
 	lw = place;
	if(*place == '<') { // Skip html markup.
	  for(;(*place != '>') && place != eos;place++) ;
	  if(place != eos)
	    place++;
	  len = (place - lw);
	  StringC word(lw,len,len);
	  // Don't look at text inside links....
	  if(word.matches("<a") || word.matches("<A"))
	    inAnchor = true;
	  if(word.matches("</a") || word.matches("</A"))
	    inAnchor = false;
	  ONDEBUG(std::cerr << "Got markup:" << word << "\n");
	  Output() << word;
	  continue;
	}
	for(;(isalnum(*place) || *place == '_' || *place == ':') && place != eos;place++) ;
	len = (place - lw);
	StringC word(lw,len,len);
	ONDEBUG(std::cerr << "Found word :'" << word << "'\n");
	if(isdigit(word.firstchar()) || inAnchor) {
	  Output() << word;
	  continue;
	}
	//////////////////////////////////
	// State machine to find 'using namespace xyz' in
	// text.  Use as a hint about where to look for symbols.
	
	switch(namespaceState)
	  {
	  case 0:
	    if(word == "using")
	      namespaceState = 1;
	    break;
	  case 1:
	    if(word == "namespace")
	      namespaceState = 2;
	    else
	      namespaceState = 0;
	    break;
	  case 2:
	    {
	      namespaceState = 0;
	      ObjectC nscope = parentScope.ResolveName(word);
	      //cerr << "Found namespace '" << word << "' " << nscope.IsValid() <<"\n";
	      if(!nscope.IsValid())
		break;
	      if(!ScopeC::IsA(nscope)) 
		break;
	      parentScope = ScopeC(nscope);
	      namespaceState = 3; // Don't look for anymore.
	    }
	    break;
	  case 3: // Do nothing.
	    break;
	  };
	if(ignoreWords[word]) { // Ingore native type names.
	  Output() << word;
	  continue;
	}
	ObjectC dobj = parentScope.ResolveName(word);
	if(!dobj.IsValid()) {
	  Output() << word;
	  continue; // Nothing found.
	}
	obj.Push(dobj);
	Output() << TextFor(dobj);
	obj.DelTop();
      }
      return true;
    }
    if(linkMode == 2) {
      int at = 0,lat = 0,cat = 0;
      while(1) {
	lat = at;
	at = text.index('[',lat);
	if(at < 0) {
	  Output() << text.from(lat);
	  return true; // Done.
	}
	cat = text.index(']',at);
	if(cat < 0) {
	  Output() << text.from(lat);
	  return true; // No close, so done.
	}
	at++;
	Output() << text.at(lat,at - lat); // Output text upto '['	
	StringC word = text.at(at,cat - at);
	ONDEBUG(std::cerr << "Found word :'" << word << "'\n");
	ObjectC dobj = parentScope.ResolveName(word);
	if(dobj.IsValid())
	  Output() << TextFor(dobj);
	else
	  Output() << word;
	at = cat;
      }
      return true;
    }
    Output() << text;
    return true;
  }
  
  
}

