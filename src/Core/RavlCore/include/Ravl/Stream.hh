// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STREAM_HEADER
#define RAVL_STREAM_HEADER 1
/////////////////////////////////////////////////////////////
//! file="Ravl/Core/Base/Stream.hh"
//! lib=RavlCore
//! date="23/04/1998"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.IO.Streams"
//! rcsid="$Id$"
//! userlevel=Normal

#include "Ravl/config.h"
#include "Ravl/String.hh"
#include "Ravl/Assert.hh"
#include "Ravl/HandleRefCounter.hh"
#include "Ravl/Exception.hh"
#include <stdarg.h>

#if RAVL_HAVE_ANSICPPHEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

#if !RAVL_HAVE_STREAMSIZE
typedef unsigned int streamsize;
#endif

//#define RAVL_DUMP_IOSTATE 1

namespace RavlN {
#if (RAVL_COMPILER_VISUALCPP && !RAVL_HAVE_STDNAMESPACE)
  inline std::istream& operator>>(std::istream& is, bool & b) {
    int val;
    is >> val;
    b = (val != 0);
    return is;
  }
  //: Fix for Visual C++'s lack of a bool stream operator.

  inline std::ostream& operator<<(std::ostream& os, bool b) {
    os << ((int)b);
    return os;
  }
  //: Fix for Visual C++'s lack of a bool stream operator.
#endif

  typedef StringC (*URLMapperFuncT)(const StringC &fn);
  //: Pointer to URL mapping function.

  //! userlevel=Normal

  bool EnableURLMapper();
  //: Enable url mapping of filenames.
  // With the expection of 'file:' specifications this enabled the  changing of URL's
  // to a RAVL special files. i.e. 'http:' becomes '@http:'  This allows us to
  // implement handlers in seperate libraries which can link in as required.
  // If the 'file:' if found not to have a domain specifier then the 'file:' is stripped
  // off and the file is treated normally. <p>
  // Note: At the moment the URL handling mechanism may not handle relative filenames
  // correctly as it uses the program current working directory as a start point. If this
  // becomes a problem some optional data maybe added to StreamC which holds a current directory.

  bool DisableURLMapper();
  //: Disable url mapping of filenames.

  bool IsURLMappingEnabled();
  //: Test if URL mapping enabled.
  // Returns true if it is.

  //:-

  //! userlevel=Develop
  //: Reference counter IO stream base class.

  class StreamBaseC
    : public HandleRefCounterC
  {
  public:
#if RAVL_HAVE_STDNAMESPACE
    typedef std::ios::seekdir SeekDirT;
#else
#if RAVL_HAVE_IOS_SEEKDIR
    // GCC...
    typedef ::ios::seekdir SeekDirT;
#else
    // IRIX...
    typedef ios::seek_dir SeekDirT;
#endif
#endif

    StreamBaseC()
      : s(0),
        m_openFailed(false)
    {}
    //: Default constructor.

    StreamBaseC(const StringC &nname)
      : name(nname),
      	s(0),
        m_openFailed(false)
    {}
    //: Constructor.

    StreamBaseC(const StreamBaseC &oth)
      : HandleRefCounterC(oth),
        name(oth.name),
        s(oth.s),
        ptrManager(oth.ptrManager),
        m_openFailed(oth.m_openFailed)

    {}
    //: Copy Constructor.

    virtual ~StreamBaseC();
    //: Ensure correct destructor is used.

    inline const StringC &Name() const
    { return name; }
    //: Returns the name of the stream.

    void SetName(const StringC &nname)
    { name = nname; }
    //: Set the name to be used for the stream

    inline bool IsOpen() const
    { return !m_openFailed && !bad(); }
    //: Test if this stream is open.

#if RAVL_DUMP_IOSTATE
    void dumpState(const char *caller) const
    {
      if (s != 0)
      {
        ios_base::iostate sState = s->rdstate();
        std::cerr << "iostate(" << caller << ")" \
             << " s=" << (void*)s \
             << " eofbit=" << ((sState & ios_base::eofbit) != 0 ? "Y" : "N") \
             << " failbit=" << ((sState & ios_base::failbit) != 0 ? "Y" : "N") \
             << " badbit=" << ((sState & ios_base::badbit) != 0 ? "Y" : "N") \
             << " goodbit=" << ((sState & ios_base::goodbit) != 0 ? "Y" : "N") \
             << std::endl;
      }
    }
#endif

    bool good() const {
      if(s == 0)
        return false;
#if RAVL_DUMP_IOSTATE
      dumpState("good");
#endif
      return (s->good() != 0);
    }
    //: Is stream good ?

    bool bad() const {
      if(s == 0)
        return true;
#if RAVL_DUMP_IOSTATE
      dumpState("bad");
#endif
      return (s->bad() != 0);
    }
    //: Is stream corrupted ?

    inline bool eof() const {
      if (s == 0)
        return true;
#if RAVL_DUMP_IOSTATE
      dumpState("eof");
#endif
      return (s->eof() != 0);
    }
    //: End of file ?

    inline bool fail() const {
      if (s == 0)
        return true;
#if RAVL_DUMP_IOSTATE
      dumpState("fail");
#endif
      return (s->fail() != 0);
    }
    //: Operation failed ?

    bool operator!() const {
      return fail();
    }
    //: Not failed ?

    operator void*() const  {
      return fail() ? (void*)0 : (void*)(-1);
    }
    //: Ok ?

    bool DiagnoseStream(std::ostream &out);
    //: Print diagnostic message about the streams state to out.

    bool Close();
    //: Close this stream.
    // After this is called no further IO should be attempted!

    RCHandleC<RCBodyVC> &PointerManager()
    { return ptrManager; }
    //: Access the pointer manager.

    static bool EnableStreamExceptions(bool value = true, std::ios_base::iostate except = std::ios_base::badbit);
    //: make STL stream throw exceptions

  protected:
    StreamBaseC(std::ostream *ns,StringC afilename,bool nDelOnClose = true);
    //: Body Constructor.

    StreamBaseC(std::istream *ns,StringC afilename,bool nDelOnClose = true);
    //: Body Constructor.

    bool Init(std::istream *ns,StringC afilename,bool nDelOnClose = true);
    //: Setup
    // This should only be called on Stream's constructed with the
    // default constructor!

    bool Init(std::ostream *ns,StringC afilename,bool nDelOnClose = true);
    //: Setup
    // This should only be called on Stream's constructed with the
    // default constructor!

    std::ios &buf() {
      RavlAssert(s != 0);
      return *s;
    }
    //: Access handle.

    const std::ios &buf() const  {
      RavlAssert(s != 0);
      return *s;
    }
    //: Access handle.

  protected:
    StringC name; // Name of stream.
    std::ios *s;
    RCHandleC<RCBodyVC> ptrManager;
    bool m_openFailed;
  };

  //! userlevel=Normal
  //: Standard output streams with reference counting.

  class OStreamC
    : public StreamBaseC
  {
  public:
    OStreamC()
     : out(0)
    {}
    //:Default constructor.

    OStreamC(const StringC &filename,bool binary = true,bool buffered=true,bool append = false);
    //: Open a file for output.
    // '-' is treated as std::cout.

#if RAVL_HAVE_INTFILEDESCRIPTORS
    OStreamC(int fd,bool binary = true,bool buffered = true);
    //: Send data to unix file handle.
#endif

    inline OStreamC(std::ostream &strm,bool deletable = false)
      : StreamBaseC(&strm,"",deletable),
        out(&strm)
    {}
    //: Constructor.

    inline OStreamC(const OStreamC &oth)
      : StreamBaseC(oth),
	out(oth.out)
    {}
    //: Copy constructor.

    std::ostream& form(const char *format ...);
    //: Print to stream using good old 'C' style formating.
    // This isn't the safest function, it uses a fixed
    // buffer of 4096 bytes.  <p>
    // This is a duplication of the function  GNU iostreams
    // for those platforms that don't have this function.

    std::ostream &os() {
      RAVL_PARANOID(RavlAssertMsg(out != 0,"Attempt to use invalid OStreamC. "));
      return *out;
    }
    //: Access std::ostream.
    // Enables access to the underlying std::ostream functionality

    const std::ostream &os() const {
      RAVL_PARANOID(RavlAssertMsg(out != 0,"Attempt to use invalid OStreamC. "));
      return *out;
    }
    //: Access std::ostream.
    // Enables access to the underlying std::ostream functionality

    operator std::ostream &()
    { return os(); }
    //: Converter.

    operator const std::ostream &() const
    { return os(); }
    //: Converter.

    std::ostream &write(const char *d,StreamSizeT n)
    { return os().write(d,n); }
    //: Write data.
    // std::ostream compatible.

    std::ostream &put(char ch)
    { return os().put(ch); }
    //: Put character.
    // std::ostream compatible.

    std::ostream &operator<<(const char *txt)
    { return os() << txt; }
    //: Output text.

    template<class DataT>
    std::ostream &operator<<(const DataT &dat)
    { return os() << dat; }

    std::streampos Tell() const { return out->tellp(); }
    //: Where are we in the stream.

    void Seek(std::streampos to) { os().seekp(to); }
    //: Goto a position in the stream.

    std::streampos tellp() const { return out->tellp(); }
    //: Where are we in the stream.
    // std::ostream compatible.

    std::ostream &seekp(std::streampos to) { return os().seekp(to); }
    //: Goto a position in the stream.
    // std::ostream compatible.

    std::ostream &seekp(std::streampos to,SeekDirT dir) { return os().seekp(to,dir); }
    //: Goto a position in the stream.
    // std::ostream compatible.

    bool Close()
    { os().flush(); return StreamBaseC::Close(); }
    //: Flush and close stream


  protected:
    std::ostream *out;
  };


  //! userlevel=Normal
  //: Standard input streams with reference counting.

  class IStreamC
    : public StreamBaseC
  {
  public:
    IStreamC()
     : in(0)
    {}
    //:Default constructor.

    IStreamC(const StringC &filename,bool binary = true,bool buffered=true);
    //: Open a file.
    // '-' is treated as cin.

#if RAVL_HAVE_INTFILEDESCRIPTORS
    IStreamC(int fd,bool binary = true,bool buffered = true);
    //: Get data from unix file handle.
#endif

    inline IStreamC(std::istream &strm,bool deletable = false)
      : StreamBaseC(&strm,StringC(""),deletable),
        in(&strm)
    {}
    //: Constructor.

    inline IStreamC(const IStreamC &oth)
      : StreamBaseC(oth),
        in(oth.in)
    {}
    //: Copy constructor.

    void Unget(StringC text);
    //: Put string back.
    // Note: you must unget exactly what was previously read.
    // This includes a '\n' as a terminator

    void Unget(const char *dat,int len);
    //: Put string back.
    // Note: you must unget exactly what was previously read.

    IntT CopyTo(OStreamC &out,IntT maxChars = -1);
    //: Copy stream to output.

    std::istream &is() {
      RAVL_PARANOID(RavlAssertMsg(in != 0,"Attempt to use invalid IStreamC. "));
      return *in;
    }
    //: Access std::istream.
    // Enables access to the underlying std::istream functionality

    const std::istream &is() const {
      RAVL_PARANOID(RavlAssertMsg(in != 0,"Attempt to use invalid IStreamC. "));
      return *in;
    }
    //: Access std::istream.
    // Enables access to the underlying std::istream functionality

    operator std::istream &()
    { return is(); }
    //: Converter.

    operator const std::istream &() const
    { return is(); }
    //: Converter.

    std::istream &read(char *d,std::streamsize n)
    { return is().read(d,n); }
    //: read data.
    // std::istream compatible function.

    std::istream &get(char &ch)
    { return is().get(ch); }
    //: Get character.
    // std::istream compatible function.

    char get()
    { return is().get(); }
    //: Get character.
    // std::istream compatible function.

    std::istream &get(char *buff,int buffsize)
    { return is().get(buff,buffsize); }
    //: Read in a line.
    // std::istream compatible function.

    std::istream &getline(char *buff,int buffsize)
    { return is().getline(buff,buffsize); }
    //: Read in a line.
    // std::istream compatible function.

    std::streamsize gcount()
    { return is().gcount(); }
    //: Get number of character read in last operation.
    // std::istream compatible function.

    template<class DataT>
    std::istream &operator>>(DataT &dat)
    { return is() >> dat; }

    std::istream &operator>>(char *dat)
    { return is() >> dat; }

    std::streampos Tell() const { return in->tellg(); }
    //: Where are we in the stream.

    void Seek(std::streampos to) { is().clear(); is().seekg(to); }
    //: Goto a position in the stream.

    std::streampos tellg() const { return in->tellg(); }
    //: Where are we in the stream.
    // std::istream compatible function.

    std::istream &seekg(std::streampos to) { return is().seekg(to); }
    //: Goto a position in the stream.
    // std::istream compatible function.

    std::istream &seekg(std::streampos to,SeekDirT dir) { is().clear(); return is().seekg(to,dir); }
    //: Goto a position in the stream.
    // std::istream compatible.

    inline bool IsEndOfStream() {
      // Check there's more to read...
      if(!in->good())
      	return true;
      char cbuff;
      read(&cbuff,1);
      if(!in->good())
        return true;
      Unget(&cbuff,1);
      return false;
    }
    //: Test if its the end of the stream is comming up...
    // Maybe there's a better way ?

    //:-------------------------------------------------
    //: Parsing Functions.
    // Some methods to help parse text streams.

    char GetChar();
    //: Get next character.
    // This will throw an ExceptionEndOfStreamC if the end
    // of the input stream is found.

    char SkipWhiteSpace();
    //: Skip white space characters.
    // returns the first non-white space character found. This stream is left on
    // the character after the one returned. <p>
    // This will throw an ExceptionEndOfStreamC if the end
    // of the input stream is found. <br>

    bool Skip(const char *delim = " \n\t\r");
    //: Skip all 'delim' characters.
    // Leaves stream at first character this is not one of those
    // listed in 'delim'. If end of stream is found before
    // returns false.

    bool SkipWord(const char *word);
    //: Skip to after word if it matches the characters in the stream.
    //: otherwise return false and leave stream where it is.

    bool SkipTo(const StringC &endStr);
    //: Skip through stream until endStr is found.
    // this leaves the stream positioned at the
    // first character after the string. <p>
    // returns false if the end of the stream is found first.

    bool SkipTo(char let);
    //: Skip through stream until 'let' is found.
    // returns false if the end of the stream is found first.

    StringC ClipTo(char let);
    //: Return all characters before let.
    // the 'let' character is discarded. <p>
    // If the end of stream is found, all data found
    // upto that point is returned.

    StringC ClipTo(const StringC &endStr);
    //: Return all characters before 'endStr'.
    // this leaves the stream positioned at the
    // first character after the 'endStr'. <p>
    // If the end of stream is found, all data found
    // upto that point is returned.

    StringC ClipWord(const char *delim = " \n\t\r",bool initalSkipDelim = false);
    //: Clip word until one of 'delim' characters are found.
    // Stream is left at the terminating delimiter.

    StringC ClipNumber();
    //: Clip a number, digits 0-9 '.' '-' '+' and 'e'
    // Stream is left at first non matching character.

    bool Close()
    { return StreamBaseC::Close(); }
    //: Close stream

  protected:
    std::istream *in;
  };
}

#if RAVL_COMPILER_MIPSPRO
istream & operator >> ( std::istream & str,  signed char & dat )
{
  char tmp ;
  str >> tmp ;
  dat = (signed char) tmp ;
  return str ;
}
#endif

#endif
