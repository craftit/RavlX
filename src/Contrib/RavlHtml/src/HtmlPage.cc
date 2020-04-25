//! lib=RavlHtml

#include "Ravl/HtmlPage.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Stream.hh"
#include "Ravl/Text/TextFile.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  using namespace RavlN;
  
  HtmlPageC::HtmlPageC() {}

  HtmlPageC::HtmlPageC(const FilenameC & sf) 
    : styleFile(sf)
  {}

  bool 
  HtmlPageC::Heading(const StringC & heading, bool center) {
    HtmlStringC hdg = "<h1>" + heading + "</h1>"; 
    if(center) hdg = "<center>" + hdg + "</center>";
    InsLast(hdg);
    return true;
  }
  

  bool 
  HtmlPageC::Line(const HtmlStringC & line, bool center) {
    HtmlStringC ln = line;
    if(center) {
      ln = "<center>" + line + "</center>"; 
    } 
    InsLast(ln);
    return true;
  }

  StringC HtmlPageC::Html() const {
    StringC html;

    //: begin html
    html += "<html>\n";

    html += "<head>\n";    
    //: Need to add a style file if requested
    if(styleFile.Exists()) {
      TextFileC file(styleFile);
      for(UIntT i=1;i<=file.NoLines();i++) {
	html += file[i];
      }
    }
    html += "</head>\n";    
    
    if(m_backGroundColour.IsEmpty())
      html += "<body>\n";
    else
      html += "<body bgcolor=\"" + m_backGroundColour + "\">\n"; 
    
    //: set title
    html += "<title>" + m_title + "</title>\n";

    for(DLIterC<StringC>it(*this);it;it++) 
      html += *it;

    //: end html
    html += "</body>\n";
    html += "</html>\n"; 

    return html;
  }

  bool HtmlPageC::Save(const StringC & filename) {
    OStreamC os(filename);
    os << Html();
    return true;
  }
  

}
