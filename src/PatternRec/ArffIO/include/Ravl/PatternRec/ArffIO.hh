// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=ArffIO

// FIXME:- Change to use RAVL IO mechanism.

#ifndef ARFFIO_HH
#define ARFFIO_HH

#include "Ravl/String.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/Vector.hh"

namespace RavlN
{
using namespace RavlN;

void SaveArff(const StringC& FileName, const SampleC<VectorC> &TrVec,
              const SampleC<SByteT> &TrLab);

void LoadArff(const StringC& FileName, SampleC<VectorC> &TrVec,
              SampleC<SByteT> &TrLab);

void TrimFeatures(SampleC<VectorC> &TrVec, const char *Mask);
};
#endif
