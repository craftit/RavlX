// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////
//! rcsid="$Id$"
// Test arbitary hash.
//! lib=RavlCore
//! file="Ravl/Core/Container/Hash/WordFreq.cc"
//! docentry="Ravl.API.Core.Hash Tables"

#include "Ravl/String.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/Hash.hh"
#include "Ravl/IntC.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int main(int nargs,char *argv[]) {
  std::cout << "Counting..... \n";
  
  // Creat the table...
  
  HashC<StringC,IntC> wordFreq;

  // Read the standard input one word at a time.
  
  StringC word;
  while(!cin.eof()) {
    cin >> word;
    wordFreq[word.Copy()]++;
    // Because the Word buffer is overwritten by
    // successive reads we have to make a copy of it
    // when it's used as a key in the hash table.
  }
  
  // Print out the results.
  
  std::cout << wordFreq.Size() << " Unique words found: \n";
  
  // Print out the number of times we've seen each word.
  
  for(HashIterC<StringC,IntC> it(wordFreq);it.IsElm();it.Next())
    std::cout << "'" << it.Key() <<  "' " << it.Data() << " \n";

  return 0;
}
