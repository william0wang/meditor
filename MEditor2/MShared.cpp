// MShared.cpp

#include "stdafx.h"
#include "MShared.h"
#include "unrar.h"
#include "7zDec.h"
#include "resource.h"
//#include "zlib.h"
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
	wsprintf(tmp, _T("%d"), value);
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

bool UnRarFile(CString fn, CString Path)
{
	HMODULE h = LoadLibrary(_T("unrar.dll"));
	if(!h) return(false);

	RAROpenArchiveEx OpenArchiveEx = (RAROpenArchiveEx)GetProcAddress(h, "RAROpenArchiveEx");
	RARCloseArchive CloseArchive = (RARCloseArchive)GetProcAddress(h, "RARCloseArchive");
	RARReadHeaderEx ReadHeaderEx = (RARReadHeaderEx)GetProcAddress(h, "RARReadHeaderEx");
	RARProcessFile ProcessFile = (RARProcessFile)GetProcAddress(h, "RARProcessFile");
	RARSetChangeVolProc SetChangeVolProc = (RARSetChangeVolProc)GetProcAddress(h, "RARSetChangeVolProc");
	RARSetProcessDataProc SetProcessDataProc = (RARSetProcessDataProc)GetProcAddress(h, "RARSetProcessDataProc");
	RARSetPassword SetPassword = (RARSetPassword)GetProcAddress(h, "RARSetPassword");

	if(!(OpenArchiveEx && CloseArchive && ReadHeaderEx && ProcessFile 
		&& SetChangeVolProc && SetProcessDataProc && SetPassword))
	{
		FreeLibrary(h);
		return(false);
	}

	struct RAROpenArchiveDataEx ArchiveDataEx;
	memset(&ArchiveDataEx, 0, sizeof(ArchiveDataEx));
#ifdef UNICODE
	int len = 0;
	ArchiveDataEx.ArcNameW = (LPTSTR)(LPCTSTR)fn;
	char *fnA = UnicodeToLocal( fn, len);
	ArchiveDataEx.ArcName = fnA;
#else
	ArchiveDataEx.ArcName = (LPTSTR)(LPCTSTR)fn;
#endif
	ArchiveDataEx.OpenMode = RAR_OM_EXTRACT;
	ArchiveDataEx.CmtBuf = 0;
	HANDLE hrar = OpenArchiveEx(&ArchiveDataEx);
	if(!hrar) 
	{
		FreeLibrary(h);
		return(false);
	}

	struct RARHeaderDataEx HeaderDataEx;
	HeaderDataEx.CmtBuf = NULL;

	char * DestPath = NULL;
	if(Path.GetLength() > 1)
		DestPath = UnicodeToLocal(Path, len);

	while(ReadHeaderEx(hrar, &HeaderDataEx) == 0)
	{
#ifdef UNICODE
		CString subfn(HeaderDataEx.FileNameW);
#else
		CString subfn(HeaderDataEx.FileName);
#endif

		if(subfn.GetLength() > 0)
		{
			ProcessFile(hrar, RAR_EXTRACT, DestPath, NULL);
			continue;
		}
		ProcessFile(hrar, RAR_EXTRACT, NULL, NULL);
	}

	CloseArchive(hrar);
	FreeLibrary(h);

#ifdef UNICODE
	delete fnA;
#endif
	return true;
}

bool Decode7zFile(CString filename , CString Path , CString ex_name)
{
	if(filename.GetLength() < 1)
		return false;
	int len = 0;
	char * dpath = NULL;
	char * exfile = NULL;
	if(Path.GetLength() > 1)
		dpath =  UnicodeToLocal(Path, len);
	if(ex_name.GetLength() > 1)
		exfile =  UnicodeToLocal(ex_name, len);
	char * file = UnicodeToLocal(filename, len);
	int result = Un7zFile(file,dpath,exfile);
	if(dpath)
		delete dpath;
	delete file;
	if(result)
		return true;
	return false;
}

bool ExtractResource( LPCTSTR lpName, LPCTSTR lpType, LPCTSTR lpFilePath, bool bOverWrite , bool unzip,CString ex_filename)
{
    if(bOverWrite==false)
    {
        CFileFind ff;
        if(ff.FindFile(lpFilePath))
            return false;
    }
    HRSRC  res = FindResource(NULL,  lpName,  lpType);
    if(res==NULL)
        return false;
    HGLOBAL  gl = ::LoadResource(NULL,res);
    if(gl==NULL)
        return false;

    LPVOID  lp = ::LockResource(gl);   //  查找，加载，锁定资源
    if(lp==NULL)
        return false;

	CString ex_filepath = _T(""),tmp_path = _T("");
	if(unzip)
	{
		TCHAR szFilePath[MAX_PATH + 1];
		::GetTempPath(MAX_PATH,szFilePath);
		tmp_path.Format(_T("%s"),szFilePath);
		ex_filepath = tmp_path + _T("tmpzfile");
	}
	else
	{
		ex_filepath.Format(_T("%s"),lpFilePath);
	}

    HANDLE fp;
    if(bOverWrite)
        fp = CreateFile(ex_filepath,      GENERIC_WRITE, FILE_SHARE_READ
			, NULL,      CREATE_ALWAYS,      0,     NULL);
    else
        fp = CreateFile(ex_filepath, GENERIC_WRITE, FILE_SHARE_READ
			,   NULL, CREATE_NEW,  0,  NULL);

    bool ret = false;

    if(fp != INVALID_HANDLE_VALUE)
    {
        DWORD aa;
        if (WriteFile (fp,lp,::SizeofResource(NULL,res),&aa,NULL))
            ret = true;
    }
    ::CloseHandle (fp);       //关闭句柄
    ::FreeResource (gl);     //释放内存

	if(ret && unzip)
	{
		if(Decode7zFile(ex_filepath , tmp_path , ex_filename))
		{
			DeleteFile(ex_filepath);
			CopyFile(tmp_path + ex_filename , lpFilePath,FALSE);
			DeleteFile(tmp_path + ex_filename );
		}
	}

    return ret;
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
	int len = dig_str.GetLength();
	if(len <= 0)
		return false;
	for(int i = 0 ; i < len ; i++)
	{
		if(!_istdigit(dig_str[i]))
			return false;
	}
	return true;
}

