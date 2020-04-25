// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PROCINFO_HEADER
#define RAVL_PROCINFO_HEADER 1
///////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Processes" 
//! lib=RavlIO
//! file="Ravl/Core/IO/ProcInfo.hh"
//! author="Charles Galambos"
//! date="17/08/98"
//! userlevel=Default

#include "Ravl/DP/Process.hh"
#include "Ravl/DP/ProcIStream.hh"
#include "Ravl/DP/ProcOStream.hh"
#include "Ravl/RCWrap.hh"
#include "Ravl/RCAbstract.hh"
#include "Ravl/SArray1d.hh"

namespace RavlN {
  template <class DataT> class DListC;
  class DPProcInfoC;
  
  ///////////////////////////////
  //! userlevel=Develop
  //: Process information body.
  
  class DPProcInfoBodyC 
    : public DPEntityBodyC 
  {
  public:
    DPProcInfoBodyC();
    //: Default constructor.
    
    virtual const std::type_info &ProcType() const;
    //: Get type of process
    
    virtual IntT Args() const;
    //: Number of args for process.
    
    virtual const std::type_info &ArgType(IntT i) const;
    //: Type of args.
    
    virtual const std::type_info &Output() const;
    //: Output type.
    
    virtual DPProcessBaseC CreateProc() const;
    //: Create instance of process.
    
    virtual DPIPortBaseC CreateIStream(const DPIPortBaseC &inp) const;
    //: Create instance of process.
    
    virtual DPOPortBaseC CreateOStream(const DPOPortBaseC &outp) const;
    //: Create instance of process.
    
    virtual RCAbstractC Apply(const DPProcessBaseC &proc,SArray1dC<RCAbstractC> dat);
    //: Apply process using abstract data handle.
    // These should be RCWrap's of the actual data.
    
    virtual RCAbstractC Apply1(const DPProcessBaseC &proc,const RCAbstractC &arg);
    //: Apply process using abstract data handle.
    // Use only if you know the process uses 1 arg ! 
    
    virtual RCAbstractC Apply2(const DPProcessBaseC &proc,const RCAbstractC &arg1,const RCAbstractC &arg2);
    //: Apply process using abstract data handle.
    // Use only if you know the process uses 2 args ! 
    
    static DListC<DPProcInfoC> &Procs();
    //: List of process types.
  };
  
  
  ///////////////////////////////
  //! userlevel=Normal
  //: Process information.
  
  class DPProcInfoC 
    : public DPEntityC 
  {
  public:
    DPProcInfoC()
      : DPEntityC(false)
      {}
    //: Default Constructor.
    
    static bool IsA(const DPEntityC &ent)
      { return dynamic_cast<const DPProcInfoBodyC *>(&ent.Body()) != 0; }
    //: Is entity a proc info ?
    
  protected:
    DPProcInfoC(DPProcInfoBodyC &bod)
      : DPEntityC(bod)
      {}
    //: Body Constructor.
    
    inline DPProcInfoBodyC &Body() 
      { return static_cast<DPProcInfoBodyC & > (DPEntityC::Body()); }
    //: Access body.
    
    inline const DPProcInfoBodyC &Body() const
      { return static_cast<const DPProcInfoBodyC & > (DPEntityC::Body()); }
    //: Access body.
    
  public:
    inline const std::type_info &ProcType() const
      { return Body().ProcType(); }
    //: Get type of process
    
    inline IntT Args() const
    { return Body().Args(); }
    //: Number of args for process.
    
    inline const std::type_info &ArgType(IntT i) const
      { return Body().ArgType(i); }
    //: Type of args.
    
    inline const std::type_info &Output() const
      { return Body().Output(); }
    //: Output type.
    
    inline DPProcessBaseC CreateProc() const
      { return Body().CreateProc(); }
    //: Create instance of process.  
    
    inline DPIPortBaseC CreateIStream(const DPIPortBaseC &inp) const
      { return Body().CreateIStream(inp); }
    //: Create instance of process.
    
    inline DPOPortBaseC CreateOStream(const DPOPortBaseC &outp) const
      { return Body().CreateOStream(outp); }
    //: Create instance of process.
    
    inline RCAbstractC Apply(const DPProcessBaseC &proc,SArray1dC<RCAbstractC> dat)
      { return Body().Apply(proc,dat); }
    //: Apply process using abstract data handles.
    
    inline RCAbstractC Apply1(const DPProcessBaseC &proc,const RCAbstractC &arg)
      { return Body().Apply1(proc,arg); }
    //: Apply process using abstract data handle.
    // Use only if you know the process uses 1 arg ! 
  
    inline RCAbstractC Apply2(const DPProcessBaseC &proc,const RCAbstractC &arg1,const RCAbstractC &arg2)
      { return Body().Apply2(proc,arg1,arg2); }
    //: Apply process using abstract data handle.
    // Use only if you know the process uses 2 args ! 
    
    friend class DPProcInfoBodyC;
    
  };

}
#endif
