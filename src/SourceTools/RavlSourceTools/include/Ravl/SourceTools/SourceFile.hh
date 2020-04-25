// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#ifndef RAVL_SOURCEFILE_HEADER
#define RAVL_SOURCEFILE_HEADER 1
///////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"  
//! docentry="Ravl.API.Source Tools.Code Manager"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/SourceFile.hh"

#include "Ravl/Text/TextFile.hh"
#include "Ravl/Hash.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Source File Body

  class SourceFileBodyC
    : public TextFileBodyC
  {
  public:
    
    SourceFileBodyC();
    //: Default constructor.

    SourceFileBodyC(const StringC &filename);
    //: Constructor from a file.
    
    SourceFileBodyC(const StringC &text,bool noFinalReturn,bool concat = false);
    //: Construct from a string. 
    // If 'noFinalReturn' is false, ensure each line ends with '\n', otherwise leave it alone. <p>
    // concat, if true don't split up lines,  otherwise do.
    
    SourceFileBodyC(std::istream &fin);
    //: Constructor from a stream.

    bool Load(const StringC &filename);
    //: Load from file.
    
    bool CheckDocVar(const StringC &var,const StringC &value,bool force = false);
    //: Check variable exists in hdr file.
    // if 'force' is true then update variable regardless if is there 
    // already or not.
    
    bool CheckDocVarSub(const StringC &var,const StringC &subs,const StringC &value,bool force = false);
    //: Check variable exists in hdr file.
    // if 'force' is true then update variable regardless if is there 
    // already or not.
    
    StringC GetDocValue(const StringC &var);
    //: Find a return a documentation value from the buffer.
    // Returns empty string if not found.
    
    bool CheckHeader(const TextBufferC &hdr,const StringC &eoh,const StringC &name,const StringC &desc,const StringC &org,bool replaceExisting = false);
    //: Check a header is on file.
    
    bool GetDocVars(HashC<StringC,StringC> &vars);
    //: Retrieve all documentation variables from the file.
    
    StringC &FileType()
    { return fileType; }
    //: Access file type.
    
    const StringC &FileType() const
    { return fileType; }
    //: Access file type.
    
    bool LeadingComment(StringC &title,StringC &detail);
    //: Extract leading comment from source file 
    // if there is one...
    
  protected:
    StringC IdFile();
    //: Id the current file.
    
    bool InsertVar(const StringC &line);
    //: Insert variable line in file at an appropriate place.
		    
    StringC fileType; 
    // "Unknown"=Completely unknown.
    // "Text"=Unknown text file.
    // "C"=C source.
    // "CH"=C header file.
    // "C++"=C++ source
    // "C++H"=C++ Header
    // "Make"=Make file.
    // "Java"=Java code.
    // "Perl"=Perl code.
    // "Lex"=Lex or Flex file.
    // "Yacc"=Yacc or Bison file.
    
    static HashC<StringC,StringC> extToType;
    static HashC<StringC,StringC> commentString;
    static HashC<StringC,StringC> commentEndString;
  };

  //! userlevel=Develop
  //: Source File Body
  
  class SourceFileC
    : public TextFileC
  {
  public:
    SourceFileC()
    {}
    //: Default constructor.
    // Creates an in valid handle.
    
    explicit SourceFileC(bool)
      : TextFileC(*new SourceFileBodyC())
    {}
    //: Default constructor.
    // Creates an in valid handle.
    
    SourceFileC(const StringC &filename)
      : TextFileC(*new SourceFileBodyC(filename))
    {}
    //: Constructor from a file.
    
    SourceFileC(const StringC &text,bool noFinalReturn,bool concat = false)
      : TextFileC(*new SourceFileBodyC(text,noFinalReturn,concat))
    {}
    //: Construct from a string.
    // If 'noFinalReturn' is false, ensure each line ends with '\n', otherwise leave it alone. <p>
    // concat, if true don't split lines at \n's, otherwise do.
    
    SourceFileC(std::istream &fin)
      : TextFileC(*new SourceFileBodyC(fin))
    {}
    //: Constructor from a stream.
    
  protected:
    SourceFileC(SourceFileBodyC &bod)
      : TextFileC(bod)
    {}
    //: Body constructor.
    
    SourceFileBodyC &Body()
    { return static_cast<SourceFileBodyC &>(TextBufferC::Body()); }
    //: Access body.

    const SourceFileBodyC &Body() const
    { return static_cast<const SourceFileBodyC &>(TextBufferC::Body()); }
    //: Access body.

  public:

    bool Load(const StringC &filename) { 
      if(!IsValid())
	(*this) = SourceFileC(true);
      return Body().Load(filename);
    }
    //: Try and load a file.
    // NB. If this is called on an invalid buffer a new buffer
    // will be created. <p>
    // ** MUST BE OVERLOADED IN DERIVED CLASSES **
    
    bool Load(std::istream &instrm) { 
      if(!IsValid())
	(*this) = SourceFileC(true);
      return Body().Load(StringC(instrm)); 
    }
    //: Load buffer from a stream.
    // NB. If this is called on an invalid buffer a new buffer
    // will be created. <p>
    // ** MUST BE OVERLOADED IN DERIVED CLASSES **

    bool CheckDocVar(const StringC &var,const StringC &value,bool force = false)
    { return Body().CheckDocVar(var,value,force); }
    //: Check variable exists in hdr file.
    // if 'force' is true then update variable regardless if is there 
    // already or not.
    
    bool CheckDocVarSub(const StringC &var,const StringC &subs,const StringC &value,bool force = false)
    { return Body().CheckDocVarSub(var,subs,value,force); }
    //: Check variable exists in hdr file.
    // if 'force' is true then update variable regardless if is there 
    // already or not.
    
    StringC GetDocValue(const StringC &var)
    { return Body().GetDocValue(var); }
    //: Find a return a documentation value from the buffer.
    // Returns empty string if not found.
    
    bool CheckHeader(const TextBufferC &hdr,const StringC &eoh,
		     const StringC &name,const StringC &desc,const StringC &org,bool replaceExisting = false)
    { return Body().CheckHeader(hdr,eoh,name,desc,org,replaceExisting); }
    //: Check a header is on file.
    // eoh, is the string that marks the 'end of header'.
    // Returns true if the header has been updated.
    
    StringC &FileType()
    { return Body().FileType(); }
    //: Access file type.
    
    const StringC &FileType() const
    { return Body().FileType(); }
    //: Access file type.
    
    bool GetDocVars(HashC<StringC,StringC> &vars)
    { return Body().GetDocVars(vars); }
    //: Retrieve all documentation variables from the file.
    
    bool LeadingComment(StringC &title,StringC &detail)
    { return Body().LeadingComment(title,detail); }
    //: Extract leading comment from source file 
    // if there is one...
    
    friend class SourceFileBodyC;
  };
  
}

#endif
