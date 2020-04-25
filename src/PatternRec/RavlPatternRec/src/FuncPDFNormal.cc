// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncPDFNormal.cc"

#include "Ravl/PatternRec/FuncPDFNormal.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/VirtualConstructor.hh"

namespace RavlN {
  
  //: Load from stream.
  
  FuncPDFNormalBodyC::FuncPDFNormalBodyC(std::istream &strm)
    : FuncPDFBodyC(strm)
  { strm >> dists; }
  
  //: Load from binary stream.
  
  FuncPDFNormalBodyC::FuncPDFNormalBodyC(BinIStreamC &strm)
    : FuncPDFBodyC(strm)
  { strm >> dists; }
  
  //: Writes object to stream.
  
  bool FuncPDFNormalBodyC::Save (std::ostream &out) const {
    if(!FuncPDFBodyC::Save(out))
      return false;
    out << dists << ' ';
    return true;
  }
  
  //: Writes object to binary stream.
  
  bool FuncPDFNormalBodyC::Save (BinOStreamC &out) const {
    if(!FuncPDFBodyC::Save(out))
      return false;
    out << dists;
    return true;
  }
    

  //: Create from an array of distributions.
  
  FuncPDFNormalBodyC::FuncPDFNormalBodyC(const SArray1dC<NormalC> &ndists)
    : FuncPDFBodyC(0,ndists.Size()),
      dists(ndists)
  {
    if(!ndists.IsEmpty())
      InputSize(ndists[0].Size());
  }
  
  //: Apply function to 'data'

  VectorC FuncPDFNormalBodyC::Apply(const VectorC &data) const {
    VectorC ret(dists.Size());
    for(BufferAccessIter2C<RealT,NormalC> it(ret,dists);it;it++)
      it.Data1() = it.Data2().Evaluate(data);
    return ret;
  }
  
  ////////////////////////////////////////////////////////////////////////
  
  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FuncPDFNormalBodyC,FuncPDFNormalC,FuncPDFC);
  
  
}
