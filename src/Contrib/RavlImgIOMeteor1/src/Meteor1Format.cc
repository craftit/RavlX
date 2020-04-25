// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlImgIOMeteor1
//! file="Ravl/Contrib/Meteor1/Meteor1Format.cc"

#include "Ravl/Image/Meteor1Format.hh"
#include "Ravl/Image/ImgIOMeteor1.hh"
#include "Ravl/TypeName.hh"

#define DPDEBUG 0

#if DPDEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlImageN {
  
  void InitMeteor1Format()
  {}
  
  // Meteor1 ////////////////////////////////////////////////////////////////
  
  //: Constructor.
  FileFormatMeteor1BodyC::FileFormatMeteor1BodyC(const StringC &nvName)
    : FileFormatBodyC(nvName,StringC("Meteor1 driver.")),
      vName(nvName)
  {}
  
  
  const type_info &
  FileFormatMeteor1BodyC::ProbeLoad(IStreamC &in,const type_info &obj_type) const
  { return typeid(void); }
  
  const type_info &
  FileFormatMeteor1BodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const
  { 
    if(filename.length() == 0)
    return typeid(void);
    if(filename[0] != '@')
      return typeid(void);
    if(ExtractDevice(filename) != "METEOR")
      return typeid(void);
    return typeid(ImageC<ByteRGBValueC>); 
  }
  
  const type_info &
  FileFormatMeteor1BodyC::ProbeSave(const StringC &nfilename,const type_info &obj_type,bool forceFormat) const
  { return typeid(void); }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  DPIPortBaseC FileFormatMeteor1BodyC::CreateInput(IStreamC &in,const type_info &obj_type) const
  { return DPIPortBaseC(); }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  DPOPortBaseC FileFormatMeteor1BodyC::CreateOutput(OStreamC &out,const type_info &obj_type) const 
  { return DPOPortBaseC(); }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  DPIPortBaseC FileFormatMeteor1BodyC::CreateInput(const StringC &filename,const type_info &obj_type) const {
    ONDEBUG(cerr << "FileFormatMeteor1BodyC::CreateInput(const StringC &,const type_info &), Called. \n");
    if(filename.length() == 0)
      return DPIPortBaseC();
    if(filename[0] != '@')
      return DPIPortBaseC();
    StringC fn = ExtractParams(filename);
    if(fn == "")
      fn = "/dev/meteor0";
    if(obj_type == typeid(ImageC<ByteRGBValueC>)) {
      return DPIImageMeteor1C<ByteRGBValueC>(fn);
    }
    return DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>

  DPOPortBaseC FileFormatMeteor1BodyC::CreateOutput(const StringC &filename,const type_info &obj_type) const
  { return DPOPortBaseC(); }
  
  //: Get prefered IO type.
  
  const type_info &FileFormatMeteor1BodyC::DefaultType() const 
  { return typeid(ImageC<ByteRGBValueC>); }
  
  // Some common cif formats.
  
  FileFormatMeteor1C RegisterFileFormatMeteor1("meteor1");

}
