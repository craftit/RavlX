// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneTypeFloatExp.hh"
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

  //! Factory constructor
  GeneTypeFloatExpC::GeneTypeFloatExpC(const XMLFactoryContextC &factory)
   : GeneTypeFloatC(factory)
  {
  }

  //! Constructor
  GeneTypeFloatExpC::GeneTypeFloatExpC(const std::string &name,float min,float max,float inc)
  : GeneTypeFloatC(name,min,max,inc)
  {}

  //! Load form a binary stream
  GeneTypeFloatExpC::GeneTypeFloatExpC(BinIStreamC &strm)
   : GeneTypeFloatC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeFloatGuass");
  }

  //! Load form a binary stream
  GeneTypeFloatExpC::GeneTypeFloatExpC(std::istream &strm)
   : GeneTypeFloatC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeFloatExpC::Save(BinOStreamC &strm) const {
    if(!GeneTypeFloatC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    return true;
  }

  //! Save to binary stream
  bool GeneTypeFloatExpC::Save(std::ostream &strm) const {
    GeneTypeFloatC::Save(strm);
    return true;
  }

  //! Generate a new value
  void GeneTypeFloatExpC::RandomValue(GenePaletteC &palette,float &newValue) const {
    float lmin = Log10(m_min);
    float lmax = Log10(m_max);
    newValue = Pow(10.0,(palette.Random1() * (lmax - lmin)) + lmin);
  }


  //! Mutate a gene
  bool GeneTypeFloatExpC::Mutate(
      GenePaletteC &palette,
      float fraction,
      bool mustChange,
      const GeneC &original,
      RavlN::SmartPtrC<GeneC> &newGene
      ) const
  {
    if(fraction <= 0 && !mustChange) {
      newGene = &original;
      return false;
    }
    const GeneFloatC &originalFloat = dynamic_cast<const GeneFloatC &>(original);

    // FIXME:- Is this the best way of mutating ?
    float lmin = Log10(m_min);
    float lmax = Log10(m_max);
    float value = (palette.Random1() * (lmax - lmin)) + lmin;

    float loldValue = Log10(static_cast<float>(originalFloat.Value()));
    float lnewValue = (1.0f - fraction) * loldValue + fraction * value;

    // Make sure there is some change if required
    if(mustChange && IsEffectivelyEqual(loldValue,lnewValue)) {
      if(m_inc > 0) {
        if(palette.Random1() > 0.5 && lnewValue < m_max) {
          lnewValue += m_inc;
        } else {
          lnewValue -= m_inc;
        }
      } else {
        // Just pick another random one, the chances of it being the same are very small.
        lnewValue = (palette.Random1() * (m_max - m_min)) + lmin;
        // Maybe we have a stupidly small range?
        if(IsEffectivelyEqual(loldValue,lnewValue))
          lnewValue = (palette.Random1() * (m_max - m_min)) + lmin;
      }
    }

    // Clip to valid range just in case of rounding problems
    if(lnewValue < lmin)
      lnewValue = lmin;
    if(lnewValue > lmax)
      lnewValue = lmax;

    newGene = new GeneFloatC(*this,Pow(10,lnewValue));
    return IsEffectivelyEqual(loldValue,lnewValue);
  }


  static XMLFactoryRegisterConvertC<GeneTypeFloatExpC,GeneTypeFloatC> g_registerGeneTypeFloatExp("RavlN::GeneticN::GeneTypeFloatExpC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeFloatExpC,"RavlN::GeneticN::GeneTypeFloatExpC");
  static RavlN::TypeNameC g_typePtrGeneTypeFloatExp(typeid(GeneTypeFloatExpC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeFloatExpC>");

}}

