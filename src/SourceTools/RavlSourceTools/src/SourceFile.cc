// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSourceTools
//! rcsid="$Id$"
//! file="Ravl/SourceTools/CodeManager/SourceFile.cc"

#include <time.h>
#include <stdio.h>

#include "Ravl/SourceTools/SourceFile.hh"
#include "Ravl/Text/TextCursor.hh"
#include "Ravl/OS/Filename.hh"

#if VISUAL_CPP
#define snprintf _snprintf
#endif

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  static HashC<StringC,StringC> GenerateCommentStr() {
    HashC<StringC,StringC> ret;
    ret["Unknown"] = "";
    ret["C"] = "/*";
    ret["CH"] = "/*";
    ret["C++H"] = "//";
    ret["C++"] = "//";
    ret["Lex"] = "/*";
    ret["Yacc"] = "/*";
    ret["Java"] = "//";
    ret["Make"] = "#";
    ret["Perl"] = "#";
    ret["Html"] = "<!--";
    return ret;
  }

  static HashC<StringC,StringC> GenerateCommentEndStr() {
    HashC<StringC,StringC> ret;
    ret["Unknown"] = " \n";
    ret["C"] = " */\n";
    ret["CH"] = " */\n";
    ret["C++H"] = "\n";
    ret["C++"] = "\n";
    ret["Lex"] = " */\n";
    ret["Yacc"] = " */\n";
    ret["Java"] = "\n";
    ret["Make"] = "\n";
    ret["Perl"] = "\n";
    ret["Html"] = " -->\n";
    return ret;
  }
  
  static HashC<StringC,StringC> GenerateExtentionMap() {
    HashC<StringC,StringC> ret;
    // C
    ret["c"] = "C";
    ret["h"] = "CH";
    // C++ 
    ret["cc"] = "C++";
    ret["C"] = "C++";
    ret["cpp"] = "C++";
    ret["hh"] = "C++H";
    ret["hpp"] = "C++H";
    
    ret["mk"] = "Make";
    ret["pl"] = "Perl";
    ret["java"] = "Java";
    ret["Lex"] = "Lex";
    ret["Yacc"] = "Yacc";    
    ret["html"] = "Html";    
    ret["htm"] = "Html";    
    return ret;
  }
  
  HashC<StringC,StringC> SourceFileBodyC::extToType = GenerateExtentionMap();
  HashC<StringC,StringC> SourceFileBodyC::commentString = GenerateCommentStr();
  HashC<StringC,StringC> SourceFileBodyC::commentEndString = GenerateCommentEndStr();
  //: Default constructor.
  
  SourceFileBodyC::SourceFileBodyC()
    : fileType("Unknown")
  {}
  
  //: Constructor from a file.
  
  SourceFileBodyC::SourceFileBodyC(const StringC &filename)
    : TextFileBodyC(filename),
      fileType("Unknown")
  {
    fileType = IdFile();
  }
  
  //: Construct from a string. 
  // concat, if true don't split lines, 
  // otherwise do.
  
  SourceFileBodyC::SourceFileBodyC(const StringC &text,bool noFinalRet,bool concat)
    : TextFileBodyC(text,noFinalRet,concat),
      fileType("Unknown")
  {
    fileType = IdFile();
  }
  
  //: Constructor from a stream.
  
  SourceFileBodyC::SourceFileBodyC(std::istream &fin)
    : TextFileBodyC(fin),
      fileType("Unknown")
  {}
  
  //: Id the current file.
  
  StringC SourceFileBodyC::IdFile() {
    FilenameC fn = Filename();
    StringC ext = fn.Extension();
    //ONDEBUG(std::cerr << "SourceFileBodyC::IdFile(), File '" << fn << "' of type '" <<extToType[ext] << "' Loaded\n");
    return extToType[ext];
  }

  //: Load from file.
  
  bool SourceFileBodyC::Load(const StringC &filename) {
    bool ret = TextFileBodyC::Load(filename);
    fileType = IdFile();
    return ret;
  }
  
  //: Insert variable line in file at an appropriate place.
  
  bool SourceFileBodyC::InsertVar(const StringC &line) {
    const StringC &comStr = commentString[fileType];
    //const StringC &comEndStr = commentEndString[fileType];
    SourceFileC me(*this);
    TextCursorC at((TextFileC &)me);
    
    // This is a bit of a hack at the moment. We ought to distinguish between
    // types of file more clearly, and make sure each is handled correctly.
    
    while(at.IsElm()) {
      if(at.LineText().matches("#",0,false) > 0) {
	//cerr << "Skip1:" << at.LineText();
	at.NextLine();
	continue; // After other macro defs a top of file.
      }
      if(at.LineText().matches("/*",0,false) > 0) {
	at.SkipTo("/*");
	if(!at.SkipTo("*/")) {
	  std::cerr << "WARNING: End of 'C' comment not found. ";
	  at.GotoCol(0);
	  // Insert variable before comment.
	  break;
	}
	if(!at.RestOfLine().contains("/*")) {
	  at.NextLine();
	  break;
	}
	continue;
      }
      if(at.LineText().matches(comStr,0,false) > 0) {
	//cerr << "Skip2:" << at.LineText();
	at.NextLine();
	continue; // After comment bar.
      }
      if(at.LineText().matches(comStr + "! ",0,false) > 0) {
	//cerr << "Skip3:" << at.LineText();
	at.NextLine();
	continue; // After other variables.
      }
      break;
    }
    return at.InsertLine(line);
  }

  //: Check variable exists in hdr file.
  // if 'force' is true then update variable regardless if is there 
  // already or not.
  
  bool SourceFileBodyC::CheckDocVar(const StringC &var,const StringC &value,bool force)  {
    IntT atline;
    const StringC &comStr = commentString[fileType];
    const StringC &comEndStr = commentEndString[fileType];
    if(comStr.IsEmpty()) {
      ONDEBUG(std::cerr << "SourceFileBodyC::CheckDocVar() Comment str not know for :" << fileType << "\n");
      return false; // Don't know the comment string for this filetype.
    }
    StringC newline =  comStr + "! " + var + '=' + value + comEndStr;
    ONDEBUG(std::cerr << "SourceFileBodyC::CheckDocVar() Newline :" << newline);
    if((atline = FindLineMatch(comStr + "! " + var)) >= 0) {
      StringC &theLine = (*this)[atline];
      //cerr << "(Found " + var +") ";
      bool update = force;
      if(!update) {
	int eqAt = theLine.index("=");
	if(eqAt > 0) {
	  StringC tmp = theLine.after(eqAt);
	  tmp = tmp.TopAndTail();
	  if(tmp == "")  // Empty ?
	    update = true;
	}
      }
      
      if(update) {
	if(theLine != newline) {
	  theLine = comStr + "! " + var + '=' + value + comEndStr;
	  SetModified();
	}
      }
      return true;
    }
    return InsertVar(newline);
  }
  
  bool SourceFileBodyC::CheckDocVarSub(const StringC &var,const StringC &subs,const StringC &value,bool force) {
    IntT atline;
    const StringC &comStr = commentString[fileType];
    const StringC &comEndStr = commentEndString[fileType];
    if(comStr.IsEmpty())
      return false; // Don't know the comment string for this filetype.
    StringC newline =  comStr + "! " + var + '=' + value + comEndStr;
    std::cerr << "SourceFileBodyC::CheckDocVarSub() Newline :" << newline;
    if((atline = (*this).FindLineMatch(comStr + "! " + var)) >= 0) {
      StringC &theLine = (*this)[atline];
      bool update = force;
      //    std::cerr << "(Found " + var +") ";
      if(!update) {
	int eqAt = theLine.index("=");
	if(eqAt > 0) {
	  if(theLine.after(eqAt).TopAndTail() == "")  // Empty ?
	    update = true;
	}
      }
      if(update) {
	if(theLine != newline) {
	  theLine = newline;
	  SetModified();
	}
      }
      return true;
    }
    if((atline = FindLineMatch(subs)) >= 0) {
      //cerr << "Doing subst on " << subs << " " << buff[atline] <<endl;
      (*this)[atline] = newline;
      SetModified();
      return true;
    }
    return InsertVar(newline);
  }
  
  ////////////////////////////
  //: Extract variable value.
  // Returns empty string if not found.
  
  StringC SourceFileBodyC::GetDocValue(const StringC &var) {
    IntT atline;
    const StringC &comStr = commentString[fileType];
    const StringC &comEndStr = commentEndString[fileType];
    if(comStr.IsEmpty())
      return StringC(); // Don't know the comment string for this filetype.
    StringC prefix = comStr + "! ";
    //ONDEBUG(std::cerr << "SourceFileBodyC::CheckDocVarSub() prefix :" << prefix << "\n");
    StringC matchline = prefix + var;
    if((atline = FindLineMatch(matchline)) < 0)
      return StringC(); // Not found.
    StringC &theLine = (*this)[atline];
    if(StringC(theLine.after(prefix)).before('=') != var)
      return StringC();
    StringC data = theLine.after('=');
    if(!(comEndStr.length() == 1 && comEndStr.lastchar() == '\n')) {
      StringC end = comEndStr.Copy().before('\n');
      data = data.before(end);
    }
    data = data.TopAndTail();
    if(data.length() < 1)
      return StringC();
    if(data.firstchar() == '"') { // Quoted ?
      // FIXME :- Tackle quoted strings.
      data = data.after('"');
      return data.before('"');
    }
    // Maybe just return data ??
    if(!data.contains(' '))
      return data;
    return data.before(' ');
  }
  

  //: Retrieve all documentation variables from the file.
  
  bool SourceFileBodyC::GetDocVars(HashC<StringC,StringC> &vars) {
    SourceFileC me(*this);
    const StringC &comStr = commentString[fileType];
    const StringC &comEndStr = commentEndString[fileType];
    if(comStr.IsEmpty())
      return false; // Don't know the comment string for this filetype.
    StringC prefix = comStr + "! ";
    TextCursorC at((TextFileC &)me);
    while(at.SkipTo(prefix)) {
      StringC tag = at.ClipTo('=').TopAndTail();
      at.SkipWhite();
      
      StringC value;
      // Sort out quoted strings.
      switch(at.Char())  {
      case '"':  at.NextChar();  value = at.ClipTo('"').TopAndTail();   break;
      case '\'': at.NextChar();  value = at.ClipTo('\'').TopAndTail();  break;
      default:   value = at.ClipTo(comEndStr).TopAndTail(); break;
      }
      vars[tag] = value;
    }
    
    return true;
  }
  

  //: Check a header is on file.
  
  bool SourceFileBodyC::CheckHeader(const TextBufferC &hdr,const StringC &ceoh,const StringC &name,const StringC &desc,const StringC &org,bool replaceExisting) {
    StringC eoh(ceoh);
    // Setup default end of header value.
    if(eoh.IsEmpty()) {
      eoh = const_cast<TextBufferC &>(hdr).Lines().Last().Text().Copy();
      if(eoh.lastchar() == '\n')
	eoh = eoh.before('\n');
    }
    if(eoh.IsEmpty() || eoh == "\n") {
      std::cerr << "ERROR: End of file marker is an empty string. \n";
      return false;
    }
    ONDEBUG(std::cerr << "Using marker:'" << eoh << "' \n");
    DLIterC<TextFileLineC> it1(const_cast<TextBufferC &>(hdr).Lines());
    DLIterC<TextFileLineC> it2(Lines());
    for(;it1 && it2;it1++,it2++) {
      if(it1->Text() != it2->Text())
	break;
    }
    if(!it1) 
      return false; // Header is present and correct.
    int eohAt = FindLineMatch(eoh); // Look for end of header marker.
    if(eohAt >= 0) { // Found end of header marker.
      if(eohAt > 30) {
	cerr << "WARNING: End of header marker more than 30 lines into the file. (" << eohAt <<") \n";
	return false;
      }
      if(replaceExisting)
        it2.Nth(eohAt).Head(); // Goto the last line, and remove the head of the list.
      else
        return true;      // Don't replace existing header.
    }

    char year[5], yr[3];
    time_t t;
    struct tm *now;

    t=time(NULL);
    now=localtime(&t);
    if (now == NULL)
    {
       std::cerr << "WARNING: Problem calculating current year - using default.\n";
       snprintf (year, 5, "%4d", 2012);
       snprintf (yr, 3, "%2d", 12);
    }
    else
    {
       strftime(year, 5, "%Y", now);
       strftime(yr, 3, "%y", now);
    }

    // Copy in new section.
    it2.First();
    for(it1.First();it1;it1++) {
      if(it1->Text().contains("%")) {
	StringC newLine = it1->Text().Copy();
	newLine.gsub("%name%",name);
	newLine.gsub("%desc%",desc);
	newLine.gsub("%org%",org);
	newLine.gsub("%yr%",yr);
	newLine.gsub("%year%",year);
	it2.InsertBef(TextFileLineC(0,newLine));
      } else
	it2.InsertBef(*it1);
    }
    SetModified();    
    return true;
  }
  
  //: Extract leading comment from source file 
  // if there is one...
  
  bool SourceFileBodyC::LeadingComment(StringC &title,StringC &detail) {
    SourceFileC me(*this);
    TextCursorC at((TextFileC &)me);
    const StringC &comStr = commentString[fileType];
    StringC leadStart = comStr + ":";
    //cerr << "LeadStart='" << leadStart <<"'\n";
    title = StringC();
    detail = StringC();
    // Find title.
    while(at.IsElm()) {
      if(at.LineText().index(leadStart) == 0)
	break;
      at.NextLine();
    }
    if(!at.IsElm()) return false; // Start not found.
    //cerr << "Title='" << at.LineText() <<"'\n";
    // Read in title.
    while(at.IsElm()) {
      if(at.LineText().index(leadStart) != 0)
	break;
      title += at.LineText().after(leadStart);
      at.NextLine();
    }
    //cerr << "Detail='" << at.LineText() <<"'\n";
    // Read in detail.
    while(at.IsElm()) {
      if(at.LineText().index(comStr) != 0) 
	break;
      detail += at.LineText().after(comStr);
      at.NextLine();
    }
    return true;
  }

}
