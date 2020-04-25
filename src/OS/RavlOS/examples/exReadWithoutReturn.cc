/*
*  Author: as
*
*/

#include "Ravl/OS/CharIO.hh"
#include "Ravl/Types.hh"
#include "Ravl/Stream.hh"
#include <unistd.h>

using namespace std;
using namespace RavlN;


unsigned long int getChoice(const char *text, bool echoOn) {
  cout << text << " " ;
  cout.flush();
  return GetKeypress(echoOn);
}

int main() {
   UInt64T c;
   getChoice("Menue Demo, press any key to start! ", false);

   while (1) {
     RavlAssert(!system("clear"));   // Unix
     // system("cls");  // Windows
       
     cout << " ---- Menu ----" << endl;
     cout << "1  item 1" << endl;
     cout << "2  item 2" << endl;
     cout << "0  exit" << endl;
     c = getChoice("Your choice (0-2):", true);
     switch (c) {
       case '1': cout << endl << "item 1" << endl;
               sleep(2);
               break;
       case '2': cout << endl << "item 2" << endl;
               sleep(2);
               break;
       case '0': cout << endl<< "bye" << endl;
               exit(0);
     default: cout << endl << hex << c << ": wrong input" << endl;
                sleep(2);
     }  // switch
  } // while
}
