// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRecIO
//! file="Ravl/PatternRec/IO/DataSetIO.cc"

#include "Ravl/Vector.hh"
#include "Ravl/PatternRec/Sample.hh"
#include "Ravl/DP/FileFormatStream.hh"
#include "Ravl/DP/FileFormatBinStream.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/Types.hh"
#include "Ravl/PatternRec/SampleVector.hh"
#include "Ravl/PatternRec/SampleLabel.hh"
#include "Ravl/PatternRec/DataSet1.hh"
#include "Ravl/PatternRec/DataSet2.hh"
#include "Ravl/PatternRec/DataSet3.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"

namespace RavlN {
  void InitRavlDataSetIO()
  {}
  
  static TypeNameC type1(typeid(DataSet2C<SampleVectorC,SampleLabelC>),"RavlN::DataSet2C<SampleVectorC,SampleLabelC>");
  static TypeNameC type2(typeid(RavlN::DataSetVectorLabelC),"RavlN::DataSetVectorLabelC");
  
  static FileFormatStreamC<DataSet2C<SampleVectorC,SampleLabelC> > g_FileFormatStream_DataSet1C_SampleVectorC_SampleLabelC;
  static FileFormatBinStreamC<DataSet2C<SampleVectorC,SampleLabelC> > g_FileFormatBinStream_DataSet1C_SampleVectorC_SampleLabelC;
  static FileFormatBinStreamC<DataSetVectorLabelC> g_FileFormatBinStream_DataSetVectorLabel("RavlN::DataSetVectorLabelC");

  
}
