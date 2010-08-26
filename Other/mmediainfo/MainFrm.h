// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define FileExist(lpszFileName) (::GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)

class CMainFrame : public CFrameWindowImpl<CMainFrame>, public CUpdateUI<CMainFrame>,
		public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CTabView m_view;
	CString m_filename;
	CString m_htmlpage;
	CString minfodll;
	CString htmldll;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return m_view.PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_RANGE_HANDLER(ID_WINDOW_TABFIRST, ID_WINDOW_TABLAST, OnWindowActivate)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

	void SetFileName(LPCTSTR filename)
	{
		m_filename = filename;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CRect rc;
		int width = 620;
		int height = 560;
		int x = GetSystemMetrics(SM_CXFULLSCREEN);
		int y = GetSystemMetrics(SM_CYFULLSCREEN);

		x = (x-width)/2;
		y = (y-height)/2;

		rc.SetRect(0, 0, width, height);
		::SetWindowPos(m_hWnd, NULL, x, y, width, height, 0);

		m_hWndClient = m_view.Create(m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		CString infofile;
		TCHAR szFilePath[MAX_PATH + 1];
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
		minfodll.Format(_T("%smediainfo.dll"), szFilePath);
		htmldll.Format(_T("%shtmlayout.dll"), szFilePath);
		m_htmlpage.Format(_T("%smedia_info.html"), szFilePath);
		infofile.Format(_T("%smedia_info.txt"), szFilePath);

		CMediaInfoDlg *dlg = NULL;
		CMediaInfoDlg *dlg2 = NULL;
		if(FileExist(infofile)) {
			dlg = new CMediaInfoDlg;
			dlg->Create(m_view, rc);
			dlg->loadInfoFile(infofile);
			m_view.AddPage(dlg->m_hWnd, _T("MPlayer Info"));
		}

		if(FileExist(minfodll)) {
			dlg2 = new CMediaInfoDlg;
			dlg2->Create(m_view, rc);
			dlg2->GetMediaInfo(m_filename);
			m_view.AddPage(dlg2->m_hWnd, _T("MediaInfo"));
		}

		if(!dlg && !dlg2) {
			PostMessage(WM_CLOSE, 0, 0);
			return 0;
		}

		SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		m_view.SetActivePage(0);
		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		DeleteFile(m_htmlpage);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnWindowActivate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int nPage = wID - ID_WINDOW_TABFIRST;
		m_view.SetActivePage(nPage);

		return 0;
	}
	
};
