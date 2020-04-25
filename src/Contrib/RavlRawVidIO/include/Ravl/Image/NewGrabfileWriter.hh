// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#if !defined(RAVL_NEWGRABFILEWRITER_HEADER)
#define RAVL_NEWGRABFILEWRITER_HEADER
//! lib=RavlRawVidIO
//! author="Simon Tredwell"
//! docentry="Ravl.API.Images.Video.Video IO.RawVid"

#include "Ravl/Image/GrabfileWriter.hh"

namespace RavlImageN {

GrabfileWriterC* NewGrabfileWriter(const int version);
//: Creates a new grabfile writer.
// This allocates an new GrabfileWriter. The caller is responsible for deleting the allocated object.
//!param: version - The version of the writer to create.
//!return: A pointer to a newly allocated GrabfileWriter or 0 if there is no writer for the given <tt>version</tt>.
}

#endif // RAVL_NEWGRABFILEWRITER_HEADER
