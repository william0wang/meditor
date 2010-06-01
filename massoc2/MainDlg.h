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
	int m_recommand;

	void Set(CString ext, CString info, int type, int icon, int recommand = 0)
	{
		m_ext = ext;
		m_info = info;
		m_type = type;
		m_icon = icon;
		m_recommand = recommand;
	}
};

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler, public CWinDataExchange<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	vector<AssocItem> m_AssocList;

	int	m_appLang;
	BOOL m_rightOpen;
	BOOL m_rightPlay;
	BOOL m_mpc;
	bool m_is_vista;
	bool m_dll_getted;
	bool m_have_icons;
	CString inifile;
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
	CString m_str_assos_ct;
	CString m_str_assos_ex;
	CString m_str_assos_mv;
	CString m_str_assos_mov;
	CString m_str_assos_inf;
	CString m_str_assos_ico;
	CString m_str_assos_typ;
	CString m_str_assos_avi;
	CString m_str_assos_avs;
	CString m_str_assos_list;

	CString m_str_ui_ok;
	CString m_str_ui_add;
	CString m_str_ui_del;
	CString m_str_ui_all;
	CString m_str_ui_none;
	CString m_str_ui_rem;
	CString m_str_ui_icons;
	CString m_str_ui_right;
	CString m_str_ui_open;
	CString m_str_ui_play;

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

	CMainDlg(HINSTANCE dll, int appLang = 0);

	BEGIN_DDX_MAP(CMainDlg)
		DDX_CHECK(IDC_CHECK_RMENU, m_rightOpen)
		DDX_CHECK(IDC_CHECK_RMENU2, m_rightPlay)
	END_DDX_MAP();

	void LoadAssocINI();
	void SaveAssocIni();

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
		COMMAND_HANDLER(IDC_ADD, BN_CLICKED, OnBnClickedAdd)
		COMMAND_HANDLER(IDC_ALL, BN_CLICKED, OnBnClickedAll)
		COMMAND_HANDLER(IDC_RECOMMAND, BN_CLICKED, OnBnClickedRecommand)
		COMMAND_HANDLER(IDC_NONE, BN_CLICKED, OnBnClickedNone)
		COMMAND_HANDLER(IDC_DEL, BN_CLICKED, OnBnClickedDel)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);


	void CloseDialog(int nVal);
	LRESULT OnBnClickedAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedRecommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedNone(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnBnClickedDel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};
