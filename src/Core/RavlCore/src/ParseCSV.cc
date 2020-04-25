// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/ParseCSV.hh"
#include "Ravl/String.hh"
#include "Ravl/StringArray.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/Exception.hh"

namespace RavlN {

  //! Constructor
  ParseCSVC::ParseCSVC(UIntT expectedColumns,
                       bool expectHeader,
                       const char *seperators,
                       bool throwExceptionOnParseError)
   : m_columns(expectedColumns),
     m_expectHeaders(expectHeader),
     m_seperators(seperators),
     m_throwExceptionOnParseError(throwExceptionOnParseError)
  {}

  //! Read values from the file.
  bool ParseCSVC::ReadValues(SArray1dC<StringC> &values) {
    StringC text;
    if(!ReadNonEmptyLine(text))
      return false;
    StringArrayC strArr = StringArrayC::SplitQuote(text,m_seperators,true);
    if(m_columns > 0 && strArr.Size() != m_columns) {
      if(m_throwExceptionOnParseError) {
        throw ExceptionInvalidStreamC("Invalid number of columns found in record");
      }
      RavlWarning("Incorrect number of columns in record %u, expected %u. in line:'%s' ",(unsigned) strArr.Size(),m_columns,text.c_str());
      for(int i = 0;i < strArr.Size();i++) {
        RavlDebug("Field %d : '%s' ",i,strArr[i].c_str());
      }
      return false;
    }
    values = strArr.Array();
    return true;
  }


  //! Read line from stream.
  bool ParseCSVC::ReadLine(StringC &text) {
    int n = readline(m_strm,text,'\n',true);
    if(n > 0) {
      // DOS/Windows compatibility remove line feeds.
      if(text.lastchar() == '\r') {
        text = text.before((int) text.Size().V()-1);
      }
      return true;
    }
    return m_strm.good();
  }

  //! Read next non-empty line from stream.
  //! Returns false if none found.
  bool ParseCSVC::ReadNonEmptyLine(StringC &text)
  {
    do {
      if(!ReadLine(text))
        return false;
    } while(text.TopAndTail().IsEmpty());
    return true;
  }

  //! Start parsing a stream;
  bool ParseCSVC::Open(IStreamC &strm) {
    m_strm = strm;
    m_columns = 0;
    if(m_expectHeaders) {
      StringC text;
      if(!ReadNonEmptyLine(text))
        return false;
      StringArrayC getHeaders = StringArrayC::Split(text,m_seperators);
      m_headers = getHeaders.Array();
      if(m_columns == 0) {
        m_columns = m_headers.Size();
      } else {
        if(m_columns != m_headers.Size()) {
          if(m_throwExceptionOnParseError) {
            throw ExceptionInvalidStreamC("Invalid number of columns found in record");
          }
          RavlError("Incorrect number of columns in header %u, expected %u ",(unsigned) m_headers.Size(),m_columns);
          return false;
        }
      }
    }
    return true;
  }

  //! Start parsing a stream;
  bool ParseCSVC::Open(const StringC &filename) {
    IStreamC strm(filename);
    if(!strm.IsOpen())
      return false;
    return Open(strm);
  }





}
