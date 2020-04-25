// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Container/Trees/exAVLTree.cc"
//! docentry="Ravl.API.Core.Trees"
//! author="Charles Galambos"
//! userlevel=Normal

//: Using and iterating through AVLTreeC 

#include "Ravl/AVLTree.hh"
#include "Ravl/BinaryTreeUpIter.hh"
#include "Ravl/BinaryTreeDownIter.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int main() {
  
  AVLTreeC<int,const char *> tree(true);

  // Put some data into the tree.
  
  tree.Insert(4,"c");
  tree.Insert(2,"a");
  tree.Insert(3,"b");
  
  // Go through the tree from the smallest to largest
  // key values.
  
  std::cout << "Up....\n";
  for(BinaryTreeUpIterC<int,const char *> it(tree);it;it++)
    std::cout << "Key=" << it.Key() <<" Data=" << it.Data() << "\n";

  // Go through the tree from the largest to smallest 
  // key values.
  
  std::cout << "Down....\n";  
  for(BinaryTreeDownIterC<int,const char *> it(tree);it;it++)
    std::cout << "Key=" << it.Key() <<" Data=" << it.Data() << "\n";

  return 0;
}
