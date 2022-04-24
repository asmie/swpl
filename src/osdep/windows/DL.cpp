/**
 *  @file   DL.cpp
 *  @brief  Dynamic loader Windows implementation.
 *
 *  @author Piotr "asmie" Olszewski
 *
 *  @date   2021.10.06
 */

#include "osdep/DL.hpp"

#include <windows.h>

void* DL::loadLibrary(const char *name, DL_FLAGS flags)
{
	void *instance = nullptr;

	instance = LoadLibraryEx(name, NULL, NULL);

	return (instance);
}

void* DL::getFuncAddr(void* libraryHandle, const char* functionName)
{
	void* addr = nullptr;

	addr = GetProcAddress(libraryHandle, functionName);

	return (addr);
}
