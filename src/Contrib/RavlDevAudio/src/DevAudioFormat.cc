// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.Audio.Audio IO.Unix"
//! lib=RavlDevAudio
//! file="Ravl/Contrib/Audio/DevAudio/DevAudioFormat.cc"

#include "Ravl/Audio/DevAudioFormat.hh"
#include "Ravl/Audio/DevAudioIO.hh"
#include "Ravl/TypeName.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {
  
  void InitDevAudioFormat()
  {}
  
  // DevAudio ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  FileFormatDevAudioBodyC::FileFormatDevAudioBodyC(const StringC &nvName)
    : FileFormatBodyC(nvName,StringC("DevAudio driver. (@DEVAUDIO)")),
      vName(nvName)
  {}
  
  
  const type_info &FileFormatDevAudioBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const
  { return typeid(void); }
  
  const type_info &
  FileFormatDevAudioBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const { 
    if(filename.length() == 0)
      return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    StringC device = ExtractDevice(filename);
    StringC file = ExtractParams(filename);
    int channel = 0;
    int pn = file.index('#');
    if(pn >= 0) { // Got a channel number ?
      channel = file.after(pn).IntValue();
      file = file.before(pn);
    }
    ONDEBUG(cerr << "FileFormatDevAudioBodyC::ProbeLoad(), Checking file type." << obj_type.name() << " Device=" << device <<"\n");
    if(device != "DEVAUDIO")
      return typeid(void);
    if(obj_type == typeid(SampleElemC<2,Int16T>))
      return obj_type;
    return typeid(SampleElemC<1,Int16T>);
  }
  
  const type_info &
  FileFormatDevAudioBodyC::ProbeSave(const StringC &nfilename,const type_info &obj_type,bool forceFormat) const { 
    StringC filename(nfilename);
    if(filename.length() == 0)
      return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    StringC device = ExtractDevice(filename);
    StringC file = ExtractParams(filename);
    int channel = 0;
    int pn = file.index('#');
    if(pn >= 0) { // Got a channel number ?
      channel = file.after(pn).IntValue();
      file = file.before(pn);
    }
    ONDEBUG(cerr << "FileFormatDevAudioBodyC::ProbeLoad(), Checking file type." << obj_type.name() << " Device=" << device <<"\n");
    if(device != "DEVAUDIO")
      return typeid(void);
    if(obj_type == typeid(SampleElemC<2,Int16T>))
      return obj_type;
    return typeid( SampleElemC<1,Int16T> );
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatDevAudioBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const
  { return DPIPortBaseC(); }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatDevAudioBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const 
  { return DPOPortBaseC(); }

  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatDevAudioBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatDevAudioBodyC::CreateInput(const StringC &,const type_info &), Called for type " << RavlN::TypeName(obj_type) <<" \n");
    if(filename[0] != '@')
    return DPIPortBaseC();
    StringC fn = ExtractParams(filename);
    StringC dev = ExtractDevice(filename);
    int channel = 0;
    int pn = fn.index('#');
    if(pn >= 0) { // Got a channel number ?
      channel = fn.after(pn).IntValue();
      fn = fn.before(pn);
    }
    if(fn == "")
      fn = "/dev/audio";
    if(obj_type == typeid(SampleElemC<1,Int16T>)) {
      DPIAudioC<SampleElemC<1,Int16T>,DevAudioBaseC> ret(fn,channel);
      return ret;
    }
    if(obj_type == typeid(SampleElemC<2,Int16T>)) {
      DPIAudioC<SampleElemC<2,Int16T>,DevAudioBaseC> ret(fn,channel);
      return ret;
    }
    ONDEBUG(cerr << "Open failed. \n");
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatDevAudioBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  { 
    ONDEBUG(cerr << "FileFormatDevAudioBodyC::CreateOutput(const StringC &,const type_info &), Called. \n");
    if(filename[0] != '@')
      return DPOPortBaseC();
    StringC fn = ExtractParams(filename);
    StringC dev = ExtractDevice(filename);
    int channel = 0;
    int pn = fn.index('#');
    if(pn >= 0) { // Got a channel number ?
      channel = fn.after(pn).IntValue();
      fn = fn.before(pn);
    }
    if(fn == "")
      fn = "/dev/audio";
    if(obj_type == typeid(SampleElemC<1,Int16T>)) {
      DPOAudioC<SampleElemC<1,Int16T> ,DevAudioBaseC> ret(fn,channel);
      if(ret.IsPutReady()) // Did open succeed ?
	return ret;
      cerr << "Failed to open device '" << fn << "'\n"; 
    }
    if(obj_type == typeid(SampleElemC<2,Int16T>)) {
      DPOAudioC<SampleElemC<2,Int16T>,DevAudioBaseC> ret(fn,channel);
      if(ret.IsPutReady()) // Did open succeed ?
	return ret;
      cerr << "Failed to open device '" << fn << "'\n"; 
    }
    return DPOPortBaseC();    
  }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatDevAudioBodyC::DefaultType() const 
  { return typeid( SampleElemC<1,Int16T> ); }
  
  // Some common cif formats.
  
  FileFormatDevAudioC RegisterFileFormatDevAudio("dev_audio");
}
