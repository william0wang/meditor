// OtherDlg.cpp : implementation of the COtherDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "Reg.h"
#include "RealOnline.h"
#include "OtherDlg.h"
#include "LinkDlg.h"

extern CString strSatellite;

COtherDlg::COtherDlg(UINT IDD_DLG)
{
	IDD = IDD_DLG;

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	m_assoc_exe.Format(_T("%s"),szFilePath);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
}

BOOL COtherDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL COtherDlg::OnIdle()
{
	return FALSE;
}

void COtherDlg::InitCheckBox()
{
	if(mconfig.GetBoolean(_T("info_to_html"), false, true))
		m_info_html = TRUE;
	else
		m_info_html = FALSE;

	if(mconfig.GetBoolean(_T("disable_screensaver"), false, true))
		m_screensaver = TRUE;
	else
		m_screensaver = FALSE;
}

void COtherDlg::SaveConfig()
{
	DoDataExchange(TRUE);

	if(m_info_html)
		mconfig.SetValue(_T("info_to_html"), _T("1"), true, ex_setting);
	else	
		mconfig.RemoveValue(_T("info_to_html"), true);

	if(m_screensaver)
		mconfig.SetValue(_T("disable_screensaver"), _T("0"), true, ex_sysinfo);
	else
		mconfig.RemoveValue(_T("disable_screensaver"), true);
}

LRESULT COtherDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	UIAddChildWindowContainer(m_hWnd);

	::SetWindowText(GetDlgItem(IDC_CHECK_SCREENSAVER), rStr.screensave);
	::SetWindowText(GetDlgItem(IDC_CHECK_INFO), rStr.mediainfo);
	::SetWindowText(GetDlgItem(IDC_BUTTON_LINK), rStr.clink);
	::SetWindowText(GetDlgItem(IDC_RADIO_MPLAYER), rStr.real_mp);
	::SetWindowText(GetDlgItem(IDC_RADIO_MEDITOR), rStr.real_ds);
	::SetWindowText(GetDlgItem(IDC_BUTTON_ONLINE), rStr.real_reg);
	::SetWindowText(GetDlgItem(IDC_BUTTON_DONLINE), rStr.real_dreg);

	InitCheckBox();

	OSVERSIONINFO version;
	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(GetVersionEx(&version)) {
		if(version.dwMajorVersion >= 6) {
			Button_SetElevationRequiredState(GetDlgItem(IDC_BUTTON_ONLINE), TRUE);
			Button_SetElevationRequiredState(GetDlgItem(IDC_BUTTON_DONLINE), TRUE);
		}
	}

	if(FileExist(m_program_dir + _T("codecs\\Real\\mloader.ini")))
		CheckRadioButton(IDC_RADIO_MPLAYER, IDC_RADIO_MEDITOR, IDC_RADIO_MEDITOR);
	else
		CheckRadioButton(IDC_RADIO_MPLAYER, IDC_RADIO_MEDITOR, IDC_RADIO_MPLAYER);

	m_mpc_exe = m_program_dir + _T("tools\\mplayerc.exe");
	if(FileExist(m_mpc_exe)) {
		GetDlgItem(IDC_CHECK_MPC).EnableWindow(TRUE);
		m_mpc = TRUE;
	}

	DoDataExchange();
	
	return TRUE;
}

LRESULT COtherDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT COtherDlg::OnBnClickedReg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(IsRunAsAdmin()) {
		CRealOnline real(strSatellite);
		real.m_cmdline = _T("--real-online-reg");
		real.m_dir = m_program_dir + _T("codecs\\");
		real.InstallReal();
		return 0;
	} else {
		SHELLEXECUTEINFO sei = { sizeof(sei) };
		sei.lpVerb = _T("runas");
		sei.lpFile = m_assoc_exe;
		sei.lpParameters = _T("--real-online-reg");
		sei.hwnd = m_hWnd;
		sei.nShow = SW_NORMAL;

		ShellExecuteEx(&sei);
	}
	return 0;
}

LRESULT COtherDlg::OnBnClickedDReg(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(IsRunAsAdmin()) {
		CRealOnline real(strSatellite);
		real.m_cmdline = _T("--real-online-dreg");
		real.m_dir = m_program_dir + _T("codecs\\");
		real.InstallReal();
		return 0;
	} else {
		SHELLEXECUTEINFO sei = { sizeof(sei) };
		sei.lpVerb = _T("runas");
		sei.lpFile = m_assoc_exe;
		sei.lpParameters = _T("--real-online-dreg");
		sei.hwnd = m_hWnd;
		sei.nShow = SW_NORMAL;

		ShellExecuteEx(&sei);
	}
	return 0;
}

LRESULT COtherDlg::OnBnClickedRadioMplayer(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CheckRadioButton(IDC_RADIO_MPLAYER, IDC_RADIO_MEDITOR, IDC_RADIO_MPLAYER);
	return 0;
}
LRESULT COtherDlg::OnBnClickedRadioMeditor(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CheckRadioButton(IDC_RADIO_MPLAYER, IDC_RADIO_MEDITOR, IDC_RADIO_MEDITOR);
	return 0;
}

LRESULT COtherDlg::OnBnClickedButtonFlash(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShellExecute(0, _T("open"), m_program_dir + _T("meditor.exe"), _T(" --flash-player"), NULL, SW_SHOW);
	return 0;
}

LRESULT COtherDlg::OnBnClickedButtonMedia(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	ShellExecute(0, _T("open"), m_program_dir + _T("meditor.exe"), _T(" --dshow-player"), NULL, SW_SHOW);
	return 0;
}

BOOL CreateLink(CString PathExe, CString LinkPath, BOOL bCreate, CString Arguments = _T(""), int icon = 0)
{
	IShellLink* psl;
	IPersistFile* ppf;

	if(!FileExist(PathExe))
		return FALSE;

	if(SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl))) {
		psl->SetPath(PathExe);
		if(SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (void**)&ppf)) ) {
			if(bCreate) {
				if(Arguments.GetLength() > 1)
					psl->SetArguments(Arguments);
				if(icon > 0)
					psl->SetIconLocation(PathExe, icon);
				ppf->Save(LinkPath, STGM_READWRITE);
			} else
				DeleteFile(LinkPath);
		}
		ppf->Release();
	}
	psl->Release();

	return TRUE;
}

LRESULT COtherDlg::OnBnClickedButtonLink(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CLinkDlg dlg;
	if(dlg.DoModal() != IDOK)
		return 0;

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr)) {
		TCHAR szPath[MAX_PATH + 1];
		GetModuleFileName(NULL, szPath, MAX_PATH);

		CString meditor_exe(szPath);
		CString mplayer_exe(m_program_dir + _T("mplayer.exe"));
		CString tools_name;
		CString DeskTop_dir;
		CString StartMenu_dir;
		CString LinkPath;

		if(GetSpecialFolder(CSIDL_DESKTOPDIRECTORY , szPath))
			DeskTop_dir.Format(_T("%s\\"), szPath);

		if(GetSpecialFolder(CSIDL_PROGRAMS, szPath)) {
			CString dir;
			dir.Format(_T("%s\\MPlayer"), szPath);
			if(dlg.m_meditor_start || dlg.m_mplayer_start || dlg.m_tools_start
				|| dlg.m_flash_start  || dlg.m_dshow_start) {
				CreateDirectory(dir, NULL);
				StartMenu_dir.Format(_T("%s\\"), dir);
				dir.Format(_T("%s\\MPlayer\\Tools"), szPath);
				if(dlg.m_tools_start && FileExist(m_program_dir + _T("tools")))
					CreateDirectory(dir, NULL);
				else
					DeleteFolder(dir);
			} else
				DeleteFolder(dir);
		}

		if(FileExist(DeskTop_dir)) {
			LinkPath.Format(_T("%sMPlayer.lnk") , DeskTop_dir);
			CreateLink(mplayer_exe, LinkPath, dlg.m_mplayer_desk);

			LinkPath.Format(_T("%s%s.lnk") , DeskTop_dir, rStr.meditor);
			CreateLink(meditor_exe, LinkPath, dlg.m_meditor_desk);
		}

		if(FileExist(StartMenu_dir)) {
			LinkPath.Format(_T("%sMPlayer.lnk") , StartMenu_dir);
			CreateLink(mplayer_exe, LinkPath, dlg.m_mplayer_start);

			LinkPath.Format(_T("%s%s.lnk") , StartMenu_dir, rStr.meditor);
			CreateLink(meditor_exe, LinkPath, dlg.m_meditor_start);

			LinkPath.Format(_T("%sFlashPlayer.lnk"), StartMenu_dir);
			CreateLink(meditor_exe, LinkPath, dlg.m_flash_start, _T("--flash-player"), 1);

			LinkPath.Format(_T("%sDShowPlayer.lnk"), StartMenu_dir);
			CreateLink(meditor_exe, LinkPath, dlg.m_dshow_start, _T("--dshow-player"), 2);

			tools_name.Format(_T("%stools\\mplayerc.exe") , m_program_dir);
			LinkPath.Format(_T("%sMPC-HC.lnk") , StartMenu_dir);
			CreateLink(tools_name, LinkPath, dlg.m_mpc_start);

			if(dlg.m_tools_start) {
				tools_name.Format(_T("%stools\\MKVextractGUI.exe") , m_program_dir);
				LinkPath.Format(_T("%sTools\\MKVextractGUI.lnk") , StartMenu_dir);
				CreateLink(tools_name, LinkPath, TRUE);

				tools_name.Format(_T("%stools\\flvmdigui.exe") , m_program_dir);
				LinkPath.Format(_T("%sTools\\flvmdigui.lnk") , StartMenu_dir);
				CreateLink(tools_name, LinkPath, TRUE);

				tools_name.Format(_T("%stools\\FLVExtract.exe") , m_program_dir);
				LinkPath.Format(_T("%sTools\\FLVExtract.lnk") , StartMenu_dir);
				CreateLink(tools_name, LinkPath, TRUE);

				tools_name.Format(_T("%stools\\SubDownloader.exe") , m_program_dir);
				LinkPath.Format(_T("%sTools\\SubDownloader.lnk") , StartMenu_dir);
				CreateLink(tools_name, LinkPath, TRUE);

				tools_name.Format(_T("%stools\\MediaInfo.exe") , m_program_dir);
				LinkPath.Format(_T("%sTools\\MediaInfo.lnk") , StartMenu_dir);
				CreateLink(tools_name, LinkPath, TRUE);

				tools_name.Format(_T("%stools\\tsMuxerGUI.exe") , m_program_dir);
				LinkPath.Format(_T("%sTools\\tsMuxerGUI.lnk") , StartMenu_dir);
				CreateLink(tools_name, LinkPath, TRUE);

				tools_name.Format(_T("%stools\\TSSplitter.exe") , m_program_dir);
				LinkPath.Format(_T("%sTools\\TSSplitter.lnk") , StartMenu_dir);
				CreateLink(tools_name, LinkPath, TRUE);
			}
		}
		CoUninitialize();
	}
	return 0;
}
