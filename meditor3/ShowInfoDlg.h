// ShowInfoDlg.h : interface of the CShowInfoDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

enum
{
	type_reload = 0,
	type_coreavc,
	type_ass_expand,
	type_boost,
	type_realtime,
};

class CShowInfoDlg : public CDialogImpl<CShowInfoDlg>, public CUpdateUI<CShowInfoDlg>,
	public CMessageFilter, public CIdleHandler, public CWinDataExchange<CShowInfoDlg>
{
public:
	enum { IDD = IDD_DIALOG_INFO };
	BOOL m_info_nshow;
	CString m_info;

	int m_type;
	BYTE unshow;
	void set_bit(BYTE &date , UINT bit);
	void clear_bit(BYTE &date , UINT bit);
	bool get_bit(BYTE date , UINT bit);

	bool IsShow(int type);
	void ResetInfo();

	BEGIN_DDX_MAP(CShowInfoDlg)
		DDX_CHECK(IDC_CHECK_INFO, m_info_nshow)
		DDX_TEXT(IDC_STATIC_INFO, m_info);
	END_DDX_MAP();

	BEGIN_UPDATE_UI_MAP(CShowInfoDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CShowInfoDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnBnClickedOk)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

void ShowInfoDlg(int type);