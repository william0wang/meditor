#include "StdAfx.h"
#include "shared.h"

#include <wininet.h>
#include <tlhelp32.h> 
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "version.lib")

#define HTTP_BUFFER_LEN 2048

static int sys_language_id = CP_ACP;	//ANSI

BOOL FileIsDirectory(LPCTSTR lpszFileName)
{
	DWORD fa = GetFileAttributes((lpszFileName));
	if(fa == INVALID_FILE_ATTRIBUTES)
		return FALSE;

	if(fa & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;

	return FALSE;
}

void GetLanguageID()
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

int ConvertUtf8ToGBK(const char *szUtf8, char *szGBK) 
{
	int len=MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL,0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte (CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL,NULL);

	delete[] wszGBK;
	return len;
}

string gbk2utf8(const string str_gbk)
{
	string sutf8;

	int len = MultiByteToWideChar(CP_ACP, 0, str_gbk.c_str(), -1, NULL,0);

	WCHAR *wszUtf8 = new WCHAR[len+1];
	memset(wszUtf8, 0, len * 2 + 2);

	MultiByteToWideChar(CP_ACP, 0, str_gbk.c_str(), -1, wszUtf8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL);

	char *szUtf8 = new char[len+1];
	memset(szUtf8, 0, len + 1);

	WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL);

	sutf8 = szUtf8;
	delete wszUtf8;
	delete szUtf8;

	return sutf8;
}

std::wstring mutil2unicode(std::string mut_str, UINT code_page)
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

std::wstring local2unicode(std::string mut_str)
{
	return mutil2unicode(mut_str, sys_language_id);
}

std::wstring utf2unicode(std::string mut_str)
{
	return mutil2unicode(mut_str, CP_UTF8);
}

std::string unicode2mutil(std::wstring uni_str, UINT code_page)
{
	std::string outstr;

	DWORD dwFlags = 0;
	if(code_page < 50220 || code_page > CP_UTF8)
		dwFlags = WC_COMPOSITECHECK|WC_DEFAULTCHAR;

	int nMultiByteLength = WideCharToMultiByte(code_page, dwFlags, uni_str.c_str(), uni_str.length(), 0, 0, 0, 0);   

	if(nMultiByteLength <= 0)
		nMultiByteLength = uni_str.length()*2;

	char* buff = new char[nMultiByteLength + 2];
	memset(buff, 0, nMultiByteLength + 2);

	WideCharToMultiByte(code_page, dwFlags, uni_str.c_str(), uni_str.length(), buff, nMultiByteLength, 0, 0);

	outstr = buff;
	delete buff;

	return outstr;
}

std::string unicode2local(std::wstring uni_str)
{
	return unicode2mutil(uni_str, sys_language_id);
}

std::string unicode2utf8(std::wstring uni_str)
{
	return unicode2mutil(uni_str, CP_UTF8);
}

void DeleteFolder(CString dir)
{
	if(FileIsDirectory(dir))
	{
		//SHFILEOPSTRUCT   Op;
		//ZeroMemory(&Op,   sizeof(Op));   //删除文件夹
		//Op.hwnd   =   NULL; 
		//Op.wFunc   =   FO_DELETE;
		//Op.pFrom   =   dir;
		//Op.fFlags   =   FOF_ALLOWUNDO   |   FOF_NOCONFIRMATION;     
		//SHFileOperation(&Op);

		CFindFile   tempFind;
		CString   tempFileFind;
		tempFileFind.Format(_T("%s\\*.*"),dir);
		BOOL   IsFinded   =   tempFind.FindFile(tempFileFind);
		while   (IsFinded)
		{
			IsFinded   =   tempFind.FindNextFile();
			if(!tempFind.IsDots())
			{
				if(tempFind.IsDirectory())
					DeleteFolder(tempFind.GetFilePath());
				else
					DeleteFile(tempFind.GetFilePath());
			}
		}
		tempFind.Close();
		RemoveDirectory(dir);
	}
}

bool get_url_string(wstring url, string &out)
{
	char buffer[HTTP_BUFFER_LEN];//下载文件的缓冲区
	DWORD bytes_read = 1;//下载的字节数
	bool getre = false;
	out = "";

	if(url.length() < 6)
		return false;

	//打开一个internet连接
	HINTERNET internet=InternetOpen(_T("User-Agent"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
	if(!internet)
		return false;

	DWORD timeout = 5000;
	InternetSetOption(internet, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(DWORD));

	timeout = 10000;
	InternetSetOption(internet, INTERNET_OPTION_SEND_TIMEOUT, &timeout, sizeof(DWORD));

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
			out += buffer;
		}
		getre = true;
	}
	//关闭连接
	InternetCloseHandle(internet);

	return getre;
}

bool Decode7zFile(wstring filename , wstring decpach, wstring ignore_path
	, vector<wstring> ignore_list, wstring exfile, Un7z_CallBack call_back)
{
	if(filename.empty())
		return false;

	char * zfile = _strdup(unicode2local(filename).c_str());
	int ign_num = ignore_list.size();
	const wchar_t **ign_list = new const wchar_t *[ign_num];
	for(int i = 0; i < ign_num; i++)
		ign_list[i] = _wcsdup(ignore_list[i].c_str());

	int result = Decode7zipFile(zfile, decpach.c_str(), exfile.c_str(), ignore_path.c_str(), ign_list, ign_num, call_back);

	for(int i = 0; i < ign_num; i++)
		delete ign_list[i];
	delete zfile;

	if(result)
		return true;

	return false;
}

bool Decode7zFileOne(CString filename , CString Path , CString ex_name)
{
	if(filename.GetLength() < 1)
		return false;
	wchar_t * dpath = NULL;
	wchar_t * exfile = NULL;

	const wchar_t **ign_list = new const wchar_t *[1];

	if(Path.GetLength() > 1) {
		dpath =  _wcsdup(Path.GetBuffer());
		Path.ReleaseBuffer();
	}
	if(ex_name.GetLength() > 1) {
		exfile =  _wcsdup(ex_name.GetBuffer());
		ex_name.ReleaseBuffer();
	}

	int result = Decode7zipFile(unicode2local(filename.GetBuffer()).c_str(), dpath, exfile, NULL, ign_list, 0, 0);
	filename.ReleaseBuffer();

	if(dpath)
		delete dpath;
	if(exfile)
		delete exfile;

	if(result)
		return true;

	return false;
}

bool ExtractResource( LPCTSTR lpName, LPCTSTR lpType, LPCTSTR lpFilePath, bool bOverWrite , bool unzip,CString ex_filename)
{
	if(!bOverWrite && FileExist(lpFilePath))
		return false;

	HRSRC  res = FindResource(NULL,  lpName,  lpType);
	if(!res)
		return false;

	HGLOBAL  gl = ::LoadResource(NULL,res);
	if(!gl)
		return false;

	LPVOID  lp = ::LockResource(gl);   //  查找，加载，锁定资源
	if(!lp)
		return false;

	CString ex_filepath = _T(""),tmp_path = _T("");
	if(unzip) {
		TCHAR szFilePath[MAX_PATH + 1];
		::GetTempPath(MAX_PATH,szFilePath);
		tmp_path.Format(_T("%s"),szFilePath);
		ex_filepath = tmp_path + _T("tmpzfile");
	} else {
		ex_filepath.Format(_T("%s"),lpFilePath);
	}

	HANDLE fp;
	if(bOverWrite)
		fp = CreateFile(ex_filepath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	else
		fp = CreateFile(ex_filepath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL);

	bool ret = false;

	if(fp != INVALID_HANDLE_VALUE) {
		DWORD aa;
		if (WriteFile (fp,lp,::SizeofResource(NULL,res),&aa,NULL))
			ret = true;
	}
	::CloseHandle (fp);       //关闭句柄
	::FreeResource (gl);     //释放内存

	if(ret && unzip) {
		if(Decode7zFileOne(ex_filepath , tmp_path , ex_filename)) {
			DeleteFile(ex_filepath);
			CopyFile(tmp_path + ex_filename , lpFilePath,FALSE);
			DeleteFile(tmp_path + ex_filename );
		}
	}

	return ret;
}

bool GetMPlayerVersion(LPCTSTR filepath, int &version, int &date)
{
	int   iVerInfoSize;
	BYTE   *pBuf;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	wstring values;
	bool res = false;
	version = 0;
	date = 0;

	iVerInfoSize = GetFileVersionInfoSize(filepath, NULL);

	if(iVerInfoSize != 0) {  
		pBuf = new BYTE[iVerInfoSize];
		TCHAR *ver;
		if(GetFileVersionInfo(filepath, 0, iVerInfoSize, pBuf) )
			if(VerQueryValue(pBuf, _T("\\StringFileInfo\\000004b0\\FileVersion"), (void **)&ver, &iFileInfoSize)) {
				values = ver;
				int index = values.find(L"SVN-r");

				if(index != wstring::npos)
					version = _ttoi(values.substr(index + 5, 5).c_str());
				index = values.find(L"(", index + 5);
				if(index != wstring::npos)
					date = _ttoi(values.substr(index + 1, 8).c_str());

				res = true;
			}
			delete pBuf;  
	}

	return res;
}
bool IsFileNew(LPCTSTR oldfile, LPCTSTR newfile)
{
	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;
	FILETIME lpLastWriteTime2;

	HANDLE file = CreateFile(oldfile, GENERIC_READ, FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if(file == INVALID_HANDLE_VALUE)
		return false;

	if(!GetFileTime(file, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime)) {
		CloseHandle(file);
		return false;
	}

	CloseHandle(file);

	file = CreateFile(newfile, GENERIC_READ, FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if(file == INVALID_HANDLE_VALUE)
		return false;

	if(!GetFileTime(file, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime2)) {
		CloseHandle(file);
		return false;
	}

	CloseHandle(file);

	if(lpLastWriteTime2.dwHighDateTime > lpLastWriteTime.dwHighDateTime ||
		lpLastWriteTime2.dwLowDateTime > lpLastWriteTime.dwLowDateTime)
		return true;

	return false;
}

void MyTerminateProcess(LPCTSTR proname)
{
	HANDLE hProcess = NULL;
	PROCESSENTRY32 pe32;
	HANDLE hProcessSnap;

	pe32.dwSize = sizeof(pe32); 
	hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE) {
		return;
	}

	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while(bMore) {
		if(_tcsicmp(pe32.szExeFile, proname) == 0) {
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pe32.th32ProcessID);
			if(hProcess) {
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
			}
			break;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}

	::CloseHandle(hProcessSnap);
}