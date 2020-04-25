// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_CLASSWIZARD_HEADER
#define RAVL_CLASSWIZARD_HEADER 1
//! rcsid="$Id$"
//! lib=RavlClassWizard
//! author="Charles Galambos"
//! docentry="Ravl.API.Source Tools.Class Wizard"
//! file="Ravl/Applications/ClassWizard/ClassWizard.hh"

#include "Ravl/CxxDoc/Parser.hh"
#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/Text/TextBuffer.hh"
#include "Ravl/SourceTools/SourceCursor.hh"
#include   "Ravl/CxxDoc/CxxElements.hh"

namespace RavlN {
  using namespace RavlCxxDocN;

#if 0
  //! userlevel=develop
  //: Wooping class
  // To test auto handle generation.
  
  class WoopBodyC
    : public RCBodyVC
  {
  public:
    WoopBodyC(int nn)
      : n(nn)
    {}
    //: Constructor.
    
    ~WoopBodyC();
    //: Destructor
    
    IntT N() const
    { return n; }
    //: Get n

  protected:
    int n;
  };
#endif
  
  //! userlevel=Develop
  //: Class wizard.
  
  class ClassWizardBodyC
    : public RCBodyC
  {
  public:
    ClassWizardBodyC(const StringC &rootDir,const StringC &localDir,bool verbose,bool writeStubs = false,bool dryrun = false);
    //: Constructor.
    
#if 0
    ClassWizardBodyC(bool xyz);
    //: Constructor.
#endif
    
    bool GatherInfo();
    //: Gather information about the source code.
    
    bool GatherDir(StringC &dirname,DefsMkFileC &defsmk);
    //: Gather info from a directory.
    //!param:dirname - Current directory
    //!param:defsmk - Access class for defs.mk in the current directory.
    //!return:True if directory has been processed successfully.
    
    bool GatherFile(StringC &filename);
    //: Gather info from file.
    //!param:filename - file to scan for class information
    //!return:true if file has been processed successfully.
    
    bool ApplyClass(ScopeC &scope,ObjectC &classObj);
    //: Apply to a directory.
    
    bool ApplyWizard();
    //: Go through and update local source files.
    
    bool ScanScope(ScopeC &scope);
    //: Scan scope
    
    bool WriteHandleClass(SourceCursorC &sc,ObjectC &bodyObj,const StringC &className);
    //: Generate text for handle class.
    
    bool WriteComment(SourceCursorC &sc,ObjectC &obj,bool markAsAuto = true);
    //: Write comment into source.
    
    bool WriteHandleMethod(SourceCursorC &sc,ObjectC &obj);
    //: Write the handle method for the given method.
    
    bool WriteHandleConstructor(SourceCursorC &sc,ObjectC &obj,const StringC &handleBaseClass);
    //: Write the handle constructor.
    
    bool WriteStubsFile(ObjectC &obj);
    //: Write stubs file.
    
    void AddLine(TextBufferC &buff,int indent,const StringC &info);
    //: Add line to buffer
    
    void SetModifiedPrefix(const StringC &value)
    { modifiedPrefix = value; }
    //: Set the prefix to use for modified files.
    
    void DumpParseTree(ostream &out);
    //: Dump the parse tree.
    
  protected:
    IntT FindPublicSection(ObjectC &classObj);
    //: Find public section in classObj
    
    TextFileC TextFile(const StringC &filename);
    //: Get a text file for editing.
    
    MethodC GenerateHandleConstructor(ObjectC &bodyConstructor);
    //: Generate a handle constructor prototype from its body counterpart
    
    MethodC GenerateBodyConstructor(ObjectC &handleConstructor);
    //: Generate a body constructor prototype from its handle counterpart
    
    StringC BaseClassName(ObjectC &handleClass);
    //: Find name of base class.
    
    DListC<StringC> BaseClassNames(ObjectC &handleClass);
    //: Generate a list of base class names.
    
    StringC rootDir; // Root of source tree in which we're working.
    StringC localDir;  // Directory where we want to make changes.
    
    ParserC parseTree; // Handle to parse tree of local source.
    
    DesciptionGeneratorC descGen;// The default generator is fine..
    
    HashC<StringC,TextFileC> textFiles; // Table of open files.
    
    StringC modifiedPrefix; // Prefix to use for modified files.
    
    HSetC<StringC> editList; // List of files we're allowed to edit, empty = All.
    
    bool verbose;
    bool writeStubs;
    bool dryRun;
  };

  //! userlevel=Normal
  //: Class wizard.
  
  class ClassWizardC
    : public RCHandleC<ClassWizardBodyC>
  {
  public:
    ClassWizardC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    ClassWizardC(const StringC &rootDir,const StringC &localDir,bool verbose,bool writeStubs = false,bool dryrun = false)
      : RCHandleC<ClassWizardBodyC>(*new ClassWizardBodyC(rootDir,localDir,verbose,writeStubs,dryrun))
    {}
    //: Constructor.

#if 0
    ClassWizardC(int xyz)
    {}
    //: Dummy constructor.
    // Stuff that should be removed
    //!author:cwiz
    
#endif
    
    bool GatherDir(StringC &dirname,DefsMkFileC &defsmk)
    { return Body().GatherDir(dirname,defsmk); }
    //: Gather info from a directory.
    
    bool GatherInfo()
    { return Body().GatherInfo(); }
    //: Gather information about the source code.
    
    bool ApplyWizard()
    { return Body().ApplyWizard(); }
    //: Go through and update local source files.
    // Fred
    
    void SetModifiedPrefix(const StringC &value)
    { Body().SetModifiedPrefix(value); }
    //: Set the prefix to use for modified files.
    
#if 0
    void SomeDross()
    { Body().SomeDross(); }
    //: Old stuff
    // to test handle method removal
    //!author:cwiz
    
#endif
    
    void DumpParseTree(ostream &out) 
    { Body().DumpParseTree(out);  }
    //: Dump the parse tree.
    
  protected:
    ClassWizardC(ClassWizardBodyC &bod)
      : RCHandleC<ClassWizardBodyC>(bod)
    {}
    //: Body constructor.

    friend class ClassWizardBodyC;
  };
}



#endif
