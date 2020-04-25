// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, React AI Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL) Version 2.1  See the lgpl.licence file
// for details or see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here

#include "Ravl/IO/IPortIDXFormat.hh"
#include "Ravl/IO/OPortIDX.hh"
#include "Ravl/TypeName.hh"
#include <ctype.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace ION {

  //! Constructor.

  FileFormatIPortIDXBodyC::FileFormatIPortIDXBodyC(const StringC &nvName)
    : RavlN::FileFormatBodyC(nvName,nvName)
  {}

  const std::type_info &
  FileFormatIPortIDXBodyC::ProbeLoad(RavlN::IStreamC &in,const std::type_info &obj_type) const {
#if 0
    DataStreamIDXC fds;
    std::streampos pos =in.Tell();
    if(!fds.Open(in)) {
      in.Seek(pos);
      return typeid(void);
    }
    in.Seek(pos);
    return typeid(RavlN::TVectorC<float>);
#else
    return typeid(void);
#endif
  }
  
  const std::type_info &
  FileFormatIPortIDXBodyC::ProbeLoad(const StringC &nfilename,RavlN::IStreamC &in,const std::type_info &obj_type) const {
    StringC suffix = Extension(nfilename);
#if 0
    if (suffix != "idx")
      return typeid(void);
#endif
    DataStreamIDXC fds;
    if(!fds.Open(nfilename))
      return typeid(void);
    if(obj_type == typeid(RavlN::ByteT) || obj_type == typeid(RavlN::IntT) || obj_type == typeid(unsigned))
      return typeid(RavlN::ByteT);
    return typeid(RavlN::TVectorC<float>);
  }
  
  const std::type_info &
  FileFormatIPortIDXBodyC::ProbeSave(const StringC &nfilename,const std::type_info &obj_type,bool forceFormat) const {
    StringC suffix = Extension(nfilename);
    ONDEBUG(RavlDebug("Probe save idx '%s' Type:%s ",nfilename.c_str(),RavlN::TypeName(obj_type)));
    if (suffix != "idx")
      return typeid(void);
    if(obj_type == typeid(ByteT) || obj_type == typeid(TVectorC<float>))
      return obj_type;
    return typeid(TVectorC<float>);
  }
  
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  
  RavlN::DPIPortBaseC FileFormatIPortIDXBodyC::CreateInput(RavlN::IStreamC &in,const std::type_info &obj_type) const {
    if(!in.good())
      return RavlN::DPIPortBaseC();
#if 0
    if(obj_type == typeid(RavlN::TVectorC<float>))
      return IPortIDXC<RavlN::TVectorC<float> >(in);
    if(obj_type == typeid(RavlN::ByteT))
      return IPortIDXC<RavlN::ByteT>(in);
#endif
    return RavlN::DPIPortBaseC();
  }
  
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  
  RavlN::DPOPortBaseC FileFormatIPortIDXBodyC::CreateOutput(RavlN::OStreamC &out,const std::type_info &obj_type) const  {
    return RavlN::DPOPortBaseC();
  }
  
  //: Create a input port for loading from file 'filename'.
  // Will create an Invalid port if not supported. <p>
  
  RavlN::DPIPortBaseC FileFormatIPortIDXBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const {
    ONDEBUG(std::cerr << "FileFormatIPortIDXBodyC::CreateInput(const StringC &,const std::type_info &), Called. \n");
    if(obj_type == typeid(RavlN::TVectorC<float>)) {
      return DPIPortIDXC<RavlN::TVectorC<float> >(filename);
    }
    if(obj_type == typeid(RavlN::ByteT)) {
      return DPIPortIDXC<ByteT>(filename);
    }
    return RavlN::DPIPortBaseC();
  }
  
  //: Create a output port for saving to file 'filename'..
  // Will create an Invalid port if not supported. <p>
  
  RavlN::DPOPortBaseC FileFormatIPortIDXBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const {
    if(obj_type == typeid(ByteT))
      return DPOPortIDXC<ByteT>(filename);
    if(obj_type == typeid(TVectorC<float>))
      return DPOPortIDXC<RavlN::TVectorC<float> >(filename);
    return RavlN::DPOPortBaseC();
  }
  
  //: Get preferred IO type.
  
  const std::type_info &FileFormatIPortIDXBodyC::DefaultType() const
  { return typeid(RavlN::TVectorC<float>); }


  static FileFormatIPortIDXC g_registerFileFormatIPortIDX ("idx");

  void InitIPortIDX()
  {}


}}
