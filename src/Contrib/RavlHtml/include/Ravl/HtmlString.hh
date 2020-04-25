#ifndef RAVL_HTMLSTRING_HEADER
#define RAVL_HTMLSTRING_HEADER 1

//! lib=RavlHtml
//! docentry="Ravl.API.Contrib.Html"
//! file="Ravl/HtmlString.hh"
//! author="Kieron J Messer"

#include "Ravl/String.hh"

namespace RavlN {

  using namespace RavlN;
  
  //! userlevel="Normal"
  //: A html string
  
  class HtmlStringC 
  : public StringC
  {

  public:
    HtmlStringC()
    {}
    //: Construct a simple html string

    HtmlStringC(const char * str);
    //: Construct a simple html string

    HtmlStringC(const StringC & str);
    //: Construct a simple html string

    HtmlStringC(const StringC & str, const StringC & htmlAddress);
    //: Construct a simple html link

    HtmlStringC(const StringC & alt, const StringC & imagePath, IntT rows, IntT cols);
    //: Construct a simple html image
    
    bool MakeLink(const StringC & htmlAddress, bool newPage=false);
    //: Make current string a link

    bool MakeImage(const StringC & imagePath, IntT rows=-1, IntT cols=-1);
    //: Make current string an image

    void Center();
    //: Center string


  private:
  };
}

#endif
