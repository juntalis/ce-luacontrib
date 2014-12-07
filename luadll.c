#define LUADLL_SRC
#include "pch.h"

#define LUA_DLL_NAME LUADLL_BASE LUA_PLATFORM_BITS LUADLL_EXT

static luadll_imports_t* lp_lua_imports = NULL;

struct import_proc { FARPROC* pproc; const char* name; };
#define LuaImport(imports,func) { ((FARPROC*)&(imports->imp_##func)), #func }
#define LuaImportsEnd()			{ NULL, NULL }

static void luadll_populate(LUADLL_HMODULE hLua, luadll_imports_t* luaImports)
{
	struct import_proc* imp, proctab[] = {
		LuaImport(luaImports, lua_newstate),
		LuaImport(luaImports, lua_close),
		LuaImport(luaImports, lua_newthread),
		LuaImport(luaImports, lua_atpanic),
		LuaImport(luaImports, lua_gettop),
		LuaImport(luaImports, lua_settop),
		LuaImport(luaImports, lua_pushvalue),
		LuaImport(luaImports, lua_remove),
		LuaImport(luaImports, lua_insert),
		LuaImport(luaImports, lua_replace),
		LuaImport(luaImports, lua_checkstack),
		LuaImport(luaImports, lua_xmove),
		LuaImport(luaImports, lua_isnumber),
		LuaImport(luaImports, lua_isstring),
		LuaImport(luaImports, lua_iscfunction),
		LuaImport(luaImports, lua_isuserdata),
		LuaImport(luaImports, lua_type),
		LuaImport(luaImports, lua_typename),
		LuaImport(luaImports, lua_equal),
		LuaImport(luaImports, lua_rawequal),
		LuaImport(luaImports, lua_lessthan),
		LuaImport(luaImports, lua_tonumber),
		LuaImport(luaImports, lua_tointeger),
		LuaImport(luaImports, lua_toboolean),
		LuaImport(luaImports, lua_tolstring),
		LuaImport(luaImports, lua_objlen),
		LuaImport(luaImports, lua_tocfunction),
		LuaImport(luaImports, lua_touserdata),
		LuaImport(luaImports, lua_tothread),
		LuaImport(luaImports, lua_topointer),
		LuaImport(luaImports, lua_pushnil),
		LuaImport(luaImports, lua_pushnumber),
		LuaImport(luaImports, lua_pushinteger),
		LuaImport(luaImports, lua_pushlstring),
		LuaImport(luaImports, lua_pushstring),
		LuaImport(luaImports, lua_pushvfstring),
		LuaImport(luaImports, lua_pushfstring),
		LuaImport(luaImports, lua_pushcclosure),
		LuaImport(luaImports, lua_pushboolean),
		LuaImport(luaImports, lua_pushlightuserdata),
		LuaImport(luaImports, lua_pushthread),
		LuaImport(luaImports, lua_gettable),
		LuaImport(luaImports, lua_getfield),
		LuaImport(luaImports, lua_rawget),
		LuaImport(luaImports, lua_rawgeti),
		LuaImport(luaImports, lua_createtable),
		LuaImport(luaImports, lua_newuserdata),
		LuaImport(luaImports, lua_getmetatable),
		LuaImport(luaImports, lua_getfenv),
		LuaImport(luaImports, lua_settable),
		LuaImport(luaImports, lua_setfield),
		LuaImport(luaImports, lua_rawset),
		LuaImport(luaImports, lua_rawseti),
		LuaImport(luaImports, lua_setmetatable),
		LuaImport(luaImports, lua_setfenv),
		LuaImport(luaImports, lua_call),
		LuaImport(luaImports, lua_pcall),
		LuaImport(luaImports, lua_cpcall),
		LuaImport(luaImports, lua_load),
		LuaImport(luaImports, lua_dump),
		LuaImport(luaImports, lua_yield),
		LuaImport(luaImports, lua_resume),
		LuaImport(luaImports, lua_status),
		LuaImport(luaImports, lua_gc),
		LuaImport(luaImports, lua_error),
		LuaImport(luaImports, lua_next),
		LuaImport(luaImports, lua_concat),
		LuaImport(luaImports, lua_getallocf),
		LuaImport(luaImports, lua_setallocf),
		LuaImport(luaImports, lua_setlevel),
		LuaImport(luaImports, lua_getstack),
		LuaImport(luaImports, lua_getinfo),
		LuaImport(luaImports, lua_getlocal),
		LuaImport(luaImports, lua_setlocal),
		LuaImport(luaImports, lua_getupvalue),
		LuaImport(luaImports, lua_setupvalue),
		LuaImport(luaImports, lua_sethook),
		LuaImport(luaImports, lua_gethook),
		LuaImport(luaImports, lua_gethookmask),
		LuaImport(luaImports, lua_gethookcount),
		LuaImport(luaImports, luaL_openlib),
		LuaImport(luaImports, luaL_register),
		LuaImport(luaImports, luaL_getmetafield),
		LuaImport(luaImports, luaL_callmeta),
		LuaImport(luaImports, luaL_typerror),
		LuaImport(luaImports, luaL_argerror),
		LuaImport(luaImports, luaL_checklstring),
		LuaImport(luaImports, luaL_optlstring),
		LuaImport(luaImports, luaL_checknumber),
		LuaImport(luaImports, luaL_optnumber),
		LuaImport(luaImports, luaL_checkinteger),
		LuaImport(luaImports, luaL_optinteger),
		LuaImport(luaImports, luaL_checkstack),
		LuaImport(luaImports, luaL_checktype),
		LuaImport(luaImports, luaL_checkany),
		LuaImport(luaImports, luaL_newmetatable),
		LuaImport(luaImports, luaL_checkudata),
		LuaImport(luaImports, luaL_where),
		LuaImport(luaImports, luaL_error),
		LuaImport(luaImports, luaL_checkoption),
		LuaImport(luaImports, luaL_ref),
		LuaImport(luaImports, luaL_unref),
		LuaImport(luaImports, luaL_loadfile),
		LuaImport(luaImports, luaL_loadbuffer),
		LuaImport(luaImports, luaL_loadstring),
		LuaImport(luaImports, luaL_newstate),
		LuaImport(luaImports, luaL_gsub),
		LuaImport(luaImports, luaL_findtable),
		LuaImport(luaImports, luaL_buffinit),
		LuaImport(luaImports, luaL_prepbuffer),
		LuaImport(luaImports, luaL_addlstring),
		LuaImport(luaImports, luaL_addstring),
		LuaImport(luaImports, luaL_addvalue),
		LuaImport(luaImports, luaL_pushresult),
		LuaImport(luaImports, luaopen_base),
		LuaImport(luaImports, luaopen_table),
		LuaImport(luaImports, luaopen_io),
		LuaImport(luaImports, luaopen_os),
		LuaImport(luaImports, luaopen_string),
		LuaImport(luaImports, luaopen_math),
		LuaImport(luaImports, luaopen_debug),
		LuaImport(luaImports, luaopen_package),
		LuaImport(luaImports, luaL_openlibs),
		LuaImportsEnd()
	};
	imp = &(proctab[0]);
	while(imp->pproc) {
		if(!(*(imp->pproc) = LUADLL_GETPROC(hLua, imp->name))) {
			fprintf(stderr, "WARNING: Failed to GetProcAddress(\"%s\")\n", imp->name);
		}
		imp++;
	}
}

int luadll_init(const char* sDllPath)
{	
	LUADLL_HMODULE hLua;
	if(!(hLua = LUADLL_GETLIB(sDllPath))) 
		return 0;
	
	if(!( lp_lua_imports = (luadll_imports_t*)HeapAlloc(GetProcessHeap(), 0, sizeof(luadll_imports_t)) ))
		return 0;
	
	luadll_populate((lp_lua_imports->_hModLua = hLua), lp_lua_imports);
	return TRUE;
}

void luadll_destroy(void)
{
	if(!lp_lua_imports) return;
	if(lp_lua_imports->_hModLua) LUADLL_FREELIB(lp_lua_imports->_hModLua);
	HeapFree(GetProcessHeap(), 0, lp_lua_imports);
	lp_lua_imports = NULL;
}

luadll_imports_t* get_lua_imports_postinit(void) { return lp_lua_imports; }

luadll_imports_t* get_lua_imports_preinit(void) {
	if(!lp_lua_imports) { luadll_init(LUA_DLL_NAME); }
	// lp_get_lua_imports_func = get_lua_imports_postinit;
	return lp_lua_imports;
}

luadll_imports_t* (LUADLL_CDECL *lp_get_lua_imports_func)(void) = get_lua_imports_preinit;

LUADLL_HMODULE luadll_get_hmodule(void)  {
	return lp_lua_imports != NULL ? lp_lua_imports->_hModLua : (*lp_get_lua_imports_func)()->_hModLua;
}
