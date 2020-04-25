// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncInterpolate.cc"

#include "Ravl/PatternRec/FuncInterpolate.hh"
#include "Ravl/Exception.hh"

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  void InitPattenRecFuncInterpolate()
  {}

  //: Construct from a transform matrix.

  FuncInterpolateBodyC::FuncInterpolateBodyC() {
    ONDEBUG(std::cerr << "FuncInterpolateBodyC::FuncInterpolateBodyC(), Called. \n");
  }

  //: Constructor
  FuncInterpolateBodyC::FuncInterpolateBodyC(const SArray1dC<RealRangeC> &inputLimits,const SArray1dC<RealRangeC> &outputLimits)
   : FunctionBodyC(inputLimits.Size(),outputLimits.Size()),
     m_inputLimits(inputLimits), //! Limits on expected input values.
     m_outputLimits(outputLimits) //! Limits on the output values.
  {}

  //: Factory constructor

  FuncInterpolateBodyC::FuncInterpolateBodyC(const XMLFactoryContextC &factory)
   : FunctionBodyC(factory)
  {
    factory.Attribute("inputLimits",m_inputLimits);
    factory.Attribute("outputLimits",m_outputLimits);
  }

  //: Load from stream.

  FuncInterpolateBodyC::FuncInterpolateBodyC(std::istream &strm)
    : FunctionBodyC(strm)
  {
  }

  //: Load from binary stream.

  FuncInterpolateBodyC::FuncInterpolateBodyC(BinIStreamC &strm)
    : FunctionBodyC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("FuncInterpolateBodyC");
    strm >> m_inputLimits >> m_outputLimits ;
  }

  //: Writes object to stream.

  bool FuncInterpolateBodyC::Save (std::ostream &out) const
  { return FunctionBodyC::Save(out); }

  //: Writes object to binary stream.

  bool FuncInterpolateBodyC::Save (BinOStreamC &strm) const
  {
    FunctionBodyC::Save(strm);
    ByteT version = 1;
    strm << version << m_inputLimits << m_outputLimits ;
    return true;
  }


  ////////////////////////////////////////////////////////////////////////

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FuncInterpolateBodyC,FuncInterpolateC,FunctionC);

}
