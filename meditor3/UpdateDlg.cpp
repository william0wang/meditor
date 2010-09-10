// UpdateDlg.cpp : implementation of the CUpdateDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "../Libs/tinyxml/tinyxml.h"
#include "Win7ShellApi.h"
#include <tlhelp32.h> 

#include "UpdateDlg.h"

#define DOWNLOAD_WPARAM			123123

static ChangeWindowMessageFilterFunction ChangeWindowMessageFilterDLL = NULL;

typedef struct _DownloadNotifyPara
{
	int nIndex;
	UINT nNotityType;
	LPVOID lpNotifyData;
	LPVOID pDownloader;
} t_DownloadNotifyPara;

typedef void (*FUNC_CallBack) ( t_DownloadNotifyPara *pDownloadNotifyPara, WPARAM wParam );

extern int __stdcall StartDownloaderW(const wchar_t *url, const wchar_t *path, const wchar_t *filename, FUNC_CallBack Callback, WPARAM wParam);
extern int __stdcall StopDownloader(unsigned int index);

static CUpdateDlg *updatedlg;

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

static void Callback_Download( t_DownloadNotifyPara *pDownloadNotifyPara, WPARAM wParam )
{
	if( !pDownloadNotifyPara )
		return;

	if(wParam != DOWNLOAD_WPARAM)
		return;

	if(!updatedlg)
		return;

	int index = pDownloadNotifyPara->nIndex;

	switch ( pDownloadNotifyPara->nNotityType )
	{
	case NOTIFY_TYPE_GOT_REMOTE_FILENAME:
		{
			break;
		}
	case NOTIFY_TYPE_GOT_REMOTE_FILESIZE:
		{
			//PostMessage(updatedlg->m_hWnd, WM_MSG_DOWNLOADER_FILESIZE, wParam, (LPARAM)pDownloadNotifyPara->lpNotifyData);
			updatedlg->m_filesize = (int)pDownloadNotifyPara->lpNotifyData;
			AtlTrace("Remote file size is : %d bytes\n", (int)pDownloadNotifyPara->lpNotifyData);
			break;
		}
	case NOTIFY_TYPE_START_DOWNLOAD:
		{
			AtlTrace(L"Downloading Start");
			break;
		}
	case NOTIFY_TYPE_END_DOWNLOAD:
		{
			ENUM_DOWNLOAD_RESULT eDownloadResult = (ENUM_DOWNLOAD_RESULT)(int)pDownloadNotifyPara->lpNotifyData;
			if ( eDownloadResult == ENUM_DOWNLOAD_RESULT_SUCCESS ) {
				PostMessage(updatedlg->m_hWnd, WM_MSG_DOWNLOADER_FINISHED, wParam, 1);
				AtlTrace(L"Download successfully.\n");
			} else if ( eDownloadResult == ENUM_DOWNLOAD_RESULT_FAILED ) {
				PostMessage(updatedlg->m_hWnd, WM_MSG_DOWNLOADER_FINISHED, wParam, 0);
				AtlTrace(L"Download failed.\n");
			} else if ( eDownloadResult == ENUM_DOWNLOAD_RESULT_CANCEL )
				AtlTrace(L"User cancel download.\n");
			else {
				PostMessage(updatedlg->m_hWnd, WM_MSG_DOWNLOADER_FINISHED, wParam, 0);
				AtlTrace(L"Unknow Download Finished.\n");
			}
			break;
		}
	case NOTIFY_TYPE_THREAD_DOWNLOADED_SIZE:
		{
			PostMessage(updatedlg->m_hWnd, WM_MSG_DOWNLOADER_DOWNSIZE, wParam, (LPARAM)pDownloadNotifyPara->lpNotifyData);
			AtlTrace(L"Downloaded size is : %d bytes\n", (int)pDownloadNotifyPara->lpNotifyData);
			break;
		}
	}
}

void Callback_Un7zip(int percent)
{
	PostMessage(updatedlg->m_hWnd, WM_MSG_UN7ZIP_PERCENT, percent, 0);
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

UINT Un7zipThread(LPVOID pParam)
{
	CUpdateDlg *update = (CUpdateDlg *)pParam;

	Sleep(100);

	MyTerminateProcess(_T("SubDownloader.exe"));
	MyTerminateProcess(_T("meditor2.exe"));
	MyTerminateProcess(_T("meditor.exe"));
	MyTerminateProcess(_T("massoc.exe"));
	MyTerminateProcess(_T("minfo.exe"));
	MyTerminateProcess(_T("mplayer.exe"));

	vector<wstring> ignore_list;
	if(!update->m_ini) {
		ignore_list.push_back(L"mplayer/kk.ini");
		ignore_list.push_back(L"mplayer/input.ini");
		ignore_list.push_back(L"mplayer/mplayer.ini");
	}
	
	wstring file = update->m_path;
	if (file.find_last_of(L"\\") != file.size() - 1)
		file += L"\\";
	file += update->m_filename;

	wstring path = update->m_ProPath.GetBuffer();
	update->m_ProPath.ReleaseBuffer();

	Decode7zFile(file.c_str(), path.c_str(), L"mplayer", ignore_list, L"", (Un7z_CallBack)Callback_Un7zip);

	PostMessage(update->m_hWnd, WM_MSG_UN7ZIP_FINISHED, 0, 0);
	
	return 0;
}


UINT CheckUpdate(LPVOID pParam)
{
	CUpdateDlg *update = (CUpdateDlg *)pParam;

	TiXmlDocument tixml;
	TiXmlNode* node = 0;
	TiXmlElement* root = 0;
	TiXmlElement* itemElement = 0;

	string xmlstr;
	string rsvn = "0", rdate = "0", bsvn = "0", bdate = "0";

	if(!get_url_string(L"http://mplayer-ww.sourceforge.net/html/version.xml", xmlstr))
		goto NOUPDATE;

	tixml.Parse(xmlstr.c_str());

	root = tixml.RootElement();

	if(!root)
		goto NOUPDATE;

	node = root->FirstChild("mplayer");

	if(!node)
		goto NOUPDATE;

	itemElement = node->FirstChildElement("rsvn");
	if(!itemElement)
		goto NOUPDATE;

	rsvn = itemElement->GetText();

	itemElement = node->FirstChildElement("rdate");
	if(!itemElement)
		goto NOUPDATE;

	rdate = itemElement->GetText();


	itemElement = node->FirstChildElement("bsvn");
	if(!itemElement)
		goto NOUPDATE;

	bsvn = itemElement->GetText();

	itemElement = node->FirstChildElement("bdate");
	if(!itemElement)
		goto NOUPDATE;

	bdate = itemElement->GetText();

NOUPDATE:
	int svn = update->nsvn;
	int date = update->ndate;
	int re_svn = atoi(rsvn.c_str());
	int be_svn = atoi(bsvn.c_str());
	int re_date = atoi(rdate.c_str());
	int be_date = atoi(bdate.c_str());

	CString str;

	CString url, filename;
	if((svn < re_svn && date <= re_date) || (svn <= re_svn && date < re_date)) {
		filename.Format(_T("mplayer_lite_r%d.7z"), re_svn);
		url.Format(_T("http://downloads.sourceforge.net/project/mplayer-ww/MPlayer_Release/Revision%%20%d/%s"), re_svn, filename);
		str.Format(_T("%s MPlayer SVN-r%d(%d)"), update->str_newversion, re_svn, re_date);
		update->GetDlgItem(IDC_BUTTON_UPDATE).ShowWindow(SW_SHOW);
	} else if((svn < be_svn && date <= be_date) || (svn <= be_svn && date < be_date)) {
		filename.Format(_T("mplayer-SVN-r%d.7z"), be_svn);
		url.Format(_T("http://downloads.sourceforge.net/project/mplayer-ww/MPlayer_Beta/%s"), filename);
		if(svn == re_svn && date == re_date)
			str.Format(_T("%s MPlayer SVN-r%d(%d)"), update->str_newversionrel, be_svn, be_date);
		else
			str.Format(_T("%s MPlayer SVN-r%d(%d)"), update->str_newversionbeta, be_svn, be_date);
		update->GetDlgItem(IDC_BUTTON_UPDATE).ShowWindow(SW_SHOW);
	} else if(svn == re_svn && date == re_date)
		str = update->str_relversion;
	else if(svn == be_svn && date == be_date)
		str = update->str_betaversion;
	else if((svn > be_svn && date >= be_date) || (svn >= be_svn && date > be_date)) 
		str = update->str_nversion;
	else {
		str = update->str_checkfail;
		update->GetDlgItem(IDC_BUTTON_CHECK).ShowWindow(SW_SHOW);
	}

	update->m_url = url.GetBuffer();
	url.ReleaseBuffer();
	update->m_filename = filename.GetBuffer();
	filename.ReleaseBuffer();

	update->m_info1 = str;

	return 0;

}

CUpdateDlg::CUpdateDlg(UINT DialogIDD)
{
	IDD = DialogIDD;

	updatedlg = this;
	g_pTaskbarList = NULL;
	s_uTBBC = WM_NULL;
	m_down_index = -1;

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);

	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_ProPath.Format(_T("%s"),szFilePath);

	if(DialogIDD == IDD_DIALOG_UPDATE_EN) {
		str_checkupdate = _T("Checking for updates, please wait...");
		str_downloading = _T("Downloading new version, please wait...");
		str_downloadok = _T("Download is complete, wait to update...");
		str_downloadfail = _T("Download fails, try download again...");
		str_newversion = _T("New version");
		str_newversionrel = _T("You are using the last release version\n\nNew beta version");
		str_newversionbeta = _T("You are using the beta version\n\nNew beta version");
		str_nversion = _T("You are using the latest version of MPlayer");
		str_relversion = _T("You are using the latest release version of MPlayer");
		str_betaversion = _T("You are using the latest beta version of MPlayer");
		str_checkfail = _T("Check for Updates failed!");
		str_un7ziping = _T("Extracting file, please wait...");
		str_updatefinished = _T("The update is complete\nYou had update to the latest version of MPlayer！");
		str_running = _T("MPlayer is running\n\nClick \"OK\" to close MPlayer and continue!");

	} else if(DialogIDD == IDD_DIALOG_UPDATE_TC) {
		str_checkupdate = _T("正在檢查更新，請稍等...");
		str_downloading = _T("正在下載新版本，請稍等...");
		str_downloadok = _T("下載完成，確認升級...");
		str_downloadfail = _T("下載失敗，請嘗試重新下載...");
		str_newversion = _T("發現新版本");
		str_newversionrel = _T("您正在使用最新發佈版 MPlayer\n\n發現新測試版");
		str_newversionbeta = _T("您正在使用測試版 MPlayer\n\n發現新測試版");
		str_nversion = _T("您正在使用最新版本的 MPlayer");
		str_relversion = _T("您正在使用最新發佈版的 MPlayer");
		str_betaversion = _T("您正在使用最新測試版的 MPlayer");
		str_checkfail = _T("檢查更新失敗！");
		str_un7ziping = _T("正在解壓檔，請稍等...");
		str_updatefinished = _T("更新完成，您已經升級到最新版 MPlayer！");
		str_running = _T("發現 MPlayer 正在運行\n\n單擊\"確定\"關閉 MPlayer 並繼續更新！");

	} else {
		str_checkupdate = _T("正在检查更新，请稍等...");
		str_downloading = _T("正在下载新版本，请稍等...");
		str_downloadok = _T("下载完成，确认升级...");
		str_downloadfail = _T("下载失败，请尝试重新下载...");
		str_newversion = _T("发现新版本");
		str_newversionrel = _T("您正在使用最新发布版 MPlayer\n\n发现新测试版");
		str_newversionbeta = _T("您正在使用测试版 MPlayer\n\n发现新测试版");
		str_nversion = _T("您正在使用最新版本的 MPlayer");
		str_relversion = _T("您正在使用最新发布版的 MPlayer");
		str_betaversion = _T("您正在使用最新测试版的 MPlayer");
		str_checkfail = _T("检查更新失败！");
		str_un7ziping = _T("正在解压文件，请稍等...");
		str_updatefinished = _T("更新完成，您已经升级到最新版 MPlayer！");
		str_running = _T("发现 MPlayer 正在运行\n\n单击\"确定\"关闭 MPlayer 并继续更新！");
	}


}

LRESULT CUpdateDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);

	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);
	
	m_progress.Attach(GetDlgItem(IDC_PROGRESS));

	m_progress.SetRange(0,100);
	m_progress.SetPos(0);

	m_progress.ShowWindow(SW_HIDE);

	DoDataExchange(false);

	s_uTBBC = RegisterWindowMessage(L"TaskbarButtonCreated");
	HINSTANCE user32 = GetModuleHandle(L"user32.dll");
	if(user32) ChangeWindowMessageFilterDLL = (ChangeWindowMessageFilterFunction)GetProcAddress(user32, "ChangeWindowMessageFilter");
	if(ChangeWindowMessageFilterDLL) ChangeWindowMessageFilterDLL(s_uTBBC, MSGFLT_ADD);

	SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	m_info1 = str_checkupdate;

	m_path = m_ProPath.GetBuffer();
	m_ProPath.ReleaseBuffer();
	m_path += L"update_mp";

	SetTimer(0, 100, 0);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckUpdate, this, 0, 0);
	return TRUE;
}

LRESULT CUpdateDlg::OnDownSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int oldsize = m_LastTimerSize;
	m_DownSize = lParam;
	DWORD timer = GetTickCount();
	if(oldsize > 0  && m_DownSize > 0 && m_LastTimer > 0) {
		if(timer > m_LastTimer + 300) {
			int bps = (m_DownSize - oldsize) / (timer - m_LastTimer);
			m_LastTimerSize = m_DownSize;
			if(bps > 0 && m_filesize >= m_DownSize) {
				int precent = (int)((double)m_DownSize / (double)m_filesize * 100);
				if(precent > 0 && precent <= 100) {
					m_progress.SetPos(precent);
					m_info2.Format(_T("%d%%,  %d KB / %d KB, %d KB/S"), precent, m_DownSize/1024, m_filesize/1024, bps);
					DoDataExchange();
				}
			} else if(bps > 0) {
				m_info2.Format(_T("%d KB, %d KB/S"), m_DownSize/1024, bps);
				DoDataExchange();
			}
			m_LastTimer = timer;
		}
	} else {
		m_LastTimer = timer;
	}
	if(m_LastTimerSize <= 0)
		m_LastTimerSize = m_DownSize;

	return 0;
}

LRESULT CUpdateDlg::OnUn7zPercent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam >= 0) {
		if(wParam > 100)
			wParam = 100;
		m_progress.SetPos(wParam);
		m_info2.Format(_T("%d%%"), wParam);
	}
	return 0;
}

LRESULT CUpdateDlg::OnUn7zFinished(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	KillTimer(0);
	m_progress.ShowWindow(SW_HIDE);
	m_info1 = str_updatefinished;
	m_info2 = _T("");
	if(g_pTaskbarList)
		g_pTaskbarList->SetProgressValue(m_hWnd, 100, 100);

	GetDlgItem(IDC_BUTTON_MPLAYER).ShowWindow(SW_SHOW);

	DoDataExchange();

//#ifndef _DEBUG
//	wstring filepath = m_path;
//	if (filepath.find_last_of(L"\\") != filepath.size() - 1)
//		filepath += L"\\";
//	filepath += m_filename;
//	DeleteFile(filepath.c_str());
//	DeleteFolder(m_path.c_str());
//#endif

	return 0;
}

LRESULT CUpdateDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_down_index >= 0)
		StopDownloader(m_down_index);
	return 0;
}

void CUpdateDlg::StrartDownload()
{
	m_down_index = StartDownloaderW(m_url.c_str(), m_path.c_str(), m_filename.c_str(),
		(FUNC_CallBack)Callback_Download, DOWNLOAD_WPARAM);
}

LRESULT CUpdateDlg::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	int p = m_progress.GetPos();
	if(p >= 100) {
		KillTimer(0);
		if (g_pTaskbarList)	{
			g_pTaskbarList->Release();
			g_pTaskbarList = NULL;
		}
		EndDialog(IDOK);
		return 0;
	}
	if(p > 0 && g_pTaskbarList)
		g_pTaskbarList->SetProgressValue(this->m_hWnd, p, 100);

	DoDataExchange();

	return 0;
}

LRESULT CUpdateDlg::OnTBBC(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(uMsg == WM_NULL)
		return 0;

	if (!g_pTaskbarList) {
		HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pTaskbarList));
		if (SUCCEEDED(hr))	{
			hr = g_pTaskbarList->HrInit();
			if (FAILED(hr))	{
				g_pTaskbarList->Release();
				g_pTaskbarList = NULL;
			}

			if(g_pTaskbarList) g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_NORMAL);
		}
	}

	return 0;
}

void CUpdateDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CUpdateDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CUpdateDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CUpdateDlg::OnFinished(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_down_index = -1;
	m_progress.ShowWindow(SW_HIDE);

	m_info2 = _T("");
	if(lParam) {
		KillTimer(0);
		GetDlgItem(IDC_BUTTON_UN7ZIP).ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_INI).ShowWindow(SW_SHOW);

		m_info1 = str_downloadok;
		DoDataExchange();
	} else {
		m_info1 = str_downloadfail;
		GetDlgItem(IDC_BUTTON_UPDATE).ShowWindow(SW_SHOW);
	}
	
	return 0;
}

LRESULT CUpdateDlg::OnBnClickedButtonUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	GetDlgItem(IDC_BUTTON_UPDATE).ShowWindow(SW_HIDE);

	m_info1.Format(_T("%s\n\n%s"), str_downloading, m_filename.c_str());

	m_filesize = 0;
	m_DownSize = 0;
	m_LastTimer = 0;
	m_LastTimerSize = 0;

	if(!FileExist(m_path.c_str()))
		CreateDirectory(m_path.c_str(), NULL);
	else if(!FileIsDirectory(m_path.c_str())) {
		DeleteFile(m_path.c_str());
		CreateDirectory(m_path.c_str(), NULL);
	}

	StrartDownload();

	m_progress.ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CUpdateDlg::OnBnClickedButtonUn7zip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	HWND hWnd = FindWindow(_T("MPlayer WW for Windows"), NULL);
	if(::IsWindow(hWnd)) {
		MessageBox(str_running);
		::SendMessage(hWnd, WM_CLOSE, 0, 0);
	}

	DoDataExchange(TRUE);
	GetDlgItem(IDC_BUTTON_UN7ZIP).ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_INI).ShowWindow(SW_HIDE);
	m_progress.ShowWindow(SW_SHOW);
	m_info1 = str_un7ziping;
	DoDataExchange();

	SetTimer(0, 100, 0);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Un7zipThread, this, 0, 0);
	return 0;
}

LRESULT CUpdateDlg::OnBnClickedButtonCheck(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	GetDlgItem(IDC_BUTTON_CHECK).ShowWindow(SW_HIDE);
	m_info1 = str_checkupdate;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckUpdate, this, 0, 0);

	return 0;
}


LRESULT CUpdateDlg::OnBnClickedButtonMplayer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_ProPath += _T("mplayer.exe");

	ShellExecute(NULL, _T("open"), m_ProPath, NULL, NULL, SW_SHOW);

	PostMessage(WM_CLOSE, 0, 0);

	return 0;
}
