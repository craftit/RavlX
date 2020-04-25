#include "Ravl/Threads/MemIter.hh"
#include "Ravl/Threads/RWLock.hh"
#include "Ravl/Threads/MemModules.hh"


using namespace RavlN;

//: Constructor on module name
MemIterC::MemIterC(const StringC& Source) :
  memUnit(ModuleManager.AccessModule(Source).Memory()),
  iter(memUnit.Items()),
  curItem(*iter),
  bounds(DurationC(0, TimeCodeC::Max()-1)),
  readTempItems(false)
{}


//: Constructor on module name and time bounds
MemIterC::MemIterC(const StringC& Source,
           const DurationC& Bounds) :
  memUnit(ModuleManager.AccessModule(Source).Memory()),
  iter(memUnit.Items()),
  curItem(*iter),
  bounds(Bounds),
  readTempItems(false)
{
  MoveToFirst(); 
}


//: Constructor that points to memory unit loaded directly from XML file
MemIterC::MemIterC(const StringC& Module, const StringC& Path) :
  memUnit(Module, ""),
  iter(memUnit.Items()),
  curItem(*iter),
  bounds(DurationC(0, TimeCodeC::Max()-1)),
  readTempItems(false)
{
  FilenameC path(Path);
  if (path.IsDirectory()) 
    path += "/" + Module + ".xml";
  if (path.IsReadable())
    memUnit.Load(path, PairC<IntT>(0, RavlConstN::maxInt));
  else throw ExceptionC("Error in MemIterBodyC::MemIterBodyC: Cannot open file " + path);
}


//: Moves to first item within current bounds.
// Return value: true if successful; false if there is no element within the bounds, in which case points to the next available element.

bool MemIterC::MoveToFirst()
{
  while ((curItem.Start() > bounds.Start()) && Prev());
  while ((curItem.Start() < bounds.Start()) && Next());
  return (bounds.Contains(curItem.Duration()) && !curItem.Eof());
}


bool MemIterC::MoveTo(const TimeCodeC& Tc)
{
  if ((curItem.Start() > Tc) || !bounds.Contains(curItem.Duration()))
    MoveToFirst();
  while ((curItem.Start() < Tc) && bounds.Contains(curItem.Duration()))
    Next();
  return curItem.IsValid() && bounds.Contains(curItem.Duration());
}



//: Moves iterator to next item
// Returns: true if successful; false if there is no element within the current bounds, or is at end of memory, and points to the next available element.

bool MemIterC::operator++()
{
  if(memUnit.Verbosity(10))
    cout << "Waiting for next item for " << memUnit.MemoryName()
         << ";  current item time: " << curItem.Start() << endl;
  if (curItem.Eof())  return false;

  RWLockHoldC hold(memUnit.MemLock());
  do {
    while(iter.IsLast()) { // should never execute > once
      if(memUnit.Verbosity(10))
        cout << "Sleeping while waiting for " << memUnit.MemoryName() << endl;
      hold.Unlock();
      memUnit.Sleep();  
      hold.LockRd();
    }
    //go to new item
    ++iter;
    if(!iter) {// this *should* never happen (if previous "while" ran ok)
      cerr << "Error in MemIterC::Next()\n";
      throw ExceptionC("Error in MemIterC::Next()");    
    }
  } while(!iter->Eof() && ((iter->Start() < bounds.Start()) ||
                           (!readTempItems && !iter->IsFinalValue())));
  curItem = *iter;
  
  if(!curItem.IsValid()) {
    cerr << "Error in MemIterC::Next(): invalid item\n";
    throw ExceptionC("Error in MemIterC::Next(): invalid item");    
  }

  if(memUnit.Verbosity(10))
    cout << "Got next item for " << memUnit.MemoryName()
         << ";          current item time: " << curItem.Start() << " is OK" << endl;
  return (bounds.Contains(curItem.Duration()) && !curItem.Eof());
}


bool MemIterC::operator--()
{
  if(!curItem.IsValid() || iter.IsFirst() || !iter.PrevData().IsValid())
    return false;
  {
    RWLockHoldC hold(memUnit.MemLock());
    iter--;
    curItem = *iter;
  }
  return (bounds.Contains(curItem.Duration()) && curItem.IsValid());
}

//: Processing required for non-default constructors
void MemIterC::Init()
{
  if ((iter->Start() < bounds.Start()) || 
      (!readTempItems && !iter->IsFinalValue()))
    Next();
  RavlAlwaysAssertMsg(iter.IsValid(), "There should always be at least an eof item");
}





//: Create joint access from list of names    
MemIterNC::MemIterNC(const StringListC& SourceNames)

{
  RealT frameRate(RavlConstN::nanReal);
  for(ConstDLIterC<StringC> src(SourceNames); src; ++src)
  {
    MemIterC access(*src);
    AddAccess(*src, access);
    frameRate = access->Duration().Start().FrameRate();
  }
  // (rather messy way of picking up frame rate currently used)
  bounds = DurationC(TimeCodeC(0,frameRate), TimeCodeC::Max(frameRate)-1);
  currentIntersection = DurationC(TimeCodeC(0,frameRate), TimeCodeC(0,frameRate));
}


//: Add access to managed set of accesses
void MemIterNC::AddAccess(const StringC& AccName, MemIterC& Access)
{
  if(accTable.IsElm(AccName))
  {
    cerr << "MemIterNC::AddAccess:Trying to add access which is already in the set\n";
    throw ExceptionC("MemIterNC::AddAccess:Trying to add access which is already in the set");
  }
  accTable.Insert(AccName, Access);
}



//: Moves to set of intersecting items which include Tc.
bool MemIterNC::MoveTo(const TimeCodeC& Tc)
{
  if (currentIntersection.Start() > Tc)
    MoveToFirst();
  while ((currentIntersection.Start() < Tc) && bounds.Contains(currentIntersection)) {
    for(HashIterC<StringC, MemIterC> acc(accTable); acc.IsElm(); acc.Next()) {
      acc->Next();
    }
    NextIntersection();
  }
  return currentIntersection.IsValid() && bounds.Contains(currentIntersection);
}


bool MemIterNC::NextIntersection()
{
  try {
    // Move accesses forward to current time where necessary
    for(HashIterC<StringC, MemIterC> acc(accTable); acc.IsElm(); acc.Next()) {
      while (((*acc)->Start() < currentIntersection.Start()) && !acc->Eof()) {
        acc->Next();
      }
    }
    // If the duration of each item overlaps with those of all of the others, a joint intersection is guaranteed.  (Take my word for it.)
    // The algorithm actually works the other way round:
    // if item duration from any access A is earlier than and non-overlapping with any of the others, there is no intersection,
    // so get next item of A and go round the loop again
    bool needAnotherIteration;
    do {
      needAnotherIteration = false;
      for(HashIterC<StringC, MemIterC> acc1(accTable); acc1.IsElm(); acc1.Next()) {
        bool acc1Intersected = true;
        for(HashIterC<StringC, MemIterC> acc2(accTable); acc2.IsElm(); acc2.Next()) {
          if ((*acc1)->Duration() < (*acc2)->Duration())
            acc1Intersected = false; // -> acc1 too early
        }
        if (!acc1Intersected) { // if acc1 too early, get another one
          acc1->Next();  // can't find one
          needAnotherIteration = true;
        }
      }
    } while (needAnotherIteration);
  }
  catch (ExceptionC) {
    // set current interval to trigger invalid iterator
    currentIntersection = DurationC(bounds.End()+1, bounds.End());
    return false;
  }

  // find duration of intersection of all accesses
  currentIntersection = bounds;
  for(HashIterC<StringC, MemIterC> acc(accTable); acc.IsElm(); acc.Next()) {
    currentIntersection = currentIntersection && (*acc)->Duration();
  }
  return currentIntersection.IsValid() && currentIntersection.End() != TimeCodeC::Max();
}


