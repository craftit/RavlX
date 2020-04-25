// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENETYPEARRAY_HH
#define RAVL_GENETIC_GENETYPEARRAY_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneType.hh"
#include "Ravl/Collection.hh"
#include "Ravl/DP/TypeConverter.hh"

namespace RavlN { namespace GeneticN {

  //! Base class for fixed length arrays

  class GeneTypeArrayBaseC
   : public GeneTypeC
  {
  public:
    //! Factory constructor
    GeneTypeArrayBaseC(const XMLFactoryContextC &factory);

    // Constructor
    GeneTypeArrayBaseC(const std::string &name,const GeneTypeC &contentType,size_t length);

    //! Load form a binary stream
    GeneTypeArrayBaseC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeArrayBaseC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Create randomise value
    virtual void Random(GenePaletteC &palette,GeneC::RefT &newValue) const;

    //! Mutate a gene
    virtual bool Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const;

    //! Mutate a gene
    virtual void Cross(GenePaletteC &palette,const GeneC &original1,const GeneC &original2,RavlN::SmartPtrC<GeneC> &newValue) const;

    //! Create a new Array
    virtual bool CreateArray(const GeneFactoryC &context,
                             RCWrapAbstractC &array,
                             const SArray1dC<GeneC::ConstRefT> &geneArray) const;

    //! Access maximum Array length.
    size_t Length() const
    { return m_length; }

    //! Access content type of Array.
    const GeneTypeC &ContentType() const
    { return *m_contentType; }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeArrayBaseC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeArrayBaseC> ConstRefT;

  protected:
    GeneTypeC::ConstRefT m_contentType;
    size_t m_length;
  };

  //! Gene type for fixed length array

  template<typename EntryT>
  class GeneTypeArrayC
   : public GeneTypeArrayBaseC
  {
  public:
    //! Factory constructor
    GeneTypeArrayC(const XMLFactoryContextC &factory)
     : GeneTypeArrayBaseC(factory)
    {}

    // Constructor
    GeneTypeArrayC(const std::string &name,const GeneTypeC &contentType,UIntT length)
     : GeneTypeArrayBaseC(name,contentType,length)
    {}

    //! Load form a binary stream
    GeneTypeArrayC(BinIStreamC &strm)
     : GeneTypeArrayBaseC(strm)
    {
      ByteT version = 0;
      strm >> version;
      if(version != 1)
        throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeArrayC");
    }

     //! Load form a binary stream
    GeneTypeArrayC(std::istream &strm)
      : GeneTypeArrayBaseC(strm)
    {}

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const
    {
      GeneTypeArrayBaseC::Save(strm);
      ByteT version = 1;
      strm << version;
      return true;
    }

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const
    {
      GeneTypeArrayBaseC::Save(strm);
      return false;
    }

    //! Create a new Array
    virtual bool CreateArray(const GeneFactoryC &context,
                             RCWrapAbstractC &array,
                             const SArray1dC<GeneC::ConstRefT> &geneArray) const
    {
      SArray1dC<EntryT> newArray(m_length);
      RCWrapAbstractC comp;
      for(unsigned i = 0;i < m_length;i++) {
        geneArray[i]->Generate(context,comp);
        EntryT *value = 0;
        comp.GetPtr(value);
        newArray[i] = *value;
      }
      array = RCWrapC<SArray1dC<EntryT> >(newArray);
      return true;
    }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeArrayC<EntryT> > RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeArrayC<EntryT> > ConstRefT;
  protected:
  };


  //! Fixed length array containing sub gene's of a particular type.

  class GeneArrayC
   : public GeneC
  {
  public:
    //! Construct from a geneType.
    GeneArrayC(const GeneTypeArrayBaseC &geneType,const SArray1dC<GeneC::ConstRefT> &aArray);

    //! Factory constructor
    GeneArrayC(const XMLFactoryContextC &factory);

    //! Load form a binary stream
    GeneArrayC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneArrayC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Save to binary stream
    virtual bool Save(RavlN::XMLOStreamC &strm) const;

    //! Access the Array.
    const SArray1dC<GeneC::ConstRefT> &Array() const
    { return m_array; }

    //! Generate an instance of the class.
    virtual void Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const;

    //! Visit all gene's in tree.
    virtual void Visit(GeneVisitorC &visitor) const;

    //! Generate a hash value for the gene
    virtual size_t Hash() const;

    //! Test is value is effectively equal to this within tolerances specified in the type.
    virtual bool IsEffectivelyEqual(const GeneC &other) const;

    //! Dump description in human readable form.
    virtual void Dump(std::ostream &strm,UIntT indent = 0) const;

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneArrayC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneArrayC> ConstRefT;
  protected:
    SArray1dC<GeneC::ConstRefT> m_array;
  };

}}
#endif
