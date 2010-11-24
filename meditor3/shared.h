#pragma once
#include <string>
#include <vector>
#include "../Libs/7z/Util/7z/7zMain.h"
#include "ResStr.h"
#include "MConfig.h"
using namespace std;

extern CResStr rStr;
extern CMConfig mconfig;

#define FileExist(lpszFileName) (::GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)

#define ResStr(id) CString(MAKEINTRESOURCE(id))

#ifndef _ttof
#ifdef _UNICODE
#define _ttof	_wtof
#else
#define _ttof	atof
#endif
#endif

BOOL FileIsDirectory(LPCTSTR lpszFileName);

void DeleteFolder(CString dir);

bool get_url_string(std::wstring url, std::string &out);

std::string unicode2mutil(std::wstring uni_str, UINT code_page);
std::string unicode2local(std::wstring uni_str);
std::string unicode2utf8(std::wstring uni_str);

std::wstring mutil2unicode(std::string mut_str, UINT code_page);
std::wstring local2unicode(std::string mut_str);
std::wstring utf2unicode(std::string mut_str);

bool Decode7zFile(std::wstring filename , std::wstring decpach, std::wstring ignore_path
	, vector<wstring> ignore_list, std::wstring exfile = L"", Un7z_CallBack call_back = NULL);

bool ExtractResource(LPCTSTR lpName, LPCTSTR lpType, LPCTSTR lpFilePath, bool bOverWrite , bool unzip,CString ex_filename);

bool GetMPlayerVersion(LPCTSTR filepath, int &version, int &date);

void MyTerminateProcess(LPCTSTR proname);

void TestURL(CString url, int fullbyte);
