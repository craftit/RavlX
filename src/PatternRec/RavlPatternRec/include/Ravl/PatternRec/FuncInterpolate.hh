// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNCINTERPOLATE_HEADER
#define RAVL_FUNCINTERPOLATE_HEADER 1
//! lib=RavlPatternRec
//! date="9/7/2011"
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/FuncInterpolate.hh"

#include "Ravl/PatternRec/Function.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/PatternRec/Distance.hh"
#include "Ravl/RealRange1d.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Interpolated function.

  class FuncInterpolateBodyC
    : public FunctionBodyC
  {
  public:
    FuncInterpolateBodyC();
    //: Construct from a transform matrix.

    FuncInterpolateBodyC(const SArray1dC<Tuple2C<VectorC,VectorC> > &data);
    //: Construct with an input and output size.

    FuncInterpolateBodyC(const SArray1dC<RealRangeC> &inputLimits,const SArray1dC<RealRangeC> &outputLimits);
    //: Constructor

    FuncInterpolateBodyC(const XMLFactoryContextC &factory);
    //: Factory constructor

    FuncInterpolateBodyC(std::istream &strm);
    //: Load from stream.

    FuncInterpolateBodyC(BinIStreamC &strm);
    //: Load from binary stream.

    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

  protected:
    SArray1dC<RealRangeC> m_inputLimits; //! Limits on expected input values.
    SArray1dC<RealRangeC> m_outputLimits; //! Limits on the output values.
  };


  //! userlevel=Normal
  //: Linear function.

  class FuncInterpolateC
    : public FunctionC
  {
  public:
    FuncInterpolateC()
    {}
    //: Default constructor.

    FuncInterpolateC(std::istream &strm);
    //: Load from stream.

    FuncInterpolateC(BinIStreamC &strm);
    //: Load from binary stream.

  protected:
    FuncInterpolateC(FuncInterpolateBodyC &bod)
      : FunctionC(bod)
    {}
    //: Body constructor.

    FuncInterpolateC(FuncInterpolateBodyC *bod)
      : FunctionC(bod)
    {}
    //: Body ptr constructor.

    FuncInterpolateBodyC &Body()
    { return static_cast<FuncInterpolateBodyC &>(FunctionC::Body()); }
    //: Access body.

    const FuncInterpolateBodyC &Body() const
    { return static_cast<const FuncInterpolateBodyC &>(FunctionC::Body()); }
    //: Access body.

  public:
  };

  inline std::istream &operator>>(std::istream &strm,FuncInterpolateC &obj) {
    obj = FuncInterpolateC(strm);
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.

  inline std::ostream &operator<<(std::ostream &out,const FuncInterpolateC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  inline BinIStreamC &operator>>(BinIStreamC &strm,FuncInterpolateC &obj) {
    obj = FuncInterpolateC(strm);
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.

  inline BinOStreamC &operator<<(BinOStreamC &out,const FuncInterpolateC &obj) {
    obj.Save(out);
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

}


#endif
