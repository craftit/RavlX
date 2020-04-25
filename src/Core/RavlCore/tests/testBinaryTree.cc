// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Trees/testBinaryTree.cc"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Trees"
//! userlevel=Develop

#include "Ravl/BinaryTree.hh"
#include "Ravl/BinaryTreeUpIter.hh"
#include "Ravl/BinaryTreeDownIter.hh"
#include "Ravl/AVLTree.hh"
#include "Ravl/Random.hh"
#include "Ravl/Collection.hh"
#include "Ravl/CollectionIter.hh"
#include "Ravl/HSet.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

const int testSize = 10000;

int testBinaryTree();
int testBinaryTreeIter();
int testAVLTree();

int buildTestTree(BinaryTreeC<int,int> &testTree);

int main()  {
  int line;
  
  if((line = testBinaryTree())!= 0) {
    std::cerr << "testBinaryTree(), Error line:" << line << "\n";
    return 1;
  }

  if((line = testAVLTree())!= 0) {
    std::cerr << "testBinaryTree(), Error line:" << line << "\n";
    return 1;
  }

  if((line = testBinaryTreeIter())!= 0) {
    std::cerr << "testBinaryTreeIter(), Error line:" << line << "\n";
    return 1;
  }
  std::cerr << "Test passed ok. \n";
  return 0;
}

int buildTestTree(BinaryTreeC<int,int> &testTree) {
  testTree.Insert(6,0);
  testTree.Insert(8,1);
  testTree.Insert(1,2);
  testTree.Insert(3,3);
  testTree.Insert(7,4);
  testTree.Insert(5,5);
  testTree.Insert(4,6);
  testTree.Insert(2,7);
  return 0;
}

int BasicTests(BinaryTreeC<int,int> &testTree) {
  std::cerr << "Started basic tests.\n";
  int val;
  if(!testTree.IsEmpty()) return __LINE__;
  if(testTree.Size() != 0) return __LINE__;
  if(!testTree.Insert(5,0)) return __LINE__;
  if(!testTree.Find(5,val)) return __LINE__;
  if(val != 0) return __LINE__;
  if(testTree.Find(4,val)) return __LINE__;
  if(testTree.Find(6,val)) return __LINE__; 
  if(val != 0) return __LINE__;
  if(testTree.Size() != 1) return __LINE__;  
  if(testTree.IsEmpty()) return __LINE__;
  if(!testTree.Insert(3,1)) return __LINE__;
  if(!testTree.Find(3,val)) return __LINE__; 
  if(val != 1) return __LINE__;
  if(!testTree.Find(5,val)) return __LINE__; 
  if(val != 0) return __LINE__;
  if(testTree.Size() != 2) return __LINE__;  
  if(!testTree.Insert(7,2)) return __LINE__;
  if(!testTree.Find(7,val)) return __LINE__; 
  if(val != 2) return __LINE__;
  if(testTree.Size() != 3) return __LINE__;
  if(testTree.Insert(7,2,false)) return __LINE__;
  //testTree.Dump(cerr);
  testTree.Empty();
  if(testTree.Size() != 0) return __LINE__;
  if(testTree.Find(3,val)) return __LINE__; 
  if(val != 2) return __LINE__; // Check failed find doesn't overwrite.
#if 0
  std::cerr << "Testing delete.\n";
  int x;
  if((x = buildTestTree(testTree)) != 0)
    return x;
  //testTree.Dump(cerr);
  std::cerr << "Del 3.\n";
  testTree.Remove(3);
  testTree.Empty();
  if(testTree.Size() != 0) return __LINE__;
  if(testTree.Find(3,val)) return __LINE__; 
  if(val != 2) return __LINE__; // Check failed find doesn't overwrite.
  std::cerr << "Testing delete.\n";
  if((x = buildTestTree(testTree)) != 0)
    return x;
  testTree.Dump(cerr);  testTree.Dump(cerr);
  std::cerr << "Del 4.\n";
  testTree.Remove(4);
  testTree.Dump(cerr);
  std::cerr << "Del 5.\n";
  testTree.Remove(5);
  testTree.Dump(cerr);
#endif
  std::cerr << "Testing larger tree's.\n";
  
  // Test consective numbers.
  for(int i = 0;i < 10;i++)
    if(!testTree.Insert(i,i)) return __LINE__;
  //testTree.Dump(cerr);
  for(int i = 0;i < 10;i++) {
    if(!testTree.Find(i,val)) return __LINE__;
    if(val != i) return __LINE__;
  }
  testTree.Empty();
  // Test random values.
  HSetC<int> done;
  CollectionC<int> keys(testSize * 2 + 1);
  int key;
  for(int i = 0;i < testSize+1;i++) {
    do {
      key = RandomInt();
    } while(!done.Insert(key)) ;
    keys.Insert(key);
    if(!testTree.Insert(key,i)) return __LINE__;
  }
  UIntT c = testSize+1;
  if(testTree.Size() != c) return __LINE__;
  for(int i = 0;i < testSize;i++) {
    if(RandomInt() & 1) {
      // Delete an old item.
      key = keys.Pick();
      if(!done.Remove(key)) return __LINE__; // This must be a bug in some other code...
      if(!testTree.Remove(key)) return __LINE__;
      if(testTree.Find(key,val)) return __LINE__; // Del failed.
      c--;
      if(c != testTree.Size()) return __LINE__;
    } else {
      // Add a new item.
      do {
	key = RandomInt();
      } while(!done.Insert(key)) ;
      keys.Insert(key);
      if(!testTree.Insert(key,i)) return __LINE__;
      c++;
      if(c != testTree.Size()) return __LINE__;
    }
    testTree.MaxKey(); // Check internal asserts don't fail.
    testTree.MinKey(); // Check internal asserts don't fail.
  }
  // Check tree contains all it should do.
  {
    c = 0;
    for(BinaryTreeUpIterC<int,int> it(testTree);it;it++,c++) {
      if(!done.IsMember(it.Key())) {
	cerr << "Found invalid key:" << it.Key() << " Data:" << it.Data() << "\n";
	testTree.Dump(cerr);
	return __LINE__;
      }
    }
    if(c != done.Size()) return __LINE__;
    if(c != testTree.Size()) {
      std::cerr << "Size mismatch c=" << c << " TreeSize=" << testTree.Size() << "\n";
      return __LINE__;
    }
  }
  {
    c = 0;
    for(BinaryTreeDownIterC<int,int> it(testTree);it;it++,c++)
      if(!done.IsMember(it.Key())) return __LINE__;
    if(c != done.Size()) return __LINE__;
    if(c != testTree.Size()) return __LINE__;
  }
  CollectionIterC<int> it(keys);
  
  int kmax = *it;
  int kmin = *it;
  for(;it;it++) {
    if(*it > kmax)
      kmax = *it;
    if(*it < kmin)
      kmin = *it;
  }
  if(kmax != testTree.MaxKey()) {
    std::cerr << "kmax=" << kmax << " Min=" << testTree.MaxKey() << "\n";
    return __LINE__;
  }
  if(kmin != testTree.MinKey()) {
    std::cerr << "kmin=" << kmin << " Min=" << testTree.MinKey() << "\n";
    return __LINE__;
  }
  //testTree.Dump(cerr);
  std::cerr << "Done tests.\n";
  return 0;
}

int testBinaryTree() {
  std::cerr << "Testing simple binary tree.\n";
  BinaryTreeC<int,int> testTree(true);
  return BasicTests(testTree);
}

int testAVLTree() {
  std::cerr << "Testing AVL tree.\n";
  AVLTreeC<int,int> testTree(true);
  return BasicTests(testTree);
}

int testBinaryTreeIter() {
  std::cerr << "Testing binary tree iterators.\n";
  BinaryTreeC<int,int> testTree(true);
  buildTestTree(testTree);
  //testTree.Dump(cerr);
  int i = 1;
  {
    for(BinaryTreeUpIterC<int,int> it(testTree);it;it++,i++) {
      //cerr << it.Key() << " = " << *it << "\n";
      if(i != it.Key()) return __LINE__;
    } 
  }
  i--;
  {
    for(BinaryTreeDownIterC<int,int> it(testTree);it;it++,i--) {
      //cerr << it.Key() << " = " << *it << "\n";
      if(i != it.Key()) return __LINE__;
    } 
  }
  return 0;
}

namespace RavlN {
  template
  class BinaryTreeC<int, UIntT>;

  template
  class BinaryTreeUpIterC<int, UIntT>;

  template
  class BinaryTreeDownIterC<int, UIntT>;
}