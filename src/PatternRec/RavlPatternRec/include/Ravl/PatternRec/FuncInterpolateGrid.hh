// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FuncInterpolateGridGRID_HH_
#define RAVL_FuncInterpolateGridGRID_HH_
//! lib=RavlPatternRec
//! date="9/7/2011"
//! author="Charles Galambos"
//! docentry="Ravl.API.Pattern Recognition.Numerical Modeling"
//! file="Ravl/PatternRec/Modeling/Basic/FuncInterpolateGrid.hh"

#include "Ravl/PatternRec/FuncInterpolate.hh"
#include "Ravl/RealHistogramNd.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Interpolated function.

  class FuncInterpolateGridBodyC
    : public FuncInterpolateBodyC
  {
  public:
    FuncInterpolateGridBodyC();
    //: Construct from a transform matrix.

    FuncInterpolateGridBodyC(const XMLFactoryContextC &factory);
    //: Factory constructor

    FuncInterpolateGridBodyC(const RealHistogramNdC<VectorC> &data,const SArray1dC<RealRangeC> &outputLimits);
    //: Constructor

    FuncInterpolateGridBodyC(std::istream &strm);
    //: Load from stream.

    FuncInterpolateGridBodyC(BinIStreamC &strm);
    //: Load from binary stream.

    virtual bool Save (std::ostream &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual bool Save (BinOStreamC &out) const;
    //: Writes object to stream, can be loaded using constructor

    virtual VectorC Apply(const VectorC &data) const;
    //: Apply transform to data.

  protected:
    RealHistogramNdC<VectorC> m_data;
  };


  //! userlevel=Normal
  //: Linear function.

  class FuncInterpolateGridC
    : public FuncInterpolateC
  {
  public:
    FuncInterpolateGridC()
    {}
    //: Default constructor.

    FuncInterpolateGridC(std::istream &strm);
    //: Load from stream.

    FuncInterpolateGridC(BinIStreamC &strm);
    //: Load from binary stream.

    FuncInterpolateGridC(const RealHistogramNdC<VectorC> &data,const SArray1dC<RealRangeC> &outputLimits)
     : FuncInterpolateC(new FuncInterpolateGridBodyC(data,outputLimits))
    {}
    //: Constructor

  protected:
    FuncInterpolateGridC(FuncInterpolateGridBodyC &bod)
      : FuncInterpolateC(bod)
    {}
    //: Body constructor.

    FuncInterpolateGridC(FuncInterpolateGridBodyC *bod)
      : FuncInterpolateC(bod)
    {}
    //: Body ptr constructor.

    FuncInterpolateGridBodyC &Body()
    { return static_cast<FuncInterpolateGridBodyC &>(FunctionC::Body()); }
    //: Access body.

    const FuncInterpolateGridBodyC &Body() const
    { return static_cast<const FuncInterpolateGridBodyC &>(FunctionC::Body()); }
    //: Access body.

  public:
  };

}



#endif /* FuncInterpolateGridGRID_HH_ */
