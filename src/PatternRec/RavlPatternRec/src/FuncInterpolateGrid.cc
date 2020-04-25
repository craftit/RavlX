// Copyright (C) 2011, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/Modeling/Basic/FuncInterpolateGrid.cc"

#include "Ravl/PatternRec/FuncInterpolateGrid.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/MatrixRUT.hh"

#define DODEBUG	0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  //: Construct from a transform matrix.

  FuncInterpolateGridBodyC::FuncInterpolateGridBodyC()
  {
    ONDEBUG(std::cerr << "FuncInterpolateGridBodyC::FuncInterpolateGridBodyC(), Called. \n");
  }

  //: Factory constructor
  FuncInterpolateGridBodyC::FuncInterpolateGridBodyC(const XMLFactoryContextC &factory)
   : FuncInterpolateBodyC(factory)
  {}

  //: Constructor

  FuncInterpolateGridBodyC::FuncInterpolateGridBodyC(const RealHistogramNdC<VectorC> &data,const SArray1dC<RealRangeC> &outputLimits)
  : FuncInterpolateBodyC(data.InputRange(),outputLimits),
    m_data(data)
  {}

  //: Load from stream.

  FuncInterpolateGridBodyC::FuncInterpolateGridBodyC(std::istream &strm)
    : FuncInterpolateBodyC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //: Load from binary stream.

  FuncInterpolateGridBodyC::FuncInterpolateGridBodyC(BinIStreamC &strm)
    : FuncInterpolateBodyC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1) {
      throw ExceptionUnexpectedVersionInStreamC("FuncInterpolateGridBodyC");
    }
    strm >> m_data;
  }

  //: Writes object to stream.

  bool FuncInterpolateGridBodyC::Save (std::ostream &out) const
  {
    if(!FuncInterpolateBodyC::Save(out))
      return false;
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //: Writes object to binary stream.

  bool FuncInterpolateGridBodyC::Save (BinOStreamC &strm) const
  {
    if(!FuncInterpolateBodyC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << m_data;
    return true;
  }


  //: Apply function to 'data'

  VectorC FuncInterpolateGridBodyC::Apply(const VectorC &data) const
  {
    VectorC ret;
    m_data.Interpolate(data,ret);
    return ret;
  }

  void InitPattenRecFuncInterpolateGridGrid()
  {}

  ////////////////////////////////////////////////////////////////////////

  RAVL_INITVIRTUALCONSTRUCTOR_FULL(FuncInterpolateGridBodyC,FuncInterpolateGridC,FuncInterpolateC);

}
