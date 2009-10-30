// MFlashPlayer.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MFlashPlayer.h"
#include "MPlayList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef BOOL (__stdcall *ChangeWindowMessageFilterTp)(UINT, DWORD);
static ChangeWindowMessageFilterTp ChangeWindowMessageFilterDLL = NULL;

/////////////////////////////////////////////////////////////////////////////
// CMFlashPlayer dialog

#define CMD_KEYDOWN     WM_USER + 1111
#define CMD_KEY			WM_USER + 1568
#define IDTB_PLAY_PAUSE	WM_USER + 2068
#define TIMER_HIDE_CURSOR 101

CMFlashPlayer::CMFlashPlayer(CWnd* pParent /*=NULL*/)
	: CDialog(CMFlashPlayer::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SWF);
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
	m_menu = NULL;
	g_pTaskbarList = NULL;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
	//{{AFX_DATA_INIT(CMFlashPlayer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMFlashPlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMFlashPlayer)
	DDX_Control(pDX, IDC_SLIDER_CONTROL, m_control);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH, m_flash);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMFlashPlayer, CDialog)
	//{{AFX_MSG_MAP(CMFlashPlayer)
	ON_WM_SIZE()
	ON_COMMAND(ID_FLASH_OPEN, OnFlashOpen)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CONTROL, OnReleasedcaptureSliderControl)
	ON_COMMAND(ID_MPLAYLIST, OnMplaylist)
	ON_COMMAND(ID_MPLAY, OnPlay)
	ON_COMMAND(ID_MSTOP, OnStop)
	ON_COMMAND(ID_MNEXT, OnNext)
	ON_COMMAND(ID_MPRE, OnPre)
	ON_COMMAND(IDM_RMENU, OnRmenu)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(PLS_SELCHANGE,  OnSelChange)
	ON_MESSAGE(CMD_KEYDOWN,  OnCmdKeyDown)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFlashPlayer message handlers

void CMFlashPlayer::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	// Do not call CDialog::OnPaint() for painting messages
}

HCURSOR CMFlashPlayer::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CMFlashPlayer, CDialog)
    //{{AFX_EVENTSINK_MAP(CMFlashPlayer)
	ON_EVENT(CMFlashPlayer, IDC_SHOCKWAVEFLASH, 1958 /* OnProgress */, OnProgressflash, VTS_I4)
	ON_EVENT(CMFlashPlayer, IDC_SHOCKWAVEFLASH, 197 /* FlashCall */, OnFlashCall, VTS_BSTR)
	ON_EVENT(CMFlashPlayer, IDC_SHOCKWAVEFLASH, -609 /* OnReadyStateChange */, OnOnReadyStateChange, VTS_I4)
	ON_EVENT(CMFlashPlayer, IDC_SHOCKWAVEFLASH, 150 /* FSCommand */, OnFSCommand, VTS_BSTR VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMFlashPlayer::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(m_inited)
	{
		if(!m_fs)
		{
			m_flash.MoveWindow(0,0,cx,cy - 20,FALSE);
			m_control.MoveWindow(0,cy - 20,cx,20);
		}
		else
		{
			m_flash.MoveWindow(0,0,cx,cy,FALSE);
			m_control.MoveWindow(0,cy,cx,20);
		}
	}
}

BOOL CMFlashPlayer::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	s_uTBBC = RegisterWindowMessage(L"TaskbarButtonCreated");
	HINSTANCE user32 = GetModuleHandle(L"user32.dll");
	if(user32) ChangeWindowMessageFilterDLL = (ChangeWindowMessageFilterTp)GetProcAddress(user32, "ChangeWindowMessageFilter");
	if(ChangeWindowMessageFilterDLL) {
		ChangeWindowMessageFilterDLL(s_uTBBC, MSGFLT_ADD);
		ChangeWindowMessageFilterDLL(WM_COMMAND, MSGFLT_ADD);
	}

	m_control.SetRange(0,100);
	m_control.SetPageSize(5);
	m_inited = true;
	m_list = new CMPlayList(this);
	m_list->m_playlist = &m_playlist;
	m_list->Create(IDD_DIALOG_LIST,NULL);
	SetTimer(0,300,0);
	LoadConfig();
	m_menu = this->GetMenu();
	if(m_ontop)
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	m_flash.SetLoop(FALSE);
	if(m_playlist.GetSize() > 0)
	{
		m_current = 0;
		PlayFile();
		if(m_fs_init)
			FullScreen(true);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMFlashPlayer::IninFileName(CString filename)
{
	filename.TrimLeft(_T(" "));
	filename.TrimRight(_T(" "));
	filename.TrimLeft(_T("\""));
	filename.TrimRight(_T("\""));
	if(filename.GetLength() > 4)
		m_playlist.Add(filename);
}

void CMFlashPlayer::OnFlashOpen() 
{
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		,ResStr(IDS_ASSOS_FLASH) + _T("(*.swf)|*.swf|") + ResStr(IDS_OTHER_ALL) + _T("(*.*)|*.*||"));

	dlg.m_ofn.lpstrTitle= ResStr(IDS_ASSOS_OP) + ResStr(IDS_ASSOS_FLASH);
    if(dlg.DoModal()==IDOK)
	{
		m_playlist.RemoveAll();
		m_playlist.Add(dlg.GetPathName());
		m_current = 0;
		PlayFile();
		if(m_fs_init)
			FullScreen();
	}
	dlg.DestroyWindow();
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}

void CMFlashPlayer::OnDropFiles(HDROP hDropInfo)
{
    TCHAR szFileName[MAX_PATH];
    int iFileNumber;
    // 得到拖拽操作中的文件个数
    iFileNumber = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
    
	m_playlist.RemoveAll();
    for (int i = 0; i < iFileNumber; i++) 
    {
        DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
		m_playlist.Add(szFileName);
    }
	m_current = 0;
    PlayFile();
	if(m_fs_init)
		FullScreen();
	CDialog::OnDropFiles(hDropInfo);
}

void CMFlashPlayer::OnTimer(UINT_PTR nIDEvent) 
{
	if(nIDEvent == TIMER_HIDE_CURSOR)
	{
			while(ShowCursor(FALSE)>=0);
			KillTimer(TIMER_HIDE_CURSOR);
	}
	else if(m_flash.GetPlaying())
	{
		long m_fnow = m_flash.CurrentFrame();
		if(! m_changing) {
			int pos = (int)((double)m_fnow / (double)m_fnumber * 100.0);
			m_control.SetPos(pos);
			if(g_pTaskbarList) g_pTaskbarList->SetProgressValue(this->m_hWnd, pos, 100);
		}
	}
	else if(now_state == 4)
	{
		long m_fnow = m_flash.CurrentFrame();
		if(m_fnow >= m_fnumber - 3)
			OnNext();
	}
	CDialog::OnTimer(nIDEvent);
}

void CMFlashPlayer::OnReleasedcaptureSliderControl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	bool playing = false;
	if(m_flash.IsPlaying())
		playing = true;
	m_changing = true;
	int pos = m_control.GetPos();
	m_flash.GotoFrame(m_fnumber * pos / 100);
	m_changing = false;
	if(playing)
		m_flash.Play();

	*pResult = 0;
}

void CMFlashPlayer::PlayFile(bool init)
{
	if(m_playlist.GetSize() <= m_current)
		return;
	m_flash.SetMovie(m_playlist[m_current]);
	m_fnumber = m_flash.GetTotalFrames();
	if(m_fnumber <= 0)
		m_fnumber = 1;
	m_flash.Play();
	m_control.SetPos(0);
}

void CMFlashPlayer::OnMplaylist() 
{
	m_list->ReFlashList();
	m_list->ShowWindow(SW_SHOW);
}

void CMFlashPlayer::OnPlay()
{
	m_flash.Play();
	if(g_pTaskbarList) g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_NORMAL);
}

void CMFlashPlayer::OnStop()
{
	m_flash.Stop();
	if(g_pTaskbarList) g_pTaskbarList->SetProgressState(this->m_hWnd, TBPF_NOPROGRESS);
}

void CMFlashPlayer::OnNext() 
{
	if(m_playlist.GetSize() <= 1)
		return;
	if( m_current < m_playlist.GetSize() - 1)
		m_current++;
	else
		m_current = 0;
	PlayFile();
}

void CMFlashPlayer::OnPre()
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

LRESULT CMFlashPlayer::OnSelChange(WPARAM wParam, LPARAM lParam)
{
	if(wParam >= 0)
	{
		m_current = wParam;
		PlayFile();
	}
	return TRUE;
}

LRESULT CMFlashPlayer::OnCmdKeyDown(WPARAM wParam, LPARAM lParam)
{
	if(wParam != CMD_KEY)
		return FALSE;
	if(m_keydowning)
		return  FALSE;
	m_keydowning = true;
	CString cmd, value;
	if(GetKeyCommand(lParam, cmd, value))
	{
		if( cmd == _T("quit"))
		{
			m_flash.Stop();
			OnOK();
		}
		else if( cmd == _T("pause"))
		{
			if(m_flash.IsPlaying())
				m_flash.Stop();
			else
				m_flash.Play();
		}
		else if( cmd == _T("vo_fullscreen"))
		{
			FullScreen();
		}
		else if( cmd == _T("vo_ontop"))
		{
			if(! m_ontop)
			{
				::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
				m_ontop = true;
			}
			else
			{
				::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
				m_ontop = false;
			}
		}
		else if( cmd == _T("keep_aspect"))
		{
			long type = m_flash.GetScaleMode();
			if(type == 0)
				m_flash.SetScaleMode(1);
			else if(type == 1)
				m_flash.SetScaleMode(2);
			else if(type == 2)
				m_flash.SetScaleMode(3);
			else if(type == 3)
				m_flash.SetScaleMode(4);
			else
				m_flash.SetScaleMode(0);
		}
		else if( cmd == _T("seek"))
		{
			int val = _ttoi(value);
			long m_fnow = m_flash.CurrentFrame();
			long seek = m_fnow + (val * 5);
			if(seek > 0 && seek < m_fnumber)
			{
				m_flash.GotoFrame(seek);
				m_flash.Play();
			}
			else if (seek <= 0)
			{
				m_flash.GotoFrame(0);
				m_flash.Play();
			}
			else if (seek >= m_fnumber)
			{
				m_flash.GotoFrame(m_fnumber);
				m_flash.Play();
			}
		}
		else if( cmd == _T("pt_step"))
		{
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

void CMFlashPlayer::OnProgressflash(long percentDone) 
{
}

void CMFlashPlayer::OnFlashCall(LPCTSTR request) 
{

}

void CMFlashPlayer::OnOnReadyStateChange(long newState) 
{
	now_state = newState;
}

void CMFlashPlayer::OnFSCommand(LPCTSTR command, LPCTSTR args) 
{
}

bool CMFlashPlayer::GetKeyCommand(LONG_PTR KeyNumber,CString &cmd, CString &value)
{
	CString key, abs;
	for(int i = 0 ; i < m_inputs.GetSize() ; i++)
	{
		m_inputs.GetAt(i, key ,cmd, value, abs);
		if( KeyNumber == GetKeyNumber(key) )
			return true;
	}
	return false;
}

int CMFlashPlayer::GetKeyNumber(CString Key)
{
	if(Key.GetLength() == 1)
	{
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

void CMFlashPlayer::LoadConfig()
{
	CStdioFile playercfg;
	if(playercfg.Open(m_program_dir + _T("mplayer.ini"),CFile::modeRead))
	{
		CString line;
		while(playercfg.ReadString(line))
		{
			line = LocalToUnicode(line);
			line.TrimLeft(_T(" "));
			line.TrimRight(_T(" "));
			line.MakeLower();
			if(line.Find(_T("#")) == 0)
				continue;
			if(line.Find(_T("fs=yes")) == 0)
			{
				m_fs_init = true;
				continue;
			}
			if(line.Find(_T("fs=1")) == 0)
			{
				m_fs_init = true;
				continue;
			}
			if(line.Find(_T("ontop=2")) == 0)
			{
				m_ontop = true;
				continue;
			}
			if(line.Find(_T("ontop=1")) == 0)
			{
				m_ontop = true;
				continue;
			}
		}
		playercfg.Close();
	}
	CStdioFile inputcfg;
	if(inputcfg.Open(m_program_dir + _T("input.ini"),CFile::modeRead))
	{
		CString line,command,value,key;
		while(inputcfg.ReadString(line))
		{
			line = LocalToUnicode(line);
			if(AnalyseLine(line,key,command,value))
				m_inputs.Add(key,command,value);
		}
		inputcfg.Close();
	}
	else
	{
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

bool CMFlashPlayer::AnalyseLine(CString line, CString &key, CString &cmd, CString &val)
{
	line += _T(" ");
	line.TrimLeft(_T(" "));
	int sp = -1;
	if(line.Find(_T("#")) == 0)
	{
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

HRESULT CMFlashPlayer::UpdateThumbnailToolbar(HWND hWnd)
{
	HRESULT hr = NULL;
	if (g_pTaskbarList)
	{
		THUMBBUTTON buttons[2] = {};

		buttons[0].dwMask = THB_BITMAP | THB_FLAGS;
		buttons[0].dwFlags = THBF_ENABLED;
		buttons[0].iId = IDTB_PLAY_PAUSE;

		if(m_flash.IsPlaying())
			buttons[0].iBitmap = 1;
		else
			buttons[0].iBitmap = 0;

		// Set the buttons to be the thumbnail toolbar
		hr = g_pTaskbarList->ThumbBarUpdateButtons(hWnd, 1, buttons);
	}

	return hr;
}

BOOL CMFlashPlayer::PreTranslateMessage(MSG* pMsg) 
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

							if(m_flash.IsPlaying())
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
		if(m_flash.IsPlaying())
			m_flash.Stop();
		else
			m_flash.Play();
		UpdateThumbnailToolbar(this->m_hWnd);
	} else {
		switch(pMsg->message)
		{
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
				m_flash.GotoFrame((long)(m_fnumber * pos));
				m_flash.Play();
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CMFlashPlayer::OnRmenu() 
{
	m_rmenu = !m_rmenu;
	if(m_menu)
	{
		if(m_rmenu)
			m_menu->CheckMenuItem(IDM_RMENU,MF_CHECKED);
		else
			m_menu->CheckMenuItem(IDM_RMENU,MF_UNCHECKED);
	}
}

void CMFlashPlayer::FullScreen(bool init)
{
	if(!m_fs)
	{
		m_fs = true;
		m_showctrl = false;
		RECT rc;
		this->GetWindowRect(&rc);
		save_x = rc.left;
		save_y = rc.top;
		save_width = rc.right - rc.left;
		save_height = rc.bottom - rc.top;
		m_scr_width = GetSystemMetrics(SM_CXSCREEN);
		m_scr_height = GetSystemMetrics(SM_CYSCREEN);
		if(init)
		{
			save_x = (m_scr_width - save_width) / 2;
			save_y = (m_scr_height - save_height) / 2;
		}
		this->SetMenu(NULL);
		this->ModifyStyle(WS_CAPTION | WS_THICKFRAME,WS_OVERLAPPED);
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,m_scr_width,m_scr_height,SWP_SHOWWINDOW);
		while (ShowCursor(0) >= 0)  ;
	}
	else
	{
		m_fs = false;
		m_showctrl = true;
		this->SetMenu(m_menu);
		this->ModifyStyle(WS_OVERLAPPED,WS_CAPTION | WS_THICKFRAME);
		if(!m_ontop)
			::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,save_x,save_y
				,save_width,save_height,SWP_SHOWWINDOW);
		else
			::SetWindowPos(this->m_hWnd,HWND_TOPMOST,save_x,save_y
				,save_width,save_height,SWP_SHOWWINDOW);
		KillTimer(TIMER_HIDE_CURSOR);
		ShowCursor(TRUE);
		while (ShowCursor(1) < 0)  ;
	}
}

void CMFlashPlayer::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);
}


void CMFlashPlayer::OnDestroy()
{
	CDialog::OnDestroy();

	if (g_pTaskbarList)
	{
		g_pTaskbarList->Release();
		g_pTaskbarList = NULL;
	}

}
