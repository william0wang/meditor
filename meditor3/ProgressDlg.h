// ProgressDlg.h : interface of the CProgressDlg class
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(VFC_PROGRESSDLG_H__26A6A837_F94F_4183_B98C_27E94835987F__INCLUDED_)
#define VFC_PROGRESSDLG_H__26A6A837_F94F_4183_B98C_27E94835987F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CProgressDlg : public CDialogImpl<CProgressDlg>,
	public CWinDataExchange<CProgressDlg>
{
public:
	UINT	s_uTBBC;
	ITaskbarList3 *g_pTaskbarList;
	CProgressBarCtrl	m_progress;

	CProgressDlg();

	enum { IDD = IDD_DIALOG_P };

	BEGIN_DDX_MAP(CProgressDlg)
		DDX_CONTROL_HANDLE(IDC_PROGRESS, m_progress)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CProgressDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(s_uTBBC, OnTBBC)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()
	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTBBC(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// VisualFC AppWizard will insert additional declarations immediately before the previous line.
#endif // !defined(VFC_PROGRESSDLG_H__26A6A837_F94F_4183_B98C_27E94835987F__INCLUDED_)
