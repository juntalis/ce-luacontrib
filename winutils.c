#include "pch.h"

/** Used to pass/return data to/from EnumWindows function. */
typedef struct _ENUMPARAM {
	HWND hWnd;
	DWORD PID;
} ENUMPARAM, *PENUMPARAM;

HMODULE GetModuleNoRef(LPCSTR sModule)
{
	HMODULE hModule = NULL;
	GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, sModule, &hModule);
	return hModule;
}

static inline HMODULE GetCurrentModule(void)
{
	HMODULE hModule = NULL;
	GetModuleHandleExW(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCWSTR)(&GetCurrentModule),
		&hModule
	);
	return hModule;
}

BOOLEAN ExtractResourceToMemory(LPVOID* buffer, LPCSTR lpName, LPCSTR lpType)
{
	INT i = 0;
	SIZE_T szResource = 0;
	HMODULE hModule;
	HRSRC hClientRsrc;
	HGLOBAL hClient;
	LPBYTE lpResourceBytes;

	hModule = GetCurrentModule();
	if(! (hClientRsrc = FindResource(hModule, MAKEINTRESOURCE(lpName), lpType)) ) {
		xmsg("FindResource failed!");
		return FALSE;
	}

	if(!(hClient = LoadResource(hModule, hClientRsrc))) {
		xmsg("LoadResource failed!");
		return FALSE;
	}

	szResource = (size_t)SizeofResource(hModule, hClientRsrc);
	if(!(lpResourceBytes = (u8*)LockResource(hClient))) {
		xmsg("LockResource failed!");
		return FALSE;
	}
	
	if(!(*buffer = xalloc(szResource)))
		return FALSE;
	
	memcpy(*buffer, (const void*)lpResourceBytes, szResource);
	return TRUE;
}

void ForceSetForegroundWindow(HWND hwnd)
{
	DWORD dwFGThreadId, dwFGProcessId, dwThisThreadId;
	HWND hwndForeground = GetForegroundWindow();
	dwFGThreadId = GetWindowThreadProcessId(hwndForeground, &dwFGProcessId);
	dwThisThreadId = GetCurrentThreadId();
	AttachThreadInput(dwThisThreadId, dwFGThreadId,TRUE);
	SetForegroundWindow(hwnd);
	BringWindowToTop(hwnd);
	SetFocus(hwnd);
	AttachThreadInput(dwThisThreadId, dwFGThreadId,FALSE);
}

/***********************************
* Enum windows callback function. *
***********************************/
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM ep)
{
	DWORD dwPID; // Does PID match ?
	GetWindowThreadProcessId(hWnd, &dwPID);
	if (((PENUMPARAM)ep)->PID == dwPID) {
		((PENUMPARAM)ep)->hWnd = hWnd;
		return FALSE;
	}

	return TRUE;
}

HWND GetWinHandle(DWORD pid)
{
	ENUMPARAM ep;
	ep.hWnd = NULL;
	ep.PID = pid;
	EnumWindows(EnumWindowsProc, (LPARAM)&ep);
	return ep.hWnd;
}
