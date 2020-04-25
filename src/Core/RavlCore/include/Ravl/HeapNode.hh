// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef HEAPNODE_HEADER
#define HEAPNODE_HEADER 1
/////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Queue/HeapNode.hh"
//! lib=RavlCore
//! userlevel=Develop
//! date="28/10/96"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Queues"
//! rcsid="$Id$"

#include "Ravl/Tuple2.hh"

namespace RavlN {
  //: Heap node in pointer based priority queue.
  
  template <class K,class D>
  class HeapNodeC {
  public:
    HeapNodeC();
    //: Default constructor.
    
    HeapNodeC(Tuple2C<K,D> &DataP);
    //: Construct from a keyed pair.
    
    ~HeapNodeC();
    //: Destructor.
    
    D &Data(void) { return DataP->Data2(); }
    //: Access data.
    
    const D &Data(void) const { return DataP->Data2(); }
    //: Constant access to data.
    
    K &Key(void) { return DataP->Data1(); }
    //: Access key.
    
    const K &Key(void) const { return DataP->Data1(); }
    //: Constant access to key.
    
    Tuple2C<K,D> &GetPair(void) { return *DataP; }
    //: Access key pair.
    
    const Tuple2C<K,D> &GetPair(void) const { return *DataP; }
    //: Constant access to key pair.
    
    void SetPair(Tuple2C<K,D> &Pair) { DataP = &Pair; }
    //: Set keyed pair.
    
    void SetPairNULL(void) { DataP = 0; }
    //: Set keyed pair to NULL.
    
    void MvPair(HeapNodeC<K,D> &Oth) { DataP = Oth.DataP; Oth.DataP = 0; }
    //: Move pair from another node.
    
    int GetBalance(void) const { return Balance; }
    //: Get balance for node.
    
    void SetBalance(int Val) { Balance = Val; }
    //: Set balance for node.
    
    void DeltaBalance(int Delta) { Balance += Delta; }
    //: Change balance for node.
    
    SizeT Size(void) const;
    //: Get number of nodes below this one.
    
    HeapNodeC<K,D> *&Child(int n) { return ChildP[n]; }
    //: Access child ptrs.
    
  protected:
    Tuple2C<K,D> *DataP;
    HeapNodeC<K,D> *ChildP[2];
    int Balance;
  };
  
  ///////////////////////////////
  // Constructors.
  
  template <class K,class D>
  HeapNodeC<K,D>::HeapNodeC() 
    : DataP(0),
      Balance(0)
  {
    ChildP[0] = 0;
    ChildP[1] = 0;
  }
  
  template <class K,class D>
  HeapNodeC<K,D>::HeapNodeC(Tuple2C<K,D> &DataP) 
    : DataP(&DataP),
      Balance(0)
  {
    ChildP[0] = 0;
    ChildP[1] = 0;
  }
  
  ///////////////////////////
  // Destructor.
  
  template <class K,class D>
  HeapNodeC<K,D>::~HeapNodeC() {
    if(DataP != 0)
      delete DataP;
    if(ChildP[0] != 0)
      delete ChildP[0];
    if(ChildP[1] != 0)
      delete ChildP[1];
  }
  
  ///////////////////////
  // Get number of nodes below this one.
  
  template <class K,class D>
  SizeT HeapNodeC<K,D>::Size(void) const {
    SizeT Ret = 1;
    if(ChildP[0] != 0)
      Ret += ChildP[0]->Size();
    if(ChildP[1] != 0)
      Ret += ChildP[1]->Size();
    return Ret;
  }
}

#endif
