#ifndef RAVL_HTMLTABLE_HEADER
#define RAVL_HTMLTABLE_HEADER 1
//! lib=RavlHtml
//! docentry="Ravl.API.Contrib.Html"
//! file="Ravl/HtmlTable.hh"
//! author="Kieron J Messer"

#include "Ravl/RefCounter.hh"
#include "Ravl/DList.hh" 
#include "Ravl/HtmlString.hh"

namespace RavlN {

  
  //! userlevel="Normal"
  //: A html table
  
  class HtmlTableC 
  : public DListC<DListC<HtmlStringC> > 
  {

  public:
    HtmlTableC(bool center = false);
    //: Construct html page
    
    bool Caption(const StringC & caption) {
      m_caption = caption;
      return true;
    }
    //: Set the title

    bool AddRow(const HtmlStringC & col1);
    //: Add a row with a single column

    bool AddRow(const HtmlStringC & col1, const HtmlStringC & col2);
    //: Add a row with two coloumns

    bool AddRow(const HtmlStringC & col1, const HtmlStringC & col2, const HtmlStringC & col3);
    //: Add a row with three coloumns

    bool AddRow(const HtmlStringC & col1, const HtmlStringC & col2, const HtmlStringC & col3, const HtmlStringC & col4);
    //: Add a row with four coloumns

    bool AddRow(const HtmlStringC & col1, const HtmlStringC & col2, const HtmlStringC & col3, const HtmlStringC & col4, const HtmlStringC & col5);
    //: Add a row with five coloumns

    bool AddRow(const HtmlStringC & col1, const HtmlStringC & col2, const HtmlStringC & col3, const HtmlStringC & col4, const HtmlStringC & col5, const HtmlStringC & col6);
    //: Add a row with six coloumns

    bool AddRow(const HtmlStringC & col1, const HtmlStringC & col2, const HtmlStringC & col3, const HtmlStringC & col4, const HtmlStringC & col5, const HtmlStringC & col6, const HtmlStringC &col7);
    //: Add a row with six coloumns

    bool AppendColumn(UIntT row, StringC & col) {
      if(row > Size()-1)
        return false;
      Nth(row).InsLast(col);
      return true;
    }
    
    bool Sort();
    //: Sort according to first column if it is an int string

    StringC Html() const;
    //: Get the html page

  private:
    //: Member functions
    StringC m_caption;
    bool center;
  };
}

#endif
