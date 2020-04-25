
#include "Ravl/AString.hh"
#include <string.h>

namespace RavlN {

  AStringC::AStringC(const char *str,bool freeMemory)
    : SArray1dC<char>(const_cast<char *>(str),strlen(str),freeMemory)
  {}

  //: Base class constructor.
  AStringC::AStringC(const SArray1dC<char> &arr)
   : SArray1dC<char>(arr)
  {}

  //: Construct from a string and a length.
  AStringC::AStringC(const char *str,size_t length,bool freeMemory)
    : SArray1dC<char>(const_cast<char *>(str),length,freeMemory)
  {}

  //: Construct from String.
  // FIXME:- Investigate no copy implementation
  AStringC::AStringC(const StringC &str)
   : SArray1dC<char>(str.size())
  {
    memcpy(ReferenceElm(),str.data(),str.size().V());
  }

  //: Construct from SubStringC.
  // FIXME:- Investigate no copy implementation
  AStringC::AStringC(const SubStringC &str)
   : SArray1dC<char>(str.Size())
  {
    memcpy(ReferenceElm(),str.chars(),str.Size().V());
  }

  //: Construct from an stl string.
  AStringC::AStringC(const std::string &str)
   : SArray1dC<char>(str.size())
  {
    memcpy(ReferenceElm(),str.data(),str.size());
  }


  bool operator==(const AStringC& x, const AStringC& y)
  {
    if(x.size() != y.size()) return false;
    return strncmp(x.data(),y.data(),x.size()) == 0;
  }

  bool operator!=(const AStringC& x, const AStringC& y)
  { return !(x == y); }

  bool operator==(const AStringC& x, const StringC& y)
  {
    if(y.size() != x.size()) return false;
    return strncmp(x.data(),y.chars(),x.size()) == 0;
  }

  bool operator!=(const AStringC& x, const StringC& y)
  { return !(x == y); }

  bool operator==(const AStringC& x, const char *str)
  {
    size_t len = strlen(str);
    if(len != x.size()) return false;
    return strncmp(str,x.data(),len) == 0;
  }

  bool operator!=(const AStringC& x, const char *str)
  { return !(x == str); }


  bool operator==(const AStringC& x, const std::string &y)
  {
    if(x.Size() != y.size())
      return false;
    return strncmp(y.data(),x.data(),y.size()) == 0;
  }

  bool operator!=(const AStringC& x, const std::string &y)
  { return !(x == y); }

  bool operator==(const StringC& x, const AStringC& y)
  {
    if(x.size() != y.size()) return false;
    return strncmp(x.data(),y.data(),x.size()) == 0;
  }

  bool operator!=(const StringC& x, const AStringC& y)
  { return !(x == y); }

}
