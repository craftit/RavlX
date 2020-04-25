// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneTypeBool.hh"
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
  GeneTypeBoolC::GeneTypeBoolC(const XMLFactoryContextC &factory)
   : GeneTypeC(factory),
     m_bias(factory.AttributeReal("bias",0.5))
  {}

  //! Constructor
  GeneTypeBoolC::GeneTypeBoolC(const std::string &name,float bias)
  : GeneTypeC(name),
    m_bias(bias)
  {}

  //! Load form a binary stream
  GeneTypeBoolC::GeneTypeBoolC(BinIStreamC &strm)
   : GeneTypeC(strm),
     m_bias(0.5)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeBoolC");
    strm >> m_bias;
    ONDEBUG(RavlDebug("Loading '%s' Bias:%f ",Name().data(),m_bias));
  }

  //! Load form a binary stream
  GeneTypeBoolC::GeneTypeBoolC(std::istream &strm)
   : GeneTypeC(strm),
     m_bias(0.5)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeBoolC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << m_bias;
    return true;
  }


  //! Save to binary stream
  bool GeneTypeBoolC::Save(std::ostream &strm) const
  {
    strm << " GeneTypeBool Bias=" << m_bias;
    return false;
  }


  //! Create randomise value
  void GeneTypeBoolC::Random(GenePaletteC &palette,GeneC::RefT &newGene) const
  {
    bool newValue = palette.Random1() < m_bias;
    newGene = new GeneBoolC(*this,newValue);
  }

  //! Mutate a gene
  bool GeneTypeBoolC::Mutate(GenePaletteC &palette,
                            float fraction,
                            bool mustChange,
                            const GeneC &original,
                            RavlN::SmartPtrC<GeneC> &newGene) const
  {
    if(fraction <= 0 && !mustChange) {
      newGene = &original;
      return false;
    }
    const GeneBoolC &originalBool = dynamic_cast<const GeneBoolC &>(original);
    // FIXME:- Could cash a true and false GeneBool's in type and save some memeory allocation ?
    bool newValue = false;
    if(mustChange || fraction <= 0) {
      newValue = !originalBool.Value();
    } else {
      // Decide if we're going to change the value.
      if(fraction < palette.Random1()) {
        newGene = &originalBool;
        return false;
      }

      // Pick a new value, maybe the same as the last!
      newValue = palette.Random1() < m_bias;
      if(newValue == originalBool.Value()) {
        newGene = &originalBool;
        return false;
      }
    }

    newGene = new GeneBoolC(*this,newValue);
    return originalBool.Value() != newValue;
  }

  //!  Default values for basic types
  const GeneTypeC &GeneType(bool value) {
    static GeneTypeC::RefT defaultIntType = new GeneTypeBoolC("DefaultBool",0.5);
    return *defaultIntType;
  }

  XMLFactoryRegisterConvertC<GeneTypeBoolC,GeneTypeC> g_registerGeneTypeBool("RavlN::GeneticN::GeneTypeBoolC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeBoolC,"RavlN::GeneticN::GeneTypeBoolC");
  static RavlN::TypeNameC g_typePtrGeneTypeBool(typeid(GeneTypeBoolC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeBoolC>");

  //! Access a gene type with 50/50 bias
  const GeneTypeBoolC &GeneTypeBoolBiasHalf() {
    static GeneTypeBoolC::RefT ret = new GeneTypeBoolC("BiasHalf",0.5);
    return *ret;
  }

  // ------------------------------------------------------------------

  //! Factory constructor
  GeneBoolC::GeneBoolC(const XMLFactoryContextC &factory)
   : GeneC(factory),
     m_value(factory.AttributeBool("value",false))
  {
    RavlAssertMsg(dynamic_cast<GeneTypeBoolC *>(m_type.BodyPtr()) != 0,"Wrong type");
  }

  GeneBoolC::GeneBoolC(const GeneTypeC &geneType,IntT value)
  : GeneC(geneType),
    m_value(value)
  {}

  //! Load form a binary stream
  GeneBoolC::GeneBoolC(BinIStreamC &strm)
   : GeneC(strm),
     m_value(false)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneC");
    strm >> m_value;
  }

  //! Load form a binary stream
  GeneBoolC::GeneBoolC(std::istream &strm)
   : GeneC(strm),
     m_value(false)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneBoolC::Save(BinOStreamC &strm) const
  {
    if(!GeneC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << m_value;
    return true;
  }


  //! Save to binary stream
  bool GeneBoolC::Save(std::ostream &strm) const
  {
    strm << "GeneBool " << m_value;
    return true;
  }

  //! Save to binary stream
  bool GeneBoolC::Save(RavlN::XMLOStreamC &strm) const {
    GeneC::Save(strm);
    strm << XMLAttribute("value",m_value) ;
    return true;
  }

  //! Generate an instance of the class.
  void GeneBoolC::Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const {
    if(handle.IsValid()) {
      bool *theValue = 0;
      handle.GetPtr(theValue);
      *theValue = m_value;
    } else {
      handle = RCWrapC<bool>(m_value);
    }
  }

  //! Test is value is effectively equal to this within tolerances specified in the type.
  bool GeneBoolC::IsEffectivelyEqual(const GeneC &other) const {
    if(other.Type() != Type()) return false;
    const GeneBoolC* og = dynamic_cast<const GeneBoolC*>(&other);
    if(og == 0)
      return false;
    return (og->Value() == m_value);
  }

  //! Dump description in human readable form.
  void GeneBoolC::Dump(std::ostream &strm,UIntT indent) const
  {
    GeneC::Dump(strm,indent);
    strm << " Value=";
    if(m_value)
      strm << "True";
    else
      strm << "False";
  }


  XMLFactoryRegisterConvertC<GeneBoolC,GeneC> g_registerGeneBool("RavlN::GeneticN::GeneBoolC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneBoolC,"RavlN::GeneticN::GeneBoolC");
  static RavlN::TypeNameC g_typePtrGeneBool(typeid(GeneBoolC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneBoolC>");

  void LinkGeneTypeBool()
  {}
}}
