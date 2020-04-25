// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TEXTBUFFER_HEADER
#define RAVL_TEXTBUFFER_HEADER 1
///////////////////////////////////////////
//! file="Ravl/OS/Text/TextBuffer.hh"
//! lib=RavlOS
//! userlevel=Develop
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Text Processing"
//! rcsid="$Id$"
//! date="05/12/1997"

#include "Ravl/Text/TextFileLine.hh"
#include "Ravl/DList.hh"
#include "Ravl/RCHandleV.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  
  //////////////////////////////////////
  //! userlevel=Develop
  //: Text buffer body
  
  class TextBufferBodyC 
    : public RCBodyVC
  {
  public:
    TextBufferBodyC();
    //: Create a empty text buffer.

    TextBufferBodyC(const TextBufferBodyC &body);
    //: Create a empty text buffer.

    TextBufferBodyC(BinIStreamC &strm);
    //: Create from binary stream

    TextBufferBodyC(std::istream &strm);
    //: Create from text stream

    virtual RCBodyVC &Copy() const;
    //: Make a copy

    virtual bool Save(BinOStreamC &strm) const;
    //: Write to binary stream.

    virtual bool Save(std::ostream &strm) const;
    //: Write out to ostream.
    
    bool Load(const StringC &filename);
    //: Try and load a file.
    
    bool AppendFile(const StringC &filename);
    //: Append a file to the end of this one.
    
    bool Load(istream &strm);
    //: Create buffer from stream.
    
    bool Append(istream &strm);
    //: Append contents of a stream to this file.
    
    bool SaveAs(const StringC &Filename);
    //: Try and save this file as another name.
    
    UIntT AppendLine(const StringC &text,bool noFinalReturn = false); 
    //: Append a line of text to file.
    // If 'noFinalReturn' is false, check each line ends with '\n', otherwise leave it alone. <p>
    // returns LineNo or -1 if failed.  

    UIntT Append(const StringC &text,bool noFinalReturn = false); 
    //: Append a line of text to file.
    // If 'noFinalReturn' is false, check each line ends with '\n', otherwise leave it alone. <p>
    // returns LineNo or -1 if failed.  
    
    IntT GlobalSubst(const StringC &org,const StringC &nv);
    //: Substute 'org' for 'nv' the whole buffer.
    // returns the number of substitutions done.
    
    IntT Delete(IntT firstLine,IntT lastLine);
    //: Delete 'firstLine' to 'lastLine' inclusive.
    // Note: this is based on the original lines numbers as loaded from
    // the file. <br>
    // Returns the number of lines removed. <br>
    // If we hit a patch of newly inserted lines, skip to end of it
    // if we find a line included in the delete after, then delete
    // new lines too, otherwise don't.
    
    void Empty();
    //: Empty all contents, set name to noname
    
    void Dump(ostream &strm) const ;
    //: Dump to stdout.
    
    inline UIntT NoLines() const 
    { return noLines; }
    //: Return number of lines in buffer.
    
    inline bool IsReadonly() const 
    { return Readonly; }
    //: Is readonly ?
    
    inline void SetModified() 
    { Modified = true; }
    //: Set renumber flag.
    
    DListC<TextFileLineC> &Lines() 
    { return lines; }
    //: Access list of lines directly.

    const DListC<TextFileLineC> &Lines() const
    { return lines; }
    //: Access list of lines directly.

    StringC &operator[](IntT x);
    //: Access a line in the file.
    // This function is SLOW!  Use a TextCursorC for
    // iterating through a buffer.
    
    inline bool IsModified() const 
    { return Modified; }
    //: Has buffer been modified ?
    
    inline void SetReadonly(bool newval) 
    { Readonly = newval; }
    //: Set readonly flag for file.

    bool RemoveTrailingWhiteSpace();
    //: Remove trailing white space from the file.
    // returns true if changes have been made.
    
  protected:
    UIntT noLines;
    DListC<TextFileLineC> lines; // Text from file.
    bool Modified; // File modified since loaded ?
    bool Readonly; // File read only ?
    
    friend class TextCursorC;
  };
    
  ///////////////////////////////////////
  
  //! userlevel=Normal
  //: Text buffer.
  
  class TextBufferC 
    : public RCHandleVC<TextBufferBodyC>
  {
  public:
    TextBufferC()
    {}
    //: Default constructor.
    // creates an invalid handle to a text buffer.
    
    explicit TextBufferC(bool)
      : RCHandleVC<TextBufferBodyC>(*new TextBufferBodyC())
    {}
    //: Default constructor.
    // creates an invalid handle to a text buffer.

    TextBufferC(BinIStreamC &strm);
    //: Create from binary stream

    TextBufferC(std::istream &strm);
    //: Create from text stream

  protected:
    TextBufferC(TextBufferBodyC &bod)
      : RCHandleVC<TextBufferBodyC>(bod)
    {}
    //: Body constructor.
    
    TextBufferBodyC & Body()
    { return RCHandleC<TextBufferBodyC>::Body(); }
    //: Access body.

    const TextBufferBodyC& Body() const
    { return RCHandleC<TextBufferBodyC>::Body(); }
    //: Access body.
    
  public:
    bool Load(const StringC &filename) { 
      if(!IsValid())
	(*this) = TextBufferC(true);
      return Body().Load(filename); 
    }
    //: Try and load a file.

    bool Load(istream &strm) { 
      if(!IsValid())
	(*this) = TextBufferC(true);
      return Body().Load(strm); 
    }
    //: Create buffer from stream.
    
    bool AppendFile(const StringC &filename)
    { return Body().AppendFile(filename); }
    //: Append a file to the end of this one.
    
    bool Append(istream &strm)
    { return Body().Append(strm); }
    //: Append contents of a stream to this file.
    
    bool SaveAs(const StringC &filename)
    { return Body().SaveAs(filename); }
    //: Try and save this file as another name.
    
    UIntT AppendLine(const StringC &text,bool noFinalRet = false)
    { return Body().AppendLine(text,noFinalRet); }
    //: Append a line of text to file.
    // If 'noFinalReturn' is false, ensure each line ends with '\n', otherwise leave it alone. <p>
    // returns LineNo or -1 if failed. <p>
      
    UIntT Append(const StringC &text,bool noFinalRet = false)
    { return Body().Append(text,noFinalRet); }
    //: Append a line of text to file.
    // If 'noFinalReturn' is false, ensure each line ends with '\n', otherwise leave it alone. <p>
    // returns LineNo or -1 if failed. <p>
    
    IntT GlobalSubst(const StringC &org,const StringC &nv)
    { return Body().GlobalSubst(org,nv); }
    //: Substute 'org' for 'nv' the whole buffer.
    // returns the number of substitutions done.
    
    IntT Delete(IntT firstLine,IntT lastLine)
    { return Body().Delete(firstLine,lastLine); }
    //: Delete 'firstLine' to 'lastLine' inclusive.
    // Note: this is based on the original lines numbers as loaded from
    // the file. <br>
    // Returns the number of lines removed. <br>
    // If we hit a patch of newly inserted lines, skip to end of it
    // if we find a line included in the delete after, then delete
    // new lines too, otherwise don't.
    
    void Empty()
    { Body().Empty(); }
    //: Empty all contents, set name to noname
    
    void Dump(ostream &strm) const 
    { Body().Dump(strm); }
    //: Dump to stdout.
    // For debuging only.
    
    inline UIntT NoLines() const 
    { return Body().NoLines(); }
    //: Return number of lines in buffer.
    
    inline bool IsReadonly() const 
    { return Body().IsReadonly(); }
    //: Is readonly ?
    
    inline void SetModified() 
    { Body().SetModified(); }
    //: Set renumber flag.
    
    DListC<TextFileLineC> &Lines() 
    { return Body().Lines(); }
    //: Access list of lines directly.
    
    const DListC<TextFileLineC> &Lines() const
    { return Body().Lines(); }

    //: Access list of lines directly.
    inline StringC &operator[](IntT x) 
    { return Body().operator[](x); }
    //: Access a line in the file.
    
    inline bool IsModified() const 
    { return Body().IsModified();}
    //: Has buffer been modified ?
    
    inline void SetReadonly(bool newval) 
    { Body().SetReadonly(newval); }
    //: Set readonly flag for file.
    
    bool RemoveTrailingWhiteSpace()
    { return Body().RemoveTrailingWhiteSpace(); }
    //: Remove trailing white space from the file.
    // returns true if changes have been made.
    
  };
}  
#endif
