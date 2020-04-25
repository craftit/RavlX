// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlChartDetector
//! docentry="Ravl.API.Images.Chart Detector"
//! file="Ravl/Image/Processing/ChartDetector/chartdet.cc"

//: Calibration Chart Detector
// Locates a calibration chart in an image.

#include "Ravl/Image/ChartDetector.hh"
#include "Ravl/Image/ChartLocalise.hh"
#include "Ravl/IO.hh"
#include "Ravl/StringList.hh"
#include "Ravl/Option.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Image/WarpProjective.hh"
#include "Ravl/Image/DrawCross.hh"
#include "Ravl/Image/DrawCircle.hh"

using namespace RavlN;
using namespace RavlImageN;

//: Load chart points

SArray1dC<Point2dC> LoadChartPoints(StringC &filename) {
  IStreamC is(filename);
  if(!is)
    return SArray1dC<Point2dC>();
  DListC<Point2dC> points;
  while(is) {
    StringC buffer;
    readline(is,buffer);
    StringListC lst(buffer);
    if(lst.Size() < 2)
      continue;
    DLIterC<StringC> it(lst);
    Point2dC p;
    for(int i = 0;i < 2;i++,it++)
      p[i] = it->RealValue();
    points.InsLast(p);
  }
  
  // Copy points to an array.
  SArray1dC<Point2dC> ret(points.Size());
  SArray1dIterC<Point2dC> rit(ret);
  for(DLIterC<Point2dC> it(points);it;it++,rit++)
    *rit = *it;
  return ret;

}

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  bool verbose = opt.Boolean("v",true,"Verbose mode. ");
  bool displayCourse = opt.Boolean("dc",false,"Display course localisation. ");
  bool displayMatches = opt.Boolean("dm",false,"Display matches. ");
  RealT inlierThreshold = opt.Real("id",10,"Inlier distance threshold. ");
  UIntT ransacIterations = (UIntT) opt.Int("ri",50000,"RANSAC iterations to use in search.");
  RealT acceptThreshold = opt.Real("at",0.7,"Threshold at which to just accept hypothesis ");
  UIntT minRegionSize = opt.Int("ms",100,"Minimum region size to consider.");
  UIntT minRegionMargin = opt.Int("mm",10,"Minimum region stability margin.");
  UIntT patchSize = opt.Int("p",40,"Patch size. ");
  StringC chartName = opt.String("c",PROJECT_OUT "/share/RAVL/pixmaps/CalibrationChart.png","Image of chart to search for. ");
  StringC chartCoordFile = opt.String("co",PROJECT_OUT "/share/RAVL/pixmaps/CalibrationChart.coords","Calibration coordinages in the chart. ");
  StringC matchFilename = opt.String("mf","","File containing matched coordinates.");
  StringC sceneName = opt.String("","image.pgm","Image in which to search for chart. ");
  opt.Check();
  
  // --- Load some data ---
  if(verbose)
    cerr << "Loading chart image " << chartName << "\n";
  ImageC<ByteT> chartImage;
  if(!Load(chartName,chartImage)) {
    cerr << "Failed to load image of chart. \n";
    return 1;
  }
  
  SArray1dC<Point2dC> chartPoints = LoadChartPoints(chartCoordFile);
  if(chartPoints.Size() == 0) {
    cerr << "Failed to load chart coordinates from " << chartCoordFile << " \n";
    return 1;
  }
  if(verbose)
    cerr << "Loaded " << chartPoints.Size() << " calibration points \n";
  
  if(verbose)
    cerr << "Loading scene image " << sceneName << "\n";
  ImageC<ByteT> sceneImage;
  if(!Load(sceneName,sceneImage)) {
    cerr << "Failed to load image to search. \n";
    return 1;
  }
  
  // --- Find approximate position of chart in the image ---
  if(verbose)
    cerr << "Setting up chart detector. \n";
  ChartDetectorC chartDetector(chartImage,minRegionSize,minRegionMargin);
  chartDetector.SetInlierThreshold(inlierThreshold);
  chartDetector.SetRANSACIterations(ransacIterations);
  chartDetector.SetAcceptThreshold(acceptThreshold);
  
  if(verbose)
    cerr << "Searching for chart. \n";
  
  Projection2dC transform;  
  if(!chartDetector.Apply(sceneImage,transform)) {
    cerr << "Failed to find the chart in the image. ";
    return 2;
  }
  
  if(verbose) 
    cerr << "Transform =" << transform << "\n";
  // --- Do fine localisation of chart --- 

  if(verbose)
    cerr << "Setting up fine localisation. \n";
  
  ChartLocaliseC localise(chartImage,chartPoints,patchSize);
  localise.SetVerbose(verbose);

  if(displayCourse) {
    // Put off display to here so we can use localise's Chart2Example routine.
    WarpProjectiveC<ByteT> pwarp(chartImage.Frame(),transform.Inverse());
    ImageC<ByteT> pimage = pwarp.Apply(sceneImage);
    //RavlN::Save("@X:Ref",patch);
    for(SArray1dIterC<Point2dC> it(chartPoints);it;it++)
      DrawCross(pimage,(ByteT) 0,Index2dC(localise.Chart2Example(*it)),4);
    
    Save("@X:PositionEstimate",pimage);
  }
  
  
  SArray1dC<Point2dC> matches;
  SArray1dC<bool> matchesOk;
  if(verbose)
    cerr << "Computing fine localisation. \n";
  localise.Apply(sceneImage,transform,matches,matchesOk);
  
  UIntT pass = 0;
  for(SArray1dIterC<bool> it(matchesOk);it;it++) {
    if(*it)
      pass++; 
  }
  
  if(verbose)
    cerr << "Found " << pass << " matches. \n";
  if(displayMatches) {
    ImageC<ByteT> simage(sceneImage.Copy());
    for(SArray1dIter2C<Point2dC,bool> it(matches,matchesOk);it;it++) {
      DrawCircle(simage,(ByteT) 255,it.Data1(),3);
      if(it.Data2()) 
	DrawCross(simage,(ByteT) 0,it.Data1(),3);
    }
    Save("@X:Matches",simage);
  }
  if(!matchFilename.IsEmpty()) {
    if(verbose)
      cerr << "Writing match file '" << matchFilename << "'\n";
    OStreamC os(matchFilename);
    if(!os) {
      cerr << "Failed to open output file '" << matchFilename << "'\n";
      return 1;
    }
    os << pass << "\n";
    for(SArray1dIter3C<Point2dC,Point2dC,bool> it(chartPoints,matches,matchesOk);it;it++) {
      if(it.Data3())
	os << it.Data1() << " 0 " << it.Data2() << "\n";
    }
  }
  
  // --- Actually do some calibration ?? ---
  
  if(verbose)
    cout << "Done. \n";
  return 0;
}
