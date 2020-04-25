// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLTEXTFILECURSOR_HEADER
#define RAVLTEXTFILECURSOR_HEADER 1
///////////////////////////////////////////////////////
//! file="Ravl/OS/Text/TextCursor.hh"
//! lib=RavlOS
//! author="Charles Galambos"
//! date="5/8/97"
//! userlevel=Normal
//! docentry="Ravl.API.OS.Text Processing"
//! rcsid="$Id$"

#include "Ravl/Text/TextFile.hh"

namespace RavlN {
  
  template<class DataT> class SArray1dC;
  class Regex;
  
  //: Cursor in text buffer.
  // This is a SMALL object.
  
  class TextCursorC 
  {
  public:
    TextCursorC();
    //: Default constructor.
    
    TextCursorC(const TextFileC &file);
    //: Constructor.
    
    TextCursorC(const TextBufferC &file);
    //: Constructor.
    
    TextCursorC(const StringC &file);
    //: Constructor.
    
    inline TextCursorC(TextFileC &file,DLIterC<TextFileLineC> AtLine,int ncol = 0);
    //: Constructor.
    
    inline TextCursorC(TextBufferC &buff,DLIterC<TextFileLineC> AtLine,int ncol = 0);
    //: Constructor.
    
    inline TextCursorC(const TextCursorC &oth);
    //: Copy constructor.
    
    bool Goto(UIntT lineNo,UIntT charNo = 0);
    //: Goto a specific line/char.
    
    inline UIntT LineNo() const;
    //: Get the current lineno.
    
    inline bool GotoCol(UIntT col);
    //: Goto a specific char on current line.
    
    bool Insert(StringC txt,bool leaveAtEnd = false);
    //: Insert text after character indicated by the cursor.
    // If 'leaveAtEnd' is true the cursor is left after the
    // inserted text, otherwise the cursor isn't changed.
    // Returns true on success.
  
    bool InsertLine(StringC txt);
    //: Insert line of text on line after cursor.
    
    bool InsertLineBefore(StringC txt);
    //: Insert line of text on line before cursor.
    
    StringC Clip(const Regex &exp);
    //: Clip out a substring from the current line.
    // leave the cursor positioned after the text that
    // was returend. NB. Buffer is not changed ! 
  
    StringC ClipWordInc(const SArray1dC<bool> &table);
    //: Clip out a string of characters included in 'table'.
    // leave the cursor positioned after the text that
    // was returend. NB. Buffer is not changed ! 
    
    StringC ClipID();
    //: Clip out a identifier from line.  
    // This follows the usual 'C' type rules. 
    // In Regex language: "[A-Za-z_][A-Za-z0-9_]*".
    // leave the cursor positioned after the text that
    // was returend. NB. Buffer is not changed ! 
    
    SubStringC ClipWord(const SArray1dC<bool> &table,bool initalSkipDelim = true);
    //: Clip out string deliminated by delim characters.
    // table has 256 elements one for each character. true means its a delim.
    // <p>Effectively does a SkipWhite(), the Clip("[^ ]*"); <p>
    // leave the cursor positioned after the text that
    // was returend. NB. Buffer is not changed ! <p>
    // Only if initalSkipDelim is true, deliminators at the begining of the string
    // will be skiped.
    
    static SArray1dC<bool> BuildClipTable(const char *delim);
    //: Build a clip table suitable for above.
    
    SubStringC ClipWord(const char *delim = " \n\t\r\0",bool initalSkipDelim = true);
    //: Clip out string deliminated by delim characters.
    // Effectively does a SkipWhite(), the Clip("[^ ]*"); <p>
    // leave the cursor positioned after the text that
    // was returend. NB. Buffer is not changed !  <p>
    // Only if initalSkipDelim is true, deliminators at the begining of the string
    // will be skiped.
    
    StringC ClipTo(const StringC &text,bool orToEOF = false);
    //: Clip all text in file from the cursor to text.
    // if 'text' is not found in the file an empty string is returned.
    // the iterator is left after the found 'text'.
    // NB. In the current implementation 'text' may not contain a carrage
    // return unless its the last character in the string.
    
    StringC ClipTo(char letter,bool orToEOF = false);
    //: Clip all text in file from the cursor to letter.
    // if letter is not found in the file an empty string is returned.
    // the iterator is left after the found letter.
    
    bool Skip(const Regex &exp);
    //: Skip the text described by exp.
    // Returns True if expression was matched.
    
    bool Skip(const StringC &text);
    //: Skip a fixed string.
    // Returns True if the text was found.
    // if not returns false, and leaves iterator unchanged.
    
    bool SkipTo(const StringC &text);
    //: Skip to the next occurance of a string.
    // Returns true if the text has been found, false otherwise.
    // if the string is not found the iterator is left unchanged,
    // otherwise its is left on the character after 'text'. <p>
    // NB. In the current implementation 'text' may not contain a carrage
    // return unless its the last character in the string.
    
    inline bool Skip(const char *text)
      { return Skip(StringC(text)); }
    //: Skip a fixed string.
    // Returns True if the strip was found.
    // if not returns false, and leaves iterator unchanged.
    
    bool SkipChars(const char *delim);
    //: Skip all characters in delim
    // Returns True if left at a valid place.
    
    bool SkipChars(const SArray1dC<bool> &table);
    //: Skip all characters in table.
    // the table can be created by BuildClipTable.
    // Returns True if left at a valid place.
    
    bool SkipWhite();
    //: Skip white spaces, including carrage return.
    
    inline bool IsElm() const 
      { return line.IsElm(); }
    //: Is on valid line.
    
    inline StringC &LineText() { 
      RavlAssert(line.IsElm()); 
      return line.Data().Text(); 
    }
    //: Text on current line.
    
    inline const StringC &LineText() const { 
      RavlAssert(line.IsElm()); 
      return line.Data().Text(); 
    }
    //: Text on current line.
    
    inline SubStringC RestOfLine();
    //: Get the rest of the current line.
    
    inline char &Char() 
      { return LineText()[Col]; }
    //: Get character.
    
    inline bool NextChar();
    //: Goto next character in buffer.
    
    inline bool PrevChar();
    //: Goto previous character in buffer.
    
    inline bool NextLine();
    //: Goto next line.
    
    inline void PrevLine() 
      { line.Prev(); }
    //: Goto previous line.
    
    inline void First() 
      { line.First(); Col = 0; }
    //: Goto the first column of the first line in the file.
    
    inline void FirstLine() 
      { line.First(); }
    //: Goto the first line in the file.
    
    inline void LastLine() 
      { line.Last(); }
    //: Goto last line in the file
    
    inline void BeginLine() 
      { Col = 0; }
    //: Goto begining of line.
    
    inline void EndOfBuffer() {
      LastLine();
      GotoCol(LineText().length());
    }
    //: Goto the end of the buffer.
    
    TextFileC &TextFile() 
      { return textbuff; }
    //: Get text file.
    
    inline const TextFileC &TextFile() const 
      { return textbuff; }
    //: Get text file.
  
    UIntT ColNum() const 
      { return Col; }
    //: Get the current column number.
    
    StringC PositionTxt();
    //: Print out position in human readable sytle.
  protected:
    DLIterC<TextFileLineC> line; // Ptr to current line.
    UIntT Col;                  // Col 0 to n.
    TextFileC textbuff;          // Buffer we're working with.
  };
  
  ////////////////////////////////////////////
  
  
  inline 
  TextCursorC::TextCursorC(TextFileC &file,DLIterC<TextFileLineC> AtLine,int ncol)
    : line(AtLine),
      Col(ncol),
      textbuff(file)
  {}
  
  inline 
  TextCursorC::TextCursorC(TextBufferC &buff,DLIterC<TextFileLineC> AtLine,int ncol) 
    : line(AtLine),
      Col(ncol),
      textbuff(buff)
  {}
  
  inline 
  TextCursorC::TextCursorC(const TextCursorC &oth) 
    : line(oth.line),
      Col(oth.Col),
      textbuff(oth.textbuff)
  {}

  inline 
  bool TextCursorC::NextLine()  { 
    if(!line.IsElm()) 
      return false; 
    line.Next(); 
    Col = 0; 
    return line.IsElm(); 
  }
  
  inline 
  bool TextCursorC::NextChar() { 
    Col++; 
    if((UIntT) Col < LineText().length()) 
      return true;
    return NextLine();
  }
  
  //: Goto previous character in buffer.
  
  inline 
  bool TextCursorC::PrevChar() {
    if(Col > 0) {
      Col--;
      return true;
    }
    PrevLine();
    if(!line.IsElm())
      return false;
    Col = LineText().length()-1;
    return true;
  }
  
  inline 
  UIntT TextCursorC::LineNo() const  { 
    if(!line.IsElm())
      return 0;
    return line.Data().LineNo();
  }
  
  inline 
  bool TextCursorC::GotoCol(UIntT ncol) {
    if((UIntT) ncol > LineText().length()) {
      Col = LineText().length();
      return false;
    }  
    Col = ncol;
    return true;
  }

  inline 
  SubStringC TextCursorC::RestOfLine() {
    return LineText().from((int) Col);
  }
}

#endif



