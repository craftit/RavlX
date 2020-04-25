// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! lib=RavlCore
//! file="Ravl/Core/Container/Hash/testHash.cc"
//! docentry="Ravl.API.Core.Hash Tables"
//! userlevel=Develop

#include "Ravl/Hash.hh"
#include "Ravl/RCHash.hh"
#include "Ravl/BiHash.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/String.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/Stream.hh"
#include <stdlib.h> // rand()

using namespace RavlN;

int testHashMisc();
int Hashtest();
int HashCopytest();
int HashAddFrom();
int testHashMisc();

int main() {  
  int ret;
  if((ret = Hashtest()) != 0) {
    std::cerr << "Test failed at :" << ret<< "\n";
    return 1;
  }
  if((ret = HashCopytest()) != 0) {
    std::cerr << "Test failed at :" << ret<< "\n";
    return 1;    
  }  
  if((ret = HashAddFrom()) != 0) {
    std::cerr << "Test failed at :" << ret<< "\n";
    return 1;    
  }  
  if((ret = testHashMisc()) != 0) {
    std::cerr << "Test failed at :" << ret<< "\n";
    return 1;    
  }  
  return 0;
}

template<class T>
int CountEntries(const HashC<T,T> &tab) {
  int ret = 0;
  for(HashIterC<T,T> it(tab);it;it++)
    ret++;
  if((int) tab.Size() != ret) {
    std::cerr << "CountEntries(), Found size mismatch. \n"; 
    return -1;
  }
  return ret;
}

int Hashtest() {
  std::cerr << "------Starting Hash consistancy check.------ \n";
  int n = 0;
  {
    HashC<int,int> Fwd;
    IntT t = 0;
    if(Fwd.NormaliseKey(t)) return __LINE__;
    
    HashC<int,int> Bkw;
    if(!Fwd.IsEmpty()) return __LINE__;
    int Start = -1,Total = 0;
    int Place,Next;
    for(int k = 0;k < 2000;k += 10) {
      for(int i = 0;i < 10;i++) {
	int Num;
	do {
	  Num = rand();
	} while(Fwd.Lookup(Num) != 0) ;
	Fwd[Num] = Start;
	Bkw[Start] = Num;
	Start = Num;
	Total++;
      }
      int Count = 0;
      for(Place = Start;Place  != -1;Place = *Fwd.Lookup(Place)) {
	int *aNext = Fwd.Lookup(Place);
	if(aNext == NULL) return __LINE__;
	int *Last = Bkw.Lookup(*aNext);
	if(Last == NULL) return __LINE__;
	if(*Last != Place) return __LINE__;
	Last = NULL;
	Count++;
      }
      if(Total != Count) return __LINE__;
    }
    int Sub = 0;
    for(Place = Start,Next = Start;Place != -1;Place = Next) {
      int *APtr = Fwd.Lookup(Place);
      if(APtr == NULL) return __LINE__;
      Next = *APtr;
      if((rand() % 2) == 0) {
	Fwd.Del(Place);
	if(Fwd.Lookup(Place) != NULL) return __LINE__;
	Sub++;
      } else {
	int *TI,AVal = rand();
	Fwd.Update(Place,AVal);
	TI = Fwd.Lookup(Place);
	if(TI == NULL) return __LINE__;
	if(TI == NULL) return __LINE__;
	if(*TI != AVal) return __LINE__;
      }
    }
    HashIterC<int,int> Iter(Fwd);
    int ACount = 0;
    for(;Iter.IsElm();Iter.Next()) 
      ACount++;
    if(ACount != (Total-Sub)) return __LINE__;
    n += Total;
  }
  std::cerr << "Starting Hash Iterator test. \n";
  {
    HashC<int,int> HTab;
    int Total = 0;
    for(int k = 0;k < 800;k += 10) {
      for(int i = 0;i < k;i++) {	
	int Val = rand();
	while (HTab.IsElm(Val)) 
	  Val = rand();
	HTab[Val] = Val;
	Total++;
      }
      int Count = 0;
      for(HashIterC<int,int> Iter(HTab);Iter.IsElm();Iter.Next()) {
	if(Iter.Data() != (int) Iter.Key()) return __LINE__;
	Count++;
      }
      if((int) HTab.Size() != Count) return __LINE__;
      if(CountEntries(HTab) != Count) return __LINE__;
      if(Count != Total) return __LINE__;
    }
  }
  std::cerr << "------ Hash Passed. " << n << " ops tested. ------ \n";
  return 0;
}


int HashCopytest() {
  std::cerr << "--------- Doing copy test. \n";
  for(int k = 0;k < 100;k += 5) {
    //cerr << "Loop : " << k << "\n";
    HashC<int,int> someData;
    for(int i = 0;i < k;i++)
      someData[i] = i;
    HashC<int,int> moreData(someData);
    if(moreData.Size() != someData.Size())
      return __LINE__;
    int items = 0;
    for(HashIterC<int,int> it(moreData);it.IsElm();it.Next()) {
      items++;
      if(someData[it.Key()] != it.Data()) {
	cerr << "Test Failed 1. \n";
	return __LINE__;
      }
      if(someData[it.Key()] != moreData[it.Key()]) {
	cerr << "Test Failed 2. \n";
	return __LINE__;
      }
      if(it.Key() != it.Data()) {
	cerr << "Test Failed 3. \n";
	return __LINE__;
      }
    }
    if((UIntT) items != someData.Size()) {
      std::cerr << "Size mismatch. Items:" << items << " Size:" << someData.Size() << "\n";
      return __LINE__;
    }
  }
  std::cerr << "--------- Copy test passed. \n";
  return 0;
}

int HashAddFrom() {
  std::cerr << "--------- Add from test. \n";
  HashC<StringC,StringC> tab1;
  tab1["Var1"] = "hello1";
  tab1["Var2"] = "hello2";
  if(tab1.Size() != 2) return __LINE__;
  if(CountEntries(tab1) != 2) return __LINE__;
  HashC<StringC,StringC> tab2;
  tab2["Var3"] = "hello3";
  tab2["Var4"] = "hello4";
  if(tab2.Size() != 2) return __LINE__;
  if(CountEntries(tab2) != 2) return __LINE__;
  tab1.AddFrom(tab2);
  if(tab1.Size() != 4) return __LINE__;
  if(CountEntries(tab1) != 4) return __LINE__;
  if(tab2.Size() != 0) return __LINE__;
  if(CountEntries(tab2) != 0) return __LINE__;
  if(tab2["Var3"] == "hello3") return __LINE__;
  if(tab1["Var1"] != "hello1") return __LINE__;
  if(tab1["Var4"] != "hello4") return __LINE__;
  
  HashC<StringC,StringC> tab3(tab1);
  if(CountEntries(tab3) != 4) return __LINE__;
  if(tab3.Size() != 4) return __LINE__;
  tab1.Del("Var3");
  if(tab3["Var3"] != "hello3") return __LINE__;
  if(tab1.IsElm("Var3")) return __LINE__;
  if(tab1.Size() != 3) return __LINE__;
  if(CountEntries(tab1) != 3) return __LINE__;
  
  std::cerr << "--------- Add from test passed. \n";
  return 0;
}

int testHashMisc() {
  std::cerr << "Check handling of empty hash tables. \n";
  HashC<int,int> test;
  // Check lookup in a empty copied table.
  HashC<int,int> test2(test);
  int v;
  if(test2.Lookup(1,v)) return __LINE__;
  
  // Check handling binary IO of empty tables.
  {
    StrOStreamC ostr;
    BinOStreamC bo(ostr);
    bo << test2;
    StrIStreamC istr(ostr.String());
    BinIStreamC bi(istr);
    bi >> test;
    if(test.Lookup(1,v)) return __LINE__;
  }
  
  {
    StrOStreamC ostr;
    ostr << test2;
    StrIStreamC istr(ostr.String());
    istr >> test;
    if(test.Lookup(1,v)) return __LINE__;
  }
  
#if 0
  HashC<StringC,StringC> tab;
  StringC s1("hello");
  std::cerr << "S1="<< (void *) &(s1[0]) << "\n";
  StringC s2("hello");
  StringC s3("hello");
  std::cerr << "S2="<< (void *) &(s2[0]) << "\n";
  tab[s1] = s1;
  tab.NormaliseKey(s2);
  std::cerr << "N S2="<< (void *) &(s2[0]) << "\n";
  std::cerr << "S3="<< (void *) &(s3[0]) << "\n";
  tab.NormaliseKey(s3);
  std::cerr << "N S3="<< (void *) &(s3[0]) << "\n";
#endif
  
  return 0;
}

namespace RavlN {
  template
  class BiHashC<int, UIntT>;

  template
  class HashC<int, UIntT>;

  template
  class HashIterC<int, UIntT>;
}