// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/UUId.hh"
#include <stdio.h>
#include <string.h>

namespace RavlN {
  //! Default constructor.
  UUIdC::UUIdC()
  {
    m_val[0] = 0;
    m_val[1] = 0;
  }

  //! Read from a string
  UUIdC::UUIdC(const char *text)
  {
    if(!Extract(text)) {

    }
  }

  //! Test if its a valid uuid.
  //! All zeros fails.
  bool UUIdC::IsValid() const
  { return !((m_val[0] == 0) && (m_val[1] == 0)); }

  //! Parse UUId string
  bool UUIdC::Extract(const char *str) {
    if(str == 0)
      return false;
    int len = strlen(str);
    if(len != 36)
      return false;
    if(str[8] != '-' || str[13] != '-' || str[18] != '-' || str[23] != '-')
      return false;

    UIntT v1 = 0;
    UIntT v2 = 0;
    UIntT v3 = 0;
    UIntT v4 = 0;
    UIntT v5 = 0;
    UIntT v6 = 0;

    int n = sscanf(str,"%08X-%04X-%04X-%04X-%04X%08X",&v1,&v2,&v3,&v4,&v5,&v6);
    if(n != 6) {
      std::cerr << "Only read " << n << " values. \n";
      return false;
    }
#if 0
    StringC tmp;
    tmp.form("%08X-%04X-%04X-%04X-%04X%08X",v1,v2,v3,v4,v5,v6);
    std::cerr << "Recovered:" << tmp << "\n";
#endif

    m_val[0] = ((UInt64T) v1) << 32 | ((UInt64T) v2) << 16 | v3;
    m_val[1] = ((UInt64T) v4) << 48 | ((UInt64T) v5) << 32 | v6;
    return true;
  }


  //! Obtain an text description of the UUId.
  StringC UUIdC::Text() const
  {
    StringC ret;
    // 8 4 4  4 12

    UIntT v1 = (m_val[0] >> 32) & 0xffffffff;
    UIntT v2 = (m_val[0] >> 16) & 0xffff;
    UIntT v3 = (m_val[0]) & 0xffff;
    UIntT v4 = (m_val[1] >> 48) & 0xffff;
    UIntT v5 = (m_val[1] >> 32) & 0xffff;
    UIntT v6 = m_val[1] & 0xffffffff;
    ret.form("%08X-%04X-%04X-%04X-%04X%08X",v1,v2,v3,v4,v5,v6);
    return ret;
  }

  //! Compute a hash value
  size_t UUIdC::Hash() const {
    return StdHash(m_val[0]) + StdHash(m_val[1]);
  }



  //! Compare two id's
  bool operator==(const UUIdC &u1,const UUIdC &u2) {
    return u1.Value1() == u2 .Value1() && u1.Value2() == u2.Value2();
  }

  //! Compare two id's
  bool operator!=(const UUIdC &u1,const UUIdC &u2) {
    return u1.Value1() != u2 .Value1() || u1.Value2() != u2.Value2();
  }

  //! Write an id to a binary stream
  BinOStreamC &operator<<(BinOStreamC &strm,const UUIdC &uuid) {
    strm << uuid.Value1() << uuid.Value2();
    return strm;
  }

  //! Read an id from a binary stream
  BinIStreamC &operator>>(BinIStreamC &strm,UUIdC &uuid) {
    UInt64T vals[2];
    strm >> vals[0];
    strm >> vals[1];
    uuid = UUIdC(vals[0],vals[1]);
    return strm;
  }

  //! Write an id to a binary stream
  std::ostream &operator<<(std::ostream &strm,const UUIdC &uuid)
  {
    strm << uuid.Text();
    return strm;
  }

  //! Read an id from a binary stream
  std::istream &operator>>(std::istream &strm,UUIdC &uuid)
  {
    StringC txt;
    strm >> txt;
    uuid = UUIdC(txt.chars());
    return strm;
  }
}
