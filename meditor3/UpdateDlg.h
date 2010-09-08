// UpdateDlg.h : interface of the CUpdateDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
using namespace std;

#define WM_MSG_DOWNLOADER_DOWNSIZE			WM_USER + 2120
#define WM_MSG_DOWNLOADER_FINISHED			WM_USER + 2121
#define WM_MSG_UN7ZIP_PERCENT				WM_USER + 2122
#define WM_MSG_UN7ZIP_FINISHED				WM_USER + 2123

class CUpdateDlg : public CDialogImpl<CUpdateDlg>,
	public CWinDataExchange<CUpdateDlg>
{
public:
	UINT					IDD;

	BOOL					m_ini;
	CString					m_info2;
	CString					m_info1;
	UINT					s_uTBBC;
	ITaskbarList3			*g_pTaskbarList;
	CProgressBarCtrl		m_progress;
	CString					m_ProPath;

	CString					str_checkupdate;
	CString					str_downloading;
	CString					str_downloadok;
	CString					str_downloadfail;
	CString					str_newversion;
	CString					str_newversionbeta;
	CString					str_relversion;
	CString					str_betaversion;
	CString					str_checkfail;
	CString					str_nversion;
	CString					str_un7ziping;
	CString					str_updatefinished;
	CString					str_running;

	int						m_filesize;
	int						m_down_index;
	int						m_DownSize;
	int						m_LastTimerSize;
	DWORD					m_LastTimer;

	wstring					m_url;
	wstring					m_filename;
	wstring					m_path;

	int nsvn;
	int ndate;

	CUpdateDlg(UINT DialogIDD);

	void CloseDialog(int nVal);

	void StrartDownload();
	
	BEGIN_DDX_MAP(CUpdateDlg)
		DDX_CHECK(IDC_CHECK_INI, m_ini)
		DDX_TEXT(IDC_STATIC_INFO1, m_info1)
		DDX_TEXT(IDC_STATIC_INFO2, m_info2)
		DDX_CONTROL_HANDLE(IDC_PROGRESS, m_progress)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CUpdateDlg)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(s_uTBBC, OnTBBC)
		MESSAGE_HANDLER(WM_MSG_DOWNLOADER_DOWNSIZE, OnDownSize)
		MESSAGE_HANDLER(WM_MSG_DOWNLOADER_FINISHED, OnFinished)
		MESSAGE_HANDLER(WM_MSG_UN7ZIP_PERCENT, OnUn7zPercent)
		MESSAGE_HANDLER(WM_MSG_UN7ZIP_FINISHED, OnUn7zFinished)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON_UPDATE, BN_CLICKED, OnBnClickedButtonUpdate)
		COMMAND_HANDLER(IDC_BUTTON_UN7ZIP, BN_CLICKED, OnBnClickedButtonUn7zip)
		COMMAND_HANDLER(IDC_BUTTON_CHECK, BN_CLICKED, OnBnClickedButtonCheck)
		COMMAND_HANDLER(IDC_BUTTON_MPLAYER, BN_CLICKED, OnBnClickedButtonMplayer)
	END_MSG_MAP()
	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTBBC(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonUn7zip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonCheck(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnUn7zPercent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUn7zFinished(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFinished(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDownSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBnClickedButtonMplayer(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
