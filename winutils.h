/**
 * @file winutils.h
 * 
 * TODO: Description
 */
#ifndef _WINUTILS_H_
#define _WINUTILS_H_
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

HMODULE GetModuleNoRef(LPCSTR sModule);
BOOLEAN ExtractResourceToMemory(LPVOID* buffer, LPCSTR lpName, LPCSTR lpType);

#ifdef __cplusplus
}
#endif

#endif /* _WINUTILS_H_ */
