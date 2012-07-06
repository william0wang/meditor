// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <string>
#include <vector>
#include <wininet.h>

using namespace std;

#include "Download.h"

static vector<CDownloader *> downloader_list;

static int sys_language_id = CP_ACP;	//ANSI

static void GetLanguageID()
{
	LANGID   _SysLangId   =   GetSystemDefaultLangID();

	if(PRIMARYLANGID(_SysLangId)   ==   LANG_CHINESE)
	{
		if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_SIMPLIFIED)
			sys_language_id = 936;		//Simplified Chinese GBK
		else if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_TRADITIONAL)
			sys_language_id = 950;		//Traditional Chinese Big5
		else if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_HONGKONG)
			sys_language_id = 950;		//Traditional Chinese Big5
		else
			sys_language_id = CP_ACP;		//ANSI
	}
	else if(PRIMARYLANGID(_SysLangId)   ==   LANG_JAPANESE)
		sys_language_id = 932;			//Japanese Shift-JIS
	else if(PRIMARYLANGID(_SysLangId)   ==   LANG_KOREAN)
		sys_language_id = 949;			//Korean
	else
		sys_language_id = CP_ACP;			//ANSI
}

static std::wstring mutil2unicode(std::string mut_str, UINT code_page)
{
	std::wstring outstr;

	int nWideByteLength = MultiByteToWideChar(code_page,0, mut_str.c_str(), -1, NULL,0);

	if(nWideByteLength <= 0)
		nWideByteLength = mut_str.length();

	wchar_t * buff = new wchar_t[nWideByteLength];
	memset(buff, 0, sizeof(wchar_t) * nWideByteLength);

	MultiByteToWideChar(code_page, 0, mut_str.c_str(), -1, buff, nWideByteLength);

	outstr = buff;
	delete buff;

	return outstr;
}

static std::wstring local2unicode(std::string mut_str)
{
	return mutil2unicode(mut_str, sys_language_id);
}

static UINT StopThread(LPVOID pParam)
{
	CDownloader *down = (CDownloader *) pParam;
	down->StopDownload();
	delete down;
	return 0;
}

int __stdcall StartDownloaderW(const wchar_t *url, const wchar_t *path, const wchar_t *filename, FUNC_CallBack Callback, WPARAM wParam)
{
	CDownloader *downloader = new CDownloader;
	downloader->SetCallBack(Callback, wParam);
	downloader->StartDownload(url, path, filename);
	downloader_list.push_back(downloader);
	return downloader_list.size() - 1;
}

int __stdcall StartDownloader(const char *url, const char *path, const char *filename, FUNC_CallBack Callback, WPARAM wParam)
{
	return StartDownloaderW(local2unicode(url).c_str(), local2unicode(path).c_str()
		, local2unicode(filename).c_str(), Callback, wParam);
}

int __stdcall StopDownloader(unsigned int index)
{
	unsigned int i = 0;
	if(index >= 0 && index < downloader_list.size()) {
		vector<CDownloader *>::iterator it;
		for(it=downloader_list.begin();it!= downloader_list.end();it++) {
			if(i == index) {
				if(*it) {
					CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StopThread, (*it), 0, 0);
					(*it) = NULL;
				}
				break;
			}
			++i;
		}
		return 0;
	}
	return -1;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		GetLanguageID();
		downloader_list.clear();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		vector<CDownloader *>::iterator it;
		for(it = downloader_list.begin(); it != downloader_list.end(); it++) {
			if(*it) {
				(*it)->StopDownload();
				delete (*it);
				(*it) = NULL;
			}
		}
		downloader_list.clear();
		break;
	}
	return TRUE;
}

