// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/DList/testDList.cc"
//! userlevel=Develop
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Lists"

#include "Ravl/IntrDList.hh"
#include "Ravl/IntrDLIter.hh"
#include "Ravl/DList.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/BinStream.hh"
#include "Ravl/Stream.hh"
#include <stddef.h>

using namespace RavlN;

class StuffC
  : public DLinkC
{
public:
  StuffC()
    {}
  //: Default constructor.

  StuffC(int nv)
    : v(nv)
    {}
  //: Constructor.
  
  int v;
};

#if RAVL_HAVE_TEMPLATEINSTANTIATE
template class DListC<IntT>;
template class DLIterC<IntT>;
template IntrDListC<StuffC>; // Force instanciation of all methods.
template IntrDLIterC<StuffC>; // Force instanciation of all methods.
#endif

int testDListBinIO();
int testIntrDList();
int testDLIter();
int testDoubleLinked();
int testSort();

int main(int nargs,char *argv[])
{
  int lineno;
  if((lineno = testIntrDList()) != 0) {
    std::cerr << "testIntrDListC test failed on line "<< lineno << "\n";
    return 1;
  }
  if((lineno = testDLIter()) != 0) {
    std::cerr << "testIntrDListC test failed on line "<< lineno << "\n";
    return 1;
  }
  if((lineno = testDListBinIO()) != 0) {
    std::cerr << "testDListIO test failed on line "<< lineno << "\n";
    return 1;
  }
  if((lineno = testDoubleLinked()) != 0) {
    std::cerr << "testDoubleLinked test failed on line "<< lineno << "\n";
    return 1;
  }
  if((lineno = testSort()) != 0) {
    std::cerr << "testSort test failed on line "<< lineno << "\n";
    return 1;
  }
  std::cerr << "Test passed ok. \n";
  return 0;
}

int testIntrDList() {
  IntrDListC<StuffC> lst;
  if(!lst.IsEmpty()) return __LINE__;
  int i;
  for(i = 0;i < 10;i++)
    lst.InsLast(*new StuffC(i));
  i = 0;
  if(lst.IsEmpty()) return __LINE__;
  for(IntrDLIterC<StuffC> it(lst);it;it++,i++) {
    if(it->v != i)
      return __LINE__;
  }
    
  IntrDListC<StuffC> lstCopy(lst);
  if(lstCopy.IsEmpty()) return __LINE__;
  lst.Empty();
  if(!lst.IsEmpty()) return __LINE__;
  if(lstCopy.IsEmpty()) return __LINE__;
  i = 0;
  for(IntrDLIterC<StuffC> it(lstCopy);it;it++,i++) {
    if(it->v != i)
      return __LINE__;
  }
  IntrDListC<StuffC> lstCopy2;
  lstCopy2 = lstCopy;
  if(!lst.IsEmpty()) return __LINE__;
  if(lstCopy.IsEmpty()) return __LINE__;
  if(lstCopy2.IsEmpty()) return __LINE__;
  i = 0;
  for(IntrDLIterC<StuffC> it(lstCopy2);it;it++,i++) {
    if(it->v != i)
      return __LINE__;
  }
  
  return 0;
}

int testDListBinIO() {
  DListC<IntT> val;
  val.InsLast(1);
  val.InsLast(2);
  
  StrOStreamC out;
  BinOStreamC bo(out);
  
  bo << val;
  StrIStreamC in(out.String());
  BinIStreamC bi(in);
  DListC<IntT> back;
  bi >> back;
  if(back.Size() != val.Size()) return __LINE__;
  if(back.First() != val.First()) return __LINE__;
  if(back.Last() != val.Last()) return __LINE__;
  
  return 0;
}

int testDLIter() {
  DListC<IntT> list;
  for(int i = 0;i < 10;i++)
    list.InsLast(i);
  if(list.Hash() != 455) return __LINE__;
  DLIterC<IntT> it(list);
  if(!it) return __LINE__;
  if((*it) != 0) return __LINE__;
  it++;
  if(!it) return __LINE__;
  if((*it) != 1) return __LINE__;
  DLIterC<IntT> ita = it;
  it++;
  if(!it) return __LINE__;
  if((*it) != 2) return __LINE__;
  it++;
  if(!it) return __LINE__;
  if((*it) != 3) return __LINE__;
  DLIterC<IntT> itb = it;
  it++;
  if(!it) return __LINE__;
  if((*it) != 4) return __LINE__;
  it++;
  if(!it) return __LINE__;
  if((*it) != 5) return __LINE__;
  
  DListC<IntT> incHead = ita.Head();
  DListC<IntT> incTail = itb.InclusiveTail();
#if 0
  std::cerr << "IncHead=" << incHead << "\n";
  std::cerr << "IncTail=" << incTail << "\n";
  std::cerr << "list=" << list << "\n";
#endif
  if(list.Size() != 2) return __LINE__;
  if(incTail.Size() != 7) return __LINE__;
  if(incHead.Size() != 1) return __LINE__;
  
  DLIterC<IntT> itc = list;
  DLIterC<IntT> itd = itc;
  itd++;
  DListC<IntT> xyz = itc.Head();
  if(xyz.Size() != 0) return __LINE__;
  if(list.Size() != 2) return __LINE__;
  DListC<IntT> la = itc.Tail();
  //cerr << "la=" << la << " itc=" << *itc << "\n";
  if(la.Size() != 1) return __LINE__;
  if(la.First() != 2) return __LINE__;
  // Check taking the head and tail of an empty list works.
  
#if 0
  DListC<IntT> emptyList;
  DLIterC<IntT> itz(emptyList);
  itz.Tail();
  itz.Head();
#endif
  
  {
    DListC<IntT> a;
    a.InsLast(1);
    a.InsLast(2);
    a.InsLast(3);
    DListC<IntT> b;
    b.InsLast(10);
    b.InsLast(11);
    DListC<IntT> c;
    c.InsLast(20);
    c.InsLast(21);

    DLIterC<IntT> ita1(a);
    ita1++;
    if(!ita1.IsElm()) return __LINE__;
    ita1.MoveListAfter(b);
    //cerr << "b=" << b << "\n";
    //cerr << "a=" << a << "\n";
    if(!b.IsEmpty()) return __LINE__;
    if(a.Size() != 5) return __LINE__;
    
    ita1.MoveListBefore(c);
    //cerr << "c=" << b << "\n";
    //cerr << "a=" << a << "\n";
    if(!c.IsEmpty()) return __LINE__;
    if(a.Size() != 7) return __LINE__;
    
  }
  return 0;
}


struct DoubleLinkedC 
{
  
public:
  DoubleLinkedC(int n = 0)
    : m_data(n)
  {}
  
  DLinkC m_link1;
  DLinkC m_link2;
  IntT m_data;
  
  typedef IntrDListOffsetDeRefC<DoubleLinkedC,sizeof(DLinkC) * 0> Ring1T;
  typedef IntrDListOffsetDeRefC<DoubleLinkedC,sizeof(DLinkC) * 1> Ring2T;
};


int testDoubleLinked() {
  //cerr << "Offset1=" << offsetof(DoubleLinkedC,m_link1) << "\n";
  //cerr << "Offset2=" << offsetof(DoubleLinkedC,m_link2) << "\n";
  //cerr << "size=" << sizeof(DLinkC) << "\n";
  typedef IntrDListC<DoubleLinkedC,DoubleLinkedC::Ring1T> ListRing1T;
  typedef IntrDListC<DoubleLinkedC,DoubleLinkedC::Ring2T> ListRing2T;
  typedef IntrDLIterC<DoubleLinkedC,DoubleLinkedC::Ring1T> IterRing1T;
  typedef IntrDLIterC<DoubleLinkedC,DoubleLinkedC::Ring2T> IterRing2T;
  
  ListRing1T list1(false);
  ListRing2T list2(false);
  
  DoubleLinkedC listElements[20];
  
  IterRing2T it(list2);
  if(it) return __LINE__;
  list2.InsLast(listElements[0]);
  it = list2;
  if(!it) return __LINE__;
  list2.InsLast(listElements[1]);
  list2.InsLast(listElements[2]);
  int count = 0;
  for(IterRing2T it2(list2);it2;it2++)
    count++;
  std::cerr << "Count=" << count <<"\n";
  if(count != 3) return __LINE__;
  
  for(int i = 0;i < 20;i++)
    list1.InsLast(listElements[i]);
  
  count = 0;
  for(IterRing1T it2(list1);it2;it2++)
    count++;
  std::cerr << "Count=" << count <<"\n";
  if(count != 20) return __LINE__;
  count = 0;
  for(IterRing2T it2(list2);it2;it2++)
    count++;
  std::cerr << "Count=" << count <<"\n";
  if(count != 3) return __LINE__;
  return 0;
}

int testSort() {
  DListC<int> list;
  list.InsLast(3);
  list.InsLast(1);
  list.InsLast(2);
  list.MergeSort();
  //std::cerr << list;
  if(list.Nth(0) != 1) return __LINE__;
  if(list.Nth(1) != 2) return __LINE__;
  if(list.Nth(2) != 3) return __LINE__;
  return 0;
}
