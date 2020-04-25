// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENETYPEBOOL_HH
#define RAVL_GENETIC_GENETYPEBOOL_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneType.hh"

namespace RavlN { namespace GeneticN {

  //! Type information for bool's

  class GeneTypeBoolC
   : public GeneTypeC
  {
  public:
    //! Factory constructor
    GeneTypeBoolC(const XMLFactoryContextC &factory);

    //! Constructor
    GeneTypeBoolC(const std::string &name,float bias = 0.5);

    //! Load form a binary stream
    GeneTypeBoolC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeBoolC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Create randomise value
    virtual void Random(GenePaletteC &palette,GeneC::RefT &newValue) const;

    //! Mutate a gene
    virtual bool Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const;

    //! Access bias.
    //! 1.0 = Always true, 0.0 = Always false
    float Bias() const
    { return m_bias; }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeBoolC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeBoolC> ConstRefT;
  protected:
    float m_bias;  // True/False bias,  1.0 = Always true, 0.0 = Always false
  };

  //! Access a gene type with 50/50 bias
  const GeneTypeBoolC &GeneTypeBoolBiasHalf();

  //! Gene for integer variable
  class GeneBoolC
   : public GeneC
  {
  public:
    //! Factory constructor
    GeneBoolC(const XMLFactoryContextC &factory);

    //! Constructor
    GeneBoolC(const GeneTypeC &geneType,IntT value);

    //! Load form a binary stream
    GeneBoolC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneBoolC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Save to binary stream
    virtual bool Save(RavlN::XMLOStreamC &strm) const;

    //! Access value
    bool Value() const
    { return m_value; }

    //! Generate an instance of the class.
    virtual void Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const;

    //! Access hash value.
    virtual size_t Hash() const
    { return RavlN::StdHash(m_value); }

    //! Test is value is effectively equal to this within tolerances specified in the type.
    virtual bool IsEffectivelyEqual(const GeneC &other) const;

    //! Dump description in human readable form.
    virtual void Dump(std::ostream &strm,UIntT indent = 0) const;

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneBoolC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneBoolC> ConstRefT;
  protected:
    bool m_value;
  };

}}
#endif
