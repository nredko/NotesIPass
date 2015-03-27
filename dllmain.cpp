// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "NotesIPass.h"

HINSTANCE hDllInstance = 0;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hDllInstance = (HINSTANCE)hModule;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		ExtClear();
		break;
	}
	return TRUE;
}

