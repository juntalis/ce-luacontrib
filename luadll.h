#ifndef _LUA_DLL_H_
#define _LUA_DLL_H_

#ifndef _PRECOMPILED_H_
#	include "precompiled.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

#ifdef LUA_JITLIBNAME
#	include "lua/luajit.h"
#	define LUA_FLAVOR_JIT
#	define LUA_FLAVOR_NAME "luajit"
#else
#	define LUA_FLAVOR_CLASSIC
#	define LUA_FLAVOR_NAME "lua"
#endif

#if defined(_M_AMD64) || defined(_M_IA64) || defined(WIN64) || defined(_WIN64)
#	define LUA_PLATFORM_BITS "64"
#else
#	define LUA_PLATFORM_BITS "32"
#endif

#ifdef WITH_LUADLL_CONFIG
#	include "luadll_config.h"
#endif

#ifndef LUADLL_BASE
#	define LUADLL_BASE "lua5.1-"
#endif

#ifndef LUADLL_EXT
#	define LUADLL_EXT ".dll"
#endif

#ifndef LUADLL_OVERRIDE
#	define LUADLL_OVERRIDE 1
#endif

#ifndef LUADLL_HMODULE
#	define LUADLL_HMODULE HMODULE
#endif

#ifndef LUADLL_GETLIB
#	define LUADLL_GETLIB LoadLibraryA
#endif

#ifndef LUADLL_FREELIB
#	define LUADLL_FREELIB FreeLibrary
#endif

#ifndef LUADLL_GETPROC
#	define LUADLL_GETPROC GetProcAddress
#endif

#ifndef LUADLL_STATICLIB
#	define LUADLL_STATICLIB "luadll" LUA_PLATFORM_BITS ".lib"
#endif

#ifndef LUADLL_CDECL
#	define LUADLL_CDECL __cdecl
#endif

typedef struct luadll_imports_t {
	LUADLL_HMODULE _hModLua;
	/*[[[cog
	import cog
	for fn in funcs:
		cog.outl('\t{0} (LUADLL_CDECL* imp_{1})({2});'.format(fn.restype, fn.name, ', '.join(fn.args)))
	]]]*/
	lua_State * (LUADLL_CDECL* imp_lua_newstate)(lua_Alloc f, void *ud);
	void (LUADLL_CDECL* imp_lua_close)(lua_State *L);
	lua_State * (LUADLL_CDECL* imp_lua_newthread)(lua_State *L);
	lua_CFunction (LUADLL_CDECL* imp_lua_atpanic)(lua_State *L, lua_CFunction panicf);
	int (LUADLL_CDECL* imp_lua_gettop)(lua_State *L);
	void (LUADLL_CDECL* imp_lua_settop)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_pushvalue)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_remove)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_insert)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_replace)(lua_State *L, int idx);
	int (LUADLL_CDECL* imp_lua_checkstack)(lua_State *L, int sz);
	void (LUADLL_CDECL* imp_lua_xmove)(lua_State *from, lua_State *to, int n);
	int (LUADLL_CDECL* imp_lua_isnumber)(lua_State *L, int idx);
	int (LUADLL_CDECL* imp_lua_isstring)(lua_State *L, int idx);
	int (LUADLL_CDECL* imp_lua_iscfunction)(lua_State *L, int idx);
	int (LUADLL_CDECL* imp_lua_isuserdata)(lua_State *L, int idx);
	int (LUADLL_CDECL* imp_lua_type)(lua_State *L, int idx);
	const char * (LUADLL_CDECL* imp_lua_typename)(lua_State *L, int tp);
	int (LUADLL_CDECL* imp_lua_equal)(lua_State *L, int idx1, int idx2);
	int (LUADLL_CDECL* imp_lua_rawequal)(lua_State *L, int idx1, int idx2);
	int (LUADLL_CDECL* imp_lua_lessthan)(lua_State *L, int idx1, int idx2);
	lua_Number (LUADLL_CDECL* imp_lua_tonumber)(lua_State *L, int idx);
	lua_Integer (LUADLL_CDECL* imp_lua_tointeger)(lua_State *L, int idx);
	int (LUADLL_CDECL* imp_lua_toboolean)(lua_State *L, int idx);
	const char * (LUADLL_CDECL* imp_lua_tolstring)(lua_State *L, int idx, size_t *len);
	size_t (LUADLL_CDECL* imp_lua_objlen)(lua_State *L, int idx);
	lua_CFunction (LUADLL_CDECL* imp_lua_tocfunction)(lua_State *L, int idx);
	void * (LUADLL_CDECL* imp_lua_touserdata)(lua_State *L, int idx);
	lua_State * (LUADLL_CDECL* imp_lua_tothread)(lua_State *L, int idx);
	const void * (LUADLL_CDECL* imp_lua_topointer)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_pushnil)(lua_State *L);
	void (LUADLL_CDECL* imp_lua_pushnumber)(lua_State *L, lua_Number n);
	void (LUADLL_CDECL* imp_lua_pushinteger)(lua_State *L, lua_Integer n);
	void (LUADLL_CDECL* imp_lua_pushlstring)(lua_State *L, const char *s, size_t l);
	void (LUADLL_CDECL* imp_lua_pushstring)(lua_State *L, const char *s);
	const char * (LUADLL_CDECL* imp_lua_pushvfstring)(lua_State *L, const char *fmt, va_list argp);
	const char * (LUADLL_CDECL* imp_lua_pushfstring)(lua_State *L, const char *fmt, ...);
	void (LUADLL_CDECL* imp_lua_pushcclosure)(lua_State *L, lua_CFunction fn, int n);
	void (LUADLL_CDECL* imp_lua_pushboolean)(lua_State *L, int b);
	void (LUADLL_CDECL* imp_lua_pushlightuserdata)(lua_State *L, void *p);
	int (LUADLL_CDECL* imp_lua_pushthread)(lua_State *L);
	void (LUADLL_CDECL* imp_lua_gettable)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_getfield)(lua_State *L, int idx, const char *k);
	void (LUADLL_CDECL* imp_lua_rawget)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_rawgeti)(lua_State *L, int idx, int n);
	void (LUADLL_CDECL* imp_lua_createtable)(lua_State *L, int narr, int nrec);
	void * (LUADLL_CDECL* imp_lua_newuserdata)(lua_State *L, size_t sz);
	int (LUADLL_CDECL* imp_lua_getmetatable)(lua_State *L, int objindex);
	void (LUADLL_CDECL* imp_lua_getfenv)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_settable)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_setfield)(lua_State *L, int idx, const char *k);
	void (LUADLL_CDECL* imp_lua_rawset)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_rawseti)(lua_State *L, int idx, int n);
	int (LUADLL_CDECL* imp_lua_setmetatable)(lua_State *L, int objindex);
	int (LUADLL_CDECL* imp_lua_setfenv)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_call)(lua_State *L, int nargs, int nresults);
	int (LUADLL_CDECL* imp_lua_pcall)(lua_State *L, int nargs, int nresults, int errfunc);
	int (LUADLL_CDECL* imp_lua_cpcall)(lua_State *L, lua_CFunction func, void *ud);
	int (LUADLL_CDECL* imp_lua_load)(lua_State *L, lua_Reader reader, void *dt, const char *chunkname);
	int (LUADLL_CDECL* imp_lua_dump)(lua_State *L, lua_Writer writer, void *data);
	int (LUADLL_CDECL* imp_lua_yield)(lua_State *L, int nresults);
	int (LUADLL_CDECL* imp_lua_resume)(lua_State *L, int narg);
	int (LUADLL_CDECL* imp_lua_status)(lua_State *L);
	int (LUADLL_CDECL* imp_lua_gc)(lua_State *L, int what, int data);
	int (LUADLL_CDECL* imp_lua_error)(lua_State *L);
	int (LUADLL_CDECL* imp_lua_next)(lua_State *L, int idx);
	void (LUADLL_CDECL* imp_lua_concat)(lua_State *L, int n);
	lua_Alloc (LUADLL_CDECL* imp_lua_getallocf)(lua_State *L, void **ud);
	void (LUADLL_CDECL* imp_lua_setallocf)(lua_State *L, lua_Alloc f, void *ud);
	void (LUADLL_CDECL* imp_lua_setlevel)(lua_State *from, lua_State *to);
	int (LUADLL_CDECL* imp_lua_getstack)(lua_State *L, int level, lua_Debug *ar);
	int (LUADLL_CDECL* imp_lua_getinfo)(lua_State *L, const char *what, lua_Debug *ar);
	const char * (LUADLL_CDECL* imp_lua_getlocal)(lua_State *L, const lua_Debug *ar, int n);
	const char * (LUADLL_CDECL* imp_lua_setlocal)(lua_State *L, const lua_Debug *ar, int n);
	const char * (LUADLL_CDECL* imp_lua_getupvalue)(lua_State *L, int funcindex, int n);
	const char * (LUADLL_CDECL* imp_lua_setupvalue)(lua_State *L, int funcindex, int n);
	int (LUADLL_CDECL* imp_lua_sethook)(lua_State *L, lua_Hook func, int mask, int count);
	lua_Hook (LUADLL_CDECL* imp_lua_gethook)(lua_State *L);
	int (LUADLL_CDECL* imp_lua_gethookmask)(lua_State *L);
	int (LUADLL_CDECL* imp_lua_gethookcount)(lua_State *L);
	void (LUADLL_CDECL* imp_luaL_openlib)(lua_State *L, const char *libname, const luaL_Reg *l, int nup);
	void (LUADLL_CDECL* imp_luaL_register)(lua_State *L, const char *libname, const luaL_Reg *l);
	int (LUADLL_CDECL* imp_luaL_getmetafield)(lua_State *L, int obj, const char *e);
	int (LUADLL_CDECL* imp_luaL_callmeta)(lua_State *L, int obj, const char *e);
	int (LUADLL_CDECL* imp_luaL_typerror)(lua_State *L, int narg, const char *tname);
	int (LUADLL_CDECL* imp_luaL_argerror)(lua_State *L, int numarg, const char *extramsg);
	const char * (LUADLL_CDECL* imp_luaL_checklstring)(lua_State *L, int numArg, size_t *l);
	const char * (LUADLL_CDECL* imp_luaL_optlstring)(lua_State *L, int numArg, const char *def, size_t *l);
	lua_Number (LUADLL_CDECL* imp_luaL_checknumber)(lua_State *L, int numArg);
	lua_Number (LUADLL_CDECL* imp_luaL_optnumber)(lua_State *L, int nArg, lua_Number def);
	lua_Integer (LUADLL_CDECL* imp_luaL_checkinteger)(lua_State *L, int numArg);
	lua_Integer (LUADLL_CDECL* imp_luaL_optinteger)(lua_State *L, int nArg, lua_Integer def);
	void (LUADLL_CDECL* imp_luaL_checkstack)(lua_State *L, int sz, const char *msg);
	void (LUADLL_CDECL* imp_luaL_checktype)(lua_State *L, int narg, int t);
	void (LUADLL_CDECL* imp_luaL_checkany)(lua_State *L, int narg);
	int (LUADLL_CDECL* imp_luaL_newmetatable)(lua_State *L, const char *tname);
	void * (LUADLL_CDECL* imp_luaL_checkudata)(lua_State *L, int ud, const char *tname);
	void (LUADLL_CDECL* imp_luaL_where)(lua_State *L, int lvl);
	int (LUADLL_CDECL* imp_luaL_error)(lua_State *L, const char *fmt, ...);
	int (LUADLL_CDECL* imp_luaL_checkoption)(lua_State *L, int narg, const char *def, const char *const lst[]);
	int (LUADLL_CDECL* imp_luaL_ref)(lua_State *L, int t);
	void (LUADLL_CDECL* imp_luaL_unref)(lua_State *L, int t, int ref);
	int (LUADLL_CDECL* imp_luaL_loadfile)(lua_State *L, const char *filename);
	int (LUADLL_CDECL* imp_luaL_loadbuffer)(lua_State *L, const char *buff, size_t sz, const char *name);
	int (LUADLL_CDECL* imp_luaL_loadstring)(lua_State *L, const char *s);
	lua_State * (LUADLL_CDECL* imp_luaL_newstate)();
	const char * (LUADLL_CDECL* imp_luaL_gsub)(lua_State *L, const char *s, const char *p, const char *r);
	const char * (LUADLL_CDECL* imp_luaL_findtable)(lua_State *L, int idx, const char *fname, int szhint);
	void (LUADLL_CDECL* imp_luaL_buffinit)(lua_State *L, luaL_Buffer *B);
	char * (LUADLL_CDECL* imp_luaL_prepbuffer)(luaL_Buffer *B);
	void (LUADLL_CDECL* imp_luaL_addlstring)(luaL_Buffer *B, const char *s, size_t l);
	void (LUADLL_CDECL* imp_luaL_addstring)(luaL_Buffer *B, const char *s);
	void (LUADLL_CDECL* imp_luaL_addvalue)(luaL_Buffer *B);
	void (LUADLL_CDECL* imp_luaL_pushresult)(luaL_Buffer *B);
	int (LUADLL_CDECL* imp_luaopen_base)(lua_State *L);
	int (LUADLL_CDECL* imp_luaopen_table)(lua_State *L);
	int (LUADLL_CDECL* imp_luaopen_io)(lua_State *L);
	int (LUADLL_CDECL* imp_luaopen_os)(lua_State *L);
	int (LUADLL_CDECL* imp_luaopen_string)(lua_State *L);
	int (LUADLL_CDECL* imp_luaopen_math)(lua_State *L);
	int (LUADLL_CDECL* imp_luaopen_debug)(lua_State *L);
	int (LUADLL_CDECL* imp_luaopen_package)(lua_State *L);
	void (LUADLL_CDECL* imp_luaL_openlibs)(lua_State *L);
	//[[[end]]]
} luadll_imports_t;

extern luadll_imports_t* (LUADLL_CDECL *lp_get_lua_imports_func)(void);

#ifndef LUADLL_SRC
#	define LUADLL_OVERRIDE_IMPORTS
#	ifndef LUADLL_NOLINK
#		pragma comment(lib, LUADLL_STATICLIB)
#	endif
#endif

void LUADLL_CDECL luadll_destroy(void);
LUADLL_HMODULE LUADLL_CDECL luadll_get_hmodule(void);
#define get_lua_imports() (*lp_get_lua_imports_func)()

#ifdef __cplusplus
}
#endif

#endif /* _LUA_DLL_H_ */

#if defined(LUADLL_OVERRIDE_IMPORTS) && !defined(LUADLL_IMPORTS_OVERRIDEN)
/*[[[cog
import cog
for fn in funcs:
	cog.outl('#\tdefine {0}(...) get_lua_imports()->imp_{0}( __VA_ARGS__ )'.format(fn.name))
]]]*/
#	define lua_newstate(...) get_lua_imports()->imp_lua_newstate( __VA_ARGS__ )
#	define lua_close(...) get_lua_imports()->imp_lua_close( __VA_ARGS__ )
#	define lua_newthread(...) get_lua_imports()->imp_lua_newthread( __VA_ARGS__ )
#	define lua_atpanic(...) get_lua_imports()->imp_lua_atpanic( __VA_ARGS__ )
#	define lua_gettop(...) get_lua_imports()->imp_lua_gettop( __VA_ARGS__ )
#	define lua_settop(...) get_lua_imports()->imp_lua_settop( __VA_ARGS__ )
#	define lua_pushvalue(...) get_lua_imports()->imp_lua_pushvalue( __VA_ARGS__ )
#	define lua_remove(...) get_lua_imports()->imp_lua_remove( __VA_ARGS__ )
#	define lua_insert(...) get_lua_imports()->imp_lua_insert( __VA_ARGS__ )
#	define lua_replace(...) get_lua_imports()->imp_lua_replace( __VA_ARGS__ )
#	define lua_checkstack(...) get_lua_imports()->imp_lua_checkstack( __VA_ARGS__ )
#	define lua_xmove(...) get_lua_imports()->imp_lua_xmove( __VA_ARGS__ )
#	define lua_isnumber(...) get_lua_imports()->imp_lua_isnumber( __VA_ARGS__ )
#	define lua_isstring(...) get_lua_imports()->imp_lua_isstring( __VA_ARGS__ )
#	define lua_iscfunction(...) get_lua_imports()->imp_lua_iscfunction( __VA_ARGS__ )
#	define lua_isuserdata(...) get_lua_imports()->imp_lua_isuserdata( __VA_ARGS__ )
#	define lua_type(...) get_lua_imports()->imp_lua_type( __VA_ARGS__ )
#	define lua_typename(...) get_lua_imports()->imp_lua_typename( __VA_ARGS__ )
#	define lua_equal(...) get_lua_imports()->imp_lua_equal( __VA_ARGS__ )
#	define lua_rawequal(...) get_lua_imports()->imp_lua_rawequal( __VA_ARGS__ )
#	define lua_lessthan(...) get_lua_imports()->imp_lua_lessthan( __VA_ARGS__ )
#	define lua_tonumber(...) get_lua_imports()->imp_lua_tonumber( __VA_ARGS__ )
#	define lua_tointeger(...) get_lua_imports()->imp_lua_tointeger( __VA_ARGS__ )
#	define lua_toboolean(...) get_lua_imports()->imp_lua_toboolean( __VA_ARGS__ )
#	define lua_tolstring(...) get_lua_imports()->imp_lua_tolstring( __VA_ARGS__ )
#	define lua_objlen(...) get_lua_imports()->imp_lua_objlen( __VA_ARGS__ )
#	define lua_tocfunction(...) get_lua_imports()->imp_lua_tocfunction( __VA_ARGS__ )
#	define lua_touserdata(...) get_lua_imports()->imp_lua_touserdata( __VA_ARGS__ )
#	define lua_tothread(...) get_lua_imports()->imp_lua_tothread( __VA_ARGS__ )
#	define lua_topointer(...) get_lua_imports()->imp_lua_topointer( __VA_ARGS__ )
#	define lua_pushnil(...) get_lua_imports()->imp_lua_pushnil( __VA_ARGS__ )
#	define lua_pushnumber(...) get_lua_imports()->imp_lua_pushnumber( __VA_ARGS__ )
#	define lua_pushinteger(...) get_lua_imports()->imp_lua_pushinteger( __VA_ARGS__ )
#	define lua_pushlstring(...) get_lua_imports()->imp_lua_pushlstring( __VA_ARGS__ )
#	define lua_pushstring(...) get_lua_imports()->imp_lua_pushstring( __VA_ARGS__ )
#	define lua_pushvfstring(...) get_lua_imports()->imp_lua_pushvfstring( __VA_ARGS__ )
#	define lua_pushfstring(...) get_lua_imports()->imp_lua_pushfstring( __VA_ARGS__ )
#	define lua_pushcclosure(...) get_lua_imports()->imp_lua_pushcclosure( __VA_ARGS__ )
#	define lua_pushboolean(...) get_lua_imports()->imp_lua_pushboolean( __VA_ARGS__ )
#	define lua_pushlightuserdata(...) get_lua_imports()->imp_lua_pushlightuserdata( __VA_ARGS__ )
#	define lua_pushthread(...) get_lua_imports()->imp_lua_pushthread( __VA_ARGS__ )
#	define lua_gettable(...) get_lua_imports()->imp_lua_gettable( __VA_ARGS__ )
#	define lua_getfield(...) get_lua_imports()->imp_lua_getfield( __VA_ARGS__ )
#	define lua_rawget(...) get_lua_imports()->imp_lua_rawget( __VA_ARGS__ )
#	define lua_rawgeti(...) get_lua_imports()->imp_lua_rawgeti( __VA_ARGS__ )
#	define lua_createtable(...) get_lua_imports()->imp_lua_createtable( __VA_ARGS__ )
#	define lua_newuserdata(...) get_lua_imports()->imp_lua_newuserdata( __VA_ARGS__ )
#	define lua_getmetatable(...) get_lua_imports()->imp_lua_getmetatable( __VA_ARGS__ )
#	define lua_getfenv(...) get_lua_imports()->imp_lua_getfenv( __VA_ARGS__ )
#	define lua_settable(...) get_lua_imports()->imp_lua_settable( __VA_ARGS__ )
#	define lua_setfield(...) get_lua_imports()->imp_lua_setfield( __VA_ARGS__ )
#	define lua_rawset(...) get_lua_imports()->imp_lua_rawset( __VA_ARGS__ )
#	define lua_rawseti(...) get_lua_imports()->imp_lua_rawseti( __VA_ARGS__ )
#	define lua_setmetatable(...) get_lua_imports()->imp_lua_setmetatable( __VA_ARGS__ )
#	define lua_setfenv(...) get_lua_imports()->imp_lua_setfenv( __VA_ARGS__ )
#	define lua_call(...) get_lua_imports()->imp_lua_call( __VA_ARGS__ )
#	define lua_pcall(...) get_lua_imports()->imp_lua_pcall( __VA_ARGS__ )
#	define lua_cpcall(...) get_lua_imports()->imp_lua_cpcall( __VA_ARGS__ )
#	define lua_load(...) get_lua_imports()->imp_lua_load( __VA_ARGS__ )
#	define lua_dump(...) get_lua_imports()->imp_lua_dump( __VA_ARGS__ )
#	define lua_yield(...) get_lua_imports()->imp_lua_yield( __VA_ARGS__ )
#	define lua_resume(...) get_lua_imports()->imp_lua_resume( __VA_ARGS__ )
#	define lua_status(...) get_lua_imports()->imp_lua_status( __VA_ARGS__ )
#	define lua_gc(...) get_lua_imports()->imp_lua_gc( __VA_ARGS__ )
#	define lua_error(...) get_lua_imports()->imp_lua_error( __VA_ARGS__ )
#	define lua_next(...) get_lua_imports()->imp_lua_next( __VA_ARGS__ )
#	define lua_concat(...) get_lua_imports()->imp_lua_concat( __VA_ARGS__ )
#	define lua_getallocf(...) get_lua_imports()->imp_lua_getallocf( __VA_ARGS__ )
#	define lua_setallocf(...) get_lua_imports()->imp_lua_setallocf( __VA_ARGS__ )
#	define lua_setlevel(...) get_lua_imports()->imp_lua_setlevel( __VA_ARGS__ )
#	define lua_getstack(...) get_lua_imports()->imp_lua_getstack( __VA_ARGS__ )
#	define lua_getinfo(...) get_lua_imports()->imp_lua_getinfo( __VA_ARGS__ )
#	define lua_getlocal(...) get_lua_imports()->imp_lua_getlocal( __VA_ARGS__ )
#	define lua_setlocal(...) get_lua_imports()->imp_lua_setlocal( __VA_ARGS__ )
#	define lua_getupvalue(...) get_lua_imports()->imp_lua_getupvalue( __VA_ARGS__ )
#	define lua_setupvalue(...) get_lua_imports()->imp_lua_setupvalue( __VA_ARGS__ )
#	define lua_sethook(...) get_lua_imports()->imp_lua_sethook( __VA_ARGS__ )
#	define lua_gethook(...) get_lua_imports()->imp_lua_gethook( __VA_ARGS__ )
#	define lua_gethookmask(...) get_lua_imports()->imp_lua_gethookmask( __VA_ARGS__ )
#	define lua_gethookcount(...) get_lua_imports()->imp_lua_gethookcount( __VA_ARGS__ )
#	define luaL_openlib(...) get_lua_imports()->imp_luaL_openlib( __VA_ARGS__ )
#	define luaL_register(...) get_lua_imports()->imp_luaL_register( __VA_ARGS__ )
#	define luaL_getmetafield(...) get_lua_imports()->imp_luaL_getmetafield( __VA_ARGS__ )
#	define luaL_callmeta(...) get_lua_imports()->imp_luaL_callmeta( __VA_ARGS__ )
#	define luaL_typerror(...) get_lua_imports()->imp_luaL_typerror( __VA_ARGS__ )
#	define luaL_argerror(...) get_lua_imports()->imp_luaL_argerror( __VA_ARGS__ )
#	define luaL_checklstring(...) get_lua_imports()->imp_luaL_checklstring( __VA_ARGS__ )
#	define luaL_optlstring(...) get_lua_imports()->imp_luaL_optlstring( __VA_ARGS__ )
#	define luaL_checknumber(...) get_lua_imports()->imp_luaL_checknumber( __VA_ARGS__ )
#	define luaL_optnumber(...) get_lua_imports()->imp_luaL_optnumber( __VA_ARGS__ )
#	define luaL_checkinteger(...) get_lua_imports()->imp_luaL_checkinteger( __VA_ARGS__ )
#	define luaL_optinteger(...) get_lua_imports()->imp_luaL_optinteger( __VA_ARGS__ )
#	define luaL_checkstack(...) get_lua_imports()->imp_luaL_checkstack( __VA_ARGS__ )
#	define luaL_checktype(...) get_lua_imports()->imp_luaL_checktype( __VA_ARGS__ )
#	define luaL_checkany(...) get_lua_imports()->imp_luaL_checkany( __VA_ARGS__ )
#	define luaL_newmetatable(...) get_lua_imports()->imp_luaL_newmetatable( __VA_ARGS__ )
#	define luaL_checkudata(...) get_lua_imports()->imp_luaL_checkudata( __VA_ARGS__ )
#	define luaL_where(...) get_lua_imports()->imp_luaL_where( __VA_ARGS__ )
#	define luaL_error(...) get_lua_imports()->imp_luaL_error( __VA_ARGS__ )
#	define luaL_checkoption(...) get_lua_imports()->imp_luaL_checkoption( __VA_ARGS__ )
#	define luaL_ref(...) get_lua_imports()->imp_luaL_ref( __VA_ARGS__ )
#	define luaL_unref(...) get_lua_imports()->imp_luaL_unref( __VA_ARGS__ )
#	define luaL_loadfile(...) get_lua_imports()->imp_luaL_loadfile( __VA_ARGS__ )
#	define luaL_loadbuffer(...) get_lua_imports()->imp_luaL_loadbuffer( __VA_ARGS__ )
#	define luaL_loadstring(...) get_lua_imports()->imp_luaL_loadstring( __VA_ARGS__ )
#	define luaL_newstate(...) get_lua_imports()->imp_luaL_newstate( __VA_ARGS__ )
#	define luaL_gsub(...) get_lua_imports()->imp_luaL_gsub( __VA_ARGS__ )
#	define luaL_findtable(...) get_lua_imports()->imp_luaL_findtable( __VA_ARGS__ )
#	define luaL_buffinit(...) get_lua_imports()->imp_luaL_buffinit( __VA_ARGS__ )
#	define luaL_prepbuffer(...) get_lua_imports()->imp_luaL_prepbuffer( __VA_ARGS__ )
#	define luaL_addlstring(...) get_lua_imports()->imp_luaL_addlstring( __VA_ARGS__ )
#	define luaL_addstring(...) get_lua_imports()->imp_luaL_addstring( __VA_ARGS__ )
#	define luaL_addvalue(...) get_lua_imports()->imp_luaL_addvalue( __VA_ARGS__ )
#	define luaL_pushresult(...) get_lua_imports()->imp_luaL_pushresult( __VA_ARGS__ )
#	define luaopen_base(...) get_lua_imports()->imp_luaopen_base( __VA_ARGS__ )
#	define luaopen_table(...) get_lua_imports()->imp_luaopen_table( __VA_ARGS__ )
#	define luaopen_io(...) get_lua_imports()->imp_luaopen_io( __VA_ARGS__ )
#	define luaopen_os(...) get_lua_imports()->imp_luaopen_os( __VA_ARGS__ )
#	define luaopen_string(...) get_lua_imports()->imp_luaopen_string( __VA_ARGS__ )
#	define luaopen_math(...) get_lua_imports()->imp_luaopen_math( __VA_ARGS__ )
#	define luaopen_debug(...) get_lua_imports()->imp_luaopen_debug( __VA_ARGS__ )
#	define luaopen_package(...) get_lua_imports()->imp_luaopen_package( __VA_ARGS__ )
#	define luaL_openlibs(...) get_lua_imports()->imp_luaL_openlibs( __VA_ARGS__ )
//[[[end]]]
#	define LUADLL_IMPORTS_OVERRIDEN 1
#endif

