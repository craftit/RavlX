#include "Ravl/Threads/MemModule.hh"
#include "Ravl/Threads/MemModules.hh"
#include "Ravl/IO.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/Image/Image.hh"
#include "Ravl/Image/ByteRGBValue.hh"
#include "Ravl/EntryPnt.hh"
#include "Ravl/Option.hh"
#include "Ravl/Point2d.hh"
#include "Ravl/Resource.hh"


// Define the classes for the modules
DECLARE_RAVL_MEM_MODULE(Mem1);
DECLARE_RAVL_MEM_MODULE(Mem2);
DECLARE_RAVL_MEM_MODULE(Mem3);
DECLARE_RAVL_MEM_MODULE(Mem4);
DECLARE_RAVL_MEM_MODULE(Mem5);
DECLARE_RAVL_MEM_MODULE(Mem6);
DECLARE_RAVL_MEM_MODULE(Mem7);


// Declare the module objects
inline void RegisterAllModules()
{
  Mem1C  Mem1  ("mem1");
  Mem2C  Mem2  ("mem2");
  Mem3C  Mem3  ("mem3");
  Mem4C  Mem4  ("mem4");
  Mem5C  Mem5  ("mem5");
  Mem6C  Mem6  ("mem6");
  Mem7C  Mem7  ("mem7");
}

// Create a consecutive list of items
bool Mem1BodyC::Run() {
  UIntT n(Options().Int("n", 10, "No. of items created"));
  Options().Check();

  for (UIntT i=4; i<n; ++i) {
    MemItemC newItem(DurationC(TimeCodeC(i,50)));
    Sleep(0.2);
    newItem.SetAttr("scan", i);
    ImageC<ByteRGBValueC> im(100, 150);
    newItem.SetImage("image", im);
    Add(newItem);
    Log(0) << "Added " << i << endl;
  }
  return true;
}


// Uses some items from mem1, and also feedback from mem5
bool Mem2BodyC::Run() {
  MemIterC iter("mem1");
  MemIterC feedback("mem5");
  feedback.IncludeTemporaryItems();
  TimeCodeC start;
  while (++iter) {
    if ((iter->Duration().Start().FrameCount() % 3) == 2) {
      MemItemC newItem;
      ImageC<ByteRGBValueC> im = iter->GetImage("image");
      start = iter->Duration().Start();
      for (IntT i=3; i>= 0; --i) {
        if (i < 3) {
          Log(0) << "From mem5: " << feedback->GetAttr<StringC>("a") << endl;
        }
        newItem = MemItemC(DurationC(start, TimeCodeC(iter->Start())),i);
        newItem.SetAttr("brgb", (i ? "yes" : "no"));
        Add(newItem);
        Log(0) << "Added " << "frame: " << im.Frame() 
               <<"; duration: " << newItem.Duration()
               << ";"<< newItem << endl;
        ++feedback;
      }
    }
  }
  return true;
}


bool Mem3BodyC::Run() {
  MemIterNC iter("mem1 mem2");
  for (iter.Init(); iter; ++iter) {
    const MemItemC item = iter["mem1"];
    MemItemC newItem(iter.Duration());
    newItem.SetAttr("joint", item.GetAttr<StringC>("scan"));
    Add(newItem);
    Log(0) << "Added " << newItem << endl;
  }
  return true;
}


// Processes mem2 o/p as 2 nested loops: inner one is to process temporary results.
// Can also be done as single while() if preferred.
bool Mem4BodyC::Run() {
  MemIterC iter("mem2");
  iter.IncludeTemporaryItems();
  while (++iter) {
    do {
      Log(0) << (iter->IsFinalValue() ? "Final result" : "Partial result") << endl;
      Log(0) << "Duration: " << iter->Duration() << endl;
      Log(0) << "Attribute: " << iter->GetAttr<StringC>("brgb") << endl;
      MemItemC newItem(iter->Duration(), !iter->IsFinalValue());
      newItem.SetAttr("a", "b");
      Add(newItem);
    } while (!iter->IsFinalValue() && ++iter);
  }
  return true;
}


// Processes mem2 o/p as 2 nested loops: inner one is to process temporary results.
bool Mem5BodyC::Run() {
  MemIterC iter("mem2");
  iter.IncludeTemporaryItems();
  for (iter.Init(); iter; ++iter) {
    while (!iter->IsFinalValue()) {
      Log(0) << "Partial result" << endl
             << "Duration: " << iter->Duration() << endl
             << "Attribute: " << iter->GetAttr<StringC>("brgb") << endl;
      MemItemC newItem(iter->Duration(), !iter->IsFinalValue());
      newItem.SetAttr("a", "b");
      Add(newItem);
      ++iter;
    }
    Log(0) << "Final result" << endl
           << "Duration: " << iter->Duration() << endl
           << "Attribute: " << iter->GetAttr<StringC>("brgb") << endl;
    MemItemC newItem(iter->Duration(), !iter->IsFinalValue());
    newItem.SetAttr("a", "c");
    Add(newItem);
  }
  return true;
}


// Reads from external XML file
bool Mem6BodyC::Run() {
  MemIterC iter("mem5");
  while (++iter) {
    Log(0) << "Got item value \"" << iter->GetAttr<StringC>("a")
           << "\" at " << iter->Start().FrameCount() << endl;
  }

  MemIterC ext("projection", StringC("/vol/vssp/acasva/Precomputed/Singles/Psychology/L0"));
  while (++ext) {
    Log(0) << "Got item value \"" << ext->GetAttr<Point2dC>("centreCourt") << endl;
  }
  return true;
}


// Uses some items from mem1
bool Mem7BodyC::Run() {
  MemIterC iter("mem1");
  TimeCodeC start;
  while (++iter) {
    if ((iter->Duration().Start().FrameCount() % 3) == 2) {
      MemItemC newItem;
      ImageC<ByteRGBValueC> im = iter->GetImage("image");
      start = iter->Duration().Start();
      newItem = MemItemC(DurationC(start));
      newItem.SetAttr("brgb", "yes");
      Add(newItem);
      Log(0) << "Added " << "frame: " << im.Frame() 
             <<"; duration: " << newItem.Duration()
             << ";"<< newItem << endl;
    }
  }
  return true;
}


int testsrv(int argc, char* argv[]) {
  OptionC opt(argc, argv, true);
  //storage related options
  FilenameC   memInDir(opt.String ("mi",  "", "Directory for memory loading"));
  FilenameC  memOutDir(opt.String ("mo",  Resource("data/tmp","memmodules"), "Directory for storing memory"));
  int       startField(opt.Int    ("I",   -1, "Start field to load from memory storage (default: beginning of memory)"));
  int   numberOfFields(opt.Int    ("N",   -1, "Number of fields to load from memory storage (default: until end of memory)"));
  //processing related options
  StringC  optionsFile(opt.String ("opt", "", "File containing option strings for modules (empty string -> default values)"));
  StringListC  outputs(opt.List   ("T",   "mem3", "List of desired target modules"));
  bool     runSerially(opt.Boolean("ser", "Run modules serially rather than as parallel threads"));
  bool          forget(opt.Boolean("F",   "forget unwanted data in memory"));
  //debug options
  UIntT      verbosity(opt.Int    ("v",   0, "Verbosity for modules manager"));
  bool     showLogWnds(opt.Boolean("sl",  "show log windows"));
  opt.DependXor("sf st");
    
  opt.Check();
  try {
    // create the memory system
    ModuleManager.Init(memOutDir, argc, const_cast<const char**>(argv), verbosity);
    if (runSerially)            ModuleManager.RunSerially();
    if (showLogWnds)            ModuleManager.ShowLogWindows();
    if (forget)                 ModuleManager.ForgetMemory();
    if (!optionsFile.IsEmpty()) ModuleManager.SetOptionsFile(optionsFile);

    // register all modules needed for *this* application
    RegisterAllModules();
    
    // read precomputed memory if there is any
    if(!memInDir.IsEmpty()) {
      ModuleManager.Load(memInDir, outputs, startField, numberOfFields);
    }
    
    ModuleManager.Launch(outputs);  // launch the required modules

    ModuleManager.Wait();  // & wait for them to finish
  } 
  catch(const ExceptionC& Exc)
  {
    cerr << "ExceptionC caught and processed in main:\n" << Exc.Text() << endl;
    return -1;
  } 
  catch(const std::exception& ex) 
  {
    cerr << " Exception: " << ex.what() << " in " << argv[0] << endl;
    return -1;
  } 
  catch(...) 
  {
    cerr << "Unknown exception in " << argv[0] << endl;
    return -1;
  }

  return (0);
}


RAVL_ENTRY_POINT(testsrv);

