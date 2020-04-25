// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2006, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/aamCombineActiveAppearanceModel.hh"
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

#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/Image/AAMActiveAppearanceModel.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"

using namespace RavlN;
using namespace RavlImageN;

//: Combine intermediate results into an Active Appearance Model (AAM).
//  This program terminates the generation of the AAM from a set of files containing intermediate results obtained from aamBuildActiveAppearanceModel run with the option "-pre" and a statistical model of appearance. The result files to be read are located in the directory defined by the option "-d" and are specified either by providing a list of file names (option "-l") or by providing the extension of the result files (option "-e"). The final AAM is saved in the file specified by the option "-op". For more information type "aamCombineActiveAppearanceModel -help".
int main(int nargs,char **argv) {
  OptionC opt(nargs,argv);
  StringC dir = opt.String("d","/vol/vssp/raidbuf/ees1jg/models/am_all_half1_25x22m98_98_95/","Directory containing intermediate results for building Active Appearance Model (AAM).");
  StringC list = opt.String("l","","Input file containing list of intermediate result file names.");
  StringC ext = opt.String("e","abs","Extention to search for intermediate result files (needed only if no list is provided, i.e. option -l is set to empty string \"\").");
  StringC ip = opt.String("am","/vol/vssp/raidbuf/ees1jg/models/am_all_half1_25x22m98_98_95.abs","Input statistical model of appearance.");
  StringC op = opt.String("o","/vol/vssp/raidbuf/ees1jg/models/aam_all_half1_25x22m98_98_95.abs","Output file for Active Appearance Model (AAM).");
  opt.Check();

  cout << "Creating list of marked-up files in '" << dir << "'\n"; 
  DListC<StringC> fileList;
  if(!list.IsEmpty())
  {
    // Read file list.
    TextFileC fl(list);
    for(UIntT i=1;i<=fl.NoLines();i++) {
      StringC fileName = fl[i].TopAndTail();
      fileList.InsLast(fileName);
    }
  }
  else
  {
    // Make file list from all file carrying extension "ext" located in directory "dir".
    DirectoryC md(dir);
    if(!md.IsValid()) {
      cerr << "Can't find directory '" << dir << "'\n";
      return 1;
    }
    cerr << StringC(".") + ext << endl;
    fileList = md.List("",StringC(".") + ext);
    cerr << fileList << endl;
  }

  if(!fileList.IsValid() || fileList.Size() == 0) {
    cerr << "Failed to create list of marked-up files from '" << dir << "' \n";
    return 1;
  }

  AAMAppearanceModelC am;
  AAMActiveAppearanceModelC aam;

  // Start by loading appearance model.
  cerr << "Loading appearance model '" << ip << "' \n";
  if(!Load(ip,am)) {
    cerr << "Failed to load model from '" << ip << "' \n";
    return 1;
  }

  // Terminate creation of AAM.
  if(!aam.PostDesign(am,fileList,dir)) {
    cerr << "Failed to precompute results \n";
    return 1;
  }

  // Save output AAM file.
  if(!Save(op,aam)) {
    cerr << "Failed to save AAM to file '" << op << "' \n";
    return 1;
  }


  return 0;
}
