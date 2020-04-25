// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneType.hh"
#include "Ravl/Genetic/Genome.hh"
#include "Ravl/Genetic/GenomeConst.hh"
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

  //! Virtual destructor.
  GeneVisitorC::~GeneVisitorC()
  {}

  bool GeneVisitorC::Examine(const GeneC &gene)
  {
    return true;
  }

  // ------------------------------------------------

  //! Factory constructor
  GeneTypeC::GeneTypeC(const XMLFactoryContextC &factory)
   : m_name(factory.AttributeString("name",factory.Path().data()).data()),
     m_defaultWeight(static_cast<float>(factory.AttributeReal("weight",1.0)))
  {}

  //! Constructor

  GeneTypeC::GeneTypeC(const std::string &name)
   : m_name(name),
     m_defaultWeight(1.0)
  {}

  //! Load form a binary stream
  GeneTypeC::GeneTypeC(BinIStreamC &strm)
   : RCBodyVC(strm),
     m_defaultWeight(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeC");
    strm >> m_name >> m_defaultWeight;
  }

  //! Load form a binary stream
  GeneTypeC::GeneTypeC(std::istream &strm)
   : m_defaultWeight(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeC::Save(BinOStreamC &strm) const
  {
    if(!RavlN::RCBodyVC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << m_name << m_defaultWeight;
    ONDEBUG(RavlDebug("Loading GeneType '%s' weight=%f ",m_name.data(),m_defaultWeight));
    return true;
  }

  //! Save to binary stream
  bool GeneTypeC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Dump description in human readable form.
  void GeneTypeC::Dump(std::ostream &strm,UIntT indent) const
  {
    strm << Indent(indent) << "Gene:" << Name() << " Weight:" << m_defaultWeight << " Type:" << RavlN::TypeName(typeid(*this));
  }

  //! Add a new entry to the gene
  void GeneTypeC::AddComponent(const std::string &name,const GeneTypeC &geneType)
  {
    RavlAssert(0);
  }

  //! Lookup component
  bool GeneTypeC::LookupComponent(const std::string &name,GeneTypeC::ConstRefT &geneType) const
  {
    RavlAssert(0);
    return false;
  }

  //! Create random instance

  void GeneTypeC::Random(GenePaletteC &palette,SmartPtrC<GeneC> &newValue) const
  {
    RavlAssert(0);
  }

  //! Mutate a gene

  bool GeneTypeC::Mutate(GenePaletteC &palette,float fraction,bool mustChange,const GeneC &original,RavlN::SmartPtrC<GeneC> &newValue) const
  {
    RavlAssert(0);
    return false;
  }

  //! Mutate a gene
  void GeneTypeC::Cross(GenePaletteC &palette,const GeneC &original1,const GeneC &original2,RavlN::SmartPtrC<GeneC> &newValue) const {
    if(palette.RandomDouble() > 0.5)
      newValue = &original1;
    else
      newValue = &original2;
  }

  static RavlN::TypeNameC g_typePtrGeneType(typeid(GeneTypeC),"RavlN::GeneticN::GeneTypeC");
  static RavlN::TypeNameC g_typePtrGeneTypeRef(typeid(GeneTypeC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeC>");

  // ------------------------------------------------------------------

  //! Factory constructor
  GeneC::GeneC(const XMLFactoryContextC &factory)
  {
    rThrowBadConfigContextOnFailS(factory,UseChildComponent("Type",m_type),"No type provided");
  }

  //! Constructor
  GeneC::GeneC(const GeneTypeC &theType)
   : m_type(&theType)
  {}

  //! Load form a binary stream
  GeneC::GeneC(BinIStreamC &strm)
   : RCBodyVC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneC");
    strm >> ObjIO(m_type);
    ONDEBUG(RavlDebug("Loading Gene, Type='%s' ",m_type->Name().data()));
  }

  //! Load form a binary stream
  GeneC::GeneC(std::istream &strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneC::Save(BinOStreamC &strm) const
  {
    if(!RavlN::RCBodyVC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version << ObjIO(m_type);
    return true;
  }


  //! Save to binary stream
  bool GeneC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Save to binary stream
  bool GeneC::Save(RavlN::XMLOStreamC &strm) const
  {
    strm << RavlN::XMLStartTag("Gene") << XMLAttribute("typename",RavlN::TypeName(typeid(*this)))  << XMLAttribute("name",Name().data());
    return true;
  }


  //! Dump description in human readable form.
  void GeneC::Dump(std::ostream &strm,UIntT indent) const {
    strm << Indent(indent) << "Type=" << RavlN::TypeName(typeid(*this)) << " Name=" << Name().data();
  }

  //! Lookup value
  bool GeneC::Lookup(const std::string &name,GeneC::ConstRefT &component) const
  {
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Base lookup called in %s  ",Name().data()));
    return false;
  }

  //! Add a new entry to the gene
  void GeneC::AddComponent(const std::string &name,const GeneC &newEntry,const GeneTypeC &geneType)
  {
    RavlAssert(0);
  }

  //! Mutate this gene
  bool GeneC::Mutate(GenePaletteC &palette,
                     float fraction,
                     bool mustChange,
                     GeneC::RefT &newOne) const
  {
    return m_type->Mutate(palette,fraction,mustChange,*this,newOne);
  }

  //! Cross this gene
  void GeneC::Cross(GenePaletteC &palette,const GeneC &other,GeneC::RefT &newOne) const {
    m_type->Cross(palette,*this,other,newOne);
  }

  //! Generate an instance of the class.
  void GeneC::Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const
  {
    RavlSysLogf(SYSLOG_ERR,"Default generate called for %s ",RavlN::TypeName(typeid(*this)));
    RavlAssertMsg(0,"not implemented");
  }

  //! Visit all gene's in tree.
  void GeneC::Visit(GeneVisitorC &visit) const {
    visit.Examine(*this);
  }

  //! Test is value is effectively equal to this.
  bool GeneC::IsEffectivelyEqual(const GeneC &other) const
  {
    RavlAssertMsg(0,"Abstract method called.");
    return other.Type() == Type();
  }

  //! Generate a hash value for the gene
  size_t GeneC::Hash() const {
    return RavlN::StdHash((void *) m_type.BodyPtr());
  }

  //! This actually tests if they are effectively equal which allows for some small
  //! user defined differences in some floating point numbers. This allows hash tables
  // to be created of similar genes.
  bool operator==(const GeneC::RefT &g1,const GeneC::RefT &g2) {
    if(g1.BodyPtr() == g2.BodyPtr())
      return true;
    if(!g1.IsValid() || !g2.IsValid())
      return false;
    return g1->IsEffectivelyEqual(*g2);
  }

  static RavlN::TypeNameC g_typeGene(typeid(GeneC),"RavlN::GeneticN::GeneC");
  static RavlN::TypeNameC g_typePtrGene(typeid(GeneC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneC>");
  static RavlN::TypeNameC g_typeConstPtrGene(typeid(GeneC::ConstRefT),"RavlN::SmartPtrC<const RavlN::GeneticN::GeneC>");

  static GeneC::ConstRefT ConvertGeneRefToConst(const GeneC::RefT &value)
  { return value.BodyPtr(); }

  DP_REGISTER_CONVERSION(ConvertGeneRefToConst,1.0);


  // ---------------------------------------------------------------------

}}

