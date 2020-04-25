// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_IO_FILEUTILITIES_HEADER
#define RAVL_IO_FILEUTILITIES_HEADER

//! docentry = "Ravl.API.Core.IO.IDX Files"
//! author = "Charles Galambos"
//! lib = RavlFileIDX

#include "Ravl/OS/Filename.hh"

namespace RavlN { namespace ION {
  using RavlN::StringC;

  void FilenameExtractPathAndBase(const StringC &fullFilename,
                          StringC &path,
                          StringC &baseName,
                          StringC &subType,
                          StringC &currentExtension);

  bool FilenameComputeSubType(const StringC &originalFilename,
                              const StringC &newSubType,
                              const StringC &newExtension,
                              StringC &orignalBase,
                              StringC &oldSubType,
                              RavlN::FilenameC &newFilename,
                              bool includePath = true);


}}
#endif
