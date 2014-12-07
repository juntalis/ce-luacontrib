#ifndef _FFI_STUBS_H_
#define _FFI_STUBS_H_
#pragma once

#include <stdint.h>
#if __STDC_VERSION__+0 >= 199901L
#	include <complex.h>
#	define HAVE_COMPLEX
#	define HAVE_LONG_DOUBLE
#endif

#ifndef DASM_STATE_DEFINED
#	define DASM_STATE_DEFINED
	typedef struct dasm_State dasm_State;
#endif

struct parser {
    int line;
    const char* next;
    const char* prev;
    unsigned align_mask;
};

struct page {
    size_t size;
    size_t off;
    size_t freed;
};

struct jit {
    lua_State* L;
    int32_t last_errno;
    dasm_State* ctx;
    size_t pagenum;
    struct page** pages;
    size_t align_page_size;
    void** globals;
    int function_extern;
    void* lua_dll;
    void* kernel32_dll;
};

struct ctype;
typedef void (*cfunction)(void);

#ifdef HAVE_COMPLEX
	typedef double complex complex_double;
	typedef float complex complex_float;
#else
	typedef struct {
		double real, imag;
	} complex_double;

	typedef struct {
		float real, imag;
	} complex_float;

	static double creal(complex_double c) {
		return c.real;
	}
	static float crealf(complex_float c) {
		return c.real;
	}

	static double cimag(complex_double c) {
		return c.imag;
	}
	static float cimagf(complex_float c) {
		return c.imag;
	}
#endif

double check_double(lua_State* L, int idx);
float check_float(lua_State* L, int idx);

int32_t check_int32(lua_State* L, int idx);
int64_t check_int64(lua_State* L, int idx);

uint32_t check_uint32(lua_State* L, int idx);
uint64_t check_uint64(lua_State* L, int idx);

uintptr_t check_uintptr(lua_State* L, int idx);
int32_t check_enum(lua_State* L, int idx, int to_usr, const struct ctype* tt);

void* check_typed_pointer(lua_State* L, int idx, int to_usr, const struct ctype* tt);
cfunction check_typed_cfunction(lua_State* L, int idx, int to_usr, const struct ctype* tt);

complex_double check_complex_double(lua_State* L, int idx);
complex_float check_complex_float(lua_State* L, int idx);

void unpack_varargs_stack(lua_State* L, int first, int last, char* to);
void unpack_varargs_stack_skip(lua_State* L, int first, int last, int ints_to_skip, int floats_to_skip, char* to);
void unpack_varargs_reg(lua_State* L, int first, int last, char* to);
void unpack_varargs_float(lua_State* L, int first, int last, int max, char* to);
void unpack_varargs_int(lua_State* L, int first, int last, int max, char* to);

#ifndef NDEBUG
#	define DASM_CHECKS
#endif

#ifdef Dst_DECL
#	undef Dst_DECL
#endif

#ifdef Dst_REF
#	undef Dst_REF
#endif

#define Dst_DECL	struct jit* Dst
#define Dst_REF		(Dst->ctx)

#define DASM_EXTERN(a,b,c,d) get_extern(a,b,c,d)

#include "dynasm/dasm_proto.h"

#endif /* _FFI_STUBS_H_ */
