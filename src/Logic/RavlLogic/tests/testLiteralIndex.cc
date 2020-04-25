// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/testLiteralIndex.cc"
//! docentry="Ravl.API.Logic.Index"
//! userlevel=Develop
//! author="Charles Galambos"

#include "Ravl/Logic/LiteralIndex.hh"
#include "Ravl/Logic/Tuple.hh"
#include "Ravl/Logic/LiteralIndexFilter.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/PointerManager.hh"

using namespace RavlLogicN;

IntT testBase();
IntT testVars();
IntT testIndexFilter();
IntT testVarIndexFilter();
IntT testIndexIO();

int main()
{
  UIntT err;

  if((err = testBase()) != 0) {
    cerr << "LiteralIndex test failed, line: " << err << " \n";
    return 1;
  }
#if 1
  if((err = testVars()) != 0) {
    cerr << "LiteralIndex test failed, line: " << err << " \n";
    return 1;
  }
  if((err = testIndexFilter()) != 0) {
    cerr << "LiteralIndexFilter test failed, line: " << err << " \n";
    return 1;
  }
  if((err = testIndexIO()) != 0) {
    cerr << "LiteralIndexIO test failed, line: " << err << " \n";
    return 1;
  }
  if((err = testVarIndexFilter()) != 0) {
    cerr << "LiteralIndexFilter test failed, line: " << err << " \n";
    return 1;
  }
#endif
  cerr << "LiteralIndex test passed. \n";
  return 0;
}

IntT testBase() {
  LiteralC l1 = Literal();
  LiteralC l2 = Literal();
  
  LiteralC t1 = Tuple(l1);
  LiteralC t2 = Tuple(l2,l1);
  LiteralC t3 = Tuple(l1,l1);
  if(t1 == t2) return __LINE__;
  if(t3 == t1) return __LINE__;
  
  LiteralIndexC<UIntT> index(true);
  index[l1] = 0; 
  index[t2] = 2;
  index[t1] = 0; 
  index[t1] = 1; 
  index[t3] = 3; 
  //  cerr << "Index=" << index << " " << index[t1] << "\n";
  if(index[l1] != 0) return __LINE__;
  if(index[t1] != 1) return __LINE__;
  if(index[t2] != 2) return __LINE__;
  if(index[t3] != 3) return __LINE__;
  if(index.Size() != 4) return  __LINE__;
  //cerr << "Dump:\n";
  //index.Dump(cerr);
  
  if(!index.Del(t2)) return __LINE__;
  if(index[l1] != 0) return __LINE__;
  if(index[t1] != 1) return __LINE__;
  if(index[t3] != 3) return __LINE__;
  UIntT v;
  if(index.Lookup(t2,v)) return __LINE__;
  if(index.Size() != 3) return  __LINE__;
  //cerr << "Dump:\n";
  //index.Dump(cerr);
  
  if(!index.Del(l1)) return __LINE__;
  if(!index.Del(t1)) return __LINE__;
  if(!index.Del(t3)) return __LINE__;
  if(index.Del(l1)) return __LINE__;
  if(index.Del(t3)) return __LINE__;
  return 0;
}

IntT testVars() {
  LiteralIndexC<UIntT> index(true);
  LiteralC l1 = Literal();
  LiteralC l2 = Literal();
  LiteralC v1 = Var();
  LiteralC v2 = Var();
  LiteralC t1 = Tuple(l1);
  LiteralC t2 = Tuple(l2,l1);
  LiteralC t3 = Tuple(l1,l1);
  LiteralC t3a = Tuple(l1,l1);
  LiteralC t4 = Tuple(v1,l1);
  LiteralC t5 = Tuple(v1,v1);
  LiteralC t6 = Tuple(v2,l1);
  
  index[l1] = 0; 
  index[t1] = 1; 
  index[t2] = 2;
  index[t3] = 100; 
  index[t3a] = 3; 
  index[t4] = 4; 
  index[t5] = 5; 
  index[t6] = 6; 
  
  if(index[l1] != 0) return __LINE__;
  if(index[t1] != 1) return __LINE__;
  if(index[t2] != 2) return __LINE__;
  if(index[t3] != 3) return __LINE__;
  if(index[t4] != 4) return __LINE__;
  if(index[t5] != 5) return __LINE__;
  if(index.Size() != 7) return  __LINE__;
  
  return 0;
}

IntT testIndexFilter() {
  cerr << "testIndexFilter() \n";
  LiteralIndexC<UIntT> index(true);
  LiteralC l1 = Literal();
  LiteralC l2 = Literal();
  LiteralC l3 = Literal();
  LiteralC t1 = Tuple(l1);
  LiteralC t2 = Tuple(l2,l1);
  LiteralC t3 = Tuple(l1,l1);
  LiteralC t4 = Tuple(l1,l3);
  
  VarC v1 = Var();
  
  cerr << "v1: " << v1 << "\n";
  cerr << "l1: " << l1 << "\n";
  
  index[l1] = 0; 
  index[t1] = 1; 
  index[t2] = 2;
  index[t3] = 3; 
  index[t4] = 4; 
  UIntT count = 0;
  
  // Test filter on variable.
  cerr << "--- Filter on var. \n";
  for(LiteralIndexFilterC<UIntT> it(index,v1);it;it++)
    count++;
  if(count != 5) return __LINE__;
  
  cerr << "--- Filter on tuple (v1,l1) \n";
  
  // Test filter on tuple with single var.
  count = 0;
  LiteralC f1 = Tuple(v1,l1);
  for(LiteralIndexFilterC<UIntT> it(index,f1);it;it++) {
    count++;
    if(it.MappedData() == 2)
      continue;
    if(it.MappedData() == 3)
      continue;
    cerr << "Filter failed :" << it.Data().Name() << " Value:" << it.MappedData() << "\n";
    return __LINE__;
  }
  if(count != 2) return __LINE__;
  count = 0;
  
  // Test filter on another tuple with single var.
  cerr << "--- Filter on tuple (l2,v1) \n";
  LiteralC f2 = Tuple(l2,v1);
  for(LiteralIndexFilterC<UIntT> it(index,f2);it;it++) {
    count++;
    if(it.MappedData() == 2)
      continue;
    cerr << "Filter failed :" << it.Data().Name() << " Value:" << it.MappedData() << "\n";
    return __LINE__;
  }
  if(count != 1) return __LINE__;
  
  // Test multi variable filter.
  cerr << "--- Filter on tuple (v1,v1) \n";
  count = 0;
  LiteralC f3 = Tuple(v1,v1);
  for(LiteralIndexFilterC<UIntT> it(index,f3);it;it++) {
    count++;
    if(it.MappedData() == 3)
      continue;
    cerr << "Filter failed :" << it.Data().Name() << " Value:" << it.MappedData() << "\n";
    return __LINE__;
  }
  if(count != 1) return __LINE__;
  
  return 0;
}

IntT testVarIndexFilter() {
  cerr << "testVarIndexFilter() \n";
  LiteralIndexC<UIntT> index(true);
  LiteralC l1("l1");
  LiteralC l2("l2");
  LiteralC l3("l3");
  LiteralC l4("l4");
  LiteralC v1 = Var();
  LiteralC v2 = Var();
  LiteralC v3 = Var();
  
  LiteralC t1 = Tuple(l1);
  LiteralC t2 = Tuple(l2,v1);
  LiteralC t3 = Tuple(l1,l1);
  LiteralC t4 = Tuple(v2,l3);
  LiteralC t5 = Tuple(v1,v1);
  LiteralC t6 = Tuple(l2,l2);
  LiteralC t7 = Tuple(Tuple(v1,l2),l4);
  
  index[l1] = 0; 
  index[t1] = 1; 
  index[t2] = 2;
  index[t3] = 3; 
  index[t4] = 4; 
  index[t5] = 5; 
  index[t6] = 6; 
  index[t7] = 7; 
  cerr << "Index=" << "\n";
  //index.Dump(cerr);
  UIntT count = 0;
  
  // Test filter on variable.
  cerr << "F1. Tuple(l2,l2) \n";
  LiteralC f1 = Tuple(l2,l2);
  for(LiteralIndexFilterC<UIntT> it(index,f1);it;it++) {
    cerr << "Result " << it.Data().Name() << " = "  << it.MappedData() << "\n";
    count++;
  }
  if(count != 3) return __LINE__;

  cerr << "F2. Tuple(l2,l3) \n";
  LiteralC f2 = Tuple(l2,l3);
  count =0;
  for(LiteralIndexFilterC<UIntT> it(index,f2);it;it++) {
    cerr << "Result " << it.Data().Name() << " = "  << it.MappedData() << "\n";
    count++;
  }
  if(count != 2) return __LINE__;
  
  count = 0;
  cerr << "F3. Tuple(l3,l3) \n";
  LiteralC f3 = Tuple(l3,l3);
  for(LiteralIndexFilterC<UIntT> it(index,f3);it;it++) {
    cerr << "Result " << it.Data().Name() << " = "  << it.MappedData() << "\n";
    count++;
  }
  if(count != 2) return __LINE__;

  cerr << "F4. Tuple(v,l3) \n";
  count = 0;
  LiteralC f4 = Tuple(Var(),l4);
  for(LiteralIndexFilterC<UIntT> it(index,f4);it;it++) {
    cerr << "Result " << it.Data().Name() << " = "  << it.MappedData() << "\n";
    count++;
  }
  if(count != 3) return __LINE__;
  
  return 0;
}

IntT testIndexIO() {
  cerr << "testIndexIO() \n";
  
  LiteralIndexC<UIntT> index1(true);
  
  LiteralC l1 = Literal();
  LiteralC l2 = Literal();
  LiteralC l3 = Literal();
  LiteralC t1 = Tuple(l1);
  LiteralC t2 = Tuple(l2,l1);
  LiteralC t3 = Tuple(l1,l1);
  LiteralC t4 = Tuple(l1,l3);
  
  index1[l1] = 0; 
  index1[t1] = 1; 
  index1[t2] = 2;
  index1[t3] = 3; 
  index1[t4] = 4; 
  
  // Save the index along with some literals
  StrOStreamC os;
  BinOStreamC bos(os);
  bos << index1 << ObjIO(l1) << ObjIO(l2) << ObjIO(l3);

  
  // Load up the index again.
  
  StringC data = os.String();
  StrIStreamC is(data);
  BinIStreamC bis(is);
  
  LiteralIndexC<UIntT> index2;
  LiteralC l1b;
  LiteralC l2b;
  LiteralC l3b;
  
  bis >> index2 >> ObjIO(l1b) >> ObjIO(l2b) >> ObjIO(l3b);
  
  if(index2.Size() != index1.Size()) return __LINE__;
  VarC v1 = Var();
  
  // Check we've got a working index.
  
  UIntT count = 0;
  LiteralC f1 = Tuple(v1,l1b);
  for(LiteralIndexFilterC<UIntT> it(index2,f1);it;it++) {
    count++;
    if(it.MappedData() == 2)
      continue;
    if(it.MappedData() == 3)
      continue;
    cerr << "Filter failed :" << it.Data().Name() << " Value:" << it.MappedData() << "\n";
    return __LINE__;
  }
  if(count != 2) return __LINE__;
  
  return 0;
}
