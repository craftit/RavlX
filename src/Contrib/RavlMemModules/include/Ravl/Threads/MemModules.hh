#ifndef MEMMODULES_HH
#define MEMMODULES_HH

#include "Ravl/Threads/LaunchThread.hh"
#include "Ravl/StringList.hh"
#include "Ravl/RCHash.hh"

#include "Ravl/Threads/MemModule.hh"

//! docentry="Ravl.API.OS.Multithreading Framework"
//! author="Bill Christmas"
//! lib=RavlMemModules
//! example=testMemModules.cc

namespace RavlN {


  extern MemModulesC ModuleManager;

#define DECLARE_RAVL_MEM_MODULE(x)                          \
  class x##BodyC : public MemModuleBodyC {                  \
   public:                                                  \
    x##BodyC(const StringC& ModuleName)                     \
     : MemModuleBodyC(ModuleName)                           \
      {}                                                    \
                                                            \
    bool Run();                                             \
  };                                                        \
                                                            \
  class x##C : public MemModuleC {                          \
   public:                                                  \
    x##C(const StringC& ModuleName)                         \
    : MemModuleC(*new x##BodyC(ModuleName))                 \
      { ModuleManager.RegisterModule(*this); }              \
                                                            \
    bool Run() { return Body().Run(); }                     \
  };    



  //! userlevel=Normal
  //: The set of memory modules and their interconnection
  //  A collection of MemModuleC memory modules, together with means of starting
  //  them as separate threads, and a means of visualising the memory contents.
  //  The target modules will be launched, together with any others whose
  //  outputs are needed by the target modules (recursively).  Where precomputed
  //  modules are available, the precomputed results are used unless the module
  //  is in the target list.

  class MemModulesC {

    bool runSerially;
    bool textDisplay;
    bool forget;
    bool endOfSession;
    LaunchThreadC textDisplayThread;
    FilenameC logDir;
    RCHashC<StringC,StringC> optionsTab; // memory options loaded from options file
    UIntT verbosity;
    HashC<StringC,MemModuleC> modules;
    //  Alex: I don't understand why these 2 locks are necessary, because only one thread seems to be running when they are used.
    mutable MutexC modulesTableLock; // to coordinate accesses to list of modules
    mutable MutexC endOfSessionLock; // for correct display
    FilenameC memoryDir;
    bool showLogWnds;
    StringC commandLine; // command line used to create precomputed memory (if any)

  public:
   
    MemModulesC();
    //: Constructor

    ~MemModulesC();
    //: Destructor

    void Init(const FilenameC& memOutDir, int argc, const char* argv[], UIntT Verbosity);
    //: Initialiser
    //!param: MemOutDir - the directory where the memory will be stored
    //!param: argc, argv - the command line used to run the program, for future reference
    //!param: Verbosity - controls level of diagnostic information of the memory system

    //!section: Setup methods, that should be called before modules are launched

    void RunSerially()
    { runSerially = true; }
    //: Run modules in sequence rather than as set of threads

    void ShowDviDisplay()
    { textDisplay = true; }
    //: Show memory contents as text in xdvi window

    void ShowLogWindows()
    { showLogWnds = true; }
    //: Show module logs in separate xterms

    void ForgetMemory()
    { forget = true; }
    //: Forget memory contents that is no longer wanted

    void SetLogDir(const FilenameC& Dir)
    { logDir = Dir; }
    //: Set directory for module log files.
    // Default is memory directory + "/log"

    void SetOptionsFile(const FilenameC& OptFile);
    //: Read file containing module options. 
    // If not set, no module options are read. <br>
    // Each line of the file consists of a module name followed by a set of options for that module in a format to be processed by OptionC.<br>
    // The file is copied to the memory directory

    bool RegisterModule(MemModuleC& Module)
    { return modules.Insert(Module.ModuleName(), Module); }
    //: Register a main memory function
    //!param: Func - the process associated with the memory module
    //!param: ModuleName - name of the memory module
    
    //!section: Module access functions

    MemModuleC operator[] (const StringC& name);
    //: Access the named module (thread safe)

    const MemModuleC operator[] (const StringC& name) const;
    //: Access the named module (thread safe)

    const MemModuleC AccessModule(const StringC& name) const;
    //: Access the named module, launching it if necessary

    RCHashC<StringC,StringC>& OptionsTab() {return optionsTab;}
    //: Table of options lists for the modules

    const RCHashC<StringC,StringC>& OptionsTab() const {return optionsTab;}
    //: Table of options lists for the modules

    StringListC GetModulesList() const;
    //: Returns list of names of currently launched modules

    //!section: Starting, running and stopping the system

    void Launch(const StringListC& targets);
    //: Launches all required modules
    // Determines which modules are needed in addition to those in "targets", and launches them all

    void Wait();
    //:  Waits for the threads to finish

    void StopProcessing(bool StopFlag = true);
    //:Stop processing if StopFlag = true
    // You can call this function many time with no harm<br>
    // If StopFlag = false it clears stop flag in all modules

    bool IsEndOfSession() const 
    { return endOfSession; }
    //: True if memory processes have all finished

    //!section: Miscellaneous

    void Load(const FilenameC& DirName, const StringListC& DontLoad="", int StartField=-1, int NumFields=RavlConstN::maxInt);
    //: Loads precomputed memory contents from set of files
    // Omits modules in <code>DontLoad</code>

    void Save();
    //: Saves top-level XML file

    StringC RootDir() const
    { return memoryDir; }
    //: Returns root directory

    StringC LogDir() const
    { return logDir; }
    //: Returns log file directory

    friend ostream &operator<< (ostream &strm, const MemModulesC &mem); 

  private:
    MemModuleC LaunchMemModule(const StringC& ModuleName) const;
    //: Launch mem module (thread safe)

    void AddIps(const StringC& op);
    //: Finds required i/p modules for module recursively

    bool ListMemory()
    {  RavlAlwaysAssertMsg(0, "xdvi display not implemented yet"); return false;}
    //: Displays memory contents as a text window
    // The display is updated at regular intervals

  };

  ostream &operator<< (XMLOStreamC &strm, const MemModulesC &mem); 
  //: Outputs short-term memory in text form

}

#endif
