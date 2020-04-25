#ifndef FANNCOMMON_HEADER 
#define FANNCOMMON_HEADER  1


 
#ifndef NULL
#define NULL 0
#endif	/* NULL */
 
/* ----- Macros used to define DLL external entrypoints ----- */ 
/*
 DLL Export, import and calling convention for Windows.
 Only defined for Microsoft VC++ FANN_EXTERNAL indicates
 that a function will be exported/imported from a dll
 FANN_API ensures that the DLL calling convention
 will be used for  a function regardless of the calling convention
 used when compiling.

 For a function to be exported from a DLL its prototype and
 declaration must be like this:
    FANN_EXTERNAL void FANN_API function(char *argument)

 The following ifdef block is a way of creating macros which
 make exporting from a DLL simple. All files within a DLL are
 compiled with the FANN_DLL_EXPORTS symbol defined on the
 command line. This symbol should not be defined on any project
 that uses this DLL. This way any other project whose source
 files include this file see FANN_EXTERNAL functions as being imported
 from a DLL, whereas a DLL sees symbols defined with this
 macro as being exported which makes calls more efficient.
 The __stdcall calling convention is used for functions in a
 windows DLL.

 The callback functions for fann_set_callback must be declared as FANN_API
 so the DLL and the application program both use the same
 calling convention.
*/ 
 
/*
 The following sets the default for MSVC++ 2003 or later to use
 the fann dll's. To use a lib or fixedfann.c, floatfann.c or doublefann.c
 with those compilers FANN_NO_DLL has to be defined before
 including the fann headers.
 The default for previous MSVC compilers such as VC++ 6 is not
 to use dll's. To use dll's FANN_USE_DLL has to be defined before
 including the fann headers.
*/ 
#if (_MSC_VER > 1300)
#ifndef FANN_NO_DLL
#define FANN_USE_DLL
#endif	/* FANN_USE_LIB */
#endif	/* _MSC_VER */
#if defined(_MSC_VER) && (defined(FANN_USE_DLL) || defined(FANN_DLL_EXPORTS))
#ifdef FANN_DLL_EXPORTS
#define FANN_EXTERNAL __declspec(dllexport)
#else							/*  */
#define FANN_EXTERNAL __declspec(dllimport)
#endif	/* FANN_DLL_EXPORTS*/
#define FANN_API __stdcall
#else							/*  */
#define FANN_EXTERNAL
#define FANN_API
#endif	/* _MSC_VER */

#endif
