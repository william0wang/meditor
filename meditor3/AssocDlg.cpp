// AssocDlg.cpp : implementation of the CAssocDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "Reg.h"
#include <Shobjidl.h>
#include "AssocDlg.h"
#include "AssocDlgAdmin.h"

extern CString strSatellite;
extern int AppLanguage;

CAssocDlg::CAssocDlg(UINT IDD_DLG)
{
	IDD = IDD_DLG;

	m_is_vista = FALSE;

	OSVERSIONINFO version;

	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&version)) {
		if(version.dwMajorVersion >= 6)
			m_is_vista = TRUE;
	}

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	m_assoc_exe.Format(_T("%s"),szFilePath);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);

	//m_assoc_exe = m_program_dir + _T("\\tools\\massoc.exe");

	if(!FileExist(m_assoc_exe))
		m_assoc_exe = m_program_dir + _T("\\codecs\\massoc.exe");

	if(!FileExist(m_assoc_exe))
		m_assoc_exe = m_program_dir + _T("\\massoc.exe");

	m_player_exe = m_program_dir + _T("mplayer.exe");
}

BOOL CAssocDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CAssocDlg::OnIdle()
{
	return FALSE;
}


LRESULT CAssocDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UIAddChildWindowContainer(m_hWnd);
	
	::SetWindowText(GetDlgItem(IDC_ASSOC), rStr.set_assoc);
	::SetWindowText(GetDlgItem(IDC_ASSOC_DEF), rStr.set_assoc + _T(" (Win7/Vista)"));
	::SetWindowText(GetDlgItem(IDC_CHECK_UPDATE), rStr.update_assoc);	

	if(m_is_vista) {
		Button_SetElevationRequiredState(GetDlgItem(IDC_ASSOC), TRUE);
		Button_SetElevationRequiredState(GetDlgItem(IDC_ASSOC_DEF), TRUE);
	}

	DoDataExchange();
	
	return TRUE;
}

LRESULT CAssocDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	return 0;
}


LRESULT CAssocDlg::OnBnClickedAssoc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(IsRunAsAdmin()) {
		CAssocDlgAdmin dlg(strSatellite, AppLanguage);
		dlg.DoModal();
	} else {
		SHELLEXECUTEINFO sei = { sizeof(sei) };
		sei.lpVerb = _T("runas");
		sei.lpFile = m_assoc_exe;
		sei.lpParameters = _T("--assoc-admin");
		sei.hwnd = m_hWnd;
		sei.nShow = SW_NORMAL;

		ShellExecuteEx(&sei);
	}

	return 0;
}

LRESULT CAssocDlg::OnBnClickedAssocDef(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(m_is_vista) {
		DoDataExchange(TRUE);

		if(IsRunAsAdmin()) {
			CAssocDlgAdmin dlg(strSatellite, AppLanguage);
			dlg.ApplyDefault();
			return 0;
		}

		bool needupdate = false;
		if(!m_update) {
			CReg reg;
			CString SubKey, Name, Content;

			Name =  _T("MPlayer");
			Content = _T("SOFTWARE\\MPlayer\\Capabilites");
			SubKey =  _T("SOFTWARE\\RegisteredApplications");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else if(_tcsicmp(reg.content, Content)) {
				needupdate = true;
			}

			Name =  _T("");
			Content = _T("");
			SubKey =  _T("SOFTWARE\\MPlayer");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else if(_tcsicmp(reg.content, Content)) {
				needupdate = true;
			}

			SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
			Name =  _T("ApplicationDescription");
			Content = _T("MPlayer WW - The Movie Player.");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else if(_tcsicmp(reg.content, Content)) {
				needupdate = true;
			}

			SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
			Name =  _T("ApplicationName");
			Content = _T("MPlayer WW");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else if(_tcsicmp(reg.content, Content)) {
				needupdate = true;
			}

			SubKey =  _T("SOFTWARE\\MPlayer\\Capabilites");
			Name =  _T("ApplicationIcon");
			Content = m_player_exe + _T(",0");
			if(!reg.ShowContent_STR(HKEY_LOCAL_MACHINE, SubKey, Name)) {
				needupdate = true;
			} else if(_tcsicmp(reg.content, Content)) {
				needupdate = true;
			}
		}
		if(needupdate || m_update) {
			SHELLEXECUTEINFO sei = { sizeof(sei) };
			sei.lpVerb = _T("runas");
			sei.lpFile = m_assoc_exe;
			sei.lpParameters = _T("--shell-associations-updater");
			sei.hwnd = m_hWnd;
			sei.nShow = SW_NORMAL;

			ShellExecuteEx(&sei);
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

	return 0;
}
