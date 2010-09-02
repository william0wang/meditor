
typedef enum _DownloadResult
{
	ENUM_DOWNLOAD_RESULT_SUCCESS,
	ENUM_DOWNLOAD_RESULT_FAILED,
	ENUM_DOWNLOAD_RESULT_CANCEL,
} ENUM_DOWNLOAD_RESULT;

enum
{
	NOTIFY_TYPE_GOT_REMOTE_FILENAME,				// 取得远程站点文件名, 当被下载的文件被重定向时才发送该通知，lpNotifyData 为 LPCTSTR 类型的文件名字符串指针
	NOTIFY_TYPE_GOT_REMOTE_FILESIZE,				// 取得远程站点文件大小，lpNotifyData 为 int 类型的文件大小
	NOTIFY_TYPE_START_DOWNLOAD,						// 开始下载，lpNotifyData 无用
	NOTIFY_TYPE_END_DOWNLOAD,						// 结束下载，lpNotifyData 为 ENUM_DOWNLOAD_RESULT 类型的下载结果
	NOTIFY_TYPE_WILL_DOWNLOAD_SIZE,					// 本次需要下载的大小，lpNotifyData 为 int 类型的需要下载的大小
	NOTIFY_TYPE_THREAD_DOWNLOADED_SIZE,				// 某线程已下载的数据大小，lpNotifyData 为 int 类型的已下载的大小
};

typedef struct _DownloadNotifyPara
{
	int nIndex;
	UINT nNotityType;
	LPVOID lpNotifyData;
	LPVOID pDownloader;
} t_DownloadNotifyPara;

typedef void (*FUNC_CallBack) ( t_DownloadNotifyPara *pDownloadNotifyPara, WPARAM wParam );

class CDownloader {
private:
	bool			bStop;
	HANDLE			hThreadDownload;

	wstring			download_url;
	wstring			file_name;
	wstring			file_path;

	FUNC_CallBack	m_CallBack;
	WPARAM			m_wPrarm;

	static UINT DownloadThread(LPVOID pParam);

	std::wstring& wtrimRight(std::wstring &s, std::wstring sp);

	bool ParseURL(wstring url, wstring &HostName, wstring &FileName);

	DWORD __fastcall GetHttpFileSize(HINTERNET hRequest);

	wstring __fastcall GetHttpFileEtag(HINTERNET hRequest);

	SYSTEMTIME __fastcall GetHttpFileDateTime(HINTERNET hRequest);

	void HttpGetFile(wstring url, wstring path, wstring filename);

	void StartHttpDownload();

	void DoCallBack(int nIndex, UINT nNotityType, LPVOID lpNotifyData = NULL, LPVOID pDownloader = NULL);

	FILE * OpenInfoFile(wstring infofile, wstring etag, SYSTEMTIME last_time, DWORD file_size, long &download_size, bool rewrite);
	void CloseInfoFile(FILE *fpinfo, wstring infofile, DWORD file_size, DWORD download_size);
	void UpdateInfo(FILE *fpinfo, DWORD download_size);

	BOOL CheckDownloadFile(wstring filename, DWORD file_size, SYSTEMTIME last_time);

	void SetTimeStamp(wstring filename, SYSTEMTIME last_time);

public:

	CDownloader();
	~CDownloader();

	void SetCallBack(FUNC_CallBack callback, WPARAM wParam);

	void StartDownload(wstring url, wstring path, wstring filename);

	void StopDownload();
};
