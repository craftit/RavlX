// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneTypeProxy.hh"
#include "Ravl/Genetic/GenePalette.hh"
#include "Ravl/Random.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"
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
   GeneTypeProxyC::GeneTypeProxyC(const XMLFactoryContextC &factory)
    : GeneTypeC(factory),
      m_proxyName(factory.AttributeString("proxyName",factory.Name().data()))
   {
   }

   //! Constructor
   GeneTypeProxyC::GeneTypeProxyC(const StringC &name)
     : GeneTypeC(name),
       m_proxyName(name)
   {}

   //! Load form a binary stream
   GeneTypeProxyC::GeneTypeProxyC(BinIStreamC &strm)
    : GeneTypeC(strm)
   {
     ByteT version = 0;
     strm >> version;
     if(version != 1)
       throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeProxyC");
     strm >> m_proxyName;
   }

   //! Load form a binary stream
   GeneTypeProxyC::GeneTypeProxyC(std::istream &strm)
    : GeneTypeC(strm)
   {
     RavlAssertMsg(0,"not implemented");
   }

   //! Save to binary stream
   bool GeneTypeProxyC::Save(BinOStreamC &strm) const
   {
     if(!GeneTypeC::Save(strm))
       return false;
     ByteT version = 1;
     strm << version << m_proxyName;

     return true;
   }

   //! Save to binary stream
   bool GeneTypeProxyC::Save(std::ostream &strm) const
   {
     RavlAssertMsg(0,"not implemented");
     return false;
   }

   //! Dump description in human readable form.
   void GeneTypeProxyC::Dump(std::ostream &strm,UIntT indent) const {
     GeneTypeC::Dump(strm,indent);
     strm << "Proxy:" << m_proxyName <<"\n";
   }

   //! Create randomise value
   void GeneTypeProxyC::Random(GenePaletteC &palette,GeneC::RefT &newValue) const
   {
     SmartPtrC<const GeneTypeC> geneType;
     if(!palette.ProxyMap().Lookup(m_proxyName,geneType)) {
       RavlError("No proxy value for '%s' ",m_proxyName.data());
       throw RavlN::ExceptionOperationFailedC("No value for proxy");
     }
     RavlAssert(geneType.IsValid());
     geneType->Random(palette,newValue);
   }

   //! Mutate a gene
   bool GeneTypeProxyC::Mutate(GenePaletteC &palette,
                               float fraction,
                               bool mustChange,
                               const GeneC &original,
                               RavlN::SmartPtrC<GeneC> &newValue
                               ) const
   {
     SmartPtrC<const GeneTypeC> geneType;
     if(!palette.ProxyMap().Lookup(m_proxyName,geneType)) {
       RavlError("No proxy value for '%s' (%u available) ",m_proxyName.data(),(unsigned) palette.ProxyMap().Size());
       throw RavlN::ExceptionOperationFailedC("No value for proxy");
     }
     RavlAssert(geneType.IsValid());
     geneType->Mutate(palette,fraction,mustChange,original,newValue);
     return true;
   }

   static XMLFactoryRegisterConvertC<GeneTypeProxyC,GeneTypeC> g_registerGeneTypeProxy("RavlN::GeneticN::GeneTypeProxyC");
   RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeProxyC,"RavlN::GeneticN::GeneTypeProxyC");
   static RavlN::TypeNameC g_typePtrGeneTypeProxy(typeid(GeneTypeProxyC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeProxyC>");

   void LinkGeneTypeProxy()
   {}

}}
