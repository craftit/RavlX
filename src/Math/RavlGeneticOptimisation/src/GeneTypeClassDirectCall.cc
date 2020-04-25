// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneTypeClassDirectCall.hh"

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
  GeneTypeClassDirectCallC::GeneTypeClassDirectCallC(const XMLFactoryContextC &factory)
   : GeneTypeClassC(factory)
  {
  }

  //! Constructor
  GeneTypeClassDirectCallC::GeneTypeClassDirectCallC(const std::type_info &classType,const CallFunc2C<const GeneFactoryC &,RCWrapAbstractC &> &generator)
   : GeneTypeClassC(classType),
     m_generator(generator)
  {
  }

  //! Load form a binary stream
  GeneTypeClassDirectCallC::GeneTypeClassDirectCallC(BinIStreamC &strm)
   : GeneTypeClassC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("RavlN::GeneTypeClassDirectCallC");
    RavlIssueError("not supported");
  }

  //! Load form a binary stream
  GeneTypeClassDirectCallC::GeneTypeClassDirectCallC(std::istream &strm)
   : GeneTypeClassC(strm)
  {
    RavlIssueError("not implemented");
  }

  //! Save to binary stream
  bool GeneTypeClassDirectCallC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeClassC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    RavlIssueError("not supported");
    return true;
  }

  //! Save to binary stream
  bool GeneTypeClassDirectCallC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Method for generating the class.
  void GeneTypeClassDirectCallC::Generate(const GeneFactoryC &childContext,RCWrapAbstractC &handle) const
  {
    RavlAssert(m_generator.IsValid());
    m_generator.Call(const_cast<GeneFactoryC &>(childContext),handle);
  }

  static RavlN::TypeNameC g_typePtrGeneTypeClassDirectCall(typeid(GeneTypeClassDirectCallC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeClassDirectCallC>");
  static XMLFactoryRegisterConvertC<GeneTypeClassDirectCallC,GeneTypeClassC> g_registerGeneTypeClassDirectCall("RavlN::GeneticN::GeneTypeClassDirectCallC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeClassDirectCallC,"RavlN::GeneticN::GeneTypeClassDirectCallC");

  //
}}
