ce-luacontrib
=============
Since I'll probably never bother to package this properly, I'm dumping this here. I wrote this quite a while back, so I don't really recall why I saw the need to package the lua extension as a resource and load it into memory. I think I recall there being some issue with Cheat Engine encountering memory access errors after it unloaded the plugin.

Regardless, everything outside the vendor folder is released under the WTFPL 2.0 license. If you can manage to salvage anything out of it, do whatever you want with it. As for the rest:

* <a href="https://github.com/fancycode/MemoryModule">MemoryModule</a> - Copyright (c) 2004-2013 by Joachim Bauch - Released under the Mozilla Public License Version 2.0
* <a href="https://github.com/jmckaskill/luaffi">luaffi</a> - Copyright (c) 2011 James R. McKaskill. MIT same as Lua 5.1. See full license text in <a href="https://github.com/juntalis/ce-luacontrib/blob/master/vendor/ffi/ffi.h">ffi.h</a>.

**Note** - While the ffi interface being used in this extension is derived from luajit's ffi interface, the behavior of some of the API deviates a tad from the original. The most notable difference is that you must explicitly import functions from their DLLs. (Unlike luajit, where you as long as the DLL is loaded, you can call imported functions using `ffi.C`)

Some example code:

```lua
	local ffi = require('ffi')

	-- Load all of our DLLs
	local ntdll = ffi.load('ntdll.dll')
	local kernel32 = ffi.load('kernel32.dll')
	local user32 = ffi.load('user32.dll')
	local msvcrt = ffi.load('msvcrt.dll')

	--[[--
	  CRT Declarations (msvcrt.dll)
	--]]--
	ffi.cdef[[
	unsigned int wcstombs(char* s, const wchar_t* w, unsigned int c);
	unsigned int mbstowcs(wchar_t* w, const char* s, unsigned int c);
	unsigned int wcslen(const wchar_t* s);
	unsigned int strlen(const char* s);
	char *strcpy(char* d, const char* s);
	]]

	--[[--
	  Lua wrapper functions for the above declarations.
	--]]--

	function wcslen(ws)
		return tonumber(
			msvcrt.wcslen(
				ffi.cast('const wchar_t*', ws)
			)
		)
	end

	function wstr2str(ws, length)
		local szbuf = length or wcslen(ws)
		local buf = ffi.new('char[?]', szbuf+1)
		if msvcrt.wcstombs(buf, ffi.cast('const wchar_t*', ws), szbuf) == -1 then
			return nil
		end
		return ffi.string(buf, szbuf)
	end

	function str2wstr(s)
		local szbuf = s:len()
		local buf = ffi.new('wchar_t[?]', szbuf+1)
		if msvcrt.mbstowcs(buf, s, szbuf) == -1 then
			return nil
		end
		return buf
	end

	function str2cstr(s)
		local szbuf = s:len()
		local buf = ffi.new('char[?]', szbuf+1)
		msvcrt.strcpy(buf, s)
		return buf
	end

	--[[--
	  Native Declarations (ntdll.dll)
	--]]--
	ffi.cdef[[
	typedef union _LARGE_INTEGER {
		struct
		{
			unsigned long	LowPart;
			long			HighPart;
		} s;
		struct
		{
			unsigned long	LowPart;
			long			HighPart;
		} u;
		long long	QuadPart;
	} LARGE_INTEGER;

	typedef struct _UNICODE_STRING {
		unsigned short	Length;
		unsigned short	MaximumLength;
		wchar_t			***Buffer;
	} UNICODE_STRING;

	typedef struct _SYSTEM_PROCESS_INFORMATION_DETAILD {
		unsigned long	NextEntryOffset;
		unsigned long	NumberOfThreads;
		LARGE_INTEGER	SpareLi1;
		LARGE_INTEGER	SpareLi2;
		LARGE_INTEGER	SpareLi3;
		LARGE_INTEGER	CreateTime;
		LARGE_INTEGER	UserTime;
		LARGE_INTEGER	KernelTime;
		UNICODE_STRING	ImageName;
		long			BasePriority;
		void			*UniqueProcessId;
		unsigned long	InheritedFromUniqueProcessId;
		unsigned long	HandleCount;
		unsigned char	Reserved4[4];
		void			*Reserved5[11];
		unsigned long	PeakPagefileUsage;
		unsigned long	PrivatePageCount;
		LARGE_INTEGER	Reserved6[6];
	} SYSTEM_PROCESS_INFORMATION_DETAILD, *PSYSTEM_PROCESS_INFORMATION_DETAILD;

	long NtQuerySystemInformation(unsigned long, void*, unsigned long, unsigned long*);
	]]

	-- Constants
	local SystemProcessInformation = 5

	--[[--
	  Given a process's PID, return the filename of the executable.
	--]]--
	function pid2name(pid)
		local status, result, psinfo, retlength =
			  0, nil,
			  ffi.cast('PSYSTEM_PROCESS_INFORMATION_DETAILD', ffi.new('unsigned char[?]', 102400)),
			  ffi.new('unsigned long[1]')
		
		status = tonumber(ntdll.NtQuerySystemInformation(
			SystemProcessInformation,
			ffi.cast('void*', psinfo),
			102400,
			result
		))
		
		if status ~= 0 then return result end
		while (status == 0) and (tonumber(psinfo.NextEntryOffset) > 0) do
			if pid == tonumber(psinfo.UniqueProcessId) then
				result = cutil.wstr2str(psinfo.ImageName.Buffer, psinfo.ImageName.Length - 4)
				status = 1
			else
				psinfo = ffi.cast(
					'PSYSTEM_PROCESS_INFORMATION_DETAILD',
					tonumber(ffi.cast('void*', psinfo)) + psinfo.NextEntryOffset
				)
			end
		end
		
		return result
	end

	--[[--
	  Win32 API Declarations (user32.dll, kernel32.dll)
	--]]--
	ffi.cdef[[
	typedef void* HWND, *LPARAM;
	typedef signed long BOOL;
	typedef unsigned long DWORD;
	typedef DWORD* LPDWORD;
	typedef BOOL(__stdcall* WNDENUMPROC)(HWND hWnd, LPARAM lParam);

	typedef struct _ENUMPARAM {
		HWND hWnd;
		DWORD PID;
	} ENUMPARAM, *PENUMPARAM;

	HWND SetFocus(HWND hWnd);
	DWORD GetCurrentThreadId(void);
	HWND GetForegroundWindow(void);
	BOOL BringWindowToTop(HWND hWnd);
	BOOL SetForegroundWindow(HWND hWnd);
	BOOL EnumWindows(WNDENUMPROC lpEnumFunc, LPARAM lParam);
	DWORD GetWindowThreadProcessId(HWND hWnd, LPDWORD lpdwProcessId);
	BOOL AttachThreadInput(DWORD idAttach, DWORD idAttachTo, BOOL fAttach);
	]]

	-- Constants & Explicit Imports
	local TRUE,FALSE = 1,0
	local GetCurrentThreadId = kernel32.GetCurrentThreadId
	local SetFocus = user32.SetFocus
	local GetForegroundWindow = user32.GetForegroundWindow
	local BringWindowToTop = user32.BringWindowToTop
	local SetForegroundWindow = user32.SetForegroundWindow
	local EnumWindows = user32.EnumWindows
	local GetWindowThreadProcessId = user32.GetWindowThreadProcessId
	local AttachThreadInput = user32.AttachThreadInput

	local enumWndsProc = ffi.cast('WNDENUMPROC', function(hWnd, lParam)
		local lpdwPID = ffi.new('DWORD[1]')
		local ep = ffi.cast('PENUMPARAM', lParam)
		GetWindowThreadProcessId(hWnd, lpdwPID)
		if ep[0].PID == lpdwPID[0] then
			ep[0].hWnd = hWnd
			return FALSE
		end
		return TRUE
	end)

	-- Obviously this function suffers a few serious
	-- flaws. It only returns the first window found
	-- for a particular PID, so if your target has more
	-- than one window open, there's no guarantee that
	-- you're finding the window you're looking for.
	pid2hwnd = function(pid)
		pid = pid or getOpenedProcessID()
		local ep = ffi.new('ENUMPARAM[1]')
		ep[0].hWnd = nil
		ep[0].PID  = pid
		EnumWindows(enumWndsProc, ffi.cast('LPARAM', ep))
		return ep[0].hWnd
	end

	setForegroundWindow = function(hWnd)
		local dwFgPID = ffi.new('DWORD[1]')
		local hWndFg = GetForegroundWindow()
		local dwFgTID = GetWindowThreadProcessId(hWndFg, dwFgPID)
		local dwMyTID = GetCurrentThreadId()
		AttachThreadInput(dwMyTID, dwFgTID, TRUE)
		SetForegroundWindow(hWnd)
		BringWindowToTop(hWnd)
		SetFocus(hWnd)
		AttachThreadInput(dwMyTID, dwFgTID, False)
	end
	```
