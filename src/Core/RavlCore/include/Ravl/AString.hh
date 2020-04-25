// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! docentry="Ravl.API.Core.Strings"
//! file="Ravl/Core/Base/String.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="24/05/2012"

#ifndef RAVL_ASTRING_HEADER
#define RAVL_ASTRING_HEADER 1

#include "Ravl/String.hh"
#include "Ravl/SArray1d.hh"
#include <string>

namespace RavlN {

  //: SArray1dC based string.
  // This allows parsing buffers without copying its contents.
  // Note this string may not be null terminated.

  class AStringC
    : public SArray1dC<char>
  {
  public:
    AStringC()
    {}
    //: Construct an empty string.

    AStringC(const char *str,bool freeMemory = false);
    //: Create a string from a 'C' string.

    AStringC(const SArray1dC<char> &arr);
    //: Base class constructor.

    AStringC(const char *str,size_t length,bool freeMemory = false);
    //: Construct from a string and a length.

    AStringC(const StringC &str);
    //: Construct from String.

    AStringC(const SubStringC &str);
    //: Construct from SubStringC.

    AStringC(const std::string &str);
    //: Construct from an stl string.

    const char *data() const
    { return ReferenceElm(); }
    //: Access data in string.
    // May not be null terminated!

    size_t size() const
    { return SArray1dC<char>::Size().V(); }
    //: Access size of string.

  protected:
  };

  bool operator==(const AStringC& x, const AStringC& y);
  bool operator!=(const AStringC& x, const AStringC& y);
  bool operator==(const AStringC& x, const StringC& y);
  bool operator!=(const AStringC& x, const StringC& y);
  bool operator==(const AStringC& x, const char *str);
  bool operator!=(const AStringC& x, const char *str);
  bool operator==(const AStringC& x, const std::string &y);
  bool operator!=(const AStringC& x, const std::string &y);
  bool operator==(const StringC& x, const AStringC& y);
  bool operator!=(const StringC& x, const AStringC& y);

}

#endif
