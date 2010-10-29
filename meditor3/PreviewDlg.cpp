// PreviewDlg.cpp : implementation of the CPreviewDlg class
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"

#include <atlfile.h>
#include "shared.h"
#include "../Libs/CxImage/CxImage/ximage.h"
#include "PreviewDlg.h"

static DWORD WINAPI ProgressThread(LPVOID pParam)
{
	CPreviewDlg *This = (CPreviewDlg *)pParam;

	This->m_pdlg.DoModal();

	return 0;
}

CPreviewDlg::CPreviewDlg(UINT DLG_IDD)
{
	IDD = DLG_IDD;
	m_time = 0;
	m_parti = 4;
	m_partj = 4;
	m_width = 1024;
	m_filename = _T("");
	m_savename = _T("");
	m_show = TRUE;
	ltime = 0;
	m_have_preview = FALSE;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);
	m_player_exe = m_program_dir + _T("mplayer.exe");
}


BOOL CPreviewDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CPreviewDlg::OnIdle()
{
	return FALSE;
}

LRESULT CPreviewDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
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

	m_font_c.Attach(GetDlgItem(IDC_ACOMBO_FONT));
	m_spinr.Attach(GetDlgItem(IDC_SPIN_R));
	m_spinv.Attach(GetDlgItem(IDC_SPIN_V));
	m_spinw.Attach(GetDlgItem(IDC_SPIN_W));

	if(FileExist(_T("./preview")))
		m_have_preview = TRUE;

	m_spinr.SetRange(1, 8);
	m_spinr.SetBuddy(GetDlgItem(IDC_EDIT_R));

	m_spinv.SetRange(1, 8);
	m_spinv.SetBuddy(GetDlgItem(IDC_EDIT_V));

	m_spinw.SetRange(200, 1920);
	m_spinw.SetBuddy(GetDlgItem(IDC_EDIT_W));

	TCHAR name[MAX_PATH*2];
	_tcscpy_s(name, MAX_PATH*2, m_filename);
	TCHAR *p = _tcsrchr(name, _T('.'));
	if(p) p[0] = 0;

	m_savename.Format(_T("%s Preview.jpg"), name);

	m_font = _T("* MS Shell Dlg");
	m_font_c.AddString(m_font);
	m_font_c.SetCurSel(0);
	EnumerateFonts();

	CString font_def = _T("Î¢ÈíÑÅºÚ");
	int index = m_font_c.FindStringExact(0, font_def);

	if(index < 0) {
		font_def = _T("consolas");
		index = m_font_c.FindStringExact(0, font_def);
	}

	if(index > 0) {
		m_font_c.SetCurSel(index);
		m_font = font_def;
	}

	DoDataExchange(DDX_LOAD, IDC_EDIT_FILE);
	DoDataExchange(DDX_LOAD, IDC_EDIT_SAVEFILE);
	DoDataExchange(DDX_LOAD, IDC_EDIT_R);
	DoDataExchange(DDX_LOAD, IDC_EDIT_V);
	DoDataExchange(DDX_LOAD, IDC_EDIT_W);
	DoDataExchange(DDX_LOAD, IDC_EDIT_T);
	DoDataExchange(DDX_LOAD, IDC_CHECK_SHOW);

	SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;
}

void CPreviewDlg::GetPreview(int index, int resolution, int time)
{
	CString cmd;
	STARTUPINFO si;
	PROCESS_INFORMATION procInfo;
	ZeroMemory(&procInfo,sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	cmd.Format(_T("\"%s\" -generate-preview -noidle -autoplay 0 \"%s\" -vf ass,scale=%d:-10 -vo jpeg:outdir=\"./preview\" -ao null -frames 3 -ss %d")
		, m_player_exe, m_filename, resolution, time);

	CreateProcess(NULL, cmd.GetBuffer(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &procInfo);
	cmd.ReleaseBuffer();

	WaitForSingleObject(procInfo.hProcess, INFINITE);

	cmd.Format(_T("./preview/preview_%02d.jpg"), index);

	CopyFile(_T("./preview/00000003.jpg"), cmd,FALSE);
	DeleteFile(_T("./preview/00000001.jpg"));
	DeleteFile(_T("./preview/00000002.jpg"));
	DeleteFile(_T("./preview/00000003.jpg"));

}

void CPreviewDlg::GenerateThumbnails(CString ThumbName)
{
	int h = 0;
	int he = 0;
	HDC dc2;
	RECT r;
	HBITMAP bmp;
	VOID *pbits32;
	HBRUSH brush;
	LOGFONT fontRect;
	
	if(m_font_c.GetCurSel() == 0)
		m_font = _T("MS Shell Dlg");

	CxImage img, logo, full;
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_LOGO), TEXT("LOGO"));
	logo.LoadResource(hRsrc, CXIMAGE_FORMAT_JPG);

	for(int i = 0; i < m_parti; i++) {
		for(int j = 0; j < m_partj; j++) {
			int re = (m_width-10)/m_partj-10;
			if(re > 16) re = re/16*16;
			long nowt = (m_time)*(i*m_partj+j+1)/(m_parti*m_partj+1);
			GetPreview(i*m_partj+j+1, re, nowt);
			CString jpg;
			jpg.Format(_T("./preview/preview_%02d.jpg"), i*m_partj+j+1);
			img.Load(jpg, CXIMAGE_FORMAT_JPG);
			DeleteFile(jpg);
			if(i == 0 && j == 0) {
				CString time;
				time.Format(_T("%02d:%02d:%02d"), ltime/3600,(ltime/60)%60,ltime%60);
				TCHAR *Name = _tcsrchr(m_filename.GetBuffer(), _T('\\'));
				m_filename.ReleaseBuffer();

				double fsize = 0;
				CAtlFile fl;
				if(SUCCEEDED(fl.Create(m_filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))) {
					ULONGLONG len;
					if(SUCCEEDED(fl.GetSize(len)))
						fsize = (double)(len/1024/1024.0);
					fl.Close();
				}
				he = img.GetHeight();
				h = (he+10) * m_parti + 90;

				BITMAPINFOHEADER RGB32BITSBITMAPINFO = {sizeof(BITMAPINFOHEADER),
					m_width, h, 1, 32, BI_RGB, 0, 0, 0, 0, 0 };

				dc2 = CreateCompatibleDC(NULL);

				r.top = r.left = 0;
				r.right = m_width;
				r.bottom = h;

				bmp = CreateDIBSection(dc2,(BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);

				brush = CreateSolidBrush(RGB(234,234,234));
				SelectObject(dc2, bmp);
				FillRect(dc2, &r, brush);
				DeleteObject(brush);

				logo.Draw(dc2, m_width-10 - logo.GetWidth(), 20);

				if(Name) ++Name;

				memset(&fontRect,0,sizeof(LOGFONT));
				fontRect.lfHeight = -14;
				fontRect.lfWeight = FW_BOLD;
				_tcscpy_s(fontRect.lfFaceName, 32, m_font);

				SelectObject(dc2, CreateFontIndirect(&fontRect));
				SetBkMode(dc2, TRANSPARENT);
				SetTextColor(dc2, RGB(0, 0, 0));

				TextOut(dc2, 10, 12, Name?Name:m_filename, Name?(int)_tcslen(Name):m_filename.GetLength());

				TextOut(dc2, 10, 36, time, time.GetLength());

				if(fsize > 0){
					time.Format(_T("%.02f MB"), fsize);
					TextOut(dc2, 10, 60, time, time.GetLength());
				}

			}
			int spw = m_partj > 1 ? (m_width-m_partj*(re)-20)/(m_partj-1) : 10;
			int left = 10+j*(re+spw);
			int top = (he+10)*i+90;

			img.Draw(dc2, left, top);

			jpg.Format(_T("%02d:%02d:%02d"), nowt/3600,(nowt/60)%60,nowt%60);

			SetBkMode(dc2, TRANSPARENT);
			SetTextColor(dc2, RGB(0, 0, 0));
			TextOut(dc2, left+3, top+3, jpg, jpg.GetLength());

			SetBkMode(dc2, TRANSPARENT);
			SetTextColor(dc2, RGB(255, 255, 255));
			TextOut(dc2, left+2, top+2, jpg, jpg.GetLength());
			m_percent = (i*m_partj+j+1)*100/(m_parti*m_partj);
			m_pdlg.m_progress.SetPos(m_percent);
		}
	}

	full.CreateFromHBITMAP(bmp);
	m_pdlg.m_progress.SetPos(100);

	full.Save(ThumbName, CXIMAGE_FORMAT_JPG);
	if(!m_have_preview)
		DeleteFolder(_T("./preview"));

}

BOOL CPreviewDlg::EnumerateFonts()
{
	HDC hDC;

	// Get screen fonts
	hDC = ::GetWindowDC(NULL);

	LOGFONT lf;

	ZeroMemory(&lf,sizeof(lf));
	lf.lfCharSet = DEFAULT_CHARSET;

	if (!EnumFontFamiliesEx(
		hDC,	// handle to device context
		&lf,	// pointer to logical font information
		(FONTENUMPROC)EnumFamScreenCallBackEx,	// pointer to callback function
		(LPARAM) this,	// application-supplied data
		(DWORD) 0))
		return FALSE;

	::ReleaseDC(NULL,hDC);	

	return TRUE; // All's ok
}

BOOL CALLBACK CPreviewDlg::EnumFamScreenCallBackEx(ENUMLOGFONTEX* pelf, 
	NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)

{
	// don't put in non-printer raster fonts
	if (FontType & RASTER_FONTTYPE)
		return 1;

	if (!(FontType & TRUETYPE_FONTTYPE))
		return 1;

	((CPreviewDlg*)pThis)->AddFont(pelf->elfLogFont.lfFaceName);

	return 1; // Call me back
}

void CPreviewDlg::AddFont(CString strName)
{
	if(m_font_c.FindStringExact(0,strName) > 0 || strName.Find(_T("@")) == 0)
		return;
	m_font_c.AddString(strName);
}

LRESULT CPreviewDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(DDX_SAVE);
	if(m_parti < 1 || m_parti > 8 || m_partj < 1 || m_partj > 8 || m_width < 200 || m_width > 2560)
		return 0;

	if(m_time <= 0 || m_time > ltime)
		m_time = ltime;

	m_percent = 0;

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ProgressThread, (LPVOID)this, 0, 0);
	ShowWindow(SW_HIDE);
	GenerateThumbnails(m_savename);

	if(m_show)
		ShellExecute(0, _T("open"), m_savename, NULL, NULL, SW_SHOW);

	CloseDialog(wID);
	return 0;
}

LRESULT CPreviewDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CPreviewDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

void CPreviewDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CPreviewDlg::OnBnClickedButtonB(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	TCHAR szFilePath[MAX_PATH*2 + 1], Path[MAX_PATH*2 + 1], *Name;
	::GetCurrentDirectory(MAX_PATH*2,szFilePath);
	_tcscpy_s(Path, MAX_PATH*2, m_filename);
	TCHAR *p = _tcsrchr(Path, _T('\\'));
	if(p) p[1] = 0;
	Name = _tcsrchr(m_savename.GetBuffer(), _T('\\'));
	m_savename.ReleaseBuffer();
	if(Name) ++Name;

	TCHAR strBuffer[MAX_PATH * 2]  = {0};
	CFileDialog dlg(FALSE, 0, Name, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, _T("JPEG File (*.jpg)\0*.jpg\0"));
	dlg.m_ofn.lpstrInitialDir= Path;
	dlg.m_ofn.lpstrFile = strBuffer;
	dlg.m_ofn.nMaxFile = MAX_PATH * 2;
	if(dlg.DoModal()==IDOK) {
		m_savename = dlg.m_ofn.lpstrFile;
		if(m_savename.Find(_T(".jpg")) < 0)
			m_savename += _T(".jpg");
	}

	::SetCurrentDirectory(szFilePath);

	DoDataExchange(DDX_LOAD, IDC_EDIT_SAVEFILE);
	return 0;
}
