// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_GENETIC_GENETYPECLASSDIRECT_HH
#define RAVL_GENETIC_GENETYPECLASSDIRECT_HH 1
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/Calls.hh"

namespace RavlN { namespace GeneticN {

  //! Generate a class via a function pointer

  class GeneTypeClassDirectFuncC
  : public GeneTypeClassC
  {
  public:
    typedef void (GenerateFuncT)(const GeneFactoryC &context,RCWrapAbstractC &handle);

    //! Factory constructor
    GeneTypeClassDirectFuncC(const XMLFactoryContextC &factory);

    //! Constructor
    GeneTypeClassDirectFuncC(const std::type_info &classType,GenerateFuncT *generatorFunc);

    //! Load form a binary stream
    GeneTypeClassDirectFuncC(BinIStreamC &strm);

    //! Load form a binary stream
    GeneTypeClassDirectFuncC(std::istream &strm);

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const;

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const;

    //! Set function to use to generate
    void SetGenerator(GenerateFuncT *generator)
    { m_generator = generator; }

    //! Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeClassDirectFuncC > RefT;

    //! Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeClassDirectFuncC > ConstRefT;

  protected:
    //! Method for generating the class from a GeneFactoryC which has a
    //! current context of a 'GeneClassC'
    virtual void Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const;

    GenerateFuncT *m_generator; //!< Function for generating the class.
  };

  //! Generate a class via its constructor

  template<class ClassT>
  class GeneTypeClassDirectC
  : public GeneTypeClassC
  {
  public:
    //! Constructor
    GeneTypeClassDirectC()
     : GeneTypeClassC(typeid(ClassT))
    {}

    //! Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeClassDirectC<ClassT> > RefT;

    //! Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeClassDirectC<ClassT> > ConstRefT;

  protected:
    //! Method for generating the class from a GeneFactoryC which has a
    //! current context of a 'GeneClassC'
    virtual void Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const {
     handle = RCWrapC<ClassT>(ClassT(context));
    }
  };

  //! Generate a class via its constructor passing back a smart ptr

  template<class ClassT>
  class GeneTypeClassDirectSmartPtrC
  : public GeneTypeClassC
  {
  public:

    //! Method to convert to base class.
    //! Constructor
    GeneTypeClassDirectSmartPtrC(const char *nameOfType)
     : GeneTypeClassC(typeid(ClassT))
    {
      static bool doRegister = true;
      if(doRegister) {
        doRegister = false;
        RegisterType(nameOfType);
      }
    }

    //! Factory constructor
    GeneTypeClassDirectSmartPtrC(const XMLFactoryContextC &factory)
     : GeneTypeClassC(factory,false)
    {}

    //! Load form a binary stream
    GeneTypeClassDirectSmartPtrC(BinIStreamC &strm)
     : GeneTypeClassC(strm)
    {
      ByteT version = 0;
      strm >> version;
      if(version != 1)
        throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeClassDirectSmartPtrC");
    }

    //! Load form a binary stream
    GeneTypeClassDirectSmartPtrC(std::istream &strm)
     : GeneTypeClassC(strm)
    { RavlIssueError("not implemented"); }

    //! Save to binary stream
    virtual bool Save(BinOStreamC &strm) const {
      GeneTypeClassC::Save(strm);
      ByteT version = 1;
      strm << version;
      return true;
    }

    //! Save to binary stream
    virtual bool Save(std::ostream &strm) const
    { RavlIssueError("not implemented"); return false; }


    //! Reference to this gene.
    typedef RavlN::SmartPtrC<GeneTypeClassDirectSmartPtrC<ClassT> > RefT;

    //! Const reference to this gene.
    typedef RavlN::SmartPtrC<const GeneTypeClassDirectSmartPtrC<ClassT> > ConstRefT;

  protected:
    static typename GeneTypeClassC::RefT ConvertToBase(const typename GeneTypeClassDirectSmartPtrC<ClassT>::RefT &x)
    { return x.BodyPtr(); }

    static int RegisterType(const char *nameOfType) {
      RavlN::AddTypeName(typeid(ClassT),nameOfType);
      RavlDebug("Registering class %s ",nameOfType);
      static RavlN::StringC refName = StringC(nameOfType) + "::RefT";
      RavlN::AddTypeName(typeid(typename ClassT::RefT),refName.c_str());
      static RavlN::StringC geneTypeName = StringC("RavlN::GeneticN::GeneTypeClassDirectSmartPtrC<") + StringC(nameOfType) + ">";
      RavlN::AddTypeName(typeid(GeneTypeClassDirectSmartPtrC<ClassT>),geneTypeName.c_str());
      static RavlN::StringC geneTypeName2 = StringC("RavlN::GeneticN::GeneTypeClassDirectSmartPtrC<") + StringC(nameOfType) + ">::RefT";
      RavlN::AddTypeName(typeid(typename GeneTypeClassDirectSmartPtrC<ClassT>::RefT),geneTypeName2.c_str());

      RavlN::RegisterConversion(&ConvertToBase);
      XMLFactoryC::RegisterTypeFactory(typeid(GeneTypeClassDirectSmartPtrC<ClassT>),&XMLFactoryC::DefaultFactoryFunc<GeneTypeClassDirectSmartPtrC<ClassT> >);
      return 1;
    }

    //! Method for generating the class from a GeneFactoryC which has a
    //! current context of a 'GeneClassC'
    virtual void Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const {
      handle = RCWrapC<typename ClassT::RefT>(new ClassT(context));
    }

  };

}}
#endif
