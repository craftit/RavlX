#ifndef MEMITER_HH
#define MEMITER_HH

#include "Ravl/String.hh"
#include "Ravl/OS/Directory.hh"
#include "Ravl/DList.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/Threads/ThreadSignal.hh"
#include "Ravl/XMLStream.hh"

#include "Ravl/Threads/MemItem.hh"

//! docentry="Ravl.API.OS.Multithreading Framework"
//! author="Bill Christmas"
//! lib=RavlMemModules


namespace RavlN {

  //! userlevel=Develop
  //: Body class for <a href="RavlN.MemUnitC.html">MemUnitC</a>

  class MemUnitBodyC
    : public RCBodyC {

    StringC memName;
    DirectoryC modulePath;  // dir for images; also XML file name without ".xml"
    DListC<MemItemC> items;


    bool wasLoaded;            // was loaded from precomputed file
    XMLOStreamC xmlOStream;    // stream for XML output
    IntT verbosity;

    mutable RWLockC lock; // to coordinate reading from and writing to memory unit
    mutable ThreadSignalC sync; // to allow modules to wait for input
    mutable RWLockC accessesLock; // lock for accesses hash table


  public:
    MemUnitBodyC(const StringC& ModuleName, const StringC& SaveDir, IntT Verbosity);
    //: Constructor

    void OpenXMLStream();
    // prepare XML stream
  
    void CloseXMLStream();
    // Close XML stream

    const StringC& MemoryName() const 
    { return memName; }  
    //: Returns memory name

    void Add(const MemItemC& NewItem);
    //: Add item to memory.

    void AddComment(const StringC& Comment)
    { xmlOStream.Comment(Comment); }
    //: Add comment to memory

    bool Load(const StringC& XMLFileName, const PairC<IntT>& Bounds);
    //: Load elements into memory from file  

    bool WasLoaded() const
    { return wasLoaded; }
    //: True if memory was loaded from XML file

    void SetVerbosityLevel(IntT Verbosity)  
    { verbosity = Verbosity; }

    ConstDLIterC<MemItemC> GetMemoryAccess(const StringC& Name);
    //: Creates memory access. 

    void ReleaseMemoryAccess(const StringC& Name)
    {}
    //: Removes access to other memory from internal reference table 
    //Name - name of other memory
    // Currently a null method

    const DListC<MemItemC> Items() const
    { return items; }
    //: Const access to memory item  list

    RWLockC& MemLock() const
    { return lock; }
    //: Memory R/W lock

    void Sleep() const
    { sync.Wait(); }
    //: Sleep until new item arrives

    bool Verbosity(IntT Level) const
    { return verbosity >= Level; }
    //: Returns true if verbosity >= Level
  
  };
  
  //! userlevel=Normal
  //: The memory storage component of a MemModuleC
  // Provides storage for time sequence of data, including:<ul>
  //<li>  a list of time-stamped memory elements MemItemC</li>
  //<li>  means of access by other modules</li>
  //<li>  locking for shared memory access</li>
  //</ul>

  
  class MemUnitC
    : public RavlN::RCHandleC<MemUnitBodyC>  {
  public:
    MemUnitC()
    {}
    //: Constructor of invalid memory
    
    MemUnitC(const StringC & ModuleName, const StringC& SaveDir, IntT Verbosity=0) 
      : RCHandleC<MemUnitBodyC>(*new MemUnitBodyC(ModuleName, SaveDir, Verbosity))
    {}
    //: Constructor
    
    void OpenXMLStream()
    { Body().OpenXMLStream(); }
    // Open XML stream

    void CloseXMLStream()
    { Body().CloseXMLStream(); }
    // Close XML stream

    const StringC & MemoryName() const
    { return Body().MemoryName(); }
    //: Returns memory name 
    
    void Add(const MemItemC & NewItem) 
    { Body().Add(NewItem); }
    //: Add item to memory. 
    
    void AddComment(const StringC& Comment)
    { Body().AddComment(Comment); }
    //: Add comment to memory

    bool Load(const StringC & XMLFileName, const PairC<IntT>& Bounds) 
    { return Body().Load(XMLFileName, Bounds); }
    //: Prepare for loading elements in memory from file   
    
    bool WasLoaded() const
    { return Body().WasLoaded(); }
    //: True if memory was loaded from XML file 
    
    void SetVerbosityLevel(IntT Verbosity) 
    { Body().SetVerbosityLevel(Verbosity); }
    
    ConstDLIterC<MemItemC> GetMemoryAccess(const StringC& Name)
    { return Body().GetMemoryAccess(Name); }
    //: Creates memory access. 

    void ReleaseMemoryAccess(const StringC & Name) 
    { Body().ReleaseMemoryAccess(Name); }
    //: Removes access to other memory from internal reference table  
    //Name - name of other memory
    
    const DListC<MemItemC> Items() const
    { return Body().Items(); }
    //: Const access to memory item  list
    
    RWLockC& MemLock() const
    { return  Body().MemLock(); }
    //: Memory R/W lock

    void Sleep() const
    { Body().Sleep(); }
    //: Sleep until new item arrives

    bool Verbosity(IntT Level) const
    { return Body().Verbosity(Level); }
    //: Returns true if verbosity >= Level
  
  protected:

    MemUnitC(MemUnitBodyC &bod)
     : RCHandleC<MemUnitBodyC>(bod)
    {}
    //: Body constructor. 
    
    MemUnitBodyC& Body()
    { return static_cast<MemUnitBodyC &>(RCHandleC<MemUnitBodyC>::Body()); }
    //: Body Access. 
    
    const MemUnitBodyC& Body() const
    { return static_cast<const MemUnitBodyC &>(RCHandleC<MemUnitBodyC>::Body()); }
    //: Body Access. 
    
  };

}

#endif


