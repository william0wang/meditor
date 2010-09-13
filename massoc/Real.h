#pragma once


// CRealDlg ¶Ô»°¿ò

class CRealDlg
{
public:
	CRealDlg();

	HANDLE CheckRealThread;
	CString m_cmdline;
	CString m_dir;
	BOOL m_reg_ok;
	BOOL CheckRealOnline();
	BOOL RegRealOnline();
	BOOL DRegRealOnline();

	BOOL InstallReal();
};
