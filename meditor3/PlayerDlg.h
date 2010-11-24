// PlayerDlg.h : interface of the CPlayerDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ACombo.h"

class CPlayerDlg : public CDialogImpl<CPlayerDlg>, public CUpdateUI<CPlayerDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CPlayerDlg>
{
public:
	UINT IDD;
	AdvCombo	aComboGui;
	BOOL		m_seekrt;

	CPlayerDlg(UINT IDD_DLG);

	void InitCheckBox();
	void SaveConfig();

	BEGIN_DDX_MAP(CPlayerDlg)
		DDX_CHECK(IDC_CHECK_RTSEEK, m_seekrt)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CPlayerDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CPlayerDlg)
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
