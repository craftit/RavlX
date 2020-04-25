// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.Contrib.Audio IO.Unix"
//! lib=RavlAudioIO
//! file="Ravl/Audio/IO/SphereFormat.cc"

#include "Ravl/Audio/SphereFormat.hh"
#include "Ravl/Audio/SphereIO.hh"
#include "Ravl/TypeName.hh"

#define DPDEBUG 0
#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlAudioN {
  
  void InitSphereFormat()
  {}
  
  // Sphere ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  FileFormatSphereBodyC::FileFormatSphereBodyC(const StringC &nvName)
    : FileFormatBodyC(nvName,StringC("Sphere audio format.")),
      vName(nvName)
  {}
  
  
  const type_info &FileFormatSphereBodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const
  { return typeid(void); }
  
  const type_info &
  FileFormatSphereBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const { 
    if(filename.length() == 0)
      return typeid(void);
    StringC formatName;
    if(!ReadString(in,formatName))
      return typeid(void);
    if(formatName != "NIST_1A")
      return typeid(void);
    return typeid(SampleElemC<1,Int16T>);
  }
  
  const type_info &
  FileFormatSphereBodyC::ProbeSave(const StringC &nfilename,const type_info &obj_type,bool forceFormat) const { 
    return typeid(void);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatSphereBodyC::CreateInput(IStreamC &in,const type_info &obj_type) const
  { return DPIPortBaseC(); }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatSphereBodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const 
  { return DPOPortBaseC(); }

  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatSphereBodyC::CreateInput(const StringC &filename,const type_info &obj_type) const
  {
    ONDEBUG(cerr << "FileFormatSphereBodyC::CreateInput(const StringC &,const type_info &), Called. \n");
    int channel = 0;
    if(obj_type == typeid(SampleElemC<1,Int16T>)) {
      DPIAudioC< SampleElemC<1,Int16T> ,SphereBaseC> ret(filename,channel);
      if(!ret.IsGetEOS()) // Did open succeed ?
	return ret;
    }
#if 0
    if(obj_type == typeid(SampleElemC<2,Int16T>)) {
      DPIAudioC<SampleElemC<2,Int16T>,SphereBaseC> ret(fn,channel);
      if(!ret.IsGetEOS()) // Did open succeed ?
	return ret;
    }
#endif
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  DPOPortBaseC FileFormatSphereBodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  { 
    ONDEBUG(cerr << "FileFormatSphereBodyC::CreateOutput(const StringC &,const type_info &), Called. \n");
    return DPOPortBaseC();    
  }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatSphereBodyC::DefaultType() const 
  { return typeid(SampleElemC<1,Int16T>); }
  
  // Some common cif formats.
  
  FileFormatSphereC RegisterFileFormatSphere("sphere");
}
