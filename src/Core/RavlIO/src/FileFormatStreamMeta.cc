// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatStream.cc"

#include "Ravl/DP/FileFormatStreamMeta.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/DP/TypeConverter.hh"
#include "Ravl/SysLog.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {

  FileFormatStreamMetaBodyC::FileFormatStreamMetaBodyC()
    : FileFormatBodyC("stream","Standard C++ iostream headed with the class name. ")
  {}
  //: Default constructor.

  const std::type_info &FileFormatStreamMetaBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const  {
    if(!in.good())
      return typeid(void);
    StringC classname;
    if(!ReadString(in,classname)) {
      ONDEBUG(RavlDebug("Failed to read classname for probe load of '%s' .",TypeName(obj_type)));
      return typeid(void);
    }
    ONDEBUG(RavlDebug("Stream probe load '%s' Looking for type: %s ",classname.c_str(),TypeName(obj_type)));

    FileFormatBaseC ff;
    MTReadLockC lock(3);
    if(!m_class2format.Lookup(classname,ff))
      return typeid(void);
    ONDEBUG(RavlDebug("Stream probe load '%s' Looking for type: %s ",classname.c_str(),TypeName(obj_type)));
    return ff.DefaultType();
  }
  //: Is stream in std stream format ?

  const std::type_info &FileFormatStreamMetaBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const std::type_info &obj_type) const  {
    //cout << "File Probe '" << filename << "' Looking for:" << TypeName(obj_type) << endl;
    ONDEBUG(RavlDebug("Stream probe load file '%s' Looking for type: %s ",filename.c_str(),TypeName(obj_type)));
    if(filename == "") {
      FileFormatBaseC ff;
      if(!m_class2format.Lookup(TypeName(obj_type),ff))
        return typeid(void);
      return ff.DefaultType();
    }
    return ProbeLoad(in,obj_type); // Check load from stream.
  }

  const std::type_info &FileFormatStreamMetaBodyC::ProbeSave(const StringC &filename,const std::type_info &obj_type,bool forceFormat) const {
    // If there's no extension or the extension is 'strm' we can handle it.
    ONDEBUG(RavlDebug("Stream save file '%s' Looking for type: %s ",filename.c_str(),TypeName(obj_type)));
    const std::type_info *bestType = &typeid(void);
    MTReadLockC lock(3);
    if(!m_type2use.Lookup(obj_type.name(),bestType)) {
      lock.Unlock();
      FileFormatBaseC ff;
      if(!m_class2format.Lookup(TypeName(obj_type),ff)) {

        RealT bestCost = 1000000;
        for(RavlN::HashIterC<StringC,FileFormatBaseC> it(m_class2format);it;it++) {
          RealT finalCost = 10000;
          DListC<DPConverterBaseC> c = SystemTypeConverter().FindConversion(obj_type,
              it.Data().DefaultType(),
              finalCost);
          if(!c.IsEmpty() && finalCost < bestCost) {
            bestCost = finalCost;
            bestType = &it.Data().DefaultType();
          }
        }
        ONDEBUG(RavlDebug("Found conversion from '%s' to '%s' ",bestType->name(),obj_type.name()));

      } else {
        bestType = &(ff.DefaultType());
        ONDEBUG(RavlDebug("Found direct map to '%s' ",bestType->name()));
      }
#if DODEBUG
      if(*bestType == typeid(void)) {
        RavlDebug("Don't know how to save '%s' (%s) ",TypeName(obj_type),obj_type.name());
      }
#endif
      MTWriteLockC lockwr(3);
      ONDEBUG(RavlDebug("Storing save type '%s' (%s) as '%s' (%s) ",TypeName(obj_type),obj_type.name(),TypeName(*bestType),bestType->name()));
      m_type2use[obj_type.name()] = bestType;
    } else {
      lock.Unlock();
    }
    if(*bestType == typeid(void))
      return typeid(void);
    if(forceFormat) {
      ONDEBUG(RavlDebug("Stream save file '%s' with forced format type '%s' found!! conversion via '%s' (%d) (tu %d)",filename.c_str(),TypeName(obj_type),TypeName(*bestType),
              (int) m_class2format.IsElm(TypeName(*bestType)),
                  (int)m_type2use.IsElm(obj_type.name())
                  ));
      RavlAssert(m_class2format.IsElm(TypeName(*bestType)));
      return *bestType;
    }
    if(filename.IsEmpty())
      return typeid(void); // Nope.
    if(filename[0] == '@')
      return typeid(void); // Nope.
    StringC ext = Extension(filename);
    if(ext == ""  || ext == "strm" || ext == "txt" ) {
      ONDEBUG(RavlDebug("Stream save file '%s' type %s found via %d !!  ",filename.c_str(),TypeName(obj_type),TypeName(*bestType)));
      return *bestType; // Yep, can save in format.
    }
    return typeid(void); // Nope.
  }

  //: Create a input port for loading.
  // Will create an Invalid port if not supported.
  DPIPortBaseC FileFormatStreamMetaBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    FileFormatBaseC ff;
    MTReadLockC lock(3);
    if(!m_class2format.Lookup(TypeName(obj_type),ff)) {
      RavlError("Failed to create input stream for type '%s' ",RavlN::TypeName(obj_type));
      return DPIPortBaseC();
    }
    lock.Unlock();
    return ff.CreateInput(in,obj_type);
  }

  //: Create a output port for saving.
  // Will create an Invalid port if not supported.
  DPOPortBaseC FileFormatStreamMetaBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const {
    FileFormatBaseC ff;
    MTReadLockC lock(3);
    if(!m_class2format.Lookup(TypeName(obj_type),ff)) {
      RavlError("Failed to create output stream for type '%s' ",RavlN::TypeName(obj_type));
      return DPOPortBaseC();
    }
    lock.Unlock();
    return ff.CreateOutput(out,obj_type);
  }

  //: Get preferred IO type.
  const std::type_info &FileFormatStreamMetaBodyC::DefaultType() const
  { return typeid(void); }

  //: Just to make it clear its a streamable format.
  bool FileFormatStreamMetaBodyC::IsStream() const
  { return true; }

  //: Register format
  bool FileFormatStreamMetaBodyC::RegisterFormat(FileFormatBaseC &fileformat) {
    RavlAssert(fileformat.DefaultType() != typeid(void));
    FileFormatBaseC &entry = m_class2format[RavlN::TypeName(fileformat.DefaultType())];
    if(entry.IsValid()) {
      RavlError("File stream format for '%s' already registered.",RavlN::TypeName(DefaultType()));
      RavlAssert(0);
      return false;
    }
    entry = fileformat;
    return true;
  }


  FileFormatStreamMetaC &DefaultFormatStreamMeta() {
    static FileFormatStreamMetaC ffbsm;
    return ffbsm;
  }

  //: Register file format.

  bool RegisterFormatStreamMeta(FileFormatBaseC &fileformat) {
    return DefaultFormatStreamMeta().RegisterFormat(fileformat);
  }

  //: Register file format.

  void IncludeFileFormatStreamMeta()
  {  }
  
}
