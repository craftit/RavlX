// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_EXCEPTION_HEADER
#define RAVL_EXCEPTION_HEADER 1
///////////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Error Handling"
//! file="Ravl/Core/Base/Exception.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"

#include "Ravl/Types.hh"
#if RAVL_HAVE_ANSICPPHEADERS
#include <typeinfo>
#else
#include <typeinfo.h>
#endif
#include <iostream>

namespace RavlN {
#if RAVL_HAVE_STDNAMESPACE
  using namespace std;
#endif
  
  void DisplayStackTrace(std::ostream &strm,void **stack,int depth);
  //: Display a stack trace, if supported.
  //: 'stack' should be obtained with the 'backtrace()' provided on some
  //: systems. If its not supported this method will do nothing.


  enum StackTraceT {
    StackTraceOff,
    StackTraceOn
  };

  void SetAssertOnException(bool debugException);
  //: Cause an assertion failure on throwing exceptions.
  // Exceptions included are: ExceptionUnexpectedVersionInStreamC, ExceptionNumericalC, ExceptionOperationFailedC


  //: RAVL Exception.
  // Base class for Ravl's exceptions. <P>

  class ExceptionC {
  public:
    ExceptionC(const char *ntext,StackTraceT stackTrace = StackTraceOn);
    //: Constructor

    ExceptionC(const char *ntext,bool copy,StackTraceT stackTrace = StackTraceOn);
    //: Constructor.
    // Copy string if 'copy' is true.

    ExceptionC(const ExceptionC &oth);
    //: Copy Constructor
    // This assumes the only time you use a copy constructor
    // on an exception is when passing it as an argument!

    virtual ~ExceptionC();
    //: Virtualise destructor.

    const char *Text() const { return desc; }
    //: Get error description.

    virtual void Dump(std::ostream &strm);
    //: Dump contents of exception to strm;

    const char *what() const { return desc; }
    //: Standard exception text message.
  protected:
    const char *desc;
    bool ref; // Delete string in constructor ?

    static const int m_maxStackDepth = 64;
    int m_stackTraceDepth;
    void *m_stackTrace[m_maxStackDepth];

  };

  //: Casting exception class.
  // Thrown when an illegal cast is attempted.

  class ExceptionErrorCastC : public ExceptionC {
  public:
    ExceptionErrorCastC(const char *ndesc,const std::type_info &nfrom,const std::type_info &nto);
    //: Constructor.

    const std::type_info &From() const { return from; }
    //: Cast from.

    const std::type_info &To() const { return to; }
    //: Cast to.

    virtual void Dump(std::ostream &strm = std::cerr);
    //: Dump contents of exception to std::cerr;
  protected:
    const std::type_info &from;
    const std::type_info &to;
  };

  //: Exception: Operation Failed
  // Thrown if a requested operation failed because of
  // conditions outside the programs control. (Such as running
  // out of disk space.)

  class ExceptionOperationFailedC : public ExceptionC {
  public:
    ExceptionOperationFailedC(const char *ndesc);
    //: Constructor.

    ExceptionOperationFailedC(const char *ntext,bool copy);
    //: Constructor.
    // if copy is true, a copy is made of string ntext.
  };

  //: Exception: Bad Configuration
  // Exception indicating problem loading a configuration.

  class ExceptionBadConfigC 
   : public ExceptionC
  {
  public:
    ExceptionBadConfigC(const char *ndesc);
    //: Constructor.

    ExceptionBadConfigC(const char *ntext,bool copy);
    //: Constructor.
    // if copy is true, a copy is made of string ntext.
  };

  //! Exception indicating an invalid version id found in a stream.

  class ExceptionUnexpectedVersionInStreamC
    : public RavlN::ExceptionC
  {
  public:
    ExceptionUnexpectedVersionInStreamC(const char *text);
    //: Constructor.
  };

  //: Exception: Bad Configuration
  // Exception indicating problem loading a configuration.

  class ExceptionOutOfMemoryC
   : public ExceptionC
  {
  public:
    ExceptionOutOfMemoryC(const char *ndesc);
    //: Constructor.

    ExceptionOutOfMemoryC(const char *ntext,bool copy);
    //: Constructor.
  // if copy is true, a copy is made of string ntext.
  };

  //: Exception: Out of Range
  // Thrown if an operation in some way exceeds the allowed value.
  // Examples: Access outside the range of an array, or
  // one to many pop's from a stack

  class ExceptionOutOfRangeC : public ExceptionC {
  public:
    //: Constructor.
    ExceptionOutOfRangeC(const char *ndesc);

    //: Constructor.
    // if copy is true, a copy is made of string ntext.
    ExceptionOutOfRangeC(const char *ntext,bool copy);
  };


  //: Exception: Assertion Failed
  // Thrown if an assertion failed during the running of the program.
  // these should only be thrown if an internal inconstancy
  // has been detected.

  class ExceptionAssertionFailedC : public ExceptionC {
  public:
    ExceptionAssertionFailedC(const char *ndesc);
    //: Constructor.

    ExceptionAssertionFailedC(const char *ntext,bool copy);
    //: Constructor.
  // if copy is true, a copy is made of string ntext.
  };

  //: Exception: Numerical exception.
  // May be thrown where an operation fails for
  // a numerical reason, such as taking the Sqrt(-1) or
  // inverting a singular matrix.

  class ExceptionNumericalC
    : public ExceptionC
  {
  public:
    ExceptionNumericalC(const char *ndesc);
    //: Constructor.

    ExceptionNumericalC(const char *ntext,bool copy);
    //: Constructor.
    // if copy is true, a copy is made of string ntext.
  };

  //! userlevel=Normal
  //: Exception thrown when end of stream found.

  class ExceptionEndOfStreamC
    : public ExceptionC
  {
  public:
    ExceptionEndOfStreamC(const char *ntext);
    //: Constructor

    ExceptionEndOfStreamC(const char *ntext,bool copy);
    //: Constructor.
    // if copy is true, make a copy of ntext.
  };

  //! userlevel=Normal
  //: Exception issued when an parse error occurs.

  class ExceptionInvalidStreamC
    : public ExceptionC
  {
  public:
    ExceptionInvalidStreamC(const char *ntext);
    //: Constructor

    ExceptionInvalidStreamC(const char *ntext,bool copy);
    //: Constructor.
    // if copy is true, make a copy of ntext.
  };

}



#endif
