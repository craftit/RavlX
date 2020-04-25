// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_UUId_HEADER
#define RAVL_UUId_HEADER 1
//! lib=RavlCore

#include "Ravl/String.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {

  //! Universally Unique Identifier

  class UUIdC
  {
  public:
    typedef unsigned char rawUUId_t[16];

    //! Default constructor.
    UUIdC();

    //! Constructor
    UUIdC(UInt64T v1,UInt64T v2)
    {
      m_val[0] = v1;
      m_val[1] = v2;
    }

    //! Read from a string
    UUIdC(const char *text);

    //! Parse UUId string
    bool Extract(const char *str);

    //! Test if its a valid uuid.
    //! All zeros fails, which is the value constructed by the default constructor.
    bool IsValid() const;

    //! Access first half of uuid
    UInt64T Value1() const
    { return m_val[0]; }

    //! Access second half of uuid
    UInt64T Value2() const
    { return m_val[1]; }

    //! Access raw bytes
    const rawUUId_t &Raw() const
    { return m_raw; }

    //! Access raw bytes
    rawUUId_t &Raw()
    { return m_raw; }

    //! Obtain an text description of the UUId.
    StringC Text() const;

    //! Compute a hash value
    size_t Hash() const;
  protected:
    union {
      UInt64T m_val[2];
      rawUUId_t m_raw;
    };
  };

  //! Compare two id's
  bool operator==(const UUIdC &u1,const UUIdC &u2);

  //! Compare two id's
  bool operator!=(const UUIdC &u1,const UUIdC &u2);

  //! Write an id to a binary stream
  BinOStreamC &operator<<(BinOStreamC &strm,const UUIdC &uuid);

  //! Read an id from a binary stream
  BinIStreamC &operator>>(BinIStreamC &strm,UUIdC &uuid);

  //! Write an id to a binary stream
  std::ostream &operator<<(std::ostream &strm,const UUIdC &uuid);

  //! Read an id from a binary stream
  std::istream &operator>>(std::istream &strm,UUIdC &uuid);


}

#endif
