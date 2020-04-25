// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TEXTFILE_HEADER
#define RAVL_TEXTFILE_HEADER 1
///////////////////////////////////////////////////////
//! file="Ravl/OS/Text/TextFile.hh"
//! lib=RavlOS
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Text Processing"
//! rcsid="$Id$"
//! date="05/12/1997"

#include "Ravl/Text/TextBuffer.hh"

namespace RavlN {
  
  class TextCursorC;
  
  ////////////////////////////////
  //: Text File, for use in text processing.
  // This is a BIG object!
  
  class TextFileBodyC 
    : public TextBufferBodyC
  {
  public:
    TextFileBodyC();
    //: Default constructor.

    TextFileBodyC(const TextBufferBodyC &tb);
    //: Default constructor.
    
    TextFileBodyC(const StringC &filename);
    //: Constructor from a file.
    
    TextFileBodyC(const StringC &text,bool noFinalRet,bool concat = false);
    //: Construct from a string. 
    // If 'noFinalReturn' is false, ensure each line ends with '\n', otherwise leave it alone. <p>
    // concat, if true don't split lines, otherwise do.
    
    TextFileBodyC(istream &fin);
    //: Constructor from a stream.

    TextFileBodyC(BinIStreamC &strm);
    //: Create from binary stream
    
    virtual RCBodyVC &Copy() const;
    //: Make a copy

    virtual bool Save(BinOStreamC &strm) const;
    //: Write to binary stream.

    virtual bool Save(std::ostream &strm) const;
    //: Write out to ostream.

    bool Load(const StringC &filename);
    //: Try and load a file.
        
    bool Load(istream &instrm);
    //: Load buffer from a stream.
    
    bool Save();
    //: Try and save this file.
    
    bool SetFilename(const StringC &filename);
    //: Set filename to use.
    
    inline StringC &Filename() 
      { return filename; }
    //: Get the name of the file.
    
    inline const StringC &Filename() const
      { return filename; }
    //: Get the name of the file.
    
    inline void operator+=(const StringC &text)
    { Append(text); }
    //: Append a line of text to the end of the file.
    
    
    inline void SetRenumber() 
      { renumber = true; }
    //: Set renumber flag.
    
    void Empty();
    //: Empty all contents, set name to noname
    
    void Dump(ostream &strm) const;
    //: Dump to stdout.
    
    inline bool IsLoaded() const
      { return loaded; }
    //: Have file loaded into memory ?
    
    IntT FindLineMatch(const StringC &str);
    //: Find a line begining with string 'str'.
    // returns line no otherwise -1.
    
  protected:    
    StringC filename;// Name of file.
    bool renumber; // Need to renumber
    bool loaded;
    friend class TextCursorC;
  };

  ////////////////////////////////
  //: Text File, for use in text processing.
  // This is a BIG object!
  
  class TextFileC 
    : public TextBufferC
  {
  public:
    TextFileC()
    {}
    //: Default constructor.
    // creates an invalid handle.
    
    explicit TextFileC(bool)
      : TextBufferC(*new TextFileBodyC())
    {}
    //: Constructor.
    // Creates an empty text file.
    
    TextFileC(const TextBufferC &tb)
      : TextBufferC(*new TextFileBodyC(*BodyPtr(tb)))
    {}
    //: Construct from a text buffer.
    
    TextFileC(const StringC &filename)
      : TextBufferC(*new TextFileBodyC(filename))
    {}
    //: Constructor from a file.
    
    TextFileC(const StringC &text,bool noFinalReturn,bool concat = false)
      : TextBufferC(*new TextFileBodyC(text,noFinalReturn,concat))
    {}
    //: Construct from a string. 
    // If 'noFinalReturn' is false, ensure each line ends with '\n', otherwise leave it alone. <p>
    // concat, if true don't split lines,  otherwise do.
    
    TextFileC(istream &fin)
      : TextBufferC(*new TextFileBodyC(fin))
      {}
    //: Constructor from a stream.
    
  protected:
    TextFileC(TextFileBodyC &bod)
      : TextBufferC(bod)
    {}
    //: Body constructor.
    
    TextFileBodyC &Body()
    { return static_cast<TextFileBodyC &>(TextBufferC::Body()); }
    //: Access body.

    const TextFileBodyC &Body() const
    { return static_cast<const TextFileBodyC &>(TextBufferC::Body()); }
    //: Access body.
    
  public:    
    bool Load(const StringC &filename) { 
      if(!IsValid())
	(*this) = TextFileC(true);
      return Body().Load(filename);
    }
    //: Try and load a file.
    // NB. If this is called on an invalid buffer a new buffer
    // will be created. <p>
    // ** MUST BE OVERLOADED IN DERIVED CLASSES **
    
    bool Load(istream &instrm) { 
      if(!IsValid())
	(*this) = TextFileC(true);
      return Body().Load(instrm); 
    }
    //: Load buffer from a stream.
    // NB. If this is called on an invalid buffer a new buffer
    // will be created. <p>
    // ** MUST BE OVERLOADED IN DERIVED CLASSES **
    
    bool Save()
      { return Body().Save(); }
    //: Try and save this file.
    
    bool SetFilename(const StringC &filename)
      { return Body().SetFilename(filename); }
    //: Set filename to use.
    
    inline const StringC &Filename() const
      { return Body().Filename(); }
    //: Get the name of the file.
    
    inline StringC &Filename()
      { return Body().Filename(); }
    //: Get the name of the file.
    
    inline bool operator+=(const StringC &text) 
      { return Body().AppendLine(text) != ((UIntT) -1); }
    //: Append a line of text to the end of the file.
    
    inline void SetRenumber() 
      { Body().SetRenumber(); }
    //: Set renumber flag.
    
    void Empty() 
      { Body().Empty(); }
    //: Empty all contents, set name to noname
    
    void Dump(ostream &strm) const
      { Body().Dump(strm); }
    //: Dump to stdout.
    
    inline bool IsLoaded() const
      { return Body().IsLoaded(); }
    //: Have file loaded into memory ?
    
    IntT FindLineMatch(const StringC &str)
    { return Body().FindLineMatch(str); }
    //: Find a line begining with string 'str'.
    // returns line no otherwise -1.
  protected:
    friend class TextCursorC;
  };
  
}
#endif
