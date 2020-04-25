// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeMeta.hh"
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
   GeneTypeEnumC::GeneTypeEnumC(const XMLFactoryContextC &factory)
    : GeneTypeC(factory)
   {
     factory.UseComponentGroup("Values",m_values);
   }

   //! Constructor
   GeneTypeEnumC::GeneTypeEnumC(const std::string &name,const std::vector<GeneC::ConstRefT> &values)
     : GeneTypeC(name),
       m_values(values)
   {}

   //! Load form a binary stream
   GeneTypeEnumC::GeneTypeEnumC(BinIStreamC &strm)
    : GeneTypeC(strm)
   {
     ByteT version = 0;
     strm >> version;
     if(version != 1)
       throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneC");
     UInt32T size;
     strm >> size;
     for(UInt32T i = 0;i < size;i++) {
       GeneC::ConstRefT theGene;
       strm >> ObjIO(theGene);
       m_values.push_back(theGene);
     }
   }

   //! Load form a binary stream
   GeneTypeEnumC::GeneTypeEnumC(std::istream &strm)
    : GeneTypeC(strm)
   {
     RavlAssertMsg(0,"not implemented");
   }

   //! Make a copy of this class.
   RavlN::RCBodyVC &GeneTypeEnumC::Copy() const
   { return *new GeneTypeEnumC(*this); }

   //! Save to binary stream
   bool GeneTypeEnumC::Save(BinOStreamC &strm) const
   {
     if(!GeneTypeC::Save(strm))
       return false;
     ByteT version = 1;
     strm << version;
     UInt32T size = m_values.size();
     strm << size;
     for(unsigned i = 0;i < m_values.size();i++) {
       strm << ObjIO(m_values[i]);
     }

     return true;
   }

   //! Save to binary stream
   bool GeneTypeEnumC::Save(std::ostream &strm) const
   {
     RavlAssertMsg(0,"not implemented");
     return false;
   }

   //! Dump description in human readable form.
   void GeneTypeEnumC::Dump(std::ostream &strm,UIntT indent) const {
     GeneTypeC::Dump(strm,indent);
     strm << "\n";
     for(unsigned i = 0;i <  m_values.size();i++) {
       m_values[i]->Dump(strm,indent+1);
       strm << "\n";
     }
   }

   //! Create randomise value
   void GeneTypeEnumC::Random(GenePaletteC &palette,GeneC::RefT &newValue) const
   {
     if(m_values.size() == 0) {
       RavlError("No values to choose from in enumeration '%s' ",m_name.data());
       throw RavlN::ExceptionOperationFailedC("No values to choose from.");
     }
     RavlAssert(m_values.size() > 0);
     IntT n = palette.RandomUInt32() % m_values.size();
     newValue = m_values[n];
   }

   //! Mutate a gene
   bool GeneTypeEnumC::Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const
   {
     if(fraction < palette.Random1()) {
       return original.Mutate(palette,fraction,mustChange,newValue);
     }
     // FIXME:- Do some sanity check on the number of options ?
     int retryLimit = 1000;
     do {
       Random(palette,newValue);
     } while((newValue.BodyPtr() != &original) && mustChange && retryLimit-- > 0) ;
     if(retryLimit <= 0)
       RavlWarning("Retry limit exceeded trying to mutate enum.");
     return newValue.BodyPtr() != &original;
   }

   XMLFactoryRegisterConvertC<GeneTypeEnumC,GeneTypeC> g_registerGeneTypeEnum("RavlN::GeneticN::GeneTypeEnumC");
   RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeEnumC,"RavlN::GeneticN::GeneTypeEnumC");
   static RavlN::TypeNameC g_typePtrGeneTypeEnum(typeid(GeneTypeEnumC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeEnumC>");

   // ---------------------------------------------------------------------

   //! Factory constructor
   GeneTypeMetaC::GeneTypeMetaC(const XMLFactoryContextC &factory)
    : GeneTypeC(factory)
   {
     factory.UseComponentGroup("Types",m_types);
   }

   //! Constructor
   GeneTypeMetaC::GeneTypeMetaC(const std::string &name)
     : GeneTypeC(name)
   {}

   //! Constructor
   GeneTypeMetaC::GeneTypeMetaC(const std::string &name,
                                const std::vector<GeneTypeC::ConstRefT> &types)
     : GeneTypeC(name),
       m_types(types)
   {}

   //! Constructor
   GeneTypeMetaC::GeneTypeMetaC(const std::string &name,
                                const GeneTypeC &type1,
                                const GeneTypeC &type2)
    : GeneTypeC(name)
   {
     m_types.reserve(2);
     m_types.push_back(&type1);
     m_types.push_back(&type2);
   }

   //! Constructor
   GeneTypeMetaC::GeneTypeMetaC(const std::string &name,
                                const GeneTypeC &type1,
                                const GeneTypeC &type2,
                                const GeneTypeC &type3)
   : GeneTypeC(name)
   {
     m_types.reserve(3);
     m_types.push_back(&type1);
     m_types.push_back(&type2);
     m_types.push_back(&type3);
   }

   //! Load form a binary stream
   GeneTypeMetaC::GeneTypeMetaC(BinIStreamC &strm)
    : GeneTypeC(strm)
   {
     ByteT version = 0;
     strm >> version;
     if(version != 1)
       throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeMetaC");
     UInt32T size;
     strm >> size;
     for(UInt32T i = 0;i < size;i++) {
       GeneTypeC::ConstRefT theGene;
       strm >> ObjIO(theGene);
       m_types.push_back(theGene);
     }
   }

   //! Load form a binary stream
   GeneTypeMetaC::GeneTypeMetaC(std::istream &strm)
    : GeneTypeC(strm)
   {
     RavlAssertMsg(0,"not implemented");
   }

   //! Make a copy of this class.
   RavlN::RCBodyVC &GeneTypeMetaC::Copy() const
   { return *new GeneTypeMetaC(*this); }


   //! Save to binary stream
   bool GeneTypeMetaC::Save(BinOStreamC &strm) const
   {
     if(!GeneTypeC::Save(strm))
       return false;
     ByteT version = 1;
     strm << version;
     UInt32T size = m_types.size();
     strm << size;
     for(unsigned i = 0;i < m_types.size();i++) {
       strm << ObjIO(m_types[i]);
     }

     return true;
   }

   //! Save to binary stream
   bool GeneTypeMetaC::Save(std::ostream &strm) const
   {
     RavlAssertMsg(0,"not implemented");
     return false;
   }

   //! Dump description in human readable form.
   void GeneTypeMetaC::Dump(std::ostream &strm,UIntT indent) const {
     GeneTypeC::Dump(strm,indent);
     strm << "\n";
     for(unsigned i = 0;i < m_types.size();i++) {
       m_types[i]->Dump(strm,indent+1);
       strm << "\n";
     }
   }

   //! Add type to list
   void GeneTypeMetaC::AddType(const GeneTypeC &geneType,float weight)
   {
     m_types.push_back(&geneType);
   }

   //! Create randomise value
   void GeneTypeMetaC::Random(GenePaletteC &palette,GeneC::RefT &newValue) const
   {
     if(m_types.size() == 0) {
       RavlError("No values to choose from in enumeration '%s' ",m_name.data());
       throw RavlN::ExceptionOperationFailedC("No values to choose from.");
     }
     RavlAssert(m_types.size() > 0);
     unsigned n = palette.RandomUInt32() % m_types.size();
     ONDEBUG(RavlDebug("Choosing %d of %zu '%s' ",n,m_types.size(),m_types[n]->Name().data()));
     m_types[n]->Random(palette,newValue);
   }

   //! Mutate a gene
   bool GeneTypeMetaC::Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const
   {
     if(fraction < palette.Random1() && !mustChange) {
       return original.Mutate(palette,fraction,mustChange,newValue);
     }
     // FIXME:- Do some sanity check on the number of options ?
     Random(palette,newValue);
     return true;
   }

   static XMLFactoryRegisterConvertC<GeneTypeMetaC,GeneTypeC> g_registerGeneTypeMeta("RavlN::GeneticN::GeneTypeMetaC");
   RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeMetaC,"RavlN::GeneticN::GeneTypeMetaC");
   static RavlN::TypeNameC g_typePtrGeneTypeMeta(typeid(GeneTypeMetaC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeMetaC>");

   void LinkGenomeMeta()
   {}

}}
