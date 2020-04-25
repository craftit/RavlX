
#ifndef RAVL_HTMLPAGE_HEADER
#define RAVL_HTMLPAGE_HEADER 1

#include "Ravl/HtmlTable.hh"
#include "Ravl/HtmlString.hh"
#include "Ravl/OS/Filename.hh"

//! lib=RavlHtml
//! docentry="Ravl.API.Contrib.Html"
//! file="Ravl/HtmlTable.hh"
//! author="Kieron J Messer"

namespace RavlN {

  
  //! userlevel="Normal"
  //: A html page
  
  class HtmlPageC 
  : public DListC<StringC> 
  {

  public:
    HtmlPageC();
    //: Construct html page

    HtmlPageC(const FilenameC & stylefile);
    //: Construct html page and style file
    
    bool Title(const StringC & title) {
      m_title = title;
      return true;
    }
    //: Set the title

    bool Heading(const StringC & heading, bool center = false);
    //: Set a heading

    bool Line(const HtmlStringC & line, bool center = false); 
    //: Add a html line

    
    bool BackgroundColour(const StringC & backGroundColour) {
      m_backGroundColour = backGroundColour;
      return true;
    }
    //: Set background colour
    
    bool Paragraph() {
      (*this).InsLast("<p>");
      return true;
    }
    //: New paragraph
    
    bool HorizontalLine() {
      (*this).InsLast("<hr>");
      return true;
    }
    //: Draw a horizontal line

    bool Break() {
      (*this).InsLast("<br>");
      return true;
    }
    //: Do a break
    
    bool AddTable(HtmlTableC & table) {
      (*this).InsLast(table.Html());
      return true;
    }
    //: Add a table to the page

    StringC Html() const;
    //: Get the html page

    bool Save(const StringC & fname);
    //: Save to file

  private:
    //: Member functions
    StringC m_title;

    FilenameC styleFile;
    //: Include a style file

    StringC m_backGroundColour;
    // the background colour
    
  };
}

#endif
