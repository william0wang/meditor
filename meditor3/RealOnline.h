#pragma once


// CRealOnline ¶Ô»°¿ò

class CRealOnline
{
private:
	CString str_title;
	CString str_ok;
	CString str_fail;
	CString str_uok;
	CString str_ufail;
	CString str_again;
	CString str_on;

public:
	CRealOnline(CString LangDll);

	HANDLE CheckRealThread;
	CString m_cmdline;
	CString m_dir;
	BOOL m_reg_ok;
	BOOL CheckRealOnline();
	BOOL RegRealOnline();
	BOOL DRegRealOnline();

	BOOL InstallReal();
};
