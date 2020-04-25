// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, James Smith
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlURLIO
//! file="Ravl/Contrib/Plotting/GnuPlot/exGnuPlot.cc"
//! author="Kieron Messer"
//! docentry="Ravl.API.Graphics.Plotting.GnuPlot"
//! userlevel=Normal

#include "Ravl/Plot/GnuPlot2d.hh"
#include "Ravl/Option.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/StdConst.hh"
#include "Ravl/IO.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Collection.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Vector2d.hh"
#include "Ravl/Resource.hh"
#include "Ravl/PatternRec/DataSetIO.hh"
#include "Ravl/PatternRec/DesignDiscriminantFunction.hh"
#include "Ravl/PatternRec/DesignFuncLSQ.hh"
#include "Ravl/PatternRec/DesignClassifierLogisticRegression.hh"
#include "Ravl/PatternRec/DesignClassifierNeuralNetwork2.hh"
#include "Ravl/PatternRec/DesignKNearestNeighbour.hh"
#include "Ravl/PatternRec/DesignWeakLinear.hh"

using namespace RavlN;

int exGnuPlot(int nargs, char *args[])
{

  // Get command-line options
  OptionC opt(nargs, args);

  opt.Check();

  /*
   * Load a data set
   */
  DataSetVectorLabelC iris;
  if (!LoadDataSetVectorLabel(PROJECT_OUT "/share/Ravl/PatternRec/iris.csv", iris)) {
    RavlError("Trouble loading iris dataset");
    return 1;
  }

  // really only want two dimensions...it will work with more, however the graphs
  // can be pretty meaningless as it is just a slice taken of a much larger
  // dimensional picture!
  SArray1dC<IndexC> featureSet(2);
  featureSet[0] = 0;
  featureSet[1] = 1;
  DataSetVectorLabelC trainData(SampleVectorC(iris.Sample1(), featureSet), iris.Sample2());

  // Linear classifier
  DesignFuncLSQC linear(1, false);
  DesignDiscriminantFunctionC designLinear(linear);
  GnuPlot2dC plotLinear("Linear Classifier");
  plotLinear.Plot(designLinear.Apply(trainData), trainData);

  // Quadratic classifier
  DesignFuncLSQC quadratic(2, false);
  DesignDiscriminantFunctionC designQuadratic(quadratic);
  GnuPlot2dC plotQuadratic("Quadratic Classifier");
  plotQuadratic.Plot(designQuadratic.Apply(trainData), trainData);

  // 3rd Order Polynomial classifier
  DesignFuncLSQC poly3(3, false);
  DesignDiscriminantFunctionC designPoly3(poly3);
  GnuPlot2dC plotPoly3("Poly3 Classifier");
  plotPoly3.Plot(designPoly3.Apply(trainData), trainData);

  // Logistic Regression
  DesignClassifierLogisticRegressionC designLR(0.0);
  GnuPlot2dC plotLR("Logistic Regression Classifier");
  plotLR.Plot(designLR.Apply(trainData), trainData);

  // 1 Nearest Neighbour, we would expect this to circle all the points
  DesignKNearestNeighbourC designKNN1(1);
  GnuPlot2dC plotKNN1("KNN-1 Classifier");
  plotKNN1.Plot(designKNN1.Apply(trainData), trainData);

  // 5 Nearest Neighbour, we would expect this to be a bit smoother!
  DesignKNearestNeighbourC designKNN5(5);
  GnuPlot2dC plotKNN5("KNN-5 Classifier");
  plotKNN5.Plot(designKNN5.Apply(trainData), trainData);

  // Ok lets do some NNets with different HU's...we would expect decisions to get a bit more crazy
  for (UIntT hu = 1; hu < 10; hu += 3) {
    DesignClassifierNeuralNetwork2C design(3, hu);
    StringC title = "Neural Network with " + (StringC) hu + " hidden units.";
    GnuPlot2dC classifierPlot(title);
    classifierPlot.Plot(design.Apply(trainData), trainData);
  }
  return 0;
}

RAVL_ENTRY_POINT(exGnuPlot);
