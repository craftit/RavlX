// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENOMEMETA_HH
#define RAVL_GENETIC_GENOMEMETA_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneType.hh"

namespace RavlN { namespace GeneticN {

  //! Gene type which is an enumeration of its possible values.

  class GeneTypeEnumC
   : public GeneTypeC
  {
  public:
    //! Factory constructor
    GeneTypeEnumC(const XMLFactoryContextC &factory);

    //! Constructor
    GeneTypeEnumC(const std::string &name,const std::vector<GeneC::ConstRefT> &values);

    //! Load form a binary stream
    GeneTypeEnumC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeEnumC(std::istream &strm);

    //! Make a copy of this class.
    virtual RavlN::RCBodyVC &Copy() const;

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

    //! Access values in enumeration.
    const std::vector<GeneC::ConstRefT> &Values() const
    { return m_values; }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeEnumC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeEnumC> ConstRefT;

  protected:
    std::vector<GeneC::ConstRefT> m_values;
  };

   //! Gene type which is an enumeration of its possible types.
   //! Where each is given an equal chance of occurring.

   class GeneTypeMetaC
    : public GeneTypeC
   {
   public:
     //! Factory constructor
     GeneTypeMetaC(const XMLFactoryContextC &factory);

     //! Constructor
     GeneTypeMetaC(const std::string &name);

     //! Constructor
     GeneTypeMetaC(const std::string &name,const std::vector<GeneTypeC::ConstRefT> &types);

     //! Constructor
     GeneTypeMetaC(const std::string &name,const GeneTypeC &type1,const GeneTypeC &type2);

     //! Constructor
     GeneTypeMetaC(const std::string &name,const GeneTypeC &type1,const GeneTypeC &type2,const GeneTypeC &type3);

     //! Load form a binary stream
     GeneTypeMetaC(BinIStreamC &strm);

     //! Load form a binary stream
     GeneTypeMetaC(std::istream &strm);

     //! Make a copy of this class.
     virtual RavlN::RCBodyVC &Copy() const;

     //! Save to binary stream
     virtual bool Save(BinOStreamC &strm) const;

     //! Save to binary stream
     virtual bool Save(std::ostream &strm) const;

     //! Dump description in human readable form.
     virtual void Dump(std::ostream &strm,UIntT indent) const;

     //! Create randomise value
     virtual void Random(GenePaletteC &palette,GeneC::RefT &newValue) const;

     //! Mutate a gene
     virtual bool Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const;

     //! Access list of types
     const std::vector<GeneTypeC::ConstRefT> &Types() const
     { return m_types; }

     //! Add type to list
     //! Setting the weight to a negative number causes the default to be used.
     virtual void AddType(const GeneTypeC &geneType,float weight = -1.0);

     // Reference to this gene.
     typedef RavlN::SmartPtrC<GeneTypeMetaC> RefT;

     // Const reference to this gene.
     typedef RavlN::SmartPtrC<const GeneTypeMetaC> ConstRefT;

   protected:
     std::vector<GeneTypeC::ConstRefT> m_types;
   };


}}

#endif
