// NotesIPass.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <global.h>
#include <extmgr.h>
#include <bsafeerr.h>
#include <nsferr.h>
#include <osenv.h>
#include "KeePassHttpClient\src\KeePassHttpClient.h"
#include "NotesIPass.h"

#include <neon/ne_socket.h>
#include <neon/ne_session.h>
#include <neon/ne_request.h>
#include <neon/ne_utils.h>
#include <neon/ne_uri.h>

#ifdef _DEBUG
#define LOG(s) OutputDebugString((s).c_str());OutputDebugString(_T("\n"))
#else
#define LOG(s)
#endif

HEMREGISTRATION hHandler = 0;
bool passwordRequested = false;
tstring uuid = "";

DLL_EXPORT STATUS LNPUBLIC MainEntryPoint() {
	STATUS  status;
	char    msgBuf[256];

	status = EMRegister(EM_GETPASSWORD, EM_REG_BEFORE, ExtHandler, 0, &hHandler);
	if (NOERROR != status) {
		wsprintf(msgBuf, "Could not register extension handler - status: 0x%lX",
			status);
		MessageBox(NULL, msgBuf, "NotesIPass", MB_ICONERROR | MB_OK);
	}
	return (status);
}

DLL_EXPORT STATUS LNPUBLIC ExtClear() {
	STATUS status;
	if (0 != hHandler){
		status = EMDeregister(hHandler);
		hHandler = 0;
	}
	else
		status = NOERROR;
	return (status);
}

STATUS LNCALLBACK ExtHandler(EMRECORD* pRecord) {
	VARARG_PTR            pArgs;

	DWORD                 MaxPwdLen;
	DWORD far *           retLength;
	char far *            retPassword;
	char far *            FileName;
	char far *            OwnerName;
	char buff[512];

	if (EM_GETPASSWORD != pRecord->EId)
		return (ERR_EM_CONTINUE);

	/* check error status */
	if (pRecord->Status != NOERROR)
		return (ERR_EM_CONTINUE);

	/* Fetch the arguments */
	pArgs = pRecord->Ap;
	MaxPwdLen = VARARG_GET(pArgs, DWORD);
	retLength = VARARG_GET(pArgs, DWORD *);
	retPassword = VARARG_GET(pArgs, char *);
	FileName = VARARG_GET(pArgs, char *);
	OwnerName = VARARG_GET(pArgs, char *);

	/* Use the current password */
	if ((NULL != retLength)
		&& (NULL != retPassword))
	{
		KeePassHttpClient* kee;
		try{
			if (OSGetEnvironmentString("KeePassSettings", buff, 512))
				kee = new KeePassHttpClient(buff);
			else{
				if (OSGetEnvironmentString("KeePassPort", buff, 512))
					kee = new KeePassHttpClient(buff, "", "");
				else
					kee = new KeePassHttpClient("19455", "", "");
			}
			OSSetEnvironmentVariable("KeePassSettings", kee->Settings().c_str());
			tstring url = tstring("notes://") + OwnerName;
			Json::Value logins = kee->GetLogins(url, FileName);
			if (logins.size() > 0)
				uuid = logins[0]["Uuid"].asString();
			if (passwordRequested || logins.size() == 0){
				EXT_DLG_DATA data;
				data.FileName = FileName;
				data.MaxPwdLen = MaxPwdLen;
				data.OwnerName = OwnerName;
				data.retLength = retLength;
				data.retPassword = retPassword;
				if (IDOK != PasswordDialog(&data)){
					if (kee != NULL) delete kee;
					return (ERR_EM_CONTINUE);
				}
				kee->SetLogin(url, FileName, OwnerName, retPassword, uuid);
			}
			else {
				tstring tmp = logins[0]["Password"].asCString();
				strcpy_s(retPassword, MaxPwdLen, logins[0]["Password"].asCString());
			}
			if (kee != NULL) delete kee;
			*retLength = strlen(retPassword);
			passwordRequested = true;
			return (ERR_BSAFE_EXTERNAL_PASSWORD);
		}
		catch (std::exception e)
		{
			MessageBox(NULL, e.what(), "Error in NotesIPass", MB_OK|MB_ICONERROR);
		}
		if (kee != NULL) delete kee;
	}
	return ERR_EM_CONTINUE;
}
