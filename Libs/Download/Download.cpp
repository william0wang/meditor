// DownloaderAtl.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include <wininet.h>

using namespace std;

#include "Download.h"

#pragma comment(lib, "wininet.lib")

#define FileExist(lpszFileName) (::GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)

#define HTTP_BUFFER_LEN		4096

BOOL EqualSystemTime(SYSTEMTIME st1, SYSTEMTIME st2)
{
	if(st1.wYear != st2.wYear)
		return FALSE;
	if(st1.wMonth != st2.wMonth)
		return FALSE;
	if(st1.wDay != st2.wDay)
		return FALSE;

	if(st1.wHour != st2.wHour)
		return FALSE;
	if(st1.wMinute != st2.wMinute)
		return FALSE;
	if(st1.wSecond != st2.wSecond)
		return FALSE;

	return TRUE;
}

UINT CDownloader::DownloadThread(LPVOID pParam)
{
	CDownloader *downloader = (CDownloader *)pParam;
	downloader->StartHttpDownload();
	SetEvent(downloader->hDownloadFinished);
	return 0;
}

CDownloader::CDownloader()
{
	bStop = false;
	m_CallBack = NULL;
	hDownloadFinished = NULL;
	m_wPrarm = 0;
}

CDownloader::~CDownloader()
{
	if(!bStop)
		StopDownload();
	m_CallBack = NULL;
}

void CDownloader::StartDownload(wstring url, wstring path, wstring filename)
{
	bStop = false;
	download_url = url;
	file_path = path;
	file_name = filename;

	hDownloadFinished = CreateEvent(NULL, FALSE, FALSE, NULL);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DownloadThread, this, 0, 0);
}

void CDownloader::StopDownload()
{
	bStop = true;
	
	WaitForSingleObject(hDownloadFinished, INFINITE);
	return;
}

void CDownloader::SetCallBack(FUNC_CallBack callback, WPARAM wParam)
{
	m_CallBack = callback;
	m_wPrarm = wParam;
}

void CDownloader::DoCallBack(int nIndex, UINT nNotityType, LPVOID lpNotifyData, LPVOID pDownloader)
{
	if(!m_CallBack)
		return;

	t_DownloadNotifyPara *pDownloadNotifyPara = new t_DownloadNotifyPara;
	pDownloadNotifyPara->nIndex = nIndex;
	pDownloadNotifyPara->nNotityType = nNotityType;
	pDownloadNotifyPara->lpNotifyData = lpNotifyData;
	pDownloadNotifyPara->pDownloader = pDownloader;

	(*m_CallBack)(pDownloadNotifyPara, m_wPrarm);
}

void CDownloader::StartHttpDownload()
{
	HttpGetFile(download_url, file_path, file_name);
}

std::wstring& CDownloader::wtrimRight(std::wstring &s, std::wstring sp)
{
	if (s.empty())
		return s;
	s.erase(s.find_last_not_of(sp) + 1);
	return s;
}

bool CDownloader::ParseURL(wstring url, wstring &HostName, wstring &FileName)
{
	int index = url.find(L"http://");
	if(index != wstring::npos)
		url = url.substr(index+7);

	index = url.find(L"/");
	if(index != wstring::npos) {
		HostName = url.substr(0, index);
		FileName = url.substr(index);
		return true;
	}

	return false;
}

DWORD __fastcall CDownloader::GetHttpFileSize(HINTERNET hRequest)
{
	TCHAR Buffer[HTTP_BUFFER_LEN];
	DWORD BufLen = HTTP_BUFFER_LEN;
	DWORD dwIndex=0;
	BOOL RetQueryInfo = HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, Buffer, &BufLen, &dwIndex);
	if(!RetQueryInfo)
		return 0;

	DWORD len = (DWORD) (0xffffffff & _ttoi64(Buffer));
	
	return len;
}

SYSTEMTIME __fastcall CDownloader::GetHttpFileDateTime(HINTERNET hRequest)
{
	SYSTEMTIME Buffer;
	DWORD BufLen = sizeof(SYSTEMTIME);

	DWORD dwIndex=0;
	BOOL RetQueryInfo = HttpQueryInfo(hRequest, HTTP_QUERY_LAST_MODIFIED|HTTP_QUERY_FLAG_SYSTEMTIME, &Buffer, &BufLen, &dwIndex);
	if(!RetQueryInfo)
		ZeroMemory(&Buffer, BufLen);

	return Buffer;
}

wstring __fastcall CDownloader::GetHttpFileEtag(HINTERNET hRequest)
{
	TCHAR Buffer[HTTP_BUFFER_LEN];
	DWORD BufLen = HTTP_BUFFER_LEN;
	wstring etag;

	DWORD dwIndex=0;
	BOOL RetQueryInfo = HttpQueryInfo(hRequest, HTTP_QUERY_ETAG, &Buffer, &BufLen, &dwIndex);
	if(RetQueryInfo)
		etag = Buffer;

	return etag;
}

FILE * CDownloader::OpenInfoFile(wstring infofile, wstring etag, SYSTEMTIME last_time, DWORD file_size, long &download_size, bool rewrite)
{
	wstring etagw = etag;

	etagw += L"                                    ";
	etagw += L"                                    ";
	const int etaglen = 64;
	FILE *fpinfo = NULL;

	if(file_size <= 0)
		return 0;

	DWORD dsize = 0;
	SYSTEMTIME dtime;
	download_size = 0;
	if(!rewrite && FileExist(infofile.c_str())){
		wstring str;
		wchar_t wbuffer[etaglen+1];
		fpinfo = _wfopen(infofile.c_str(), L"r+b");
		if(fpinfo) {
			size_t rs = fread(&dsize, sizeof(DWORD), 1, fpinfo);
			if(rs != 1 || dsize != file_size)
				goto ReWriteInfoFile;
			rs = fread(&dtime, sizeof(SYSTEMTIME), 1, fpinfo);
			if(rs != 1 || !EqualSystemTime(dtime, last_time))
				goto ReWriteInfoFile;
			rs = fread(&dsize, sizeof(DWORD), 1, fpinfo);
			if(rs != 1)
				goto ReWriteInfoFile;

			rs = fread(wbuffer, sizeof(wchar_t), etaglen, fpinfo);
			if(rs != etaglen)
				goto ReWriteInfoFile;

			wbuffer[etaglen]=0;
			str = wbuffer;
			wtrimRight(str, L" ");

			if(str.compare(etag))
				goto ReWriteInfoFile;
			download_size = dsize;
		}
	} else {
		fpinfo = _wfopen(infofile.c_str(), L"w+b");
		if(fpinfo) {
ReWriteInfoFile:
			fseek(fpinfo, 0, SEEK_SET);
			fwrite(&file_size, sizeof(DWORD), 1, fpinfo);
			fwrite(&last_time, sizeof(SYSTEMTIME), 1, fpinfo);
			fwrite(&dsize, sizeof(DWORD), 1, fpinfo);
			fwrite(etagw.c_str(), sizeof(wchar_t), etaglen, fpinfo);
		}
	}

	return fpinfo;
}

void CDownloader::UpdateInfo(FILE *fpinfo, DWORD download_size)
{
	if(!fpinfo)
		return;

	fseek(fpinfo, sizeof(DWORD) + sizeof(SYSTEMTIME), SEEK_SET);
	fwrite(&download_size, sizeof(DWORD), 1, fpinfo);
	if(!bStop)
		DoCallBack(0, NOTIFY_TYPE_THREAD_DOWNLOADED_SIZE, (LPVOID)download_size);
}

void CDownloader::CloseInfoFile(FILE *fpinfo, wstring infofile, DWORD file_size, DWORD download_size)
{
	if(fpinfo)
		fclose(fpinfo);

	if(file_size <= download_size)
		DeleteFile(infofile.c_str());
}

void CDownloader::SetTimeStamp(wstring filename, SYSTEMTIME last_time)
{
	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;

	HANDLE file = CreateFile(filename.c_str(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if(file == INVALID_HANDLE_VALUE)
		return;


	if(!GetFileTime(file, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime)) {
		CloseHandle(file);
		return;
	}

	if(!SystemTimeToFileTime(&last_time, &lpLastWriteTime)) {
		CloseHandle(file);
		return;
	}

	SetFileTime(file, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime);

	CloseHandle(file);

}

BOOL CDownloader::CheckDownloadFile(wstring filename, DWORD file_size, SYSTEMTIME last_time)
{
	if(!FileExist(filename.c_str()))
		return FALSE;

	HANDLE file = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ
		, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if(file == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD lsize;

	DWORD dsize = GetFileSize(file, &lsize);

	if(lsize > 0) {
		CloseHandle(file);
		return FALSE;
	}

	if(dsize != file_size) {
		CloseHandle(file);
		return FALSE;
	}

	FILETIME lpCreationTime;
	FILETIME lpLastAccessTime;
	FILETIME lpLastWriteTime;
	SYSTEMTIME ltime;

	if(!GetFileTime(file, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime)) {
		CloseHandle(file);
		return FALSE;
	}

	if(!FileTimeToSystemTime(&lpLastWriteTime, &ltime)) {
		CloseHandle(file);
		return FALSE;
	}

	if(!EqualSystemTime(ltime, last_time)) {
		CloseHandle(file);
		return FALSE;
	}

	CloseHandle(file);

	return TRUE;
}

void CDownloader::HttpGetFile(wstring url, wstring path, wstring filename)
{
	wstring HostName,fileName;
	ParseURL(url, HostName, fileName);
	ENUM_DOWNLOAD_RESULT eDownloadResult;

	DoCallBack(0, NOTIFY_TYPE_START_DOWNLOAD);

	eDownloadResult = ENUM_DOWNLOAD_RESULT_FAILED;

	HINTERNET hSession = InternetOpen(_T("HTTP"), INTERNET_OPEN_TYPE_PRECONFIG,	NULL, NULL, NULL); 
	if(!hSession) {
		DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
		return;
	}

	DWORD timeout = 5000;
	InternetSetOption(hSession, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(DWORD));

	timeout = 10000;
	InternetSetOption(hSession, INTERNET_OPTION_SEND_TIMEOUT, &timeout, sizeof(DWORD));

	HINTERNET hConnect = InternetConnect(hSession, HostName.c_str(), 0, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if(!hConnect) {
		InternetCloseHandle(hSession);
		DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
		return;
	}
	
	if(bStop) {
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		eDownloadResult = ENUM_DOWNLOAD_RESULT_CANCEL;
		DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
		return;
	}
	
	TCHAR szHead[] = _T("Accept: */*\r\n\r\n");
	TCHAR **p = new TCHAR*[2];
	*p = szHead;
	*(p+1) = NULL;

	HINTERNET hRequest = HttpOpenRequest(hConnect, _T("GET"), fileName.c_str(), _T("HTTP/1.1"), NULL, (LPCTSTR *)p, INTERNET_FLAG_RELOAD, 0);
	if(!hRequest) {
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
		return;
	}

	if(bStop) {
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		eDownloadResult = ENUM_DOWNLOAD_RESULT_CANCEL;
		DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
		return;
	}

	if(!HttpSendRequest(hRequest, NULL, 0, NULL, 0)) {
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
		return;
	}

	if(bStop) {
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		eDownloadResult = ENUM_DOWNLOAD_RESULT_CANCEL;
		DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
		return;
	}

	DWORD fsize = GetHttpFileSize(hRequest);
	wstring etag = GetHttpFileEtag(hRequest);
	SYSTEMTIME ltime = GetHttpFileDateTime(hRequest);
	bool bRewrite = false;

	DoCallBack(0, NOTIFY_TYPE_GOT_REMOTE_FILESIZE, (LPVOID)fsize);

	if (path.find_last_of(L"\\") != path.size() - 1)
		path += L"\\";

	wstring infofile = path + filename + L".info";
	filename = path + filename;

	if(CheckDownloadFile(filename, fsize, ltime)) {
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		eDownloadResult = ENUM_DOWNLOAD_RESULT_SUCCESS;
		DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
		return;
	}

	DWORD bytes_read = 1;
	FILE *fpinfo = NULL;
	long seek_pos = 0;
	if(fsize > 0) {
		fpinfo = OpenInfoFile(infofile, etag, ltime, fsize, seek_pos, bRewrite);

		wstring flag = L"w+b";

		if(FileExist(filename.c_str())) {
			if(seek_pos > 0)
				flag = L"r+b";
			else
				DeleteFile(filename.c_str());
		}

		FILE *downfile = _wfopen(filename.c_str(), flag.c_str());

		fseek(downfile, 0, SEEK_END);
		long dfsize = ftell(downfile);

		if(dfsize < seek_pos)
			seek_pos = dfsize;

		if(fpinfo && seek_pos > 0) {

			wstring header;
			wostringstream tmp;
			tmp << L"Range:bytes=" << seek_pos << L"-";
			header = tmp.str();


			InternetCloseHandle(hRequest);
			HINTERNET hRequest = HttpOpenRequest(hConnect, _T("GET"), fileName.c_str(), _T("HTTP/1.1"), NULL, (LPCTSTR *)p, INTERNET_FLAG_RELOAD, 0);
			if(!hRequest) {
				InternetCloseHandle(hConnect);
				InternetCloseHandle(hSession);
				DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
				return;
			}

			if(bStop) {
				InternetCloseHandle(hRequest);
				InternetCloseHandle(hConnect);
				InternetCloseHandle(hSession);
				eDownloadResult = ENUM_DOWNLOAD_RESULT_CANCEL;
				DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
				return;
			}

			if(!HttpSendRequest(hRequest, header.c_str(), header.length(), NULL, 0))
				seek_pos = 0;

			if(bStop) {
				fclose(downfile);
				InternetCloseHandle(hRequest);
				InternetCloseHandle(hConnect);
				InternetCloseHandle(hSession);
				eDownloadResult = ENUM_DOWNLOAD_RESULT_CANCEL;
				DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
				return;
			}
		}
		
		DWORD writelen = seek_pos;
		if(downfile) {
			if(seek_pos >= 0)
				fseek(downfile, seek_pos, SEEK_SET);

			DWORD willsize = fsize - seek_pos;
			DoCallBack(0, NOTIFY_TYPE_WILL_DOWNLOAD_SIZE, (LPVOID)willsize);

			byte *buffer = new byte[HTTP_BUFFER_LEN * 10];
			while(bytes_read > 0) {
				if(bStop) {
					eDownloadResult = ENUM_DOWNLOAD_RESULT_CANCEL;
					break;
				}
				if(!InternetReadFile(hRequest, buffer, HTTP_BUFFER_LEN , &bytes_read))
					break;
				if(bytes_read <= 0)
					break;
				fwrite(buffer, 1, bytes_read, downfile);
				writelen += bytes_read;

				UpdateInfo(fpinfo, writelen);
			}
			delete buffer;
			fclose(downfile);
		}

		CloseInfoFile(fpinfo, infofile, fsize, writelen);

		SetTimeStamp(filename, ltime);
	}

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);

	if(CheckDownloadFile(filename, fsize, ltime))
		eDownloadResult = ENUM_DOWNLOAD_RESULT_SUCCESS;

	DoCallBack(0, NOTIFY_TYPE_END_DOWNLOAD, (LPVOID)eDownloadResult);
}

