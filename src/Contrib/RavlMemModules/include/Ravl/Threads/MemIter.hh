#ifndef MEMUNIT_ACCESS_HH
#define MEMUNIT_ACCESS_HH

//! docentry="Ravl.API.OS.Multithreading Framework"
//! author="Bill Christmas"
//! lib=RavlMemModules
//! example=testMemModules.cc

//! userlevel=Normal

#include "Ravl/Threads/MemItem.hh"
#include "Ravl/Threads/MemUnit.hh"
#include "Ravl/CDLIter.hh"

namespace RavlN {

  class StringListC;

//: Provides thread-safe read-only iteration through another module's memory
  class MemIterC {

    MemUnitC memUnit;  // the memory unit that this access points to
    ConstDLIterC<MemItemC> iter;  // iterator on memory item list
    MemItemC curItem; // current memory item
    DurationC bounds;  // time range for which access currently allowed
    bool readTempItems; // true: read all items, inc. those whose values will change;
                        // false: ignore items that do not have final values

  public:

    //:-----
    //: Constructors

    MemIterC() :
      bounds(0, TimeCodeC::Max()-1),
      readTempItems(false)
    {}
    //: Default constructor

    MemIterC(const StringC& Source);
    //: Constructor on module name

    MemIterC(const StringC& Source,
               const DurationC& Bounds);
    //: Constructor on module name and time bounds

    MemIterC(const StringC& Module, const StringC& Path);
    //: Constructor that points to memory unit loaded directly from XML file
    // If <code>Path</code> is a valid directory, this will attempt to load an XML file <code>Path/Module.xml</code>.<br>
    // Otherwise if  <code>Path</code> is a valid XML file, this will attempt to load that file.<br>
    // The <code>Module</code> argument <i>must</i> match the opening tag in the file.


    //:--------------
    //:  Moving access to new item

    void IncludeTemporaryItems()
    { readTempItems = true; }
    //: Iteratory will include memory items marked as temporary results
    // Useful when modules form loops.  Default behaviour is to skip these items.

    bool SetBounds(const TimeCodeC& Start, const TimeCodeC& End);
    //: Sets bounds of memory access for which Eof() is true
    // This also attempts to move access to point to first item within the bounds.<br>
    // Return value: true if successful; false if there is no item within the bounds, in which case points to the next available item.

    bool SetBounds(const DurationC& Duration)
    { bounds = Duration; return MoveToFirst(); }
    //: Sets bounds of memory access for which Eof() is true
    // This also attempts to move access to point to first item within the bounds.<br>
    // Return value: true if successful; false if there is no item within the bounds, in which case points to the next available item.

    bool MoveToFirst();
    //: Moves to first item within current bounds.
    // Return value: true if successful; false if there is no element within the bounds, in which case points to the next available element.

    bool MoveTo(const TimeCodeC& Tc);
    //: Moves to item whose duration includes Tc.
    // Return value: true if successful; false if there is no element within the bounds, or no element that includes Tc, in which case points to the next available element.

  const MemItemC Get(const TimeCodeC& Tc) 
    { MoveTo(Tc); return curItem; }
    //: Gets next item from memory
    // Waits until item is available

    bool operator++(); 
    //: Moves iterator to next item
    // Returns: true if successful; false if there is no element within the current bounds, and points to the next available element.

    bool operator--();
    //: Moves iterator to previous item
    // Returns: true if successful; false if there is no element.  (Not sure what it points to in that case.)

    bool Next()
    { return operator++(); }
    //!deprecated: Use operator++()
    //: Moves iterator to next item
    // Returns: true if successful; false if there is no element within the current bounds, and points to the next available element.

    bool Prev()
    { return operator--(); }
    //!deprecated: Use operator--()
    //: Moves iterator to previous item
    // Returns: true if successful; false if there is no element.  (Not sure what it points to in that case.)

    void Init();
    //: Ensures access is pointing to valid memory item

    //:--------
    //: Access to current memory item

    operator bool() const
    { return ((curItem.End() <= bounds.End()) && !curItem.Eof()); }
    //: True if current item is within current bounds and "end of file" memory item is not reached

    bool Eof() const
    { return !operator bool(); }
    //!deprecated: Use operator bool()
    //: Inverse of operator bool()

    const MemItemC& operator*() const
    { return curItem; }
    //: Constant access to current item
  
    const MemItemC* operator->() const
    { return &curItem; }
    //: Constant access to current item

  };



//: Provides synchronised, thread-safe read-only iteration jointly through several modules' memories
// <p> This is an iterator to index jointly through several other memory
// modules in parallel.  It is synchronised in that it will only return memory
// items that have jointly intersecting time periods.  It skips over any other
// items.</p>
// <p>If you have 2 modules where Module A has several time periods for each
// time period of Module B, and you want to iterate over all of the time
// periods of A within the current time period of B, this class is not what
// you need.  Instead, use separate accesses to each module, in <a href=
// "../Tree/Ravl.API.OS.Multithreading_Framework.Application_Module.html">
// nested loops</a>.</p>

  class MemIterNC {

    RCHashC<StringC, MemIterC> accTable;
    DurationC bounds;
    DurationC currentIntersection;

  public:
    MemIterNC(const StringListC& SourceNames);
    //: Create joint access from list of names    

    void AddAccess(const StringC& AccName, MemIterC& Access);
    //: Adds access to managed set of accesses

    //:--------------
    //:  Moving access to new item

    bool SetBounds(const TimeCodeC& Start, const TimeCodeC& End);
    //: Sets bounds of memory access
    // This also moves access to point to first item within the bounds.<br>
    // Return value: true if successful; false if there is no set of intersecting items within the bounds, in which case points to the next available set.


    bool SetBounds(const DurationC& Duration)
    { bounds = Duration; currentIntersection = bounds.Start(); return NextIntersection(); }
    //: Sets bounds of memory access
    // This also moves access to point to first item within the bounds.<br>
    // Return value: true if successful; false if there is no set of intersecting items within the bounds, in which case points to the next available set.

    bool MoveToFirst()
    { currentIntersection = bounds.Start(); return NextIntersection(); }
    //: Moves to first set of intersecting items within current bounds.
    // Return value: true if successful; false if there is no set of intersecting items within the bounds, in which case points to the next available set.

    bool MoveTo(const TimeCodeC& Tc);
    //: Moves to set of intersecting items which include Tc.
    // Return value: true if successful; false if there is no set of intersecting items within the current bounds, in which case points to the next available item.

    bool operator++()
    { currentIntersection = currentIntersection.End() + 1; return NextIntersection(); }
    //: Moves to next set of intersecting items
    // Return value: true if successful, false if there is no set of intersecting items within the current bounds, in which case points to the next available item.

    bool Next()
    { return operator++(); }
    //: Moves to next set of intersecting items
    // Return value: true if successful, false if there is no set of intersecting items within the current bounds, in which case points to the next available item.

    //bool Prev();
    //: Moves to previous set of intersecting items
    // Return value: true if successful, false if attempt to move before beginning of memory

    void Init()
    {
      for(HashIterC<StringC, MemIterC> acc(accTable); acc.IsElm(); acc.Next()) 
        acc->Init();
      NextIntersection();
    }
    //: Ensures access is pointing to valid memory item

    //:--------
    //: Access to current joint intersection

    DurationC Duration() const
    { return currentIntersection; }
    //: Duration of current joint intersection

    operator bool() const
    { return bounds.Contains(currentIntersection); }
    //: True if current item is within current bounds and "end of file" memory item is not reached

    bool Eof() const
    { return !operator bool(); }
    //: Current meaning: true if either accessing memory item out of current bounds or "end of file" memory item is reached.
    // Should mean: True if "end of file" memory item is reached.

    const MemItemC& operator[](const StringC& ModuleName) const
    { return *(accTable[ModuleName]); }
    //: Constant access to named module in current item

    const MemItemC& operator[](const char* ModuleName) const
    { return *(accTable[ModuleName]); }
    //: Constant access to named module in current item

  protected:

    bool NextIntersection();
    //: Finds next set of jointly intersecting items
    // Does not move pointer if currently in non-empty intersection
    // Returns true if non-empty intersection is found.
  };

  
  /*
  MemIterC GetAccess(const StringC& ModuleName);
  //: Returns iterator pointing to start of ModuleName

  MemIterNC GetJointAccess(const StringC& ModuleNames);
  //: Returns iterator pointing to joint starts of modules in string ModuleNames
  */

}

#endif
