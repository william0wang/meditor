// MMedaiPlayer.cpp : implementation file
//

#include "stdafx.h"
#include "meditor2.h"
#include "MMediaPlayer.h"
#include "MMediaList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
	
#define WMP_KEYDOWN     WM_USER + 1115
#define WMP_KEY     WM_USER + 1528
#define TIMER_HIDE_CURSOR 104

/////////////////////////////////////////////////////////////////////////////
// CMMediaPlayer dialog


CMMediaPlayer::CMMediaPlayer(CWnd* pParent /*=NULL*/)
	: CDialog(CMMediaPlayer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMMediaPlayer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_filename = _T("");
	m_inited = false;
	m_keydowning = false;
	m_ontop = false;
	m_fs = false;
	m_fs_init = false;
	m_rmenu = false;
	m_changing = false;
	m_showctrl = true;
	m_status = -1;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
}


void CMMediaPlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMMediaPlayer)
	DDX_Control(pDX, IDC_SLIDER_CTRL, m_control);
	DDX_Control(pDX, IDC_OCX_MEDIA, m_wmp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMMediaPlayer, CDialog)
	//{{AFX_MSG_MAP(CMMediaPlayer)
	ON_WM_SIZE()
	ON_COMMAND(ID_MEDIA_OPEN, OnMediaOpen)
	ON_COMMAND(IDM_PLAY, OnPlay)
	ON_COMMAND(IDM_STOP, OnStop)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_PREV, OnPrev)
	ON_COMMAND(IDM_NEXT, OnNext)
	ON_COMMAND(IDM_FULLSCREEN, OnFullscreen)
	ON_COMMAND(IDM_RIGHTMENU, OnRightmenu)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CTRL, OnReleasedcapture)
	ON_COMMAND(IDM_PLAYLIST, OnPlaylist)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WMP_KEYDOWN,  OnCmdKeyDown)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMMediaPlayer message handlers

void CMMediaPlayer::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(m_inited)
	{
		if(!m_fs)
		{
			m_wmp.MoveWindow(0,0,cx,cy - 20,FALSE);
			m_control.MoveWindow(0,cy - 20,cx,20);
		}
		else
		{
			m_wmp.MoveWindow(0,0,cx,cy,FALSE);
			m_control.MoveWindow(0,cy,cx,20);
		}
	}
	// TODO: Add your message handler code here
	
}

void CMMediaPlayer::IninFileName(CString filename)
{
	filename.TrimLeft(_T(" "));
	filename.TrimRight(_T(" "));
	filename.TrimLeft(_T("\""));
	filename.TrimRight(_T("\""));
	if(filename.GetLength() > 4)
		m_filename = filename;
}

HCURSOR CMMediaPlayer::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMMediaPlayer::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
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


BOOL CMMediaPlayer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: Add extra initialization here	
	m_list = new CMMediaList(this);
	m_list->m_wmp = &m_wmp;
	m_list->Create(IDD_DIALOG_MLIST,NULL);
	m_wmpctrl = m_wmp.GetControls();
	m_wmpset = m_wmp.GetSettings();
	LoadConfig();
	m_menu = this->GetMenu();
	m_inited = true;
//	if(!m_fs)
//		m_wmp.SetFullScreen(FALSE);
	if(m_ontop)	
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	if(m_filename.GetLength() > 4)
	{
		m_wmp.SetUrl(m_filename);
		m_wmpctrl.play();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMMediaPlayer::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)
	{
	case   WM_KEYDOWN:
		SendMessage(WMP_KEYDOWN, WMP_KEY, pMsg->wParam );
		return TRUE;
	case   WM_LBUTTONDOWN:
		POINT point;
		GetCursorPos(&point);
		RECT rc;
		this->GetWindowRect(&rc);
		if(point.y < rc.bottom - 20)
		{
			SendMessage(WMP_KEYDOWN, WMP_KEY, WM_LBUTTONDOWN );
			return TRUE;
		}
		else if(m_fs && m_showctrl)
		{
			if(point.y < m_scr_height - 16)
				break;
			if(point.x < 5 || point.x > m_scr_width - 5)
				break;
			m_changing = true;
			double pos = (double)(point.x - 5) / (double) (m_scr_width - 10);
			m_wmpctrl.SetCurrentPosition(m_length * pos);
			m_changing = false;
			return TRUE;
		}
		break;
	case   WM_LBUTTONDBLCLK:
		SendMessage(WMP_KEYDOWN, WMP_KEY, WM_LBUTTONDBLCLK );
		return TRUE;
	case   WM_RBUTTONDOWN:
		if(!m_rmenu)
		{
			SendMessage(WMP_KEYDOWN, WMP_KEY, WM_RBUTTONDOWN );
			m_rmenu = false;
			if(m_wmp.GetEnableContextMenu())
				m_wmp.SetEnableContextMenu(FALSE);
			return TRUE;
		}
		else
		{
			m_rmenu = false;
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CMMediaPlayer::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR szFileName[MAX_PATH];
	int iFileNumber;
	iFileNumber = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
	CWMPMedia m_media;
	m_wmp.SetUrl(szFileName);
	m_wmplist = m_wmp.GetCurrentPlaylist();
	m_wmplist.SetName(_T("MMediaPlayer 播放列表"));
	for (int i = 1; i < iFileNumber; i++) 
	{
		DragQueryFile(hDropInfo, i, szFileName, MAX_PATH);
		m_media = m_wmp.newMedia(szFileName);
		m_wmplist.appendItem(m_media);
	}
	m_wmpctrl.play();
	CDialog::OnDropFiles(hDropInfo);
}

void CMMediaPlayer::OnMediaOpen() 
{
	// TODO: Add your command handler code here
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		,_T("常用视频格式|*.avi;*.mkv;*.ogm;*.mp4;*.m4v;*.m4p;*.m4b;*.flv;*.vp6;\
*.divx;*.vg2;*.dat;*.mpg;*.mpeg;*.tp;*.ts;*.tpr;*.pva;*.pps;*.mpe;*.vob;*.rm;*.rmvb;\
*.wmv;*.asf;*.wmp;*.wm;*.mov;*.qt;*.3gp;*.3gpp;*.3g2;*.3gp2\
|常用音频格式|*.mp3;*.mp2;*.m4a;*.aac;*.mpc;*.wma;*.ogg;\
*.arm;*.mka;*.flac;*.ac3;*.dts;*.wav;*.ra;*.aif|所有文件(*.*)|*.*||"));

	dlg.m_ofn.lpstrTitle=_T("打开媒体文件");
	if(dlg.DoModal()==IDOK)
	{
		m_wmp.SetUrl(dlg.GetPathName());
		m_wmplist = m_wmp.GetCurrentPlaylist();
		m_wmplist.SetName(_T("MMediaPlayer 播放列表"));
		m_wmpctrl.play();
	}
	dlg.DestroyWindow();
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
	
}

void CMMediaPlayer::OnPlay()
{
	// TODO: Add your command handler code here
	if(m_status == 3)
		m_wmpctrl.pause();
	else if (m_status == 2 || m_status == 1 )
		m_wmpctrl.play();
}

void CMMediaPlayer::OnStop()
{
	// TODO: Add your command handler code here
	m_wmpctrl.stop();
}

BEGIN_EVENTSINK_MAP(CMMediaPlayer, CDialog)
    //{{AFX_EVENTSINK_MAP(CMMediaPlayer)
	ON_EVENT(CMMediaPlayer, IDC_OCX_MEDIA, 5101 /* PlayStateChange */, OnPlayStateChange, VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMMediaPlayer::OnPlayStateChange(long NewState)
{
	// TODO: Add your control notification handler code here
	if(m_status == 9 && NewState == 3)
	{
		SetTimer(0,300,0);
		CWMPMedia m_media = m_wmp.GetCurrentMedia();
		long width = m_media.GetImageSourceWidth();
		long height = m_media.GetImageSourceHeight();
		m_length = m_media.GetDuration();
		if(width >= 100 && height >= 100)
		{
			RECT rc;
			GetWindowRect(&rc);
			int nheight = rc.bottom - rc.top;
			int nwidth = rc.right - rc.left;
			GetClientRect(&rc);
			int xwap = nwidth - rc.right + rc.left;
			int ywap = nheight - rc.bottom + rc.top;
			nwidth = width + xwap;
			nheight = height + ywap + 20;
			int swidth = GetSystemMetrics(SM_CXSCREEN);
			int sheight = GetSystemMetrics(SM_CYSCREEN);
			int x = (swidth - nwidth) / 2;
			int y = (sheight - nheight) / 2;
			MoveWindow(x,y,nwidth,nheight,TRUE);
		}
		if(m_fs_init)
			FullScreen();
	}
	m_status = NewState;
}

bool CMMediaPlayer::GetKeyCommand(LONG KeyNumber,CString &cmd, CString &value)
{
	CString key;
	for(int i = 0 ; i < m_inputs.GetSize() ; i++)
	{
		m_inputs.GetAt(i, key ,cmd, value);
		if( KeyNumber == GetKeyNumber(key) )
			return true;
	}
	return false;
}

int CMMediaPlayer::GetKeyNumber(CString Key)
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

	else if(Key == _T("MOUSE_BTN0"))
		return WM_LBUTTONDOWN;
	else if(Key == _T("MOUSE_BTN2"))
		return WM_RBUTTONDOWN;
	
	return -1;
}

void CMMediaPlayer::LoadConfig()
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
			if(line.Find(_T("fs=yes")) == 0 )
			{
				m_fs_init = true;
			}
			else if(line.Find(_T("fs=1")) == 0 )
			{
				m_fs_init = true;
				continue;
			}
			if(line.Find(_T("ontop=2")) == 0 )
			{
				m_ontop = true;
				continue;
			}
			else if(line.Find(_T("ontop=1")) == 0 )
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

bool CMMediaPlayer::AnalyseLine(CString line, CString &key, CString &cmd, CString &val)
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

LONG CMMediaPlayer::OnCmdKeyDown(UINT lParam, LONG wParam)
{
	if(lParam != WMP_KEY)
		return FALSE;
	if(m_keydowning)
		return  FALSE;
	m_keydowning = true;
	CString cmd, value;
	if(GetKeyCommand( wParam, cmd, value))
	{
		if( cmd == _T("quit"))
		{
			if(m_status > 1)
				m_wmpctrl.stop();
			OnOK();
		}
		else if( cmd == _T("pause"))
		{
			if(m_status == 3)
				m_wmpctrl.pause();
			else if (m_status == 2 || m_status == 1 )
				m_wmpctrl.play();
		}
		else if( cmd == _T("vo_ontop"))
		{
			if(m_wmp.GetFullScreen())
			{
				m_keydowning = false;
				return FALSE;
			}
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
		else if( cmd == _T("vo_fullscreen"))
		{
			if(m_status < 2 || m_status > 3)
			{
				m_keydowning = false;
				return FALSE;
			}
			FullScreen();
//			if(m_wmp.GetFullScreen())
//				m_wmp.SetFullScreen(FALSE);
//			else
//				m_wmp.SetFullScreen(TRUE);
		}
		else if( cmd == _T("keep_aspect"))
		{
			if(m_wmp.GetStretchToFit())
				m_wmp.SetStretchToFit(FALSE);
			else
				m_wmp.SetStretchToFit(TRUE);
		}
		else if( cmd == _T("mute"))
		{
			if(m_wmpset.GetMute())
				m_wmpset.SetMute(FALSE);
			else
				m_wmpset.SetMute(TRUE);
		}
		else if( cmd == _T("seek"))
		{
			if(m_status == 3)
			{
				int val = _ttoi(value);
				double pos = m_wmpctrl.GetCurrentPosition() + (double)val;
				m_wmpctrl.SetCurrentPosition(pos);
			}
		}
		else if( cmd == _T("pt_step"))
		{
			int val = _ttoi(value);
			if(val > 0)
				m_wmpctrl.next();
			else if(val < 0)
				m_wmpctrl.previous();
		}
		else if( cmd == _T("volume"))
		{
			int val = _ttoi(value);
			long vol = m_wmpset.GetVolume() + (long)val;
			m_wmpset.SetVolume(vol);
		}
	}
	m_keydowning = false;
	return TRUE;
}

void CMMediaPlayer::OnPrev() 
{
	// TODO: Add your command handler code here
	m_wmpctrl.next();
}

void CMMediaPlayer::OnNext() 
{
	// TODO: Add your command handler code here
	m_wmpctrl.previous();
}

void CMMediaPlayer::OnFullscreen() 
{
	// TODO: Add your command handler code here
	if(m_status < 2 || m_status > 3)
		return;
	if(m_wmp.GetFullScreen())
		m_wmp.SetFullScreen(FALSE);
	else
		m_wmp.SetFullScreen(TRUE);
}

void CMMediaPlayer::OnRightmenu() 
{
	// TODO: Add your command handler code here
	m_rmenu = !m_rmenu;
	m_wmp.SetEnableContextMenu(TRUE);
}


void CMMediaPlayer::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_HIDE_CURSOR)
	{
			while(ShowCursor(FALSE)>=0);
			KillTimer(TIMER_HIDE_CURSOR);
	}
	else if(m_status == 3 || m_status == 2)
	{
		double pos = m_wmpctrl.GetCurrentPosition();
		if(! m_changing)
			m_control.SetPos((int)(pos / m_length * 100.0));
	}
	else
	{
		m_control.SetPos(0);
		KillTimer(0);
	}
	CDialog::OnTimer(nIDEvent);
}

void CMMediaPlayer::OnReleasedcapture(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_changing = true;
	double pos = m_control.GetPos();
	m_wmpctrl.SetCurrentPosition( m_length * pos / 100);
	m_changing = false;
	*pResult = 0;
}

void CMMediaPlayer::FullScreen(bool init)
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

void CMMediaPlayer::OnPlaylist() 
{
	// TODO: Add your command handler code here
	m_list->ReFlashList();
	m_list->ShowWindow(SW_SHOW);
}

BOOL CMMediaPlayer::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ShowCursor(1);
	return CDialog::DestroyWindow();
}
