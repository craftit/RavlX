
#include "Ravl/DP/DPNetwork.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/XMLFactoryRegister.hh"

namespace RavlN {

  //: Default constructor.
  // Creates an empty network

  DPNetworkC::DPNetworkC()
  {}

  //: Create from an XML factory.
  DPNetworkC::DPNetworkC(const XMLFactoryContextC &factory)
  {
    XMLFactoryContextC componentContext;
    if(factory.ChildContext("Components",componentContext)) {
      m_components = CollectionC<RCAbstractC>(componentContext.Children().Size());
      for(RavlN::DLIterC<XMLTreeC> it(componentContext.Children());it;it++) {
        XMLFactoryContextC ff;
        componentContext.ChildContext(it->Name(),ff);
        StringC loadAs = ff.AttributeString("loadAs","RavlN::RCAbstractC");

        RCAbstractC value;
        if(loadAs == "RavlN::RCAbstractC") {
          if(!componentContext.UseChildComponent(it->Name(),value,false)) {
            RavlError("Failed to load child component %s, at %s ",it->Name().data(),componentContext.Path().data());
            throw RavlN::ExceptionBadConfigC("Failed to load component");
          }
        } else if(loadAs == "RavlN::DPStreamOpC") {
          DPStreamOpC valueStreamOp;
          if(!componentContext.UseChildComponent(it->Name(),valueStreamOp,false)) {
            RavlError("Failed to load child component %s, at %s ",it->Name().data(),componentContext.Path().data());
            throw RavlN::ExceptionBadConfigC("Failed to load component");
          }
          value = valueStreamOp.Abstract();
        } else {
          RavlError("Don't know how to load as %s ",loadAs.c_str());
          throw RavlN::ExceptionBadConfigC("Unknown load as.");
        }

        m_components.Append(value);

        StringC aName;

        if(!(aName = ff.AttributeString("exportAsOPort")).IsEmpty()) {
          DPOPortBaseC aPortBase(value);
          if(!aPortBase.IsValid()) {
            RavlError("Not an OPort %s ",aName.c_str());
            throw RavlN::ExceptionBadConfigC("Not an OPort.");
          }
          m_oports.InsLast(aPortBase);
          RavlDebug("Exporting pure OPort '%s' from %s ",aName.c_str(),ff.Path().c_str());
        }

        if(!(aName = ff.AttributeString("exportAsIPort")).IsEmpty()) {
          DPIPortBaseC aPortBase(value);
          if(!aPortBase.IsValid()) {
            RavlError("Not an IPort %s ",aName.c_str());
            throw RavlN::ExceptionBadConfigC("Not an IPort.");
          }
          m_iports.InsLast(aPortBase);
          RavlDebug("Exporting pure IPort '%s' from %s ",aName.c_str(),ff.Path().c_str());
        }


        DPStreamOpC aStreamOp(value);
        if(aStreamOp.IsValid()) {
          if(!(aName = ff.AttributeString("exportOPlug")).IsEmpty()) {
            DPOPlugBaseC oplug;
            if(!aStreamOp.GetOPlug(aName,oplug)) {
              RavlError("Failed for find OPlug %s ",aName.c_str());
              throw RavlN::ExceptionBadConfigC("Failed to find OPlug.");
            }
            m_oplugs.InsLast(oplug);
            RavlDebug("Exporting OPlug '%s' from %s ",aName.c_str(),ff.Path().c_str());
          }
          if(!(aName = ff.AttributeString("exportIPlug")).IsEmpty()) {
            DPIPlugBaseC iplug;
            if(!aStreamOp.GetIPlug(aName,iplug)) {
              RavlError("Failed for find IPlug %s ",aName.c_str());
              throw RavlN::ExceptionBadConfigC("Failed to find OPlug.");
            }
            m_iplugs.InsLast(iplug);
            RavlDebug("Exporting IPlug '%s' from %s ",aName.c_str(),ff.Path().c_str());
          }
          if(!(aName = ff.AttributeString("exportOPort")).IsEmpty()) {
            DPOPortBaseC oport;
            if(!aStreamOp.GetOPort(aName,oport)) {
              RavlError("Failed for find OPort %s ",aName.c_str());
              throw RavlN::ExceptionBadConfigC("Failed to find OPlug.");
            }
            m_oports.InsLast(oport);
            RavlDebug("Exporting OPort '%s' from %s ",aName.c_str(),ff.Path().c_str());
          }
          if(!(aName = ff.AttributeString("exportIPort")).IsEmpty()) {
            DPIPortBaseC iport;
            if(!aStreamOp.GetIPort(aName,iport)) {
              RavlError("Failed for find IPort %s ",aName.c_str());
              throw RavlN::ExceptionBadConfigC("Failed to find OPlug.");
            }
            m_iports.InsLast(iport);
            RavlDebug("Exporting IPort '%s' from %s ",aName.c_str(),ff.Path().c_str());
          }
        }
      }
    }

    XMLFactoryContextC connectionContext;
    if(factory.ChildContext("InternalConnect",connectionContext)) {
      for(RavlN::DLIterC<XMLTreeC> it(connectionContext.Children());it;it++) {
        StringC from = it->AttributeString("from","");
        StringC to = it->AttributeString("to","");
        StringC plug = it->AttributeString("plug","");
        StringC port = it->AttributeString("port","");
        StringC mode = it->AttributeString("mode","in");
        DPStreamOpC fromOp,toOp;
        componentContext.UseChildComponent(from,fromOp);
        componentContext.UseChildComponent(to,toOp);
        mode.downcase();
        if(mode == "in" || mode == "i") {
          DPIPortBaseC iport;
          if(!fromOp.GetIPort(port,iport)) {
            RavlError("Failed to get IPort '%s' from '%s' in %s ",
                port.c_str(),
                from.c_str(),
                componentContext.Path().c_str());
            fromOp.Dump(std::cerr);
            throw RavlN::ExceptionBadConfigC("Failed to find IPort.");
          }
          if(!toOp.SetIPort(plug,iport)) {
            RavlError("Failed to set IPlug '%s' from '%s' type %s in %s ",
                plug.c_str(),
                to.c_str(),
                TypeName(iport.InputType()),
                componentContext.Path().c_str());
            toOp.Dump(std::cerr);
            throw RavlN::ExceptionBadConfigC("Failed to find IPlug.");
          }
        } else if(mode == "out" || mode == "o") {
          DPOPortBaseC oport;
          if(!fromOp.GetOPort(port,oport)) {
            RavlError("Failed to get IPort '%s' from '%s' in %s ",
                port.c_str(),
                from.c_str(),
                componentContext.Path().c_str());
            fromOp.Dump(std::cerr);
            throw RavlN::ExceptionBadConfigC("Failed to find IPort.");
          }
          if(!toOp.SetOPort(plug,oport)) {
            RavlError("Failed to set IPlug '%s' from '%s' type %s in %s ",
                plug.c_str(),
                to.c_str(),
                TypeName(oport.OutputType()),
                componentContext.Path().c_str());
            toOp.Dump(std::cerr);
            throw RavlN::ExceptionBadConfigC("Failed to find IPlug.");
          }
        } else {
          RavlError("Unknown mode '%s' ",mode.c_str());
          throw RavlN::ExceptionBadConfigC("Unknown mode.");
        }
      }
    }

    Setup(factory);
  }

  DListC<DPIPlugBaseC> DPNetworkC::IPlugs() const {
    return m_iplugs;
  }
  //: Input plugs.

  DListC<DPOPlugBaseC> DPNetworkC::OPlugs() const {
    return m_oplugs;
  }
  //: Output plugs

  DListC<DPIPortBaseC> DPNetworkC::IPorts() const {
    return m_iports;
  }
  //: Input ports.

  DListC<DPOPortBaseC> DPNetworkC::OPorts() const {
    return m_oports;
  }
  //: Output ports

  static XMLFactoryRegisterConvertC<DPNetworkC,DPStreamOpBodyC> g_registerDPNetwork("RavlN::DPNetworkC");

  void LinkDPNetwork()
  {}
}
