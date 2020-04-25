// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_VECTORLABELIO_HEADER
#define RAVL_VECTORLABELIO_HEADER 1
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/VectorLabelIO.hh"

#include "Ravl/PatternRec/DataSet2.hh"

namespace RavlN {

  DataSet2C<SampleC<VectorC>, SampleC<UIntT> > LoadVectorLabel(const StringC &fn);
//: Load a vector label dataset.


}

#endif
