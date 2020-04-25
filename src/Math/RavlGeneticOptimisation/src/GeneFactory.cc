// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneFactory.hh"
#include "Ravl/Genetic/GenomeConst.hh"
#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/GeneTypeBool.hh"
#include "Ravl/Random.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/BListIter.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/DP/FileFormatBinStream.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {

  //! Construct from a genome.

  GenomeScaffoldC::GenomeScaffoldC(const GenomeC &genome)
   : m_genome(&genome),
     m_allowUpdate(true)
  {}


  // ----------------------------------------------------------------------

  //! Default factory
  GeneFactoryC::GeneFactoryC()
   : m_palette(new GenePaletteC())
  {}

  //! First level constructor.
  GeneFactoryC::GeneFactoryC(const GenomeC &genome)
   :  m_scaffold(new GenomeScaffoldC(genome)),
      m_palette(new GenePaletteC())
  {
    genome.UpdateShares(*this);
  }

  //! First level constructor.
  GeneFactoryC::GeneFactoryC(const GenomeC &genome,GenePaletteC &palette)
   :  m_scaffold(new GenomeScaffoldC(genome)),
      m_palette(&palette)
  {
    genome.UpdateShares(*this);
  }

  //! First level constructor from just a gene type
  GeneFactoryC::GeneFactoryC(const GeneTypeC &geneType,GenePaletteC &palette)
   : m_palette(&palette)
  {
    GeneC::RefT aGene;
    geneType.Random(*m_palette,aGene);
    GenomeC::RefT genome = new GenomeC(*aGene);
    m_scaffold = new GenomeScaffoldC(*genome);
    genome->UpdateShares(*this);
    m_path.Push(*aGene);
  }

  //! Push another level on the stack.
  GeneFactoryC::GeneFactoryC(const GeneFactoryC &geneFactory,const GeneC &gene)
   : m_path(geneFactory.m_path),
     m_scaffold(geneFactory.m_scaffold),
     m_palette(&geneFactory.GenePalette())
  {
    m_path.Push(&gene);
  }

  //! Get the component.
  void GeneFactoryC::GetComponent(const std::string &name,GeneC::ConstRefT &component,const GeneTypeC &geneType) const {
    if(m_path.First()->Lookup(name,component)) {
      ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Lookup of '%s' in current context '%s' succeeded .",name.data(),m_path.First()->Name().data()));
      return ;
    }
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Failed to lookup component '%s' in current context '%s' .",name.data(),m_path.First()->Name().data()));
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Node: %s ",RavlN::StringOf(m_path.First()).data()));
    {
      if(m_scaffold->IsGenomeConst()) {
        RavlSysLogf(SYSLOG_ERR,"Incomplete genome, can't instantiate.");
        throw RavlN::ExceptionOperationFailedC("Can't instantiate incomplete genome. ");
      }
      GeneC::RefT newComponent;
      geneType.Random(*m_palette,newComponent);
      m_path.First()->AddComponent(name,*newComponent,geneType);
      component = newComponent.BodyPtr();
    }
  }

  //! Get an integer.
  void GeneFactoryC::Get(const std::string &name,bool &value,const GeneTypeC &geneType) const {
    GeneC::ConstRefT component;
    GetComponent(name,component,geneType);
    const GeneBoolC &theGene = dynamic_cast<const GeneBoolC &>(*component);
    value = theGene.Value();
  }

  void GeneFactoryC::Get(const std::string &name,IntT &value,const GeneTypeC &geneType) const
  {
    GeneC::ConstRefT component;
    GetComponent(name,component,geneType);
    const GeneIntC &theGene = dynamic_cast<const GeneIntC &>(*component);
    value = theGene.Value();
  }

  //! Get an integer.
  void GeneFactoryC::Get(const std::string &name,UIntT &value,const GeneTypeC &geneType) const {
    GeneC::ConstRefT component;
    GetComponent(name,component,geneType);
    const GeneIntC &theGene = dynamic_cast<const GeneIntC &>(*component);
    if(theGene.Value() >= 0)
      value = theGene.Value();
    else
      value = 0;
  }


  void GeneFactoryC::Get(const std::string &name,float &value,const GeneTypeC &geneType) const
  {
    GeneC::ConstRefT component;
    GetComponent(name,component,geneType);
    const GeneFloatC &theGene = dynamic_cast<const GeneFloatC &>(*component);
    value = theGene.Value();
  }

  //! Get a real value.
  void GeneFactoryC::Get(const std::string &name,double &value,const GeneTypeC &geneType) const
  {
    GeneC::ConstRefT component;
    GetComponent(name,component,geneType);
    const GeneFloatC &theGene = dynamic_cast<const GeneFloatC &>(*component);
    value = theGene.Value();
  }


  //! Get the root object as an abstract handle
  void GeneFactoryC::Get(RCWrapAbstractC &obj,const type_info &to) const
  {
    RCWrapAbstractC handle;
    m_scaffold->Genome().RootGene().Generate(*this,handle);
    RavlAssert(handle.IsValid());
    obj = SystemTypeConverter().DoConversion(handle.Abstract(),handle.DataType(),to);
    if(!obj.IsValid()) {
      RavlSysLogf(SYSLOG_ERR,"Failed to convert generated type from %s to %s ",RavlN::TypeName(handle.DataType()),RavlN::TypeName(to));
      RavlAssert(0);
      throw RavlN::ExceptionOperationFailedC("Failed to instantiate genome. ");
    }
  }


  //! Check if gene is in stack already.
  bool GeneFactoryC::CheckStackFor(const GeneC &gene) const {
    for(BListIterC<GeneC::RefT> it(m_path);it;it++) {
      if(it.Data().BodyPtr() == &gene)
        return true;
    }
    return false;
  }

  //! Get type attribute
  const RavlN::RCAbstractC *GeneFactoryC::GetTypeAttribute(const RavlN::StringC &attrName) const {
    if(m_path.IsEmpty())
      return 0;
    const GeneTypeC &geneType = m_path.Top()->Type();
    const GeneTypeNodeC *geneTypeNode = dynamic_cast<const GeneTypeNodeC *>(&geneType);
    if(geneTypeNode == 0) {
      RavlWarning("Attribute asked for non-GeneTypeNodeC. '%s' ",geneType.Name().c_str());
      return 0;
    }
    return geneTypeNode->GetAttribute(attrName);
  }


  static RavlN::TypeNameC g_typeGeneFactory(typeid(GeneFactoryC),"RavlN::GeneticN::GeneFactoryC");

  std::ostream &operator<<(std::ostream &strm,const GeneFactoryC &factory) {
    strm << "GeneFactoryC.";
    return strm;
  }

  std::istream &operator>>(std::istream &strm,GeneFactoryC &factory)
  {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }

  RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &strm,const GeneFactoryC &factory) {
    RavlAssertMsg(0,"not implemented");
    return strm;
  }

  RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm,GeneFactoryC &factory){
    RavlAssertMsg(0,"not implemented");
    return strm;
  }

  // Generate a gene factory.

  static GeneFactoryC ConvertGenome2GeneFactory(const GenomeC::RefT &genome)
  { return GeneFactoryC(*genome); }

  //DP_REGISTER_CONVERSION_NAMED(ConvertGenome2GeneFactory,1.0,"ConvertGenome2GeneFactory");
  static RavlN::DPConverterBaseC DPConv_ConvertGenome2GeneFactory(RavlN::RegisterConversion(ConvertGenome2GeneFactory,1.0));


}}

