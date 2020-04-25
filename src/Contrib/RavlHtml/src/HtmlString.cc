
#include "Ravl/HtmlString.hh"
#include "Ravl/HashIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  using namespace RavlN;
  
  HtmlStringC::HtmlStringC(const char * str)
    : StringC(str)
  {
  }

  HtmlStringC::HtmlStringC(const StringC & str)
    : StringC(str)
  {
  }

  HtmlStringC::HtmlStringC(const StringC & str, const StringC & htmlAddress)
    : StringC(str)
  {
    MakeLink(htmlAddress);
  }

  HtmlStringC::HtmlStringC(const StringC & alt, const StringC & imagePath, IntT rows, IntT cols) 
    : StringC(alt)
  {
    MakeImage(imagePath, rows, cols);
  }


  bool
  HtmlStringC::MakeLink(const StringC & htmlAddress, bool newPage) {
    if(newPage)
      *this = "<a href=\"" + htmlAddress + "\" target=\"_blank\">" + (*this) + "</a>";
    else
      *this = "<a href=\"" + htmlAddress + "\">" + (*this) + "</a>";
    return true;
  }

  bool
  HtmlStringC::MakeImage(const StringC & imagePath, IntT rows, IntT cols) {

    if(rows==-1 && cols ==-1)
      *this = "<img src=\"" + imagePath + "\" alt=\"" + (*this) + "\">";
    else if(rows==-1)
      *this = "<img src=\"" + imagePath + "\" width =" + (StringC)cols + " alt=\"" + (*this) + "\">";
    else if(cols==-1)
      *this = "<img src=\"" + imagePath + "\" height =" + (StringC)rows + " alt=\"" + (*this) + "\">";
    else
      *this = "<img src=\"" + imagePath + "\" height=" +(StringC)rows + " width =" + (StringC)cols + " alt=\"" + (*this) + "\">";
    return true;
  }

  void
  HtmlStringC::Center() {
    *this = "<center>" + *this + "</center>";
  }
  


}
