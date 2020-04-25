// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FILEFORMATDESC_HEADER
#define RAVL_FILEFORMATDESC_HEADER 1
//////////////////////////////////////////////////
//! docentry="Ravl.API.Core.IO.Formats" 
//! lib=RavlIO
//! author="Charles Galambos"
//! date="13/08/1998"
//! rcsid="$Id$"
//! file="Ravl/Core/IO/FileFormatDesc.hh"
//! userlevel=Default

#include "Ravl/DList.hh"
#include "Ravl/DP/FileFormat.hh"
#include "Ravl/DP/DataConv.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN {
  
  class DPSeekCtrlC;
  
  ///////////////////////////////////
  //! userlevel=Advanced
  //: File Format descriptor.
  // Small object.
  
  class FileFormatDescC {
  public:
    FileFormatDescC()
      : stype(&typeid(void))
    {}
    //: Default constructor.
    
    FileFormatDescC(FileFormatBaseC nform,DListC<DPConverterBaseC> nconv,const std::type_info &ntype,bool nisInput)
      : form(nform),
        conv(nconv),
        stype(&ntype),
        isInput(nisInput)
    {}
    //: Constructor.
    //!param: nform - Target file format
    //!param: ntype - Requested target type
    //!param: nconv - Conversion sequence.
    //!param: nisInput - Is this conversion for input ? if not its output.
    
    FileFormatDescC(DListC<DPConverterBaseC> nconv,bool nisInput)
      : conv(nconv),
        stype(&typeid(void)),
        isInput(nisInput)
    {}
    //: Constructor.
    //!param: nconv - Conversion sequence.
    //!param: nisInput - Is this conversion for input ? if not its output.
    
    const FileFormatBaseC &Format() const { return form; }
    //: File format.
    
    const DListC<DPConverterBaseC> &Conversion() const { return conv; }
    //: Required type conversion.
    
    const std::type_info &SourceType() const { return *stype; }
    //: Type to request in load.
    
    bool IsInput() const { return isInput; }
    //: Is an input descriptor ?
    
    DPIPortBaseC CreateInput(StringC filename,IStreamC &inStream) const;
    //: Create an input from the descriptor.
    // If the file has already been opened 'inStream' will be valid.
    
    DPOPortBaseC CreateOutput(StringC filename) const;
    //: Create an input from the descriptor.
    
    DPIPortBaseC CreateInput(StringC filename,IStreamC &inStream,DPSeekCtrlC &sc) const;
    //: Create an input from the descriptor.
    
    DPOPortBaseC CreateOutput(StringC filename,DPSeekCtrlC &sc) const;
    //: Create an input from the descriptor.
    
    DPIPortBaseC BuildInputConv(const DPIPortBaseC &ip) const;
    //: Build input conversion.
    
    DPOPortBaseC BuildOutputConv(const DPOPortBaseC &op) const;
    //: Build output conversion.
    
    void DumpConv(std::ostream &out) const;
    //: Dump conversion to out.
    
  protected:
    FileFormatBaseC form;
    DListC<DPConverterBaseC> conv;
    const std::type_info *stype;  // Not used internally.
    bool isInput;
  };
  
}
#endif
