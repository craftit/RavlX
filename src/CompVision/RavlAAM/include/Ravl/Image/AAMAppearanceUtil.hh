// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLIMAGE_APPEARANCEUTIL_HEADER
#define RAVLIMAGE_APPEARANCEUTIL_HEADER 1
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMAppearanceUtil.hh"
//! docentry="Ravl.API.Images.AAM"
//! example = "aamBuildShapeModel.cc;aamBuildAppearanceModel.cc;aamBuildActiveAppearanceModel.cc"

#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/Image/AAMAppearanceModel.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/OS/Filename.hh"

namespace RavlImageN {

  using namespace RavlN;

  AAMAppearanceC LoadFeatureFile(const StringC &file,const StringC &dir,HashC<IntT,IntT> &typeMap,HashC<StringC,IntT> &namedTypeMap,bool &useTypeId,bool ignoreSuspect = true,bool loadImages = true);
  //: Load ImagePointFeatureSetC object from XML file and store as an appearance.
  //!param: file - names of XML file.
  //!param: dir - Name of directory containing appearance files.
  //!param: typeMap - maps typeid to point index
  //!param: namedTypeMap - maps description to point index
  //!param: useTypeId - true uses typeid else descr, if maps are uninitialised then will be set when they are populated
  //!param: ignoreSuspect - Ignore XML files marked as "Suspect"? True = yes.
  //!param: loadImages - Load image in memory? True = yes.
  // Note that if 'loadImages' is set to false, only the shape of the model instance will be loaded.
  // Note that if the maps are empty they will be initialised from the first sample

  SampleC<AAMAppearanceC> LoadFeatureSet(const StringC &dir,const StringC &ext,HashC<IntT,IntT> &typeMap,HashC<StringC,IntT> &namedTypeMap,bool &useTypeId,bool ignoreSuspect,bool loadImages);
  //: Load ImagePointFeatureSetC objects from all XML files located in a directory.
  //!param: dir - Name of directory containing XML files.
  //!param: ext - Extension of XML files.
  //!param: ignoreSuspect - Ignore XML files marked as "Suspect"? True = yes.
  //!param: loadImages - Load image in memory? True = yes.
  // Note that if 'loadImages' is set to false, only the shape of the model instance will be loaded.

  SampleC<AAMAppearanceC> LoadFeatureSet(const DListC<StringC> &files,const StringC &dir,HashC<IntT,IntT> &typeMap,HashC<StringC,IntT> &namedTypeMap,bool &useTypeId,bool ignoreSuspect,bool loadImages);
  //: Load ImagePointFeatureSetC objects from a set of XML files and store as set of appearances.
  //!param: files - list of names of XML files.
  //!param: dir - Name of directory containing XML files.
  //!param: ignoreSuspect - Ignore XML files marked as "Suspect"? True = yes.
  //!param: loadImages - Load image in memory? True = yes.
  // Note that if 'loadImages' is set to false, only the shape of the model instance will be loaded.

  bool SaveFeatureSet(const AAMAppearanceC &appear, const ImagePointFeatureSetC &ModelFeatureSet, ImagePointFeatureSetC &featureSet, const FilenameC Name="");
  //: Convert appearance to ImagePointFeatureSetC .
  //!param: appear - Appearance to be converted.
  //!param: ModelFeatureSet - Example of feature set.
  //!param: featureSet - Output feature set representing appearance.
  //!param: Name - Path of the image corresponding to the feature set (optional).
  // Note that ModelFeatureSet is required to define the convention used for feature set representation, its content is not important as long as it respects the convention adopted.

  bool SaveFeatureSet(const AAMAppearanceC &appear, const ImagePointFeatureSetC &ModelFeatureSet, const StringC &file,const StringC &dir, const FilenameC Name="");
  //: Convert appearance to ImagePointFeatureSetC .

  bool SaveFeatureSet(const SampleC<AAMAppearanceC> &appears, const ImagePointFeatureSetC &ModelFeatureSet, const DListC<StringC> &files,const StringC &dir, const DListC<FilenameC> Names=DListC<FilenameC>());
  //: Convert appearance to ImagePointFeatureSetC .

  bool LoadTypeMap(const StringC &filename, HashC<IntT,IntT> &typeMap, HashC<StringC,IntT> &namedTypeMap);
  //: Load a typeMap and namedTypeMap from file
}


#endif
