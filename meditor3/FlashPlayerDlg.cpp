// FlashPlayerDlg.cpp : implementation of the CFlashPlayerDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "flash10h.h"
#include "ListCtrl/ListCtrl.h"
#include "InputList.h"
#include "AtlStdioFile.h"
#include "Win7ShellApi.h"
#include "FlashPlayerDlg.h"

static IShockwaveFlash *pFlashPtr = NULL;
static CAxWindow m_wndFlashPlayer;
static ChangeWindowMessageFilterFunction ChangeWindowMessageFilterDLL = NULL;

#define CMD_KEY			WM_USER + 1568
#define IDTB_PLAY_PAUSE	WM_USER + 2068
#define TIMER_HIDE_CURSOR 101

CFlashPlayerDlg::CFlashPlayerDlg()
{
	m_inited = false;
	m_keydowning = false;
	m_ontop = false;
	m_rmenu = false;
	m_fs = false;
	m_fs_init = false;
	m_showctrl = true;
	m_changing = false;
	m_current = 0;
	now_state = 0;
	m_applang = 0;
	g_pTaskbarList = NULL;
	s_uTBBC = NULL;

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);

}

bool CFlashPlayerDlg::IsFlashPlaying()
{
	VARIANT_BOOL playing;
	if(SUCCEEDED(pFlashPtr->IsPlaying(&playing)))
		if(playing) return TRUE;

	return FALSE;
}

HRESULT CFlashPlayerDlg::UpdateThumbnailToolbar(HWND hWnd)
{
	HRESULT hr = NULL;
	if (g_pTaskbarList)
	{
		THUMBBUTTON buttons[2] = {};

		buttons[0].dwMask = THB_BITMAP | THB_FLAGS;
		buttons[0].dwFlags = THBF_ENABLED;
		buttons[0].iId = IDTB_PLAY_PAUSE;

		if(IsFlashPlaying())
			buttons[0].iBitmap = 1;
		else
			buttons[0].iBitmap = 0;

		// Set the buttons to be the thumbnail toolbar
		hr = g_pTaskbarList->ThumbBarUpdateButtons(hWnd, 1, buttons);
	}

	return hr;
}

void CFlashPlayerDlg::FullScreen(bool init)
{
	if(!m_fs) {
		m_fs = true;
		m_showctrl = false;
		RECT rc;
		GetWindowRect(&rc);
		save_x = rc.left;
		save_y = rc.top;
		save_width = rc.right - rc.left;
		save_height = rc.bottom - rc.top;
		m_scr_width = GetSystemMetrics(SM_CXSCREEN);
		m_scr_height = GetSystemMetrics(SM_CYSCREEN);
		if(init) {
			save_x = (m_scr_width - save_width) / 2;
			save_y = (m_scr_height - save_height) / 2;
		}
		ModifyStyle(WS_CAPTION | WS_THICKFRAME,WS_OVERLAPPED);
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,m_scr_width,m_scr_height,SWP_SHOWWINDOW);
		while (ShowCursor(0) >= 0)  ;
	} else {
		m_fs = false;
		m_showctrl = true;
		ModifyStyle(WS_OVERLAPPED,WS_CAPTION | WS_THICKFRAME);
		if(!m_ontop)
			::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,save_x,save_y	,save_width,save_height,SWP_SHOWWINDOW);
		else
			::SetWindowPos(this->m_hWnd,HWND_TOPMOST,save_x,save_y,save_width,save_height,SWP_SHOWWINDOW);
		KillTimer(TIMER_HIDE_CURSOR);
		ShowCursor(TRUE);
		while (ShowCursor(1) < 0)  ;
	}
}

BOOL CFlashPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	int const wmId = LOWORD(pMsg->wParam);
	if (pMsg->message == s_uTBBC)
	{
		if (!g_pTaskbarList)
		{
			HRESULT hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pTaskbarList));
			if (SUCCEEDED(hr))
			{
				hr = g_pTaskbarList->HrInit();
				if (FAILED(hr))
				{
					g_pTaskbarList->Release();
					g_pTaskbarList = NULL;
				}

				if(g_pTaskbarList) {
					hImglist = ImageList_LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP_96),
						16, 0, RGB(255,0,255), IMAGE_BITMAP, LR_CREATEDIBSECTION);
					if (hImglist)
					{
						hr = g_pTaskbarList->ThumbBarSetImageList(this->m_hWnd, hImglist);
						if (SUCCEEDED(hr))
						{
							THUMBBUTTON buttons[2] = {};

							buttons[0].dwMask = THB_BITMAP | THB_FLAGS;
							buttons[0].dwFlags = THBF_ENABLED;
							buttons[0].iId = IDTB_PLAY_PAUSE;

							if(IsFlashPlaying())
								buttons[0].iBitmap = 1;
							else
								buttons[0].iBitmap = 0;

							g_pTaskbarList->ThumbBarAddButtons(this->m_hWnd, 1, buttons);
						}
						g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_NORMAL);
						ImageList_Destroy(hImglist);
					}
				}
			}
		}
	} else if(pMsg->message == WM_COMMAND && (wmId == IDTB_PLAY_PAUSE)) {
		if(IsFlashPlaying())
			pFlashPtr->Stop();
		else
			pFlashPtr->Play();
		UpdateThumbnailToolbar(m_hWnd);
	} else {
		switch(pMsg->message)
		{
		case WM_NCRBUTTONDOWN:
			{
				if(pMsg->wParam == HTMAXBUTTON || pMsg->wParam == HTMINBUTTON || pMsg->wParam == HTCLOSE)
					break;
				m_popmenu.GetSubMenu(0).TrackPopupMenu(TPM_LEFTALIGN, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam), m_hWnd);
				return TRUE;
			}
		case WM_SYSCHAR:
			if (pMsg->wParam == 13)
				FullScreen();
			break;
		case   WM_KEYDOWN:
			SendMessage(CMD_KEYDOWN, CMD_KEY, pMsg->wParam );
			if(pMsg->wParam == 13)
				return TRUE;
			break;
		case   WM_LBUTTONDOWN:
			POINT point;
			GetCursorPos(&point);
			if(m_fs && m_showctrl)
			{
				if(point.y < m_scr_height - 16)
					break;
				if(point.x < 5 || point.x > m_scr_width - 5)
					break;
				m_changing = true;
				double pos = (double)(point.x - 5) / (double) (m_scr_width - 10);
				pFlashPtr->GotoFrame((long)(m_fnumber * pos));
				pFlashPtr->Play();
				m_changing = false;
				return TRUE;
			}
			break;
		case  WM_RBUTTONDOWN:
			if(!m_rmenu)
			{
				SendMessage(CMD_KEYDOWN, CMD_KEY, WM_RBUTTONDOWN );
				return TRUE;
			}
			break;
		case  WM_MOUSEMOVE:
			{
				if(!m_fs)
					break;
				ShowCursor(TRUE);
				SetTimer(TIMER_HIDE_CURSOR, 2000, NULL);
				POINT point;
				GetCursorPos(&point);
				if(!m_showctrl && point.y >= m_scr_height - 20)
				{
					m_control.MoveWindow(0,m_scr_height - 16,m_scr_width,20);
					m_showctrl = true;
				}
				else if(m_showctrl && point.y < m_scr_height - 20)
				{
					m_control.MoveWindow(0,m_scr_height,m_scr_width,20);
					m_showctrl = false;
				}
				break;
			}
		}
	}
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CFlashPlayerDlg::OnIdle()
{
	return FALSE;
}

LRESULT CFlashPlayerDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	if(m_applang == 2)
		m_popmenu.LoadMenu(IDR_MENU_FLASH_EN);
	else if(m_applang == 3 || m_applang == 4)
		m_popmenu.LoadMenu(IDR_MENU_FLASH_TC);
	else
		m_popmenu.LoadMenu(IDR_MENU_FLASH);

	s_uTBBC = RegisterWindowMessage(L"TaskbarButtonCreated");
	HINSTANCE user32 = GetModuleHandle(L"user32.dll");
	if(user32) ChangeWindowMessageFilterDLL = (ChangeWindowMessageFilterFunction)GetProcAddress(user32, "ChangeWindowMessageFilter");
	if(ChangeWindowMessageFilterDLL) {
		ChangeWindowMessageFilterDLL(s_uTBBC, MSGFLT_ADD);
		ChangeWindowMessageFilterDLL(WM_COMMAND, MSGFLT_ADD);
	}

	m_wndFlashPlayer.Attach(GetDlgItem(IDC_SHOCKWAVEFLASH));
	m_control.Attach(GetDlgItem(IDC_SLIDER_CONTROL));

	m_wndFlashPlayer.QueryControl(&pFlashPtr);

	m_control.SetRange(0, 100);
	m_inited = true;


	if(m_ontop)
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	SetTimer(0, 300, 0);

	DoDataExchange();

	LoadConfig();
	
	pFlashPtr->put_Loop(FALSE);
	if(m_playlist.GetSize() > 0) {
		m_current = 0;
		PlayFile();
		if(m_fs_init)
			FullScreen(true);
	}
	
	return TRUE;
}

LRESULT CFlashPlayerDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_inited = false;
	pFlashPtr->Release();

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);


	return 0;
}

void CFlashPlayerDlg::IninFileName(CString filename)
{
	filename.TrimLeft(_T(" "));
	filename.TrimRight(_T(" "));
	filename.TrimLeft(_T("\""));
	filename.TrimRight(_T("\""));
	if(filename.GetLength() > 4)
		m_playlist.Add(filename);
}

void CFlashPlayerDlg::PlayFile(bool init)
{
	if(m_playlist.GetSize() <= m_current)
		return;
	pFlashPtr->put_Movie(m_playlist[m_current].AllocSysString());
	pFlashPtr->get_TotalFrames(&m_fnumber);
	if(m_fnumber <= 0)
		m_fnumber = 1;
	pFlashPtr->Play();
	m_control.SetPos(0);
}

LRESULT CFlashPlayerDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//UINT nType = (UINT)wParam;
	CSize size = _WTYPES_NS::CSize(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));

	if(m_inited) {
		if(!m_fs) {
			m_wndFlashPlayer.MoveWindow(0, 0, size.cx, size.cy - 20, FALSE);
			m_control.MoveWindow(0, size.cy - 20, size.cx, 20);
		} else {
			m_wndFlashPlayer.MoveWindow(0, 0, size.cx, size.cy, FALSE);
			m_control.MoveWindow(0, size.cy, size.cx, 20);
		}
	}
	return 0;
}


void CFlashPlayerDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CFlashPlayerDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CFlashPlayerDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CFlashPlayerDlg::OnFlashOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);

	TCHAR strBuffer[MAX_PATH * 2]  = {0};
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY, _T("Flash (*.swf)\0*.swf\0All File(*.*)\0*.*\0"));
	dlg.m_ofn.lpstrFile = strBuffer;
	dlg.m_ofn.nMaxFile = MAX_PATH * 2;
	if(dlg.DoModal()==IDOK) {
		m_playlist.RemoveAll();
		m_playlist.Add(dlg.m_ofn.lpstrFile);
		m_current = 0;
		PlayFile();
		if(m_fs_init)
			FullScreen();
	}
	::SetCurrentDirectory(szFilePath);
	DoDataExchange(FALSE);

	return 0;
}

void CFlashPlayerDlg::OnPlay()
{
	pFlashPtr->Play();
	if(g_pTaskbarList) g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_NORMAL);
}

void CFlashPlayerDlg::OnStop()
{
	pFlashPtr->Stop();
	if(g_pTaskbarList) g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_NOPROGRESS);
}

void CFlashPlayerDlg::OnNext() 
{
	if(m_playlist.GetSize() <= 1)
		return;
	if( m_current < m_playlist.GetSize() - 1)
		m_current++;
	else
		m_current = 0;
	PlayFile();
}

void CFlashPlayerDlg::OnPre()
{
	if(m_playlist.GetSize() <= 1)
		return;
	if(m_current <= 0)
		m_current = m_playlist.GetSize() - 1;
	else if(m_current > m_playlist.GetSize())
		m_current = m_playlist.GetSize() - 1;
	else
		m_current--;

	PlayFile();
}

void CFlashPlayerDlg::LoadConfig()
{
	CAtlStdioFile playercfg;
	CString inifile = m_program_dir + _T("mplayer.ini");
	if(SUCCEEDED(playercfg.OpenFile(inifile.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
		CStringA lineA;
		CString line;
		while(playercfg.ReadLineA(lineA)) {
			line = local2unicode(lineA.GetBuffer()).c_str();
			lineA.ReleaseBuffer();
			line.TrimLeft(_T(" "));
			line.TrimRight(_T(" "));
			line.MakeLower();
			if(line.Find(_T("#")) == 0)
				continue;
			if(line.Find(_T("fs=yes")) == 0) {
				m_fs_init = true;
				continue;
			}
			if(line.Find(_T("fs=1")) == 0) {
				m_fs_init = true;
				continue;
			}
			if(line.Find(_T("ontop=2")) == 0) {
				m_ontop = true;
				continue;
			}
			if(line.Find(_T("ontop=1")) == 0) {
				m_ontop = true;
				continue;
			}
		}
		playercfg.Close();
	}
	inifile.ReleaseBuffer();

	CAtlStdioFile inputcfg;
	inifile = m_program_dir + _T("input.ini");
	if(SUCCEEDED(inputcfg.OpenFile(inifile.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
		CString line,command,value,key;
		CStringA lineA;
		while(inputcfg.ReadLineA(lineA)) {
			line = local2unicode(lineA.GetBuffer()).c_str();
			lineA.ReleaseBuffer();
			if(AnalyseLine(line,key,command,value))
				m_inputs.Add(key,command,value);
		}
		inputcfg.Close();
	} else {
		m_inputs.Add(_T("F"),_T("vo_fullscreen"),_T(""));
		m_inputs.Add(_T("f"),_T("vo_fullscreen"),_T(""));
		m_inputs.Add(_T("T"),_T("vo_ontop"),_T(""));
		m_inputs.Add(_T("t"),_T("vo_ontop"),_T(""));
		m_inputs.Add(_T("RIGHT"),_T("seek"),_T("30"));
		m_inputs.Add(_T("LEFT"),_T("seek"),_T("-30"));
		m_inputs.Add(_T("DOWN"),_T("seek"),_T("10"));
		m_inputs.Add(_T("UP"),_T("seek"),_T("-10"));
		m_inputs.Add(_T("SPACE"),_T("pause"),_T(""));
		m_inputs.Add(_T("ESC"),_T("quit"),_T(""));
		m_inputs.Add(_T("K"),_T("keep_aspect"),_T(""));
		m_inputs.Add(_T("k"),_T("keep_aspect"),_T(""));
		m_inputs.Add(_T(">"),_T("pt_step"),_T("1"));
		m_inputs.Add(_T("."),_T("pt_step"),_T("1"));
		m_inputs.Add(_T("<"),_T("pt_step"),_T("-1"));
		m_inputs.Add(_T(","),_T("pt_step"),_T("-1"));
	}
}

LRESULT CFlashPlayerDlg::OnCmdKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam != CMD_KEY)
		return FALSE;
	if(m_keydowning)
		return  FALSE;
	m_keydowning = true;
	CString cmd, value;
	if(GetKeyCommand(lParam, cmd, value)) {
		if( cmd == _T("quit")) {
			pFlashPtr->Stop();
			CloseDialog(0);
		} else if( cmd == _T("pause")) {
			if(IsFlashPlaying())
				pFlashPtr->Stop();
			else
				pFlashPtr->Play();
		} else if( cmd == _T("vo_fullscreen")) {
			FullScreen();
		} else if( cmd == _T("vo_ontop")) {
			if(! m_ontop) {
				::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
				m_ontop = true;
			} else {
				::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
				m_ontop = false;
			}
		} else if( cmd == _T("keep_aspect")) {
			int type;
			pFlashPtr->get_ScaleMode(&type);
			if(type == 0)
				pFlashPtr->put_ScaleMode(1);
			else if(type == 1)
				pFlashPtr->put_ScaleMode(2);
			else if(type == 2)
				pFlashPtr->put_ScaleMode(3);
			else if(type == 3)
				pFlashPtr->put_ScaleMode(4);
			else
				pFlashPtr->put_ScaleMode(0);
		} else if( cmd == _T("seek")) {
			int val = _ttoi(value);
			long m_fnow = 0;
			pFlashPtr->CurrentFrame(&m_fnow);
			long seek = m_fnow + (val * 5);
			if(seek > 0 && seek < m_fnumber) {
				pFlashPtr->GotoFrame(seek);
				pFlashPtr->Play();
			} else if (seek <= 0) {
				pFlashPtr->GotoFrame(0);
				pFlashPtr->Play();
			} else if (seek >= m_fnumber) {
				pFlashPtr->GotoFrame(m_fnumber);
				pFlashPtr->Play();
			}
		} else if( cmd == _T("pt_step")) {
			int val = _ttoi(value);
			if(val > 0)
				OnNext();
			else if(val < 0)
				OnPre();
		}
	}
	m_keydowning = false;
	return TRUE;
}
LRESULT CFlashPlayerDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT_PTR nIDEvent = (UINT_PTR)wParam;
	if(nIDEvent == TIMER_HIDE_CURSOR) {
		while(ShowCursor(FALSE)>=0);
		KillTimer(TIMER_HIDE_CURSOR);
	} else {
		VARIANT_BOOL bPlaying;
		pFlashPtr->get_Playing(&bPlaying);
		if(bPlaying && IsFlashPlaying()) {
			if(!m_changing) {
				long m_fnow = 0;
				pFlashPtr->CurrentFrame(&m_fnow);
				int pos = (int)((double)m_fnow / (double)m_fnumber * 100.0);
				if(g_pTaskbarList) g_pTaskbarList->SetProgressValue(this->m_hWnd, pos, 100);
				m_control.SetPos(pos);
			}
		} else if(now_state == 4) {
			long m_fnow = 0;
			pFlashPtr->CurrentFrame(&m_fnow);
			if(m_fnow >= m_fnumber - 3)
				OnNext();
		}
	}
	return 0;
}
bool CFlashPlayerDlg::GetKeyCommand(LONG_PTR KeyNumber,CString &cmd, CString &value)
{
	CString key, abs;
	for(int i = 0 ; i < m_inputs.GetSize() ; i++) {
		m_inputs.GetAt(i, key ,cmd, value, abs);
		if( KeyNumber == GetKeyNumber(key) )
			return true;
	}
	return false;
}

int CFlashPlayerDlg::GetKeyNumber(CString Key)
{
	if(Key.GetLength() == 1) {
		TCHAR x = Key.GetAt(0);
		int xi =  x - _T('a');
		if(xi < 26 && xi >= 0)
			return xi + 0x41;
		xi =   x - _T('A');
		if(xi < 26 && xi >= 0)
			return xi + 0x41;
		xi =   x - _T('0');
		if(xi < 10 && xi >= 0)
			return xi + 0x30;
	}

	if(Key == _T("LEFT"))
		return VK_LEFT;
	else if(Key == _T("RIGHT"))
		return VK_RIGHT;
	else if(Key == _T("UP"))
		return VK_UP;
	else if(Key == _T("DOWN"))
		return VK_DOWN;
	else if(Key == _T("PGUP"))
		return VK_PRIOR;
	else if(Key == _T("PGDWN"))
		return VK_NEXT;
	else if(Key == _T("HOME"))
		return VK_HOME;
	else if(Key == _T("END"))
		return VK_END;
	else if(Key == _T("INS"))
		return VK_INSERT;
	else if(Key == _T("DEL"))
		return VK_DELETE;
	else if(Key == _T("SPACE"))
		return VK_SPACE;
	else if(Key == _T("ENTER"))
		return VK_RETURN;
	else if(Key == _T("ESC"))
		return VK_ESCAPE;

	//	else if(Key == _T("["))
	//		return 78;
	//	else if(Key == _T("]"))
	//		return 79;
	//	else if(Key == _T("{"))
	//		return 80;
	//	else if(Key == _T("}"))
	//		return 81;
	//	else if(Key == _T("\\"))
	//		return 82;
	//	else if(Key == _T("'"))
	//		return 84;
	//	else if(Key == _T("|"))
	//		return 87;
	//	else if(Key == _T("\""))
	//		return 89;
	else if(Key == _T("<"))
		return 188;
	else if(Key == _T(">"))
		return 190;
	else if(Key == _T(","))
		return 188;
	else if(Key == _T("."))
		return 190;
	else if(Key == _T(";"))
		return 0x3b;
	else if(Key == _T(":"))
		return 0x3a;
	else if(Key == _T("="))
		return 0x3d;
	else if(Key == _T("-"))
		return VK_SUBTRACT;
	else if(Key == _T("+"))
		return VK_ADD;
	else if(Key == _T("/"))
		return VK_DIVIDE;
	else if(Key == _T("*"))
		return VK_MULTIPLY;

	else if(Key == _T("F1"))
		return VK_F1;
	else if(Key == _T("F2"))
		return VK_F2;
	else if(Key == _T("F3"))
		return VK_F3;
	else if(Key == _T("F4"))
		return VK_F4;
	else if(Key == _T("F5"))
		return VK_F5;
	else if(Key == _T("F6"))
		return VK_F6;
	else if(Key == _T("F7"))
		return VK_F7;
	else if(Key == _T("F8"))
		return VK_F8;
	else if(Key == _T("F9"))
		return VK_F9;
	else if(Key == _T("F10"))
		return VK_F10;
	else if(Key == _T("F11"))
		return VK_F11;
	else if(Key == _T("F12"))
		return VK_F12;

	//	else if(Key == _T("MOUSE_BTN0"))
	//		return WM_LBUTTONDOWN;
	else if(Key == _T("MOUSE_BTN2"))
		return WM_RBUTTONDOWN;
	//	else if(Key == _T("MOUSE_BTN0_DBL"))
	//		return 108;
	//	else if(Key == _T("MOUSE_BTN2_DBL"))
	//		return 110;
	//	else if(Key == _T("MOUSE_BTN1"))
	//		return 111;
	//	else if(Key == _T("MOUSE_BTN3"))
	//		return 112;
	//	else if(Key == _T("MOUSE_BTN4"))
	//		return 113;

	//	else if(Key == _T("JOY_RIGHT"))
	//		return 114;
	//	else if(Key == _T("JOY_LEFT"))
	//		return 115;
	//	else if(Key == _T("JOY_UP"))
	//		return 116;
	//	else if(Key == _T("JOY_DOWN"))
	//		return 117;
	//	else if(Key == _T("JOY_BTN0"))
	//		return 118;
	//	else if(Key == _T("JOY_BTN1"))
	//		return 119;
	//	else if(Key == _T("JOY_BTN2"))
	//		return 120;
	//	else if(Key == _T("JOY_BTN3"))
	//		return 121;
	return -1;
}

bool CFlashPlayerDlg::AnalyseLine(CString line, CString &key, CString &cmd, CString &val)
{
	line += _T(" ");
	line.TrimLeft(_T(" "));
	int sp = -1;
	if(line.Find(_T("#")) == 0) {
		sp = line.Find(_T("#None"));
		if(sp < 0)
			return false;
		key = line.Left(sp);
		line.Delete(0,sp);
	}
	sp = line.Find(_T(" "));
	if(sp <= 0)
		return false;
	key = line.Left(sp);
	line.Delete(0,sp);

	line.TrimLeft(_T(" "));
	sp = line.Find(_T(" "));
	if(sp <= 0)
		return false;
	cmd = line.Left(sp);
	line.Delete(0,sp);

	line.TrimLeft(_T(" "));
	sp = line.Find(_T(" "));
	if(sp <= 0)
		val = _T("");
	else
		val = line.Left(sp);
	return true;
}

void CFlashPlayerDlg::OnReadyStateChange(long newState) 
{
	now_state = newState;
}

LRESULT CFlashPlayerDlg::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int iFileNumber;
	CString filename;
	TCHAR szFileName[MAX_PATH*2];
	iFileNumber = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);

	m_playlist.RemoveAll();
	for (int i = 0; i < iFileNumber; i++) {
		DragQueryFile((HDROP)wParam, i, szFileName, MAX_PATH*2);
		m_playlist.Add(szFileName);
	}
	m_current = 0;
	PlayFile();
	if(m_fs_init)
		FullScreen();

	return 0;
}

LRESULT CFlashPlayerDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT nSBCode = (int)LOWORD(wParam);
	//UINT nPos = (short)HIWORD(wParam);
	//CScrollBar pScrollBar = (HWND)lParam;

	if(nSBCode == TB_THUMBTRACK) {
		m_changing = true;
	} else if(nSBCode == TB_ENDTRACK) {
		m_changing = true;
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(&p);
		int pos = p.x-5;
		if(pos >= 0) {
			CRect rc;
			m_control.GetClientRect(&rc);
			pos = pos * 100 / (rc.Width()-10);
			if(g_pTaskbarList) g_pTaskbarList->SetProgressValue(this->m_hWnd, pos, 100);
			m_control.SetPos(pos);
		}
	}

	return 0;
}

LRESULT CFlashPlayerDlg::OnNmReleasedcaptureSlider(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	bool playing = false;
	if(IsFlashPlaying())
		playing = true;
	int pos = m_control.GetPos();
	pFlashPtr->GotoFrame(m_fnumber * pos / 100);
	if(playing)
		pFlashPtr->Play();

	m_changing = false;
	return 0;
}

LRESULT CFlashPlayerDlg::OnPlayM(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnPlay();
	return 0;
}

LRESULT CFlashPlayerDlg::OnStopM(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnStop();
	return 0;
}

LRESULT CFlashPlayerDlg::OnNextM(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnNext();
	return 0;
}

LRESULT CFlashPlayerDlg::OnPreM(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnPre();
	return 0;
}

LRESULT CFlashPlayerDlg::OnRmenu(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_rmenu = !m_rmenu;
	if(m_rmenu)
		m_popmenu.CheckMenuItem(IDM_RMENU, MF_CHECKED);
	else
		m_popmenu.CheckMenuItem(IDM_RMENU, MF_UNCHECKED);
	return 0;
}
