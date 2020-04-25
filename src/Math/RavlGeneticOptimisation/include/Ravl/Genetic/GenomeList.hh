// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENOMEList_HH
#define RAVL_GENETIC_GENOMEList_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneType.hh"
#include "Ravl/Collection.hh"
#include "Ravl/DP/TypeConverter.hh"

namespace RavlN { namespace GeneticN {

  //! Base class for Lists with child fields.

  class GeneTypeListBaseC
   : public GeneTypeC
  {
  public:
    //! Factory constructor
    GeneTypeListBaseC(const XMLFactoryContextC &factory);

    // Constructor
    GeneTypeListBaseC(const std::string &name,const GeneTypeC &contentType,UIntT maxSize);

    //! Load form a binary stream
    GeneTypeListBaseC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeListBaseC(std::istream &strm);

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

    //! Create a new list
    virtual bool CreateList(RCWrapAbstractC &list) const;

    //! Add item to list.
    virtual bool AddToList(RCWrapAbstractC &list,RCWrapAbstractC &item) const;

    //! Access maximum list length.
    UIntT MaxLength() const
    { return m_maxLength; }

    //! Access content type of list.
    const GeneTypeC &ContentType() const
    { return *m_contentType; }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeListBaseC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeListBaseC> ConstRefT;

  protected:
    GeneTypeC::ConstRefT m_contentType;
    UIntT m_maxLength;
  };

  //! Gene type for a list

  template<typename EntryT>
  class GeneTypeListC
   : public GeneTypeListBaseC
  {
  public:
    //! Factory constructor
    GeneTypeListC(const XMLFactoryContextC &factory)
     : GeneTypeListBaseC(factory)
    {}

    // Constructor
    GeneTypeListC(const std::string &name,const GeneTypeC &contentType,UIntT maxSize)
     : GeneTypeListBaseC(name,contentType,maxSize)
    {}

    //! Load form a binary stream
    GeneTypeListC(BinIStreamC &strm)
     : GeneTypeListBaseC(strm)
    {
      ByteT version = 0;
      strm >> version;
      if(version != 1)
        throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeListC");
    }

     //! Load form a binary stream
    GeneTypeListC(std::istream &strm)
      : GeneTypeListBaseC(strm)
    {}

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const
    {
      GeneTypeListBaseC::Save(strm);
      ByteT version = 1;
      strm << version;
      return true;
    }

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const
    {
      GeneTypeListBaseC::Save(strm);
      return false;
    }

    //! Create a new list
    virtual bool CreateList(RCWrapAbstractC &list) const {
      CollectionC<EntryT> newCollection(32);
      list = RCWrapC<CollectionC<EntryT> >(newCollection);
      return true;
    }

    //! Add item to list.
    virtual bool AddToList(RCWrapAbstractC &list,RCWrapAbstractC &item) const {
      RCWrapC<CollectionC<EntryT> > lw(list,true);
      RavlAssert(lw.IsValid());
      EntryT value;
      if(!SystemTypeConverter().TypeConvert(item,value)) {
        RavlAssertMsg(0,"Item not compatible with collection. ");
        return false;
      }
      lw.Data().Append(value);
      return true;
    }

    // Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeListC<EntryT> > RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeListC<EntryT> > ConstRefT;
  protected:
  };


  //! List containing sub gene's of a particular type.

  class GeneListC
   : public GeneC
  {
  public:
    //! Construct from a geneType.
    GeneListC(const GeneTypeListBaseC &geneType,const std::vector<GeneC::ConstRefT> &aList);

    //! Factory constructor
    GeneListC(const XMLFactoryContextC &factory);

    //! Load form a binary stream
    GeneListC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneListC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Save to binary stream
    virtual bool Save(RavlN::XMLOStreamC &strm) const;

    //! Access the list.
    const std::vector<GeneC::ConstRefT> &List() const
    { return m_list; }

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
    typedef RavlN::SmartPtrC<GeneListC> RefT;

    // Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneListC> ConstRefT;
  protected:
    std::vector<GeneC::ConstRefT> m_list;
  };


}}
#endif
