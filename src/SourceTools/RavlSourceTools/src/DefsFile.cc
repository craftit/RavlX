// This file is part of CxxDoc, The RAVL C++ Documentation tool
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlSourceTools
//! file="Ravl/SourceTools/CodeManager/DefsFile.cc"

#include "Ravl/SourceTools/DefsFile.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/Text/TextCursor.hh"
#include "Ravl/Text/TextFragment.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/HSet.hh"
#include "Ravl/HSetExtra.hh"

#define DDEBUG 1

namespace RavlN {

  const char *validtags[] = {"DESCRIPTION","MAINS","HEADERS","SOURCES","PLIB",
                       "USESLIBS","PROGLIBS","NESTED","SUPPORT_ONLY",
                       "DONOT_SUPPORT","PACKAGE","OPTLIB","OPTINCPATH",
                       "OPTLIBPATH","LIBDEPS","EXAMPLES","TESTEXES", "TESTLIBS",
                       "LOCAL_FILES","HTML","MAN1","MAN3","MAN5","LOCALHEADERS","PRIVATE",
                       "AUXDIR","AUXFILES","AUXINSTALL","EHT","STATE",
                       "DOCNODE","SCRIPTS","MUSTLINK","ANSIFLAG","LICENSE","PACKAGENAME",
                       "REQUIRES","ORGANISATION","OBJEXT",
                       "CXXEXT","CXXAUXEXT","CHXXEXT","CEXT","CHEXT","EXEEXT","USERCPPFLAGS","EXTERNALLIBS",
                       "LICENCE", "SWIGOPTS", "CCFLAGS", "LDFLAGS", "CPPFLAGS", "CCPPFLAGS",
                       "FLEX_DO_NOT_GENERATE", "BISON_DO_NOT_GENERATE", "SINGLESO", "NOSHAREDLIBDEPENDS",
                       "SUMMARY_LIB","EXTRA_RULES","CXX_STD","EXTIMGIO_RESOURCES","CFLAGS", 0 };

  /////////////////
  // Construct from a filename.

  DefsMkFileC::DefsMkFileC(const StringC &fn,bool doChecking) {
    Load(fn,doChecking);
  }

  //: Load a def file.

  bool DefsMkFileC::Load(const StringC &fn,bool doChecking) {
    TextFileC af;
    if(!af.Load(fn)) {
      std::cerr << "ERROR: Failed to load defs file :'" << fn << "'\n";
      return false;
    }
    defsFilename = fn;
    HashC<StringC,TextFragmentC> frags;
    return Load(af,doChecking,frags);
  }

  static SArray1dC<bool> DefMKFile_SkipSpace = TextCursorC::BuildClipTable(" \t\r\n");

  ///////////////////////
  // Load a def file.

  bool DefsMkFileC::Load(TextFileC &af,bool doChecking,HashC<StringC,TextFragmentC> &frags) {
    bool ret = true;
    tab.Empty();
    //  af.Dump(std::cout);
    int ifnest = 0;
    bool ignore = false;
    bool addTo = false;
    TextCursorC tc(af);
    while(tc.IsElm()) {
      tc.SkipWhite();
      if(!tc.IsElm())
	break;
      if(tc.Char() == '#') { // Comment...
	if(!tc.NextLine())
	  break;
	continue;
      }
#if DDEBUG
      //cout << "Doing line:" << tc.LineText();
#endif
      StringC tag,data;
      tag = tc.ClipID();
      if(tag.IsEmpty()) {
	cerr << "ERROR: reading tag at " << tc.PositionTxt() << "\n";
	ret = false;
	return false;
      }
      if(tab.IsElm(tag)) {
	cerr << "ERROR: repeated definition of variable '" << tag <<"' at " << tc.PositionTxt() << "  Value:" << tab[tag] << std::endl;
	return false;
      }
      if(!tc.IsElm())
	break;
      if(tag == "ifeq" || tag == "ifneq" || tag == "ifndef" || tag == "ifdef") {
	ifnest++;
	tc.NextLine(); // Ignore them.
	continue;
      }
      if(tag =="endif") {
	ifnest--;
	if(ifnest == 0)
	  ignore = false;
	tc.NextLine(); // Skip 'ifndef'
	continue;
      }
      if(tag == "else") { // Ignore all in 'else'
	if(!ignore) {
	  ignore = true;
	  ifnest = 1;
	}
	tc.NextLine(); // Skip 'endif'
	continue;
      }
      if(doChecking && !ignore) { // Check tags ?
	if(!CheckTag(tag)) {
	  std::cerr << "ERROR: unrecognised tag '" << tag <<"' at " << tc.PositionTxt() << "\n";
	  std::cerr << "Line :'" << tc.LineText().before('\n') << "'" << std::endl;
	  ret = false; // Check failed...
	}
      }
      tc.SkipWhite();
      if(!tc.IsElm())
	break;
      if(tc.Char() != '=') {
	addTo = false;
	if(tc.Char() == '+') { // Look out for '+='
	  if(tc.NextChar()) {
	    if(tc.Char() == '=')
	      addTo = true;
	  }
	}
	if(!addTo) {
	  std::cerr << "ERROR: assignment expected at " << tc.PositionTxt() << "\n";
	  ret = false;
	  tc.NextLine();
	  continue; // Skip line.
	}
      }
      TextCursorC startData = tc; // Mark start of data.
      tc.NextChar();
      if(!tc.IsElm())
      break;

      tc.SkipChars(DefMKFile_SkipSpace);
      if(!tc.IsElm()) {
	if(!ignore) {
	  frags[tag] = TextFragmentC(startData,startData);
	  tab[tag] = StringC();
	}
	break;
      }
      if(tc.ColNum() == 0) { // Must have got a newline.
	if(!ignore) {
	  frags[tag] = TextFragmentC(startData,startData);
	  tab[tag]=StringC();
	}
	continue;
      }

      //cout << tag << "=";
      bool done = false;

      TextFragmentC nf(startData,tc);
      while(tc.IsElm()) {
	switch(tc.Char())
	  {
	  case '\\':
	    tc.NextChar();
	    if(!tc.IsElm()) {
	      done = true;
	      break;
	    }
            if(tc.Char() == '\r') { // Look out for line feeds and ingore.
              tc.NextChar();
              if(!tc.IsElm()) {
                done = true;
                break;
              }
            }
	    if(tc.Char() != '\n') {
	      std::cerr << "Escaped : " <<  ((int) tc.Char()) << " " << ((int) '\\') << std::endl;
	      std::cerr << " '" << tc.LineText() << "'\n";
	      for(unsigned int i = 0;i < tc.LineText().length();i++) {
		cerr << (int) tc.LineText()[i] << " ";
	      }
	      std::cerr << std::endl;
	      data += tc.Char(); // Added escaped character.
	    }
	    break;
	  case '#':
	    tc.NextLine();
	    /* no break */
	  case '\n':
	    nf.End() = tc;
	    nf.End().PrevChar();
	    done = true;
	    break;
          case '\r':
            break; // Discard line feeds.
	  default:
	    //std::cout << tc.Char();
	    data += tc.Char();
	    /* no break */
	  }
	if(done)
	  break;
	tc.NextChar();
      }
      //cout << "\n";
      if(!ignore) {
	if(addTo) {
	  tab[tag] += data;
	  if(!frags.IsElm(tag))
	    frags[tag] = nf;
	  addTo = false;
	} else {
	  tab[tag] = data;
	  frags[tag] = nf;
	}
	//cerr << tag << " = " << data << std::endl;
      }
    }
    return ret;
  }

  //////////////////////////
  // Check if a tag is valid.

  bool DefsMkFileC::CheckTag(StringC tag)  {
    const char **place = validtags;
    for(;*place != 0;place++) {
      if(tag == *place)
	return true;
    }
    return false;
  }

  //: Make a list of all source files that should exists.
  // FIXME :- A better way of doing this, add flag to list
  // of var names ??

  StringListC DefsMkFileC::AllSources() {
    HSetC<StringC> sources;
    // Source code.
    sources += StringListC(Value("HEADERS"));
    sources += StringListC(Value("SOURCES"));
    sources += StringListC(Value("MUSTLINK"));
    sources += StringListC(Value("MAINS"));
    sources += StringListC(Value("LOCAL_FILES"));
    sources += StringListC(Value("LOCALHEADERS"));
    sources += StringListC(Value("DEVHDR"));
    sources += StringListC(Value("DEVSRC"));
    sources += StringListC(Value("TESTEXES"));
    sources += StringListC(Value("SCRIPTS"));
    // Documentation
    sources += StringListC(Value("HTML"));
    sources += StringListC(Value("MAN1"));
    sources += StringListC(Value("MAN3"));
    sources += StringListC(Value("MAN5"));
    sources += StringListC(Value("EXAMPLES"));
    sources += StringListC(Value("AUXFILES"));
    sources += StringListC(Value("EHT"));
    sources += StringListC(Value("DOCNODE"));
    // Auxiluary files.
    if(!IsDefined("USESLIBS") && !IsDefined("PROGLIBS"))
      sources += StringListC(Value("LIBDEPS"));
    sources += "defs.mk";
    StringListC ret;
    ret += sources;
    return ret;
  }


  ////////////////////////////////
  //: Make a list of nested directories.
  // This returns both active and inactive directories.
  // the .r's are striped from all strings.

  StringListC DefsMkFileC::AllNested() {
    StringListC ret(Value("NESTED"));
    for(DLIterC<StringC> it(ret);it.IsElm();it.Next()) {
      if(it.Data().matches(".r",it.Data().length() - 2))
	it.Data() = it.Data().before((IntT)(it.Data().length() - 2));
    }
    return ret;
  }

  //////////////////////////////////////
  //: Make a list of nested directories.
  // This returns only active directories.
  // the .r's are striped from all strings.

  StringListC DefsMkFileC::Nested() {
    StringListC ret(Value("NESTED"));
    for(DLIterC<StringC> it(ret);it.IsElm();it.Next()) {
      if(it.Data().matches(".r",it.Data().length() - 2))
	it.Data() = it.Data().before((IntT) (it.Data().length() - 2));
      else
	it.Del();
    }
    return ret;
  }

}
