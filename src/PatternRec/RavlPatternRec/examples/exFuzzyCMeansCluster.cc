// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Cluster"
//! userlevel=Normal
//! file="Ravl/PatternRec/Cluster/exFuzzyCMeansCluster.cc"

#include "Ravl/PatternRec/DesignFuzzyCMeansCluster.hh"
#include "Ravl/PatternRec/DataSet1.hh"
#include "Ravl/PatternRec/DataSet1Iter.hh"
#include "Ravl/PatternRec/SampleVector.hh"

using namespace RavlN;

int main(int nargs,char **argv) {
  
  DataSet1C<SampleVectorC> dataset(true);
  
  dataset.Append(VectorC(0.1,0.1));
  dataset.Append(VectorC(0.3,0.2));
  dataset.Append(VectorC(0.2,0.1));
  dataset.Append(VectorC(0.4,0.9));
  dataset.Append(VectorC(0.3,0.8));
  dataset.Append(VectorC(0.5,0.9));
  dataset.Append(VectorC(0.9,0.3));
  dataset.Append(VectorC(0.9,0.2));
  dataset.Append(VectorC(0.9,0.4));
  
  DesignFuzzyCMeansClusterC cluster(3); // Generate 3 clusters
  
  // Do clustering.
  
  ClassifierC classifier = cluster.Apply(dataset.Sample1());
  
  // In which cluster is each vector ?
  
  for(DataSet1IterC<SampleVectorC> it(dataset);it;it++) {
    UIntT cluster = classifier.Classify(*it);
    cerr << "Vector " << *it << " is in cluster " << cluster << "\n";
  }
  return 0;
}
