#include "Ravl/SArray1d.hh"
#include "Ravl/DArray1dIter.hh"
#include "Ravl/CDLIter.hh"
#include "Ravl/Array1dIter.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/IO.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/XMLTree.hh"

#include "Ravl/Threads/MemModules.hh"

using namespace RavlN;

namespace RavlN{
  MemModulesC ModuleManager;
}

//: Constructor
MemModulesC::MemModulesC()
  : runSerially(false), textDisplay(false), forget(false), endOfSession(false), 
    verbosity(0), modulesTableLock(true), showLogWnds(false)
{}

MemModulesC::~MemModulesC()
{
  if(verbosity > 1)
    cout << "MemModulesC::~MemModulesC called\n";
  
  MutexLockC lock(modulesTableLock);
  
  for(HashIterC<StringC, MemModuleC> curModule(modules); curModule; curModule++)
  {
    if (curModule->IsUsed()) {
      if(verbosity > 7)
        cout << "Release accesses in: " << curModule.Key() << endl;
      curModule->ReleaseAllAccesses();
      if(verbosity > 7)
        cout << "Done\n";
    }
    if (curModule->References() != 1)
      cout << curModule->References() << " reference(s) for " << curModule.Key() << endl;
  }
  
  if(verbosity > 1)
    cout << "MemModulesC::~MemModulesC done\n";
}


//: Initialiser
void MemModulesC::Init(const FilenameC& MemOutDir, int argc, const char* argv[], UIntT Verbosity)
{
  verbosity = Verbosity;
  memoryDir = MemOutDir;
  commandLine = StringC(argv[0]);
  for (int i(1); i<argc; ++i) {
    commandLine += StringC(' ') + argv[i];
  }

  // If the main o/p memory doesn't exist, create it
  if (!((memoryDir.Exists() && memoryDir.IsDirectory() && memoryDir.IsWritable()) || memoryDir.MakeDir()))
    throw ExceptionC("Can't create directory for writing memory: " + memoryDir);

  // Strip off trailing '/'
  while (memoryDir.lastchar() == '/')
    memoryDir = StringC(memoryDir.Size()-1, memoryDir);

  // If a relative path name given, create an absolute one
  if (memoryDir[0] != '/') 
    memoryDir = StringC(getenv("PWD")) + '/' + memoryDir;

  // If the log directory is not there, create it
  logDir = memoryDir + "/log";
  logDir.MakeDir();
  if (!(logDir.IsDirectory() && logDir.IsWritable())) {
    cerr << "Cannot write to log directory " << logDir
         << "; setting to /dev/null"<<endl;
    logDir = "/dev/null";
  }

  // If there is already a top-level mem.xml file, save backup copy
  FilenameC top(memoryDir+"/mem.xml");
  if (top.Exists())  top.Link(top+'~');
}

//: Read file containing module options.  
void MemModulesC::SetOptionsFile(const FilenameC& OptFile)
{
  if ((OptFile == "-") || (OptFile.IsReadable() && !OptFile.IsDirectory())) {
    if(verbosity > 0)
      cout << "Reading options from \"" << OptFile << "\"" << endl;
    IStreamC in(OptFile);
    StringC s;
    while (in) {
      readline(in,s);
      if (!s.IsEmpty())
        optionsTab.Insert(s.before(' '), s.after(' '));
    }
    if (OptFile.IsReadable())  OptFile.CopyTo(memoryDir+"/options");
  }
  else if (!OptFile.IsEmpty())
    cout << "Could not read options file \"" << OptFile << "\"" << endl;
}


//: Access the named module
const MemModuleC MemModulesC::operator[] (const StringC& name) const 
{ 
  MutexLockC lock(modulesTableLock);
  RavlAssertMsg(modules.IsElm(name), "Cannot create access to non-existent source: " + name);
  const MemModuleC module = *modules.Lookup(name);
  return module;
}

//: Access the named module
MemModuleC MemModulesC::operator[] (const StringC& name) 
{
  MutexLockC lock(modulesTableLock);
  RavlAssertMsg(modules.IsElm(name), "Cannot create access to non-existent source: " + name);
  return *modules.Lookup(name);
}

//: Access the named module, launching it if necessary
const MemModuleC MemModulesC::AccessModule(const StringC& name) const 
{ 
  MutexLockC lock(modulesTableLock);
  const MemModuleC module = (*this)[name];
  if (module.IsUsed())  return module;
  lock.Unlock();
  return LaunchMemModule(name);
}


//: Launch mem module thread

MemModuleC MemModulesC::LaunchMemModule(const StringC& ModuleName) const
{
  MutexLockC lock(modulesTableLock);
  
  //check if module registered
  MemModuleC module;
  if(!modules.Lookup(ModuleName,module)) {
    StringC err = StringC("Attempting to launch unregistered module: ") + ModuleName;
    cerr << err << endl;
    throw ExceptionC(err);
  }
    
  if(verbosity > 0)
    cout << "Creating and launching module: " << ModuleName << endl;
  
  // Launch mem module
  StringC moduleOptions;
  if (optionsTab.IsElm(ModuleName))  moduleOptions = optionsTab[ModuleName];
  module.Init(moduleOptions, logDir, showLogWnds);
  module.Launch(runSerially);
  
  return module;
}


//: Launches the modules as a set of separate threads
void MemModulesC::Launch(const StringListC& ModulesList) 
{  
  // Launch the modules
  for (ConstDLIterC<StringC> i(ModulesList); i; ++i) {
    LaunchMemModule(*i);
  }
  // If wanted, set the xdvi display going as another thread
  if (textDisplay) 
    textDisplayThread = LaunchThread(TriggerR(*this, &MemModulesC::ListMemory));
}

//:  Waits for the threads to finish. Wait for only currently running modules.
void MemModulesC::Wait() 
{  
  bool done;
  do { //  need to repeat 'cos not all modules are launched 1st time round
    done = true;
    for(HashIterC<StringC,MemModuleC> i(modules); i; ++i) {
      if (i->IsUsed()) {
        if (i->IsRunning())  done = false;
        i->Wait();
      }
    }
  } while (!done);

  if(verbosity > 0)
    cout << "All modules have finished" << endl;
  this->Save();
  endOfSessionLock.Lock();
  endOfSession = true;
  endOfSessionLock.Unlock();
  if (textDisplay) 
    textDisplayThread.WaitForExit();
  //output top level XML file
}

void MemModulesC::StopProcessing(bool StopFlag)
{
  StringListC modulesList = GetModulesList();

  for(ConstDLIterC<StringC> moduleName(modulesList); moduleName; ++moduleName)
  {
    MemModuleC curModule = operator[](*moduleName);
    curModule.Stop(StopFlag);
  }
}

//: returns list of names of currently launched modules
StringListC MemModulesC::GetModulesList() const
{
  StringListC modulesList;
  MutexLockC lock(modulesTableLock);
  
  for(HashIterC<StringC, MemModuleC> curModule(modules); curModule; curModule++)
    modulesList.InsLast(curModule->ModuleName());

  return modulesList;
}


void PrintTablePreampleLatex(OStreamC &Stream, 
                             const SArray1dC<StringC> &Names1, 
                             const SArray1dC<StringC> &Names2)
{
  int numCols = Names1.Size();

  Stream << "\\newpage";
  Stream << "\\begin{tabular}{|";
  for(int j = 0; j < numCols; j++)  
    Stream << "c|";  

  Stream << "}\\hline" << endl;

  Stream << "{\\bf " << Names1[0] << "}";
  for(int j = 1; j < numCols; j++)  
    Stream << "&{\\bf " << Names1[j] << "}";  
  Stream << "\\\\\n";

  Stream << "\\ [" << Names2[0] << "]";
  for(int j = 1; j < numCols; j++)  
    Stream << "\\ &[" << Names2[j] << "]";  
  Stream << "\\\\\n";

  Stream <<  "\\hline" << endl;
}

void PrintTableBottomLatex(OStreamC &Stream)
{
  Stream << "\\hline";
  Stream << "\\hline\\end{tabular}" << endl;
}



//: Loads precomputed memory contents from set of files
void MemModulesC::Load(const FilenameC&DirName, const StringListC& DontLoad,
                 int StartField, int NumFields)
{
  try {
    // read mem.xml into XML tree
    if(verbosity > 0)
      cout << "Reading memory dir:" << DirName << endl;
    if (!DirName.Exists()) 
      throw ExceptionC("Can't find memory directory " + DirName);
    StringC inPrefix = DirName+"/";
    XMLIStreamC memStream(inPrefix+"mem.xml");
    if (!memStream.IsOpen() || memStream.eof() || !memStream)
      throw ExceptionInvalidStreamC("Can't open stream for reading memory");
    XMLTreeC tree("mem.xml");
    if (!tree.Read(memStream))
      throw ExceptionInvalidStreamC("Can't parse XML stream");
    XMLTreeC top;
    if (!tree.Child("all", top))
      throw ExceptionInvalidStreamC("Can't find outer tag \"all\"");
    XMLTreeC next;
    
    // read command line if reqd.
    if(verbosity > 0) {
      if (!top.Child("commandLine", next))
        throw ExceptionInvalidStreamC("Can't find tag \"commandLine\"");
      cout << "Command line:" << next.Content() << endl;
    }
    
    if (StartField < 0)  StartField = 0;
    if (NumFields < 0)  NumFields = RavlConstN::maxInt;

    // make hash table of modules that must not be loaded
    HashC<StringC,bool> dontLoad;
    for (ConstDLIterC<StringC> m(DontLoad); m; ++m)  dontLoad.Insert(*m, true);    

    //read memmodules
    if (!top.Child("memory", next))
      throw ExceptionInvalidStreamC("Can't find tag \"memory\"");
    for (ConstDLIterC<XMLTreeC> i(next.Children()); i; ++i) {
      if (dontLoad.IsElm(i->Name())) {
        if (verbosity > 1)  cout << "Not loading module: " << i->Name() << endl;
      }
      else {
        // process XML for module
        //verbosity of the module will be set to verbosity for memory management
        if (!modules.IsElm(i->Name()))
          throw ExceptionInvalidStreamC("MemModulesC::Load: unknown module: " + i->Name());
        MemModuleC module = modules[i->Name()];
        if (module.IsUsed()) {
          throw ExceptionInvalidStreamC("MemModulesC::Load: cannot load running module: " + i->Name());
        }
        IntT endField = Min(StartField + NumFields - 1, RavlConstN::maxInt);
        module.Load(i->Data(), PairC<IntT>(StartField, endField));
        if(module.ModuleName().IsEmpty())
          throw ExceptionInvalidStreamC("MemModulesC::Load: error loading module: " + i->Name());
        if(verbosity > 0)
        cout << "Module " << i->Name() << " flagged for loading\n";
      }
    }  
    
    memStream.Close();
  }
  catch(ExceptionC ex) {
    cerr << "Exception loading memory from " << DirName << endl;
    cerr << ex.Text() << endl;
    throw;
  }
  catch( ... ) {
    cerr << "Exception loading memory from " << DirName << endl;
    throw;
  }
}

//: Saves top level XML file
void MemModulesC::Save()
{
  try {
    if(verbosity > 0)
      cout << "saving memory top level file\n";
    FilenameC memFile(memoryDir+"/mem.xml");
    if (memFile.Exists())  memFile.Remove();  // prevents overwriting backup copy
    XMLOStreamC memStream(memFile);
    if(!memStream.IsOpen())
      throw ExceptionC("Can't open stream for writing top-level memory XML");
    
    memStream.StartTag("all");
    memStream.StartContents();
    
    //save command line
    memStream.StartTag("commandLine");
    memStream.StartContents();
    memStream << commandLine;
    memStream.EndTag();
    
    //save all memory modules
    memStream.StartTag("memory");
    memStream.StartContents();
    for(HashIterC<StringC,MemModuleC> i(modules); i; ++i) 
      i->SaveTop(memStream); // only actually saved if it has ever been run
    memStream.EndTag();

    memStream.EndTag();
    memStream.Close();
    if(verbosity > 0)
      cout << "top level file saved\n";
  }
  catch( ... ) {
    cerr << "unhandled exception in storing top-level XML file\n";
    throw;
  }
}


