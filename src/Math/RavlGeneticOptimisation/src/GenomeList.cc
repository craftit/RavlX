// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2011, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation
//! author=Charles Galambos
//! docentry=Ravl.API.Math.Genetic.Optimisation

#include "Ravl/Genetic/GenomeList.hh"
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
  GeneTypeListBaseC::GeneTypeListBaseC(const XMLFactoryContextC &factory)
   : GeneTypeC(factory),
     m_maxLength(factory.AttributeInt("maxLength",30))
  {
    XMLFactoryContextC childContext;
    factory.UseComponent("ContentType",m_contentType);
  }

  // Constructor
  GeneTypeListBaseC::GeneTypeListBaseC(const std::string &name,const GeneTypeC &contentType,UIntT maxSize)
   : GeneTypeC(name),
     m_contentType(&contentType),
     m_maxLength(maxSize)
  {}

  GeneTypeListBaseC::GeneTypeListBaseC(BinIStreamC &strm)
   : GeneTypeC(strm),
     m_contentType(0),
     m_maxLength(0)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneTypeFloatC");
    strm >> m_contentType >> m_maxLength;
  }

  //! Load form a binary stream
  GeneTypeListBaseC::GeneTypeListBaseC(std::istream &strm)
   : GeneTypeC(strm),
     m_contentType(0),
     m_maxLength(0)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneTypeListBaseC::Save(BinOStreamC &strm) const
  {
    if(!GeneTypeC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    strm << m_contentType << m_maxLength;
    return true;
  }

  //! Save to text stream
  bool GeneTypeListBaseC::Save(std::ostream &strm) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Create randomise value
  void GeneTypeListBaseC::Random(GenePaletteC &palette,GeneC::RefT &newValue) const
  {
    UIntT len = palette.RandomUInt32() % m_maxLength;
    if(len == 0)
      len = 1;
    std::vector<GeneC::ConstRefT> newList;
    newList.reserve(len);
    for(unsigned i =0 ;i < len;i++) {
      GeneC::RefT newGene;
      m_contentType->Random(palette,newGene);
      newList.push_back(newGene.BodyPtr());
    }
    newValue = new GeneListC(*this,newList);
  }

  //! Mutate a gene
  bool GeneTypeListBaseC::Mutate(GenePaletteC &palette,
                                 float fraction,
                                 bool mustChange,
                                 const GeneC &original,
                                 RavlN::SmartPtrC<GeneC> &newValue) const
  {
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Mutate list. "));
    if(fraction < palette.Random1() && !mustChange) {
      newValue = &original;
      return false;
    }
    const GeneListC &oldListGene = dynamic_cast<const GeneListC &>(original);
    const std::vector<GeneC::ConstRefT> &oldList = oldListGene.List();
    std::vector<GeneC::ConstRefT> newList;
    newList.reserve(oldList.size() + 16);
    bool ret = false;

    int changeIndex = -1;
    if(mustChange) {
      if(oldList.size() > 0)
        changeIndex = palette.RandomUInt32() % oldList.size();
    }
    // Randomly insert something at the beginning.
    if(oldList.size() == 0 && (fraction < palette.Random1() || mustChange)) {
      GeneC::RefT newGene;
      m_contentType->Random(palette,newGene);
      newList.push_back(newGene.BodyPtr());
      ret = true;
    }
    for(unsigned i = 0;i < oldList.size();i++) {
      int doOp = -1;
      if(static_cast<int>(i) == changeIndex || fraction > palette.Random1()) {
        doOp = palette.RandomUInt32() % 3;
      }
      // Insert a new random element.
      switch(doOp) {
        case 1: { // Omit element
          ret = true;
        } break;
        case 2: { // Mutate an element
          GeneC::RefT newGene;
          if(oldList[i]->Mutate(palette,fraction,changeIndex == (int) i,newGene)) {
            ret = true;
          }
          newList.push_back(newGene.BodyPtr());
        } break;
        case 0: { // Insert a new element
          GeneC::RefT newGene;
          m_contentType->Random(palette,newGene);
          newList.push_back(newGene.BodyPtr());
          ret = true;
        }  // Fall through
        /* no break */
        default:  // Just leave it alone
          newList.push_back(oldList[i]);
          break;
      }
    }
    if(ret) {
      newValue = new GeneListC(*this,newList);
    } else {
      newValue = &original;
    }
    return ret;
  }

  //! Mutate a gene
  void GeneTypeListBaseC::Cross(GenePaletteC &palette,const GeneC &original1,const GeneC &original2,RavlN::SmartPtrC<GeneC> &newValue) const
  {
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Crossing list."));
    const GeneListC &oldListGene1 = dynamic_cast<const GeneListC &>(original1);
    const GeneListC &oldListGene2 = dynamic_cast<const GeneListC &>(original2);

    float crossAt = static_cast<float>(palette.Random1());
    float crossAt1 = crossAt + static_cast<float>((palette.Random1() * 0.3)-0.15);
    float crossAt2 = crossAt + static_cast<float>((palette.Random1() * 0.3)-0.15);
    size_t size1 = oldListGene1.List().size();
    size_t size2 = oldListGene2.List().size();
    std::vector<GeneC::ConstRefT> newList;

    int ind1 = static_cast<int>(crossAt1 * size1);
    int ind2 = static_cast<int>(crossAt2 * size2);
    if(ind1 >= (int) size1)
      ind1 = size1 - 1;
    if(ind1 < 0)
      ind1 = 0;
    if(ind2 >= (int) size2)
       ind2 = (int) size2-1;
    if(ind2 < 0)
      ind2 = 0;

    newList.reserve(ind1 + (oldListGene2.List().size()-ind2));
    for(int i = 0;i < ind1;i++) {
      newList.push_back(oldListGene1.List()[i]);
    }
    if(size1 > 0 && size2 > 0) {
      const GeneTypeC &gt1 =  oldListGene1.List()[ind1]->Type();
      const GeneTypeC &gt2 =  oldListGene2.List()[ind2]->Type();
      if(&gt1 == &gt2) {
        GeneC::RefT newGene;
        gt1.Cross(palette,*oldListGene1.List()[ind1],*oldListGene2.List()[ind2],newGene);
        if(!newGene.IsValid()) {
          RavlSysLogf(SYSLOG_ERR,"Gene type %s failed to produce a cross value. ",TypeName(typeid(gt1)));
          RavlAssert(0);
          newGene = oldListGene1.List()[ind1];
        }
        newList.push_back(newGene.BodyPtr());
        ind2++;
      }
    }
    for(int j = ind2;j < (int) size2;j++) {
      newList.push_back(oldListGene2.List()[j]);
    }
    newValue = new GeneListC(*this,newList);
    ONDEBUG(RavlSysLogf(SYSLOG_DEBUG,"Cross at %f, %zu %zu new size:%zu  ",crossAt,size1,size2,newList.size()));
  }

  //! Create a new list
  bool GeneTypeListBaseC::CreateList(RCWrapAbstractC &list) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }

  //! Add item to list.
  bool GeneTypeListBaseC::AddToList(RCWrapAbstractC &list,RCWrapAbstractC &item) const
  {
    RavlAssertMsg(0,"not implemented");
    return false;
  }


  XMLFactoryRegisterConvertC<GeneTypeListBaseC,GeneTypeC> g_registerGeneTypeList("RavlN::GeneticN::GeneTypeListBaseC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneTypeListBaseC,"RavlN::GeneticN::GeneTypeListBaseC");
  static RavlN::TypeNameC g_typePtrGeneTypeList(typeid(GeneTypeListBaseC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneTypeListBaseC>");

  // -----------------------------------------------------------------

  GeneListC::GeneListC(const GeneTypeListBaseC &geneType,const std::vector<GeneC::ConstRefT> &aList)
   : GeneC(geneType),
     m_list(aList)
  {}

  //! Factory constructor
  GeneListC::GeneListC(const XMLFactoryContextC &factory)
   : GeneC(factory)
  {
    XMLFactoryContextC childContext;
    m_list.reserve(32);
    if(factory.ChildContext("List",childContext)) {
      for(RavlN::DLIterC<XMLTreeC> it(childContext.Children());it;it++) {
        GeneC::ConstRefT gene;
        if(!childContext.UseChildComponent(it->Name(),gene)) {
          SysLog(SYSLOG_ERR,"Failed to load child component %s, at %s ",it->Name().data(),childContext.Path().data());
          throw RavlN::ExceptionBadConfigC("Failed to load component");
        }
        // FIXME:- Check type exists for field
        m_list.push_back(gene);
      }
    }
  }

  //! Load form a binary stream
  GeneListC::GeneListC(BinIStreamC &strm)
   : GeneC(strm)
  {
    ByteT version = 0;
    strm >> version;
    if(version != 1)
      throw RavlN::ExceptionUnexpectedVersionInStreamC("GeneC");
    UInt32T size;
    strm >> size;
    m_list.reserve(size);
    for(UInt32T i = 0;i < size;i++) {
      GeneC::ConstRefT theGene;
      strm >> ObjIO(theGene);
      m_list.push_back(theGene);
    }
  }

  //! Load form a binary stream
  GeneListC::GeneListC(std::istream &strm)
   : GeneC(strm)
  {
    RavlAssertMsg(0,"not implemented");
  }

  //! Save to binary stream
  bool GeneListC::Save(BinOStreamC &strm) const
  {
    if(!GeneC::Save(strm))
      return false;
    ByteT version = 1;
    strm << version;
    UInt32T size = m_list.size();
    strm << size;
    for(unsigned i = 0;i < size;i++) {
      strm << ObjIO(m_list[i]);
    }

    return true;
  }

  //! Save to binary stream
  bool GeneListC::Save(std::ostream &strm) const
  {
    strm << "List:\n";
    unsigned size = m_list.size();
    for(unsigned i = 0;i < size;i++) {
      strm << "{\n";
      m_list[i]->Save(strm);
      strm << "}\n";
    }
    return true;
  }

  //! Save to binary stream
  bool GeneListC::Save(RavlN::XMLOStreamC &strm) const
  {
    GeneC::Save(strm);
    strm << XMLStartTag("List");
    unsigned size = m_list.size();
    for(unsigned i = 0;i < size;i++) {
      m_list[i]->Save(strm);
      strm << XMLIndentDown << XMLEndTag;
    }
    strm << XMLIndentDown << XMLEndTag;
    return true;
  }

  //! Generate an instance of the class.
  void GeneListC::Generate(const GeneFactoryC &context,RCWrapAbstractC &handle) const
  {
    const GeneTypeListBaseC *gtlb = dynamic_cast<const GeneTypeListBaseC *>(&Type());
    RavlAssert(gtlb != 0);
    RCWrapAbstractC listHandle;
    gtlb->CreateList(listHandle);
    RavlAssert(listHandle.IsValid());
    for(unsigned i = 0;i < m_list.size();i++) {
      RCWrapAbstractC comp;
      m_list[i]->Generate(context,comp);
      RavlAssert(comp.IsValid());
      gtlb->AddToList(listHandle,comp);
    }
    handle = listHandle;
  }

  //! Visit all gene's in tree.
  void GeneListC::Visit(GeneVisitorC &visitor) const {
    GeneC::Visit(visitor);
    for(unsigned i = 0;i < m_list.size();i++) {
      RavlAssert(m_list[i].IsValid());
      m_list[i]->Visit(visitor);
    }
  }

  //! Generate a hash value for the gene
  size_t GeneListC::Hash() const {
    size_t ret = m_list.size();
    for(unsigned i = 0;i < m_list.size();i++)
      ret += m_list[i]->Hash();
    return ret;
  }

  //! Test is value is effectively equal to this within tolerances specified in the type.
  bool GeneListC::IsEffectivelyEqual(const GeneC &other) const
  {
    if(other.Type() != Type()) return false;
    const GeneListC *geneList = dynamic_cast<const GeneListC *>(&other);
    if(geneList == 0) return false;
    if(geneList->m_list.size() != m_list.size()) return false;
    for(unsigned i = 0;i < m_list.size();i++) {
      if(!m_list[i]->IsEffectivelyEqual(*geneList->m_list[i]))
        return false;
    }
    return true;
  }

  //! Dump description in human readable form.
  void GeneListC::Dump(std::ostream &strm,UIntT indent) const {
    GeneC::Dump(strm,indent);
    strm << "\n";
    for(unsigned i = 0;i < m_list.size();i++) {
      m_list[i]->Dump(strm,indent+1);
      strm << "\n";
    }
  }

  static XMLFactoryRegisterConvertC<GeneListC,GeneC> g_registerGeneList("RavlN::GeneticN::GeneListC");
  RAVL_INITVIRTUALCONSTRUCTOR_NAMED(GeneListC,"RavlN::GeneticN::GeneListC");
  static RavlN::TypeNameC g_typePtrGeneList(typeid(GeneListC::RefT),"RavlN::SmartPtrC<RavlN::GeneticN::GeneListC>");

  void LinkGenomeList()
  {}

}}
