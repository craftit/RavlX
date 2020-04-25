// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PARSECSV_HH_
#define RAVL_PARSECSV_HH_
//! lib=RavlCore

#include "Ravl/String.hh"
#include "Ravl/Stream.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {

  //! Class to help with parsing a CSV file.
  //! SMALL Object

  class ParseCSVC
  {
  public:
    //! Constructor
    //! expectedColumns - Number of columns expected in file, 0 if unknown.
    //! expectHeader - Are column headers included as the first record in the file.
    //! separators - Column separators to use.
    ParseCSVC(UIntT expectedColumns = 0,
              bool expectHeader = false,
              const char *seperators=",",
              bool throwExceptionOnParseError = false);

    //! Start parsing a stream;
    bool Open(IStreamC &strm);

    //! Start parsing a stream;
    bool Open(const StringC &filename);

    //! Set number of columns to expect.
    // Setting to zero allows variable lengths.
    void SetColumns(UIntT cols)
    { m_columns = cols; }

    //! Access number of columns
    UIntT ColumnCount() const
    { return m_columns; }

    //! Access header names.
    const SArray1dC<StringC> &Headers() const
    { return m_headers; }

    //! Read values from the file.
    bool ReadValues(SArray1dC<StringC> &values);

    //! Read line from stream.
    //! Returns false if none found.
    bool ReadLine(StringC &text);

    //! Read next non-empty line (ignoring whitespace) from stream.
    //! Returns false if none found.
    bool ReadNonEmptyLine(StringC &text);

    //! Test if we're at the end of the stream.
    bool IsEndOfStream() const
    { return !m_strm.good(); }

  protected:
    IStreamC m_strm;
    UIntT m_columns;
    bool m_expectHeaders;
    const char *m_seperators;
    bool m_throwExceptionOnParseError;
    SArray1dC<StringC> m_headers;
  };
}

#endif
