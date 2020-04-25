// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
// TextFile.cc
// $Id$
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/TextFile.cc"

#include "Ravl/Text/TextFile.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  /////////////////////////////
  // Default constructor.
  
  TextFileBodyC::TextFileBodyC() 
    : filename("noname"),
      renumber(false),
      loaded(false)
  {}
  
  /////////////////////////////
  // Default constructor.
  
  TextFileBodyC::TextFileBodyC(const StringC &nFilename) 
    : filename("noname"),  // This will get replace by a copy in the Load function.
      renumber(false),
      loaded(false)
  { 
    ONDEBUG(cerr << "TextFileBodyC::TextFileBodyC(), Asked to load '" << nFilename << "' \n");
    if(!Load(nFilename)) {
      std::cerr << "ERROR: Failed to load file '" << nFilename << "' \n";
      throw ExceptionOperationFailedC("Failed to load template file. ");
    }
  }
  
  //: Construct from a string. 
  
  TextFileBodyC::TextFileBodyC(const StringC &text,bool noFinalRet,bool concat) 
    : filename("noname"),
      renumber(false),
      loaded(false)
  {
    if(concat)
      AppendLine(text,noFinalRet);  
    else
      Append(text,noFinalRet);  
  }
  
  /////////////////////////////
  //: Constructor from a stream.
  
  TextFileBodyC::TextFileBodyC(istream &fin)
    : filename("-"),
      renumber(false),
      loaded(false)
  {
    if(!Load(fin)) {
      std::cerr << "ERROR: Failed to load template from stream. \n";
      throw ExceptionOperationFailedC("Failed to load template from stream. ");
    }
  }
  
  /////////////////////////////
  // Construct from a buffer.
  
  TextFileBodyC::TextFileBodyC(const TextBufferBodyC &nbuff)
    : TextBufferBodyC(nbuff),
      filename("-noname-"),
      renumber(false),
      loaded(false)
  {}


    //: Create from binary stream
  TextFileBodyC::TextFileBodyC(BinIStreamC &strm)
   : TextBufferBodyC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("TextBufferBodyC");
    strm >> filename >> renumber >> loaded;
  }
  
  //: Make a copy
  RCBodyVC &TextFileBodyC::Copy() const
  { return *new TextFileBodyC(*this); }

  //! Write to binary stream.
  bool TextFileBodyC::Save(BinOStreamC &strm) const
  {
    if(!TextBufferBodyC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << filename << renumber << loaded;
    return true;
  }

  //! Write out to ostream.
  bool TextFileBodyC::Save(std::ostream &strm) const
  {
    return TextBufferBodyC::Save(strm);
  }


  /////////////////////////////
  // Try and load a file.
  
  bool TextFileBodyC::Load(const StringC &nFilename)  {
    ONDEBUG(cerr << "TextFileBodyC::Load(), Asked to load '" << nFilename << "' \n");
    Empty(); 
    filename = nFilename.Copy(); 
    ONDEBUG(cerr << "TextFileBodyC::Load(), loading '" << nFilename << "' \n");
    if(!TextBufferBodyC::Load(nFilename)) 
      return false;
    loaded = true;
    return true;
  }
  

  /////////////////////////////
  // Try and load a file.
  
  bool TextFileBodyC::Load(istream &istrm)  {
    Empty();
    if(!TextBufferBodyC::Load(istrm)) 
      return false;
    filename = "-";
    loaded = true;
    return true;
  }
  
  ////////////////////////////
  // Empty all contents.
  
  void TextFileBodyC::Empty()  {
    TextBufferBodyC::Empty();
    filename ="noname";
    loaded = false;
  }
  
  ////////////////////////////
  // Dump to stdout.
  
  void TextFileBodyC::Dump(ostream &strm) const  {
    strm << "File:" << filename << "\n";
    TextBufferBodyC::Dump(strm);
  }
  
  ////////////////////////////
  // Set filename to use.
  
  bool TextFileBodyC::SetFilename(const StringC &nfilename)  {
    filename = nfilename.Copy();
    return true;
  }

  //: Try and save this file.
  
  bool TextFileBodyC::Save() {
    if(filename.IsEmpty()) {
      ONDEBUG(cerr << "TextFileBodyC::Save(), ");
      std::cerr << "ERROR: Save of text file attempted with empty filename. \n";
      return false;
    }
    return SaveAs(filename);
  }
  
  ///////////////////////////////
  // Find a line begining with string 'str'.
  // returns line no otherwise -1.
  
  IntT TextFileBodyC::FindLineMatch(const StringC &str)  {
    if(!IsLoaded())
      return -1; // Failed.
    IntT lineno = 1;
    DLIterC<TextFileLineC> it(TextBufferBodyC::Lines());
    //cerr << "Looking for:'" << str << "'\n";
    for(;it.IsElm();it.Next(),lineno++) {
      //cerr << "Checking" << it.Data().Text().matches(str) << ":" << it.Data().Text();
      if(it.Data().Text().matches(str,0,false) > 0)
	return lineno;
    }
    return -1;
  }
  
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(TextFileBodyC,"RavlN::TextFileBodyC");

}
