// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id: aamBuildPoseEstimator.cc,v 1.3 2005/07/25 16:31:07 ees1wc Exp $"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/aamBuildPoseEstimator.cc"
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Normal"
//! author="Jean-Yves Guillemaut"

#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Stream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/IO.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/Option.hh"
#include "Ravl/StringList.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/Image/Reflect.hh"
#include "Ravl/Image/HistogramEqualise.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/Text/TextFile.hh"

#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMShapeModel.hh"
#include "Ravl/Image/AAMScaleRotationShapeModel.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/Image/AAMActiveAppearanceModel.hh"

#include "Ravl/Image/AAMPoseEstimate.hh"

using namespace RavlN;
using namespace RavlImageN;

//: Build statistical model of pose variation.
//  This program reads an appearance model and a set of appearance files and uses them to build a statistical model of pose variation. A model of pose variation allows to estimate the correspondence between a shape or appearance of a face and its pose represented by the pan and tilt angle. The appearance files to be read are located in the directory defined by the option "-td" and are specified either by providing a list of file names (option "-tl") or by providing the extension of the appearance files (option "-e"). In addition, it is possible to double the effective number of appearances used for building the statistical model of pose variation by considering mirror appearances of the appearance defined in the files. This is done by providing a mirror file (option "-m"). The mirror file consists of a list of pairs of indices which represent feature points in correspondence when carrying out the vertical symmetry (mirror effect). The model of pose variation can be built either from the shape information only or from the shape information plus the grey-level information. The option "-uam" is used to control whether the grey-level information should be incorporated or not. For more information type "aamBuildPoseEstimator -help".
int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC trainDir = opt.String("td","/vol/vssp/vampire/people/jean-yves/appmodel/markup/","Directory containing markup files defining the appearances.");
  StringC trainList = opt.String("tl","/vol/vssp/vampire/people/jean-yves/appmodel/lists/pose_half1.list","Input file containing list of markup file names.");
  StringC ext = opt.String("e","xml","Extention to search for markup files (needed only if no list is provided, i.e. option -l is set to empty string \"\").");
  StringC in_am = opt.String("am","/vol/vssp/vampire/people/jean-yves/appmodel/models/am_all_half1_25x22m98_98_95.abs","Input statistical model of appearance.");
  StringC mirrorFile = opt.String("m","/vol/vssp/vampire/people/jean-yves/appmodel/mirror.txt","Input mirror file for generation of mirror appearances. To disable mirror appearances, the option -m should be set to the empty string \"\".");
  bool ignoreSuspect = opt.Boolean("is",true,"Ignore suspect markups (i.e. files tagged with 'suspect=\"1\"').");
  bool useAM = opt.Boolean("uam",false,"Include grey-level information in statistical model of pose variation? true=yes. If set to false, only the shape information will be used.");
  StringC op = opt.String("o","/vol/vssp/vampire/people/jean-yves/appmodel/models/pose_sm_all_half1_25x22m98_98_95.abs","Output file for statistical model of pose variation.");
  opt.Check();

  cout << "Creating list of marked-up files in '" << trainDir << "'\n";
  DListC<StringC> fileList;
  HashC<IntT,IntT> typeMap;
  HashC<StringC,IntT> namedTypeMap;
  bool useTypeId;
  if(!trainList.IsEmpty())
  {
    // load list from file
    TextFileC fl(trainList);
    for(UIntT i=1;i<=fl.NoLines();i++) {

      StringC fileName = fl[i].TopAndTail();

      // check it is a valid appearance and eliminate suspect files if required
      AAMAppearanceC appear = LoadFeatureFile(fileName,trainDir,typeMap,namedTypeMap,useTypeId,ignoreSuspect,true);
      if (!appear.IsValid()) {
        continue;
      }

      fileList.InsLast(fileName);
    }
  }
  else
  {
    DirectoryC md(trainDir);
    if(!md.IsValid()) {
      cerr << "Can't find directory '" << trainDir << "'\n";
      return 1;
    }
    DListC<StringC> fileList = md.List("",StringC(".") + ext);
  }

  if(!fileList.IsValid() || fileList.Size() == 0) {
    cerr << "Failed to create list of marked-up files from '" << trainDir << "' \n";
    return 1;
  }

  cerr  << "Got " << fileList.Size() << " samples. \n";

  // load in active appearance model
  AAMAppearanceModelC am;
  AAMShapeModelC sm;
  cerr << "Loading appearance model '" << in_am << "' \n";
  if(!Load(in_am,am)) {
    cerr << "Failed to load  model '" << in_am << "' \n";
    return 1;
  }
  sm = am.ShapeModel();

  IntT NoPerFile;
  AAMAppearanceMirrorC mirror;
  if (mirrorFile.IsEmpty()) {
    NoPerFile = 1;
  }
  else {
    mirror = AAMAppearanceMirrorC(mirrorFile);
    if(!mirror.IsValid()) {
      cerr << "ERROR: Failed to read mirror file. \n";
      return 1;
    }
    NoPerFile = 2;
  }

  SampleC<VectorC> samples(fileList.Size());
  SampleC<RealT> Tangles(fileList.Size());
  SampleC<RealT> Nangles(fileList.Size());
  RealT degT = (22.5/180) * RavlConstN::pi;
  RealT degN = (15.0/180) * RavlConstN::pi;

  for(DLIterC<StringC> it(fileList);it;it++) {
    for (IntT k=1;k<=NoPerFile;k++) {

      AAMAppearanceC appear = LoadFeatureFile(*it,trainDir,typeMap,namedTypeMap,useTypeId);
      if (k == 2) {
        appear = mirror.Reflect(appear);
      }

      StringC source_name = StringListC(appear.SourceFile(), ".").First();
      StringListC strlist(source_name, "_");
      StringC aT = strlist.Nth(3);
      StringC aN = strlist.Nth(2);

      VectorC vec;
      if(useAM) {
        vec = am.Parameters(appear).From(am.NoFixedParameters());
      }
      else {
        vec = sm.Parameters(appear).From(sm.NoFixedParameters());
      }
      samples.Append(vec.Copy());
      RealT sign=1.0;
      if(strlist.Last() == "reflect") sign = -1.0;
      if(aT == "067") Tangles.Append(sign * degT);
      else if((aT  == "090")||(aT  == "000")) Tangles.Append(0.0);
      else if(aT == "112") Tangles.Append(sign * -degT);
      else {
        cerr << "ERROR: Unknown angle \t" << aT  << endl;
        return 1;
      }
      if( aT == "000") Nangles.Append(0);
      else {
        if(aN == "0") Nangles.Append(2.0 * degN);
        else if(aN  == "1")  Nangles.Append(degN);
        else if(aN == "2")  Nangles.Append(0);
        else if(aN  == "3")  Nangles.Append(-1.0* degN);
        else if(aN == "4") Nangles.Append(-2.0 * degN);
        else {
          cerr << "ERROR: Unknown angle \t" << aN  << endl;
          return 1;
        }
      }
    }
  }

  // Now estimate the pose parameters
  AAMPoseEstimateC posest;

  posest.Design(samples, Tangles, Nangles);

  BinOStreamC fout(op);
  posest.Save(fout);

  return 0;
}

