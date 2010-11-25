// AssocDlg.h : interface of the CAssocDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CAssocDlg : public CDialogImpl<CAssocDlg>, public CUpdateUI<CAssocDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CAssocDlg>
{
public:
	UINT IDD;

	BOOL	m_update;
	BOOL	m_is_vista;
	CString m_assoc_exe;
	CString m_player_exe;
	CString m_program_dir;

	bool AssosTypeDef(CString type, CString info, CString icons, bool isplaylist = false);

	CAssocDlg(UINT IDD_DLG);

	BEGIN_DDX_MAP(CAssocDlg)
		DDX_CHECK(IDC_CHECK_UPDATE, m_update)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CAssocDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CAssocDlg)
		COMMAND_HANDLER(IDC_ASSOC, BN_CLICKED, OnBnClickedAssoc)
		COMMAND_HANDLER(IDC_ASSOC_DEF, BN_CLICKED, OnBnClickedAssocDef)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBnClickedAssoc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedAssocDef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

};
