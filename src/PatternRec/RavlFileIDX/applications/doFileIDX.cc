// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

#include "Ravl/SysLog.hh"
#include "Ravl/Option.hh"
#include "Ravl/TVector.hh"
#include "Ravl/IO.hh"
#include "Ravl/DP/SequenceIO.hh"
#include "Ravl/PatternRec/DataSetVectorLabel.hh"
#include "Ravl/IO/DataStreamIDX.hh"

int main(int nargs,char **argv)
{
  RavlN::OptionC opts(nargs,argv);
  RavlN::StringC ifilename = opts.String("i","","Filename ");
  RavlN::StringC ofilename = opts.String("o",ifilename + ".abs","Filename ");
  bool asByte = opts.Boolean("b","","Treat data as bytes ");
  bool convMINST = opts.Boolean("cm",false,"Convert MINST file pair to RAVL dataset. ");
  bool info = opts.Boolean("di",true,"Dump info on file.");
  RavlN::Int64T sample = opts.Int("s",0,"Default sample to retrieve");
  opts.Check();

  if(info) {
    RavlN::ION::DataStreamIDXC idx;
    if(!idx.Open(ifilename)) {
      RavlError("Failed to open '%s' ",ifilename.c_str());
      return 1;
    }
    RavlInfo("Data has %u dimensions. ",(unsigned) idx.Dims().Size());
    for(unsigned i = 0;i < idx.Dims().Size();i++) {
      RavlInfo(" Dim %u - %u ",i,(unsigned) idx.Dims()[i]);
    }
    //
    RavlN::TVectorC<float> vec;
    if(!idx.FetchVector(sample,vec)) {
      RavlError("Failed to fetch sample vector.");
      return 1;
    }
    RavlInfo("Frame 0: %s ",RavlN::StringOf(vec).c_str());
    return 0;
  }
  if(convMINST) {
    RavlN::DPISPortC<RavlN::TVectorC<float> > vecIn;
    RavlN::DPISPortC<RavlN::ByteT > labelIn;
    RavlN::StringC vecFile = ifilename + "-images-idx3-ubyte";
    RavlN::StringC labFile = ifilename + "-labels-idx1-ubyte";

    if(!RavlN::OpenISequence(vecIn,vecFile)) {
      RavlError("Failed to open '%s' ",ifilename.c_str());
      return 1;
    }
    if(!RavlN::OpenISequence(labelIn,labFile)) {
      RavlError("Failed to open '%s' ",ifilename.c_str());
      return 1;
    }

    RavlN::DataSetVectorLabelC dset(vecIn.Size());
    RavlN::TVectorC<float> vec;
    RavlN::ByteT lab;
    while (vecIn.Get(vec) && labelIn.Get(lab)) {
      dset.Append(vec,lab);
    }
    if(!RavlN::Save(ofilename,dset)) {
      RavlError("Failed to save %s ",ofilename.c_str());
      return 1;
    }
    return 0;
  }
  if(!asByte) {
    RavlN::DPIPortC<RavlN::TVectorC<float> > seqIn;
    if(!RavlN::OpenISequence(seqIn,ifilename)) {
      RavlError("Failed to open '%s' ",ifilename.c_str());
      return 1;
    }
    RavlN::TVectorC<float>  val;
    while(seqIn.Get(val)) {
      RavlDebug("Val:%s ",RavlN::StringOf(val).c_str());
    }
  } else {
    RavlN::DPIPortC<RavlN::ByteT > seqIn;
    if(!RavlN::OpenISequence(seqIn,ifilename)) {
      RavlError("Failed to open '%s' ",ifilename.c_str());
      return 1;
    }
    RavlN::ByteT val;
    while(seqIn.Get(val)) {
      RavlDebug("Val:%x ",(int) val);
    }

  }

  return 0;
}
