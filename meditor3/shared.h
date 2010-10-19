#pragma once
#include <string>
#include <vector>
#include "../Libs/7z/Util/7z/7zMain.h"
using namespace std;

#define FileExist(lpszFileName) (::GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)

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

bool GetMPlayerVersion(LPCTSTR filepath, int &version, int &date);

void MyTerminateProcess(LPCTSTR proname);
