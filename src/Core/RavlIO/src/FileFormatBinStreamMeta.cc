// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! lib=RavlIO
//! file="Ravl/Core/IO/FileFormatBinStream.cc"

#include "Ravl/DP/FileFormatBinStreamMeta.hh"
#include "Ravl/DP/BinFileIO.hh"
#include "Ravl/DP/TypeConverter.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/MTLocks.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  FileFormatBinStreamMetaBodyC::FileFormatBinStreamMetaBodyC(bool pubList)
    : FileFormatBodyC("abs","RAVL binary stream. ",pubList)
  {}
  //: Constructor.

  FileFormatBinStreamMetaBodyC::FileFormatBinStreamMetaBodyC(const StringC &formatId,
                                                             const StringC &formatDescriptor,
                                                             bool pubList)
    : FileFormatBodyC(formatId,formatDescriptor,pubList)
  {}
  //: Constructor with full format info.

  const std::type_info &FileFormatBinStreamMetaBodyC::ProbeLoad(IStreamC &in,const std::type_info &obj_type) const
  {
    ONDEBUG(RavlDebug("ProbeLoad for type '%s' ",TypeName(obj_type)));
    if(!in.good()) {
      ONDEBUG(RavlDebug("ProbeLoad failed, stream not good."));
      return typeid(void);
    }
    BinIStreamC bin(in);
    std::streampos mark = bin.Tell();
    UInt16T id;
    // Check magic number.
    bin >> id;
    ONDEBUG(RavlDebug("ProbeLoad got stream id '%04x' .",id));
    switch(id)
    {
      case RavlN::RAVLBinaryID64:
        bin.SetCompatibilityMode32Bit(false);
        break;
      case RavlN::RAVLInvBinaryID64:
        bin.SetCompatibilityMode32Bit(false);
        bin.UseNativeEndian(!bin.NativeEndian());
        break;
      case RavlN::RAVLBinaryID32:
        bin.SetCompatibilityMode32Bit(true);
        break;
      case RavlN::RAVLInvBinaryID32:
        bin.SetCompatibilityMode32Bit(true);
        bin.UseNativeEndian(!bin.NativeEndian());
        break;
      case RavlN::RAVLInvBinaryID:
        bin.UseNativeEndian(!bin.NativeEndian());
        /* no break */
      case RavlN::RAVLBinaryID:
        // Use what every default 32/64 bit mode is set in the stream.
        break;
      default:
        // Unknown format string.
        bin.Seek(mark);
        ONDEBUG(RavlDebug("ProbeLoad unknown stream type '%04x' .",id));
        return typeid(void);
    }
    // Check class name.
    StringC classname;
    bin >> classname;
    ONDEBUG(RavlDebug("ProbeLoad got class name '%s' .",classname.c_str()));
    bin.Seek(mark);
    FileFormatBaseC ff;
    if(!m_class2format.Lookup(classname,ff))
      return typeid(void);
    return ff.DefaultType();
  }
  //: Is stream in std stream format ?

  const std::type_info &FileFormatBinStreamMetaBodyC::ProbeLoad(const StringC &filename,IStreamC &in,const type_info &obj_type) const
  {
    //cout << "File Probe '" << filename << "' Looking for:" << TypeName(obj_type) << endl;
    if(filename == "") {
      // Pure stream ?
      FileFormatBaseC ff;
      if(!m_class2format.Lookup(TypeName(obj_type),ff))
        return typeid(void);
      return ff.DefaultType();
    }


    return ProbeLoad(in,obj_type); // Check load from stream.
  }

  const std::type_info &FileFormatBinStreamMetaBodyC::ProbeSave(const StringC &filename,
                                                                const std::type_info &obj_type,
                                                                bool forceFormat) const
  {
    ONDEBUG(RavlDebug("Probe save for '%s' type '%s' ForceFormat:%d ",filename.c_str(),RavlN::TypeName(obj_type),(int) forceFormat));
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
#if 0
        if(*bestType == typeid(void)) {
          RavlDebug("Don't know how to save '%s' (%s) ",TypeName(obj_type),obj_type.name());
        }
#endif
      } else {
        bestType = &(ff.DefaultType());
      }
      MTWriteLockC lockwr(3);
      m_type2use[obj_type.name()] = bestType;
    } else {
      lock.Unlock();
      bestType = &obj_type;
    }
    if(*bestType == typeid(void)) {
      ONDEBUG(RavlDebug("Probe save for '%s' unknown type. ",filename.c_str()));
      return typeid(void);
    }
    if(forceFormat) {
      return *bestType;
    }
    ONDEBUG(RavlDebug("Probe save for '%s' checking extension. ",filename.c_str()));
    // abs = RAVL Binary Stream.
    if(filename.IsEmpty())
      return typeid(void); // Nope.
    if(filename[0] == '@')
      return typeid(void); // Nope.
    StringC ext = Extension(filename);
    // If there's no extension or the extension is 'abs' we can handle it.
    if(ext == ""  || ext == "abs" || ext == "bin" || m_ext.IsMember(ext)) {
      ONDEBUG(RavlDebug("Probe save for '%s' ok. ",filename.c_str()));
      return *bestType; // Yep, can save in format.
    }
    ONDEBUG(RavlDebug("Probe save for '%s' rejected on extension. ",filename.c_str()));
    return typeid(void); // Nope.
  }

  DPIPortBaseC FileFormatBinStreamMetaBodyC::CreateInput(IStreamC &in,const std::type_info &obj_type) const {
    FileFormatBaseC ff;
    if(!m_class2format.Lookup(TypeName(obj_type),ff)) {
      RavlDebug("Failed to lookup type '%s' ",TypeName(obj_type));
      return DPIPortBaseC();
    }
    return ff.CreateInput(in,obj_type);
  }
  //: Create a input port for loading.
  // Will create an Invalid port if not supported.

  DPOPortBaseC FileFormatBinStreamMetaBodyC::CreateOutput(OStreamC &out,const std::type_info &obj_type) const {
    FileFormatBaseC ff;
    if(!m_class2format.Lookup(TypeName(obj_type),ff)) {
      RavlDebug("Failed to lookup type '%s' ",TypeName(obj_type));
      return DPOPortBaseC();
    }
    return ff.CreateOutput(out,obj_type);
  }
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.


  DPIPortBaseC FileFormatBinStreamMetaBodyC::CreateInput(const StringC &filename,const std::type_info &obj_type) const  {
    FileFormatBaseC ff;
    if(!m_class2format.Lookup(TypeName(obj_type),ff)) {
      RavlDebug("Failed to lookup type '%s' ",TypeName(obj_type));
      return DPIPortBaseC();
    }
    return ff.CreateInput(filename,obj_type);
  }

  //: Create a input port for loading.
  // Will create an Invalid port if not supported.

  DPOPortBaseC FileFormatBinStreamMetaBodyC::CreateOutput(const StringC &filename,const std::type_info &obj_type) const {
    FileFormatBaseC ff;
    if(!m_class2format.Lookup(TypeName(obj_type),ff)) {
      RavlDebug("Failed to lookup type '%s' ",TypeName(obj_type));
      return DPOPortBaseC();
    }
    return ff.CreateOutput(filename,obj_type);
  }
  //: Create a output port for saving.
  // Will create an Invalid port if not supported.

  const std::type_info &FileFormatBinStreamMetaBodyC::DefaultType() const
  { return typeid(void); }
  //: Get preferred IO type.

  bool FileFormatBinStreamMetaBodyC::IsStream() const
  { return true; }
  //: Just to make it clear its a streamable format.

  //: Register format
  bool FileFormatBinStreamMetaBodyC::RegisterFormat(FileFormatBaseC &fileformat) {
    ONDEBUG(RavlDebug("Registering type '%s' Name:'%s' ",RavlN::TypeName(fileformat.DefaultType()),fileformat.Name().c_str()));
    RavlAssert(fileformat.DefaultType() != typeid(void));
    if(!HaveTypeName(fileformat.DefaultType())) {
      RavlError("No typename registered for '%s', binary IO may fail to work. Hint: Ensure TypeNameC is used to register the name BEFORE the format is registered. ",fileformat.DefaultType().name());
    }
    FileFormatBaseC &entry = m_class2format[RavlN::TypeName(fileformat.DefaultType())];
    if(entry.IsValid()) {
      RavlError("File abs format for '%s' already registered.",RavlN::TypeName(DefaultType()));
      return false;
    }
    entry = fileformat;

    StringC aliasName = fileformat.TypenameAlias();
    if(!aliasName.IsEmpty()) {
      FileFormatBaseC &aliasEntry = m_class2format[aliasName];
      if(aliasEntry.IsValid()) {
        RavlError("File abs format for '%s' already registered.",aliasName.c_str());
      }
      aliasEntry = fileformat;
    }

    if(!m_ext.IsMember(fileformat.Name())) {
      ONDEBUG(RavlDebug("Adding extension '%s' ",fileformat.Name().c_str()));
      m_ext += fileformat.Name();
    }

    return true;
  }


  FileFormatBinStreamMetaC &DefaultFormatBinStreamMeta() {
    static FileFormatBinStreamMetaC ffbsm("abs","RAVL binary file formats");
    return ffbsm;
  }

  //: Register file format.

  bool RegisterFormatBinStreamMeta(FileFormatBaseC &fileformat) {
    return DefaultFormatBinStreamMeta().RegisterFormat(fileformat);
  }

  
  void InitFileFormatBinStreamMeta()
  {
    DefaultFormatBinStreamMeta();
  }
  
}
