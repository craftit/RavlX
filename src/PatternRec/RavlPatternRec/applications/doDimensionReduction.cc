// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec

#include "Ravl/Option.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/Resource.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/PatternRec/DataSet2Iter.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/PatternRec/DataSetIO.hh"
#include "Ravl/PatternRec/DesignFuncLDA.hh"
#include "Ravl/PatternRec/DesignFuncPCA.hh"
#include "Ravl/Plot/Plot2d.hh"


using namespace RavlN;

#define USE_EXCEPTIONS 0

// Simple program which performs leave one out

int main(int nargs, char **argv) {

  OptionC opts(nargs, argv);

  StringC installDir = opts.String("i", PROJECT_OUT, "Install directory.");
  RavlN::SetResourceRoot(installDir);
  //StringC configFile = opts.String("c", RavlN::Resource("Ravl/PatternRec", "doDimensionReduction.xml"),"Dimension r config file.");
  //StringC classifierType = opts.String("classifier", "KNN", "The type of classifier to train [KNN|GMM|SVM|NN2|LR|SVMOneClass].");

  StringC dataSetFile = opts.String("d", "", "The data set to processor");
  RealT variationPreserved = opts.Real("vp",0.99,"Variation preserved");
  bool doPCA = opts.Boolean("pca",true,"Do PCA ");
  bool doLDA = opts.Boolean("lda",false,"Do LDA ");
  bool plotData = opts.Boolean("pl",true,"Plot statistics. ");

  unsigned maxVecPlot = opts.Int("mp",6,"Maximum number of eigen vector plots to display.");
  FilenameC classifierOutFile = opts.String("o", "function.abs", "Reduction function.");

  //bool verbose = opts.Boolean("v", false, "Verbose mode.");
  opts.Check();

  SysLogOpen("doDimensionReduction",false,true,true,-1,true);

#if USE_EXCEPTIONS
  try {
#endif
    //XMLFactoryContextC context(configFile);

    DataSetVectorLabelC dataSet; // the normalised data set
    if(!LoadDataSetVectorLabel(dataSetFile,dataSet)) {
      RavlError("Failed to load data set '%s'", dataSetFile.data());
      return 1;
    }
    if(doPCA) {
      DesignFuncPCAC pca(variationPreserved);

      FunctionC func = pca.Apply(dataSet.Sample1());

      if(plotData) {
        VectorMatrixC vm = pca.Pca();
        RavlDebug("Eigen values:%s ",RavlN::StringOf(vm.Vector()).c_str());
        RavlDebug("Matrix: %d %d  ",vm.Matrix().Rows().V(),vm.Matrix().Cols().V());

        Plot2dC::RefT eigenPlot = CreatePlot2d(dataSetFile + " Eigen values");
        eigenPlot->Plot(vm.Vector(),"Eigen values");

        Plot2dC::RefT meanPlot = CreatePlot2d(dataSetFile + " Mean");
        meanPlot->SetLineStyle("line");
        meanPlot->Plot(pca.Mean(),"Mean");

        for(unsigned i = 0;i < maxVecPlot && i < vm.Matrix().Rows();i++) {
          StringC name =StringC("Vector ") + StringC(i);
          Plot2dC::RefT vecPlot = CreatePlot2d(dataSetFile + " vec " + StringC(i));
          vecPlot->SetLineStyle("line");
          vecPlot->Plot(vm.Matrix().SliceRow(i),name);
        }
      }
    }
    if(doLDA) {
      DesignFuncLDAC lda(variationPreserved);

      FunctionC func = lda.Apply(dataSet);

      RavlDebug("Matrix: %d %d  ",lda.Lda().Rows().V(),lda.Lda().Cols().V());

      Plot2dC::RefT meanPlot = CreatePlot2d(dataSetFile + " Mean");
      meanPlot->SetLineStyle("line");
      meanPlot->Plot(lda.Mean(),"Mean");


      for(unsigned i = 0;i < maxVecPlot && i < lda.Lda().Rows();i++) {
        StringC name =StringC("Vector ") + StringC(i);
        Plot2dC::RefT vecPlot = CreatePlot2d(dataSetFile + " vector ");
        vecPlot->SetLineStyle("line");
        vecPlot->Plot(lda.Lda().SliceRow(i),name);
      }

    }

#if 0
    // And save the classifier
    RavlInfo( "Saving function to '%s'", classifierOutFile.data());
    if (!Save(classifierOutFile, func)) {
      RavlError( "Trouble saving classifier");
      return 1;
    }
#endif

#if USE_EXCEPTIONS
  } catch (const RavlN::ExceptionC &exc) {
    RavlError( "Exception:%s", exc.Text());
  } catch (...) {
    RavlError( "Unknown exception");
  }
#endif
}
