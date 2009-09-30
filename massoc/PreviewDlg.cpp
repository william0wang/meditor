// PreviewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "massoc.h"
#include "MShared.h"
#include "PreviewDlg.h"
#include "../CxImage/CxImage/ximage.h"

static DWORD WINAPI ProgressThread(LPVOID pParam)
{
	CPreviewDlg *This = (CPreviewDlg *)pParam;

	This->m_pdlg.DoModal();

	//while(This->m_percent < 100)
	//{
	//	m_pdlgm_pdlg.m_progress.SetPos(This->m_percent);
	//}


	return 0;
}

// CPreviewDlg 对话框

IMPLEMENT_DYNAMIC(CPreviewDlg, CDialog)


CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
: CDialog(theApp.IDD, pParent)
	, m_time(0)
	, m_parti(4)
	, m_partj(4)
	, m_width(1024)
	, m_filename(_T(""))
	, m_savename(_T(""))
	, m_show(TRUE)
{
	ltime = 0;
	m_have_preview = FALSE;
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	m_program_dir.Format(_T("%s"),szFilePath);

	CString right = m_program_dir.Right(8);
	if(right == _T("\\codecs\\"))
		m_program_dir = m_program_dir.Left(m_program_dir.GetLength() - 7);
	m_player_exe = m_program_dir + _T("mplayer.exe");
}

CPreviewDlg::~CPreviewDlg()
{
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE, m_filename);
	DDX_Text(pDX, IDC_EDIT_R, m_parti);
	DDV_MinMaxInt(pDX, m_parti, 1, 8);
	DDX_Text(pDX, IDC_EDIT_V, m_partj);
	DDV_MinMaxInt(pDX, m_partj, 1, 8);
	DDX_Text(pDX, IDC_EDIT_W, m_width);
	DDV_MinMaxInt(pDX, m_width, 200, 1920);
	DDX_Text(pDX, IDC_EDIT_SAVEFILE, m_savename);
	DDX_Check(pDX, IDC_CHECK_SHOW, m_show);
	DDX_Control(pDX, IDC_SPIN_R, m_spinr);
	DDX_Control(pDX, IDC_SPIN_V, m_spinv);
	DDX_Control(pDX, IDC_SPIN_W, m_spinw);
	DDX_Text(pDX, IDC_EDIT_T, m_time);
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPreviewDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_B, &CPreviewDlg::OnBnClickedButtonB)
END_MESSAGE_MAP()


// CPreviewDlg 消息处理程序


BOOL CPreviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(IsFileExist(_T("./preview")))
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

	UpdateData(FALSE);

	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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

	cmd.Format(_T("\"%s\" -noidle -autoplay 0 \"%s\" -vf scale=%d:-3 -vo jpeg:outdir=\"./preview\" -ao null -frames 3 -ss %d")
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
	RGBQUAD rgb;

	CxImage img, logo, full;
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_LOGO), TEXT("LOGO"));
	logo.LoadResource(hRsrc, CXIMAGE_FORMAT_JPG);

	for(int i = 0; i < m_parti; i++) {
		for(int j = 0; j < m_partj; j++) {
			int re = (m_width-10)/m_partj-10;
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
				CFile fl;
				if(fl.Open(m_filename, CFile::modeRead|CFile::shareDenyWrite)){
					fsize = (double)(fl.GetLength()/1024/1024.0);
					fl.Close();
				}
				he = img.GetHeight();
				h = (he+10) * m_parti + 90;
				full.Create(m_width, h, 24, CXIMAGE_FORMAT_JPG);
				rgb.rgbBlue = rgb.rgbGreen = rgb.rgbRed = 0;
				rgb.rgbReserved = 0;
				full.MixFrom(logo, m_width - logo.GetWidth(), h - logo.GetHeight());
				if(Name) ++Name;
				full.DrawString(NULL, 10, 22, Name?Name:m_filename, rgb, _T("MS Sans Serif"), -14, 600);
				full.DrawString(NULL, 10, 45, time, rgb, _T("MS Sans Serif"), -14, 600);
				if(fsize > 0){
					time.Format(_T("%.02f MB"), fsize);
					full.DrawString(NULL, 10, 68, time, rgb, _T("MS Sans Serif"), -14, 600);
				}

			}
			int left = 10+j*(re+10);
			int right = (he+10)*(i+1)+80;
			full.MixFrom(img, left, h - right);
			rgb.rgbBlue = rgb.rgbGreen = rgb.rgbRed = 255;
			jpg.Format(_T("%02d:%02d:%02d"), nowt/3600,(nowt/60)%60,nowt%60);
			full.DrawString(NULL, left, right, jpg, rgb, _T("MS Sans Serif"), -14, 600);
			m_percent = (i*m_partj+j+1)*100/(m_parti*m_partj);
			m_pdlg.m_progress.SetPos(m_percent);
		}
	}
	m_pdlg.m_progress.SetPos(100);

	full.Save(ThumbName, CXIMAGE_FORMAT_JPG);
	if(!m_have_preview)
		DeleteFolder(_T("./preview"));

}

void CPreviewDlg::OnBnClickedButtonB()
{
	TCHAR szFilePath[MAX_PATH*2 + 1], Path[MAX_PATH*2 + 1], *Name;
	::GetCurrentDirectory(MAX_PATH*2,szFilePath);
	_tcscpy_s(Path, MAX_PATH*2, m_filename);
	TCHAR *p = _tcsrchr(Path, _T('\\'));
	if(p) p[1] = 0;
	Name = _tcsrchr(m_savename.GetBuffer(), _T('\\'));
	m_savename.ReleaseBuffer();
	if(Name) ++Name;

	CFileDialog dlg(FALSE, _T("*.jpg"), Name,OFN_HIDEREADONLY, _T("JPEG File (*.jpg)|*.jpg||"));
	dlg.m_ofn.lpstrInitialDir= Path;
	if(dlg.DoModal()==IDOK)
		m_savename = dlg.GetPathName();
	dlg.DestroyWindow();
	if(m_savename.Find(_T(".jpg")) < 0)
		m_savename += _T(".jpg");

	::SetCurrentDirectory(szFilePath);
	UpdateData(FALSE);
}

void CPreviewDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(m_parti < 1 || m_parti > 8 || m_partj < 1 || m_partj > 8 || m_width < 400 || m_width > 1920)
		return;

	if(m_time <= 0 || m_time > ltime)
		m_time = ltime;

	m_percent = 0;

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ProgressThread, (LPVOID)this, 0, 0);
	ShowWindow(SW_HIDE);
	GenerateThumbnails(m_savename);

	if(m_show)
		ShellExecute(0, _T("open"), m_savename, NULL, NULL, SW_SHOW);

	OnOK();
}

