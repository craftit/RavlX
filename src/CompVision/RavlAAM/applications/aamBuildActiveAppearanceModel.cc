// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/aamBuildActiveAppearanceModel.hh"
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
#include "Ravl/Types.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Matrix.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/DList.hh"
#include "Ravl/StringList.hh"

#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/Image/AAMActiveAppearanceModel.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"

using namespace RavlN;
using namespace RavlImageN;

//: Build/pre-process an Active Appearance Model (AAM).
//
//  This program reads an appearance model and a set of appearance files and uses them to build
// an Active Appearance Model (AAM).
//
// The appearance files to be read are located in the directory
// defined by the option "-d" and are specified either by providing a list of file names (option "-l")
// or by providing the extension of the appearance files (option "-e").
//
// In addition, it is possible to double the effective number of appearances used for building the AAM by
// considering mirror appearances of the appearance defined in the files. This is done by providing a mirror
// file (option "-m"). The mirror file consists of a list of pairs of indices which represent feature points
// in correspondence when carrying out the vertical symmetry (mirror effect).
//
// The AAM is trained by displacing successively each of the parameters from their ground truth value
// and measuring the resulting error. The option "-i" represents the half number of displacements
// carried out for each parameter. Typically a value of 1 (i.e. 2 displacements, 1 in the positive
// direction and 1 in the negative direction) is sufficient.
//
// The generation of an AAM can be very computer intensive especially if there is a large number of
// appearances available for training. The option "-pre" enables to save partial results for a set of files
// (before generation of the final AAM). This means that it is not necessary to process all the files
// simultaneously. Several jobs can be run in parallel, and the partial results can be ultimately merged
// by using the program "aamCombineActiveAppearanceModel". In the case where partial results must be computed
// (option "-pre"), the format for the output file is filename_i_n.abs, where 'i' and 'n' are integers. 'i'
// is the rank of the first appearance file to be considered from the list, 'n' is the number of files to
// be included starting from the 'i'th one. For more information type "aamBuildActiveAppearanceModel -help".

int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC dir = opt.String("d","./markup/","Directory containing markup files defining the appearances.");
  StringC list = opt.String("l","./all_half1.list","Input file containing list of markup file names.");
  StringC ext = opt.String("e","xml","Extention to search for markup files (needed only if no list is provided, i.e. option -l is set to empty string \"\").");
  StringC mirrorFile = opt.String("m","./mirror.txt","Input mirror file for generation of mirror appearances. To disable mirror appearances, the option -m should be set to the empty string \"\".");
  bool ignoreSuspect = opt.Boolean("is",true,"Ignore suspect markups (i.e. files tagged with 'suspect=\"1\"').");
  StringC ip = opt.String("am","./am.abs","Input statistical model of appearance.");
  UIntT incrSize = opt.Int("i", 1, "Half number of displacements carried out for each parameter when perturbing the model.");
  bool precompute = opt.Boolean("pre",false,"Only pre-process the data? true=yes. In the case where the option is set to true, partial results will be saved rather than the whole Active Appearance Model (AAM). This is useful to pre-process part of the files. The partial results thus obtained can be merged into the final AAM by running the program \"aamCombineActiveAppearanceModel\".");

  StringC op = opt.String("o","./am_all_half1_25x22m98_98_95_1_100.abs","Output file for Active Appearance Model (AAM). In the case where partial results must be computed (option \"-pre\"), the format for the output file is filename_i_n.abs, where 'i' and 'n' are integers. 'i' is the rank of the first appearance file to be considered from the list, 'n' is the number of files to be included starting from the 'i'th one.");
  opt.Check();

  // Extract start file number and number of files to read from output name
  StringC outFile = StringListC(op,"/").Last();
  StringListC outNames(StringListC(outFile,".").First(),"_");
  UIntT noFiles = 0;
  UIntT startNo = -1;
  if(precompute) {
    noFiles = outNames.Nth(-1).UIntValue();
    startNo = outNames.Nth(-2).UIntValue();
  }
  cout << "Creating list of marked-up files in '" << dir << "'\n";
  DListC<StringC> fileList;

  // Make file list.
  TextFileC fl(list);
  HashC<IntT,IntT> typeMap;
  HashC<StringC,IntT> namedTypeMap;
  bool useTypeId;
  if (startNo<=fl.NoLines()) {
    for(UIntT i=startNo;i<Min(startNo+noFiles,fl.NoLines()+1);i++) {
      StringC fileName = fl[i].TopAndTail();
      // check it is a valid appearance and eliminate suspect files if required
      AAMAppearanceC appear = LoadFeatureFile(fileName,dir,typeMap,namedTypeMap,useTypeId,ignoreSuspect,true);
      if (!appear.IsValid()) {
        continue;
      }
      fileList.InsLast(fileName);
    }
  } else {
    for(UIntT i=1;i<fl.NoLines();i++) {
      StringC fileName = fl[i].TopAndTail();
      fileList.InsLast(fileName);
    }

  }
  if(!fileList.IsValid() || fileList.Size() == 0) {
    cerr << "Failed to create list of marked-up files from '" << dir << "' \n";
    return 1;
  }

  AAMAppearanceModelC am;
  AAMActiveAppearanceModelC aam;

  // start by loading appearance model
  cerr << "Loading appearance model '" << ip << "' \n";
  if(!Load(ip,am)) {
    cerr << "Failed to load model from '" << ip << "' \n";
    return 1;
  }

  if(precompute) {
    // Pre-compute only.
    if(!aam.PreDesign(am,fileList,dir,mirrorFile,incrSize,op)) {
      cerr << "Failed to precompute results \n";
      return 1;
    }
  } else {
    // Build the whole AAM.
    if(!aam.Design(am,fileList,dir,mirrorFile,incrSize,ignoreSuspect)) {
      std::cerr << "Failed to design active appearence model. \n";
      return 1;
    }
    if(!Save(op,aam)) {
      cerr << "Failed to save model to '" << op << "' \n";
      return 1;
    }
  }

  return 0;
}
