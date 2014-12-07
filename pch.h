/**
 * @file tinypch.h
 * @brief Precompiled header (maybe)
 *
 * This is sort of a general-use header I've been throwing into a lot of projects
 * for the sake of convenience. Despite the name, whether or not this header is
 * actually being used as a precompiled header is dependent on the size of the
 * project I'm currently using it for.
 */
#ifndef _PRECOMPILED_H_
#define _PRECOMPILED_H_
#pragma once

// Macro used to combine two things. (Including other macros)
#define XMERGE2(A,B) A##B
#define XMERGE(A,B)  XMERGE2(A,B)

// Stringifying macros
#define XWIDEN(X) XMERGE2(L,X)
#define XSTR2(X) #X
#define XSTRA(X) XSTR2(X)
#define XSTRW(X) XWIDEN(XSTRA(X))

// Compiler/MSVC Version Detection and Warning Macros
#if defined(_MSC_VER) && !defined(RC_INVOKED) && !defined(__clang__)
#	if _MSC_VER == 1200
#		define COMPILER_VC6
#	elif _MSC_VER == 1300
#		define COMPILER_VC7
#	elif _MSC_VER == 1310
#		define COMPILER_VC2003
#	elif (_MSC_VER >= 1400) && (_MSC_VER < 1500)
#		define COMPILER_VC2005
#	elif (_MSC_VER >= 1500) && (_MSC_VER < 1600)
#		define COMPILER_VC2008
#	elif (_MSC_VER >= 1600) && (_MSC_VER < 1700)
#		define COMPILER_VC2010
#	elif (_MSC_VER >= 1700) && (_MSC_VER < 1800)
#		define COMPILER_VC2012
#	else
#		pragma message("_MSC_VER defined as " XSTRA(_MSC_VER))
#		error Did not recognize MSVC version!
#	endif
#	define COMPILER_MSVC
#	define XWARN(M) \
	__pragma(message( "[" __FILE__ "(" XSTRA(__LINE__) ")][WARNING]: " M ))
#	define XMSG(M) \
	__pragma(message( "[" __FILE__ "(" XSTRA(__LINE__) ")][INFO:" __TIMESTAMP__ "]: " M ))
#else
#	if defined(__clang__)
#		define COMPILER_CLANG
#	elif defined(RC_INVOKED)
#		define COMPILER_RC
#	elif (defined(__MINGW32__) || defined(__GNUC__))
#		define COMPILER_GCC
#	endif
#	if defined(COMPILER_CLANG) || defined(COMPILER_GCC)
#		define XWARN(M) \
	__Pragma(message( "[" __FILE__ "(" XSTRA(__LINE__) ")][WARNING]: " M ))
#		define XMSG(M) \
	__Pragma(message( "[" __FILE__ "(" XSTRA(__LINE__) ")][INFO:" __TIMESTAMP__ "]: " M ))
#	endif
#endif

/* Windows Version */
// - Target Windows XP and higher.
// - Throw a compile-time error if a lower platform is detected.
#if !defined(WINVER)
#	define WINVER 0x0501
#elif (WINVER < 0x0501)
#	error Windows XP is currently the lowest version of Windows supported by this project.
#endif
#if !defined(_WIN32_WINNT)
#	define _WIN32_WINNT 0x0501
#elif (_WIN32_WINNT < 0x0501)
#	error Windows XP is currently the lowest version of Windows supported by this project.
#endif
#if !defined(NTDDI_VERSION)
#	define NTDDI_VERSION 0x05010000
#elif (NTDDI_VERSION < 0x05010000)
#	error Windows XP is currently the lowest version of Windows supported by this project.
#endif

// Architecture Detection
#if defined(_M_IA64) || defined(_M_AMD64) || defined(_M_X64)
#	define BUILD_ARCH_X64
#	define BUILD_ARCH_BITS 64
#	ifndef _WIN64
#		define _WIN64
#	endif
#elif defined(_M_IX86)
#	define BUILD_ARCH_X86
#		define BUILD_ARCH_BITS 32
#endif

#ifndef BUILD_ARCH_BITS
#	error Could not detect platform architecture.
#endif

// Speed up build process with minimal headers.
#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#ifndef VC_EXTRALEAN
#	define VC_EXTRALEAN
#endif

#ifdef COMPILER_MSVC
/* Warning Suppression */
//	Insecure function usage warnings
#	ifndef _CRT_SECURE_NO_WARNINGS
#		define _CRT_SECURE_NO_WARNINGS
#	endif
#	ifndef _CRT_NON_CONFORMING_SWPRINTFS
#		define _CRT_NON_CONFORMING_SWPRINTFS
#	endif
//	Depreciation warnings
#	pragma warning(disable:4996)
//	WinSDK Version
#	include <SDKDDKVer.h>
#endif

#include <windows.h>
#include <winnt.h>
#include <psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include <limits.h>

#ifdef COMPILER_MSVC
#	pragma comment(lib, "user32.lib")
#	if (PSAPI_VERSION == 1)
#		pragma comment(lib, "psapi.lib")
#	endif
#	ifdef inline
#		undef inline
#	endif
#	define inline __forceinline
	typedef   signed __int8    s8;
	typedef   signed __int16   s16;
	typedef   signed __int32   s32;
	typedef   signed __int64   s64;
	typedef   signed __int3264 sPtr;
	typedef unsigned __int8    u8;
	typedef unsigned __int16   u16;
	typedef unsigned __int32   u32;
	typedef unsigned __int64   u64;
	typedef unsigned __int3264 uPtr;
#else
#	ifdef inline
#		undef inline
#	endif
#	define inline __inline
#	include <stdint.h>
#	ifndef _INTPTR_T_DEFINED
#		ifdef BUILD_ARCH_X64
		typedef int64_t  intptr_t;
		typedef uint64_t uintptr_t;
#		else
		typedef int32_t  intptr_t;
		typedef uint32_t uintptr_t;
#		endif
#	endif
	typedef int8_t    s8;
	typedef int16_t   s16;
	typedef int32_t   s32;
	typedef int64_t   s64;
	typedef intptr_t  sPtr;
	typedef uint8_t   u8;
	typedef uint16_t  u16;
	typedef uint32_t  u32;
	typedef uint64_t  u64;
	typedef uintptr_t uPtr;
#endif

#define CPP_UTYP(BITS) XMERGE(UI,BITS)
#define CPP_STYP(BITS) XMERGE(SI,BITS)

#define X2TYP(X,TYP) ((TYP)(X))
#define X2UTYP(X,BITS) X2TYP(X,CPP_UTYP(BITS))
#define X2STYP(X,BITS) X2TYP(X,CPP_STYP(BITS))

#define x2ptr(X) X2TYP(X, UPTR)
#define ptr2x(X) x2ptr(&(X))

#define x2u8(X)  X2UTYP(X,8)
#define x2u16(X) X2UTYP(X,16)
#define x2u32(X) X2UTYP(X,32)
#define x2u64(X) X2UTYP(X,64)

#define x2s8(X)  X2STYP(X,8)
#define x2s16(X) X2STYP(X,16)
#define x2s32(X) X2STYP(X,32)
#define x2s64(X) X2STYP(X,64)

/** Lastly, our project-specific includes/declarations */
#define WITH_LUADLL_CONFIG

#include "luadll.h"
#include "packaged/resource.h"

#define xfree(PTR) _xfree_((void*)(PTR))
void xmsg(char* message);
void _xfree_(void* ptr);
void* xalloc(size_t szBuf);

// Pass-thrus to CE exports
DWORD CE_GetPID(void);
lua_State* CE_GetLuaState(void);
HMODULE GetModuleNoRef(LPCSTR sModule);
HANDLE CE_GetProcessHandle(void);

#undef CPP_UTYP
#undef X2STYP
#undef X2UTYP

#endif /* _PRECOMPILED_H_ */
