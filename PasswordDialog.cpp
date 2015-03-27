#include "stdafx.h"
#include "resource.h"
#include "NotesIPass.h"

extern HINSTANCE hDllInstance;

/* Windows dialog proc to obtain password */
BOOL CALLBACK PasswordDlgProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	WORD wNotify;
	WORD wID;
	HWND hControl;
	UINT ret;
	static EXT_DLG_DATA* data;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		data = (EXT_DLG_DATA*)lParam;
		SetDlgItemText(hWndDlg, IDC_USERNAME, data->OwnerName);
		SetDlgItemText(hWndDlg, IDC_FILEPATH, data->FileName);
		SendDlgItemMessage(hWndDlg, IDC_PASSWORD, WM_SETFOCUS, 0, 0);
		return TRUE;
	case WM_COMMAND:
		wNotify = HIWORD(wParam);
		wID = LOWORD(wParam);
		hControl = (HWND)lParam;

		switch (wID)
		{
		case IDOK:
			ret = GetDlgItemText(hWndDlg, IDC_PASSWORD, data->retPassword, data->MaxPwdLen);
			if (0 != ret){
				EndDialog(hWndDlg, IDOK);
			}
			else {
				*(data->retPassword) = '\0';
				EndDialog(hWndDlg, IDABORT);
			}
			return (TRUE);
		case IDCANCEL:
			*(data->retPassword) = '\0';
			EndDialog(hWndDlg, IDCANCEL);
			return (TRUE);
		}
		return (FALSE);
	}
	return (FALSE);
}

UINT PasswordDialog(EXT_DLG_DATA* data) {
	FARPROC lpfnPasswordDlgProc;

	lpfnPasswordDlgProc = MakeProcInstance(
		(FARPROC)PasswordDlgProc,
		hDllInstance);

	UINT result = DialogBoxParam(hDllInstance, MAKEINTRESOURCE(IDD_FORMVIEW), HWND_DESKTOP, (DLGPROC)lpfnPasswordDlgProc, (LPARAM)(data));
	return result;
}
