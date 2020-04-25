// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENOMECLASS_HH
#define RAVL_GENETIC_GENOMECLASS_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/STL.hh"
#include "Ravl/Genetic/GeneType.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/Collection.hh"
#include "Ravl/RCWrap.hh"

namespace RavlN { namespace GeneticN {

  class GeneClassC;

  //! Base class for nodes with child fields.
  class GeneTypeNodeC
   : public GeneTypeC
  {
  public:
    //! Factory constructor
    GeneTypeNodeC(const XMLFactoryContextC &factory);

    // Constructor
    GeneTypeNodeC(const std::string &name);

    //! Load form a binary stream
    GeneTypeNodeC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeNodeC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Add a new entry to the gene
    virtual void AddComponent(const std::string &name,const GeneTypeC &geneType);

    //! Lookup component
    virtual bool LookupComponent(const std::string &name,GeneTypeC::ConstRefT &geneType) const;

    //! Create randomise value
    virtual void Random(GenePaletteC &palette,GeneC::RefT &newValue) const;

    //! Mutate a gene
    virtual bool Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const;

    //! Mutate a gene
    virtual void Cross(GenePaletteC &palette,const GeneC &original1,const GeneC &original2,RavlN::SmartPtrC<GeneC> &newValue) const;

    //! List names of fields.
    void ListFields(RavlN::CollectionC<Tuple2C<StringC,GeneTypeC::ConstRefT> > &col) const;

    //! Set attribute against type.
    //! Attributes can be used to store constants that should be used
    //! when generating the class.
    template<class DataT>
    void SetAttribute(const StringC &attrName,const DataT &val)
    { m_attributes[attrName] = ToRCAbstract(val); }

    //! Get attribute
    template<class DataT>
    bool GetAttribute(const StringC &attrName,DataT &val) const {
      const RCAbstractC *attrVal = m_attributes.Lookup(attrName);
      if(attrVal == 0) return false;
      FromRCAbstract(*attrVal,val);
      return true;
    }

    //! Get attribute, throw exception on failure.
    template<class DataT>
    void AlwaysGetAttribute(const StringC &attrName,DataT &val) const {
      const RCAbstractC *attrVal = m_attributes.Lookup(attrName);
      if(attrVal == 0) throw ExceptionOperationFailedC("No value");
      FromRCAbstract(*attrVal,val);
    }

    //! Get raw abstract value.
    const RCAbstractC *GetAttribute(const StringC &attrName) const
    { return  m_attributes.Lookup(attrName); }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeNodeC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeNodeC> ConstRefT;

  protected:
    RavlN::HashC<std::string,GeneTypeC::ConstRefT> m_componentTypes;
    RavlN::HashC<StringC,RCAbstractC> m_attributes;
  };

  //! Node containing sub gene's

  class GeneNodeC
   : public GeneC
  {
  public:
    //! Construct from a geneType.
    GeneNodeC(const GeneTypeNodeC &geneType);

    //! Factory constructor
    GeneNodeC(const XMLFactoryContextC &factory);

    //! Load form a binary stream
    GeneNodeC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneNodeC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Save to binary stream
    virtual bool Save(RavlN::XMLOStreamC &strm) const;

    //! Lookup a component.
    virtual bool Lookup(const std::string &name, GeneC::ConstRefT &component) const;

    //! Add a new entry to the gene
    virtual void AddComponent(const std::string &name,const GeneC &newEntry,const GeneTypeC &geneType);

    //! Get component.
    bool GetComponent(const std::string &name,GeneC::ConstRefT &gene) const;

    //! Set component
    void SetComponent(const std::string &name,const GeneC &newGene);

    //! Visit all gene's in tree.
    virtual void Visit(GeneVisitorC &visitor) const;

    //! Generate a hash value for the gene
    virtual size_t Hash() const;

    //! Test is value is effectively equal to this within tolerances specified in the type.
    virtual bool IsEffectivelyEqual(const GeneC &other) const;

    //! Dump description in human readable form.
    virtual void Dump(std::ostream &strm,UIntT indent = 0) const;

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneNodeC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneNodeC> ConstRefT;
  protected:
    RavlN::HashC<std::string,GeneC::ConstRefT> m_components;
  };

  //! Gene type for a class.

  class GeneTypeClassC
  : public GeneTypeNodeC
  {
  public:
    //! Factory constructor
    GeneTypeClassC(const XMLFactoryContextC &factory,bool checkType = true);

    //! Constructor
    GeneTypeClassC(const std::type_info &classType);

    //! Load form a binary stream
    GeneTypeClassC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeClassC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Dump description in human readable form.
    virtual void Dump(std::ostream &strm,UIntT indent = 0) const;

    //! Create randomise value
    virtual void Random(GenePaletteC &palette,GeneC::RefT &newValue) const;

    //! Mutate a gene
    virtual bool Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const;

    //! Mutate a gene
    virtual void Cross(GenePaletteC &palette,const GeneC &original1,const GeneC &original2,RavlN::SmartPtrC<GeneC> &newValue) const;

    //! Access type of class generated.
    const std::type_info &TypeInfo() const
    { return *m_typeInfo; }

    //! Name of class to be generated
    const std::string &TypeName() const
    { return m_typeName; }

    //! Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeClassC > RefT;

    //! Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeClassC > ConstRefT;

  protected:
    //! Method for generating the class from a GeneFactoryC which has a
    //! current context of a 'GeneClassC'
    virtual void Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const;

    std::string m_typeName;
    const std::type_info *m_typeInfo;
    friend class GeneClassC;
  };


  //! Gene representing an instance of a class.

  class GeneClassC
   : public GeneNodeC
  {
  public:
    //! Factory constructor
    GeneClassC(const XMLFactoryContextC &factory);

    //! Constructor.
    GeneClassC(const GeneTypeClassC &geneType);

    //! Load form a binary stream
    GeneClassC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneClassC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Save to binary stream
    virtual bool Save(RavlN::XMLOStreamC &strm) const;

    //! Generate an instance of the class.
    virtual void Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const;

    //! Access class type.
    const GeneTypeClassC &ClassType() const
    { return dynamic_cast<const GeneTypeClassC &>(*m_type); }

    //! Generate a hash value for the gene
    virtual size_t Hash() const
    { return GeneNodeC::Hash(); }

    //! Test is value is effectively equal to this within tolerances specified in the type.
    virtual bool IsEffectivelyEqual(const GeneC &other) const
    { return GeneNodeC::IsEffectivelyEqual(other); }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneClassC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneClassC> ConstRefT;
  protected:
  };


  //! Helper for registering a class which has a constructor from (const GeneFactoryC &factory)

  template<typename ClassT>
  class RegisterGeneClassC
  {
  public:
    static typename ClassT::RefT ConvertGeneFactory2Inst(const GeneFactoryC &factory)
    { return new ClassT(factory); }

    RegisterGeneClassC(const char *nameOfType)
    {
      RavlN::AddTypeName(typeid(ClassT),nameOfType);
      m_refName = std::string(nameOfType) + "::RefT";
      RavlN::AddTypeName(typeid(typename ClassT::RefT),m_refName.data());
      RavlN::RegisterConversion(&ConvertGeneFactory2Inst);
    }

  protected:
    std::string m_refName;
  };


}}

#endif
