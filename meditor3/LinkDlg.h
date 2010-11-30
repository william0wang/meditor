// LinkDlg.h : interface of the CLinkDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CLinkDlg : public CDialogImpl<CLinkDlg>, public CUpdateUI<CLinkDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CLinkDlg>
{
public:
	enum { IDD = IDD_DIALOG_LINK };

	BOOL m_mplayer_desk;
	BOOL m_meditor_desk;
	BOOL m_mplayer_start;
	BOOL m_meditor_start;
	BOOL m_tools_start;
	BOOL m_flash_start;
	BOOL m_dshow_start;
	BOOL m_mpc_start;

	CLinkDlg();

	BEGIN_DDX_MAP(CLinkDlg)
		DDX_CHECK(IDC_CHECK1, m_mplayer_desk)
		DDX_CHECK(IDC_CHECK2, m_meditor_desk)
		DDX_CHECK(IDC_CHECK3, m_mplayer_start)
		DDX_CHECK(IDC_CHECK4, m_meditor_start)
		DDX_CHECK(IDC_CHECK5, m_tools_start)
		DDX_CHECK(IDC_CHECK6, m_flash_start)
		DDX_CHECK(IDC_CHECK7, m_dshow_start)
		DDX_CHECK(IDC_CHECK8, m_mpc_start)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CLinkDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CLinkDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	void CloseDialog(int nVal);
};
