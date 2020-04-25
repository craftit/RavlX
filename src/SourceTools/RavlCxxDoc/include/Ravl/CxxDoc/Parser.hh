// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVLCXXDOC_PARSER_HEADER
#define RAVLCXXDOC_PARSER_HEADER 1
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! file="Ravl/SourceTools/CxxDoc/Parser.hh"
//! userlevel=Normal
//! docentry="Ravl.API.Source Tools.CxxDoc.Internal"
//! lib=RavlCxxDoc
//! author="Charles Galambos"
//! date="06/12/1999"

#include "Ravl/String.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/CxxDoc/Object.hh"
#include "Ravl/CxxDoc/CxxScope.hh"

class FlexLexer;

namespace RavlCxxDocN
{
  using namespace RavlN;

#if RAVL_VISUALCPP_NAMESPACE_BUG  
  using RavlN::RCBodyC;
  using RavlN::FilenameC;
  using RavlN::RCHandleC;
#endif
  
  //! userlevel=Develop
  //: C++ File Parser
  
  class ParserBodyC
    : public RCBodyC
  {
  public:
    ParserBodyC();
    //: Constructor.

    ~ParserBodyC();
    //: Destructor.
    
    bool Parse(const FilenameC &fn);
    //: Parse a file.

    bool ParseTree(const FilenameC &fn);
    //: Parse a directory.
    
    int NextToken();
    //: Get the next token;

    int LineNo();
    //: Get current lineno
    
    const FilenameC &CurrentFilename() const { return curFilename; }
    //: Get current filename.
    
    void Dump(std::ostream &out);
    //: Dump database to 'out'
    
    ObjectListC &Data()
    { return data; }
    //: Access parsed data.

    bool Resolve();
    //: Resolve references in input data.
    
    bool SetRootFilename(const StringC &filename)
    { rootFilename = filename; return true; }
    //: Set the root filename.
    
  protected:
    bool ParseOuter();
    //: Do top level parse.
    
    FlexLexer *lex; // Ptr because its really an external class.
    
    FilenameC curFilename;
    FilenameC rootFilename; // Directory where the search started.
    ScopeC data;
  };
  
  //! userlevel=Normal
  //: C++ File Parser
  
  class ParserC
    : public RCHandleC<ParserBodyC>
  {
  public:
    ParserC()
    {}
    //: Default constructor.
    
    explicit ParserC(bool)
      : RCHandleC<ParserBodyC>(*new ParserBodyC())
    {}
    //: Constructor.  
    
    bool Parse(const FilenameC &fn)
    { return Body().Parse(fn); }
    //: Parse a file.
    
    bool ParseTree(const FilenameC &fn)
    { return Body().ParseTree(fn); }
    //: Parse a directory.

    void Dump(std::ostream &out)
    { Body().Dump(out); }
    //: Dump database to 'out'
    
    ObjectListC &Data()
    { return Body().Data(); }
    //: Access parsed data.
    
    bool Resolve()
    { return Body().Resolve(); }
    //: Resolve references in input data.
    
    bool SetRootFilename(const StringC &filename)
    { return Body().SetRootFilename(filename); }
    //: Set root filename.
  };
  
}


#endif


