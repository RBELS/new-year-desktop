#pragma once
#include <Windows.h>

#ifdef REPLACELIBRARY_EXPORTS
#define REPLACELIBRARY_API __declspec(dllexport)
#else
#define REPLACELIBRARY_API __declspec(dllimport)
#endif

extern "C" REPLACELIBRARY_API LRESULT CALLBACK __stdcall HookProc(int nCode, WPARAM wParam, LPARAM lParam);
extern "C" REPLACELIBRARY_API void __stdcall InstallHook();
extern "C" REPLACELIBRARY_API void __stdcall UninstallHook();
