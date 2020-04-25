// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeConst.hh"
#include "Ravl/Genetic/GenePalette.hh"

#include "Ravl/Random.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"

#include <math.h>

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {

  // ------------------------------------------------------------------

  //! Factory constructor
  GeneTypeIntC::GeneTypeIntC(const XMLFactoryContextC &factory)
   : GeneTypeC(factory),
     m_min(factory.AttributeInt("min",0)),
     m_max(factory.AttributeInt("max",100))
  {}

  //! Constructor
  GeneTypeIntC::GeneTypeIntC(const std::string &name,IntT min,IntT max)
  : GeneTypeC(name),
    m_min(min),
    m_max(max)
  {}

  //! Load form a binary stream
  GeneTypeIntC::GeneTypeIntC(BinIStreamC &strm)
   : GeneTypeC(strm),
     m_min(0),
     m_max(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeIntC");
    strm >> m_min >> m_max;
    ONDEBUG(RavlDebug("Loading '%s'  Min:%d Max:%d ",Name().data(),m_min,m_max));
  }

  //! Load form a binary stream
  GeneTypeIntC::GeneTypeIntC(std::istream &strm)
   : GeneTypeC(strm),
     m_min(0),
     m_max(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeIntC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << m_min << m_max;
    return true;
  }


  //! Save to binary stream
  bool GeneTypeIntC::Save(std::ostream &strm) const
  {
    strm << " GeneTypeInt " << m_min << " - " << m_max;
    return false;
  }


  //! Create randomise value
  void GeneTypeIntC::Random(GenePaletteC &palette,GeneC::RefT &newGene) const
  {
    float value = palette.Random1() * static_cast<float>((1 + m_max) - m_min) + static_cast<float>(m_min) ;
    IntT newValue = Floor(value);
    if(newValue < m_min)
       newValue = m_min;
     if(newValue > m_max)
       newValue = m_max;
     newGene = new GeneIntC(*this,newValue);
  }

  //! Mutate a gene
  bool GeneTypeIntC::Mutate(GenePaletteC &palette,
                            float fraction,
                            bool mustChange,
                            const GeneC &original,
                            RavlN::SmartPtrC<GeneC> &newGene) const
  {
    if(fraction <= 0 && !mustChange) {
      newGene = &original;
      return false;
    }
    const GeneIntC &originalInt = dynamic_cast<const GeneIntC &>(original);

    IntT newValue;

    float value = static_cast<float>(palette.Random1() * ((1 + m_max) - m_min)) + static_cast<float>(m_min);
    // FIXME:- Is this the best way of mutating ? Often it won't be any change at all??
    newValue = Floor((1.0 -fraction) * static_cast<float>(originalInt.Value()) + fraction * value);

    // Make sure there is some change if required
    if((mustChange || (fraction > palette.Random1())) && originalInt.Value() == newValue) {
      if(palette.Random1() > 0.5 && newValue < m_max) {
        newValue++;
      } else {
        newValue--;
      }
    }

    // Clip to valid range just in case of rounding problems, or no room for change.
    if(newValue < m_min)
      newValue = m_min;
    if(newValue > m_max)
      newValue = m_max;

    newGene = new GeneIntC(*this,newValue);
    return originalInt.Value() != newValue;
  }

  //!  Default values for basic types
  const GeneTypeC &GeneType(IntT value) {
    static GeneTypeC::RefT defaultIntType = new GeneTypeIntC("DefaultInt",0,100);
    return *defaultIntType;
  }

  XMLFactoryRegisterConvertC<GeneTypeIntC,GeneTypeC> g_registerGeneTypeInt("RavlN::GeneticN::GeneTypeIntC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeIntC,"RavlN::GeneticN::GeneTypeIntC");
  static RavlN::TypeNameC g_typePtrGeneTypeInt(typeid(GeneTypeIntC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeIntC>");

  // ------------------------------------------------------------------

  //! Factory constructor
  GeneIntC::GeneIntC(const XMLFactoryContextC &factory)
   : GeneC(factory),
     m_value(factory.AttributeInt("value",0))
  {
    RavlAssertMsg(dynamic_cast<GeneTypeIntC *>(m_type.BodyPtr()) != 0,"Wrong type");
  }

  GeneIntC::GeneIntC(const GeneTypeC &geneType,IntT value)
  : GeneC(geneType),
    m_value(value)
  {}

  //! Load form a binary stream
  GeneIntC::GeneIntC(BinIStreamC &strm)
   : GeneC(strm),
     m_value(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneC");
    strm >> m_value;
  }

  //! Load form a binary stream
  GeneIntC::GeneIntC(std::istream &strm)
   : GeneC(strm),
     m_value(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneIntC::Save(BinOStreamC &strm) const
  {
    if(!GeneC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << m_value;
    return true;
  }


  //! Save to binary stream
  bool GeneIntC::Save(std::ostream &strm) const
  {
    strm << "GeneInt " << m_value;
    return true;
  }

  //! Save to binary stream
  bool GeneIntC::Save(RavlN::XMLOStreamC &strm) const {
    GeneC::Save(strm);
    strm << XMLAttribute("value",m_value) ;
    return true;
  }

  //! Test is value is effectively equal to this within tolerances specified in the type.
  bool GeneIntC::IsEffectivelyEqual(const GeneC &other) const {
    if(other.Type() != Type()) return false;
    const GeneIntC* og = dynamic_cast<const GeneIntC*>(&other);
    if(og == 0)
      return false;
    return (og->Value() == m_value);
  }

  //! Dump description in human readable form.
  void GeneIntC::Dump(std::ostream &strm,UIntT indent) const {
    GeneC::Dump(strm,indent);
    strm << " Value=" << m_value;
  }

  //! Generate an instance of the class.
  void GeneIntC::Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const {
    if(handle.IsValid()) {
      IntT *theValue = 0;
      handle.GetPtr(theValue);
      *theValue = m_value;
    } else {
      handle = RCWrapC<IntT>(m_value);
    }
  }

  XMLFactoryRegisterConvertC<GeneIntC,GeneC> g_registerGeneInt("RavlN::GeneticN::GeneIntC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneIntC,"RavlN::GeneticN::GeneIntC");
  static RavlN::TypeNameC g_typePtrGeneInt(typeid(GeneIntC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneIntC>");

  // ------------------------------------------------------------------

  //! Factory constructor
  GeneTypeFloatC::GeneTypeFloatC(const XMLFactoryContextC &factory)
   : GeneTypeC(factory),
     m_min(static_cast<float>(factory.AttributeReal("min",0.0))),
     m_max(static_cast<float>(factory.AttributeReal("max",100.0))),
     m_inc(static_cast<float>(factory.AttributeReal("inc",0.0)))
  {}

  //! Constructor
  GeneTypeFloatC::GeneTypeFloatC(const std::string &name,float min,float max,float inc)
  : GeneTypeC(name),
    m_min(min),
    m_max(max),
    m_inc(inc)
  {}

  GeneTypeFloatC::GeneTypeFloatC(BinIStreamC &strm)
   : GeneTypeC(strm),
     m_min(0),
     m_max(0),
     m_inc(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version < 1 || version > 2)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeFloatC");
    strm >> m_min >> m_max;
    if(version > 1) {
      strm >> m_inc;
    } else {
      m_inc = 0.0;
    }
  }

  //! Load form a binary stream
  GeneTypeFloatC::GeneTypeFloatC(std::istream &strm)
   : GeneTypeC(strm),
     m_min(0),
     m_max(0),
     m_inc(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeFloatC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeC::Save(strm))
      return false;
    ByteT version = 2;
    strm << version << m_min << m_max << m_inc;
    return true;
  }


  //! Save to binary stream
  bool GeneTypeFloatC::Save(std::ostream &strm) const
  {
    strm << "GeneTypeFloatC";
    return false;
  }

  //! Generate a new value
  void GeneTypeFloatC::RandomValue(GenePaletteC &palette,float &newValue) const {
    newValue = static_cast<float>(palette.Random1() * (m_max - m_min)) + static_cast<float>(m_min);
    if(newValue < m_min)
       newValue = m_min;
     if(newValue > m_max)
       newValue = m_max;
  }

  //! Create randomise value
  void GeneTypeFloatC::Random(GenePaletteC &palette,GeneC::RefT &newGene) const
  {
    float newValue;
    RandomValue(palette,newValue);
    newGene = new GeneFloatC(*this,newValue);
  }

  //! Check if values are the same under constraints included in the type.
  bool GeneTypeFloatC::IsEffectivelyEqual(float v1,float v2) const {
    if(m_inc > 0) return Abs(v1 - v2) < m_inc;
    return v1 != v2;
  }

  //! Mutate a gene
  bool GeneTypeFloatC::Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newGene) const
  {
    if(fraction <= 0 && !mustChange) {
      newGene = &original;
      return false;
    }
    const GeneFloatC &originalFloat = dynamic_cast<const GeneFloatC &>(original);
    float value;
    // FIXME:- Is this the best way of mutating ?
    RandomValue(palette,value);
    float newValue = (1.0f - fraction) * static_cast<float>(originalFloat.Value()) + fraction * value;

    // Make sure there is some change if required
    if(mustChange && IsEffectivelyEqual(originalFloat.Value(),newValue)) {
      if(m_inc > 0) {
        if(palette.Random1() > 0.5 && newValue < m_max) {
          newValue += m_inc;
        } else {
          newValue -= m_inc;
        }
      } else {
        // Just pick another random one, the chances of it being the same are very small.
        RandomValue(palette,newValue);
        // Maybe we have a stupidly small range?
        if(IsEffectivelyEqual(originalFloat.Value(),newValue))
          RandomValue(palette,newValue);
      }
    }

    // Clip to valid range just in case of rounding problems
    if(newValue < m_min)
      newValue = m_min;
    if(newValue > m_max)
      newValue = m_max;

    newGene = new GeneFloatC(*this,newValue);
    return IsEffectivelyEqual(originalFloat.Value(),newValue);
  }

  //!  Default values for basic types
  const GeneTypeC &GeneType(float value) {
    static GeneTypeC::RefT defaultFloatType = new GeneTypeFloatC("DefaultFloat",0.0,1.0);
    return *defaultFloatType;
  }

  XMLFactoryRegisterConvertC<GeneTypeFloatC,GeneTypeC> g_registerGeneTypeFloat("RavlN::GeneticN::GeneTypeFloatC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeFloatC,"RavlN::GeneticN::GeneTypeFloatC");
  static RavlN::TypeNameC g_typePtrGeneTypeFloat(typeid(GeneTypeFloatC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeFloatC>");

  // ------------------------------------------------------------------

  //! Factory constructor
  GeneFloatC::GeneFloatC(const XMLFactoryContextC &factory)
   : GeneC(factory),
     m_value(static_cast<float>(factory.AttributeReal("value",0.0)))
  {
    RavlAssertMsg(dynamic_cast<GeneTypeFloatC *>(m_type.BodyPtr()) != 0,"Wrong type");
  }

  //! Constructor
  GeneFloatC::GeneFloatC(const GeneTypeC &geneType,float value)
   : GeneC(geneType),
     m_value(value)
  {}

  //! Load form a binary stream
  GeneFloatC::GeneFloatC(BinIStreamC &strm)
   : GeneC(strm),
     m_value(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneC");
    strm >> m_value;
  }

  //! Load form a binary stream
  GeneFloatC::GeneFloatC(std::istream &strm)
   : GeneC(strm),
     m_value(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneFloatC::Save(BinOStreamC &strm) const
  {
    if(!GeneC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << m_value;
    return true;
  }


  //! Save to binary stream
  bool GeneFloatC::Save(std::ostream &strm) const
  {
    strm << "GeneFloat " << m_value;
    return false;
  }

  //! Save to binary stream
  bool GeneFloatC::Save(RavlN::XMLOStreamC &strm) const {
    GeneC::Save(strm);
    strm << XMLAttribute("value",m_value) ;
    return true;
  }

  //! Generate a hash value for the type.
  size_t GeneFloatC::Hash() const {
    const GeneTypeFloatC *gtf = dynamic_cast<const GeneTypeFloatC *>(m_type.BodyPtr());
    if(gtf != 0 && gtf->SmallestIncrement() > 0)
      return StdHash(Floor(m_value / gtf->SmallestIncrement()));

    return StdHash(m_value);
  }

  //! Test is value is effectively equal to this within tolerances specified in the type.
  bool GeneFloatC::IsEffectivelyEqual(const GeneC &other) const {
    if(other.Type() != Type()) return false;
    const GeneFloatC* og = dynamic_cast<const GeneFloatC*>(&other);
    if(og == 0) return false;
    const GeneTypeFloatC *gtf = dynamic_cast<const GeneTypeFloatC *>(m_type.BodyPtr());
    RavlAssert(gtf != 0);
    return gtf->IsEffectivelyEqual(og->Value(),m_value);
  }

  //! Generate an instance of the class.
  void GeneFloatC::Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const {
    if(handle.IsValid()) {
      float *theValue = 0;
      handle.GetPtr(theValue);
      *theValue = m_value;
    } else {
      handle = RCWrapC<float>(m_value);
    }
  }

  //! Dump description in human readable form.
  void GeneFloatC::Dump(std::ostream &strm,UIntT indent) const {
    GeneC::Dump(strm,indent);
    strm << " Value=" << m_value;
  }

  XMLFactoryRegisterConvertC<GeneFloatC,GeneC> g_registerGeneFloat("RavlN::GeneticN::GeneFloatC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneFloatC,"RavlN::GeneticN::GeneFloatC");
  static RavlN::TypeNameC g_typePtrGeneFloat(typeid(GeneFloatC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneFloatC>");

}}

