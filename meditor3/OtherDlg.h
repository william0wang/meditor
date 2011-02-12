// OtherDlg.h : interface of the COtherDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ACombo.h"

class COtherDlg : public CDialogImpl<COtherDlg>, public CUpdateUI<COtherDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<COtherDlg>
{
public:
	UINT IDD;
	AdvCombo	aComboGui;
	CString		m_assoc_exe;
	CString		m_mpc_exe;
	CString		m_program_dir;
	BOOL		m_screensaver;
	BOOL		m_info_html;
	BOOL		m_mpc;

	COtherDlg(UINT IDD_DLG);

	void InitCheckBox();
	void SaveConfig();

	BEGIN_DDX_MAP(COtherDlg)
		DDX_CHECK(IDC_CHECK_SCREENSAVER, m_screensaver)
		DDX_CHECK(IDC_CHECK_INFO, m_info_html)
		DDX_CHECK(IDC_CHECK_MPC, m_mpc)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(COtherDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(COtherDlg)
		COMMAND_HANDLER(IDC_BUTTON_FLASH, BN_CLICKED, OnBnClickedButtonFlash)
		COMMAND_HANDLER(IDC_BUTTON_MEDIA, BN_CLICKED, OnBnClickedButtonMedia)
		COMMAND_HANDLER(IDC_BUTTON_LINK, BN_CLICKED, OnBnClickedButtonLink)
		COMMAND_HANDLER(IDC_RADIO_MPLAYER, BN_CLICKED, OnBnClickedRadioMplayer)
		COMMAND_HANDLER(IDC_RADIO_MEDITOR, BN_CLICKED, OnBnClickedRadioMeditor)
		COMMAND_HANDLER(IDC_BUTTON_ONLINE, BN_CLICKED, OnBnClickedReg)
		COMMAND_HANDLER(IDC_BUTTON_DONLINE, BN_CLICKED, OnBnClickedDReg)
		COMMAND_HANDLER(IDC_BUTTON_ME2, BN_CLICKED, OnBnClickedME2)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnBnClickedME2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedReg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedDReg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedRadioMplayer(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedRadioMeditor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButtonFlash(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButtonMedia(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedButtonLink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};
