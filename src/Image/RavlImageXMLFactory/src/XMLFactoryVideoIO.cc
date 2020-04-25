// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlImageXMLFactory
//! author=Charles Galambos
//! docentry=Ravl.API.Images.XMLFactory

#include "Ravl/Image/XMLFactoryVideoIO.hh"
#include "Ravl/Image/ByteRGBValue.hh"

namespace RavlN {
  
  XMLFactoryVideoIORegisterC<RavlN::ByteT> g_registerXMLFactoryVideoIOByteT("RavlN::DPIPortC<RavlImageN::ImageC<RavlN::ByteT>>",
                                                                            "RavlN::DPOPortC<RavlImageN::ImageC<RavlN::ByteT>>");
  XMLFactoryVideoIORegisterC<RavlN::UInt16T> g_registerXMLFactoryVideoIOUInt16T("RavlN::DPIPortC<RavlImageN::ImageC<RavlN::UInt16T>>",
                                                                            "RavlN::DPOPortC<RavlImageN::ImageC<RavlN::UInt16T>>");
  XMLFactoryVideoIORegisterC<RavlImageN::ByteRGBValueC> g_registerXMLFactoryVideoIOByteRGBValueC("RavlN::DPIPortC<RavlImageN::ImageC<RavlImageN::ByteRGBValueC>>",
                                                                                                 "RavlN::DPOPortC<RavlImageN::ImageC<RavlImageN::ByteRGBValueC>>"
                                                                                                 );
  XMLFactoryVideoIORegisterC<float> g_registerXMLFactoryVideoIOfloat("RavlN::DPIPortC<RavlImageN::ImageC<float>>",
                                                                     "RavlN::DPOPortC<RavlImageN::ImageC<float>>"
                                                                     );
  
  XMLFactoryVideoIORegisterC<RealT> g_registerXMLFactoryVideoIORealT("RavlN::DPIPortC<RavlImageN::ImageC<RealT>>",
                                                                     "RavlN::DPOPortC<RavlImageN::ImageC<RealT>>"
                                                                     );
  
  void linkXMLFactoryVideoIOByte()
  {}
  
}
