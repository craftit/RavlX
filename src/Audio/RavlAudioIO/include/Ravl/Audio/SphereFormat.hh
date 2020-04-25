// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DEVAUDIOFORMAT_HEADER
#define RAVL_DEVAUDIOFORMAT_HEADER 1
////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Audio.IO"
//! example=exAudioIO.cc
//! lib=RavlAudioIO
//! rcsid="$Id$"
//! file="Ravl/Audio/IO/SphereFormat.hh"

#include "Ravl/DP/FileFormat.hh"

namespace RavlAudioN {
  using namespace RavlN;
  
  //! userlevel=Develop
  //: Sphere File format information.
  
  class FileFormatSphereBodyC 
    : public FileFormatBodyC 
  {
  public:
    FileFormatSphereBodyC(const StringC &vName);
    //: Constructor.
    
    virtual const type_info &ProbeLoad(IStreamC &in,const type_info &obj_type) const;
    //: Is stream in std stream format ?
    
    virtual const type_info &ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const;
    //: Probe for load.
    
    virtual const type_info &ProbeSave(const StringC &filename,const type_info &obj_type,bool forceFormat) const;
    //: Probe for Save.
    
    virtual DPIPortBaseC CreateInput(IStreamC &in,const type_info &obj_type) const;
    //: Create a input port for loading.
    // This creates a plain Sphere stream.
    // Will create an Invalid port if not supported.
    
    virtual DPOPortBaseC CreateOutput(OStreamC &out,const type_info &obj_type) const;
    //: Create a output port for saving.
    // This creates a plain Sphere stream.
    // Will create an Invalid port if not supported.
    
    virtual DPIPortBaseC CreateInput(const StringC &filename,const type_info &obj_type) const;
    //: Create a input port for loading from file 'filename'.
    // Will create an Invalid port if not supported. <p>
    
    virtual DPOPortBaseC CreateOutput(const StringC &filename,const type_info &obj_type) const;
    //: Create a output port for saving to file 'filename'..
    // Will create an Invalid port if not supported. <p>
    
    virtual const type_info &DefaultType() const; 
    //: Get prefered IO type.
    
    virtual IntT Priority() const { return 1; }
    //: Find the priority of the format. the higher the better.
    // Default is zero, this is better than the default (streams.)
    
    virtual bool IsStream() const { return true; }
    //: Test if format is a fully streamable.
    // i.e. check if you can read/write more than object object.
    
  protected:
    StringC vName;
  };
  
  /////////////////////////////
  //! userlevel=Advanced
  //: Create an instance of a Sphere File Format.
  
  class FileFormatSphereC 
    : public FileFormatC<Int16T > 
  {
  public:
    FileFormatSphereC(const StringC &vName)
      : FileFormatC<Int16T>(*new FileFormatSphereBodyC(vName))
    {}
  };
  
}
#endif
