// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneTypeWeightedMeta.hh"
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
   // ---------------------------------------------------------------------

   //! Factory constructor
   GeneTypeWeightedMetaC::GeneTypeWeightedMetaC(const XMLFactoryContextC &factory)
    : GeneTypeMetaC(factory),
      m_totalWeights(0)
   {
   }

   //! Constructor
   GeneTypeWeightedMetaC::GeneTypeWeightedMetaC(const std::string &name,
                                                const std::vector<GeneTypeC::ConstRefT> &types)
   : GeneTypeMetaC(name,types),
     m_totalWeights(0)
   {
     m_weights.reserve(types.size());
     for(unsigned i = 0;i < types.size();i++)
       m_weights.push_back(types[i]->DefaultWeight());
     Remap();
   }

   //! Constructor
   GeneTypeWeightedMetaC::GeneTypeWeightedMetaC(const std::string &name,
                                                const std::vector<GeneTypeC::ConstRefT> &types,
                                                const std::vector<float> &weights)
     : GeneTypeMetaC(name,types),
       m_weights(weights),
       m_totalWeights(0)
   {
     Remap();
   }

   //! Constructor
   GeneTypeWeightedMetaC::GeneTypeWeightedMetaC(const std::string &name,
                                               const GeneTypeC &type1,float weight1,
                                               const GeneTypeC &type2,float weight2)

     : GeneTypeMetaC(name,type1,type2),
       m_totalWeights(0)
   {
     m_weights.reserve(2);
     m_weights.push_back(weight1);
     m_weights.push_back(weight2);
     Remap();
   }

   //! Constructor
   GeneTypeWeightedMetaC::GeneTypeWeightedMetaC(const std::string &name,
                                               const GeneTypeC &type1,float weight1,
                                               const GeneTypeC &type2,float weight2,
                                               const GeneTypeC &type3,float weight3)
    : GeneTypeMetaC(name,type1,type2,type3),
      m_totalWeights(0)
   {
     m_weights.reserve(3);
     m_weights.push_back(weight1);
     m_weights.push_back(weight2);
     m_weights.push_back(weight3);
     Remap();
   }



   //! Constructor
   GeneTypeWeightedMetaC::GeneTypeWeightedMetaC(const std::string &name)
     : GeneTypeMetaC(name),
       m_totalWeights(0)
   {}

   //! Load form a binary stream
   GeneTypeWeightedMetaC::GeneTypeWeightedMetaC(BinIStreamC &strm)
    : GeneTypeMetaC(strm),
      m_totalWeights(0)
   {
     ByteT version = 0;
     strm >> version;
     if(version != 1)
       throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeWeightedMetaC");
     strm >> m_weights;
     Remap();
   }

   //! Load form a binary stream
   GeneTypeWeightedMetaC::GeneTypeWeightedMetaC(std::istream &strm)
    : GeneTypeMetaC(strm)
   {
     RavlAssertMsg(0,"not implemented");
   }

   //! Save to binary stream
   bool GeneTypeWeightedMetaC::Save(BinOStreamC &strm) const
   {
     if(!GeneTypeMetaC::Save(strm))
       return false;
     ByteT version = 1;
     strm << version;
     strm << m_weights;
     return true;
   }

   //! Save to binary stream
   bool GeneTypeWeightedMetaC::Save(std::ostream &strm) const
   {
     RavlAssertMsg(0,"not implemented");
     return false;
   }


   //! Create randomise value
   void GeneTypeWeightedMetaC::Random(GenePaletteC &palette,GeneC::RefT &newValue) const
   {
     if(m_types.size() == 0) {
       RavlSysLogf(SYSLOG_ERR,"No values to choose from in enumeration '%s' ",m_name.data());
       throw RavlN::ExceptionOperationFailedC("No values to choose from.");
     }
     float choice = static_cast<float>(palette.Random1() * m_totalWeights);
     //RavlSysLogf(SYSLOG_DEBUG," %f from %f ",choice,m_totalWeights);
     std::map<float,GeneTypeC::ConstRefT>::const_iterator iter = m_typeMap.upper_bound(choice);
     --iter;
     RavlAssert(iter != m_typeMap.end());

     ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Choosing %f of %f '%s' ",choice,m_totalWeights,iter->second->Name().data()));

     iter->second->Random(palette,newValue);
   }

   //! Generate map again
   void GeneTypeWeightedMetaC::Remap()
   {
     m_totalWeights = 0;
     RavlAssert(m_types.size() == m_weights.size());
     for(unsigned i = 0; i < m_types.size();i++) {
       m_typeMap.insert(EntryT(m_totalWeights,m_types[i]));
       m_totalWeights += m_weights[i];;
     }
   }

   //! Add type to list
   void GeneTypeWeightedMetaC::AddType(const GeneTypeC &geneType,float weight)
   {
     if(weight < 0)
       weight = geneType.DefaultWeight();
     m_weights.push_back(weight);
     m_types.push_back(&geneType);
     m_typeMap.insert(EntryT(m_totalWeights,&geneType));
     m_totalWeights += weight;
   }

   XMLFactoryRegisterConvertC<GeneTypeWeightedMetaC,GeneTypeC> g_registerGeneTypeWeightedMeta("RavlN::GeneticN::GeneTypeWeightedMetaC");
   RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeWeightedMetaC,"RavlN::GeneticN::GeneTypeWeightedMetaC");
   static RavlN::TypeNameC g_typePtrGeneTypeWeightedMeta(typeid(GeneTypeWeightedMetaC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeWeightedMetaC>");

   void LinkGenomeWeightedMeta()
   {}

}}
