// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2007, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_STL_HEADER
#define RAVL_STL_HEADER 1
//////////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/System/STL.hh"
//! author="Charles Galambos"
//! lib=RavlCore
//! date="12/11/2007"
//! docentry="Ravl.API.Core.STL"
//! userlevel=Normal

// STL Compatibly methods.

#include "Ravl/StdHash.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Traits.hh"
#include "Ravl/BufferSTLVector.hh"
#include <vector>
#include <set>
#include <string>
#include <typeinfo>
#include <map>

namespace RavlN {
  class BinOStreamC;
  class BinIStreamC;

  BinOStreamC &operator<<(BinOStreamC &strm,const std::string &str);
  //: Write an STL string to binary stream.

  BinIStreamC &operator>>(BinIStreamC &strm,std::string &str);
  //: Read an STL string from binary stream.

  template<typename Data1T,typename Data2T>
  inline BinOStreamC &operator<<(BinOStreamC &strm,const std::pair<Data1T,Data2T> &element) {
    strm << element.first << element.second;
    return strm;
  }
  //: Write to binary stream.

  template<typename Data1T,typename Data2T>
  inline BinIStreamC &operator>>(BinIStreamC &strm,std::pair<Data1T,Data2T> &element) {
    // This is a little brutal, but if there is a const used in the types
    // there is no other way of reading them in.
    strm >> const_cast<typename TraitsC<Data1T>::BaseTypeT &>(element.first);
    strm >> const_cast<typename TraitsC<Data2T>::BaseTypeT &>(element.second);
    return strm;
  }
  //: Read from binary stream.

  template<class DataT>
  BinOStreamC &WriteSTLContainer(BinOStreamC &strm, const DataT &container) {
    SizeT size = container.size();
    strm << size;
    typename DataT::const_iterator it;
    for (it = container.begin(); it != container.end(); ++it)
      strm << *it;
    return strm;
  }

  template<class DataT>
  BinIStreamC &ReadSTLContainer(BinIStreamC &strm, DataT &container) {
    SizeT size = 0;
    strm >> size;
    container.clear();
    for (SizeT i = 0; i < size; i++) {
      typename TraitsC<typename DataT::value_type>::BaseTypeT val;
      strm >> val;
      typename DataT::iterator it = container.end();
      container.insert(it, val);
    }
    return strm;
  }

  template<typename DataT>
  BinOStreamC &operator<<(BinOStreamC &strm,const std::vector<DataT> &vec)
  { return WriteSTLContainer(strm,vec); }
  //: Write an STL vector to binary stream.

  template<typename DataT>
  BinIStreamC &operator>>(BinIStreamC &strm,std::vector<DataT> &vec)
  { return ReadSTLContainer(strm,vec); }
  //: Read an STL vector from binary stream.

  template<typename DataT>
  BinOStreamC &operator<<(BinOStreamC &strm,const std::set<DataT> &vec)
  { return WriteSTLContainer(strm,vec); }
  //: Write an STL vector to binary stream.

  template<typename DataT>
  BinIStreamC &operator>>(BinIStreamC &strm,std::set<DataT> &vec)
  { return ReadSTLContainer(strm,vec); }
  //: Read an STL vector from binary stream.

  template<typename Data1T,typename Data2T,typename Data3T, typename Data4T>
  BinOStreamC &operator<<(BinOStreamC &strm,const std::map<Data1T,Data2T,Data3T,Data4T> &themap)
  { return WriteSTLContainer(strm,themap); }
  //: Write an STL map to a binary stream.

  template<typename Data1T,typename Data2T,typename Data3T, typename Data4T>
  BinIStreamC &operator>>(BinIStreamC &strm,std::map<Data1T,Data2T,Data3T,Data4T> &themap)
  { return ReadSTLContainer(strm,themap); }
  //: Read an STL map from a binary stream.

  inline UIntT StdHash(const std::type_info &ti)
  { return StdHash(ti.name()); }
  //: Hash value for type info

  inline UIntT StdHash(const std::string &ti)
  { return StdHash(ti.data()); }
  //: Hash value for STL string

  template<typename DataT>
  inline UIntT StdHash(const std::vector<DataT> &vec)
  {
    UIntT hv = vec.size();
    for(UIntT i = 0;i < vec.size();i++) {
      UIntT lv = StdHash(vec[i]);
      hv += lv ^ (lv << (i % 20));
    }
    return hv;
  }
  //: Hash value for STL vector.

  template<typename DataT>
  SArray1dC<DataT> SArrayOf(const std::vector<DataT> &data) {
    if(data.size() < 1) return SArray1dC<DataT>();
    return SArray1dC<DataT>(BufferSTLVectorC<DataT>(data),data.size());
  }
  //: Turn an STL vector into an SArray1dC.

  template<typename DataT>
  std::ostream &operator<<(std::ostream &strm,const std::vector<DataT> &value) {
    strm << value.size();
    for(unsigned i = 0;i < value.size();i++)
      strm  << " " << value[i];
    return strm;
  }
  //: Helpers for some RAVL templates.

  template<typename DataT>
  static std::istream &operator>>(std::istream &strm,std::vector<DataT> &value) {
    throw RavlN::ExceptionOperationFailedC("Not implemented ");
    return strm;
  }
  //: Helpers for some RAVL templates.

  template<typename KeyT,typename DataT>
  std::ostream &operator<<(std::ostream &strm,const std::map<KeyT,DataT> &value) {
    value.size();
    strm << value.size() << " ";
    for(typename std::map<KeyT,DataT>::const_iterator it = value.begin();it != value.end();it++) {
      strm << it->first << " = " << it->second << ";\n";
    }
    return strm;
  }
  //: Helpers for some RAVL templates.

  template<typename KeyT,class DataT>
  static std::istream &operator>>(std::istream &strm,std::map<KeyT,DataT> &value) {
    throw RavlN::ExceptionOperationFailedC("Not implemented ");
    return strm;
  }
  //: Helpers for some RAVL templates.

}


#endif
