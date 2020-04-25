// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneTypeClassDirect.hh"

#include "Ravl/Random.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/Genetic/GeneFactory.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {

  //! Factory constructor
  GeneTypeClassDirectFuncC::GeneTypeClassDirectFuncC(const XMLFactoryContextC &factory)
   : GeneTypeClassC(factory),
     m_generator(0)
  {
  }

  //! Constructor
  GeneTypeClassDirectFuncC::GeneTypeClassDirectFuncC(const std::type_info &classType,GenerateFuncT *generatorFunc)
   : GeneTypeClassC(classType),
     m_generator(generatorFunc)
  {
    RavlAssert(m_generator != 0);
  }

  //! Load form a binary stream
  GeneTypeClassDirectFuncC::GeneTypeClassDirectFuncC(BinIStreamC &strm)
   : GeneTypeClassC(strm),
     m_generator(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("RavlN::GeneTypeClassDirectFuncC");
  }

  //! Load form a binary stream
  GeneTypeClassDirectFuncC::GeneTypeClassDirectFuncC(std::istream &strm)
   : GeneTypeClassC(strm),
     m_generator(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeClassDirectFuncC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeClassC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;

    return true;
  }

  //! Save to binary stream
  bool GeneTypeClassDirectFuncC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Method for generating the class.
  void GeneTypeClassDirectFuncC::Generate(const GeneFactoryC &childContext,RCWrapAbstractC &handle) const
  {
    RavlAssert(m_generator != 0);
    (*m_generator)(childContext,handle);
  }

  static RavlN::TypeNameC g_typePtrGeneTypeClassDirect(typeid(GeneTypeClassDirectFuncC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeClassDirectFuncC>");
  static XMLFactoryRegisterConvertC<GeneTypeClassDirectFuncC,GeneTypeClassC> g_registerGeneTypeClassDirect("RavlN::GeneticN::GeneTypeClassDirectFuncC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeClassDirectFuncC,"RavlN::GeneticN::GeneTypeClassDirectFuncC");

  //
}}
