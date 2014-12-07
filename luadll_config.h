/**
 * @file luadll_config.h
 * @brief Configuration for luadll framework.
 */

#include "winutils.h"

#define LUADLL_HMODULE HMODULE
#define LUADLL_GETLIB(MODNAME) GetModuleNoRef(MODNAME)
// No need, since we're not incrementing the ref count on the dll.
#define LUADLL_FREELIB(X) 
#define LUADLL_NOLINK 1
