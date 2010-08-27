// mloader.cpp : main source file for mloader.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include <string>

#include "resource.h"

CAppModule _Module;

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#define MAX_URI MAX_PATH*4
#define HTTP_BUFFER_LEN 512

#define IsFileExist(lpszFileName) (GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)

bool GetRealURL(std::string url, int maxsize, std::string &outstr, int timeout = 10)
{
	char buffer[HTTP_BUFFER_LEN];//下载文件的缓冲区
	DWORD bytes_read = 1;//下载的字节数
	bool getre = false;
	outstr = "";

	if(url.length() < 6)
		return false;

	//打开一个internet连接
	HINTERNET internet=InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if(!internet)
		return false;

	DWORD dtimeout = timeout * 300;
	InternetSetOption(internet, INTERNET_OPTION_CONNECT_TIMEOUT, &dtimeout, sizeof(DWORD));
	InternetSetOption(internet, INTERNET_OPTION_SEND_TIMEOUT, &dtimeout, sizeof(DWORD));
	InternetSetOption(internet, INTERNET_OPTION_RECEIVE_TIMEOUT, &dtimeout, sizeof(DWORD));
	dtimeout = timeout/10;
	InternetSetOption(internet, INTERNET_OPTION_CONNECT_RETRIES, &dtimeout, sizeof(DWORD));

	//打开一个http url地址
	HINTERNET file_handle=InternetOpenUrl(internet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);

	if(file_handle) {
		//从url地址中读取文件内容到缓冲区buffer
		BOOL b = 0;
		int readbyte = 0;
		while(bytes_read > 0) {
			b = InternetReadFile(file_handle, buffer, 512 , &bytes_read);
			if(!b)
				break;
			readbyte += bytes_read;
			buffer[bytes_read] = 0;
			outstr += buffer;
		}
		getre = true;
	}
	//关闭连接
	InternetCloseHandle(internet);

	return getre;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = 0;//Run(lpstrCmdLine, nCmdShow);

	CString ProgramFile;
	CString lpszArgument(lpstrCmdLine);

	TCHAR szFilePath[MAX_URI], szIniPath[MAX_URI];
	GetModuleFileName(NULL, szFilePath, MAX_URI);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;

	_tcsncpy_s(szIniPath, MAX_URI, szFilePath, MAX_URI);
	_tcsncat_s(szIniPath, MAX_URI, _T("mloader.ini"), MAX_URI);

	TCHAR *mainpath = _tcsstr(szFilePath, _T("\\codecs\\Real"));
	if(mainpath) mainpath[1] = 0;
	_tcsncat_s(szFilePath, MAX_URI, _T("mplayer.exe"), MAX_URI);

	ProgramFile = szFilePath;

	if(IsFileExist(szIniPath)) {
		TCHAR temp[MAX_URI];
		GetPrivateProfileString(_T("Command"), _T("Program_Path"), _T(""), temp, MAX_URI, szIniPath);
		if(_tcslen(temp) > 3) ProgramFile = temp;
	}

	int index = lpszArgument.Find(_T("/command:Screensize(full)"));
	if(index >= 0)
		lpszArgument.Delete(index, _tcslen(_T("/command:Screensize(full)")));

	index = lpszArgument.Find(_T("\""));
	if(index >= 0) {
		int len = lpszArgument.Find(_T("\""), index+1);
		if(len > index) {
			CString url = lpszArgument.Mid(index+1, len-index-1);
			if(url.Find(_T("http://")) >= 0) {
				std::string surl;
				if(GetRealURL(url.GetBuffer(), 4096, surl))
					url = surl.c_str();
				if(url.Find(_T("rtsp://")) >= 0)
					lpszArgument = _T("\"") + url + _T("\"");
			}
		}
	}

	ShellExecute(0, _T("open"), ProgramFile,  lpszArgument, NULL, SW_SHOW);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
