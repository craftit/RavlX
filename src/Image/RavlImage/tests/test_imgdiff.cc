// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImage
//! file="Ravl/Image/Base/test_imgdiff.cc"
 #include <iostream>
 #include <cstdlib>

 using namespace std;

 const bool Test_Command(const char* const test_name,
                         const char* const command,
                         const int expected_ret_val)
 {
   // Assume we've failed!
   bool passed = false;

   std::cerr<<"## Start command test: "<<test_name<<endl;

   std::cerr<<"-- Start: Running command: "<<command<<endl;
   const int ret_val = system(command);
   std::cerr<<"-- End: Running command"<<endl;

   // Did we pass?
   if(ret_val == expected_ret_val) {
     passed = true;
   }

   std::cerr<<"Test "<<(passed?"passed":"failed")<<" with return value: "<<ret_val<<endl;

   std::cerr<<"## End command test: "<<test_name<<endl;

   return passed;
 }

 int main(int argc, char* argv[])
 {
   // Assume we've failed!
   bool passed = false;

   // Make sure we're called with no arguments because we're not
   // expecting any.
   if(argc == 1) {
     const char command[] = "imgdiff \"@str:P2 2 2 255 0 255 255 0\" \"@str:P2 2 2 255 255 0 0 255\" -o /dev/null";
     // This program must be called with a controlled setting of PATH to ensure
     // the correct version of imgdiff is tested

     passed = Test_Command("imgdiff test", command, 0);
   }

   return passed?0:1;
 }
