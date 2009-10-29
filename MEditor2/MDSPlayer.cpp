// MDSPlayer.cpp : 实现文件
//

#include "stdafx.h"
#include "meditor2.h"
#include "MDSPlayer.h"

// CMDSPlayer 对话框

#define CMDS_KEYDOWN     WM_USER + 1131
#define CMDS_KEY     WM_USER + 1538
#define TIMER_HIDE_CURSOR 103

IMPLEMENT_DYNAMIC(CMDSPlayer, CDialog)

CMDSPlayer::CMDSPlayer(CWnd* pParent /*=NULL*/)
	: CDialog(CMDSPlayer::IDD, pParent)
{
	m_FilterGraph = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DS);
	m_keydowning = false;
	m_ontop = false;
	m_fs = false;
	m_fs_init = false;
	m_showctrl = true;
	m_menu = NULL;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
}

CMDSPlayer::~CMDSPlayer()
{
}

void CMDSPlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_CTRL, m_SliderGraph);
}


BEGIN_MESSAGE_MAP(CMDSPlayer, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_CTRL, &CMDSPlayer::OnNMReleasedcaptureSliderCtrl)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_DS_OPEN, &CMDSPlayer::OnDsOpen)
	ON_COMMAND(IDM_DSPLAY, &CMDSPlayer::OnDsPlay)
	ON_COMMAND(IDM_DSSTOP, &CMDSPlayer::OnDsStop)
	ON_COMMAND(IDM_DSFULLSCREEN, &CMDSPlayer::OnDsFullscreen)
	ON_MESSAGE(CMDS_KEYDOWN,  OnCmdKeyDown)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CMDSPlayer 消息处理程序

BOOL CMDSPlayer::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_SliderGraph.SetRange(0,1000);
	m_SliderGraph.SetPageSize(25);
	m_SliderGraph.SetLineSize(5);
	m_SliderGraph.SetPos(0);
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	LoadConfig();
	m_menu = this->GetMenu();
	if(m_ontop)
		::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	if(m_filename.GetLength() > 3)
	{
		CreateGraph(m_filename);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMDSPlayer::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

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
	else
	{
		if(m_FilterGraph)
		{
			CDialog::OnPaint();
			return;
		}
		CRect rect;
		CPaintDC dc(this);
		GetClientRect(rect);
		dc.FillSolidRect(rect,RGB(0,0,0));
		CDialog::OnPaint();
	}
}

BOOL CMDSPlayer::OnEraseBkgnd(CDC* pDC)
{

	if(m_FilterGraph)
	{
		if(!m_fs)
			return TRUE;
		RECT rc;
		m_FilterGraph->GetVideoRect(&rc);
		CBrush backBrush(RGB(0, 0, 0));          //改变后的背景色
		CBrush* pOldBrush = pDC->SelectObject(&backBrush);
		CRect rcClip;
		GetClientRect(&rcClip);//Paint the area.
		pDC->PatBlt(rcClip.left , rcClip.top ,rc.left - rcClip.left, rcClip.Height() ,PATCOPY);
		pDC->PatBlt(rcClip.left , rcClip.top ,rcClip.Width() , rc.top - rcClip.top ,PATCOPY);
		if(m_SliderGraph.IsWindowVisible())
			pDC->PatBlt(rcClip.left , rc.bottom ,rcClip.Width()
				, rcClip.Height() - rc.bottom - SLIDERHEIGHT,PATCOPY);
		else
			pDC->PatBlt(rcClip.left , rc.bottom ,rcClip.Width(), rcClip.Height() - rc.bottom,PATCOPY);
		pDC->PatBlt(rc.right, rcClip.top ,rcClip.Width() - rc.right, rcClip.Height() ,PATCOPY);
		pDC->SelectObject (pOldBrush);
		//pDC->ExcludeClipRect(&rc);
		return TRUE;
	}
	return CDialog::OnEraseBkgnd(pDC);
}

HCURSOR CMDSPlayer::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMDSPlayer::OnNMReleasedcaptureSliderCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_FilterGraph)
	{
		double length = 0;
		m_FilterGraph->GetDuration(&length);
		double newPos = (double)m_SliderGraph.GetPos() * (length / 1000.0);
		m_FilterGraph->SetCurrentPosition( newPos);
	}
	*pResult = 0;
}

void CMDSPlayer::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(m_FilterGraph)
	{
		if(!m_fs)
			m_FilterGraph->ResizeVideoWindow(0,0,cx,cy - SLIDERHEIGHT);
		else
			m_FilterGraph->ResizeVideoWindow(0,0,cx,cy);
	}
	m_SliderGraph.MoveWindow(0,cy - SLIDERHEIGHT,cx,SLIDERHEIGHT);
}

void CMDSPlayer::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_HIDE_CURSOR)
	{
			while(ShowCursor(FALSE)>=0);
			KillTimer(TIMER_HIDE_CURSOR);
	}
	else
	{
		if(m_FilterGraph)
		{
			if(!m_FilterGraph->IsRunning())
				return;
			double pos,length;
			m_FilterGraph->GetDuration(&length);
			m_FilterGraph->GetCurrentPosition(&pos);
			// Get the new position, and update the slider
			int newPos = (int)(pos * 1000.0 / length);
			if (m_SliderGraph.GetPos() != newPos)
				m_SliderGraph.SetPos(newPos);
			if(pos >= length)
			{
				m_FilterGraph->SetCurrentPosition(0);
				m_SliderGraph.SetPos(0);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CMDSPlayer::IninFileName(CString filename)
{
	filename.TrimLeft(_T(" "));
	filename.TrimRight(_T(" "));
	filename.TrimLeft(_T("\""));
	filename.TrimRight(_T("\""));
	if(filename.GetLength() > 3)
		m_filename = filename;
}

void CMDSPlayer::CreateGraph(const CString& strSourceFile)
{
	DestroyGraph();
	m_FilterGraph = new CDXGraph();
	if (m_FilterGraph->Create())
	{
		// Render the source clip
		const GUID RGB_FILTER = 
		{0x8B498501, 0x1218, 0x11CF, {0xad, 0xc4, 0x00,0xa0, 0xd1, 0x00, 0x04, 0x1b}};
//		m_FilterGraph->AddFilters(RGB_FILTER);
		m_FilterGraph->RenderFile(strSourceFile);
		m_FilterGraph->QueryInterfaces();
		// Set video window and notification window
		m_FilterGraph->SetNotifyWindow(this->GetSafeHwnd());
		m_FilterGraph->SetDisplayWindow(this->GetSafeHwnd());//m_VideoWindow
		// Show the first frame
//		m_FilterGraph->GetFrameStepInterface();
		m_FilterGraph->Run();
		SetWindowText(_T("MDSPlayer - ") + m_filename);
		SetTimer(0,200,0);
		TimerRuning = true;
		if(m_fs_init && !m_fs)
			OnFullScreen();
	}
}


void CMDSPlayer::DestroyGraph(void)
{
	if(m_FilterGraph)
	{
		// Stop the filter graph first
		m_FilterGraph->Stop();
		m_FilterGraph->SetNotifyWindow(NULL);
		
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

void CMDSPlayer::OnFullScreen()
{
	if(m_FilterGraph)
	{
		// Stop the filter graph first
		m_fs = !m_fs;
		if(m_fs)
		{
			ModifyStyle(WS_CAPTION | WS_THICKFRAME,WS_OVERLAPPED);
			m_SliderGraph.ShowWindow(SW_HIDE);
			SetMenu(NULL);
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
		}
		else
		{
			SetMenu(m_menu);
			ModifyStyle(WS_OVERLAPPED,WS_CAPTION | WS_THICKFRAME);
			if(save_width < 5 || save_height < 5)
			{
				RECT rc;
				if(m_FilterGraph->GetVideoRect(&rc))
				{
					save_height = rc.bottom - rc.top;
					save_width = rc.right - rc.left;
					save_x = rc.left;
					save_y = rc.top;
				}
			}
			if(!m_ontop)
				::SetWindowPos(this->m_hWnd,HWND_NOTOPMOST,save_x,save_y
					,save_width,save_height,SWP_SHOWWINDOW);
			else
				::SetWindowPos(this->m_hWnd,HWND_TOPMOST,save_x,save_y
					,save_width,save_height,SWP_SHOWWINDOW);
			m_SliderGraph.ShowWindow(SW_SHOW);
			KillTimer(TIMER_HIDE_CURSOR);
			ShowCursor(TRUE);
			while (ShowCursor(1) < 0)  ;
			//m_FilterGraph->SetDisplayWindow(this->GetSafeHwnd());
		}
	}
}

void CMDSPlayer::Seek(double len)
{
	if(m_FilterGraph)
	{
		double pos;
		m_FilterGraph->GetCurrentPosition(&pos);
		m_FilterGraph->SetCurrentPosition(pos + len);
	}
}
void CMDSPlayer::OnDsOpen()
{
	TCHAR szFilePath[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH,szFilePath);
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT
		,ResStr(IDS_OTHER_TYPEV) + _T("|*.avi;*.mkv;*.ogm;*.mp4;*.m4v;*.m4p;*.m4b;*.flv;*.vp6;\
*.divx;*.vg2;*.dat;*.mpg;*.mpeg;*.tp;*.ts;*.tpr;*.pva;*.pps;*.mpe;*.vob;*.rm;*.rmvb;\
*.wmv;*.asf;*.wmp;*.wm;*.mov;*.qt;*.3gp;*.3gpp;*.3g2;*.3gp2\
|") + ResStr(IDS_OTHER_TYPEA) + _T("|*.mp3;*.mp2;*.m4a;*.aac;*.mpc;*.wma;*.ogg;\
*.arm;*.mka;*.flac;*.ac3;*.dts;*.wav;*.ra;*.aif|") + ResStr(IDS_OTHER_ALL) + _T("(*.*)|*.*||"));

	dlg.m_ofn.lpstrTitle=ResStr(IDS_ASSOS_OP) + ResStr(IDS_ASSOS_DS);
	if(dlg.DoModal()==IDOK)
	{
		m_filename = dlg.GetPathName();
		CreateGraph(m_filename);
	}
	dlg.DestroyWindow();
	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}

void CMDSPlayer::OnDsPlay()
{
	if (m_FilterGraph)
	{
		if(m_FilterGraph->IsPaused() || m_FilterGraph->IsStopped())
			m_FilterGraph->Run();
		else
			m_FilterGraph->Pause();
	}
}

void CMDSPlayer::OnDsStop()
{
	if (m_FilterGraph)
	{
		if(m_FilterGraph->IsStopped())
			return;
		else
		{
			m_FilterGraph->SetCurrentPosition(0);
			m_SliderGraph.SetPos(0);
			m_FilterGraph->Stop();
			SetWindowText(_T("MDSPlayer"));
		}
	}
}

void CMDSPlayer::OnDsFullscreen()
{
	OnFullScreen();
}

void CMDSPlayer::OnDestroy()
{
	CDialog::OnDestroy();
	DestroyGraph();
}

bool CMDSPlayer::GetKeyCommand(LPARAM KeyNumber,CString &cmd, CString &value)
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

int CMDSPlayer::GetKeyNumber(CString Key)
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

void CMDSPlayer::LoadConfig()
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

bool CMDSPlayer::AnalyseLine(CString line, CString &key, CString &cmd, CString &val)
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

LRESULT CMDSPlayer::OnCmdKeyDown(WPARAM wParam, LPARAM lParam)
{
	if(lParam != CMDS_KEY)
		return FALSE;
	if(m_keydowning)
		return  FALSE;
	m_keydowning = true;
	CString cmd, value;
	if(GetKeyCommand( lParam, cmd, value))
	{
		if( cmd == _T("quit"))
		{
			OnOK();
		}
		else if( cmd == _T("pause"))
		{
			OnDsPlay();
		}
		else if( cmd == _T("vo_fullscreen"))
		{
			OnFullScreen();
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
		else if( cmd == _T("seek"))
		{
			int val = _ttoi(value) * 1000;
			Seek(val);
		}
		//else if( cmd == _T("keep_aspect"))
		//{
		//}
		//else if( cmd == _T("pt_step"))
		//{
		//}
	}
	m_keydowning = false;
	return TRUE;
}

BOOL CMDSPlayer::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case   WM_KEYDOWN:
		SendMessage(CMDS_KEYDOWN, CMDS_KEY, pMsg->wParam );
		break;
	case   WM_LBUTTONDOWN:	
		if(m_FilterGraph && TimerRuning)
		{
			CPoint cursorPos;
			cursorPos.x = GetCurrentMessage()->pt.x;
			cursorPos.y = GetCurrentMessage()->pt.y;
			OnLButtonDown(MK_LBUTTON,cursorPos);
		}
		break;
	case  WM_LBUTTONUP:
		if(m_FilterGraph && !TimerRuning)
		{
			SetTimer(0,200,0);
			TimerRuning = 1;
		}
		break;
	case   WM_LBUTTONDBLCLK:
		SendMessage(CMDS_KEYDOWN, CMDS_KEY, WM_LBUTTONDBLCLK );
		return TRUE;
	case  WM_RBUTTONDOWN:
		SendMessage(CMDS_KEYDOWN, CMDS_KEY, WM_RBUTTONDOWN );
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMDSPlayer::OnLButtonDown(UINT nFlags, CPoint point)
{
	RECT rc;
	GetClientRect(&rc);
	if(point.y > rc.bottom - SLIDERHEIGHT)
	{
		KillTimer(0);
		TimerRuning = 0;
	}
	else
	{
		SendMessage(CMDS_KEYDOWN, CMDS_KEY, WM_LBUTTONDOWN );
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CMDSPlayer::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_fs)
		return;
	ShowCursor(TRUE);
	SetTimer(TIMER_HIDE_CURSOR, 2000, NULL);
	if(m_FilterGraph)
	{
		LONG m_Height=GetSystemMetrics(SM_CYSCREEN);
		if(point.y > m_Height - 16 && !m_showctrl)
		{
			m_showctrl = true;
			m_FilterGraph->ShowSlider();
			m_SliderGraph.ShowWindow(SW_SHOW);
		}
		else if(m_SliderGraph.IsWindowVisible() && m_showctrl)
		{
			m_showctrl = false;
			m_FilterGraph->ShowSlider(0);
			m_SliderGraph.ShowWindow(SW_HIDE);
			Invalidate();
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CMDSPlayer::OnDropFiles(HDROP hDropInfo)
{

    TCHAR szFileName[MAX_PATH];
    int iFileNumber = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
    if (iFileNumber > 0) 
    {
        DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
		m_filename.Format(_T("%s") , szFileName);
		CreateGraph(m_filename);
    }
	CDialog::OnDropFiles(hDropInfo);
}
