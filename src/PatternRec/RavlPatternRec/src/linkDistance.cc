// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec

namespace RavlN {

  extern void linkDistance();
  extern void linkDistanceCityBlock();
  extern void linkDistanceEuclidean();
  extern void linkDistanceMahalanobis();
  extern void linkDistanceMax();
  extern void linkDistanceRobust();
  extern void linkDistanceSqrEuclidean();
  extern void linkDistanceChi2();

  void LinkDistance() {
    linkDistance();
    linkDistanceCityBlock();
    linkDistanceEuclidean();
    linkDistanceMahalanobis();
    linkDistanceMax();
    linkDistanceRobust();
    linkDistanceSqrEuclidean();
    linkDistanceChi2();
  }
  
}
