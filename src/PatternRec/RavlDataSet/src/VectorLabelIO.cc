// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/VectorLabelIO.cc"

#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/DP/FileFormatBinStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  DataSet2C<SampleC<VectorC>, SampleC<UIntT> > LoadVectorLabel(const StringC &fn) {
    DataSet2C<SampleC<VectorC>, SampleC<UIntT> > ret;
    IStreamC in(fn);
    if (!in)
      return ret;
    IntT samples;
    in >> samples;
    ONDEBUG(cerr << "Reading " << samples << " samples. \n");
    ret = DataSet2C<SampleC<VectorC>, SampleC<UIntT> >(samples);
    for (int i = 0; i < samples; i++) {
      IntT vs;
      // Read vector.
      in >> vs;
      ONDEBUG(cerr << "Vector size " << vs << " elements. \n");
      if (vs > 1000) {
        std::cerr << "Unexpectedly large vector. \n";
        return ret;
      }

      VectorC nv(vs);
      for (BufferAccessIterC<RealT> it(nv); it; it++)
        in >> *it;
      // Read label.
      UIntT label;
      in >> label;
      if (!in)
        break;
      ret.Append(nv, label);
    }
    return ret;
  }
}
