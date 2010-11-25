// ExtraDlg.h : interface of the CExtraDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CExtraDlg : public CDialogImpl<CExtraDlg>, public CUpdateUI<CExtraDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CExtraDlg>
{
public:
	UINT IDD;
	CString	m_mplayer_ini;

	void SaveConfig();

	CExtraDlg(UINT IDD_DLG);

	BEGIN_DDX_MAP(CExtraDlg)
		DDX_TEXT(IDC_EDIT_EXTRA, m_mplayer_ini)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CExtraDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CExtraDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

};
