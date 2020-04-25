// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
// TxCursore.cc
// $Id$
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/TextCursor.cc"

#include "Ravl/Text/TextCursor.hh"
#include "Ravl/SArray1d.hh"
#include <ctype.h>

namespace RavlN {
  
  static const UIntT badLineNo = 0xffffffff;
  
  /////////////////////////
  // Default constructor.
  
  TextCursorC::TextCursorC()
    : Col(0)
  {}
  
  /////////////////////
  //: Constructor.
  
  TextCursorC::TextCursorC(const StringC &file)
    : Col(0),
      textbuff(file)
  { line = textbuff.Lines(); }

  /////////////////////
  // Constructor.
  
  TextCursorC::TextCursorC(const TextFileC &file)
    : line(file.Lines()),
      Col(0),
      textbuff(file)
  {}
  
  //////////////////////
  // Constructor.
  
  TextCursorC::TextCursorC(const TextBufferC &buff)
    : line(buff.Lines()),
      Col(0),
      textbuff(buff)
  {}
  
  
  //////////////////////
  // Goto a specific line/char.
  
  bool TextCursorC::Goto(UIntT lineno,UIntT charno) {
    if(lineno >= textbuff.NoLines())
      return false;
    line.Nth(lineno);
    Col = charno;
    return true;
  }

  //////////////////////
  // Insert text at the cursor.
  
  bool TextCursorC::Insert(StringC txt,bool leaveAtEnd) {
    if(textbuff.IsReadonly())
      return false;
    StringC OldLine = line.Data().Text();
    SubStringC bef = OldLine.through((int) Col);
    SubStringC aft = OldLine.after((int) Col);
    IntT newLineAt;
    if((newLineAt = txt.index('\n')) >= 0) {
      DLIterC<TextFileLineC> atline(line);
      
      StringC newTxt = txt.through(newLineAt);
      line.Data().Text() = bef + newTxt;
      // Append as many lines as are needed.
      StringC theRest = txt.after(newLineAt);
      while((newLineAt = theRest.index('\n')) >= 0) {
	newTxt = theRest.through(newLineAt);
	atline.InsertAft(TextFileLineC(badLineNo,newTxt));
	atline.Next();
	theRest = theRest.after(newLineAt);
      }
      // Finish up.
      atline.InsertAft(TextFileLineC(badLineNo,theRest + aft));
      if(leaveAtEnd) {
	if(theRest.length() > 0) {
	  Col = theRest.length()-1;
	  atline.Next();
	  line = atline;
	} else { // At the end of the last line.
	  line = atline;
	  Col = line.Data().Text().length()-1;
	}
      }
    } else {
      //cerr << "TextCursorC::Insert(), Within a line. of '" <<txt <<"' \n";
      line.Data().Text() = bef + txt + aft;
      if(leaveAtEnd) {
	Col += txt.length();
	//cerr << "TextCursorC::Insert(), Moved cursor to '" <<Col <<"' \n";
      }
    }
    textbuff.SetModified();
    return true;
  }

  //////////////////////
  // Insert line of text on line after cursor.

  bool TextCursorC::InsertLine(StringC txt) {
    if(textbuff.IsReadonly())
      return false;
    if(txt.lastchar() != '\n')
      txt += '\n';
    line.InsertBef(TextFileLineC(badLineNo,txt));
    textbuff.SetModified();
    return true;
  }
  
  ///////////////////////////////
  // Insert line of text on line before cursor.
  
  bool TextCursorC::InsertLineBefore(StringC txt) {
    if(textbuff.IsReadonly())
      return false;
    if(txt.lastchar() != '\n')
      txt += '\n';
    line.InsertAft(TextFileLineC(badLineNo,txt));
    textbuff.SetModified();
    return true;
  }
  
  //////////////////////
  //: Skip a fixed string.
  // Returns True if the strip was found.
  // if not returns false, and leaves iterator unchanged.
  
  bool TextCursorC::Skip(const StringC &text) {
    StringC &l = LineText();
    if((l.length() - Col) < text.length())
      return false; // Line too short.
    const char *p1 = &l[Col];
    const char *p2 = &text[0]; 
    const char *eos = &p2[text.length()];
    for(;p2 != eos;p1++,p2++)
      if(*p1 != *p2)
      return false;
    Col += text.length();
    if(LineText().length() <= Col)
      NextLine();
    return true;
  }
  
  //: Skip to the next occurance of a string.
  // Returns true if the text has been found, false otherwise.
  // if the string is not found the iterator is left unchanged,
  // otherwise its is left on the character after 'text'

  bool TextCursorC::SkipTo(const StringC &text) {
    DLIterC<TextFileLineC> itl(line);
    UIntT at = Col;
    for(;itl;itl++) {
      IntT ind = itl->Text().index(text,at);
      if(ind >= 0) {
	Col = ind + text.length();
	line = itl;
	return true;
      }
      at = 0;
    }
    return false;
  }
  
  //: Clip all text in file from the cursor to text.
  // if 'text' is not found in the file an empty string is returned.
  // the iterator is left after the found 'text'.
  // NB. In the current implementation 'text' may not contain a carrage
  // return unless its the last character in the string.
  
  StringC TextCursorC::ClipTo(const StringC &text,bool orToEOF) {
    StringC ret;
    DLIterC<TextFileLineC> itl(line);
    IntT at = Col;
    for(;itl;itl++) {
      IntT ind = itl->Text().index(text,at);
      if(ind >= 0) {
	if(itl == line) {
	  ret = itl->Text().from(at);
	  ret = ret.before(ind-at);
	} else
	  ret += itl->Text().before(ind);
	Col = ((int) ind) + text.length();
	line = itl;
	return ret;
      }
      ret += itl->Text().from(at);
      at = 0;
    }
    if(orToEOF)
      return ret;
    return StringC();
  }
  
  //: Clip all text in file from the cursor to letter.
  // if letter is not found in the file an empty string is returned.
  // the iterator is left after the found letter.
  
  StringC TextCursorC::ClipTo(char letter,bool orToEOF) {
    StringC ret;
    DLIterC<TextFileLineC> itl(line);
    IntT at = Col;
    for(;itl;itl++) {
      IntT ind = itl->Text().index(letter,at);
      if(ind >= 0) {
	if(itl == line) {
	  ret = itl->Text().from(at);
	  ret = ret.before(ind-at);
	} else
	  ret += itl->Text().before(ind);
	Col = ((int) ind) + 1;
	line = itl;
	return ret;
      }
      ret += itl->Text().from(at);
      at = 0;
    }
    if(orToEOF)
      return ret;
    return StringC();    
  }
  
  //: Skip all characters in table.
  // the table can be created by BuildClipTable.
  // Returns True if left at a valid place.
  
  bool TextCursorC::SkipChars(const SArray1dC<bool> &delim) {
    // This could be faster... use ptr's etc....
    do {
      if(!delim[ Char() ])
	return true;
      NextChar();
    } while(IsElm());
    return false;
  }
  
  //: Skip all characters in delim
  // Returns True if left at a valid place.
  
  bool TextCursorC::SkipChars(const char *delim) { 
    return SkipChars(BuildClipTable(delim)); 
  }
  
  //////////////////////
  // Skip white spaces.
  
  SArray1dC<bool> textCursorC_WhiteSpace = TextCursorC::BuildClipTable(" \t\r\n");
  
  bool TextCursorC::SkipWhite() {
    if(!IsElm())
      return false;
    bool ret = textCursorC_WhiteSpace[Char()];
    SkipChars(textCursorC_WhiteSpace);
    return ret;
}
  
  // Print out position in human readable sytle.
  
  StringC TextCursorC::PositionTxt()  {
    return textbuff.Filename() + ':' + StringC(LineNo()) + ';' + StringC(Col) ;
  }
  
  //: Build a clip table suitable for above.
  
  SArray1dC<bool> TextCursorC::BuildClipTable(const char *rdelim) {
    SArray1dC<bool> delim(256);
    delim.Fill(false);
    //RavlAssert(!delim[0]); // bool defaults to false, but in case this changes.
    const char *place;
    // Fill out delim table.
    for(place = rdelim;*place != 0;place++)
      delim[(int) *place] = true;
    return delim;
  }
  
  
  //: Clip out string deliminated by delim characters.
  
  SubStringC TextCursorC::ClipWord(const SArray1dC<bool> &delim,bool initalSkipDelim) {
    RavlAssert(delim.Size() > 255);
    if(!IsElm())
      return SubStringC();
    if(initalSkipDelim) {
      while(delim[Char()]) {
	if(!NextChar())
	  return SubStringC();
      }
    }
    // Setup search...
    const char *place,*eos;
    StringC ret; //("",0,63-sizeof(StrRepC));
    do {
      place = &(LineText()[Col]);
      eos = &(LineText().chars()[LineText().length()]);
      const char *start = place;
      // Skip over word.
      for(;!delim[*place] && place != eos;place++) ; 
      if(place != eos) { // found on this line.
	// Cut it out.
	int len = (int) (place - start);
	if(LineText().length() <= Col)
	  NextLine();
	if(ret.length() == 0) {
	  int fromcol = Col;
	  Col += len;
	  return LineText().at((int) fromcol,len);
	}
	ret += LineText().at((int) Col,len);
	Col += len;
	return ret;
      }
      ret += LineText().from((int) Col);
    } while(NextLine());
    return ret;  
  }
  
  
  //: Clip out a string of characters included in 'table'.
  // leave the cursor positioned after the text that
  // was returend. NB. Buffer is not changed ! 
  
  StringC TextCursorC::ClipWordInc(const SArray1dC<bool> &table) {
    StringC ret;
    const char *place,*eos;
    do {
      place = &(LineText()[Col]);
      eos = &(LineText().chars()[LineText().length()]);
      const char *start = place;
      // Skip over word.
      for(;table[*place] && place != eos;place++) ; 
      if(place != eos) { // found on this line.
	// Cut it out.
	int len = (int) (place - start);
	if(LineText().length() <= Col)
	  NextLine();
	ret += LineText().at((int) Col,len);
	Col += len;
	return ret;
      }
      ret += LineText().from((int) Col);
    } while(NextLine());  
    return ret;  
  }
  
  static SArray1dC<bool> BuildClipID() {
    SArray1dC<bool> tab(256);
    tab.Fill(false);
    {for(int i = 'a';i <= 'z';i++)
      tab[i] = true;}
    {for(int i = 'A';i <= 'Z';i++)
      tab[i] = true;}
    {for(int i = '0';i <= '9';i++)
      tab[i] = true;}
    tab[(int) '_'] = true;
    return tab;
  }

  //: Clip out a identifier from line.  
  // This follows the usual 'C' type rules. 
  // In Regex language: "[A-Za-z_][A-Za-z0-9_]*".
  // leave the cursor positioned after the text that
  // was returend. NB. Buffer is not changed ! 
  
  StringC TextCursorC::ClipID() {
    if(!isalpha(Char()))
      return StringC();
    static SArray1dC<bool> tab = BuildClipID();
    return ClipWordInc(tab);
  }
  
  // Cut out next word.
  // A little wastefull..
  
  SubStringC TextCursorC::ClipWord(const char *rdelim,bool initalSkipDelim) { 
    return ClipWord(BuildClipTable(rdelim),initalSkipDelim); 
  }

}



