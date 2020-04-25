// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/aamBuildAppearanceModel.hh"
//! docentry="Ravl.API.Images.AAM"
//! userlevel="Normal"
//! author="Jean-Yves Guillemaut"

#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/IO.hh"
#include "Ravl/Option.hh"
#include "Ravl/Collection.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/SArray1dIter3.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Random.hh"
#include "Ravl/Text/TextFile.hh"
#include "Ravl/String.hh"

#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/Image/AAMActiveAppearanceModel.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"

using namespace RavlN;
using namespace RavlImageN;

//: Build a statistical model of appearance.
//  This program reads a set of appearance files and uses them to build a statistical model of appearance.
// The appearance files to be read are located in the directory defined by the option "-d" and are specified either
// by providing a list of file names (option "-l") or by providing the extension of the appearance files (option "-e").
// In addition, it is possible to double the effective number of appearances used for building the statistical model of
// appearance by considering mirror appearances of the appearance defined in the files. This is done by providing a mirror
// file (option "-m"). The mirror file consists of a list of pairs of indices which represent feature points in
// correspondence when carrying out the vertical symmetry (mirror effect). The resolution of the appearance model
// is controlled by the option "-ms" which controls the size of the shape-free image. Among the options it is possible
// to control the proportion of variation preserved by the statistical model of appearance for shape, texture and
// combined values (options "-vs", "-vt" and "-vc") and to set limits to the maximum number of shape, texture and
// combined parameters contained in the statistical model of appearance (option "-maxs", "-maxt" and "-maxc").
// For more information type "aamBuildAppearanceModel -help".

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC dir = opt.String("d","./markup/","Directory containing markup files defining the appearances.");
  StringC list = opt.String("l","./all_half1.list","Input file containing list of markup file names.");
  StringC ext = opt.String("e","xml","Extention to search for markup files (needed only if no list is provided, i.e. option -l is set to empty string \"\").");
  StringC mirrorFile = opt.String("m","./mirror.txt","Input mirror file for generation of mirror appearances. To disable mirror appearances, the option -m should be set to the empty string \"\".");
  RealT varS = opt.Real("vs",0.98,"Proportion of shape variation preserved by the PCA during dimension reduction (must be between 0 and 1).");
  RealT varT = opt.Real("vt",0.98,"Proportion of texture variation preserved by the PCA during dimension reduction (must be between 0 and 1).");
  RealT varC = opt.Real("vc",0.95,"Proportion of combined variation preserved by the PCA during dimension reduction (must be between 0 and 1).");
  UIntT maxS = opt.Int("maxs",1000,"Upper limit on number of shape parameters in the statistical shape model. If number of parameters required to explain the proportion of variation specified by option -vs exceeds this value, the number of parameters will be troncated to this value.");
  UIntT maxT = opt.Int("maxt",1000,"Upper limit on number of texture parameters in the statistical shape model. If number of parameters required to explain the proportion of variation specified by option -vs exceeds this value, the number of parameters will be troncated to this value.");
  UIntT maxC = opt.Int("maxc",1000,"Upper limit on number of combined parameters in the statistical shape model. If number of parameters required to explain the proportion of variation specified by option -vs exceeds this value, the number of parameters will be troncated to this value.");
  RealT sigma = opt.Real("s",2,"Warping sigma. ");
  Index2dC maskSize = opt.Index2d("ms",25,22,"Size of the shape free image.");
  bool ignoreSuspect = opt.Boolean("is",true,"Ignore suspect markups (i.e. files tagged with 'suspect=\"1\"').");
  StringC op = opt.String("o","./am.abs","Output file for statistical model of appearance.");
  opt.Check();

  cout << "Creating list of marked-up files in '" << dir << "'\n";
  DListC<StringC> fileList;
  if(!list.IsEmpty())
  {
    // Read file list.
    TextFileC fl(list);
    HashC<IntT,IntT> typeMap;
    HashC<StringC,IntT> namedTypeMap;
    bool useTypeId;
    for(UIntT i=1;i<=fl.NoLines();i++) {

      StringC fileName = fl[i].TopAndTail();

      // check it is a valid appearance and eliminate suspect files if required
      AAMAppearanceC appear = LoadFeatureFile(fileName,dir,typeMap,namedTypeMap,useTypeId,ignoreSuspect,true);
      if (!appear.IsValid()) {
        continue;
      }

      fileList.InsLast(fileName);
    }
  }
  else
  {
    // Make list from all files with extension 'ext' located in directory 'dir'.
    DirectoryC md(dir);
    if(!md.IsValid()) {
      cerr << "Can't find directory '" << dir << "'\n";
      return 1;
    }
    fileList = md.List("",StringC(".") + ext);
  }

  if(!fileList.IsValid() || fileList.Size() == 0) {
    cerr << "Failed to create list of marked-up files from '" << dir << "' \n";
    return 1;
  }

  // build appearance model
  AAMAppearanceModelC am(sigma);
  if(!am.Design(fileList,dir,mirrorFile,maskSize,varS,varT,varC,maxS,maxT,maxC,ignoreSuspect)) {
    std::cerr << "Failed to design model. \n";
    return 1;
  }
  cout << "Dimensions in appearance model = " << am.Dimensions() << "\n";

  // save appearance model to file
  if(!Save(op,am)) {
    cerr << "Failed to save model to '" << op << "' \n";
    return 1;
  }

  return 0;
}
