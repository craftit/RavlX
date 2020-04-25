// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GeneTypeArray.hh"
#include "Ravl/Random.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/PointerManager.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/Genetic/GenePalette.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN { namespace GeneticN {

  // -----------------------------------------------------------------

  //! Factory constructor
  GeneTypeArrayBaseC::GeneTypeArrayBaseC(const XMLFactoryContextC &factory)
   : GeneTypeC(factory),
     m_length(factory.AttributeInt("maxLength",30))
  {
    XMLFactoryContextC childContext;
    factory.UseComponent("ContentType",m_contentType);
  }

  // Constructor
  GeneTypeArrayBaseC::GeneTypeArrayBaseC(const std::string &name,
                                         const GeneTypeC &contentType,
                                         size_t length)
   : GeneTypeC(name),
     m_contentType(&contentType),
     m_length(length)
  {
  }

  GeneTypeArrayBaseC::GeneTypeArrayBaseC(BinIStreamC &strm)
   : GeneTypeC(strm),
     m_contentType(0),
     m_length(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeFloatC");
    strm >> m_contentType >> m_length;
  }

  //! Load form a binary stream
  GeneTypeArrayBaseC::GeneTypeArrayBaseC(std::istream &strm)
   : GeneTypeC(strm),
     m_contentType(0),
     m_length(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeArrayBaseC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    strm << m_contentType << m_length;
    return true;
  }

  //! Save to text stream
  bool GeneTypeArrayBaseC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Create randomise value
  void GeneTypeArrayBaseC::Random(GenePaletteC &palette,GeneC::RefT &newValue) const
  {
    SArray1dC<GeneC::ConstRefT> newArray(m_length);
    for(unsigned i =0 ;i < m_length;i++) {
      GeneC::RefT gene;
      m_contentType->Random(palette,gene);
      newArray[i] = gene;
    }
    newValue = new GeneArrayC(*this,newArray);
  }

  //! Mutate a gene
  bool GeneTypeArrayBaseC::Mutate(GenePaletteC &palette,
                                 float fraction,
                                 bool mustChange,
                                 const GeneC &original,
                                 RavlN::SmartPtrC<GeneC> &newValue) const
  {
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Mutate Array. "));
    if((fraction < palette.Random1() && !mustChange) || m_length == 0) {
      newValue = &original;
      return false;
    }
    const GeneArrayC &oldArrayGene = dynamic_cast<const GeneArrayC &>(original);

    const SArray1dC<GeneC::ConstRefT> &oldArray = oldArrayGene.Array();

    SArray1dC<GeneC::ConstRefT> newArray(m_length);

    bool ret = false;

    int changeIndex = -1;
    if(mustChange) {
      if(oldArray.Size() > 0)
        changeIndex = palette.RandomUInt32() % oldArray.Size().V();
    }

    for(unsigned i = 0;i < m_length;i++) {
      GeneC::RefT newGene;
      if(static_cast<int>(i) == changeIndex || fraction > palette.Random1()) {
        if(oldArray[i]->Mutate(palette,fraction,changeIndex == (int) i,newGene))
          ret = true;
        newArray[i] = newGene.BodyPtr();
      } else
        newArray[i] = oldArray[i];
    }
    if(ret) {
      newValue = new GeneArrayC(*this,newArray);
    } else {
      newValue = &original;
    }
    return ret;
  }

  //! Mutate a gene
  void GeneTypeArrayBaseC::Cross(GenePaletteC &palette,
                                 const GeneC &original1,
                                 const GeneC &original2,
                                 RavlN::SmartPtrC<GeneC> &newValue) const
  {
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Crossing Array."));
    // Zero length array means there's no information...
    if(m_length < 1) {
      newValue = &original1;
      return ;
    }
    const GeneArrayC &oldArrayGene1 = dynamic_cast<const GeneArrayC &>(original1);
    const GeneArrayC &oldArrayGene2 = dynamic_cast<const GeneArrayC &>(original2);

    size_t crossAtIndex = palette.RandomUInt32() % m_length;
    SArray1dC<GeneC::ConstRefT> newArray(m_length);

    size_t i = 0;
    for(;i < crossAtIndex;i++)
      newArray[i] = oldArrayGene1.Array()[i];
    GeneC::RefT newGene;
    m_contentType->Cross(palette,
                          *oldArrayGene1.Array()[crossAtIndex],
                          *oldArrayGene2.Array()[crossAtIndex],
                          newGene);
    newArray[crossAtIndex] = newGene.BodyPtr();
    for(;i < m_length;i++)
      newArray[i] = oldArrayGene2.Array()[i];

    newValue = new GeneArrayC(*this,newArray);
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Cross at %f, %zu %zu new size:%zu  ",crossAt,size1,size2,newArray.size()));
  }

  //! Create a new Array
  bool GeneTypeArrayBaseC::CreateArray(const GeneFactoryC &context,
                                       RCWrapAbstractC &array,
                                       const SArray1dC<GeneC::ConstRefT> &geneArray) const
  {
    RavlIssueError("abstract method");
    return false;
  }


  XMLFactoryRegisterConvertC<GeneTypeArrayBaseC,GeneTypeC> g_registerGeneTypeArray("RavlN::GeneticN::GeneTypeArrayBaseC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeArrayBaseC,"RavlN::GeneticN::GeneTypeArrayBaseC");
  static RavlN::TypeNameC g_typePtrGeneTypeArray(typeid(GeneTypeArrayBaseC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeArrayBaseC>");

  // -----------------------------------------------------------------

  GeneArrayC::GeneArrayC(const GeneTypeArrayBaseC &geneType,
                         const SArray1dC<GeneC::ConstRefT> &anArray)
   : GeneC(geneType),
     m_array(anArray)
  {
    RavlAssert(m_array.Size() == geneType.Length());
  }

  //! Factory constructor
  GeneArrayC::GeneArrayC(const XMLFactoryContextC &factory)
   : GeneC(factory)
  {
    XMLFactoryContextC childContext;

    if(factory.ChildContext("Array",childContext)) {
      SArray1dC<GeneC::ConstRefT> array(childContext.Children().Size());
      unsigned i = 0;
      for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
        GeneC::ConstRefT gene;
        if(!childContext.UseChildComponent(it->Name(),gene)) {
          SysLog(SYSLOG_ERR,"Failed to load child component %s, at %s ",it->Name().data(),childContext.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to load component");
        }
        // FIXME:- Check type exists for field
        m_array[i++] = gene;
      }
    }
  }

  //! Load form a binary stream
  GeneArrayC::GeneArrayC(BinIStreamC &strm)
   : GeneC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneC");
    UInt32T size;
    strm >> size;
    m_array = SArray1dC<GeneC::ConstRefT>(size);
    for(UInt32T i = 0;i < size;i++)
      strm >> ObjIO(m_array[i]);
  }

  //! Load form a binary stream
  GeneArrayC::GeneArrayC(std::istream &strm)
   : GeneC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneArrayC::Save(BinOStreamC &strm) const
  {
    if(!GeneC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    UInt32T size = m_array.Size();
    strm << size;
    for(unsigned i = 0;i < size;i++) {
      strm << ObjIO(m_array[i]);
    }

    return true;
  }

  //! Save to binary stream
  bool GeneArrayC::Save(std::ostream &strm) const
  {
    strm << "Array:\n";
    unsigned size = m_array.Size();
    for(unsigned i = 0;i < size;i++) {
      strm << "{\n";
      m_array[i]->Save(strm);
      strm << "}\n";
    }
    return true;
  }

  //! Save to binary stream
  bool GeneArrayC::Save(RavlN::XMLOStreamC &strm) const
  {
    GeneC::Save(strm);
    strm << XMLStartTag("Array");
    unsigned size = m_array.Size();
    for(unsigned i = 0;i < size;i++) {
      m_array[i]->Save(strm);
      strm << XMLIndentDown << XMLEndTag;
    }
    strm << XMLIndentDown << XMLEndTag;
    return true;
  }

  //! Generate an instance of the class.
  void GeneArrayC::Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const
  {
    const GeneTypeArrayBaseC *gtlb = dynamic_cast<const GeneTypeArrayBaseC *>(&Type());
    RavlAssert(gtlb != 0);
    gtlb->CreateArray(context,handle,m_array);
  }

  //! Visit all gene's in tree.
  void GeneArrayC::Visit(GeneVisitorC &visitor) const {
    GeneC::Visit(visitor);
    for(unsigned i = 0;i < m_array.Size();i++) {
      RavlAssert(m_array[i].IsValid());
      m_array[i]->Visit(visitor);
    }
  }

  //! Generate a hash value for the gene
  size_t GeneArrayC::Hash() const {
    size_t ret = m_array.Size();
    for(unsigned i = 0;i < m_array.Size();i++)
      ret += m_array[i]->Hash();
    return ret;
  }

  //! Test is value is effectively equal to this within tolerances specified in the type.
  bool GeneArrayC::IsEffectivelyEqual(const GeneC &other) const
  {
    if(other.Type() != Type()) return false;
    const GeneArrayC *geneArray = dynamic_cast<const GeneArrayC *>(&other);
    if(geneArray == 0) return false;
    if(geneArray->m_array.Size() != m_array.Size()) return false;
    for(unsigned i = 0;i < m_array.Size();i++) {
      if(!m_array[i]->IsEffectivelyEqual(*geneArray->m_array[i]))
        return false;
    }
    return true;
  }

  //! Dump description in human readable form.
  void GeneArrayC::Dump(std::ostream &strm,UIntT indent) const {
    GeneC::Dump(strm,indent);
    strm << "\n";
    for(unsigned i = 0;i < m_array.Size();i++) {
      m_array[i]->Dump(strm,indent+1);
      strm << "\n";
    }
  }

  static XMLFactoryRegisterConvertC<GeneArrayC,GeneC> g_registerGeneArray("RavlN::GeneticN::GeneArrayC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneArrayC,"RavlN::GeneticN::GeneArrayC");
  static RavlN::TypeNameC g_typePtrGeneArray(typeid(GeneArrayC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneArrayC>");

  void LinkGenomeArray()
  {}

}}
