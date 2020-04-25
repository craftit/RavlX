// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/aamBuildShapeModel.hh"
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Normal"
//! author="Jean-Yves Guillemaut"

#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/IO.hh"
#include "Ravl/Option.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/SArray1dIter2.hh"
#include "Ravl/Stream.hh"
#include "Ravl/DList.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/Text/TextFile.hh"

#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMShapeModel.hh"
#include "Ravl/Image/AAMScaleRotationShapeModel.hh"
#include "Ravl/Image/AAMAffineShapeModel.hh"
#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"

using namespace RavlN;
using namespace RavlImageN;

//: Build a statistical shape model.
//  This program reads a set of appearance files and uses them to build a statistical shape model.
// The appearance files to be read are located in the directory defined by the option "-d" and are specified either by providing a list of file names (option "-l") or by providing the extension of the appearance files (option "-e"). In addition, it is possible to double the effective number of appearances used for building the statistical shape model by considering mirror appearances of the appearance defined in the files. This is done by providing a mirror file (option "-m"). The mirror file consists of a list of pairs of indices which represent feature points in correspondence when carrying out the vertical symmetry (mirror effect). Among the options it is possible to control the proportion of variation preserved by the shape model (option "-vs") and to set a limit to the maximum number of parameters contained in the statistical shape model (option "-maxs"). For more information type "aamBuildShapeModel -help".
int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC dir = opt.String("d","./markup/","Directory containing markup files defining the appearances.");
  StringC list = opt.String("l","./all_half1.list","Input file containing list of markup file names.");
  StringC ext = opt.String("e","xml","Extention to search for markup files (needed only if no list is provided, i.e. option -l is set to empty string \"\").");
  StringC mirrorFile = opt.String("m","./mirror.txt","Input mirror file for generation of mirror appearances. To disable mirror appearances, the option -m should be set to the empty string \"\".");
  StringC mapFile = opt.String("p", "", "Input map file. Disabled if not specified");
  RealT var = opt.Real("vs",0.98,"Proportion of variation preserved by the PCA during dimension reduction (must be between 0 and 1).");
  UIntT maxP = opt.Int("maxs",1000,"Upper limit on number of parameters in the statistical shape model. If number of parameters required to explain the proportion of variation specified by option -vs exceeds this value, the number of parameters will be troncated to this value.");
  bool ignoreSuspect = opt.Boolean("is",true,"Ignore suspect markups (i.e. files tagged with 'suspect=\"1\"').");
  StringC op = opt.String("o","./sm_all_half1m98.abs","Output file for statistical shape model.");
  StringC meanPnts = opt.String("om","","Output file for list of mean feature points representing the mean shape.");
  bool verbose = opt.Boolean("v",false,"Verbose mode, print messages to stderr. ");
  opt.Check();

#if 0
  AAMShapeModelC sm(true); // Basic shape model.
#else
  AAMScaleRotationShapeModelC sm(true); // Shape model with scale and rotation normalisation.
#endif

  SampleC<AAMAppearanceC> appearanceSet;

  HashC<IntT,IntT> typeMap;
  HashC<StringC,IntT> namedTypeMap;
  bool useTypeId;
  if(!mapFile.IsEmpty()) {
    // Load map file.
    if (!LoadTypeMap(mapFile, typeMap, namedTypeMap)) {
      cerr << "ERROR: Failed to read map file." << endl;
      return 1;
    }
    useTypeId = false;
  }

  if(!list.IsEmpty())
  {
    // Read file list.
    DListC<StringC> fileList;
    TextFileC fl(list);
    for(UIntT i=1;i<=fl.NoLines();i++)
      fileList.InsLast(fl[i].TopAndTail());
    appearanceSet = LoadFeatureSet(fileList,dir,typeMap,namedTypeMap,useTypeId,ignoreSuspect,false);
  }
  else
  {
    // Make file list from all files with extension 'ext' located in directory 'dir'.
    appearanceSet = LoadFeatureSet(dir,ext,typeMap,namedTypeMap,useTypeId,ignoreSuspect,false);
  }
  //cout << appearanceSet << endl;

/*  if(!mirrorFile.IsEmpty()) {
    // Load mirror file.
    AAMAppearanceMirrorC mirror(mirrorFile);
    if(!mirror.IsValid()) {
      cerr << "ERROR: Failed to read mirror file. \n";
      return 1;
    }
    // Create mirror appearances
    mirror.Reflect(appearanceSet);
  }*/

  // Set verbose mode.
  sm.SetVerbose(verbose);

  cerr << "Got " << appearanceSet.Size() << " samples. \n";

  // Design shape model from list of appearances.
  if(!sm.Design(appearanceSet,var,maxP)) {
    cerr << "Failed to design shape model. \n";
    return 1;
  }

  cerr << "Dimension of shape model = " << sm.Dimensions() << "\n";

  if(!meanPnts.IsEmpty()) {
    // Save mean position of feature points (representing mean shape)
    if(!Save(meanPnts,sm.MeanPoints())) {
      cerr << "Failed to save mean position of feature points to '" << meanPnts << "'\n";
      return 1;
    }
  }

  if(!op.IsEmpty()) {
    // Save shape model
    if(!Save(op,sm)) {
      cerr << "Failed to save model to '" << op << "'\n";
      return 1;
    }
  }

  return 0;
}
