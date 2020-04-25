#include "Ravl/Threads/MemUnit.hh"

using namespace RavlN;

//: Constructor
MemUnitBodyC::MemUnitBodyC(const StringC& ModuleName, const StringC& SaveDir, IntT Verbosity) :
  memName(ModuleName),
  modulePath(SaveDir),
  wasLoaded(false),
  xmlOStream(""),
  verbosity(Verbosity)
{ items += MemItemC(); }
// Invalid memory item is added to ensure that memory unit is never empty.  This is necessary for MemIterC() to work without MemIterC::Wait().  Since an invalid memory item always has -ve time code, Next() and operator++ will always ignore it.


// Prepare XML stream
void MemUnitBodyC::OpenXMLStream()
{
  StringC xmlFileName(modulePath + ".xml");
  xmlOStream = XMLOStreamC(xmlFileName);
  if(!xmlOStream.IsOpen())
    throw ExceptionC("MemUnitBodyC::OpenXMLStream: Can't open stream for writing memory: " + xmlFileName);
  
  xmlOStream.StartTag(MemoryName());
  xmlOStream.StartContents();
}



//: Close XML stream
void MemUnitBodyC::CloseXMLStream()
{
  xmlOStream.EndTag();
  xmlOStream.Stream() << " " << endl;
  xmlOStream.Close();
}
 

//: Add item to memory.
void MemUnitBodyC::Add(const MemItemC& NewItem)
{
  NewItem.Save(xmlOStream, modulePath); // save to XML
  xmlOStream.os().flush();  // flush XML stream
  RWLockHoldC hold(lock, RWLOCK_WRITE);
  items += NewItem;        // add to mem list
  sync.WakeAll ();
  if(verbosity > 5)
    cout << "Post from " << memName << endl;
}


//: Load elements into memory from file  
bool MemUnitBodyC::Load(const StringC& LoadPath, const PairC<IntT>& Bounds)
{
  try
  {
    FilenameC xmlLoadFile = LoadPath;
    modulePath = DirectoryC(xmlLoadFile.before(".xml")); // overrides constructor value when loading
    RavlAlwaysAssertMsg(xmlLoadFile.Exists(), "MemUnitBodyC::Load in " + memName + ": File for reading memory doesn't exist: " + xmlLoadFile);
    if(verbosity > 0)
      cout << "loading module '" << memName << "' from " << xmlLoadFile << endl
           << "prefix: " << xmlLoadFile.before(memName + ".xml") << endl
           << "bounds: " << Bounds << endl;
    StringC tagName;
    XMLTagOpsT readTagRes;
    if(verbosity > 0)
      cout << "reading module '" << memName << "' from " << xmlLoadFile << endl;
    XMLIStreamC partStream(xmlLoadFile);

    RavlAlwaysAssertMsg(partStream.IsOpen() && !partStream.eof()  && partStream,
                        "MemUnitBodyC::Load in " + memName + ": Can't open stream for reading memory");

    readTagRes = partStream.ReadTag(tagName);
    RavlAlwaysAssertMsg(readTagRes == XMLBeginTag, "MemUnitBodyC::Load in " + memName + ": Missed 'begin' tag, name: \"" + tagName + "\" (possibly mem file empty or not found)");
    RavlAlwaysAssertMsg(tagName == memName, "MemUnitBodyC::Load in " + memName + ": Missed <" + memName + "> tag in file " + xmlLoadFile);

    //read items
    int itemCounter = 0;
    while(partStream.PeekTag(tagName) == XMLBeginTag)
    {
      //create new MemItemC
      MemItemC newItem;
      newItem.Load(partStream, modulePath);

      if((newItem.Start().FrameCount() >= Bounds[0]) && 
         (newItem.End().FrameCount()   <= Bounds[1]) )
      {
        if(verbosity > 10)
          cout << "item start: " << newItem.Start() << "  end: " << newItem.End()
               << "; inserting item into memory\n";
        RWLockHoldC hold(lock, RWLOCK_WRITE);
        items.InsLast(newItem);
        itemCounter++;
      }
    }
    if(verbosity > 5)
      cout << "finishing reading\n";

    readTagRes = partStream.ReadTag(tagName);
    RavlAlwaysAssertMsg(readTagRes == XMLEndTag, "MemUnitBodyC::Load in " + memName + ": Missed 'end' tag in " + memName);
    
    if(verbosity > 5)
      cout << "closing stream\n";
    partStream.Close(); //close file
    
    //add eof item
    if(verbosity > 5)
      cout << "adding eof element\n";
    {
      RWLockHoldC hold(lock, RWLOCK_WRITE);
      items.InsLast(MemItemC::EofItem()); 
    }
    if(verbosity > 5)
      cout << "done\n";
  }
  catch(const ExceptionC& ex)
  {
    sync.WakeAll();
    cerr << "Caught exception in MemUnitBodyC::LoadItems while loading memory "
         << memName << " from file " << LoadPath << ": \"" << ex.Text() << "\"" << endl;
    throw;
  }
  catch( ... )
  {
    sync.WakeAll();
    cerr << "Caught exception in MemUnitBodyC::LoadItems while loading memory"
         << memName << " from file " << LoadPath << endl;
    throw;
  }
  if(verbosity > 5)
    cout << "unlocking memory\n";
  sync.WakeAll();
  if(verbosity > 0)
    cout << "Items read: " << items.Size() << endl;

  wasLoaded = true;
  return true;
}

