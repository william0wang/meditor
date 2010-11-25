// FlashPlayerDlg.h : interface of the CFlashPlayerDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ListCtrl/ListCtrl.h"

#define CMD_KEYDOWN     WM_USER + 1111

class CFlashPlayerDlg : public CAxDialogImpl<CFlashPlayerDlg>, public CUpdateUI<CFlashPlayerDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CFlashPlayerDlg>,
		public IDispEventImpl<IDC_SHOCKWAVEFLASH,CFlashPlayerDlg>
{
public:
	CTrackBarCtrl	m_control;
	enum { IDD = IDD_DIALOG_FLASH };

	CString m_program_dir;

	CFlashPlayerDlg();

	UINT	s_uTBBC;
	ITaskbarList3 *g_pTaskbarList;
	HIMAGELIST hImglist;
	HICON m_hIcon;
	INT_PTR m_current;
	long m_fnumber;
	bool m_inited;
	bool m_keydowning;
	bool m_ontop;
	bool m_fs_init;
	bool m_fs;
	bool m_rmenu;
	bool m_showctrl;
	bool m_changing;
	int save_x;
	int save_y;
	int save_width;
	int save_height;
	int m_scr_width;
	int m_scr_height;
	int now_state;
	int m_applang;
	//CMPlayList *m_list;
	CListArray<CString> m_playlist;
	CInputList m_inputs;
	CMenu m_popmenu;
	void PlayFile(bool init = false);
	void FullScreen(bool init = false);
	int GetKeyNumber(CString Key);
	bool GetKeyCommand(LONG_PTR KeyNumber,CString &cmd, CString &value);
	void LoadConfig();
	bool AnalyseLine(CString line, CString &key, CString &cmd, CString &val);
	HRESULT UpdateThumbnailToolbar(HWND hWnd);
	bool IsFlashPlaying();
	void IninFileName(CString filename);
	void OnPlay();
	void OnStop();
	void OnNext();
	void OnPre();

	BEGIN_DDX_MAP(CFlashPlayerDlg)
		DDX_CONTROL_HANDLE(IDC_SLIDER_CONTROL, m_control)
	END_DDX_MAP();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CFlashPlayerDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CFlashPlayerDlg)
		MESSAGE_HANDLER(WM_DROPFILES, OnDropFiles)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(CMD_KEYDOWN, OnCmdKeyDown)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(ID_FLASH_OPEN, OnFlashOpen)
		COMMAND_ID_HANDLER(ID_MPLAY, OnPlayM)
		COMMAND_ID_HANDLER(ID_MSTOP, OnStopM)
		COMMAND_ID_HANDLER(ID_MNEXT, OnNextM)
		COMMAND_ID_HANDLER(ID_MPRE, OnPreM)
		COMMAND_ID_HANDLER(IDM_RMENU, OnRmenu)
		NOTIFY_HANDLER(IDC_SLIDER_CONTROL, NM_RELEASEDCAPTURE, OnNmReleasedcaptureSlider)
	END_MSG_MAP()

	BEGIN_SINK_MAP(CFlashPlayerDlg)
		SINK_ENTRY(IDC_SHOCKWAVEFLASH, -609, OnReadyStateChange)
	END_SINK_MAP()
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
	LRESULT OnFlashOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPlayM(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStopM(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnNextM(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPreM(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnRmenu(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCmdKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNmReleasedcaptureSlider(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	void __stdcall OnReadyStateChange(long newState);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
