// PreviewDlg.h : interface of the CPreviewDlg class
//
#pragma once
#include "AdvCombo.h"
#include "ProgressDlg.h"

class CPreviewDlg : public CDialogImpl<CPreviewDlg>, public CUpdateUI<CPreviewDlg>,
	public CMessageFilter, public CIdleHandler, public CWinDataExchange<CPreviewDlg>
{
public:
	UINT IDD;

	CPreviewDlg(UINT DLG_IDD = IDD_PREVIEW_DIALOG);

	BOOL	m_have_preview;
	CString m_player_exe;
	CString m_program_dir;

	static BOOL CALLBACK EnumFamScreenCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID pThis);

	int		m_percent;
	int		m_parti;
	int		m_partj;
	int		m_width;
	int		m_time;
	long	ltime;
	BOOL	m_show;
	CString m_filename;
	CString m_savename;
	CString m_font;

	CProgressDlg	m_pdlg;
	CUpDownCtrl		m_spinr;
	CUpDownCtrl		m_spinv;
	CUpDownCtrl		m_spinw;
	CAdvComboBox	m_font_c;

	void GetPreview(int index, int resolution, int time);

	void GenerateThumbnails(CString ThumbName);

	BOOL EnumerateFonts();
	void AddFont(CString strName);

	BEGIN_UPDATE_UI_MAP(CPreviewDlg)
	END_UPDATE_UI_MAP()

	BEGIN_DDX_MAP(CPreviewDlg)
		DDX_CONTROL_HANDLE(IDC_SPIN_R, m_spinr)
		DDX_CONTROL_HANDLE(IDC_SPIN_V, m_spinv)
		DDX_CONTROL_HANDLE(IDC_SPIN_W, m_spinw)
		DDX_CONTROL_HANDLE(IDC_ACOMBO_FONT, m_font_c)
		DDX_TEXT(IDC_ACOMBO_FONT, m_font)
		DDX_TEXT(IDC_EDIT_FILE, m_filename)
		DDX_TEXT(IDC_EDIT_SAVEFILE, m_savename)
		DDX_INT(IDC_EDIT_R, m_parti);
		DDX_INT(IDC_EDIT_V, m_partj);
		DDX_INT(IDC_EDIT_W, m_width);
		DDX_INT(IDC_EDIT_T, m_time);
		DDX_CHECK(IDC_CHECK_SHOW, m_show)
	END_DDX_MAP()

	BEGIN_MSG_MAP(CPreviewDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON_B, BN_CLICKED, OnBnClickedButtonB)
	END_MSG_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	void CloseDialog(int nVal);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonB(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
