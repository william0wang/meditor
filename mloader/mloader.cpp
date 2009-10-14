// mloader.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "mloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <afxinet.h>
#pragma comment(lib, "wininet.lib")

#define MAX_URI MAX_PATH*4
#define HTTP_BUFFER_LEN 512

#define IsFileExist(lpszFileName) (GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)

// CmloaderApp

BEGIN_MESSAGE_MAP(CmloaderApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CmloaderApp 构造

CmloaderApp::CmloaderApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CmloaderApp 对象

CmloaderApp theApp;

// CmloaderApp 初始化



CString GetRealURL(CString url, int maxsize, int timeout = 10)
{
	TCHAR error[512];
	char buffer[512];
	int readbyte = 0, bytes_read;
	CHttpFile* pFile = NULL;
	CInternetSession session;
	CString RealUrl = _T("");

	try {
		session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, timeout * 300);
		session.SetOption(INTERNET_OPTION_SEND_TIMEOUT, timeout * 300);
		session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, timeout * 1000);
		session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, timeout/10);

		pFile = (CHttpFile *)session.OpenURL(url);

		if(pFile) {
			while(readbyte < maxsize) {
				bytes_read = pFile->Read(buffer,512);
				if(bytes_read <= 0)
					break;
				buffer[bytes_read] = 0;
				readbyte += bytes_read;
				RealUrl += buffer;
			}

			pFile->Close();
			delete pFile;
		}

		session.Close();

		return RealUrl;
	} catch (CInternetException* pEx) {

		if(pFile != NULL)
			pFile->Close();
		delete pFile;

		pEx->GetErrorMessage(error,512);
		pEx->Delete();

		return RealUrl;
	}
}


//CString GetRealURL(CString url , DWORD maxsize)
//{
//	char buffer[HTTP_BUFFER_LEN];
//	DWORD bytes_read;
//	DWORD readbyte = 0;
//	CString RealUrl = _T("");
//
//	HINTERNET internet = InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
//	if(internet) {
//
//		DWORD timeout = 5000;
//		InternetSetOption(internet, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(DWORD));
//
//		HINTERNET file_handle = InternetOpenUrl(internet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
//
//		if(file_handle) {
//			while(readbyte < maxsize) {
//				if(!InternetReadFile(file_handle, buffer, HTTP_BUFFER_LEN, &bytes_read))
//					break;
//				if(bytes_read <= 0)
//					break;
//
//				buffer[bytes_read] = 0;
//				readbyte += bytes_read;
//				RealUrl += buffer;
//			}
//		}
//
//		InternetCloseHandle(internet);
//	}
//
//	return RealUrl;
//}

BOOL CmloaderApp::InitInstance()
{
	CWinApp::InitInstance();

	CString ProgramFile;
	CString lpszArgument(this->m_lpCmdLine);

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
				url = GetRealURL(url, 4096);
				if(url.Find(_T("rtsp://")) >= 0)
					lpszArgument = _T("\"") + url + _T("\"");
			}
		}
	}

	ShellExecute(0, _T("open"), ProgramFile,  lpszArgument, NULL, SW_SHOW);

	return FALSE;
}
