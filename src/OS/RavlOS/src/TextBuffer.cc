// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////
// TxtPBuff.cc
// $Id$
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/TextBuffer.cc"

#include "Ravl/Text/TextBuffer.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/Stream.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/Exception.hh"
#include "Ravl/BinStream.hh"

#include <ctype.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  /////////////////////////////////////////
  //: Create a empty text buffer.
  
  TextBufferBodyC::TextBufferBodyC()
    : noLines(0),
      Modified(false),
      Readonly(false)
  {}

  //: Copy constructor
  TextBufferBodyC::TextBufferBodyC(const TextBufferBodyC &other)
   : noLines(other.noLines),
     lines(other.lines.Copy()),
     Modified(other.Modified),
     Readonly(other.Readonly)
  {}
  
  //: Create from binary stream
  TextBufferBodyC::TextBufferBodyC(BinIStreamC &strm)
   : RCBodyVC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("TextBufferBodyC");
    strm >> noLines >> Modified >> Readonly >> lines;
  }

  //: Create from text stream
  TextBufferBodyC::TextBufferBodyC(std::istream &strm)
  {
    if(!Load(strm)) {
      std::cerr << "ERROR: Failed to load template from stream. \n";
      throw ExceptionOperationFailedC("Failed to load template from stream. ");
    }
  }
  
  //: Make a copy
  RCBodyVC &TextBufferBodyC::Copy() const
  { return *new TextBufferBodyC(*this); }

  //! Write to binary stream.
  bool TextBufferBodyC::Save(BinOStreamC &strm) const
  {
    if(!RCBodyVC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << noLines << Modified << Readonly << lines;
    return true;
  }

  //! Write out to ostream.
  bool TextBufferBodyC::Save(std::ostream &strm) const
  {
    for(ConstDLIterC<TextFileLineC> it(lines);it.IsElm();it.Next())
      strm << it->Text();
    return true;
  }

  /////////////////////////////
  // Try and load a file.
  
  bool TextBufferBodyC::Load(const StringC &nFilename)  {
    ONDEBUG(cerr << "TextBufferBodyC::Load(), Asked to load '" << nFilename << "' \n");
    Empty();
    bool Ret = AppendFile(nFilename);
    Modified = false;
    return Ret;
  }
  
  /////////////////////////////
  // Append a file to the end of this one.
  
  bool TextBufferBodyC::AppendFile(const StringC &nFilename)  {
    ONDEBUG(cerr << "TextBufferBodyC::AppendFile(), Asked to append '" << nFilename << "' \n");
    IStreamC fin(nFilename);
    if(!fin)
      return false;
    return Append(fin);
  }
  
  /////////////////////////////
  //: Create buffer from stream.
  
  bool TextBufferBodyC::Load(istream &strm)  {
    Empty();
    bool ret = Append(strm);
    Modified = false;
    return ret;
  }
  
  /////////////////////////////
  //: Append contents of a stream to this file.
  
  bool TextBufferBodyC::Append(istream &fin) {
    ONDEBUG(cerr << "TextBufferBodyC::Append(istream), Called. \n");
    if(!fin) {
      ONDEBUG(cerr << "TextBufferBodyC::Append(istream), Passed invalid stream. \n");
      return false;
    }
    const int BuffSize = 1024;
    //const int BuffSize = 2048;
    char buff[BuffSize+2];
    StringC last("");
    while(fin) {
      StringC Buff("",80);
      fin.read(buff,BuffSize);
      std::streamsize n = fin.gcount();
      //printf("Chars:%d \n",n);
      if(n <= 0)
	    break;
      char *start,*place,*eb = &buff[n];
      start = buff;
      while(start != eb) {
	// Look for carrage returns.
	for(place = start;place != eb;place++) {
	  if(*place == '\n')
	    break;
	}
	// 'Place' is now at next seperator, or end of file.
	StringC tline;
	if(place == eb) {
	  place[0] = 0;
	  tline = StringC(start); // Turn it into a StringC.
	} else {
	  char tmp = place[1]; // Save character after seperator.
	  place[1] = 0; // Terminate string.
	  tline = StringC(start); // Turn it into a StringC.
	  place[1] = tmp; // Restore character after seperator.
	}
	//printf("tline:%s \n",tline.chars());
	start = (place+1); // Goto next bit of string to be read.
	if(place == eb) { // Goto to end of read buffer ?
	  last += tline; // Just append string to last.
	  break;
	} else {
	  // Append text to buffer, use anything stored in last.
	  if(last.length() > 0) { 
	    tline = last + tline;
	    last = "";
	  }
	  AppendLine(tline);
	}
      }
    }
    if(last.length() > 0) { // Anything left over ?
      last += '\n'; // Make sure its terminated with a newline.
      //cerr << "Appending:" << last << "\n";
    AppendLine(last); 
    }
    Modified = true;
    return true;
  }
  
  /////////////////////////////
  // Try and save this file as another name.
  
  bool TextBufferBodyC::SaveAs(const StringC &nFilename) {
    FilenameC afile(nFilename);
    if(afile.Exists()) {
      if(!afile.IsWritable())
	return false;
      if(afile.IsRegular()) {
	if(!afile.Rename(nFilename + '~')) {
	  cerr << "TextBufferBodyC::SaveAs(), Can't save file '" << nFilename << "', can't rename.\n";
	  return false;
	}
      } else {
	cerr << "TextBufferBodyC::SaveAs(), Can't save file '" << nFilename << "', its not a normal file!\n";
	return false;
      }
    }
    OStreamC fout(nFilename);
    if(!fout)
      return false;
    for(DLIterC<TextFileLineC> It(lines);It.IsElm();It.Next())
      fout << It.Data().Text();
    Modified = false;
    return true;
  }
  
  /////////////////////////////
  // Append a line of text to file.
  // returns LineNo or -1 if failed.  
  
  UIntT TextBufferBodyC::AppendLine(const StringC &text,bool noFinalRet) {
    Modified = true;
    //cerr << "TextBufferBodyC::AppendLine '" << text << "'\n";
    StringC newLine(text);
    if(!noFinalRet) {
      if(newLine.IsEmpty())
	newLine += '\n';
      else {
	if(newLine.lastchar() != '\n')
	  newLine += '\n';
      }
    }
    lines.InsLast(TextFileLineC(++noLines,newLine));
    return noLines;
  }
  
  //: Append a line of text to file.
  // returns LineNo or -1 if failed.  
  
  UIntT TextBufferBodyC::Append(const StringC &text,bool noFinalRet) {
    Modified = true;
    StringC buff(text);
    int nat,at = 0;
    if(buff == "")
      return noLines;
    //cerr << "TextBufferBodyC::Append '" << text << "'\n";
    if(!lines.IsEmpty()) {
      StringC nline;
      if(lines.Last().Text().lastchar() != '\n') {
	at = buff.index('\n',at);
	if(at < 0)
	  at = ((int) buff.length())-1;
	lines.Last().Text() += buff.through(at);
	at++;
      }
    }
    while(at < ((int) buff.length())) {
      nat = buff.index('\n',at);
      if(nat < 0)
	break;
      nat++;
      StringC nline = buff.at(at,(nat - at));
      AppendLine(nline,true);
      at = nat;
    }
    
    if(at < ((int) buff.length())) {
      StringC nline = buff.from(at);
      if(!noFinalRet)
	nline += '\n';
      AppendLine(nline,true);
    }
    
    return noLines;
  }

  //////////////////////////////////////////////
  //: Substute 'org' for 'nv' the whole buffer.
  // returns the number of substitutions done.
  
  IntT TextBufferBodyC::GlobalSubst(const StringC &org,const StringC &nv) {
    IntT count = 0;
    for(DLIterC<TextFileLineC> It(lines);It.IsElm();It.Next())
      count += It.Data().Text().gsub(org,nv);
    if(count > 0)
      Modified = true;
    return count;
  }
 
  //: Delete 'firstLine' to 'lastLine' inclusive.
  // Note: this is based on the original lines numbers as loaded from
  // the file. <br>
  // Returns the number of lines removed.
  
  IntT TextBufferBodyC::Delete(IntT firstLine,IntT lastLine) {
    RavlAssert(firstLine <= lastLine);
    DLIterC<TextFileLineC> fit(lines);
    for(;fit;fit++) {
      if((IntT) fit->LineNo() >= firstLine && fit->LineNo() != ((UIntT) -1))
	break;
    } 
    if(!fit || (IntT) fit->LineNo() > lastLine) {
      //cerr << "Failed to find first line. \n";
      return 0;
    }
    DLIterC<TextFileLineC> lit(fit);
    for(;lit;lit++) {
      // If we hit a patch of newly inserted stuff, skip to end of it
      // if we find a line included in the delete after, then include
      // new stuff too, otherwise don't.
      if(lit->LineNo() == ((UIntT) -1)) {
	DLIterC<TextFileLineC> sit(lit);
	for(;sit;sit++)
	  if(sit->LineNo() != ((UIntT) -1))
	    break;
	if(!sit) {
	  lit--;
	  break;
	}
	if((IntT) lit->LineNo() > lastLine)
	  break;
	lit = sit;
      }
      if((IntT) lit->LineNo() > lastLine)
	break;
    }
    //cerr << "Deleting lines... \n";
    IntT count = 0;
    // Delete upto lit
    for(;fit != lit;fit++) {
      count++;
      fit.Del();
    }
    if(count > 0)
      Modified = true;
    return count;
  }
  
  /////////////////////////////
  // Empty all contents, set name to noname
  
  void TextBufferBodyC::Empty() {
    Modified = false;
    Readonly = false;
    noLines = 0;
    lines.Empty();
  }
  
  /////////////////////////////
  // Dump to stdout.
  
  void TextBufferBodyC::Dump(ostream &strm) const {
    strm <<" Lines:" << noLines <<"\n";
    for(ConstDLIterC<TextFileLineC> It(lines);It.IsElm();It.Next())
      It.Data().Dump(strm);
  }

  StringC &TextBufferBodyC::operator[](IntT x) {
    RavlAssert(x >= 1);
    DLIterC<TextFileLineC> it(lines);
    for(;it && x > 1;it++,x--)
      ;
    RavlAssertMsg(it,"Text line index out of range."); 
    return it->Text();
  }

  //: Remove trailing white space from the file.
  // returns true if changes have been made.
  
  bool TextBufferBodyC::RemoveTrailingWhiteSpace() {
    bool ret = false;
    while(!lines.IsEmpty()) {
      StringC &line = lines.Last().Text();
      if(line.length() == 0) {
	lines.DelLast();
	ret = true;
	continue;
      }
      if(line.length() == 1) {
	if(line.lastchar() == '\n') {
	  lines.DelLast();
	  ret = true;
	  continue;
	}
      }
      if(isspace(line.lastchar())) {
	ret = true;
	if(line.length() == 1) {
	  lines.DelLast();
	  continue;
	}
	line = line.before(-1);
      }
    }
    return ret;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL_NAMED(TextBufferBodyC,TextBufferC,RCHandleVC<TextBufferBodyC>,"RavlN::TextBufferBodyC");
}

