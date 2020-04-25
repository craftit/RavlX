// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImgIOV4L
//! file="Ravl/Contrib/V4L/InitV4L.cc"


namespace RavlImageN {
  extern void InitV4LFormat();
}

void InitV4L() {
  RavlImageN::InitV4LFormat();
}
