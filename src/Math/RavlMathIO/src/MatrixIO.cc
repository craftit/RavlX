// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////////////
//! lib=RavlMathIO
//! file="Ravl/Math/IO/MatrixIO.cc"

#include "Ravl/Matrix.hh"
#include "Ravl/Vector.hh"
#include "Ravl/VectorMatrix.hh"
#include "Ravl/MeanCovariance.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN {
  static TypeNameC type1(typeid(MatrixC),"RavlN::MatrixC");
  static TypeNameC type2(typeid(VectorC),"RavlN::VectorC");
  static TypeNameC type3(typeid(VectorMatrixC),"RavlN::VectorMatrixC");
  static TypeNameC type4(typeid(SArray1dC<MatrixC>),"RavlN::SArray1dC<MatrixC>");
  static TypeNameC type5(typeid(SArray1dC<VectorC>),"RavlN::SArray1dC<VectorC>");
  static TypeNameC type6(typeid(SArray1dC<VectorMatrixC>),"RavlN::SArray1dC<VectorMatrixC>");
  static TypeNameC type7(typeid(MeanCovarianceC),"RavlN::MeanCovarianceC");
  

  static RavlN::RCWrapAbstractC MatrixXMLFactory(const XMLFactoryContextC &node)
  {
    MatrixC values;
    if(!node.Attribute("values",values)) {
      throw RavlN::ExceptionBadConfigC("No values specified for the matrix");
    }
    return RavlN::RCWrapC<MatrixC >(values);
  }

  static RavlN::RCWrapAbstractC VectorXMLFactory(const XMLFactoryContextC &node)
  {
    VectorC values;
    if(!node.Attribute("values",values)) {
      throw RavlN::ExceptionBadConfigC("No values specified for the vector");
    }
    return RavlN::RCWrapC<VectorC >(values);
  }

  static RavlN::RCWrapAbstractC VectorMatrixXMLFactory(const XMLFactoryContextC &node)
  {
    VectorC vec;
    if(!node.Attribute("vector",vec)) {
      throw RavlN::ExceptionBadConfigC("No values specified for the vector component");
    }
    MatrixC mat;
    if(!node.Attribute("matrix",mat)) {
      throw RavlN::ExceptionBadConfigC("No values specified for the matrix component");
    }
    VectorMatrixC value(vec,mat);
    return RavlN::RCWrapC<VectorMatrixC >(value);
  }

  static RavlN::RCWrapAbstractC MeanCovarianceXMLFactory(const XMLFactoryContextC &node)
  {

    RealT samples;
    if(!node.Attribute("samples", samples)) {
      throw RavlN::ExceptionBadConfigC("No values specified for the samples component");
    }

    VectorC vec;
    if(!node.Attribute("mean",vec)) {
      throw RavlN::ExceptionBadConfigC("No values specified for the vector component");
    }

    MatrixC mat;
    if(!node.Attribute("covariance",mat)) {
      throw RavlN::ExceptionBadConfigC("No values specified for the matrix component");
    }
    MeanCovarianceC value(samples,vec,mat);
    return RavlN::RCWrapC<MeanCovarianceC>(value);
  }


  static int InitMatrixXMLFactory() {
    XMLFactoryC::RegisterTypeFactory(typeid(RavlN::MatrixC),&MatrixXMLFactory);
    XMLFactoryC::RegisterTypeFactory(typeid(RavlN::VectorC),&VectorXMLFactory);
    XMLFactoryC::RegisterTypeFactory(typeid(RavlN::VectorMatrixC),&VectorMatrixXMLFactory);
    XMLFactoryC::RegisterTypeFactory(typeid(RavlN::MeanCovarianceC),&MeanCovarianceXMLFactory);
    return 0;
  }

  static bool initXMLFactoryStuff = InitMatrixXMLFactory() != 0;

  static FileFormatStreamC<MatrixC> FileFormatStream_Matrix;
  static FileFormatStreamC<VectorC> FileFormatStream_Vector;
  static FileFormatStreamC<VectorMatrixC> FileFormatStream_VectorMatrix;
  static FileFormatStreamC<MeanCovarianceC> FileFormatStream_MeanCovariance;
  
  static FileFormatBinStreamC<MatrixC> FileFormatBinStream_Matrix;
  static FileFormatBinStreamC<VectorC> FileFormatBinStream_Vector;
  static FileFormatBinStreamC<VectorMatrixC> FileFormatBinStream_VectorMatrix;
  static FileFormatBinStreamC<MeanCovarianceC> FileFormatBinStream_MeanCovariance;
  
  static FileFormatStreamC<SArray1dC<MatrixC> > FileFormatStream_SArray1dC_Matrix;
  static FileFormatStreamC<SArray1dC<VectorC> > FileFormatStream_SArray1dC_Vector;
  static FileFormatStreamC<SArray1dC<VectorMatrixC> > FileFormatStream_SArray1dC_VectorMatrix;
  
  static FileFormatBinStreamC<SArray1dC<MatrixC> > FileFormatBinStream_SArray1dC_Matrix;
  static FileFormatBinStreamC<SArray1dC<VectorC> > FileFormatBinStream_SArray1dC_Vector;
  static FileFormatBinStreamC<SArray1dC<VectorMatrixC> > FileFormatBinStream_SArray1dC_VectorMatrix;

  static VectorC SArray1d_RealT2Vector(const SArray1dC<RealT> &vec)
  { return VectorC(vec); }

  static SArray1dC<RealT> Vector2SArray1d(const VectorC &vec)
  { return vec; }

  DP_REGISTER_CONVERSION(SArray1d_RealT2Vector,1);
  DP_REGISTER_CONVERSION(Vector2SArray1d,1);

  void InitMatrixIO()
  {}

}
