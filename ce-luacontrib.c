#include "pch.h"
#include "winutils.h"
#include "cepluginsdk.h"
#include "MemoryModule.h"

typedef lua_State*(WINAPI* GetLuaStateFunc)(void);

/** Our plugin struct */
struct CE_LuaContrib_Plugin
{
	int ID;
	PExportedFunctions LPCE;
	lua_State* L;
} Plugin = { 0, NULL, NULL };

#define cexp() (*Plugin.LPCE)

static HMEMORYMODULE hFFI = NULL;

void xmsg(char* message)
{
	cexp().ShowMessage(message);
}

void _xfree_(void* ptr)
{
	if(ptr) free(ptr);
}

void* xalloc(size_t szBuf)
{
	void* p = malloc(szBuf);
	if(!p) {
		xmsg("Malloc failed!");
		return NULL;
	}
	memset(p, 0, szBuf);
	return p;
}

static inline HMEMORYMODULE UnpackLuaFFI(void)
{
#	ifdef BUILD_ARCH_X64
#		define LUA_FFILIBID LUACONTRIB_FFI64
#	else
#		define LUA_FFILIBID LUACONTRIB_FFI32
#	endif
	HMEMORYMODULE hMod = NULL;
	LPVOID lpModule = NULL;
	if(!ExtractResourceToMemory(&lpModule, (const char*)LUA_FFILIBID, "LUA_FFILIB")) return FALSE;
	hMod = MemoryLoadLibrary((const void*)lpModule);
	xfree(lpModule);
	return hMod;
}

HANDLE CE_GetProcessHandle(void)
{
	return *(cexp().OpenedProcessHandle);
}

DWORD CE_GetPID(void)
{
	return *(cexp().OpenedProcessID);
}

lua_State* CE_GetLuaState(void)
{
	return ((GetLuaStateFunc)(cexp().GetLuaState))();
}

BOOL WINAPI CEPlugin_GetVersion(PPluginVersion pv, int szVers)
{
	pv->version = CESDK_VERSION;
	pv->pluginname = "PPSSPP Lua Contrib Plugin";
	return TRUE;
}

BOOL WINAPI CEPlugin_InitializePlugin(PExportedFunctions pef, int id)
{
	Plugin.ID = id;
	Plugin.LPCE = pef;
	Plugin.L = CE_GetLuaState();
	if(!hFFI) {
		int (__cdecl* luaopen_ffi)(lua_State*);
		hFFI = UnpackLuaFFI();
		*((FARPROC*)(&luaopen_ffi)) = MemoryGetProcAddress(hFFI, "luaopen_ffi");
		luaopen_ffi(Plugin.L);
	}
	return TRUE;
}

BOOL WINAPI CEPlugin_DisablePlugin(void)
{
	//clean up memory you might have allocated
	luadll_destroy();
	Plugin.L = NULL;
	Plugin.LPCE = NULL;
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) {
		case DLL_PROCESS_DETACH:
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		default: break;
	}
	return TRUE;
}
