// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLCXXDOC_DOCUMENT_HEADER
#define RAVLCXXDOC_DOCUMENT_HEADER 1
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/SourceTools/CxxDoc/Document.hh"
//! userlevel=Default
//! lib=RavlCxxDoc
//! docentry="Ravl.API.Source Tools.CxxDoc.Internal"
//! author="Charles Galambos"
//! date="03/02/2000"

#include "Ravl/Text/TemplateComplex.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Hash.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/HSet.hh"
#include "Ravl/CxxDoc/Object.hh"
#include "Ravl/CxxDoc/DocTree.hh"
#include "Ravl/OS/Filename.hh"

//: C++ parser namespace.
// This namespace contains the classes that make up the parser, the parse tree 
// and the C++ documenation system.

namespace RavlCxxDocN {

  using namespace RavlN;
  
  class DataTypeC ; 
  
  //! userlevel=Develop
  //: Documenter Body.
  
  class DocumentBodyC 
    : public TemplateComplexBodyC,
      public DesciptionGeneratorC
  {
  public:
    DocumentBodyC(const FilenameC &tmplName,const StringC &outDir,const DocTreeC &docTree = DocTreeC(),const StringC &projName = StringC(),const StringC &projDesc = StringC());
    //: Default contructor.
    
    bool Foralli(StringC &data,bool ifAny = false,bool inClassScope = false);
    //: Do a forall build.

    bool Forall(StringC &data)
    { return Foralli(data); }
    //: Do a forall build.
    
    bool IfAny(StringC &data)
    { return Foralli(data,true); }
    //: Do a forall build.
    
    bool GotoInherit(StringC &param);
    //: Goto an inherited class.
    
    bool MakeFilenameCmd(StringC &pattern);
    //: Make a filename from a pattern and a full object name.
    
    bool InsertDocNode(StringC &node);
    //: Insert a DocNodeC into the documentation tree.

    bool AutoLink(StringC &text);
    //: Automaticly put links in some text.

    bool Exec(StringC &text);
    //: Execute a child process.
    
    bool ForDir(StringC &text);
    //: Go through files in a directory.
    
    void Document(ObjectListC &ol);
    //: Document list of files.

    FilenameC MakeFilename(ObjectC &obj,bool relative = false);
    //: Makefile name
    
    FilenameC MakeFilename(StringC pattern,ObjectC &obj,bool relative = false);
    //: Make a filename from a pattern and a full object name.
    
    bool HtmlMethodName(StringC &arg);
    //: Make a linked version of the method name.
    
    StringC GetHtmlTypeName(const DataTypeC &tn);
    //: Make a linked version of the method name.
    
    bool HtmlTypeName(StringC &arg);
    //: Make a linked version of the method name.
    
    StringC MakeHRef(StringC name);
    //: Make a string suitable for an href.
        
    virtual bool Lookup(const StringC &varname,StringC &buff);
    //: Lookup variable.
    // if found put value into 'buff' and return true.
    // otherwise return false.
    
    DocTreeC &DocTree()
    { return docTree; }
    //: Access doc tree.
    
  protected:
    void Init();
    //: Get initalise information from template file.

    void InitVars();
    //: Init known vars list.
    
    virtual StringC TextFor(const ObjectC &obj);
    //: How to render an object into html.
    
    virtual StringC TextFor(char let);
    //: How to render an char into html.
    
    virtual StringC TextFor(const StringC &obj);
    //: How to render a string in html.

    virtual StringC MethodNameText(const StringC &obj);
    //: Render a method name appropriatly. (e.g. bold.)
    
    StringC TemplateNameSubst(const StringC &x);
    //: If there's a substituation use it.
    
    StringC outputDir;
    StringC fileObject;
    StringC filePattern;
    StringC refPattern;
    StackC<ObjectC> obj;
    StackC<RCHashC<StringC,ObjectC> > templArgSub; // Template argument subsitutions.
    HSetC<StringC> knownVars;
    DocTreeC docTree;
    ObjectListC root;
    bool verbose;
  };
  
  //! userlevel=Normal
  //: Documentor
  
  class DocumentC 
    : public TemplateComplexC
  { 
  public:
    DocumentC()
    {}
    //: Default contructor.
    // Creates an invalid handle.
    
    DocumentC(const FilenameC &tmplName,const StringC &outDir,const DocTreeC &docTree = DocTreeC(),const StringC &projName = StringC(),const StringC &projDesc = StringC())
      : TemplateComplexC(*new DocumentBodyC(tmplName,outDir,docTree,projName,projDesc))
    {}
    //: Contructor.
    // Create a documentor
    
  protected:
    
    DocumentBodyC &Body() 
    { return  static_cast<DocumentBodyC &>(TemplateComplexC::Body()); }
    //: Access body.
    
    const DocumentBodyC &Body() const
    { return  static_cast<const DocumentBodyC &>(TemplateComplexC::Body()); }
    //: Access body.
  public:
    void Document(ObjectListC &ol)
    { Body().Document(ol); }
    //: Document list of files.
    
    DocTreeC &DocTree()
    { return Body().DocTree(); }
    //: Access doc tree.
  
  };

}
#endif
