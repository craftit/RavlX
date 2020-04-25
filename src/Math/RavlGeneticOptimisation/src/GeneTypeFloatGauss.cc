// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneTypeFloatGauss.hh"
#include "Ravl/Genetic/GenePalette.hh"

#include "Ravl/Random.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/Exception.hh"
#include "Ravl/OS/SysLog.hh"

namespace RavlN { namespace GeneticN {

  static const char *g_foldModeNames[] = {
      "None",
      "Up",
      "Down",
      0
  };

  std::ostream &operator<<(std::ostream &strm,GeneTypeFloatGaussC::FoldModeT fm) {
    strm << g_foldModeNames[(int) fm];
    return strm;
  }

  std::istream &operator>>(std::istream &strm,GeneTypeFloatGaussC::FoldModeT &fm) {
    std::string name;
    strm >> name;
    unsigned i = 0;
    for(;g_foldModeNames[i] != 0;i++) {
      if(name == g_foldModeNames[i]) {
        fm = static_cast<GeneTypeFloatGaussC::FoldModeT>(i);
        break;
      }
    }
    if(g_foldModeNames[i] == 0) {
      RavlSysLogf(SYSLOG_ERR,"Unknown fold mode name '%s' ",name.data());
      throw RavlN::ExceptionBadConfigC("Unknown fold mode.");
    }
    return strm;
  }

  RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &strm,GeneTypeFloatGaussC::FoldModeT fm) {
    strm << (Int32T) fm;
    return strm;
  }
  RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm,GeneTypeFloatGaussC::FoldModeT &fm) {
    Int32T mode;
    strm >> mode;
    fm = static_cast<GeneTypeFloatGaussC::FoldModeT>(mode);
    return strm;
  }

  //! Factory constructor
  GeneTypeFloatGaussC::GeneTypeFloatGaussC(const XMLFactoryContextC &factory)
   : GeneTypeFloatC(factory),
     m_width(static_cast<float>(factory.AttributeReal("width",1.0))),
     m_offset(static_cast<float>(factory.AttributeReal("offset",0.0)))
  {
    factory.Attribute("foldMode",m_foldMode,FoldNone);
  }

  //! Constructor
  GeneTypeFloatGaussC::GeneTypeFloatGaussC(const std::string &name,float min,float max,float width,float offset,FoldModeT foldMode)
  : GeneTypeFloatC(name,min,max),
    m_width(width),
    m_offset(offset),
    m_foldMode(foldMode)
  {}

  //! Load form a binary stream
  GeneTypeFloatGaussC::GeneTypeFloatGaussC(BinIStreamC &strm)
   : GeneTypeFloatC(strm),
     m_width(0),
     m_offset(0),
     m_foldMode(FoldNone)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeFloatGuass");
    strm >> m_width >> m_offset >> m_foldMode;
  }

  //! Load form a binary stream
  GeneTypeFloatGaussC::GeneTypeFloatGaussC(std::istream &strm)
   : GeneTypeFloatC(strm),
     m_width(0),
     m_offset(0),
     m_foldMode(FoldNone)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeFloatGaussC::Save(BinOStreamC &strm) const {
    if(!GeneTypeFloatC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    strm << m_width << m_offset << m_foldMode;
    return true;
  }

  //! Save to binary stream
  bool GeneTypeFloatGaussC::Save(std::ostream &strm) const {
    GeneTypeFloatC::Save(strm);
    strm << " Width=" << m_width << " Offset=" << m_offset << " Fold=" <<m_foldMode;
    return true;
  }

  //! Generate a new value
  void GeneTypeFloatGaussC::RandomValue(GenePaletteC &palette,float &newValue) const {
    newValue = palette.RandomGauss() * m_width;
    switch(m_foldMode) {
      case FoldNone:  break;
      case FoldUp: {
        if(newValue < 0)
          newValue *= -1;
      } break;
      case FoldDown: {
        if(newValue > 0)
          newValue *= -1;
      } break;
    }
    newValue += m_offset;
    if(newValue < m_min)
      newValue = m_min;
    if(newValue > m_max)
      newValue = m_max;
  }

  static XMLFactoryRegisterConvertC<GeneTypeFloatGaussC,GeneTypeFloatC> g_registerGeneTypeFloatGauss("RavlN::GeneticN::GeneTypeFloatGaussC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeFloatGaussC,"RavlN::GeneticN::GeneTypeFloatGaussC");
  static RavlN::TypeNameC g_typePtrGeneTypeFloatGauss(typeid(GeneTypeFloatGaussC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeFloatGaussC>");

}}

