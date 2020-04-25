// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2006-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/config.h"
#include "Ravl/CPUID.hh"
//! lib=RavlCore

#if RAVL_HAVE_SYS_TYPES_H
// This needs to be included before <iostream>
#include <sys/types.h>
#endif

#include <iostream>

namespace RavlN {

  // These features can be checked on register edx at address 0x00000001
#define cpuid_mmx       8388608         // 2^23 (bit 23)
#define cpuid_sse       33554432        // 2^25 (bit 25)
#define cpuid_sse2      67108864        // 2^26 (bit 26)
#define cpuid_htt       268435456       // 2^28 (bit 28)

#if CHECK_FOR_3DNOW
// These features can be checked on register edx at address 0x80000001
#define cpuid_3dnow     2147483648      // 2^31 (bit 31)
#define cpuid_3dnow_p   1073741824      // 2^30 (bit 30)
#endif

unsigned long cpudetect(unsigned long address)
{

  //: need to protect ebx and ecx for being clobbered
  unsigned long id = 0;
#if RAVL_CPU_IX86
#if RAVL_OS_WIN32

  __asm {
    push ebx
    push ecx
    mov eax, address
    CPUID
    mov id, edx
    pop ecx
    pop ebx
    };

#else

  __asm__ (
       "push %%ebx\n"
       "push %%ecx\n"
       "mov %%eax, %1\n"
       "CPUID\n"
       "mov %0, %%edx\n"
       "pop %%ecx\n"
       "pop %%ebx\n"
       :"=d" (id)
       :"a" (address)
       );

#endif
#else
#if RAVL_CPU_X86_64
  __asm__ (
       "push %%rbx\n"
       "push %%rcx\n"
       "mov %%rax, %1\n"
       "CPUID\n"
       "mov %0, %%rdx\n"
       "pop %%rcx\n"
       "pop %%rbx\n"
       :"=d" (id)
       :"a" (address)
       );
#endif
#endif
  //std::cerr << "CPUID:\t\t" << (int)id << std::endl;
  return id;
}



  CPUIDC::CPUIDC()
    : m_mmx(false),
      m_sse(false),
      m_sse2(false),
      m_htt(false)
  {

    //: These addresses were got from AMD web site but work for Intel to
    unsigned long address= 0x00000001;
    unsigned long id = cpudetect(address);

    if(cpuid_mmx & id)
      m_mmx = true;

    if(cpuid_sse & id)
      m_sse = true;

    if(cpuid_sse2 & id)
      m_sse2 = true;

    if(cpuid_htt & id)
      m_htt = true;
#if CHECK_FOR_3DNOW
    m_3dnow = false;
    m_3dnow_p = false;
    address= 0x80000001;
    id = cpudetect(address);

    if(cpuid_3dnow & id)
      m_3dnow = true;

    if(cpuid_3dnow_p & id)
      m_3dnow_p = true;
#endif
  }

  void CPUIDC::Info() const {
    using namespace std;
    std::cerr << "MMX:\t\t" << (int)m_mmx << std::endl;
    std::cerr << "SSE:\t\t" << (int)m_sse << std::endl;
    std::cerr << "SSE2:\t\t" << (int)m_sse2 << std::endl;
    std::cerr << "HTT:\t\t" << (int)m_htt << std::endl;
#if CHECK_FOR_3DNOW
    std::cerr << "3dNow:\t\t" << (int)m_3dnow << std::endl;
    std::cerr << "3dNow Ext:\t" << (int)m_3dnow_p << std::endl;
#endif
  }

static unsigned long cpu_id = 0;
static bool cpu_id_initialized = false;

//: do we have mmx
bool MMX()
{
  if(!cpu_id_initialized)
  {
    cpu_id = cpudetect(0x00000001);
    cpu_id_initialized = true;
  }

  return (cpuid_mmx & cpu_id) != 0;
}

//: do we have sse
bool SSE()
{
  if(!cpu_id_initialized)
  {
    cpu_id = cpudetect(0x00000001);
    cpu_id_initialized = true;
  }

  return (cpuid_sse & cpu_id) != 0;
}

//: do we have sse2
bool SSE2()
{
  if(!cpu_id_initialized)
  {
    cpu_id = cpudetect(0x00000001);
    cpu_id_initialized = true;
  }
  //std::cerr << "CPUID:" << cpu_id << std::endl;
  return (cpuid_sse2 & cpu_id) != 0;
}

//: do we have hyperthreading processor
bool HTT()
{
  if(!cpu_id_initialized)
  {
    cpu_id = cpudetect(0x00000001);
    cpu_id_initialized = true;
  }

  return (cpuid_htt & cpu_id) != 0;
}

}
