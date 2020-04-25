// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/DimensionReduction/DesignFuncReduce.cc"

#include "Ravl/PatternRec/DesignFuncReduce.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  //: Default constructor.
  
  DesignFuncReduceBodyC::DesignFuncReduceBodyC(RealT variationPreserved)
    : varPreserved(variationPreserved)
  {}
  
  //: Load from stream.
  
  DesignFuncReduceBodyC::DesignFuncReduceBodyC(istream &strm) 
    : DesignFunctionUnsupervisedBodyC(strm)
  {
    strm >> varPreserved;
  }
  
  //: Load from binary stream.
  
  DesignFuncReduceBodyC::DesignFuncReduceBodyC(BinIStreamC &strm)
    : DesignFunctionUnsupervisedBodyC(strm)
  {
    strm >> varPreserved;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignFuncReduceBodyC::Save(ostream &out) const {
    if(!DesignFunctionUnsupervisedBodyC::Save(out))
      return false;
    out << ' ' << varPreserved;
    return true;
  }
  
  //: Writes object to stream, can be loaded using constructor
  
  bool DesignFuncReduceBodyC::Save(BinOStreamC &out) const {
    if(!DesignFunctionUnsupervisedBodyC::Save(out))
      return false;
    out << varPreserved;
    return true;
  }


}
