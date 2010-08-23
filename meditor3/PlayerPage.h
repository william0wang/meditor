// PlayerPage.h : interface of the CPlayerPage class
//
/////////////////////////////////////////////////////////////////////////////
#if !defined(VFC_PLAYERPAGE_H__BCE76DD0_8CA2_4de8_B67D_A1D1CDE146B4__INCLUDED_)
#define VFC_PLAYERPAGE_H__BCE76DD0_8CA2_4de8_B67D_A1D1CDE146B4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CPlayerPage : public CDialogImpl<CPlayerPage>,
	public CUpdateUI<CPlayerPage>,
	public CWinDataExchange<CPlayerPage>,
	public CMessageFilter
{
public:
	enum { IDD = IDD_DIALOG_PLAYER };

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_UPDATE_UI_MAP(CPlayerPage)
	END_UPDATE_UI_MAP()

	BEGIN_DDX_MAP(CPlayerPage)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CPlayerPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		//COMMAND_ID_HANDLER(IDOK, OnOK)
		//COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		CHAIN_MSG_MAP(CUpdateUI<CPlayerPage>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// VisualFC AppWizard will insert additional declarations immediately before the previous line.
#endif // !defined(VFC_PLAYERPAGE_H__BCE76DD0_8CA2_4de8_B67D_A1D1CDE146B4__INCLUDED_)
