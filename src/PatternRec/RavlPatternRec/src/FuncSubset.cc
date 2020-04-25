// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! author="Charles Galambos"
//! file="Ravl/PatternRec/Modeling/DimensionReduction/FuncSubset.cc"

#include "Ravl/PatternRec/FuncSubset.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Load from stream.

  FuncSubsetBodyC::FuncSubsetBodyC(std::istream &strm) :
      FunctionBodyC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if (version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("FuncSubsetBodyC");
    strm >> inds;
  }

  //: Load from binary stream.

  FuncSubsetBodyC::FuncSubsetBodyC(BinIStreamC &strm) :
      FunctionBodyC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if (version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("FuncSubsetBodyC");
    strm >> inds;
  }

  //: Writes object to stream.

  bool FuncSubsetBodyC::Save(std::ostream & strm) const
  {
    FunctionBodyC::Save (strm);
    ByteT version = 1;
    strm << version << endl;
    strm << inds << endl;
    return true;
  }

  //: Writes object to binary stream.

  bool FuncSubsetBodyC::Save(BinOStreamC &strm) const
  {
    FunctionBodyC::Save(strm);
    ByteT version = 1;
    strm << version << inds;
    return true;
  }

  //: Constructor from an array of indexes.
  
  FuncSubsetBodyC::FuncSubsetBodyC(const SArray1dC<IndexC> &ninds, UIntT inSize) :
      inds(ninds)
  {
    OutputSize(inds.Size());
    InputSize(inSize);
  }
  
  //: Reduce the dimension of 'data'.
  
  VectorC FuncSubsetBodyC::Apply(const VectorC &data) const
  {
    VectorC ret(inds.Size());
    for (BufferAccessIter2C<IndexC, RealT> it(inds, ret); it; it++)
      it.Data2() = data[it.Data1()];
    return ret;
  }

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FuncSubsetBodyC,FuncSubsetC,FunctionC);

}
