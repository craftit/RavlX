/* This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here*/
#ifndef RAVL_CONFIG_HEADER
#define RAVL_CONFIG_HEADER 1
/*! file="Ravl/Core/Base/config.h" */
/*! lib=RavlCore */

/********************************************************************************/
/******* AMMA / QMAKE COMPATIBILITY *******/
#if defined(WIN32)
  #if defined(_DEBUG)
    #define RAVL_CHECK 1
  #else
    #define RAVL_CHECK 0
  #endif
#else
  #if !defined(RAVL_CHECK)
    #if defined(AMMA_CHECK) || defined(QMAKE_CHECK)
      #define RAVL_CHECK 1
    #else
      #define RAVL_CHECK 0
    #endif
  #endif
#endif
/****************************************/
/******** RAVL Paranoid checking. ********
 * These checks may significantly slow code, but are helpful when looking for more
 * obscure bugs.
 */
#ifdef QMAKE_PARANOID
  #define RAVL_PARANOID(x) x
#else
  #define RAVL_PARANOID(x)
#endif

/********************************************************************************/
/***** Detect the OS, compiler and processor type being used. *******************/

#if defined(__llvm__)
  #define RAVL_COMPILER_LLVM 1
#else
  #define RAVL_COMPILER_LLVM 0
#endif

/* GNU compiler ? */
#if defined(__GNUC__)
  #define RAVL_COMPILER_GCC2 (__GNUC__ == 2)   /* gcc 2.x (No longer supported) */
  #define RAVL_COMPILER_GCC3 (__GNUC__ == 3)  /* gcc 3.x */
  #define RAVL_COMPILER_GCC3_4 ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 4))  /* gcc 3.4.x or later. */
  #define RAVL_COMPILER_GCC4  (__GNUC__ >= 4)  /* gcc 4.x or later. */
  #define RAVL_COMPILER_GCC42  ((__GNUC__ >= 4) && (__GNUC_MINOR__ == 2)) /* gcc 4.3.x */
  #define RAVL_COMPILER_GCC43  ((__GNUC__ >= 4) && (__GNUC_MINOR__ == 3)) /* gcc 4.3.x */
  #define RAVL_COMPILER_GCC 1
  #define RAVL_COMPILER_MIPSPRO 0  /* MIPS Pro compiler */
  #define RAVL_COMPILER_VISUALCPP 0    /* Visual C++ */
  #define RAVL_COMPILER_VISUALCPPNET 0 /* Visual C++ .NET*/
  #define RAVL_COMPILER_VISUALCPP6 0   /* Visual C++ 6 */
#else
  #define RAVL_COMPILER_GCC2 0    /* gcc 2.x (No longer supported) */
  #define RAVL_COMPILER_GCC3 0    /* gcc 3.x */
  #define RAVL_COMPILER_GCC3_4 0  /* gcc 3.4.x or later. */
  #define RAVL_COMPILER_GCC4  0   /* gcc 4.x or later. */
  #define RAVL_COMPILER_GCC42 0   /* gcc 4.3.x */
  #define RAVL_COMPILER_GCC43 0   /* gcc 4.3.x */
  #define RAVL_COMPILER_GCC 0

  /* MIPS Pro compiler */
  #if defined(__sgi)
    #define RAVL_COMPILER_MIPSPRO 1
  #else
    #define RAVL_COMPILER_MIPSPRO 0   /* MIPS Pro compiler */
  #endif
  /* Visual C++ */
  #if defined(WIN32)
    #define RAVL_COMPILER_VISUALCPP 1
    #if (_MSC_VER >= 1400)
      #define RAVL_COMPILER_VISUALCPP6 0
      #define RAVL_COMPILER_VISUALCPPNET_2005 1
      #define RAVL_COMPILER_VISUALCPPNET 0
    #elif (_MSC_VER >= 1300)
      #define RAVL_COMPILER_VISUALCPP6 0
      #define RAVL_COMPILER_VISUALCPPNET_2005 0
      #define RAVL_COMPILER_VISUALCPPNET 1
    #else
      #define RAVL_COMPILER_VISUALCPP6 1
      #define RAVL_COMPILER_VISUALCPPNET_2005 0
      #define RAVL_COMPILER_VISUALCPPNET 0
    #endif
  #else
    #define RAVL_COMPILER_VISUALCPP 0
    #define RAVL_COMPILER_VISUALCPPNET 0
  #endif
#endif

/* 386 base variant. */
#if defined(__i386__)
  #define RAVL_CPU_IX86  1
#else
  #define RAVL_CPU_IX86  0
#endif

#if (defined (__x86_64__) || defined(__LP64__))
  #define RAVL_CPU_X86_64 1
#else
  #define RAVL_CPU_X86_64 0
#endif

/* sun sparc based system. */
#if defined(__sparc)
  #define RAVL_CPU_SPARC 1
#else
  #define RAVL_CPU_SPARC 0
#endif

/* sparc v9 instruction set */
/* for __sparc_v9__ to be set the appropriate compile time options may have to be applied */
/* for example,  -Wa,-xarch=v8plus -mcpu=v9 */
#if defined(__sparc_v9__)
  #define RAVL_CPU_SPARC_V9 1
#else
  #define RAVL_CPU_SPARC_V9 0
#endif

/* mips */
#if defined(__mips)
  #define RAVL_CPU_MIPS 1
#else
  #define RAVL_CPU_MIPS 0
#endif

/* alpha based system */
#if defined(__alpha)
  #define RAVL_CPU_ALPHA 1
#else
  #define RAVL_CPU_ALPHA 0
#endif

/* arm based system */
#if defined(__arm)
  #define RAVL_CPU_ARM  1
#else
  #define RAVL_CPU_ARM  0
#endif

/* powerpc based system */
#if defined(__ppc__)
  #define RAVL_CPU_PPC 1
#else
  #define RAVL_CPU_PPC 0
#endif

/* 64 bit cpu */
#if  (defined (__x86_64__) || defined(__LP64__))
  #define RAVL_CPUTYPE_64  1
  #define RAVL_CPUTYPE_32  0
#else
  #define RAVL_CPUTYPE_64  0
  #define RAVL_CPUTYPE_32  1
#endif

/* Linux based OS. 64-bit */
#if (defined(__linux__) && defined (__x86_64__))
  #define RAVL_OS_LINUX64 1
#else
  #define RAVL_OS_LINUX64 0
#endif

/* Linux      */
#if (defined(__linux__) && !defined(__x86_64__))
  #define RAVL_OS_LINUX 1
#else
  #define RAVL_OS_LINUX 0
#endif

/* Windows      */
#if defined(WIN32)
  #define RAVL_OS_WIN32 1
#else
  #define RAVL_OS_WIN32 0
#endif

/* IRIX.      */
#if defined(__sgi__)
  #define RAVL_OS_IRIX 1
#else
  #define RAVL_OS_IRIX 0
#endif

/* Solaris.   */
#if defined(__sun)
  #define RAVL_OS_SOLARIS 1
#else
  #define RAVL_OS_SOLARIS 0
#endif

/* Solaris. 2.5.7 */
#if defined(__sol2_7__)
  #define RAVL_OS_SOLARIS7 1
#else
  #define RAVL_OS_SOLARIS7 0
#endif

/* Solaris. 2.5.9 */
#if defined(__sol2_9__)
  #define RAVL_OS_SOLARIS9 1
#else
  #define RAVL_OS_SOLARIS9 0
#endif

/* Mac OS-X */
#if defined(__APPLE__)
  #define RAVL_OS_MACOSX  1
#else
  #define RAVL_OS_MACOSX  0
#endif

/* FreeBSD */
#if defined(__FreeBSD__)
  #define RAVL_OS_FREEBSD 1
#else
  #define RAVL_OS_FREEBSD 0
#endif

/* OSF.       */
#if defined(__osf__)
  #define RAVL_OS_OSF  1
#else
  #define RAVL_OS_OSF  0
#endif

/* Cygwin is a windows/unix hybrid */
#if defined(__CYGWIN__)
  #define RAVL_OS_CYGWIN 1
#else
  #define RAVL_OS_CYGWIN 0
#endif

#if RAVL_COMPILER_GCC
  #define RAVL_GNUEXT(x) x /* Support GNU extensions */
#else
  #define RAVL_GNUEXT(x) /* No support for GNU extensions */
#endif

#define RAVL_OS_UNIX !RAVL_OS_WIN32       /* a unix style system */
#define RAVL_OS_POSIX !RAVL_OS_WIN32      /* POSIX complient OS. */

/* Processor variants. */

#define RAVL_CPU_IX86_PENTIUM4  (RAVL_CPU_IX86 && defined(__tune_pentium4__))  /* Pentium 4 processor ? */
#define RAVL_CPU_IX86_PENTIUM3  (RAVL_CPU_IX86 && !RAVL_CPU_IX86_PENTIUM4)     /* Pentium 3 processor ?  (Default assumption.) */

/********************************************************************************/
/****** RAVL behavour ***********************************************************/

/* Set the following define to 1 to have the Ravl IO system
 * to automatically use the type conversion mechanism when doing IO.
 * This is add alot of flexibity to IO operations but comes at the
 * price of larger executables and a slighty longer load times.
 */
#define RAVL_USE_IO_AUTO_TYPECONVERTER 1

/* Setting the following define to 1 will switch various routines to
 * use multi-thread implementations. Unfortunately this may mean changing
 * the USESLIBS to include the RavlThreads library for the directories
 * concerned. This is off by default as it generates slower code for single
 * processor systems.
 */
#define RAVL_USE_PARALLEL 0


/********************************************************************************/
/****** OS Configuration/System includes ****************************************/

#define RAVL_USE_LARGEFILESUPPORT (RAVL_OS_LINUX || RAVL_OS_LINUX64)   /* Special 64 bit filehandling functions like stat64. */
#define RAVL_LIMITED_FILENAMES 0                   /* have 8.3 filenames. */
#define RAVL_HAVE_UNIXDIRFUNCS RAVL_OS_POSIX       /* Unix style directory access ? */
#define RAVL_HAVE_REENTRANT_UNIXDIRFUNCS !RAVL_OS_CYGWIN /* Re-entrant directory functions. */
#define RAVL_HAVE_POSIX_THREADS_RWLOCK (RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_FREEBSD)
#define RAVL_HAVE_POSIX_THREADS_RWLOCKTIMED (RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_FREEBSD)
#define RAVL_HAVE_POSIX_THREADS  RAVL_OS_POSIX    /* Have posix threading functions. */
#define RAVL_HAVE_WIN32_THREADS  RAVL_OS_WIN32    /* Have windows threading. */
#define RAVL_HAVE_PTHREAD_COND RAVL_OS_POSIX
#define RAVL_HAVE_BYTESWAP     (RAVL_OS_LINUX || RAVL_OS_LINUX64)
#define RAVL_HAVE_NETDB_H      RAVL_OS_UNIX
#define RAVL_HAVE_UNISTD_H     RAVL_OS_UNIX       /* have unistd.h */
#define RAVL_HAVE_SYS_SOCKET_H RAVL_OS_UNIX       /* have sys/socket.h  */
#define RAVL_HAVE_TERMIOS      RAVL_OS_UNIX       /* have termios for controlling serial ports. */
#define RAVL_USE_TIMEB_H       RAVL_OS_WIN32
#define RAVL_HAVE_PROCESS_H    RAVL_OS_WIN32      /* have process.h */
#define RAVL_USE_WINSOCK       RAVL_OS_WIN32
#define RAVL_HAVE_IO_H         RAVL_OS_WIN32
#define RAVL_HAVE_SYS_TIME_H   (!RAVL_OS_SOLARIS && !RAVL_OS_WIN32) /* Have <sys/time.h> */
#define RAVL_HAVE_SIGNAL_H     (RAVL_OS_SOLARIS)                    /* have <sys/signal.h> */
#define RAVL_HAVE_SYS_TYPES_H  RAVL_OS_FREEBSD /* sys/types.h */
#define RAVL_HAVE_SYS_SELECT_H RAVL_OS_MACOSX || RAVL_OS_FREEBSD /* sys/select.h */
#define RAVL_HAVE_SYS_STAT_H   RAVL_OS_FREEBSD
#define RAVL_HAVE_ALLOCA_H     (!RAVL_OS_FREEBSD && !RAVL_COMPILER_VISUALCPP)
#if (defined(__GLIBC__) || defined(__APPLE__))
  #define RAVL_HAVE_BACKTRACE 1
#else
  #define RAVL_HAVE_BACKTRACE 0
#endif
#define RAVL_HAVE_YIELD        (RAVL_OS_SOLARIS)   /* have yield() */
#define RAVL_HAVE_SCHED_YIELD  (!RAVL_OS_WIN32 && !RAVL_OS_SOLARIS) /* have sched_yield() */
#define RAVL_HAVE_SCHED_H      !RAVL_OS_WIN32     /* have <sched.h> */
#define RAVL_HAVE_THREAD_H     RAVL_OS_SOLARIS    /* have <thread.h> (solaris threads.) */
#define RAVL_HAVE_THR_YIELD    RAVL_OS_SOLARIS    /* have thr_yield() (solaris threads.)  */
#define RAVL_READDIR_R_NOBUFF  RAVL_OS_OSF        /* readdir_r has no buffer argument. */
#define RAVL_TIMET_IS_INT      !RAVL_OS_IRIX      /* time_t is an int or long. IRIX uses a struct, effects stat() results. */
#define RAVL_HAVE_PWD_H        (RAVL_OS_UNIX || RAVL_OS_CYGWIN)       /* have <pwd.h> */
#define RAVL_ERRNO_IS_FUNC     0                  /* errno should be used as function. i.e. errno() for use with threaded code. */
#define RAVL_HAVE_GETPWNAM_R   (!RAVL_OS_LINUX64 && !RAVL_OS_LINUX && !RAVL_OS_CYGWIN && !RAVL_OS_MACOSX &&!RAVL_OS_FREEBSD)  /* have reentrant getpwnam_r */
#define RAVL_HAVE_GETPWUID_R   (!RAVL_OS_LINUX && !RAVL_OS_LINUX64 && !RAVL_OS_CYGWIN && !RAVL_OS_MACOSX &&!RAVL_OS_FREEBSD)  /* have reentrant getpwnam_r */
#define RAVL_HAVE_GETPW_RET_PW !RAVL_OS_OSF     /* Pass pointer to result ptr as last argument for  getpwuid_r, getpwnam_r */
#define RAVL_HAVE_GETPW_WITH_RESULT  RAVL_OS_IRIX  || RAVL_OS_SOLARIS || RAVL_OS_FREEBSD /* Pass pointer to result ptr as last argument for  getpwuid_r, getpwnam_r */
#define RAVL_HAVE_HSTRERROR    (RAVL_OS_IRIX || RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_FREEBSD)  /* have hstrerror, otherwise use strerror. */
#define RAVL_HAVE_SOCKLEN_T    (RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_SOLARIS || RAVL_OS_MACOSX || RAVL_OS_FREEBSD)  /* Have socklen_t */
#define RAVL_HAVE_INTFILEDESCRIPTORS (!RAVL_COMPILER_VISUALCPP)  /* Support integer file descriptors */
#define RAVL_HAVE_FDATASYNC    (RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_SOLARIS) /* have fdatasync */
#define RAVL_HAVE_LSEEK64      (RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_SOLARIS) /* have lseek64 */
#define RAVL_HAVE_CSQRT         !RAVL_OS_FREEBSD
#define RAVL_HAVE_FTW_H        RAVL_OS_POSIX

/********************************************************************************/
/****** Processor properties ****************************************************/

#define RAVL_LITTLEENDIAN (RAVL_CPU_IX86 || RAVL_CPU_ARM || RAVL_CPU_X86_64)    /* Little endian machine. */
#define RAVL_BIGENDIAN    (RAVL_CPU_PPC || !RAVL_LITTLEENDIAN)   /* Big endian machine. */
/* Yes there are other endian machines, but I've never actually met one. */

#define RAVL_BIGENDIANDOUBLES (RAVL_BIGENDIAN || RAVL_CPU_ARM)
/* ARM acts big-endian on it's double word ordering */

/* Setting the follow define to 1 will enable the use of MMX code. */
#define RAVL_USE_MMX (RAVL_CPU_IX86 && RAVL_COMPILER_GCC)

#define RAVL_HAVE_IEEE_DOUBLE 1   /* Does the machine have IEEE double's ? */

/********************************************************************************/
/****** Numerical functions and headers *****************************************/

#define RAVL_HAVE_VALUES_H (!RAVL_OS_WIN32 && !RAVL_OS_CYGWIN && !RAVL_OS_MACOSX && !RAVL_OS_FREEBSD)   /* have values.h        */
#define RAVL_HAVE_FLOAT_H  (RAVL_OS_WIN32  || RAVL_OS_CYGWIN || RAVL_OS_MACOSX) /* have float.h         */
#define RAVL_HAVE_NAN_H    (RAVL_OS_SOLARIS || RAVL_OS_IRIX) /* have nan.h           */
#define RAVL_HAVE_IEEEFP_H (RAVL_OS_SOLARIS || RAVL_OS_IRIX) /* have ieeefp.h        */
#define RAVL_HAVE_HYPOT    !RAVL_OS_WIN32   /* have hypot() in libm */
#define RAVL_HAVE_CBRT     (0 && (RAVL_OS_LINUX || RAVL_OS_LINUX64)) || RAVL_OS_FREEBSD /* have cbrt() in libm  */
#define RAVL_HAVE_ERF      (!RAVL_OS_WIN32 && !RAVL_OS_CYGWIN)   /* have erf() and erfc() in libm  */

#define RAVL_HAVE_ISINF    (RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_CYGWIN || RAVL_OS_MACOSX || RAVL_OS_FREEBSD)    /* have isinf() in libm  */
#define RAVL_HAVE__FINITE  RAVL_OS_WIN32    /* have _finite() in libm  */
#define RAVL_HAVE_FINITE   (RAVL_OS_SOLARIS || RAVL_OS_IRIX || RAVL_OS_OSF)  /* have finite() in libm  */

#define RAVL_HAVE_ISNAN    (RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_OSF || RAVL_OS_CYGWIN || RAVL_OS_MACOSX || RAVL_OS_FREEBSD)   /* have isnan() in libm  */
#define RAVL_HAVE__ISNAN   RAVL_OS_WIN32    /* have _isnan() in libm  */
#define RAVL_HAVE_ISNAND   (RAVL_OS_SOLARIS || RAVL_OS_IRIX) /* have isnand() in libm  */

#define RAVL_QINT_WORKAROUND RAVL_COMPILER_GCC3   /* Do we need a bug work around for the QInt functions (See Ravl/QInt.hh) */

#define RAVL_HAVE_LRINT     (RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_FREEBSD)/* Do we have lrint and friends ? This is a C99 extention*/

/********************************************************************************/
/****** Compiler/ C++ Library ***************************************************/

#define RAVL_HAVE_STDNAMESPACE    (RAVL_COMPILER_GCC3 || RAVL_COMPILER_GCC4 || _MSC_VER >= 1300) /* Use std namespace. */
#define RAVL_HAVE_ANSICPPHEADERS  (RAVL_COMPILER_GCC || _MSC_VER >= 1300) /* Use ansi complient c++ headers, i.e without the .h */
#define RAVL_HAVE_EXCEPTIONS      1                        /* are exceptions enabled ? */
#define RAVL_HAVE_EXCEPTION_H     !RAVL_COMPILER_VISUALCPP /* Have exception.h ? */
#define RAVL_HAVE_EXCEPTION_SPECS 0  /* These are deprecated */
#define RAVL_HAVE_RTTI            1                        /* is C++ Real Time Type Information available ? */
#define RAVL_HAVE_IOS_BINARY      !RAVL_COMPILER_MIPSPRO   /* do we have ios::binary.  */
#define RAVL_HAVE_IOS_SEEKDIR     (RAVL_HAVE_STDNAMESPACE || RAVL_COMPILER_GCC)  /* else assume ios::seek_dir exists. */
#define RAVL_HAVE_STREAMSIZE      (RAVL_HAVE_STDNAMESPACE || RAVL_COMPILER_GCC)  /* have streamsize type. */
#define RAVL_HAVE_STREAMASCLASS   !RAVL_HAVE_STDNAMESPACE    /* istream and ostream are classes not typedefs. */
#define RAVL_VISUALCPP_NAMESPACE_BUG RAVL_COMPILER_VISUALCPP /* Bug in namespace handling under Visual C++ 6.x */
#define RAVL_VISUALCPP_TYPENAME_BUG (RAVL_COMPILER_VISUALCPP && _MSC_VER < 1300) /* Restrictions in using keyword 'typename' in Visual C++ 6.x */
#define RAVL_ISTREAM_UNGET_BUG    RAVL_COMPILER_VISUALCPP    /* Bug in stream unget under Visual C++ 6.x */
#define RAVL_NEW_ANSI_CXX_DRAFT   (RAVL_COMPILER_GCC || RAVL_COMPILER_VISUALCPPNET) /* The mainly effects the use of <> in templated friend declarations */
#define RAVL_HAVE_STRINGSTREAM    (RAVL_COMPILER_GCC3 || RAVL_COMPILER_GCC4)        /* Use stringstream instead of strstream */
#define RAVL_HAVE_GCCCLASSVISIBILITY  RAVL_COMPILER_GCC4     /* Do we have the class visibility options introduced in GCC 4.0 */
#define RAVL_HAVE_TEMPLATEINSTANTIATE (RAVL_COMPILER_GCC3 && __GNUC_MINOR__ < 4)  /* Do we have forced template instanciation ? */
#define RAVL_HAVE_TEMPLATEREQUIREALLDEFINITIONS (RAVL_COMPILER_GCC3_4 || RAVL_COMPILER_GCC4)  /* Do we have forced template instanciation ? */

#define RAVL_HAVE_TEMPLATEDFUNCPTR_BUG (RAVL_COMPILER_VISUALCPP6) /* Do we have problems with resolving template arguments from function pointers ? Visual C++ 6.x */

/* Define a macro so we can easily switch in and out exception specs
// for functions.
*/
#if RAVL_HAVE_EXCEPTION_SPECS
  #define RAVL_THROW(x) throw(x)
  #define RAVL_THROW2(x,y) throw(x,y)
  #define RAVL_THROW3(x,y,z) throw(x,y,z)
#else
  #define RAVL_THROW(x)
  #define RAVL_THROW2(x,y)
  #define RAVL_THROW3(x,y,z)
#endif

/********************************************************************************/
/* Sort out some flags to ensure we get the right functions from the system
 * headers.
 */

#if (!RAVL_OS_IRIX && !RAVL_OS_FREEBSD && !RAVL_COMPILER_LLVM)
  #ifndef _GNU_SOURCE
    #define _GNU_SOURCE 1
  #endif
  #ifndef _POSIX_SOURCE
    #define _POSIX_SOURCE 1
  #endif
#endif

#if RAVL_OS_LINUX || RAVL_OS_LINUX64 || RAVL_OS_CYGWIN
  #ifndef _ISOC9X_SOURCE
    #define _ISOC9X_SOURCE 1
  #endif
#endif

#if RAVL_OS_OSF
  #ifndef _OSF_SOURCE
    #define _OSF_SOURCE 1
  #endif
  #ifndef _XOPEN_SOURCE
    #define _XOPEN_SOURCE 1
  #endif
  #ifndef _REENTRANT
    #define _REENTRANT 1
  #endif
#endif

/*********************************************************************
 * Shared library support
 */

#if RAVL_OS_WIN32
  #define RAVL_IMPORT __declspec(dllimport)
  #define RAVL_EXPORT __declspec(dllexport)
  #define RAVL_DLLLOCAL
  #define RAVL_DLLPUBLIC
#else
  #define RAVL_IMPORT
  #if RAVL_HAVE_GCCCLASSVISIBILITY
    #define RAVL_EXPORT __attribute__ ((visibility("default")))
    #define RAVL_DLLLOCAL __attribute__ ((visibility("hidden")))
    #define RAVL_DLLPUBLIC __attribute__ ((visibility("default")))
  #else
    #define RAVL_EXPORT
    #define RAVL_DLLLOCAL
    #define RAVL_DLLPUBLIC
  #endif
#endif

/* Define FXAPI for DLL builds */
#ifdef RAVL_DLL
  #ifdef RAVL_DLL_EXPORTS
    #define RAVL_API RAVL_EXPORT
  #else
    #define RAVL_API RAVL_IMPORT
  #endif /* RAVL_DLL_EXPORTS  */
#else
  #define RAVL_API
#endif /* RAVL_DLL */

/* Fix for threading primitives on Mac OSX */
#ifdef __APPLE__
#ifndef _DARWIN_C_SOURCE
#define _DARWIN_C_SOURCE
#endif
#endif

/* Throwable classes must always be visible on GCC in all binaries */
#if RAVL_OS_WIN32
  #define RAVL_EXCEPTIONAPI(api) api
#elif  RAVL_HAVE_GCCCLASSVISIBILITY
  #define RAVL_EXCEPTIONAPI(api) RAVL_EXPORT
#else
  #define RAVL_EXCEPTIONAPI(api)
#endif

#endif
