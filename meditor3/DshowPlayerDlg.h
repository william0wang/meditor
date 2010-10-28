// DShowPlayerDlg.h : interface of the CDShowPlayerDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CDXGraph.h"
#include "ListCtrl/ListCtrl.h"

#define CMD_KEYDOWN     WM_USER + 1111

class CDShowPlayerDlg : public CDialogImpl<CDShowPlayerDlg>, public CUpdateUI<CDShowPlayerDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CDShowPlayerDlg>
{
public:
	enum { IDD = IDD_DIALOG_DSHOW };

	CString m_program_dir;

	CDShowPlayerDlg();

	UINT	s_uTBBC;
	ITaskbarList3 *g_pTaskbarList;
	HIMAGELIST hImglist;
	HICON m_hIcon;
	bool m_fs;
	bool m_fs_init;
	bool m_ontop;
	bool m_keydowning;
	int save_x;
	int save_y;
	int save_width;
	int save_height;
	int m_scr_width;
	int m_scr_height;
	int m_applang;
	bool m_changing;
	bool TimerRuning;
	bool m_showctrl;
	bool m_inited;

	CInputList m_inputs;
	CMenu m_popmenu;
	CString m_filename;
	CTrackBarCtrl	m_SliderGraph;
	CDXGraph* m_FilterGraph;     // Filter Graph·â×°

	int GetKeyNumber(CString Key);
	bool GetKeyCommand(LONG_PTR KeyNumber,CString &cmd, CString &value);
	void LoadConfig();
	bool AnalyseLine(CString line, CString &key, CString &cmd, CString &val);
	HRESULT UpdateThumbnailToolbar(HWND hWnd);
	void IninFileName(CString filename);

	void CreateGraph(const CString& strSourceFile);
	void DestroyGraph(void);
	void OnFullScreen(void);
	void Seek(double len);

	BEGIN_DDX_MAP(CDShowPlayerDlg)
		DDX_CONTROL_HANDLE(IDC_SLIDER_CTRL, m_SliderGraph)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CDShowPlayerDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CDShowPlayerDlg)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(CMD_KEYDOWN, OnCmdKeyDown)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(ID_DS_OPEN, OnDShowOpen)
		COMMAND_ID_HANDLER(IDM_DSPLAY, OnDSPlay)
		COMMAND_ID_HANDLER(IDM_DSSTOP, OnDSStop)
		COMMAND_ID_HANDLER(IDM_DSFULLSCREEN, OnDSFS)
		NOTIFY_HANDLER(IDC_SLIDER_CTRL, NM_RELEASEDCAPTURE, OnNmReleasedcaptureSlider)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	void CloseDialog(int nVal);

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDShowOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDSPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDSStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDSFS(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCmdKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNmReleasedcaptureSlider(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	void __stdcall OnReadyStateChange(long newState);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
