// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENEFACTORY_HH
#define RAVL_GENETIC_GENEFACTORY_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/STL.hh"
#include "Ravl/SmartPtr.hh"
#include "Ravl/BStack.hh"
#include "Ravl/Hash.hh"
#include "Ravl/DP/TypeConverter.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Genetic/GeneType.hh"
#include "Ravl/Genetic/GenePalette.hh"
#include "Ravl/Genetic/Genome.hh"

namespace RavlN { namespace GeneticN {

  //! Information used in instantiating an genome.

  class GenomeScaffoldC
   : public RavlN::RCBodyC
  {
  public:
    //! Construct from a genome.
    GenomeScaffoldC(const GenomeC &genome);

    //! Access genome associated with scaffold.
    const GenomeC &Genome() const
    { return *m_genome; }

    //! Test if genome is const.
    bool IsGenomeConst() const
    { return m_genome->IsConst(); }

    //! Are we allowed to update the gene ?
    bool AllowUpdate() const
    { return m_allowUpdate; }

    //! Lookup instance of class data.
    bool Lookup(const void *gene,RCAbstractC &data) const
    { return m_parts.Lookup(gene,data); }

    //! Insert into table
    bool Insert(const void *gene,const RCAbstractC &data)
    { return m_parts.Insert(gene,data); }

    //! Lookup instance of class data.
    bool LookupOverride(const GeneC &gene,GeneC::RefT &theGene) const
    { return m_overrides.Lookup(&gene,theGene); }

    //! Insert into table
    bool InsertOverride(const GeneC &gene,const GeneC &data)
    { return m_overrides.Insert(&gene,&data); }

    //! Handle to scaffold
    typedef RavlN::SmartPtrC<GenomeScaffoldC> RefT;
  protected:
    GenomeC::RefT m_genome;
    bool m_allowUpdate;
    RavlN::HashC<const void *,RCAbstractC> m_parts;
    RavlN::HashC<GeneC::RefT,GeneC::RefT> m_overrides;
  };

  //!  Default values for basic types
  const GeneTypeC &GeneType(IntT value);

  //!  Default values for basic types
  const GeneTypeC &GeneType(float value);

  //!  Default values for basic types
  const GeneTypeC &GeneType(bool value);

  //! Generate a gene type for
  const GeneTypeC &CreateGeneType(const std::type_info &ti);

  //!  Default values for basic types
  template<typename DataT>
  const GeneTypeC &GeneType(DataT &value)
  {
    static GeneTypeC::RefT gt = &CreateGeneType(typeid(DataT));
    return *gt;
  }


  //! Factory class.
  class GeneFactoryC
  {
  public:
    //! Default factory
    GeneFactoryC();

    //! First level constructor.
    GeneFactoryC(const GenomeC &genome,GenePaletteC &palette);

    //! First level constructor from just a gene type
    GeneFactoryC(const GeneTypeC &geneType,GenePaletteC &palette);

    //! First level constructor.
    GeneFactoryC(const GenomeC &genome);

    //! Push another level on the stack.
    GeneFactoryC(const GeneFactoryC &geneFactory,const GeneC &gene);

  protected:
    //! Get the component.
    void GetComponent(const std::string &name,GeneC::ConstRefT &component,const GeneTypeC &geneType) const;

  public:
    //! Access genome
    const GenomeC &Genome() const
    { return m_scaffold->Genome(); }

    //! Access palette
    GenePaletteC &GenePalette()
    { return *m_palette; }

    //! Access palette
    const GenePaletteC &GenePalette() const
    { return *m_palette; }

    //! Set parameter in palette
    template<typename DataT>
    void SetParameter(const StringC &name,const DataT &value)
    { m_palette->SetParameter(name,value);  }

    //! Get parameter from palette
    template<typename DataT>
    bool GetParameter(const StringC &name,DataT &value) const
    { return m_palette->GetParameter(name,value); }

    //! Get an integer.
    void Get(const std::string &name,bool &value,const GeneTypeC &geneType) const;

    //! Get an integer.
    void Get(const std::string &name,IntT &value,const GeneTypeC &geneType) const;

    //! Get an integer.
    void Get(const std::string &name,UIntT &value,const GeneTypeC &geneType) const;

    //! Get a real value.
    void Get(const std::string &name,float &value,const GeneTypeC &geneType) const;

    //! Get a real value.
    void Get(const std::string &name,double &value,const GeneTypeC &geneType) const;

    //! Get a sub class
    template<typename ValueT>
    void Get(const std::string &name,ValueT &value,const GeneTypeC &geneType) const {
      GeneC::ConstRefT theGene;
      GetComponent(name,theGene,geneType);
      RCWrapAbstractC handle;
      theGene->Generate(*this,handle);
      RavlAssert(handle.IsValid());
      if(!SystemTypeConverter().TypeConvert(handle,value)) {
        RavlError("Failed to convert generated type from %s to %s ",
            RavlN::TypeName(handle.DataType()),
            RavlN::TypeName(typeid(ValueT)));
        RavlAssert(0);
        throw RavlN::ExceptionOperationFailedC("Failed to instantiate genome. ");
      }
    }

    //! Get a sub class
    template<typename ValueT>
    void Get(const std::string &name,ValueT &value) const
    { Get(name,value,GeneType(value)); }

    //! Get the root object as an abstract handle
    void Get(RCWrapAbstractC &obj,const type_info &to) const;

    //! Get the root object
    template<typename ValueT>
    void Get(ValueT &value) const {
      RCWrapC<ValueT> handle;
      Get(handle,typeid(ValueT));
      value = handle.Data();
    }

    //! Lookup instance of class data.
    bool Lookup(const void *gene,RCAbstractC &data) const
    { return m_scaffold->Lookup(gene,data); }

    //! Insert into table
    bool Insert(const void *gene,const RCAbstractC &data) const
    { return m_scaffold->Insert(gene,data); }

    //! Lookup instance of class data.
    bool LookupOverride(const GeneC &gene,GeneC::RefT &theGene) const
    { return m_scaffold->LookupOverride(gene,theGene); }

    //! Insert into table
    bool InsertOverride(const GeneC &gene,const GeneC &data)
    { return m_scaffold->InsertOverride(gene,data); }

    //! Get attribute
    template<class DataT>
    bool GetTypeAttribute(const StringC &attrName,DataT &val) const {
      const RCAbstractC *attrVal = GetTypeAttribute(attrName);
      if(attrVal == 0) return false;
      FromRCAbstract(*attrVal,val);
      return true;
    }

    //! Get attribute, throw exception on failure.
    template<class DataT>
    void AlwaysGetTypeAttribute(const StringC &attrName,DataT &val) const {
      const RCAbstractC *attrVal = GetTypeAttribute(attrName);
      if(attrVal == 0) throw ExceptionOperationFailedC("No value");
      FromRCAbstract(*attrVal,val);
    }

    //! Check if gene is in stack already.
    bool CheckStackFor(const GeneC &gene) const;
  protected:
    //! Get type attribute
    const RavlN::RCAbstractC *GetTypeAttribute(const RavlN::StringC &str) const;

    mutable RavlN::BStackC<GeneC::RefT> m_path;
    mutable GenomeScaffoldC::RefT m_scaffold;
    mutable GenePaletteC::RefT m_palette;
  };

  std::ostream &operator<<(std::ostream &strm,const GeneFactoryC &factory);
  std::istream &operator>>(std::istream &strm,GeneFactoryC &factory);
  RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &strm,const GeneFactoryC &factory);
  RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm,GeneFactoryC &factory);
}}

#endif
