#include "Ravl/OS/CharIO.hh"
#include "Ravl/config.h"
#include <cstdio>

//! author="Believed to be Linus Torvald.  Modified for multibyte i/p by Bill Christmas"

#if RAVL_OS_UNIX

#include <unistd.h>
#include <termios.h>


RavlN::UInt64T RavlN::GetKeypress(bool EchoOn) {
  RavlN::UInt64T buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  if (!EchoOn) old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, sizeof(UInt64T)) < 0)
    perror ("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror ("tcsetattr ~ICANON");
  return (buf);
}

#endif
