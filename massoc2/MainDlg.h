// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ListCtrl.h"
#include "AdvCombo.h"

#include <vector>
using namespace std;

enum ASSOC_TYPE
{
	ASSOC_TYPE_VIDEO = 0,
	ASSOC_TYPE_VIDEO2,
	ASSOC_TYPE_AUDIO,
	ASSOC_TYPE_FLASH,
	ASSOC_TYPE_PLAYLIST,
	ASSOC_TYPE_DSHOW,
	ASSOC_TYPE_DSHOWINNER,
};

class AssocItem
{
public:
	CString m_ext;
	CString m_info;
	int m_type;
	int m_icon;

	void Set(CString ext, CString info, int type, int icon)
	{
		m_ext = ext;
		m_info = info;
		m_type = type;
		m_icon = icon;
	}
};

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	vector<AssocItem> m_AssocList;

	BOOL m_rightmenu;
	BOOL m_rightmenu2;
	BOOL m_mpc;
	bool m_is_vista;
	bool m_dll_getted;
	bool m_have_icons;
	CString m_editor_exe;
	CString m_player_exe;
	CString m_mpc_exe;
	CString m_icons_org;
	CString m_icons_dll;
	CString	m_icons_str;
	CString m_program_dir;
	CString m_str_player_use;
	CString m_str_player_samedir;
	CString m_str_assos_input;
	CString m_str_assos_del;
	CString m_str_assos_pl;
	CString m_str_assos_op;
	CString m_str_assos_as;

	CString m_type_video;
	CString m_type_video2;
	CString m_type_audio;
	CString m_type_list;
	CString m_type_flash;
	CString m_type_dshow;
	CString m_type_inner;


	CAdvComboBox	m_icon;
	CFont m_fntCustomFont1;
	CFont m_fntCustomFont2;

	CListCtrl m_wndListCtrl;

	CMainDlg();

	BEGIN_DDX_MAP(CMainDlg)
		DDX_CHECK(IDC_CHECK_RMENU, m_rightmenu)
		DDX_CHECK(IDC_CHECK_RMENU2, m_rightmenu2)
	END_DDX_MAP();
	

	void LoadAssocINI();

	void InitBasicList();

	void ApplyChange();
	void ApplyDefault();

	bool IsAssoced(CString ext);
	bool AssocExtension(CString ext, CString info, CString icons, int type, bool bAssoc = true);
	bool AssocExtDefault(CString ext, CString info, CString icons, int type, bool bAssoc = true);
	void AssocDefaults();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);


	void CloseDialog(int nVal);
};
