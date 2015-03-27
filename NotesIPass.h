#pragma once

#if defined(NT)
#define DLL_EXPORT __declspec (dllexport)
#else
#define DLL_EXPORT
#endif

typedef std::basic_string<TCHAR> tstring;

typedef struct {
	DWORD           MaxPwdLen;
	char far *      FileName;
	char far *      OwnerName;
	DWORD far *     retLength;
	char far *      retPassword;
} EXT_DLG_DATA;

DLL_EXPORT STATUS LNPUBLIC MainEntryPoint(void);
DLL_EXPORT STATUS LNPUBLIC ExtClear(void);
STATUS LNCALLBACK ExtHandler(EMRECORD far *pRecord);
UINT PasswordDialog(EXT_DLG_DATA* data);