// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_AUTOPORTGENERATOR_HEADER
#define RAVL_AUTOPORTGENERATOR_HEADER 1
///////////////////////////////////////////////////////
//! docentry="Ravl.API.Source Tools.AutoPort"
//! rcsid="$Id: AutoPortGenerator.hh 5509 2006-06-01 15:30:48Z craftit $"
//! file="Ravl/SourceTools/VisualCPP/AutoPortGenerator.hh"
//! lib=RavlAutoPort2

#include "Ravl/AutoPort2/AutoPortSource.hh"
#include "Ravl/Text/TemplateComplex.hh"
#include "Ravl/AutoPort2/ExtLibTable.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Auto port engine
  
  class AutoPortGeneratorBodyC 
    : public TemplateComplexBodyC
  {
  public:
    AutoPortGeneratorBodyC(AutoPortSourceC &src,StringC &templ,StringC &output,const StringC &projectOut, const ExtLibTableC & extLibs);
    //: Constructor.
    
    bool BuildFiles();
    //: Build output files.
    
  protected:
    void Init();
    //: Setup template stuff.
    
    bool DoForall(StringC &data);
    //: For all template function.
    
    bool DoIfAny(StringC &data);
    //: IfAny template function
    
    bool DoIfFileExists(StringC &data);
    //: Execute code if file exists.
    
    bool DoDos(StringC &data);
    //: Generate a DOS filename from a unix one
    
    bool DoPathBack(StringC &data);
    //: Generate path back from a file.
    // So 'MyDir1/MySubDir/filename' will become '../../'
    
    bool DoShell(StringC &data);
    //: Execute a shell command and include stdout.
    
    bool DoTopAndTail(StringC &data);
    //: Remove white space from beging and end of string.
    
    bool DoLibDepends(StringC &data); 
    //: Go through all dependant libraries.

    bool HelpLibDepends(StringC &data,const StringC &library,HSetC<StringC> &done); 
    //: Helper functionf or DoLibDepends.
    
    virtual bool Lookup(const StringC &varname,StringC &buff);
    //: Lookup variable.
    // if found put value into 'buff' and return true.
    // otherwise return false.
    
    StringC MakeFilename(const StringC &obj);
    //: Make a file name for an object using filePattern.
    
    StringC &CurFilename() { 
      RavlAssert(!context.IsEmpty());
      //cerr << "CurFilename() = " << context.Top().hdrInfo.SrcFile() << "\n";
      return context.Top().hdrInfo.SrcFile(); 
    }
    //: Access info about current filename.

    StringC &CurName() { 
      RavlAssert(!context.IsEmpty());
      //cerr << "CurFilename() = " << context.Top().hdrInfo.SrcFile() << "\n";
      return context.Top().hdrInfo.Name(); 
    }
    //: Access info about current filename.
    
    LibInfoC &CurLibInfo() {
      RavlAssert(!context.IsEmpty());
      return context.Top().libInfo;
    }
    //: Access info about current lib.

    ProgInfoC &CurProgInfo() {
      RavlAssert(!context.IsEmpty());
      return context.Top().progInfo;
    }
    //: Access info about current lib.
    
    StringC VarExtraInclude();
    //: Compute value of extra inclue variable.
    
    StringC VarExtraLibPath();
    //: Compute value of extra library paths
    
    StringC VarExtraLibs();
    //: Compute value of extra libs
    
    bool BuildExtraLibs(const StringC &libName,HSetC<StringC> &done,StringC &result,bool doLibPaths);
    //: Build set of extra libraries needed.
    //!param: libName - Name of library to examine.
    //!param: done - Libraries already done.
    //!param: result - Resulting include string.
    //!param: doLibPaths - Generate list of paths instead
    
    class ContextC {
    public:
      ContextC(const LibInfoC &li)
	: libInfo(li),
	  hdrInfo(li.Name()),
	  ctype(1)
      {}
      //: Constructor.

      ContextC(const ProgInfoC &pi)
	: progInfo(pi),
	  hdrInfo(pi.Name(),StringC(),pi.MainSource()),
	  ctype(2)
      {}
      //: Constructor.
      
      ContextC(const StringC &s)
	: hdrInfo(s,StringC(),s),
	  ctype(0)
      {}
      //: Constructor.

      ContextC(const HeaderInfoC &hi)
	: hdrInfo(hi),
	  ctype(0)
      {}
      //: Constructor.

      StringC Name() {
	switch(ctype) {
	case 1: return libInfo.Name();
	case 2: return progInfo.Name();
	}
	return StringC();
      }
      
      DListC<StringC> Sources() {
	switch(ctype) {
	case 1: {
	  DListC<StringC>srcs = libInfo.Sources();
	  if(libInfo.MustLink().Size()>0 && !srcs.Contains(libInfo.MustLink()))
            srcs.InsLast(libInfo.MustLink());
	  return srcs;
	}
	case 2: return progInfo.Sources();
	}
	return DListC<StringC> ();
      }
      //: Access source list.
      
      DListC<StringC> UsesLibs() {
	switch(ctype) {
	case 1: 
	  if(!libInfo.IsValid()) return DListC<StringC>();
	  return libInfo.UsesLibs();
	case 2: 
	  if(!progInfo.IsValid()) return DListC<StringC>();
	  return progInfo.UsesLibs();
	}
	return DListC<StringC>();
      }
      //: Access useslibs list.
      
      DListC<HeaderInfoC> Headers() {
	switch(ctype) {
	case 1: return libInfo.Headers();
	  //case 2: return progInfo.Headers();
	}
	return DListC<HeaderInfoC> ();
      }
      //: Access source list.
      
      HeaderInfoC &HeaderInfo() {
	return hdrInfo;
      }
      //: Access header info.
      
      StringC MustLink() const  {
        if(!libInfo.IsValid()) return StringC();
        return libInfo.MustLink(); 
      }
      //: Access must link.
      
      DListC<StringC> AuxFiles()
      { return libInfo.AuxFiles(); }
      //: Access auxfiles.
      
      StringC &AuxDir()
      { return libInfo.AuxDir(); }
      //: Access aux dir.
      
      LibInfoC libInfo;
      ProgInfoC progInfo;
      HeaderInfoC hdrInfo;
      int ctype; // 0-file 1-lib 2-Prog 
    };
    
    StackC<ContextC> context;
    StringC target; // Current target.
    StringC filePattern;
    StringC fileObject;
    StringC outputDir;
    StringC projectOut;
    StringC outputfile;
    ExtLibTableC extLibs; // Info about external libraries
    
    AutoPortSourceC src; // Info about source.
  };
  
  //! userlevel=Normal
  //: Auto port engine
  
  class AutoPortGeneratorC 
    : public TemplateComplexC
  {
  public:
    AutoPortGeneratorC()
    {}
    //: Default constructor.
    // creates an invalid handle.

    AutoPortGeneratorC(AutoPortSourceC &src,StringC &templ,StringC &output,const StringC &projectOut, const ExtLibTableC & extLibs)
      : TemplateComplexC(*new AutoPortGeneratorBodyC(src,templ,output,projectOut, extLibs))
    {}
    //: Constructor.
    
  protected:
    AutoPortGeneratorBodyC &Body()
    { return static_cast<AutoPortGeneratorBodyC &>(TemplateComplexC::Body()); }
    //: Access body.

    const AutoPortGeneratorBodyC &Body() const
    { return static_cast<const AutoPortGeneratorBodyC &>(TemplateComplexC::Body()); }
    //: Access body.
    
  public:

    bool BuildFiles()
    { return Body().BuildFiles(); }
    //: Build output files.
    
  };

}
#endif
