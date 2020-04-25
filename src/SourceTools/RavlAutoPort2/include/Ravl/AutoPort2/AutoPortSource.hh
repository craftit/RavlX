// This file is part of VCPP, Visual C++ porting tool for QMake projects 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_SOURCEAUTOPORT_HEADER
#define RAVL_SOURCEAUTOPORT_HEADER 1
/////////////////////////////////////////////////////////
//! rcsid="$Id: AutoPortSource.hh 5298 2006-01-06 11:56:18Z kier $"
//! docentry="Ravl.API.Source Tools.AutoPort"
//! userlevel=Normal
//! file="Ravl/SourceTools/VisualCPP/AutoPortSource.hh"
//! lib=RavlAutoPort2

#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/SourceTools/LibInfo.hh"
#include "Ravl/SourceTools/ProgInfo.hh"
#include "Ravl/HSet.hh"

namespace RavlN {
  
  class AutoPortSourceC;
  
  //! userlevel=Develop
  //: Code Porter Body
  
  class AutoPortSourceBodyC 
    : public RCBodyVC
  {
  public:
    AutoPortSourceBodyC(StringC &where, DListC<StringC> & doLibs,const HSetC<StringC> &resources);
    //: Constructor.
    
    bool ScanTree(StringC &where);
    //: Scan a tree for info.
    
    bool ScanDirectory(StringC &where,DefsMkFileC &defs);
    //: Scan the contents of the directory.
    
    bool Unix2Dos(const StringC &src,const StringC &vcpp);
    //: Update a file.
    // vcpp, is the file in the Visual C++ tree.
    // src, is the unix version. <p>
    
    bool Dos2Unix(const StringC &src,const StringC &vcpp);
    //: Update a file.
    // vcpp, is the file in the Visual C++ tree.
    // src, is the unix version. <p>
    
    void SetVerbose(bool val)
    { verbose = val; }
    //: Set verbose flag.
    
    HashC<StringC,LibInfoC> &Libs()
    { return libs; }
    //: Access table of libraries.

    DListC<ProgInfoC> &Mains()
    { return mains; }
    //: Main programs in tree.
    
    DListC<ProgInfoC> &Tests()
    { return tests; }
    //: Test programs.
    
    DListC<ProgInfoC> &Examples()
    { return examples; }
    //: Examples programs.
    
    HashC<StringC, DListC<StringC> > & Deps()
    { return deps; }
    //: Get lib dependencies
    
    void Dump();
    //: Dump contents of db to stdout.
  protected:
    bool verbose;
    
    HashC<StringC,LibInfoC> libs;
    DListC<ProgInfoC> mains;
    DListC<ProgInfoC> tests;
    DListC<ProgInfoC> examples;
    HashC<StringC, DListC<StringC> > deps;
    DListC<StringC> doLibs;
    HSetC<StringC> m_resources;
    friend class AutoPortSourceC;
  };
  
  //! userlevel=Normal
  //: Code Porting tool.
  
  class AutoPortSourceC 
    : public RCHandleC<AutoPortSourceBodyC>
  {
  public:
    AutoPortSourceC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    AutoPortSourceC(StringC &where, DListC<StringC> & doLibs,const HSetC<StringC> &resources)
      : RCHandleC<AutoPortSourceBodyC>(*new AutoPortSourceBodyC(where, doLibs,resources))
    { Body().ScanTree(where); }
    //: Constructor.
    // scan's the source tree 'where' for info.
    
  protected:
    AutoPortSourceC(AutoPortSourceBodyC &bod)
      : RCHandleC<AutoPortSourceBodyC>(bod)
    {}
    //: Body constructor.
    
    AutoPortSourceBodyC &Body()
    { return RCHandleC<AutoPortSourceBodyC>::Body(); }
    //: Access body.
    
    const AutoPortSourceBodyC &Body() const
    { return RCHandleC<AutoPortSourceBodyC>::Body(); }
    //: Access body.
    
  public:
    bool ScanDirectory(StringC &where,DefsMkFileC &defs)
    { return Body().ScanDirectory(where,defs); }
    //: Scan the contents of the directory.

    void SetVerbose(bool val)
    { Body().SetVerbose(val); }
    //: Set verbose flag.

    HashC<StringC,LibInfoC> &Libs()
    { return Body().Libs(); }
    //: Access table of libraries.

    DListC<ProgInfoC> &Mains()
    { return Body().Mains(); }
    //: Main programs in tree.
    
    DListC<ProgInfoC> &Tests()
    { return Body().Tests(); }
    //: Test programs.
    
    DListC<ProgInfoC> &Examples()
    { return Body().Examples(); }
    //: Examples programs.

    HashC<StringC, DListC<StringC> > & Deps()
    { return Body().Deps(); }
    //: Get lib dependencies

    void Dump()
    { return Body().Dump(); }
    //: Dump contents of db to stdout.
    
    friend class AutoPortSourceBodyC;
  };
  
}

#endif
