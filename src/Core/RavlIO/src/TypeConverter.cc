// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////
//! lib=RavlIO
//! file="Ravl/Core/IO/TypeConverter.cc"

#include "Ravl/DP/TypeConverter.hh"
#include "Ravl/GraphBestRoute.hh"
#include "Ravl/MTLocks.hh"
#include "Ravl/TypeName.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Constructor.
  TypeConverterBodyC::TypeConverterBodyC()
   : m_conversionCache(1000),
     m_version(0)
  {}
  
  //: Get the graph node associated with a named type

  GraphNodeHC<StringC,DPConverterBaseC> TypeConverterBodyC::GetTypeNode(const StringC &name) const {
    const GraphNodeHC<StringC,DPConverterBaseC> *ret = NodeTab().Lookup(name);
    if(ret != 0) return *ret;
    return GraphNodeHC<StringC,DPConverterBaseC>();
  }

  GraphNodeHC<StringC,DPConverterBaseC> TypeConverterBodyC::GetTypeNode(const std::type_info &inf) const {
    StringC typeName(inf.name());
    return GetTypeNode(typeName);
  }

  //: Get the graph node associated with a std::type_info, create if needed

  GraphNodeHC<StringC,DPConverterBaseC> TypeConverterBodyC::UseTypeNode(const std::type_info &inf) {
    StringC typeName(inf.name());
    GraphNodeHC<StringC,DPConverterBaseC> &ret = NodeTab()[typeName];
    if(ret.IsValid())
      return ret;
    ret = ConvGraph().InsNode(typeName);
    return ret;
  }

  //! Find a conversion
  bool TypeConverterBodyC::FindConversion(const std::type_info &from,
                                          const std::type_info &to,
                                          RealT &finalCost,
                                          DListC<GraphEdgeIterC<StringC,DPConverterBaseC> > &convResult) const
  {
    if(from == to) {
      finalCost = 0;
      return true;
    }
#if !(RAVL_COMPILER_VISUALCPP && !RAVL_COMPILER_VISUALCPPNET) || RAVL_COMPILER_VISUALCPPNET_2005
    // Visual C++ can't handle ptr's to functions with reference args.
    // hopefully we'll find a way around this but for now its out.

    Tuple2C<DListC<GraphEdgeIterC<StringC,DPConverterBaseC> >,RealT> conv;
    conv.Data2() = 0;
    Tuple2C<StringC,StringC> cacheKey(from.name(),to.name());

    MTReadLockC cacheLock(5);
    if(!m_conversionCache.Lookup(cacheKey,conv)) {
      // We've not got it cached.
      GraphNodeHC<StringC,DPConverterBaseC> tFrom = GetTypeNode(cacheKey.Data1());
      GraphNodeHC<StringC,DPConverterBaseC> tTo = GetTypeNode(cacheKey.Data2());
      // Both the source and destination have to be registered for any
      // conversion to exist.
      if(tFrom.IsValid() && tTo.IsValid()) {
        conv.Data1() = GraphBestRoute(ConvGraph(),
                            tFrom,
                            tTo,
                            conv.Data2(),
                            &TypeConverterBodyC::EdgeEval);
      }

      // Remember the version of the graph we used to generate the conversion.
      UIntT version = m_version;
      cacheLock.Unlock();

      MTWriteLockC cacheWriteLock(5);
      if(version == m_version) // Check things haven't changed since we re-locked.
        m_conversionCache.Insert(cacheKey,conv);
      cacheWriteLock.Unlock();
    }
    convResult = conv.Data1();
    finalCost = conv.Data2();
    return !conv.Data1().IsEmpty();
#else
    RavlAssert(0);
    return false;
#endif
  }


  //: Test if conversion is possible.
  
  bool TypeConverterBodyC::CanConvert(const std::type_info &from,const std::type_info &to) {
    RealT finalCost = 0;
    DListC<GraphEdgeIterC<StringC,DPConverterBaseC> > conv;
    return FindConversion(from,to,finalCost,conv);
  }

  //: Do conversion through abstract handles.
  
  RCAbstractC TypeConverterBodyC::DoConversion(const RCAbstractC &dat,const std::type_info &from,const std::type_info &to) {
    if(from == to || !dat.IsValid())
      return dat;
    ONDEBUG(std::cout << "Asked to convert " << from.name() << " to " << to.name() << std::endl);
    DListC<GraphEdgeIterC<StringC,DPConverterBaseC> > conv;  
    RealT finalCost = 0;

    if(!FindConversion(from,to,finalCost,conv)) {
      ONDEBUG(std::cout << "No conversion from " << from.name() << " to " << to.name() << std::endl);
      return RCAbstractC();
    }
    
    // Do conversion.
    RCAbstractC at = dat;
    for(DLIterC<GraphEdgeIterC<StringC,DPConverterBaseC> > it(conv);it;it++)
      at = it.Data().Data().Apply(at);
    return at;
    
  }
  
  //: Find a conversion.
  // If found the cost of conversion is put into finalCost.
  
  DListC<DPConverterBaseC> TypeConverterBodyC::FindConversion(const std::type_info &from,const std::type_info &to,RealT &finalCost)  {
    ONDEBUG(std::cout << "Asked to convert " << from.name() << " to " << to.name() << std::endl);
    DListC<GraphEdgeIterC<StringC,DPConverterBaseC> > conv;  
    //  typedef RealT (*AFuncT)(const DPConverterBaseC &);

    if(!FindConversion(from,to,finalCost,conv)) {
      ONDEBUG(std::cout << "No conversion from " << from.name() << " to " << to.name() << std::endl);
      return DListC<DPConverterBaseC>(); // Failed to find conversion.
    }
    
    DListC<DPConverterBaseC> ret;
    for(DLIterC<GraphEdgeIterC<StringC,DPConverterBaseC> > it(conv);it.IsElm();it.Next())
      ret.InsLast(it.Data().Data());
    return ret;
  }
  
  //: Convert an port to a new type if possible
  // Returns true and sets 'result' if operation succeeded.
  bool TypeConverterBodyC::ConvertIPort(const DPIPortBaseC &orgPort,const std::type_info &dataType,DPIPortBaseC &result)
  {
    if(dataType == orgPort.InputType()) {
      result = orgPort;
      return true;
    }
    DListC<DPConverterBaseC> conv = SystemTypeConverter().FindConversion(orgPort.InputType(),dataType);
    if(conv.IsEmpty()) {
      RavlWarning("Failed to find conversion from '%s' to '%s'",
          RavlN::TypeName(orgPort.InputType()),
          RavlN::TypeName(dataType)
          );
      return false;
    }
    //RavlDebug("Building conversion. ");
    DPIPortBaseC convPort = orgPort;
    for(DLIterC<DPConverterBaseC> it(conv);it;it++)
      convPort = it->CreateIStream(convPort);
    RavlAssert(convPort.IsValid());
    result = convPort;
    return true;
  }

  //: Convert an port to a new type if possible
  // Returns true and sets 'result' if operation succeeded.
  bool TypeConverterBodyC::ConvertOPort(const DPOPortBaseC &orgPort,const std::type_info &dataType,DPOPortBaseC &result)
  {
    if(dataType == orgPort.OutputType()) {
      result = orgPort;
      return true;
    }
    DListC<DPConverterBaseC> conv = SystemTypeConverter().FindConversion(orgPort.OutputType(),dataType);
    if(conv.IsEmpty()) {
      RavlWarning("Failed to find conversion from '%s' to '%s'",
          RavlN::TypeName(dataType),
          RavlN::TypeName(orgPort.OutputType()));
      return false;
    }
    DPOPortBaseC convPort = orgPort;
    for(DLIterC<DPConverterBaseC> it(conv);it;it++)
      convPort = it->CreateOStream(convPort);
    result = convPort;
    return true;

  }

  
  RealT TypeConverterBodyC::EdgeEval(const DPConverterBaseC &edge)  { 
    ONDEBUG(std::cout << "Edge cost " << edge.Cost() << " : " << edge.ArgType(0).name() << " " << edge.Output().name() << std::endl);
    return edge.Cost(); 
  }
  
  
  //: Find a conversion.
  
  DListC<DPConverterBaseC> TypeConverterBodyC::FindConversion(const std::type_info &from,const std::type_info &to) {
    RealT finalCost = -1;
    return FindConversion(from,to,finalCost);
  } 

  bool TypeConverterBodyC::Insert(DPConverterBaseC &tc) {
    MTWriteLockC cacheLock(5);
    m_version++;
    ConvGraph().InsEdge(UseTypeNode(tc.ArgType(0)),UseTypeNode(tc.Output()),tc.Body());
    // Clear the cache, things may have changed.
    m_conversionCache.Empty();
    // FIXME :- Check for duplication ??
    return true;
  }
  
  //: Remove conversion from system.
    
  bool TypeConverterBodyC::Remove(DPConverterBaseC &tc)  {
    // TODO :- This is slow !!
    ONDEBUG(std::cerr << "Unregistering converter : "<< tc.ArgType(0).name() << " to " << tc.Output().name() << std::endl);
    MTWriteLockC cacheLock(5);
    m_version++;
    bool ok = false;
    IntT size = 0;
    for(GraphEdgeIterC<StringC,DPConverterBaseC> it(ConvGraph());it.IsElm();it.Next()) {
      if(&it.Data().Body() == &tc.Body()) {
        it.Del();
        ok = true;
        break;
      }
      size++;
    }
    // Clear the cache.
    m_conversionCache.Empty();

    if(!ok)
      std::cerr << "Failed !!! " << size << "\n";
    return true;
  }
  
  TypeConverterC &SystemTypeConverter() {
    static TypeConverterC tc(true);
    return tc;
  }

}
