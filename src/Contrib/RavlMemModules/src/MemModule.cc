#include "Ravl/StringList.hh"
#include "Ravl/DList.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Option.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/CDLIter.hh"

#include "Ravl/Threads/MemModule.hh"
#include "Ravl/Threads/MemModules.hh"

using namespace RavlN;


MemModuleBodyC::MemModuleBodyC(const StringC& ModuleName)
  : moduleName(ModuleName),
    opXmlFileName(ModuleManager.RootDir() + "/" + ModuleName + ".xml"),
    options(StringListC("")),
    loadMem(false),
    verbosity(0),
    logVerb(1),
    memory(ModuleName, ModuleManager.RootDir() + "/" + ModuleName, verbosity),
    devnull("/dev/null"),
    state(Uninitialised), 
    stateLock(true)
{}


//: Sets up logs and options for running 
bool MemModuleBodyC::Init(const StringC& Params, const FilenameC& LogDir, bool ShowLogWnd)
{
  if (loadMem) return false;
  StringListC optionsList = StringListC(moduleName)+=Params;
  options = OptionC(optionsList);
  memDotXmlAttribs["options"] = optionsList.Cat();
  memDotXmlAttribs["file"] = opXmlFileName;
  verbosity = options.Int("mv", 0,"Verbosity for memory management.");
  Memory().SetVerbosityLevel(verbosity);
  logVerb = options.Int("v", 1,"Verbosity for log file.");
  FilenameC logFile(LogDir + "/" + moduleName + ".log");
  log = OStreamC(logFile);
  Log(1) << "Module options:\n" << memDotXmlAttribs["options"] <<endl;
  RavlAssertMsg(log, "Couldn't open " + logFile);
  if(ShowLogWnd) {
    StringC logViewStr = StringC("xterm -sl 5000 -sb -si -T ") + moduleName +
        " -e tail --lines=+0 -f " + logFile + " &";
    if(verbosity > 0)
      cout << "starting log: " << logViewStr << endl;
    if (system (logViewStr.chars())) throw ExceptionC("Could not launch log terminal");
  }
  if(verbosity > 0)
    cout << "Verbosity level for module " << moduleName << " is " << verbosity << endl;
  return true;
}


//: Launches the module as a separate thread
void MemModuleBodyC::Launch(bool NoThread) 
{
  //check if module is already running
  MutexLockC lock(stateLock);
  if (state != Uninitialised) {
    if(memory.WasLoaded()) {
      if(verbosity > 0)  cout << "Using loaded module:" << ModuleName() << endl;
    }
    // Module is already running, nothing to do!
    return;
  }
  state = Running;
  lock.Unlock();
  
  // If running serially, call module; otherwise launch as separate thread
  if (NoThread) 
    MemModuleBodyC::Thread();
  else {
    TriggerC processTrigger = Trigger(MemModuleC(*this), &MemModuleC::Thread);
    LaunchThread(processTrigger);
  }

  if(verbosity > 0)
    cout << "launched " << ModuleName() << endl;
}


//: Sleeps until module is finished
bool MemModuleBodyC::Wait() {
  if(!memory.WasLoaded() && IsUsed()) {
    threadDone.Wait();
    if(verbosity > 0)
      cout << "Module " << ModuleName() << " has finished" << endl;
    return true;
  }
  return false;
}

//: A wrapper for the memory process to be launched
bool MemModuleBodyC::Thread() {
  bool success(false);
  try { 
    if (loadMem) {  // load from xml file
      success = memory.Load(memDotXmlAttribs["file"], loadBounds);
    }
    else { // run the code
      memory.OpenXMLStream();
      success = Run();
      memory.CloseXMLStream();
    }
  }
  catch (ExceptionC ex) {
    cerr << "Ravl exception caught in Module \"" << ModuleName() << "\"\n" << ex.Text() << endl;
    success = false;
    throw ex;
  }
  catch(const std::exception& ex) {
    cerr << "exception caught in Module \"" << ModuleName() << "\" :" << ex.what() << endl;
    success = false;
    throw ex;
  }
  catch (...) {
    cerr << "Exception caught in Module \"" << ModuleName() << "\"\n" << endl;
    success = false;
    throw std::exception();
  }
  if(!success)
    cerr << ModuleName() << " finished with possibly bad results!\n";
  Close(); 

  if(verbosity > 0)
    cout << "Module " << ModuleName() << " [" << ID() << "] closing..." << endl;
  if (!loadMem)  log << "Module closing..." << endl;
  threadDone.Post();

  MutexLockC lock(stateLock);
  state = Finished;
  return success;
}


//: Removes memory access from internal reference table of memory unit
// the access object could still exist but main memory will not care about it
void MemModuleBodyC::ReleaseAccess(const StringC& Source) 
{ 
  if(verbosity > 0)
    cout << "Removing access from " << ModuleName() << " to " << Source << endl;

  MemIterC access;
  if(!ipSources.Lookup(Source, access)) //source is not created
  {
    cerr << "MemModuleBodyC::ReleaseAccessToSource:Trying to release access which was not created\n";
    throw ExceptionC("MemModuleBodyC::ReleaseAccessToSource:Trying to release access which was not created");
  }
  ModuleManager[Source].Memory().ReleaseMemoryAccess(ModuleName());
  ipSources.Del(Source);
}

//: Releases all memory accesses to other modules
void MemModuleBodyC::ReleaseAllAccesses()
{
  if(verbosity > 1)
    cout << "MemModuleBodyC::ReleaseAllAccesses in " << ModuleName() << " called\n";
  while(!ipSources.IsEmpty())
  {
    HashIterC<StringC, MemIterC> i(ipSources);
    if(verbosity > 7)
      cout << "Releasing: " << i.Key() << endl;
    ReleaseAccess(i.Key());
    if(verbosity > 7)
      cout << "Ok\n";
  }
  if(verbosity > 1)
    cout << "MemModuleBodyC::ReleaseAllAccesses in " << ModuleName() << " done\n";
}


//: Returns access to another memory "Source". If Source doesn't exist, it creates it
MemIterC MemModuleBodyC::GetAccess(const StringC& Source)
{
  MemIterC access;
  if(!ipSources.Lookup(Source, access)) //create access
  {
    //create memory access
    if(verbosity > 0)
      cout << "Adding access from " << ModuleName() << " to " << Source << endl;
    access = MemIterC(Source);
    access.Init();
    ipSources.Insert(Source, access); 
  }
  return ipSources[Source];
}

//: Returns joint access to several sources. It creates accesses to sources or uses already created
// Note: it uses already created accesses! 
//       If you need several accesses to one source use RemoveAccess function.
MemIterNC MemModuleBodyC::GetJointAccess(const StringListC& SourceNames)
{
  MemIterNC jointAccess(SourceNames);
  jointAccess.Init();
  return jointAccess;
}                               

