

// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/SampleLabel.cc"
//! rcsid="$Id: SampleLabel.cc 3365 2003-08-28 20:41:26Z kier $"

#include "Ravl/PatternRec/SampleReal.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include "Ravl/Vector.hh"

namespace RavlN {

  SampleRealC::SampleRealC(const SampleC<float> &sample)
   : SampleC<RealT>(sample.Size())
  {
    for(SampleIterC<float> it(sample);it;it++)
      Append(*it);
  }

}
