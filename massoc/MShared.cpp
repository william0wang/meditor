// MShared.cpp

#include "stdafx.h"
#include "MShared.h"
#include "resource.h"
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#define GZIPBYTES 512
#define MEDITOR_HTTP_BUFFER_LEN 512

static int sys_language_id = CP_ACP;	//ANSI

void GetLanguageID()
{
	LANGID   _SysLangId   =   GetSystemDefaultLangID();
	if(PRIMARYLANGID(_SysLangId)   ==   LANG_CHINESE)
	{
		if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_SIMPLIFIED)
			sys_language_id = 936;		//Simplified Chinese GBK
		else if(SUBLANGID(_SysLangId)   ==   SUBLANG_CHINESE_TRADITIONAL)
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

char* UnicodeToLocal(CString instr,int &outlen)
{
#ifdef UNICODE
	CString   strTmp(instr);
	UINT nCodePage = sys_language_id;
	int   nMultiByteLength   =   WideCharToMultiByte(nCodePage,   WC_COMPOSITECHECK   |   WC_DEFAULTCHAR,     strTmp.GetBuffer(instr.GetLength()),   instr.GetLength(),   0,   0,   0,   0);   
	char*   buff   =   new   char[nMultiByteLength + 2];
	memset(buff,0,nMultiByteLength+ 1);
	WideCharToMultiByte(nCodePage,   WC_COMPOSITECHECK   |   WC_DEFAULTCHAR,   strTmp.GetBuffer(instr.GetLength()),   instr.GetLength(),buff,   nMultiByteLength ,   0,   0);  
#else
	char*   buff   =   new   char[instr.GetLength() + 1];
	memset(buff,0,instr.GetLength()+ 1);
	for(int i = 0; i < instr.GetLength(); i++)
	{
		buff[i] = instr[i];
	}
#endif
	outlen = (int)strlen(buff);
	return buff;
}

CString LocalToUnicode(CString instr)
{
#ifdef UNICODE
	TCHAR* inchar = instr.GetBuffer(instr.GetLength());
	char* szGBString = new char[instr.GetLength() + 1];
	for(int i = 0; i < instr.GetLength(); i++)
	{
		szGBString[i] = (char)inchar[i];
	}
	instr.ReleaseBuffer();
	szGBString[instr.GetLength()] = 0;
	UINT nCodePage = sys_language_id; 
	int nLength=MultiByteToWideChar(nCodePage,0,szGBString,-1,NULL,0);
	wchar_t* pBuffer = new wchar_t[nLength+1];
	MultiByteToWideChar(nCodePage,0,szGBString,-1,pBuffer,nLength);
	pBuffer[nLength]=0;
	CString out(pBuffer);
	delete szGBString;
	delete pBuffer;
	return out;
#else
	return instr;
#endif
}

double StringToDouble(CString value_str)
{
	double value = 0.0;
	int offset = value_str.Find(_T("."));
	if(offset < 0)
		return (double)_ttoi(value_str);
	int right = value_str.GetLength() - offset - 1;
	if(right < 0)
		return (double)_ttoi(value_str);
	CString Integer = value_str.Left(offset) + value_str.Right(right);
	offset = _ttoi(Integer);
	value = (double)offset;
	for(int i = 0 ; i < right ; i ++)
		value = value / 10.0;
	return value;
}

void TestURL(CString url,int fullbyte)
{
    TCHAR buffer[MEDITOR_HTTP_BUFFER_LEN];//下载文件的缓冲区
    DWORD bytes_read;//下载的字节数
    //打开一个internet连接
    HINTERNET internet=InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
    if(!internet)
        return;
        //打开一个http url地址
    HINTERNET file_handle=InternetOpenUrl(internet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);

    if(!file_handle)
        return;
    //从url地址中读取文件内容到缓冲区buffer
	BOOL b = 0;
	int readbyte = 0, time = 0;
	while(readbyte < MEDITOR_HTTP_BUFFER_LEN && time < 3000)
	{
		b = InternetReadFile(file_handle, buffer, 128 , &bytes_read);
		readbyte += bytes_read;
		time++;
	}
    if(!b)
        return;
    //关闭连接
    InternetCloseHandle(internet);
}

#ifndef BIF_USENEWUI
#define BIF_USENEWUI           (BIF_NEWDIALOGSTYLE | BIF_EDITBOX)
#endif

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE     0x0040
#endif

bool SelectFolder(HWND hWnd,CString& strFolder)
{
	strFolder.Empty(); 
	LPMALLOC lpMalloc; 
	if (::SHGetMalloc(&lpMalloc) != NOERROR)
		return false; 
	bool result = false;
	TCHAR szDisplayName[_MAX_PATH];
	TCHAR szBuffer[_MAX_PATH]; 
	BROWSEINFO browseInfo; 
	browseInfo.hwndOwner = hWnd; 
	browseInfo.pidlRoot = NULL; // set root at Desktop
	browseInfo.pszDisplayName = szDisplayName; 
	browseInfo.ulFlags = BIF_USENEWUI |BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
	browseInfo.lpfn = NULL;
	browseInfo.lParam = 0;
	LPITEMIDLIST lpItemIDList;
	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer)) 
		{
			// At this point, szBuffer contains the path the user chose. 
			// We have a path in szBuffer! Return it. 
			if (!szBuffer[0] == '\0') 
				strFolder.Format(_T("%s"),szBuffer);
			result = true; 
		}  
		lpMalloc->Free(lpItemIDList);
		lpMalloc->Release();
	} 
	return result;
}

BOOL WritePrivateProfileInt(CString section, CString key, int value, CString profile)
{
	TCHAR tmp[16];
	_stprintf_s(tmp, _T("%d"), value);
	return WritePrivateProfileString(section, key, tmp, profile);
}

bool UnGZipFile(CString sourcefile,CString destfile)
{
	//Byte uncompr[GZIPBYTES];
	//int len = 0;
	//memset(uncompr,0,GZIPBYTES);
	//const char * source_file = UnicodeToLocal( sourcefile, len);
	//gzFile gzip_file = gzopen(source_file,"rb");
	//CFile myfile;
	//if(!myfile.Open(destfile,CFile::modeCreate|CFile::modeWrite))
	//	return 0;

	//while((len = gzread(gzip_file, uncompr,GZIPBYTES)) > 0)
	//{
	//	myfile.Write(uncompr,len);
	//	memset(uncompr,0,GZIPBYTES);
	//}
	//gzclose(gzip_file);
	//myfile.Close();
	//delete source_file;
	return true;
}

bool GetSpecialFolder(int nFolder , TCHAR *path)
{
	LPITEMIDLIST  pidl;
	LPMALLOC      pShellMalloc;
	bool result = false;
	if(SUCCEEDED(SHGetMalloc(&pShellMalloc)))
	{
		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, nFolder, &pidl)))
		{
			if(SHGetPathFromIDList(pidl, path))
				result = true;
			pShellMalloc->Free(pidl);
		}
		pShellMalloc->Release();
	}
	return result;
}

void DeleteFolder(CString dir)
{
	if(IsFileExist(dir))
	{
		//SHFILEOPSTRUCT   Op;
		//ZeroMemory(&Op,   sizeof(Op));   //删除文件夹
		//Op.hwnd   =   NULL; 
		//Op.wFunc   =   FO_DELETE;
		//Op.pFrom   =   dir;
		//Op.fFlags   =   FOF_ALLOWUNDO   |   FOF_NOCONFIRMATION;     
		//SHFileOperation(&Op);

		CFileFind   tempFind;
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

bool IsDigit(CString dig_str)
{
	if(dig_str[0] == _T('-') || dig_str[0] == _T('+'))
		dig_str = dig_str.Right(dig_str.GetLength() - 1);
	int len = dig_str.GetLength();
	if(len <= 0)
		return false;
	for(int i = 0 ; i < len ; i++)
	{
		if(!_istdigit(dig_str[i]) && dig_str[i] != _T('.'))
			return false;
	}
	return true;
}
