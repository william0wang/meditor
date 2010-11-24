// GuiDlg.h : interface of the CGuiDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ACombo.h"

class CGuiDlg : public CDialogImpl<CGuiDlg>, public CUpdateUI<CGuiDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CGuiDlg>
{
public:
	UINT		IDD;
	AdvCombo	aComboLang;
	AdvCombo	aComboSkin;
	AdvCombo	aComboAlpha;
	AdvCombo	aComboTop;
	AdvCombo	aComboHide;
	AdvCombo	aComboTray;
	AdvCombo	aComboColorkey;
	BOOL		m_ctlskin;
	BOOL		m_show;
	BOOL		m_bskin;
	BOOL		m_alpha_control;
	BOOL		m_menu;
	BOOL		m_rightmenu;
	BOOL		m_filename;

	CGuiDlg(UINT IDD_DLG);

	void InitCheckBox();

	void SaveConfig();

	BEGIN_DDX_MAP(CGuiDlg)
		DDX_CHECK(IDC_CHECK_SHOW, m_show)
		DDX_CHECK(IDC_CHECK_BSKIN, m_bskin)
		DDX_CHECK(IDC_CHECK_CTLSKIN, m_ctlskin)
		DDX_CHECK(IDC_CHECK_CTLALPHA, m_alpha_control)
		DDX_CHECK(IDC_CHECK_TITLE, m_filename)
		DDX_CHECK(IDC_CHECK_MENU, m_menu)
		DDX_CHECK(IDC_CHECK_RIGHTMENU, m_rightmenu)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CGuiDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CGuiDlg)
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
