// PlayerDlg.h : interface of the CPlayerDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ACombo.h"

class CPlayerDlg : public CDialogImpl<CPlayerDlg>, public CUpdateUI<CPlayerDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CPlayerDlg>
{
public:
	CString		m_def;
	CString		m_png;
	UINT		IDD;
	AdvCombo	aComboPriority;
	AdvCombo	aComboASync;
	AdvCombo	aComboAPlay;
	AdvCombo	aComboCache;
	AdvCombo	aComboMonitor;
	AdvCombo	aComboLoop;
	AdvCombo	aComboLog;
	CListCtrl	ListTime;
	BOOL		m_seekrt;
	BOOL		m_control;
	BOOL		m_framedrop;
	BOOL		m_fullscreen;
	BOOL		m_guithread;
	BOOL		m_oneplayer;
	BOOL		m_quit;
	BOOL		m_url;
	BOOL		m_htimer;
	BOOL		m_gui_high;
	BOOL		m_idx;
	BOOL		m_reload;
	BOOL		m_no_dvdnav;
	BOOL		m_quiet;
	BOOL		m_double;
	BOOL		m_conf;

	CPlayerDlg(UINT IDD_DLG);

	void InitCheckBox();
	void SaveConfig();

	long ListStringtoTime(CString time);
	CString ListTimetoString(long time);

	BEGIN_DDX_MAP(CPlayerDlg)
		DDX_TEXT(IDC_EDIT_PNG, m_png)
		DDX_TEXT(IDC_EDIT_DEF, m_def)
		DDX_CHECK(IDC_CHECK_RTSEEK, m_seekrt)
		DDX_CHECK(IDC_CHECK_FULLSCREEN, m_fullscreen)
		DDX_CHECK(IDC_CHECK_GUI, m_gui_high)
		DDX_CHECK(IDC_CHECK_ONLYONE, m_oneplayer)
		DDX_CHECK(IDC_CHECK_GUITHREAD, m_guithread)
		DDX_CHECK(IDC_CHECK_URL, m_url)
		DDX_CHECK(IDC_CHECK_QUIT, m_quit)
		DDX_CHECK(IDC_CHECK_QUIET, m_quiet)
		DDX_CHECK(IDC_CHECK_DOUBLE, m_double)
		DDX_CHECK(IDC_CHECK_CONF, m_conf)
		DDX_CHECK(IDC_CHECK_HTIMER, m_htimer)
		DDX_CHECK(IDC_CHECK_DVDNAV, m_no_dvdnav)
		DDX_CHECK(IDC_CHECK_RELOAD, m_reload)
		DDX_CHECK(IDC_CHECK_FRAMEDROP, m_framedrop)
		DDX_CHECK(IDC_CHECK_REINDEX, m_idx)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CPlayerDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CPlayerDlg)
		COMMAND_HANDLER(IDC_BUTTON_PNG, BN_CLICKED, OnBnClickedButtonPng)
		COMMAND_HANDLER(IDC_BUTTON_DEF, BN_CLICKED, OnBnClickedButtonDef)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonPng(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButtonDef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

};
