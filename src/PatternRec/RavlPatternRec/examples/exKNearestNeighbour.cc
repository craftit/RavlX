// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! author="Charles Galambos"
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! docentry="Ravl.API.Pattern Recognition.Classifier"
//! userlevel=Normal
//! file="Ravl/PatternRec/Classify/exKNearestNeighbour.cc"

#include "Ravl/PatternRec/DesignKNearestNeighbour.hh"
#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleLabel.hh"

using namespace RavlN;

int main(int nargs,char **argv) {
  
  DataSet2C<SampleVectorC,SampleLabelC> dataset(true);
  
  dataset.Append(VectorC(0.1,0.1),1);
  dataset.Append(VectorC(0.3,0.2),1);
  dataset.Append(VectorC(0.2,0.1),1);
  dataset.Append(VectorC(0.4,0.9),2);
  dataset.Append(VectorC(0.3,0.8),2);
  dataset.Append(VectorC(0.5,0.9),2);
  dataset.Append(VectorC(0.9,0.3),3);
  dataset.Append(VectorC(0.9,0.2),3);
  dataset.Append(VectorC(0.9,0.4),3);
  
  
  // Construct a designer.
  
  DesignKNearestNeighbourC designer(3);
  
  // Build a classifier.
  
  ClassifierC classifier = designer.Apply(dataset.Sample1(),dataset.Sample2());
  
  // In which cluster is each vector ?
  
  for(DataSet2IterC<SampleVectorC,SampleLabelC> it(dataset);it;it++) {
    UIntT cluster = classifier.Classify(it.Data1());
    std::cerr << "Vector " << it.Data1() << " is in cluster " << cluster << " (True=" << it.Data2() << ")\n";
  }
  return 0;
}
