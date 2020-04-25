// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! docentry="Algorithms.Data Processing.IO" 
//! lib=RavlIO
//! author="Charles Galambos"
//! date="13/08/1998"
//! rcsid="$Id$"
//! file="Ravl/Core/IO/FileFormatDesc.cc"

#include "Ravl/DP/FileFormatDesc.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/DP/SPort.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  //: Create an input from the descriptor.
  
  DPIPortBaseC FileFormatDescC::CreateInput(StringC filename,IStreamC &inStream) const {
    RavlAssert(isInput);
    DPIPortBaseC inp;
    // Ensure the stream is at reset of the begining of the file.
    if(inStream.IsOpen()) {
      if((streamsize) inStream.tellg() != 0) {
	inStream.seekg(0);
	if((streamsize) inStream.tellg() == 0)
	  inp = form.CreateInput(inStream,SourceType());
      } else
	inp = form.CreateInput(inStream,SourceType());
    }
    // Did we manage to reset it ? If not open from the file again.
    if(!inp.IsValid())
      inp = form.CreateInput(filename,SourceType());
    if(!inp.IsValid()) {
      cerr << "Internal error: Failed to open input file '" << filename << "' in format '" << form.Name() << "' \n" ;
      RavlAssert(0);
      return DPIPortBaseC();
    }
    return BuildInputConv(inp);
  }
  
  
  //: Create an ouput from the descriptor.
  
  DPOPortBaseC FileFormatDescC::CreateOutput(StringC filename) const {
    RavlAssert(!isInput);
    DPOPortBaseC outp = form.CreateOutput(filename,SourceType());
    if(!outp.IsValid()) {
      // This can happen if we don't have permission to create the file.
      return DPOPortBaseC();
    }
    return BuildOutputConv(outp);
  }
  

  //: Create an input from the descriptor.
  
  DPIPortBaseC FileFormatDescC::CreateInput(StringC filename,IStreamC &inStream,DPSeekCtrlC &sc) const {
    RavlAssert(isInput);
    DPIPortBaseC inp;
    // Ensure the stream is at reset of the begining of the file.
    if(inStream.IsOpen()) {
      if((streamsize) inStream.tellg() != 0) {
	inStream.seekg(0);
	if((streamsize) inStream.tellg() == 0)
	  inp = form.CreateInput(inStream,SourceType());
      } else
	inp = form.CreateInput(inStream,SourceType());
    }
    // Did we manage to reset it ? If not open from the file again.
    if(!inp.IsValid())
      inp = form.CreateInput(filename,SourceType());
    sc = DPSeekCtrlC(inp); // This may or maynot work...
    if(!inp.IsValid()) {
      cerr << "Internal error: Failed to open input file '" << filename << "' in format '" << form.Name() << "' \n" ;
      RavlAssert(0);
      return DPIPortBaseC();
    }
    return BuildInputConv(inp);    
  }
  
  //: Create an input from the descriptor.
  
  DPOPortBaseC FileFormatDescC::CreateOutput(StringC filename,DPSeekCtrlC &sc) const {
    RavlAssert(!isInput);
    DPOPortBaseC outp = form.CreateOutput(filename,SourceType());
    if(!outp.IsValid()) {
      // This could happen for a number of reasons:  File permissions, directory path doesn't exist etc.
      //cerr << "Internal error: Failed to open output file '" << filename << "' in format '" << form.Name() << "' \n" ;
      return DPOPortBaseC();
    }
    sc = DPSeekCtrlC(outp); // This may or maynot work...
    return BuildOutputConv(outp);    
  }
  

  //: Build input conversion.
  
  DPIPortBaseC FileFormatDescC::BuildInputConv(const DPIPortBaseC &ip) const {
    DPIPortBaseC inp(ip);
    for(ConstDLIterC<DPConverterBaseC> it(conv);it.IsElm();it.Next()) {
      inp = it.Data().CreateIStream(inp);
      ONDEBUG(cerr << "inp:" << TypeName(inp.InputType()) << endl);
      RavlAssert(inp.IsValid());
    }  
    return inp;
  }
  
  //: Build output conversion.

  DPOPortBaseC FileFormatDescC::BuildOutputConv(const DPOPortBaseC &op) const {
    DPOPortBaseC outp(op);
    ONDEBUG(cerr << "Start from:" << TypeName(outp.OutputType()) << " Steps:" << conv.Size() << endl);
    ConstDLIterC<DPConverterBaseC> it(conv);
    for(it.Last();it.IsElm();it.Prev()) {
      ONDEBUG(cerr << "Conv: " << it.Data().ProcType().name() << endl);
      outp = it.Data().CreateOStream(outp);
      ONDEBUG(cerr << "to:" << TypeName(outp.OutputType()) << endl);
      RavlAssert(outp.IsValid());
    }
    return outp;
  }
  
  //: Dump conversion to out.
  
  void FileFormatDescC::DumpConv(std::ostream &out) const {
    if(conv.IsEmpty())
      return ;
    ConstDLIterC<DPConverterBaseC> it(conv);
    if(isInput) {
      StringC arrow = " -> ";
      out << TypeName(it.Data().ArgType(0));
      for(;it.IsElm();it.Next()) 
	out << arrow << TypeName(it.Data().Output());
      out << "\n";
    } else {
      StringC arrow = " <- ";
      it.Last();
      out << TypeName(it.Data().Output());
      for(;it.IsElm();it.Prev())
	out << arrow << TypeName(it.Data().ArgType(0));
      out << "\n";
    }
  }
}
