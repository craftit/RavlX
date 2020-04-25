// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRecIO
//! file="Ravl/PatternRec/IO/DistanceIO.cc"

#include "Ravl/Vector.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Types.hh"

#include "Ravl/PatternRec/Distance.hh"
#include "Ravl/PatternRec/DistanceEuclidean.hh"
#include "Ravl/PatternRec/DistanceSqrEuclidean.hh"
#include "Ravl/PatternRec/DistanceCityBlock.hh"
#include "Ravl/PatternRec/DistanceMahalanobis.hh"
#include "Ravl/PatternRec/DistanceRobust.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN {
  void InitRavlDistanceIO()
  {}
  
  //:- DistanceC --------------------------------------------------------

  static TypeNameC type1(typeid(DistanceC),"RavlN::DistanceC");
  
  static FileFormatStreamC<DistanceC> FileFormatStream_Distance;
  static FileFormatBinStreamC<DistanceC> FileFormatBinStream_Distance;
  
  Function1C Distance2Function1(const DistanceC &dist)
  { return dist; }
  
  DP_REGISTER_CONVERSION_NAMED(Distance2Function1 ,1,
			       "RavlN::Function1C RavlN::Convert(const RavlN::DistanceC &)");
  
  //:- DistanceMahalanobisC --------------------------------------------------------
  
  static TypeNameC type2(typeid(DistanceMahalanobisC),"RavlN::DistanceMahalanobisC");
  
  static FileFormatStreamC<DistanceMahalanobisC> FileFormatStream_DistanceMahalanobis;
  static FileFormatBinStreamC<DistanceMahalanobisC> FileFormatBinStream_DistanceMahalanobis;

  DistanceC DistanceMahalanobis2Distance(const DistanceMahalanobisC &dist)
  { return dist; }
  
  DP_REGISTER_CONVERSION_NAMED(DistanceMahalanobis2Distance ,1,
			       "RavlN::DistanceC RavlN::Convert(const RavlN::DistanceMahalanobisC &)");
  
  //:- DistanceEuclideanC --------------------------------------------------------
  
  static TypeNameC type3(typeid(DistanceEuclideanC),"RavlN::DistanceEuclideanC");
  
  DistanceC DistanceEuclidean2Distance(const DistanceEuclideanC &dist)
  { return dist; }
  
  DP_REGISTER_CONVERSION_NAMED(DistanceEuclidean2Distance ,1,
			       "RavlN::DistanceC RavlN::Convert(const RavlN::DistanceEuclideanC &)");


  //:- DistanceEuclideanC --------------------------------------------------------
  
  static TypeNameC type4(typeid(DistanceSqrEuclideanC),"RavlN::DistanceSqrEuclideanC");
  
  DistanceC DistanceSqrEuclidean2Distance(const DistanceSqrEuclideanC &dist)
  { return dist; }
  
  DP_REGISTER_CONVERSION_NAMED(DistanceSqrEuclidean2Distance ,1,
			       "RavlN::DistanceC RavlN::Convert(const RavlN::DistanceSqrEuclideanC &)");
  
  
  //:- DistanceCityBlockC --------------------------------------------------------

  static TypeNameC type5(typeid(DistanceCityBlockC),"RavlN::DistanceCityBlockC");
  
  DistanceC DistanceCityBlock2Distance(const DistanceCityBlockC &dist)
  { return dist; }
  
  DP_REGISTER_CONVERSION_NAMED(DistanceCityBlock2Distance ,1,
			       "RavlN::DistanceC RavlN::Convert(const RavlN::DistanceCityBlockC &)");

  
  //:- DistanceRobustC --------------------------------------------------------
  
  static TypeNameC type6(typeid(DistanceRobustC),"RavlN::DistanceRobustC");

  static FileFormatStreamC<DistanceRobustC> FileFormatStream_DistanceRobusts;
  static FileFormatBinStreamC<DistanceRobustC> FileFormatBinStream_DistanceRobust;
  
  DistanceC DistanceRobust2Distance(const DistanceRobustC &dist)
  { return dist; }
  
  DP_REGISTER_CONVERSION_NAMED(DistanceRobust2Distance ,1,
			       "RavlN::DistanceC RavlN::Convert(const RavlN::DistanceRobustC &)");
  
  
}
