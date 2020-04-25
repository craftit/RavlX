#ifndef MEMMODULE_HH
#define MEMMODULE_HH

#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/StringList.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/RefCounter.hh"
#include "Ravl/Option.hh"
#include "Ravl/OS/Filename.hh"
#include "Ravl/IO.hh"
#include "Ravl/Threads/ThreadEvent.hh"

#include "Ravl/Threads/MemUnit.hh"
#include "Ravl/Threads/MemIter.hh"

//! docentry="Ravl.API.OS.Multithreading Framework"
//! author="Bill Christmas"
//! lib=RavlMemModules
//! example=testMemModules.cc


namespace RavlN {
  class MemModulesC;

  //! userlevel=Develop
  //: Body of a memory module + associated process call
  // This is the class that specific modules inherit

  class MemModuleBodyC
    : public RCBodyVC {

    StringC moduleName;  // used to pass name from constructor to memory
    StringC opXmlFileName;  // where the memory o/p is stored
    OptionC options;   // to pass options through to processes
    ThreadEventC threadDone;  // signals that thread is finished
  
    HashC<StringC, MemIterC> ipSources;  // list of i/ps from other module memories
    RCHashC<StringC,StringC> memDotXmlAttribs;  // info for mem.xml
    bool loadMem;  // if true, module is loaded from memory instead of being run
    PairC<IntT> loadBounds;  // optional bounds when loading from file
    int verbosity; // verbosity for memory system and base module
    int logVerb;   // verbosity for application modules
    MemUnitC memory;  // memory items for this module
    OStreamC log;     // for logging interesting moments
    OStreamC devnull; // for disposing of uninteresting log messages

    // items that deal with the current state of the module
    typedef enum {Uninitialised, Running, Finished} ModuleStateT; // possible module states
    ModuleStateT state; // current module state
    mutable MutexC stateLock;  // lock for state

    MemModuleBodyC(const MemModuleBodyC &);
    //: Disable body copy constructor.
    // no definition -> linker error
  
  public:

    //:-----------------
    //: Constructor / destructor
    MemModuleBodyC(const StringC& ModuleName);
    //: Constructor from process name

    virtual ~MemModuleBodyC() { 
    //  ReleaseAllAccesses();
    }

  public:  
    //:-----------------
    //: Running specific modules
    // These functions are generally used by the derived module classes when running the module.
    // They do not appear in the handle.

    MemIterC GetAccess(const StringC& Source);
    //: Returns memory access to source. 
    // If the source memory is not available, the source module is launched as a separate thread.
  
    MemIterNC GetJointAccess(const StringListC& Sources);
    //: Returns joint access to several sources.  
    // <p> The access will iterate
    //   through the sources synchronously, i.e. returning a set of memory items
    //   that have intersecting timecodes.</p>
    // <p> If you need to get several items from one module for each item of
    //   another module, use nested loops with separate accesses instead.</p>
    // 
    // <p> If you need several accesses to one source use RemoveAccess function.</p>

    void Add(const MemItemC& Item)
    { memory.Add(Item); }
    //: Adds item to memory

    void AddComment(const StringC& Comment)
    { memory.AddComment(Comment); }
    //: Adds comment to memory
    // Allows informal description of data items

    OptionC Options()
    { return options; }
    //: Returns options for memory module

    OStreamC& Log(IntT Verbosity=0)
    { return (logVerb >= Verbosity) ? log : devnull; }
    //: Log message 
    // Logs message if module logging verbosity >= arg.<br>
    // Default is to log regardless

    template <class PixelT>
    void Log(IntT Verbosity, const StringC& Dest, const RavlImageN::ImageC<PixelT>& Img) const
    { if (logVerb >= Verbosity) RavlN::Save(Dest, Img); }
    //: Save/display image

    bool Verbosity(IntT Verbosity) const 
    { return (logVerb >= Verbosity); }
    //: Test if given verbosity is above threshold for log
    // True if given verbosity >= default verbosity level

    IntT Verbosity() const 
    { return (logVerb); }
    //: Verbosity level used for log

  public:
    //:-----------------
    //: The remaining functions mirror the handle functions

    void Load(const RCHashC<StringC,StringC>& XmlAttribs,
              const PairC<IntT>& Bounds)
    {
      loadMem = true;
      memDotXmlAttribs = XmlAttribs;
      loadBounds = Bounds;
    }
    //: Loads elements in memory from XML file

    void SaveTop(XMLOStreamC &Strm) const
    { 
      if (!memDotXmlAttribs.IsEmpty()) {
        Strm.StartTag(moduleName, memDotXmlAttribs, true); Strm.EndTag();
      }
    }
    //: Save module entry in memory contents table if it has ever been run

    bool Init(const StringC& Params, const FilenameC& LogDir, bool ShowLogWnd);
    //: Initialise ready for running
    //!param: params - parameters for process in form of string of options
    //!param: logDir - directory for log file
    //!param: ShowLogWnd - show module progress log in separate window
    // Not needed if module is loaded from memory

    void Launch(bool NoThread);
    //: Launches the module either as a separate thread or sequentially
    //!param: params - parameters for process in form of string of options
    //!param: logDir - directory for log file
    //!param: ShowLogWnd - show module progress log in separate window
    //!param: NoThread - run module serially rather than as separate thread

    void RunNoThread(const StringC& Params, const FilenameC& LogDir, bool ShowLogWnd) ;
    //: Runs the module without threading
    //!param: params - parameters for process in form of string of options
    //!param: logDir - directory for log file
    //!param: ShowLogWnd - show module progress log in separate window

    virtual bool Run()=0;
    //: Run the actual code of the specific module

    bool Wait();
    //: Sleeps until module is finished
    // Returns true if really waited

    const MemUnitC& Memory() const
    { return memory; }
    //: Returns the memory of this module for reading

    MemUnitC& Memory()
    { return memory; }
    //: Returns the memory of this module for reading/writing

    StringC ModuleName() const
    { return memory.MemoryName(); }
    //: Returns module name

    SizeC ID()
    { if(memory.WasLoaded()) return -1; else return (size_t) this; }
    //: Returns thread ID of this module

    void Close() 
    { memory.Add(MemItemC::EofItem()); }
    //: No more items to be added to this memory

    bool Thread();
    //: A wrapper for the memory process to be launched
    // Does anything that is common to all processes

    bool IsUsed() const  {
      MutexLockC lock(stateLock);
      return (state != Uninitialised);
    }
  
    bool IsRunning() const { 
      MutexLockC lock(stateLock);
      return (state == Running);
    }

    void Stop(bool StopFlag = true)
    { RavlAlwaysAssertMsg(0, "Discontinued function"); }
    //: stop processing nicely

    bool AskedToStop() const
    {return false;}// RavlAlwaysAssertMsg(0, "Discontinued function"); return false; }

    void ReleaseAccess(const StringC& Source);
    //: Removes memory access from internal reference table of memory unit
    // It also removes it from list of inputs for this module

    void ReleaseAllAccesses();
    //: Removes all memory accesses to other modules

  };

  //! userlevel=Normal
  //:  A memory module.

  // <p> Includes:</p> <ul>
  // <li> the data storage (MemUnitC)</li>
  // <li> associated call to process that generates the data</li>
  // <li> links (MemoryAccessC) to other modules to get data from</li>
  // <li> means of launching itself off as a separate thread</li>
  // </ul>
  // <p> Individual modules inherit the <a href="MemModuleBodyC.html">body
  //   class</a> only, which has additional functionality for accessing and
  //   manipulating the module contents.</p>

  class MemModuleC : public RCHandleC<MemModuleBodyC>
  {
  protected:
    MemModuleC(MemModuleBodyC &bod)
      : RCHandleC<MemModuleBodyC>(bod)
    {}
    //: Body constructor.

  public:
    MemModuleC()
    {}
    //: Default constructor.
    // Creates an invalid handle.
  
    void Load(const RCHashC<StringC,StringC>& XmlAttribs, const PairC<IntT>& Bounds)
    { Body().Load(XmlAttribs, Bounds); }
    //: Loads elements in memory from XML file

    void SaveTop(XMLOStreamC &Strm) const
    { Body().SaveTop(Strm); }
    //: Save module entry in memory contents table if it has ever been run

    bool Init(const StringC& Params, const FilenameC& LogDir, bool ShowLogWnd)
    { return Body().Init(Params, LogDir, ShowLogWnd); }
    //: Sets up logs and options for running
    //!param: params - parameters for process in form of string of options
    //!param: logDir - directory for log file
    //!param: ShowLogWnd - show module progress log in separate window

    void Launch(bool NoThread)
    { Body().Launch(NoThread); }
    //: Launches the module as a separate thread if needed
    //!param: params - parameters for process in form of string of options
    //!param: logDir - directory for log file
    //!param: ShowLogWnd - show module progress log in separate window
    //!param: NoThread - run module serially rather than as separate thread
    // It checks that module has not been run and results are not available; if neither, it launches the thread

    bool Wait()
    { return Body().Wait(); }
    //: Sleeps until module is finished
  
    MemUnitC Memory() const
    { return Body().Memory(); }
    //: Returns the memory of this module for reading

    StringC ModuleName() const 
    { return Body().ModuleName(); }
    //: Returns module name
  
    IntT ID()
    { return Body().ID(); }
    //: Returns thread ID of this module

    void Close()
    { Body().Close(); }
    //: No more items to be added to this memory

    bool Thread()
    { return Body().Thread(); }
    //: A wrapper for the memory process to be launched
    // Does anything that is needed for all processes

    bool IsUsed() const  
    { return Body().IsUsed(); }
    //: Is used ?
  
    bool IsRunning() const
    { return Body().IsRunning(); }

    void Stop(bool StopFlag)
    { Body().Stop(StopFlag); }
    //: stop processing nicely

    bool AskedToStop() const
    { return Body().AskedToStop(); }

    void ReleaseAccess(const StringC& Source)
    { Body().ReleaseAccess(Source); }
    //: Removes memory access from internal table

    void ReleaseAllAccesses()
    { Body().ReleaseAllAccesses(); }
    //: Removes all memory accesses

    friend void MemModuleBodyC::Launch(bool NoThread);

  };

}
  
#endif
