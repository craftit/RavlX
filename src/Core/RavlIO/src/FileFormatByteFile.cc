// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2009-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////
//! lib=RavlIO
//! file = "Ravl/Core/IO/FileFormatByteFile.cc"

#include "Ravl/DP/FileFormatByteFile.hh"
#include "Ravl/SysLog.hh"

namespace RavlN
{
  FileFormatByteFileBodyC::FileFormatByteFileBodyC(bool deletable)
  : FileFormatBodyC("bytefile", "Raw binary file.", deletable)
  {}
  //: Ctor.

  FileFormatByteFileBodyC::FileFormatByteFileBodyC(const StringC& formatId, const StringC& formatDescriptor, bool deletable)
  : FileFormatBodyC(formatId, formatDescriptor, deletable)
  {}
  //: Ctor with full format info.

  
  const std::type_info& FileFormatByteFileBodyC::ProbeLoad(IStreamC& inputStream, const type_info& objType) const
  {
    // Byte only
    if (objType != typeid(ByteT))
      return typeid(void);

    // Input stream must be good
    if (!inputStream.good())
      return typeid(void);

    return typeid(ByteT);
  }

  const std::type_info& FileFormatByteFileBodyC::ProbeLoad(const StringC& filename, IStreamC& inputStream, const type_info& objType) const
  {
    // Byte only
    if (objType != typeid(ByteT))
      return typeid(void);

    // No filename, so test input stream
    if (filename.IsEmpty())
      return ProbeLoad(inputStream, objType);

    // Need a real filename
    if (filename[0] == '@')
      return typeid(void);

    return typeid(ByteT);
  }

  const std::type_info& FileFormatByteFileBodyC::ProbeSave(const StringC& filename, const type_info& objType, bool forceFormat) const
  {
    // Byte only
    if (objType != typeid(ByteT))
      return typeid(void);

    // Need a filename
    if (filename.IsEmpty())
      return typeid(void);

    // Need a real filename
    if (filename[0] == '@')
      return typeid(void);

    return typeid(ByteT);
  }

  DPIPortBaseC FileFormatByteFileBodyC::CreateInput(IStreamC& inputStream, const std::type_info& objType) const
  {
    // Byte only
    if (objType != typeid(ByteT))
      return DPIPortBaseC();

    return DPIByteFileC(inputStream);
  }
  //: Create an input port for loading.
  //!return: An invalid port if not supported.

  DPOPortBaseC FileFormatByteFileBodyC::CreateOutput(OStreamC& outputStream, const std::type_info& objType) const
  {
    // Byte only
    if (objType != typeid(ByteT))
      return DPOPortBaseC();

    return DPOByteFileC(outputStream);
  }
  //: Create an output port for saving.
  //!return: An invalid port if not supported.

  DPIPortBaseC FileFormatByteFileBodyC::CreateInput(const StringC& filename, const std::type_info& objType) const
  {
    // Byte only
    if (objType != typeid(ByteT))
      return DPIPortBaseC();

    return DPIByteFileC(filename);
  }
  //: Create an input port for loading.
  //!return: An invalid port if not supported.

  DPOPortBaseC FileFormatByteFileBodyC::CreateOutput(const StringC& filename, const std::type_info& objType) const
  {
    // Byte only
    if (objType != typeid(ByteT))
      return DPOPortBaseC();

    return DPOByteFileC(filename);
  }
  //: Create an output port for saving.
  //!return: An invalid port if not supported.

  const std::type_info& FileFormatByteFileBodyC::DefaultType() const
  {
    return typeid(ByteT);
  }
  //: Get preferred data type.

  bool FileFormatByteFileBodyC::IsStream() const
  {
    return true;
  }
  //: Show it is a streamable format.

  IntT FileFormatByteFileBodyC::Priority() const
  {
    return -1; // Default is 0.
  }
  //: Find the priority of the format. The higher the better.

  void InitFileFormatByteFile()
  {}
  
  static FileFormatByteFileC g_fileFormatByteFile;
  
}
