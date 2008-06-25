// mloader.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "mloader.h"
#define MAX_URI 2048

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES -1
#endif

#define IsFileExist(lpszFileName) (::GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)


size_t UnicodeToLocal(TCHAR *instr,char **outstr)
{
	size_t inlen = _tcsclen(instr);
#ifdef UNICODE
	UINT nCodePage = CP_ACP;
	int   nMultiByteLength   =   WideCharToMultiByte(nCodePage, WC_COMPOSITECHECK|WC_DEFAULTCHAR
		, instr, inlen,   0,   0,   0,   0);
	*outstr = (char *)malloc(nMultiByteLength + 2);
	memset(*outstr,0,nMultiByteLength + 2);
	WideCharToMultiByte(nCodePage, WC_COMPOSITECHECK|WC_DEFAULTCHAR
		, instr, inlen ,*outstr,   nMultiByteLength ,   0,   0);
#else
	*outstr = (char *)malloc(inlen + 1);
	memset(*outstr,0,inlen + 1);
	for(int i = 0; i < inlen; i++)
	{
		(*outstr)[i] = instr[i];
	}
#endif
	size_t outlen = strlen(*outstr);
	return outlen;
}


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int cmdlen =  _tcslen(lpCmdLine) + 1;

	TCHAR szFilePath[MAX_URI], szIniPath[MAX_URI];
	GetModuleFileName(NULL, szFilePath, MAX_URI);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;

	_tcscpy_s(szIniPath, MAX_URI , szFilePath);
	_tcscat_s(szIniPath, MAX_URI , _T("mloader.ini"));

	TCHAR * mainpath = _tcsstr(szFilePath,_T("\\codecs\\Real"));
	if(mainpath)
		mainpath[1] = 0;
	_tcscat_s(szFilePath, MAX_URI , _T("mplayer.exe"));

	TCHAR *load_command = (TCHAR *)malloc(sizeof(TCHAR) * (MAX_URI + cmdlen));
	load_command[0] = 0;
	if(IsFileExist(szIniPath))
		GetPrivateProfileString(_T("Command"), _T("Program_Path"), _T(""), load_command, MAX_URI, szIniPath);
	if(_tcslen(load_command) <= 2)
		_tcscpy_s(load_command, MAX_URI , szFilePath);

	_tcscat_s(load_command, MAX_URI + cmdlen, _T(" "));

	mainpath = _tcsstr(lpCmdLine,_T("/command:Screensize(full)"));
	if(mainpath)
	{
		int len = _tcslen(_T("/command:Screensize(full)"));
		lpCmdLine += len;
	}

	_tcscat_s(load_command, MAX_URI + cmdlen, lpCmdLine);

	char *runcmd;
	UnicodeToLocal(load_command, &runcmd);
	WinExec(runcmd, SW_SHOW);

	return TRUE;
}
