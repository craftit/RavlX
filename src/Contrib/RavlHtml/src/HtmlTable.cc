
#include "Ravl/HtmlTable.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/Random.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  using namespace RavlN;

  bool CompareStringWhichAreInts(const DListC<HtmlStringC> & str1, const DListC<HtmlStringC> & str2)
  {
    return (str1.First().IntValue() >= str2.First().IntValue());
  }

  HtmlTableC::HtmlTableC(bool c) :
      center(c)
  {
  }

  bool HtmlTableC::AddRow(const HtmlStringC & col1)
  {
    DListC<HtmlStringC> list;
    list.InsLast(col1);
    (*this).InsLast(list);
    return true;
  }

  bool HtmlTableC::AddRow(const HtmlStringC & col1, const HtmlStringC & col2)
  {
    DListC<HtmlStringC> list;
    list.InsLast(col1);
    list.InsLast(col2);
    (*this).InsLast(list);
    return true;
  }

  bool HtmlTableC::AddRow(const HtmlStringC & col1, const HtmlStringC & col2, const HtmlStringC & col3)
  {
    DListC<HtmlStringC> list;
    list.InsLast(col1);
    list.InsLast(col2);
    list.InsLast(col3);
    (*this).InsLast(list);
    return true;
  }

  bool HtmlTableC::AddRow(const HtmlStringC & col1, const HtmlStringC & col2, const HtmlStringC & col3, const HtmlStringC & col4)
  {
    DListC<HtmlStringC> list;
    list.InsLast(col1);
    list.InsLast(col2);
    list.InsLast(col3);
    list.InsLast(col4);
    (*this).InsLast(list);
    return true;
  }

  bool HtmlTableC::AddRow(const HtmlStringC & col1,
      const HtmlStringC & col2,
      const HtmlStringC & col3,
      const HtmlStringC & col4,
      const HtmlStringC & col5)
  {
    DListC<HtmlStringC> list;
    list.InsLast(col1);
    list.InsLast(col2);
    list.InsLast(col3);
    list.InsLast(col4);
    list.InsLast(col5);
    (*this).InsLast(list);
    return true;
  }

  bool HtmlTableC::AddRow(const HtmlStringC & col1,
      const HtmlStringC & col2,
      const HtmlStringC & col3,
      const HtmlStringC & col4,
      const HtmlStringC & col5,
      const HtmlStringC & col6)
  {
    DListC<HtmlStringC> list;
    list.InsLast(col1);
    list.InsLast(col2);
    list.InsLast(col3);
    list.InsLast(col4);
    list.InsLast(col5);
    list.InsLast(col6);
    (*this).InsLast(list);
    return true;
  }

  bool HtmlTableC::AddRow(const HtmlStringC & col1,
      const HtmlStringC & col2,
      const HtmlStringC & col3,
      const HtmlStringC & col4,
      const HtmlStringC & col5,
      const HtmlStringC & col6,
      const HtmlStringC & col7)
  {
    DListC<HtmlStringC> list;
    list.InsLast(col1);
    list.InsLast(col2);
    list.InsLast(col3);
    list.InsLast(col4);
    list.InsLast(col5);
    list.InsLast(col6);
    list.InsLast(col7);
    (*this).InsLast(list);
    return true;
  }

  bool HtmlTableC::Sort()
  {
    MergeSort(CompareStringWhichAreInts);
    return true;
  }

  StringC HtmlTableC::Html() const
  {
    StringC html;

    //: begin table
    if (center)
      html += "<center>\n";
    html += "<table border cellpadding=10 cellspacing=0 width=90%>\n";

    //: set title
    html += "<caption>" + m_caption + "</caption>\n";
    bool isFirst = true;
    for (DLIterC<DListC<HtmlStringC> > it(*this); it; it++) {
      html += "<tr>";
      for (DLIterC<HtmlStringC> colIt(*it); colIt; colIt++) {
        if (isFirst) {
          html += "<th>" + *colIt + "</th>\n";
        } else {
          html += "<td>" + *colIt + "</td>\n";
        }
      }
      isFirst = false;
      html += "</tr>\n";
    }

    //: end html
    html += "</table>\n";
    if (center)
      html += "</center>\n";

    return html;
  }

}
