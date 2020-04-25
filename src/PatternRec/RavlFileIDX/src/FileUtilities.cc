// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

#include "Ravl/IO/FileUtilities.hh"
#include "Ravl/OS/Filename.hh"

namespace RavlN { namespace ION {
  using RavlN::StringC;

  void FilenameExtractPathAndBase(const StringC &fullFilename,
                          StringC &path,
                          StringC &baseName,
                          StringC &subType,
                          StringC &currentExtension)
  {
    RavlN::FilenameC currentDataFilename = fullFilename;

    path = currentDataFilename.PathComponent();
    if(path.IsEmpty())
      path = ".";
    baseName = currentDataFilename.NameComponent();
    currentExtension = currentDataFilename.Extension();

    if(!currentExtension.IsEmpty()) {
      baseName = baseName.before(StringC(".") + currentExtension,-1);
    }

    StringC tmp = baseName.after("_",-1);
    if(tmp == "Normalised" || tmp == "Feature" || tmp == "FeatCorr") {
      baseName = baseName.before(StringC("_") + tmp,-1);
    }

  }


  bool FilenameComputeSubType(const StringC &originalFilename,
                              const StringC &newSubType,
                              const StringC &newExtension,
                              StringC &orignalBase,
                              StringC &oldSubType,
                              RavlN::FilenameC &newFilename,
                              bool includePath)
  {
    StringC path;
    StringC currentExtension;
    FilenameExtractPathAndBase(originalFilename,
                              path,
                              orignalBase,
                              oldSubType,
                              currentExtension);

    if(includePath)
      newFilename = path +'/';
    else
      newFilename = "";
    newFilename += orignalBase + '_' + newSubType + '.' + newExtension;

    return true;
  }

}}
