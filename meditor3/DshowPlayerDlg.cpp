// DShowPlayerDlg.cpp : implementation of the CDShowPlayerDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "shared.h"
#include "ListCtrl/ListCtrl.h"
#include "InputList.h"
#include "AtlStdioFile.h"
#include "Win7ShellApi.h"
#include "DshowPlayerDlg.h"

static ChangeWindowMessageFilterFunction ChangeWindowMessageFilterDLL = NULL;

#define CMD_KEY			WM_USER + 1568
#define IDTB_PLAY_PAUSE	WM_USER + 2068
#define TIMER_HIDE_CURSOR 101

CDShowPlayerDlg::CDShowPlayerDlg()
{
	m_FilterGraph = NULL;
	m_keydowning = false;
	m_changing = false;
	m_ontop = false;
	m_fs = false;
	m_fs_init = false;
	m_showctrl = true;
	g_pTaskbarList = NULL;
	s_uTBBC = NULL;

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);

}

HRESULT CDShowPlayerDlg::UpdateThumbnailToolbar(HWND hWnd)
{
	HRESULT hr = NULL;
	if (g_pTaskbarList)
	{
		THUMBBUTTON buttons[2] = {};

		buttons[0].dwMask = THB_BITMAP | THB_FLAGS;
		buttons[0].dwFlags = THBF_ENABLED;
		buttons[0].iId = IDTB_PLAY_PAUSE;

		if(m_FilterGraph && m_FilterGraph->IsRunning())
			buttons[0].iBitmap = 1;
		else
			buttons[0].iBitmap = 0;

		// Set the buttons to be the thumbnail toolbar
		hr = g_pTaskbarList->ThumbBarUpdateButtons(hWnd, 1, buttons);
	}

	return hr;
}

void CDShowPlayerDlg::OnFullScreen()
{
	if(m_FilterGraph) {
		// Stop the filter graph first
		m_fs = !m_fs;
		if(m_fs) {
			ModifyStyle(WS_CAPTION | WS_THICKFRAME,WS_OVERLAPPED);
			m_SliderGraph.ShowWindow(SW_HIDE);
			RECT rc;
			GetWindowRect(&rc);
			save_x = rc.left;
			save_y = rc.top;
			save_width = rc.right - rc.left;
			save_height = rc.bottom - rc.top;
			m_scr_width = GetSystemMetrics(SM_CXSCREEN);
			m_scr_height = GetSystemMetrics(SM_CYSCREEN);
			m_scr_width = GetSystemMetrics(SM_CXSCREEN);
			m_scr_height = GetSystemMetrics(SM_CYSCREEN);
			m_SliderGraph.ShowWindow(SW_HIDE);
			m_showctrl = false;
			::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,m_scr_width,m_scr_height,SWP_SHOWWINDOW);
			while (ShowCursor(0) >= 0)  ;
		} else {
			ModifyStyle(WS_OVERLAPPED,WS_CAPTION | WS_THICKFRAME);
			if(save_width < 5 || save_height < 5) {
				RECT rc;
				if(m_FilterGraph->GetVideoRect(&rc)) {
					save_height = rc.bottom - rc.top;
					save_width = rc.right - rc.left;
					save_x = rc.left;
					save_y = rc.top;
				}
			}
			if(!m_ontop)
				::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,save_x,save_y ,save_width,save_height,SWP_SHOWWINDOW);
			else
				::SetWindowPos(this->m_hWnd,HWND_TOPMOST,save_x,save_y ,save_width,save_height,SWP_SHOWWINDOW);
			m_SliderGraph.ShowWindow(SW_SHOW);
			KillTimer(TIMER_HIDE_CURSOR);
			ShowCursor(TRUE);
			while (ShowCursor(1) < 0)  ;
			//m_FilterGraph->SetDisplayWindow(this->GetSafeHwnd());
		}
	}
}

void CDShowPlayerDlg::Seek(double len)
{
	if(m_FilterGraph) {
		double pos;
		m_FilterGraph->GetCurrentPosition(&pos);
		m_FilterGraph->SetCurrentPosition(pos + len);
	}
}

BOOL CDShowPlayerDlg::PreTranslateMessage(MSG* pMsg)
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

							if(m_FilterGraph && !m_FilterGraph->IsPaused())
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
		if(m_FilterGraph) {
			if(m_FilterGraph->IsRunning())
				m_FilterGraph->Pause();
		}
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
				OnFullScreen();
			break;
		case   WM_KEYDOWN:
			SendMessage(CMD_KEYDOWN, CMD_KEY, pMsg->wParam );
			if(pMsg->wParam == 13)
				return TRUE;
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
					m_SliderGraph.MoveWindow(0,m_scr_height - 16,m_scr_width,20);
					m_showctrl = true;
				}
				else if(m_showctrl && point.y < m_scr_height - 20)
				{
					m_SliderGraph.MoveWindow(0,m_scr_height,m_scr_width,20);
					m_showctrl = false;
				}
				break;
			}
		}
	}
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CDShowPlayerDlg::OnIdle()
{
	return FALSE;
}

LRESULT CDShowPlayerDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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
		m_popmenu.LoadMenu(IDR_MENU_DSHOW_EN);
	else if(m_applang == 3 || m_applang == 4)
		m_popmenu.LoadMenu(IDR_MENU_DSHOW_TC);
	else
		m_popmenu.LoadMenu(IDR_MENU_DSHOW);

	s_uTBBC = RegisterWindowMessage(L"TaskbarButtonCreated");
	HINSTANCE user32 = GetModuleHandle(L"user32.dll");
	if(user32) ChangeWindowMessageFilterDLL = (ChangeWindowMessageFilterFunction)GetProcAddress(user32, "ChangeWindowMessageFilter");
	if(ChangeWindowMessageFilterDLL) {
		ChangeWindowMessageFilterDLL(s_uTBBC, MSGFLT_ADD);
		ChangeWindowMessageFilterDLL(WM_COMMAND, MSGFLT_ADD);
	}

	m_SliderGraph.Attach(GetDlgItem(IDC_SLIDER_CTRL));

	m_SliderGraph.SetRange(0,1000);
	m_SliderGraph.SetPageSize(25);
	m_SliderGraph.SetLineSize(5);
	m_SliderGraph.SetPos(0);
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	m_inited = true;

	if(m_ontop)
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	
	DoDataExchange();

	LoadConfig();

	if(m_filename.GetLength() > 3)
		CreateGraph(m_filename);
	
	return TRUE;
}

void CDShowPlayerDlg::CreateGraph(const CString& strSourceFile)
{
	DestroyGraph();
	m_FilterGraph = new CDXGraph();
	if (m_FilterGraph->Create()) {
		// Render the source clip
		//const GUID RGB_FILTER = 
		//{0x8B498501, 0x1218, 0x11CF, {0xad, 0xc4, 0x00,0xa0, 0xd1, 0x00, 0x04, 0x1b}};
		//		m_FilterGraph->AddFilters(RGB_FILTER);

		m_FilterGraph->RenderFile(strSourceFile);
		m_FilterGraph->QueryInterfaces();
		// Set video window and notification window
		m_FilterGraph->SetNotifyWindow(m_hWnd);
		m_FilterGraph->SetDisplayWindow(m_hWnd);//m_VideoWindow
		// Show the first frame
		//		m_FilterGraph->GetFrameStepInterface();
		m_FilterGraph->Run();
		SetWindowText(_T("DShowPlayer - ") + m_filename);
		SetTimer(0, 200, 0);
		TimerRuning = true;
		SetForegroundWindow(m_hWnd);
	}
}



LRESULT CDShowPlayerDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	DestroyGraph();
	return 0;
}

void CDShowPlayerDlg::IninFileName(CString filename)
{
	filename.TrimLeft(_T(" "));
	filename.TrimRight(_T(" "));
	filename.TrimLeft(_T("\""));
	filename.TrimRight(_T("\""));
	if(filename.GetLength() > 4)
		m_filename = filename;
}


LRESULT CDShowPlayerDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//UINT nType = (UINT)wParam;
	CSize size = _WTYPES_NS::CSize(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));

	if(m_FilterGraph) {
		if(!m_fs)
			m_FilterGraph->ResizeVideoWindow(0,0,size.cx,size.cy - SLIDERHEIGHT);
		else
			m_FilterGraph->ResizeVideoWindow(0,0,size.cx,size.cy);
	}
	if(m_inited)
		m_SliderGraph.MoveWindow(0,size.cy - SLIDERHEIGHT,size.cx,SLIDERHEIGHT);
	return 0;
}

void CDShowPlayerDlg::DestroyGraph(void)
{
	if(m_FilterGraph) {
		// Stop the filter graph first
		m_FilterGraph->Stop();
		m_FilterGraph->SetNotifyWindow(NULL);

		m_SliderGraph.SetPos(0);
		if(g_pTaskbarList) g_pTaskbarList->SetProgressValue(this->m_hWnd, 0, 100);

		delete m_FilterGraph;
		m_FilterGraph = NULL;
		KillTimer(0);
		TimerRuning = false;
		save_height = 0;
		save_width = 0;
		save_x = 0;
		save_y = 0;
	}
}

void CDShowPlayerDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CDShowPlayerDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CDShowPlayerDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CDShowPlayerDlg::OnDShowOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);

	TCHAR strBuffer[MAX_PATH * 2]  = {0};
	CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY, _T("All Support File\0*.avi;*.mkv;*.ogm;*.mp4;*.m4v;*.m4p;*.m4b;\
*.flv;*.vp6;*.divx;*.vg2;*.dat;*.mpg;*.mpeg;*.tp;*.ts;*.tpr;*.pva;*.pps;*.mpe;*.vob;*.rm;*.rmvb;\
*.wmv;*.asf;*.wmp;*.wm;*.mov;*.qt;*.3gp;*.3gpp;*.3g2;*.3gp2;*.mp3;*.mp2;*.m4a;*.aac;*.mpc;*.wma;*.ogg;\
*.arm;*.mka;*.flac;*.ac3;*.dts;*.wav;*.ra;*.aif\0All File(*.*)\0*.*\0"));

	dlg.m_ofn.lpstrFile = strBuffer;
	dlg.m_ofn.nMaxFile = MAX_PATH * 2;
	if(dlg.DoModal()==IDOK) {
		m_filename = dlg.m_ofn.lpstrFile;
		CreateGraph(m_filename);
	}
	::SetCurrentDirectory(szFilePath);
	DoDataExchange(FALSE);

	return 0;
}

LRESULT CDShowPlayerDlg::OnDSPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_FilterGraph) {
		if(m_FilterGraph->IsPaused() || m_FilterGraph->IsStopped())
			m_FilterGraph->Run();
		else
			m_FilterGraph->Pause();
		if(g_pTaskbarList) g_pTaskbarList->SetProgressState(m_hWnd, TBPF_NORMAL);
	}
	return 0;
}

LRESULT CDShowPlayerDlg::OnDSStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_FilterGraph) {
		if(m_FilterGraph->IsStopped())
			return 0;
		else {
			m_FilterGraph->SetCurrentPosition(0);
			m_SliderGraph.SetPos(0);
			m_FilterGraph->Stop();
			SetWindowText(_T("MDSPlayer"));
		}
		if(g_pTaskbarList) g_pTaskbarList->SetProgressState(m_hWnd, TBPF_NOPROGRESS);
	}
	return 0;
}

void CDShowPlayerDlg::LoadConfig()
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

LRESULT CDShowPlayerDlg::OnCmdKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam != CMD_KEY)
		return FALSE;
	if(m_keydowning)
		return  FALSE;
	m_keydowning = true;
	CString cmd, value;
	if(GetKeyCommand( lParam, cmd, value))
	{
		if( cmd == _T("quit"))
			CloseDialog(0);
		else if( cmd == _T("pause")) {
			if (m_FilterGraph) {
				if(m_FilterGraph->IsPaused() || m_FilterGraph->IsStopped())
					m_FilterGraph->Run();
				else
					m_FilterGraph->Pause();
				if(g_pTaskbarList) g_pTaskbarList->SetProgressState(m_hWnd, TBPF_NORMAL);
			}
		}
		else if( cmd == _T("vo_fullscreen"))
			OnFullScreen();
		else if( cmd == _T("vo_ontop")) {
			if(! m_ontop) {
				::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
				m_ontop = true;
			} else {
				::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
				m_ontop = false;
			}
		} else if( cmd == _T("seek")) {
			int val = _ttoi(value) * 1000;
			Seek(val);
		}
	}
	m_keydowning = false;
	return TRUE;
}
LRESULT CDShowPlayerDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UINT_PTR nIDEvent = (UINT_PTR)wParam;
	if(nIDEvent == TIMER_HIDE_CURSOR) {
		while(ShowCursor(FALSE)>=0);
		KillTimer(TIMER_HIDE_CURSOR);
	} else {
		if(m_FilterGraph && !m_changing) {
			if(!m_FilterGraph->IsRunning())
				return 0;
			double pos,length;
			m_FilterGraph->GetDuration(&length);
			m_FilterGraph->GetCurrentPosition(&pos);
			// Get the new position, and update the slider
			int newPos = (int)(pos * 1000.0 / length);
			if (m_SliderGraph.GetPos() != newPos) {
				m_SliderGraph.SetPos(newPos);
				if(g_pTaskbarList) g_pTaskbarList->SetProgressValue(this->m_hWnd, newPos/10, 100);
			}
			if(pos >= length) {
				m_FilterGraph->SetCurrentPosition(0);
				m_SliderGraph.SetPos(0);
				if(g_pTaskbarList) g_pTaskbarList->SetProgressValue(this->m_hWnd, 0, 100);
			}
		}
	}
	return 0;
}
bool CDShowPlayerDlg::GetKeyCommand(LONG_PTR KeyNumber,CString &cmd, CString &value)
{
	CString key, abs;
	for(int i = 0 ; i < m_inputs.GetSize() ; i++) {
		m_inputs.GetAt(i, key ,cmd, value, abs);
		if( KeyNumber == GetKeyNumber(key) )
			return true;
	}
	return false;
}

int CDShowPlayerDlg::GetKeyNumber(CString Key)
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

bool CDShowPlayerDlg::AnalyseLine(CString line, CString &key, CString &cmd, CString &val)
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

LRESULT CDShowPlayerDlg::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int iFileNumber;
	CString filename;
	TCHAR szFileName[MAX_PATH*2];
	iFileNumber = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);

	if (iFileNumber > 0) 
	{
		DragQueryFile((HDROP)wParam, 0, szFileName, MAX_PATH*2);
		m_filename.Format(_T("%s") , szFileName);
		CreateGraph(m_filename);
	}
	return 0;
}
LRESULT CDShowPlayerDlg::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDCHandle dc = (HDC)wParam;
	if(m_FilterGraph){
		if(!m_fs)
			return TRUE;

		CRect rect;
		CPaintDC pdc(m_hWnd);
		GetClientRect(&rect);
		pdc.FillSolidRect(rect, RGB(0,0,0));
		//RECT rc;
		//m_FilterGraph->GetVideoRect(&rc);
		//CBrush backBrush(RGB(0, 0, 0));          //改变后的背景色
		//HBRUSH pOldBrush = SelectBrush(dc, backBrush);
		//CRect rcClip;
		//GetClientRect(&rcClip);//Paint the area.
		//dc.PatBlt(rcClip.left , rcClip.top ,rc.left - rcClip.left, rcClip.Height() ,PATCOPY);
		//dc.PatBlt(rcClip.left , rcClip.top ,rcClip.Width() , rc.top - rcClip.top ,PATCOPY);
		//if(m_SliderGraph.IsWindowVisible())
		//	dc.PatBlt(rcClip.left , rc.bottom ,rcClip.Width(), rcClip.Height() - rc.bottom - SLIDERHEIGHT,PATCOPY);
		//else
		//	dc.PatBlt(rcClip.left , rc.bottom ,rcClip.Width(), rcClip.Height() - rc.bottom,PATCOPY);
		//dc.PatBlt(rc.right, rcClip.top ,rcClip.Width() - rc.right, rcClip.Height() ,PATCOPY);
		//SelectBrush(dc, pOldBrush);
		return TRUE;
	}
	return 0;
}

LRESULT CDShowPlayerDlg::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDCHandle dc = (HDC)wParam;
	if(m_FilterGraph)
		return 0;

	CRect rect;
	CPaintDC pdc(m_hWnd);
	GetClientRect(&rect);
	pdc.FillSolidRect(rect, RGB(0,0,0));
	return 0;
}

LRESULT CDShowPlayerDlg::OnDSFS(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnFullScreen();
	return 0;
}

LRESULT CDShowPlayerDlg::OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
			m_SliderGraph.GetClientRect(&rc);
			pos = pos * 1000 / (rc.Width()-10);
			if(g_pTaskbarList) g_pTaskbarList->SetProgressValue(this->m_hWnd, pos/10, 100);
			m_SliderGraph.SetPos(pos);
		}
	}

	return 0;
}

LRESULT CDShowPlayerDlg::OnNmReleasedcaptureSlider(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if(m_FilterGraph) {
		double length = 0;
		m_FilterGraph->GetDuration(&length);
		double newPos = (double)m_SliderGraph.GetPos() * (length / 1000.0);
		m_FilterGraph->SetCurrentPosition( newPos);
	}
	m_changing = false;
	return 0;
}

