// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_COMPILERHINTS_HEADER
#define RAVL_COMPILERHINTS_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! file="Ravl/Core/Base/CompilerHints.hh"

#include "Ravl/config.h"

#if RAVL_COMPILER_GCC
#define RAVL_EXPECT(x,y) __builtin_expect(x,y)
// Compile code that expects the value of x to be y, used for giving hints to the compiler
// about the branch a jump is likely to take.
#define RAVL_PREFETCH_READ(addr,locality) __builtin_prefetch(addr,0,locality)
// Load data into cache for reading a little later.
// Locality 0=Read once, 3=Read many times.
#define RAVL_PREFETCH_WRITE(addr,locality) __builtin_prefetch(addr,1,locality)
// Load data into cache for writing a little later.
// Locality 0=Write once, 3=Write many times.
#else
#define RAVL_EXPECT(x,y) (x)
#define RAVL_PREFETCH_READ(addr,locality) 
#define RAVL_PREFETCH_WRITE(addr,locality) 
#endif

#endif
