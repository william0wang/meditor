// MRealPlayer.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "MRealPlayer.h"

#define WM_KEYDOWN_REAL     WM_USER + 1615
#define WM_KEY_REAL     1728

// CMRealPlayer 对话框

IMPLEMENT_DYNAMIC(CMRealPlayer, CDialog)

CMRealPlayer::CMRealPlayer(CWnd* pParent /*=NULL*/)
	: CDialog(CMRealPlayer::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_filename = _T("");
	m_ontop = false;
	m_fs = false;
	m_fs_init = false;
	m_keydowning = false;
	m_first_play = false;
	m_rmenu = false;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
}

CMRealPlayer::~CMRealPlayer()
{
}

void CMRealPlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REAL_PLAYER, m_real);
}


BEGIN_MESSAGE_MAP(CMRealPlayer, CDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(WM_KEYDOWN_REAL,  OnCmdKeyDown)
	ON_COMMAND(ID_REAL_OPEN, OnRealOpen)
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_COMMAND(IDM_RPLAY, OnRplay)
	ON_COMMAND(IDM_RIGHTMENU, OnRightmenu)
	ON_COMMAND(IDM_FULLSCREEN, OnFullscreen)
	ON_COMMAND(IDM_RSTOP, OnRstop)
END_MESSAGE_MAP()


// CMRealPlayer 消息处理程序

void CMRealPlayer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

	// Center icon in client rectangle
	int cxIcon = GetSystemMetrics(SM_CXICON);
	int cyIcon = GetSystemMetrics(SM_CYICON);
	CRect rect;
	GetClientRect(&rect);
	int x = (rect.Width() - cxIcon + 1) / 2;
	int y = (rect.Height() - cyIcon + 1) / 2;

	// Draw the icon
	dc.DrawIcon(x, y, m_hIcon);
}

BOOL CMRealPlayer::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_real.SetVideoOverlay(FALSE);
	m_real.SetMaintainAspect(TRUE);
	LoadConfig();
	m_menu = this->GetMenu();
	if(m_ontop)	
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	m_first_play = true;
	if(m_filename.GetLength() > 4)
		m_real.SetSource(m_filename);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CMRealPlayer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(pMsg->message)
	{
	case   WM_KEYDOWN:
		SendMessage(WM_KEYDOWN_REAL, WM_KEY_REAL, pMsg->wParam );
		return TRUE;
		break;
	case  WM_RBUTTONDOWN:
		if(!m_rmenu)
		{
			SendMessage(WM_KEYDOWN_REAL, WM_KEY_REAL, WM_RBUTTONDOWN );
			return TRUE;
		}
		break;
	case  WM_RBUTTONUP:
		if(!m_rmenu)
			return TRUE;
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMRealPlayer::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	m_real.MoveWindow(0,0,cx,cy,FALSE);
}
BEGIN_EVENTSINK_MAP(CMRealPlayer, CDialog)
	ON_EVENT(CMRealPlayer, IDC_REAL_PLAYER, 1042, CMRealPlayer::OnStateChangeRealPlayer, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CMRealPlayer::OnStateChangeRealPlayer(long lOldState, long lNewState)
{
	if(lOldState == 2 && lNewState == 3)
	{
		long width = m_real.GetClipWidth();
		long height = m_real.GetClipHeight();
		if( width >= 200 && height >= 150)
		{
			long ControlPanelHeight = 75;
			ControlPanelHeight += (long)((double)(height - 120) / 6);

			height += ControlPanelHeight;
			int swidth = GetSystemMetrics(SM_CXSCREEN);
			int sheight = GetSystemMetrics(SM_CYSCREEN);
			int x = (swidth - width) / 2;
			int y = (sheight - height) / 2;
			MoveWindow(x,y,width,height,TRUE);
		}
		if(m_fs_init && m_first_play)
		{
			m_first_play = false;
			m_real.SetFullScreen();
		}
	}
}

void CMRealPlayer::OnRealOpen()
{
	// TODO: 在此添加命令处理程序代码
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
		m_first_play = true;
		m_real.SetSource(dlg.GetPathName());
	}
	dlg.DestroyWindow();
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}

void CMRealPlayer::IninFileName(CString filename)
{
	filename.TrimLeft(_T(" "));
	filename.TrimRight(_T(" "));
	filename.TrimLeft(_T("\""));
	filename.TrimRight(_T("\""));
	if(filename.GetLength() > 4)
		m_filename = filename;
}

HCURSOR CMRealPlayer::OnQueryDragIcon()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return (HCURSOR) m_hIcon;
	//return CDialog::OnQueryDragIcon();
}

void CMRealPlayer::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR szFileName[MAX_PATH];
	int iFileNumber;
	iFileNumber = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
	m_first_play = true;
	m_real.SetSource(szFileName);
	CDialog::OnDropFiles(hDropInfo);
}

bool CMRealPlayer::GetKeyCommand(LONG KeyNumber,CString &cmd, CString &value)
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

int CMRealPlayer::GetKeyNumber(CString Key)
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

void CMRealPlayer::LoadConfig()
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

bool CMRealPlayer::AnalyseLine(CString line, CString &key, CString &cmd, CString &val)
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


LONG CMRealPlayer::OnCmdKeyDown(UINT lParam, LONG wParam)
{
	if(lParam != WM_KEY_REAL)
		return FALSE;
	if(m_keydowning)
		return  FALSE;
	m_keydowning = true;
	CString cmd, value;
	if(GetKeyCommand( wParam, cmd, value))
	{
		if( cmd == _T("quit"))
		{
			m_real.DoStop();
			OnOK();
		}
		else if( cmd == _T("pause"))
		{
			m_real.DoPlayPause();
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
		else if( cmd == _T("vo_fullscreen"))
		{
			if(!m_real.GetFullScreen())
				m_real.SetFullScreen();
			else
				m_real.SetOriginalSize();
		}
		else if( cmd == _T("mute"))
		{
			if(m_real.GetMute())
				m_real.SetMute(FALSE);
			else
				m_real.SetMute(TRUE);
		}
		else if( cmd == _T("seek"))
		{
			int val = _ttoi(value);
			long pos = m_real.GetPosition() + (long)(val * 1000);
			m_real.SetPosition(pos);
		}
		else if( cmd == _T("volume"))
		{
			int val = _ttoi(value);
			short vol = m_real.GetVolume() + (short)val;
			m_real.SetVolume(vol);
		}
	}
	m_keydowning = false;
	return TRUE;
}

void CMRealPlayer::OnRplay()
{
	m_real.DoPlayPause();
}

void CMRealPlayer::OnRightmenu()
{
	m_rmenu = !m_rmenu;
	if(m_menu)
	{
		if(m_rmenu)
			m_menu->CheckMenuItem(IDM_RIGHTMENU,MF_CHECKED);
		else
			m_menu->CheckMenuItem(IDM_RIGHTMENU,MF_UNCHECKED);
	}
}

void CMRealPlayer::OnFullscreen()
{
		if(!m_real.GetFullScreen())
			m_real.SetFullScreen();
		else
			m_real.SetOriginalSize();
}

void CMRealPlayer::OnRstop()
{
	m_real.DoStop();
}
