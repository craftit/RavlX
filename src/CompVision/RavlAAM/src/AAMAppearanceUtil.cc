// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2005, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlAAM
//! file="Ravl/CompVision/ActiveAppearanceModels/AAMAppearanceUtil.cc"

#include "Ravl/DLIter.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/String.hh"
#include "Ravl/DList.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/IO.hh"
#include "Ravl/Image/ImagePointFeatureSet.hh"
#include "Ravl/Image/AAMAppearance.hh"
#include "Ravl/Image/AAMAppearanceUtil.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/OS/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {

  bool LoadTypeMap(const StringC &filename, HashC<IntT,IntT> &typeMap, HashC<StringC,IntT> &namedTypeMap) {
    IStreamC is(filename);
    if(!is) {
      cerr << "Failed to open map file '" << filename << "' \n";
      return false;
    }
    IntT index = 0;
    while(is) {
      StringC line;
      readline(is,line);
      SubStringC ss = line.TopAndTail();
      if(ss.Size() <= 0)
        continue; // Skip empty lines.
      if(ss.firstchar() == '#')
        continue; // Skip comments.
      IntT id;
      StringC description;
      int sep = ss.index(' ');
      SubStringC s1 = ss.before(sep);
      SubStringC s2 = ss.after(sep);
      id = s1.IntValue();
      description = s2;

      namedTypeMap.Insert(description, index);
      typeMap.Insert(id, index);
      index++;
    }
    return true;
  }

  static bool GenerateTypeMap(HashIterC<IntT, ImagePointFeatureC> fit,bool &useTypeId,HashC<IntT,IntT> &typeMap,HashC<StringC,IntT> &namedTypeMap) {
    // Check if we have feature type id's.
    if(fit.Data().TypeID() >= 0)
      useTypeId = true;
    // Go through and setup typemap.
    IntT allocId = 0;
    for(;fit;fit++) {
      UIntT idVal;
      //if(useTypeId) idVal = fit->TypeID();
      if(useTypeId) idVal =  allocId + 1;
      else idVal = fit.Key();
      typeMap[idVal] = allocId;
      namedTypeMap[fit->Description()] = allocId;
      allocId++;
    }
    ONDEBUG(cout << "typeMap: " << typeMap << endl << "namedTypeMap: " << namedTypeMap << endl);
    return true;
  }


  //: Convert appearance to ImagePointFeatureSetC .
  bool SaveFeatureSet(const AAMAppearanceC &appear, const ImagePointFeatureSetC &ModelFeatureSet, ImagePointFeatureSetC &featureSet, const FilenameC Name) {

    featureSet = ModelFeatureSet.Copy();

    HashC<IntT,IntT> typeMap;
    HashC<StringC,IntT> namedTypeMap;

    bool useTypeId = false;

    SArray1dC<Point2dC> pnts = appear.Points();
    if(typeMap.IsEmpty()) { // Do we need to build a type map ?
      GenerateTypeMap(ModelFeatureSet.FeatureIterator(),useTypeId,typeMap,namedTypeMap);
    }
    for(HashIterC<IntT, ImagePointFeatureC> fit(ModelFeatureSet.FeatureIterator());fit;fit++) {
      if(useTypeId) {
        IntT id = fit->TypeID();
        if(id < 0) {
          featureSet.Set(fit->Description(),pnts[namedTypeMap[fit->Description()]]);
        } else {
          featureSet.Set(fit->Description(),pnts[typeMap[id]]);
        }
      } else {
        // Use description, its more reliable.;
        featureSet.Set(fit->Description(),pnts[namedTypeMap[fit->Description()]]);
      }
    }
    featureSet.ImageFile() = Name;

    return true;
  }


  //: Convert appearance to ImagePointFeatureSetC .
  bool SaveFeatureSet(const AAMAppearanceC &appear, const ImagePointFeatureSetC &ModelFeatureSet, const StringC &file,const StringC &dir, const FilenameC Name) {

    // Firstly convert AAMAppearanceC to ImagePointFeatureSetC format...
    ImagePointFeatureSetC featureSet;
    if (!SaveFeatureSet(appear, ModelFeatureSet, featureSet, Name)) {
      cerr << "WARNING: Failed to convert AAMAppearanceC appear into ImagePointFeatureSetC format.\n";
      return false;
    }

    // ... then save as a file
    StringC featureSetFile = dir + '/' + file;
    if(!Save(featureSetFile,featureSet)) {
      cerr << "WARNING: Failed to save ImagePointFeatureSetC to file '" << featureSetFile << "' \n";
      return false;
    }

    return true;
  }


  //: Convert appearance to ImagePointFeatureSetC .
  bool SaveFeatureSet(const SampleC<AAMAppearanceC> &appears, const ImagePointFeatureSetC &ModelFeatureSet, const DListC<StringC> &files,const StringC &dir, const DListC<FilenameC> Names) {
    DLIterC<StringC> it1(files);
    if (Names.IsEmpty()) {
      for(SampleIterC<AAMAppearanceC> it(appears);it;it++) {
        SaveFeatureSet(it.Data(), ModelFeatureSet, it1.Data(), dir);
        it1++;
      }
    } else {
      DLIterC<FilenameC> it2(Names);
      for(SampleIterC<AAMAppearanceC> it(appears);it;it++) {
        SaveFeatureSet(it.Data(), ModelFeatureSet, it1.Data(), dir, it2.Data());
        it1++; it2++;
      }
    }

    return true;

  }

  //: Convert an ImagePointFeatureSetC object to an appearance.
  //!param: fs - the feature object
  //!param: ignoreSuspect - Ignore XML files marked as "Suspect"? True = yes.
  //!param: loadImages - Load image in memory? True = yes.
  // Note that if 'loadImages' is set to false, only the shape of the model instance will be loaded.
  AAMAppearanceC LoadFeature(const ImagePointFeatureSetC &fs,
                             const StringC &filename,
                             HashC<IntT, IntT> &typeMap,
                             HashC<StringC, IntT> &namedTypeMap,
                             bool &useTypeId,
                             bool ignoreSuspect,
                             bool loadImages)
  {
    AAMAppearanceC appear;

    if(ignoreSuspect && fs.IsSuspect()) {
      SysLog(SYSLOG_DEBUG) << "Skipping suspect markup '" << fs.ImageFile() << "'";
      return appear;
    }
    SArray1dC<Point2dC> pnts(fs.Size());
    if(typeMap.IsEmpty()) { // Do we need to build a type map ?
      GenerateTypeMap(fs.FeatureIterator(),useTypeId,typeMap,namedTypeMap);
    }
    if (fs.Size() != (IntT)typeMap.Size()) {
      SysLog(SYSLOG_ERR) << "Feature set has wrong number of features '" << fs.ImageFile() << "' " << fs.Size() << " " << typeMap.Size();
      return appear;
    }
    for(HashIterC<IntT, ImagePointFeatureC> fit(fs.FeatureIterator());fit;fit++) {
      if(useTypeId) {
        IntT id = fit->TypeID();
        if(id < 0) {
          pnts[namedTypeMap[fit->Description()]] = fit.Data();
        } else {
          pnts[typeMap[id]] = fit.Data();
        }
      } else {
        // Use description, its more reliable.;
        pnts[namedTypeMap[fit->Description()]] = fit.Data();
      }
    }
    ImageC<ByteT> img;
    if(loadImages) {
      if(!Load(fs.ImageFile(),img)) {
        SysLog(SYSLOG_WARNING) << "Failed to load image '" << fs.ImageFile() << "'";
        return appear;
      }
    }
    appear = AAMAppearanceC(pnts,img);
    appear.SourceFile() = filename;

    return appear;
  }

  //: Load ImagePointFeatureSetC object from XML file and store as an appearance.
  //!param: file - names of XML file.
  //!param: dir - Name of directory containing appearance files.
  //!param: ignoreSuspect - Ignore XML files marked as "Suspect"? True = yes.
  //!param: loadImages - Load image in memory? True = yes.
  // Note that if 'loadImages' is set to false, only the shape of the model instance will be loaded.
  AAMAppearanceC LoadFeatureFile(const StringC &file,
                                   const StringC & dir,
                                   HashC<IntT,IntT> &typeMap,
                                   HashC<StringC,IntT> &namedTypeMap,
                                   bool &useTypeId,
                                   bool ignoreSuspect,
                                   bool loadImages) {
    ImagePointFeatureSetC fs;
    StringC featureSetFile = dir + '/' + file;
    if(!Load(featureSetFile,fs)) {
      SysLog(SYSLOG_WARNING) << "Failed to load feature file '" << featureSetFile << "'";
      return AAMAppearanceC();
    }
    return LoadFeature(fs, file, typeMap, namedTypeMap, useTypeId, ignoreSuspect, loadImages);
  }


  //: Load ImagePointFeatureSetC objects from a set of XML files and store as set of appearances.
  //!param: files - list of names of XML files.
  //!param: dir - Name of directory containing XML files.
  //!param: ignoreSuspect - Ignore XML files marked as "Suspect"? True = yes.
  //!param: loadImages - Load image in memory? True = yes.
  // Note that if 'loadImages' is set to false, only the shape of the model instance will be loaded.
  SampleC<AAMAppearanceC> LoadFeatureSet(const DListC<StringC> &files,
                                         const StringC & dir,
                                         HashC<IntT,IntT> &typeMap,
                                         HashC<StringC,IntT> &namedTypeMap,
                                         bool &useTypeId,
                                         bool ignoreSuspect,
                                         bool loadImages
                                         )
  {
    SampleC<AAMAppearanceC> appearanceSet;

    DirectoryC md(dir);
    if(!md.IsValid()) {
      cerr << "Can't find directory '" << dir << "'\n";
      return appearanceSet;
    }
    cerr << "Got " << files.Size() << " files in the list. \n";
    appearanceSet = SampleC<AAMAppearanceC>(files.Size());
    cout << "Loading " << flush;

    for(DLIterC<StringC> it(files); it; it++) {
      AAMAppearanceC appear;
      appear = LoadFeatureFile(*it, dir, typeMap, namedTypeMap, useTypeId, ignoreSuspect, loadImages);
      if (appear.IsValid()) {
        appearanceSet.Append(appear);
        //cout << "." << std::flush;
      } else {
        //cout << "x" << std::flush;
      }
    }
    cout << "Number valid samples: " << appearanceSet.Size() << endl;
    cout  << "\n";

    return appearanceSet;
  }


  //: Load ImagePointFeatureSetC objects from all XML files located in a directory.
  //!param: dir - Name of directory containing XML files.
  //!param: ext - Extension of XML files.
  //!param: ignoreSuspect - Ignore XML files marked as "Suspect"? True = yes.
  //!param: loadImages - Load image in memory? True = yes.
  // Note that if 'loadImages' is set to false, only the shape of the model instance will be loaded.
  SampleC<AAMAppearanceC> LoadFeatureSet(const StringC &dir,
                                         const StringC &ext,
                                         HashC<IntT,IntT> &typeMap,
                                         HashC<StringC,IntT> &namedTypeMap,
                                         bool &useTypeId,
                                         bool ignoreSuspect,
                                         bool loadImages
                                         )
  {
    SampleC<AAMAppearanceC> appearanceSet;
    DirectoryC md(dir);
    if(!md.IsValid()) {
      cerr << "Can't find directory '" << dir << "'\n";
      return appearanceSet;
    }

    DListC<StringC> files = md.List("",StringC(".") + ext);
    appearanceSet = LoadFeatureSet(files,dir,typeMap,namedTypeMap,useTypeId,ignoreSuspect,loadImages);

    return appearanceSet;
  }

}

