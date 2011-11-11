// MOtherPage.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MOtherPage.h"
#include "MConfig.h"
#include "reg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMOtherPage dialog

CMOtherPage::CMOtherPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMOtherPage::IDD, pParent)
	, m_mpc(FALSE)
	, m_screensaver(FALSE)
	, m_info_html(FALSE)
	, m_last_page(FALSE)
{
	//{{AFX_DATA_INIT(CMOtherPage)
	m_other = _T("");
	m_video = _T("");
	m_audio = _T("");
	m_one = FALSE;
	//}}AFX_DATA_INIT
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	m_program.Format(_T("%s"),szFilePath);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
}


void CMOtherPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OTHER, m_other_c);
	DDX_Text(pDX, IDC_EDIT_OTHER, m_other);
	DDX_Check(pDX, IDC_CHECK_PAGE, m_last_page);
	DDX_Text(pDX, IDC_EDIT_VIDEO, m_video);
	DDX_Text(pDX, IDC_EDIT_AUDIO, m_audio);
	DDX_Check(pDX, IDC_CHECK_ONE, m_one);
	DDX_Check(pDX, IDC_CHECK_SCREENSAVER, m_screensaver);
	DDX_Check(pDX, IDC_CHECK_INFO, m_info_html);
}


BEGIN_MESSAGE_MAP(CMOtherPage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AUDIO, OnButtonAudio)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN, OnButtonClean)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO, OnButtonVideo)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMOtherPage message handlers

BOOL CMOtherPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_cfg)
	{
		m_other = m_cfg->GetValue_Other();
		int value_i;
		bool value_b;
		m_cfg->GetValue_Integer(_T("use_windowblinds"),value_i,true);
		m_cfg->GetValue_Integer(_T("using_aero"),value_i,true);
		m_cfg->GetValue_Integer(_T("using_theme"),value_i,true);
		m_cfg->GetValue_Integer(_T("is_vista"),value_i,true);
		if(m_cfg->GetValue_Integer(_T("meditor_last_page"),value_i,true))
		{
			if(value_i > -1 && value_i < 7)
				m_last_page = TRUE;
			else
				m_last_page = FALSE;
		}
		if(m_cfg->GetValue_Boolean(_T("meditor_one_editor"),value_b,true))
		{
			if(value_b)
				m_one = TRUE;
			else
				m_one = FALSE;
		}
		if(m_cfg->GetValue_Boolean(_T("info_to_html"),value_b,true))
		{
			if(value_b)
				m_info_html = TRUE;
			else
				m_info_html = FALSE;
		}
		if(m_cfg->GetValue_Boolean(_T("disable_screensaver"),value_b,true))
		{
			if(!value_b)
				m_screensaver = TRUE;
			else
				m_screensaver = FALSE;
		}
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMOtherPage::SetNormal()
{
	m_other = _T("");
	m_screensaver = FALSE;
	m_info_html = FALSE;
}

void CMOtherPage::SetHigh()
{
	SetNormal();
}

void CMOtherPage::SetLower()
{
	SetNormal();
}

void CMOtherPage::SaveConfig()
{
	UpdateData(TRUE);
	if(!m_cfg)
		return;

	if(m_last_page)
		m_cfg->SetValue(_T("meditor_last_page"),_T("1"),true,ex_meditor);
	else	
		m_cfg->SetValue(_T("meditor_last_page"),_T("0"),true,ex_meditor);

	if(m_one)
		m_cfg->SetValue(_T("meditor_one_editor"),_T("1"),true,ex_meditor);
	else	
		m_cfg->RemoveValue(_T("meditor_one_editor"),true);

	if(m_info_html)
		m_cfg->SetValue(_T("info_to_html"),_T("1"),true,ex_setting);
	else	
		m_cfg->RemoveValue(_T("info_to_html"),true);

	if(m_screensaver)
		m_cfg->SetValue(_T("disable_screensaver"),_T("0"),true,ex_sysinfo);
	else	
		m_cfg->RemoveValue(_T("disable_screensaver"),true);

	m_other.TrimRight(_T("\r\n"));
	if(m_other.GetLength() > 1)
	{
		m_other += _T("\r\n");
		m_cfg->SetValue_Other(m_other);
	}
	else
		m_cfg->SetValue_Other(_T(""));

}

BOOL CMOtherPage::PreTranslateMessage(MSG* pMsg) 
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CMOtherPage::OnButtonAudio() 
{
	
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		,ResStr(IDS_OTHER_TYPEA) + _T("|*.mp3;*.mp2;*.m4a;*.aac;*.mpc;*.wma;*.ogg;\
*.arm;*.mka;*.flac;*.ac3;*.dts;*.wav;*.ra;*.aif|") + ResStr(IDS_OTHER_ALL) + _T("(*.*)|*.*||"));

	dlg.m_ofn.lpstrTitle=ResStr(IDS_OTHER_OPENA);
    if(dlg.DoModal()==IDOK)
	{
		m_audio = dlg.GetPathName();
	}
	dlg.DestroyWindow();
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}

void CMOtherPage::OnButtonClean() 
{
	m_video = _T("");
	m_audio = _T("");
	UpdateData(FALSE);
}

void CMOtherPage::OnButtonPlay() 
{
	
	UpdateData(TRUE);
	if(m_video == _T("") || m_audio == _T(""))
	{
		MessageBox(ResStr(IDS_OTHER_EMPTY), NULL, MB_TOPMOST);
		return;
	}
	if(!IsFileExist(m_program_dir + _T("mplayer.exe")))
	{
		MessageBox(ResStr(IDS_PLAYER_SAMEDIR), NULL, MB_TOPMOST);
		return;
	}
	CString mpcmd;
	mpcmd.Format(_T("-audiofile \"%s\" \"%s\""), m_audio, m_video);
	ShellExecute(0, _T("open"), m_program_dir + _T("mplayer.exe"), mpcmd, NULL, SW_SHOW);
}

void CMOtherPage::OnButtonVideo() 
{
	
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		,ResStr(IDS_OTHER_TYPEV) + _T("|*.avi;*.mkv;*.ogm;*.mp4;*.m4v;*.m4p;*.m4b;*.flv;*.vp6;\
*.divx;*.vg2;*.dat;*.mpg;*.mpeg;*.tp;*.ts;*.tpr;*.pva;*.pps;*.mpe;*.vob;*.rm;*.rmvb;\
*.wmv;*.asf;*.wmp;*.wm;*.mov;*.qt;*.3gp;*.3gpp;*.3g2;*.3gp2|") + ResStr(IDS_OTHER_ALL) + _T("(*.*)|*.*||"));

//	dlg.m_ofn.lpstrFile = new wchar_t[102400];
//	memset(dlg.m_ofn.lpstrFile,0,sizeof(wchar_t)   *   102400);
//	dlg.m_ofn.nMaxFile = 102400;
	dlg.m_ofn.lpstrTitle=ResStr(IDS_OTHER_OPENV);
    if(dlg.DoModal()==IDOK)
	{
		m_video = dlg.GetPathName();
	}
	dlg.DestroyWindow();
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}
