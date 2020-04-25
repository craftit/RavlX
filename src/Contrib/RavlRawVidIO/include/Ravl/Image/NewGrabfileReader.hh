// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#if !defined(RAVL_NEWGRABFILEREADER_HEADER)
#define RAVL_NEWGRABFILEREADER_HEADER
//! rcsid="$Id: $"
//! lib=RavlRawVidIO
//! author="Simon Tredwell"
//! docentry="Ravl.API.Images.Video.Video IO.RawVid"

#include "Ravl/Image/GrabfileReader.hh"

namespace RavlImageN {

GrabfileReaderC* NewGrabfileReader(const char* filename, bool legacy = false);
//: Creates a new grabfile reader.
// This allocates an new GrabfileReader. The caller is responsible for deleting the allocated object.
//!param: filename - The filename to create the reader for.
//!param: legacy - Create a reader for legacy files. (The <tt>filename</tt> will be ignored.)
//!return: A pointer to a newly allocated GrabfileReader or 0 if there is no reader available for the file <tt>filename</tt>.

}

#endif // RAVL_NEWGRABFILEREADER_HEADER
