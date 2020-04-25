// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_HASHTABLE_HEADER
#define RAVL_HASHTABLE_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Hash/Hash.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="1/9/1996"
//! docentry="Ravl.API.Core.Hash Tables"
//! example=WordFreq.cc;exHash.cc
//! userlevel=Normal

#include "Ravl/SArray1d.hh"
#include "Ravl/IntrDList.hh"
#include "Ravl/IntrDLIter.hh"
#include "Ravl/SArray1dIter.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/StdHash.hh"
#include "Ravl/Types.hh"
#include "Ravl/DeepCopy.hh"

#if RAVL_COMPILER_GCC3_4
#include "Ravl/BinStream.hh"
#endif

#define HASHC_DEBUG 0

#if defined(HASHC_DEBUG) || RAVL_CHECK
#include "Ravl/Assert.hh"
#endif

namespace RavlN {
  
  class BinOStreamC;
  class BinIStreamC;
 
  template<class K,class T> class HashIterC;
  template<class K,class T> class HashC;
  template<class K,class T> class HashElemC;
  
  ///////////////////////////
  //! userlevel=Develop
  //: Base class for table.
  // SMALL OBJECT!
  // <p>
  // Use HashART<K,T> in HashAR.hh in preference to this, in nearly
  // every case its faster. 
  // <p>
  // The following RAVL classes can be used as a key for this
  // template:
  // <p>
  //   StringC, Index2dC, Index3dC, IndexNdC,  PixelC, RGBValueC,
  //   RGBValueU16C, ByteRGBValueC, RGBXValueC(Doesn't use X component.)
  // <p>
  // If you want a class added e-mail me.
  
  class HashBaseC {
  public:
    HashBaseC()
      : elements(0)
    {}
    //: Default constructor.
    
    HashBaseC(SizeT nelements)
      : elements(nelements)
    {}
    //: Constructor.
    
    bool IsEmpty() const
    { return elements == 0; }
    //: Is the table empty ?
    
    SizeT Size() const
    { return elements; }
    //: Count number of elements in table.
    
    static SizeT NextPrime(SizeT v);
    //: Get the next prime not smaller than v.
    
  protected:
    SizeT elements; // Number of elements in the table.
  };
  
  
  ///////////////////////////////
  //! userlevel=Normal
  //: General hash table. (auto-resizing) 
  // Note this is a SMALL OBJECT, assignment of these hash tables creates a new copy of the object. 
  // <a href="RavlN.RCHashC.html">RCHashC</a> is a fully reference counted version of this class. <br>
  // Type K is the hash key: it must define a function <code>
  //   unsigned int K::Hash(); </code>  which returns a number fairly unique to the key,
  //    or a global function of the form <code> SizeT StdHash(const K &x); </code> which returns the key. <p>
  //   Also <code> bool K::operator== (const K &Oth); </code>  to test equality. <p>
  
  // !!!!! Update() Requires a default constructor & a working assignment operator !!!!! <p>
  
  // A few things to bear in mind when writing StdHash() functions. <p>
  // 1) Try and use all the bits in the values being hashed. <p>
  // 2) Use quick operations that tend to preserve the number of bits in the value.
  //     i.e. '+' '^' '-' <p>
  // 3) Try and generate a hash value which uses as many bits as possible,
  //    but no more than the thing your hashing.  i.e. ByteRGBValueC only
  //    uses 21 bits, and so does the default hash value. <p>
  // 4) For best results try and generate a function which distributes key values
  //    evenly over the range of hash values. <p>
  
  template<class K,class T>
  class HashC 
    : public HashBaseC 
  {
  public:
    typedef T ElementT;
    //: Allow function templates to find type of array.
    
    typedef K KeyT;
    //: Allow function templates to find type of index.
    
    typedef HashIterC<K,T> IteratorT;
    //: Type of iterator.
    
    HashC(SizeT nBins = 23)
      : table((int) nBins) 
    { RavlAssertMsg(nBins > 0,"Must have at least 1 bin in a hash table."); }
    //: Create table with nBins.
    // Bin size must be at least 1.
    //!param: nBins - Number of bin to initially put in the table.
    
    HashC(const HashC<K,T> &oth) 
      : HashBaseC(oth.elements),
        table(oth.table.Copy())
    {}
    //: Copy access structure.
    //!param: oth - Table to copy to make this one.
    
    HashC(const HashC<K,T> &oth,UIntT levels);
    //: Copy access structure.
    //!param: oth - Table to copy to make this one.
    //!param: levels - Depth of copy 
    
    HashC(const Tuple2C<K,T> *data);
    //: Initialise from simple array.
    //!param: data - pointer to array of tuples to initialise the table with, terminated with an entry with a duplicate key of the first entry.
    // Note: Array must be terminated by a duplicate of the first key. (i.e. == must return true between them)
    
    HashC(std::istream &in);
    //: Recreate from stream.
    //!param: in - Stream to construct table from
    
    HashC(BinIStreamC &in);
    //: Recreate from stream.
    //!param: in - Binary stream to construct table from
    
    void Save(std::ostream &strm) const;
    //: Save to stream.
    
    void Save(BinOStreamC &strm) const;
    //: Save to binary stream.
    
    HashC<K,T> Copy() const;
    //: Make a copy of the table.
    //!return: Copy of this table.
    
    HashC<K,T> DeepCopy(UIntT levels = ((UIntT) -1)) const
    { return HashC<K,T>(*this,levels); }
    //: Make a deep copy of the object.
    // levels == 0, no copy. <br>
    // levels == 1, As Copy(). <br>
    // levels == 2, Copy one level down. <br>
    // Note: As keys should be constant, they are not copied.
    
    inline T &operator[](const K &key) 
    { return Update(key); }
    //: Associative array style interface.
    // Update member of hash table, will create new one if it doesn't
    // exist. 
    //!param: key - Key for element to access.
    //!return: Reference to element in table
    
    inline const T &operator[](const K &key) const;
    //: Associative array style of access.
    // Note: this will cause an assertion failure if element doesn't exist as the table can't be modified.
    //!param: key - Key for element to access.
    //!return: Reference to element in table
    
    inline bool Lookup(const K &key,T &data) const;
    //: Lookup data for key.
    //!param: key - Key value to be used in lookup.
    //!param: data - Place to hold data if lookup is successful.
    //!return: true if entry is found, and is assigned to 'data'.
    // otherwise 'data' is not modified.
    
    inline bool Update(const K &key,const T &data);
    //: Update member of hash table, will create new one if it doesn't
    //: exist. 
    // Requires a default constructor & a working assignment operator !!
    //!param: key - Key for element to update.
    //!param: data - Data to update entry with.
    //!returns: true=Member existed already. false=New one was added.
    
    inline T &Update(const K &key);
    //: Get value, add default if its not there. Return reference anyway.
    //!param: key - Key for element to access.
    //!return: Reference to element in table
    
    inline bool Insert(const K &key,const T &data) 
    { return Update(key,data); }
    //: Default insertion operation, same as Update(K,T);
    //!param: key - Key for element to update.
    //!param: data - Data to update entry with.
    //!returns: true=Member existed already. false=New one was added.
    
    inline T &Access(const K &key,const T &def = T());    
    //: Access key, if it does exists create a new bin with value 'def'
    //!param: key - Key to lookup.
    //!param: def - Default value to assign to entry if it doesn't exist.
    //!return: Reference to the new entry.
    
    inline T &AccessCopy(const K &key,const T &def = T());
    //: Access key, if it does not exist create a new bin with a copy of value 'def'
    //!param: key - Key to lookup.
    //!param: def - Default value to assign a copy of if entry doesn't exist.
    //!return: Reference to the new entry.
    
    bool Del(const K &key,bool allowResize = true);
    //: Delete member from table.
    //!param: key - Key to remove from table.
    //!param: allowResize - if true allow table resize.
    // Note, if your deleting a large fraction of the entries it is more efficient to set
    // allowResize to false, then to call the Resize() method to re-bin entries after.
    
    inline T Get(const K &key,bool allowResize = true);
    //: Get data element from table, and remove it.
    //!param: key - Element to get from the table.
    
    inline bool IsElm(const K &key) const 
    { return (Lookup(key) != 0); }
    //: Is key used in the table ?
    //!param: key - Element to check for in the table.
    //!return: true if element is present in the table, false otherwise.
    
    void Empty(void); 
    //: Remove all items from table.
    
    inline SizeT Bins(void) const
    { return table.Size(); }
    //: Number of bins in the HashTable.
    //!return: Number of bins in the hash table.
    
    void Resize(SizeT newSize);
    //: Resize hash table.
    //!param: newSize - New number of bins for the hashtable.
    
    const HashC<K,T> &operator= (const HashC<K,T> &oth) { 
      table = oth.table.Copy(); 
      elements = oth.elements;
      return *this; 
    }
    //: Assign from another hash table.
    //!param: oth - Other table
    
    bool operator==(const HashC<K,T> &oth) const;
    //: Are two hash tables identical ?
    //!param: oth - Table to compare this one with.
    //!return: true if keys and corresponding data entries are identical
    
    bool operator!=(const HashC<K,T> &oth) const
    { return !operator==(oth); }
    //: Are two hash tables different ?
    //!param: oth - Table to compare this one with.
    //!return: true if keys and corresponding data entries are different
    
    SizeT Hash() const;
    //: Compute a hash value for the hash table.
    //!return: Hash value for table key and data entries.
    
    inline void Move(HashC<K,T> &oth);
    //: Move contents of another table into this one.
    // leave other empty. The previous contents of this table are removed.
    //!param: oth - Table to move entries from.
    
    void AddFrom(HashC<K,T> &oth,bool replace = true);
    //: Add contents of another table into this one.
    // Leaves 'oth' table empty.
    //!param: oth - Table to move elements from.
    //!param: replace - If true replace elements with conflicting with ones from the 'oth' table, if false keep ones in this table.
    
    void Add(const HashC<K,T> &oth,bool replace = true);
    //: Copy contents of another table 'oth' into this one.
    //!param: oth - Table to copy elements from.
    //!param: replace - If true replace elements with conflicting keys with entries from the 'oth' table, if false keep ones in this table.
    
    bool NormaliseKey(K &value) const;
    //: Normalise an equivalent key to one used the the table.
    // This function is useful when you want to normalise the use
    // of equivalent keys (think strings.) to save memory.
    // Returns true if key exists in the table, false otherwise.
    
    typedef HashElemC<K,T> HashElem;
    typedef IntrDListC<HashElemC<K,T> > HashElemLst;
    typedef IntrDLIterC<HashElemC<K,T> > HashElemIter; // Used in Del.
    // These typedef's control the types of lists used in this class
    // and its iterator.  
    
    inline const T *Lookup(const K &Key) const;
    //: Find data matching key.
    //!deprecated: use <CODE><A HREF="LookupObconst_K_Ref_T_RefCb_const">Lookup(key,data)</A></CODE> or <CODE><A HREF="operator[]Obconst_K_RefCb">operator[](const K &key)</A></CODE>
    // Ptr == NULL, if matching key not found.
    
    inline T *Lookup(const K &key);
    //: Find data matching key.
    //!deprecated: use <CODE><A HREF="LookupObconst_K_Ref_T_RefCb_const">Lookup(key,data)</A></CODE> or <CODE><A HREF="operator[]Obconst_K_RefCb">operator[](const K &key)</A></CODE>
    // Ptr == NULL, if matching key not found.
    
    const T &First() const;
    //: Find an arbitrary data value in the hash table.
    //: Note: The name 'First' is for compatibility with other containers, which element is return is undefined
    //: The table must not be empty.

    const K &FirstKey() const;
    //: Find an arbitrary key value in the hash table.
    //: Note: The name 'First' is for compatibility with other containers, which element is return is undefined
    //: The table must not be empty.
protected:
    inline T &Add(const K &Key,const T &Data);
    // Add member to table.
    // !! Doesn't check if member already exists !!
    
    inline T &Add(const K &Key);
    // Add member created with default constructor.
    // !! Doesn't check if member already exists !!
    
    inline HashElemC<K,T> *LookupHV(const K &Value,SizeT &hashVal) const;
    inline bool Del(HashElemC<K,T> *Elem,bool allowResize = true);
    
  protected:
    HashC(const HashC<K,T> &oth,bool) 
      : HashBaseC(oth.elements),
	table(oth.table)
    {}
    //: Make temporary handle.
    
    HashC(const SArray1dC<HashElemLst >  &tab,SizeT nelements)
      : HashBaseC(nelements),
        table(tab)
    {}
    //: Create new table from an array.
    
#if HASHC_DEBUG
    SizeT Count() const;
    //: Do an actual count of elements in the table.
    // Used in debug only.
#endif
    
    void CheckAdd(void) {
#if HASHC_DEBUG
      RavlAssert(elements == Count());
#endif
      if(++elements > (Bins() << 1))
	Resize(NextPrime(elements << 2));
    }
    //: Need to increase size ?
    
    void CheckDel(bool allowResize = true) {
      elements--;
#if HASHC_DEBUG
      RavlAssert(elements == Count());
#endif
      if(allowResize) {
	if(elements < (Bins() >> 3)
	   && elements >= (7 << 1))
	  Resize(NextPrime(elements >> 1));
      }
    }
    //: Need to decrease size ?
    
    SArray1dC<IntrDListC<HashElemC<K,T> > > table; // Table of lists.

    friend class HashIterC<K,T>;
  };
  
  ///////////////////////////////////////
  //! userlevel=Develop
  // Element in hash table.
  //  K - Key class.
  //  T - Data class.
  
  template<class K,class T>
  class HashElemC 
    : public DLinkC
  {
  public:
    HashElemC() 
    {}
    //: Default constructor.
    // To keep the compiler happy.
    
    inline HashElemC(std::istream &in);
    //: Load from stream.
    
    inline HashElemC(BinIStreamC &in);
    //: Load from stream.
    
    HashElemC(const K &nKey,const T &Data) 
      : Key(nKey), 
	Hold(Data) 
    {}
    //: Constructor.
    
    HashElemC(const K &nKey,SizeT nHashVal,const T &Data)
      : hashVal(nHashVal), 
        Key(nKey), 
        Hold(Data) 
    {}
    //: Constructor.
    
    const T &Data() const 
    { return Hold; }
    //: Get constant reference to data.
    
    T &Data() { return Hold; }
    //: Get reference to data.
    
    T GetData() { return Hold; }
    //: Get copy of data.
    
    const K &GetKey() const { return Key; }
    //: Get value key.
    
    bool operator==(const K &Another) const 
    { return HashIsEqual(Key,Another); }
    //: Does key equal that of this element ?
    
    SizeT GetHashVal() const
    { return hashVal; }
    //: Get hash value.
    
    void HashVal(SizeT nHashVal)
    { hashVal = nHashVal; }
    //: Get hash value.
    
  protected:
    SizeT hashVal;
    K Key;
    T Hold;
    
    friend class HashC<K,T>;
  };
  
  //////////////////////////////////////////////////
  
  template<class K,class T>
  inline HashElemC<K,T>::HashElemC(std::istream &in) { 
    in >> Key >> Hold;
    hashVal = StdHash(Key); 
  }
  
  template<class K,class T>
  inline HashElemC<K,T>::HashElemC(BinIStreamC &in) { 
    in >> Key >> Hold;
    hashVal = StdHash(Key); 
  }

  template<class K,class T>
  std::ostream &operator<<(std::ostream &out,const HashElemC<K,T> &obj) {  
    out << obj.GetKey() << ' ' << obj.Data(); 
    return out;
  }
  
  template<class K,class T>
  std::istream &operator>>(std::istream &in,HashElemC<K,T> &obj) { 
    obj = HashElemC<K,T>(in);
    return in;
  }

  template<class K,class T>
  BinOStreamC &operator<<(BinOStreamC &out,const HashElemC<K,T> &obj) {  
    out << obj.GetKey() << obj.Data(); 
    return out;
  }
  
  template<class K,class T>
  BinIStreamC &operator>>(BinIStreamC &in,HashElemC<K,T> &obj) { 
    obj = HashElemC<K,T>(in);
    return in;
  }
  
  ///////////////////////////////////////////////////////////////////

  template<class K,class T>
  HashC<K,T>::HashC(const HashC<K,T> &oth,UIntT levels) 
    : HashBaseC(oth.elements)
  {
    RavlAssertMsg(levels != 0,"HashC is not reference counted, level 0 copy is impossible. ");
    switch(levels) {
    case 1: table = oth.table.Copy(); break;
    case 2: {
      table = SArray1dC<IntrDListC<HashElemC<K,T> > >(NextPrime(oth.elements));
      for(BufferAccessIterC<IntrDListC<HashElemC<K,T> > > it(oth.table);it;it++) {
	for(IntrDLIterC<HashElemC<K,T> > place(*it);place;place++) {
	  const SizeT hashVal = place->GetHashVal();
	  const SizeT ind = hashVal % table.Size();
	  table[ind].InsFirst(*new HashElem(place->GetKey(),
					    hashVal,
					    StdCopy(place->GetData())));
	}
      } 
    } break;
    default: {
      levels--;
      table = SArray1dC<IntrDListC<HashElemC<K,T> > >(NextPrime(oth.elements));
      for(BufferAccessIterC<IntrDListC<HashElemC<K,T> > > it(oth.table);it;it++) {
        for(IntrDLIterC<HashElemC<K,T> > place(*it);place;place++) {
          const SizeT hashVal = place->GetHashVal();
          const SizeT ind = hashVal % table.Size();
          table[ind].InsFirst(*new HashElem(place->GetKey(),
                    hashVal,
                    StdDeepCopy(place->GetData(),levels)));
        }
	
      }
    } break;
    }
  }
  
  template<class K,class T>
  HashC<K,T>::HashC(const Tuple2C<K,T> *data)
    : table(7)
  {
    const K &firstKey = data->Data1();
    Insert(data->Data1(),data->Data2());
    data++;
    for(;data->Data1() != firstKey;data++)
      Insert(data->Data1(),data->Data2());
  }

  template<class K,class T>
  void HashC<K,T>::Save(std::ostream &out) const {
    out << elements << '\n';
    for(BufferAccessIterC<IntrDListC<HashElemC<K,T> > > it(table);it;it++) {
      for(IntrDLIterC<HashElemC<K,T> > place(*it);place;place++)
        out << *place << '\n';
    }    
  }
  
  template<class K,class T>
  void HashC<K,T>::Save(BinOStreamC &out) const {
    UIntT saveElements = elements;
    out << saveElements;
    for(BufferAccessIterC<IntrDListC<HashElemC<K,T> > > it(table);it;it++) {
      for(IntrDLIterC<HashElemC<K,T> > place(*it);place;place++)
        out << *place;
    }    
  }
    

  template<class K,class T>
  inline std::ostream &operator<<(std::ostream &out,const HashC<K,T> &obj) 
  { obj.Save(out);  return out;  }
  
  
  template<class K,class T>
  inline std::istream &operator>>(std::istream &in,HashC<K,T> &obj) { 
    HashC<K,T> newun(in);
    obj.Move(newun);
    return in;
  }
  
  template<class K,class T>
  inline BinOStreamC &operator<<(BinOStreamC &out,const HashC<K,T> &obj) 
  { obj.Save(out); return out; }
  
  
  template<class K,class T>
  BinIStreamC &operator>>(BinIStreamC &in,HashC<K,T> &obj) { 
    HashC<K,T> newun(in);
    obj.Move(newun);
    return in;
  }
  
  template<class K,class T>
  HashC<K,T>::HashC(std::istream &in)  {
    SizeT size;
    in >> size;
    table = SArray1dC<IntrDListC<HashElemC<K,T> > > (NextPrime(size));
    for(;size > 0;size--) {
      HashElemC<K,T> t(in);
      Add(t.GetKey(),t.Data());
    }
  }
  
  template<class K,class T>
  HashC<K,T>::HashC(BinIStreamC &in)  {
    UIntT size; // FIXME:- Should be SizeT.
    in >> size;
    table = SArray1dC<IntrDListC<HashElemC<K,T> > > (NextPrime(size));
    for(;size > 0;size--) {
      HashElemC<K,T> t(in);
      Add(t.GetKey(),t.Data());
    }
  }
  
  template<class K,class T>
  HashC<K,T> HashC<K,T>::Copy() const  
  { return *this; }
  
  template<class K,class T>
  inline HashElemC<K,T> *HashC<K,T>::LookupHV(const K &value,SizeT &hashVal) const {
    hashVal = StdHash(value);
    IntrDLIterC<HashElemC<K,T> > place(table[hashVal % table.Size()]);
    for(;place;place++) {
      if(place.Data() == value)
	return (HashElemC<K,T> *) &place.Data();
    }
    return 0;
  }
  
  template<class K,class T>
  inline const T *HashC<K,T>::Lookup(const K &Key) const  {
    SizeT hashVal;
    HashElemC<K,T> *elem = LookupHV(Key,hashVal);
    if(elem == 0) 
      return 0;
    return &elem->Data();
  }
  
  template<class K,class T>
  inline T *HashC<K,T>::Lookup(const K &key)  {
    SizeT hashVal;
    HashElemC<K,T> *elem = LookupHV(key,hashVal);
    if(elem == 0) return 0;
    return &elem->Data();
  }
  
  template<class K,class T>
  inline 
  bool HashC<K,T>::Lookup(const K &Key,T &data) const{
    SizeT hashVal;
    HashElemC<K,T> *elem = LookupHV(Key,hashVal);
    if(elem == 0) 
      return false;
    data = elem->Data();  
    return true;
  }
  
  template<class K,class T>
  inline 
  const T &HashC<K,T>::operator[](const K &key) const {
#if RAVL_CHECK
    const T *data = Lookup(key);
    RavlAssert(data != 0);
    return *data;
#else
    return *Lookup(key);
#endif
  }
  
  
  template<class K,class T>
  inline 
  bool HashC<K,T>::Del(HashElemC<K,T> *elem,bool allowResize) {
    delete elem;
    CheckDel(allowResize);
    return true;
  }
  
  template<class K,class T>
  inline T &HashC<K,T>::Add(const K &key,const T &data)  {
    CheckAdd();
    const SizeT hashVal = StdHash(key);
    const SizeT ind = hashVal % table.Size();
    table[ind].InsFirst(*new HashElem(key,hashVal,data));
    return table[ind].First().Data();
  }
  
  template<class K,class T>
  inline T &HashC<K,T>::Add(const K &key)  {
    CheckAdd();
    const SizeT hashVal = StdHash(key);
    const SizeT ind = hashVal % table.Size();
    table[ind].InsFirst(*new HashElem(key,hashVal,T()));
    return table[ind].First().Data();
  }
  
  template<class K,class T>
  inline bool HashC<K,T>::Update(const K &key,const T &data) {
    SizeT hashVal;
    HashElemC<K,T> *elem = LookupHV(key,hashVal);
    if(elem != 0) {
      elem->Data() = data;
      return true; // There already.
    }
    CheckAdd();
    const SizeT ind = hashVal % table.Size();
    table[ind].InsFirst(*new HashElem(key,hashVal,data));
    return false;  // Had to be added.
  }
  
  template<class K,class T>
  inline T &HashC<K,T>::Access(const K &key,const T &def) {
    SizeT hashVal;
    HashElemC<K,T> *elem = LookupHV(key,hashVal);
    if(elem != 0) 
      return elem->Data(); // Exists already.
    CheckAdd();
    const SizeT ind = hashVal % table.Size();
    HashElem &v = *new HashElem(key,hashVal,def);
    table[ind].InsFirst(v);
    return v.Data(); // Had to be added.  
  }
  
  template<class K,class T>
  inline T &HashC<K,T>::AccessCopy(const K &key,const T &def) {
    SizeT hashVal;
    HashElemC<K,T> *elem = LookupHV(key,hashVal);
    if(elem != 0) 
      return elem->Data(); // Exists already.
    CheckAdd();
    const SizeT ind = hashVal % table.Size();
    HashElem &v = *new HashElem(key,hashVal,StdCopy(def));
    table[ind].InsFirst(v);
    return v.Data(); // Had to be added.  
  }
  
  template<class K,class T>
  inline T &HashC<K,T>::Update(const K &key) {
    SizeT hashVal;
    HashElemC<K,T> *elem = LookupHV(key,hashVal);
    if(elem != 0)
      return elem->Data();
    CheckAdd();
    const SizeT ind = hashVal % table.Size();
    HashElem &v = *new HashElem(key,hashVal,T());
    table[ind].InsFirst(v);
    return v.Data(); 
  }
  
  template<class K,class T>
  bool HashC<K,T>::Del(const K &key,bool allowResize) {
    SizeT hashVal = StdHash(key);
    HashElemIter place(table[hashVal % table.Size()]);
    for(;place;place++) {
      if(place.Data() == key) {
	place.Del();
	CheckDel(allowResize);
	return true;
      }
    }
    return false;
  }
  
  template<class K,class T>
  inline T HashC<K,T>::Get(const K &Key,bool allowResize) {
    SizeT hashVal = StdHash(Key);
    HashElemIter place(table[hashVal % table.Size()]);
    for(;place;place++) {
      if(*place == Key) {
	T ret(place->Data());
	place.Del();
	CheckDel(allowResize);
	return ret;
      }
    }
    return T();
  }
  
  template<class K,class T>
  void HashC<K,T>::Empty(void) {
    for(BufferAccessIterC<HashElemLst> it(table);it;it++) 
      it->Empty();
    elements = 0;
  }
  
  template<class K,class T>
  void HashC<K,T>::Resize(SizeT newSize) {
    if(newSize == table.Size()) return;
    RavlAssert(newSize > 0);
    SArray1dC<HashElemLst> newTable(newSize);
    for(BufferAccessIterC<HashElemLst> it(table);it;it++) {
      while(!it->IsEmpty()) {
	const SizeT hashVal = it->First().GetHashVal();
	newTable[hashVal % newSize].InsFirst(it->PopFirst());
      }
    }
    table = newTable;
  }
  
#if HASHC_DEBUG
  template<class K,class T>
  SizeT HashC<K,T>::Count() const {
    SizeT ret = 0;
    for(BufferAccessIterC<HashElemLst> it(table);it;it++) 
      ret += it->Size();
    return ret;
  }
#endif
  
  template<class K,class T>
  bool HashC<K,T>::operator==(const HashC<K,T> &oth) const {
    // Are two hash tables identical ?
    if(&oth == this)
      return true;
    if(elements != oth.elements)
      return false;
    const T *dat;
    for(BufferAccessIterC<IntrDListC<HashElemC<K,T> > > it(oth.table);it;it++) {
      for(IntrDLIterC<HashElemC<K,T> > place(*it);place;place++) {
	if((dat = Lookup(place->GetKey())) == 0)
	  return false;
	if(*dat != place->Data())
	  return false;
      }
    }
    return true;
  }
  
  template<class K,class T>
  SizeT HashC<K,T>::Hash() const {
    // Compute a hash value for the hash table.
    SizeT ret = elements + (elements << 17);
    for(BufferAccessIterC<IntrDListC<HashElemC<K,T> > > it(table);it;it++) {
      for(IntrDLIterC<HashElemC<K,T> > place(*it);place;place++) {
        size_t hv = StdHash(place->Data());
	ret += (StdHash(place->GetKey()) + hv) ^ (hv << 11);
      }
    }
    return ret;
  }
  
  template<class K,class T>
  inline 
  void HashC<K,T>::Move(HashC<K,T> &oth) {
    // Get the contents from the other...
    table = oth.table;
    elements = oth.elements;
    // Then set it to empty.
    oth.table = SArray1dC<IntrDListC<HashElemC<K,T> > >(7);
    oth.elements = 0;
  }
  
  template<class K,class T>
  void HashC<K,T>::AddFrom(HashC<K,T> &oth,bool replace) {
    for(BufferAccessIterC<HashElemLst> it(oth.table);it;it++) {
      while(!it->IsEmpty()) {
	HashElemC<K,T> &org =it->First();
	bool found = false;
	// Check the item isn't already in the table.
	IntrDListC<HashElemC<K,T> > &tabEntry = table[org.GetHashVal() % table.Size()]; 
	for(IntrDLIterC<HashElemC<K,T> > place(tabEntry);place;place++) {
	  if(place.Data().GetKey() != org.GetKey()) 
	    continue;
	  if(replace)
	    place.Data().Data() = org.Data();
	  place.Del();
	  found = true;
	  break;
	}
	
	// If its not in, added it.
	if(!found) {
	  tabEntry.InsFirst(it->PopFirst());
	  elements++;
	}
      }
    }
    elements--; // Check add will increment back to proper value.
    CheckAdd();
    oth.elements = 0; // The other table's now empty.
  }
  
  template<class K,class T>
  void HashC<K,T>::Add(const HashC<K,T> &oth,bool replace) {
    for(BufferAccessIterC<HashElemLst> it(oth.table);it;it++) {
      for(IntrDLIterC<HashElemC<K,T> > place(*it);place;place++) {
        HashElemC<K,T> &org = *place;
        IntrDListC<HashElemC<K,T> > &tabEntry = table[org.GetHashVal() % table.Size()];
        bool found = false;
        for(IntrDLIterC<HashElemC<K,T> > place(tabEntry);place;place++) {
          if(place.Data().GetKey() != org.GetKey())
            continue;
          if(replace)
            place.Data().Data() = org.Data();
          found = true;
          break;
        }
        if(!found) { // Add in a copy.
          tabEntry.InsFirst(*new HashElemC<K,T>(org));
          elements++;
        }
      }
    }
    elements--; // Check add will increment back to proper value.
    CheckAdd();
  }
  
  template<class K,class T>
  bool HashC<K,T>::NormaliseKey(K &value) const {
    SizeT hashVal;
    HashElemC<K,T> *elem = LookupHV(value,hashVal);
    if(elem == 0) return false;
    value = elem->GetKey();
    return true;
  }

  template<class K,class T>
  const T &HashC<K,T>::First() const {
    for(BufferAccessIterC<HashElemLst> it(table);it;it++) {
      IntrDLIterC<HashElemC<K,T> > place(*it);
      if(place.IsElm())
        return place.Data().Data();
    }
    throw RavlN::ExceptionOperationFailedC("No element found in table.");
  }

  template<class K,class T>
  const K &HashC<K,T>::FirstKey() const {
    for(BufferAccessIterC<HashElemLst> it(table);it;it++) {
      IntrDLIterC<HashElemC<K,T> > place(*it);
      if(place.IsElm())
        return place.Data().GetKey();
    }
    throw RavlN::ExceptionOperationFailedC("No element found in table.");
  }

}

#endif

