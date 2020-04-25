// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation


#include "Ravl/Genetic/GeneticOptimiser.hh"
#include "Ravl/Genetic/GeneticOptimiserCheckPoint.hh"
#include "Ravl/Genetic/GenomeConst.hh"
#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/EvaluateFitnessFunc.hh"
#include "Ravl/Genetic/GeneFactory.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/Option.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/Resource.hh"
#include "Ravl/DP/PrintIOInfo.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/DP/TypeConverter.hh"

#define CATCH_EXCEPTIONS 1

using namespace RavlN::GeneticN;


float EvaluateFitness(RavlN::Point2dC &point) {
  static RavlN::Point2dC target(0.1234,0.4321);
  return static_cast<float>(1.0/(point.EuclideanDistance(target) + 0.0001));
}

// Define how to construct our target object.
static RavlN::GeneticN::GeneTypeFloatC::RefT g_numType = new RavlN::GeneticN::GeneTypeFloatC("coord",0.0,1.0);
static RavlN::TypeNameC g_type1(typeid(RavlN::Point2dC),"RavlN::Point2dC");

RavlN::Point2dC ConvertGeneFactory2Point2d(const RavlN::GeneticN::GeneFactoryC &factory)
{
  float x,y;
  factory.Get("x",x,*g_numType);
  factory.Get("y",y,*g_numType);
  //RavlDebug("Values: %f %f ",x,y);
  return RavlN::Point2dC(x,y);
}

DP_REGISTER_CONVERSION(ConvertGeneFactory2Point2d,1.0);


int main(int nargs,char **argv)
{

  RavlN::OptionC opt(nargs,argv);
  RavlN::SetResourceRoot(opt.String("i", PROJECT_OUT, "Install location. "));
  RavlN::StringC configFile = opt.String("c", RavlN::Resource("Ravl/Genetic", "exGeneticOptimisation.xml"), "Configuration file");
  bool listConv = opt.Boolean("lc",false,"List conversions");
  bool verbose = opt.Boolean("v",false,"Verbose logging.");
  opt.Check();

  RavlN::SysLogOpen("exGeneticOptimisation",false,true,true,-1,verbose);

  if(listConv) {
    RavlN::PrintIOConversions(std::cout);
    return 0;
  }
  RavlInfo("Started.");

#if CATCH_EXCEPTIONS
  try {
#endif

    RavlN::XMLFactoryContextC factory(configFile);

    GeneticOptimiserC::RefT optimiser;

    GeneticOptimiserCheckPointC::RefT optimiserCheckPoint;

    if(!factory.UseComponent("Optimiser",optimiser)) {
      RavlError("Failed to find optimiser.");
      return 1;
    }

    if(!factory.UseComponent("OptimiserCheckPoint",optimiserCheckPoint)) {
      RavlError("Failed to find optimiser checkpoint.");
      return 1;
    }

    optimiser->SetFitnessFunction(*new RavlN::GeneticN::EvaluateFitnessFuncC<RavlN::Point2dC>(&EvaluateFitness));

    RavlInfo("Running optimisation.");

    optimiserCheckPoint->Start();

    optimiser->Run();

    optimiserCheckPoint->Shutdown();

    RavlInfo("Optimisation complete");

#if CATCH_EXCEPTIONS
  } catch(...) {
    RavlError("Caught exception running model.");
  }
#endif

  return 0;
}

