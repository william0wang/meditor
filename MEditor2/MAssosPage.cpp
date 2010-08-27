// MAssosPage.cpp : 实现文件
//
#include "stdafx.h"
#include "meditor2.h"
#include "MAssosPage.h"
#include "reg.h"


// CMAssosPage 对话框

IMPLEMENT_DYNAMIC(CMAssosPage, CDialog)

CMAssosPage::CMAssosPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMAssosPage::IDD, pParent)
	, m_update(FALSE)
{
	m_is_vista = FALSE;

	OSVERSIONINFO version;

	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&version)) {
		if(version.dwMajorVersion >= 6)
			m_is_vista = TRUE;
	}

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);

	m_assoc_exe = m_program_dir + _T("\\tools\\massoc.exe");

	if(!IsFileExist(m_assoc_exe))
		m_assoc_exe = m_program_dir + _T("\\codecs\\massoc.exe");

	if(!IsFileExist(m_assoc_exe))
		m_assoc_exe = m_program_dir + _T("\\massoc.exe");

	m_player_exe = m_program_dir + _T("mplayer.exe");
}

CMAssosPage::~CMAssosPage()
{
}

void CMAssosPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_UPDATE, m_update);
}


BEGIN_MESSAGE_MAP(CMAssosPage, CDialog)
	ON_BN_CLICKED(IDC_ASSOS, OnBnClickedAssos)
	
	ON_BN_CLICKED(IDC_ASSOS_DEF, &CMAssosPage::OnBnClickedAssosDef)
END_MESSAGE_MAP()


// CMAssosPage 消息处理程序

BOOL CMAssosPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMAssosPage::OnBnClickedAssos()
{
	ShellExecute(0, _T("open"), m_assoc_exe , NULL, NULL, SW_SHOW);
}

void CMAssosPage::OnBnClickedAssosDef()
{
	if(m_is_vista) {

		UpdateData(TRUE);

		bool needupdate = false;
		if(!m_update) {
			CReg reg;
			CString SubKey, Name, Content;

			Name =  _T("MPlayer");
			Content = _T("SOFTWARE\\MPlayer\\Capabilites");
			SubKey =  _T("SOFTWARE\\RegisteredApplications");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else {
				if(_tcsicmp(reg.content, Content))
					needupdate = true;
			}

			Name =  _T("");
			Content = _T("");
			SubKey =  _T("SOFTWARE\\MPlayer");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else {
				if(_tcsicmp(reg.content, Content))
					needupdate = true;
			}

			SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
			Name =  _T("ApplicationDescription");
			Content = _T("MPlayer WW - The Movie Player.");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else {
				if(_tcsicmp(reg.content, Content))
					needupdate = true;
			}

			SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
			Name =  _T("ApplicationName");
			Content = _T("MPlayer WW");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else {
				if(_tcsicmp(reg.content, Content))
					needupdate = true;
			}

			SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
			Name =  _T("ApplicationIcon");
			Content = m_player_exe + _T(",0");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else {
				if(_tcsicmp(reg.content, Content))
					needupdate = true;
			}
		}

		if(needupdate || m_update) {
			ShellExecute(0, _T("open"), m_assoc_exe, _T("--shell-associations-updater"), NULL, SW_SHOW);
		} else {
			IApplicationAssociationRegistrationUI* pAARUI = NULL;
			HRESULT hr = ::CoCreateInstance( CLSID_ApplicationAssociationRegistrationUI
				, NULL, CLSCTX_INPROC, __uuidof( IApplicationAssociationRegistrationUI )
				, reinterpret_cast< void** >( &pAARUI ) );

			if ( SUCCEEDED( hr ) && pAARUI != NULL ) {
				hr = pAARUI->LaunchAdvancedAssociationUI( _T( "MPlayer" ) );
				pAARUI->Release();
			}
		}
	}
}
