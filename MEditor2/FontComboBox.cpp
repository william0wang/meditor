#include "stdafx.h"
#include "FontComboBox.h"
//#include   <functional>   
#include   <algorithm>

#ifndef   NTM_PS_OPENTYPE
#define   NTM_PS_OPENTYPE                0x00020000
#endif
#ifndef   NTM_TT_OPENTYPE
#define   NTM_TT_OPENTYPE                0x00040000
#endif
#ifndef   PS_OPENTYPE_FONTTYPE
#define   PS_OPENTYPE_FONTTYPE      0x10000
#define   TT_OPENTYPE_FONTTYPE      0x20000
#define   TYPE1_FONTTYPE                   0x40000
#endif

#define           FNTIMG_XSIZE             20
#define           FNTIMG_X                 20
#define           FNTIMG_Y                 12

BEGIN_MESSAGE_MAP(CFontComboBox, CComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CFontComboBox::FillFontList()
{
	//根据桌面DC的属性枚举系统的字体信息
	CDC* pDesktopDC = GetDesktopWindow()->GetWindowDC();       //取DC
	HDC hdc = pDesktopDC->GetSafeHdc();
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfCharSet = DEFAULT_CHARSET;
	::EnumFontFamilies(hdc,NULL, (FONTENUMPROC)EnumFontProc,(LPARAM)this);
	GetDesktopWindow()->ReleaseDC(pDesktopDC);      //释放DC
	//运用STL算法进行自定义的对象排序
	//std::sort(m_pFontVec.begin(), m_pFontVec.end(), CFontComboBox::CompareFontName);
	InitStorage(300, LF_FACESIZE);
	//加入字体名称到组合框
	for(UINT N=0; N<m_pFontVec.size(); ++N)
	{
		CString fontname = m_pFontVec[N]->GetFontName();
		if (fontname.Find(_T("@")) != 0)
			AddString(fontname);
	}
	//加载COMDLG32.DLL
	HMODULE hModule = ::LoadLibraryEx(_T("COMDLG32.DLL"), NULL,
		DONT_RESOLVE_DLL_REFERENCES);
	ASSERT (hModule != NULL);
	//加载成功后从这个DLL中加载字体图片
	m_FontBmp = (HBITMAP)::LoadImage(hModule, MAKEINTRESOURCE(38), 
		IMAGE_BITMAP, 100, 24, LR_DEFAULTCOLOR);
	ASSERT(m_FontBmp != NULL);
	::FreeLibrary(hModule);
}

bool CFontComboBox::CompareFontName(const CFontInfo *pInfo1, const CFontInfo *pInfo2)
{
	if (pInfo1->GetFontName() != pInfo2->GetFontName())
		return false;
	return true;
}

int CFontComboBox::EnumFontProc(ENUMLOGFONTEX *lpelfe,
								NEWTEXTMETRICEX *lpntme,
								DWORD FontType,
								LPARAM lParam)
{
	CFontComboBox *pThis = reinterpret_cast<CFontComboBox*>(lParam);
	CFontInfo *pInfo = new CFontInfo;
	pInfo->SetFontType(FontType);
	pInfo->SetFontName(lpelfe->elfLogFont.lfFaceName);
	DWORD dwFontType = FontType;
	if (FontType & TRUETYPE_FONTTYPE)
	{
		DWORD dwFontFlags = lpntme->ntmTm.ntmFlags;
		if (dwFontFlags & NTM_PS_OPENTYPE)
			dwFontType |= PS_OPENTYPE_FONTTYPE;
		else
			dwFontType |=0;
		if (dwFontFlags & NTM_TT_OPENTYPE)
			dwFontType |= TT_OPENTYPE_FONTTYPE;
		else
			dwFontType |=0;
		dwFontType |= (dwFontFlags & NTM_TYPE1 ? TYPE1_FONTTYPE : 0);
	}
	switch(dwFontType & 0x70007)
	{
	case (TRUETYPE_FONTTYPE | PS_OPENTYPE_FONTTYPE):
	case (TRUETYPE_FONTTYPE | TT_OPENTYPE_FONTTYPE):
	case (TRUETYPE_FONTTYPE | TYPE1_FONTTYPE):
		pInfo->SetImage(3); break;
	case RASTER_FONTTYPE:
	case DEVICE_FONTTYPE:
	case NULL:
		pInfo->SetImage(0XFF); break;
	case TRUETYPE_FONTTYPE:
	default:
		pInfo->SetImage(0); break;
	}
	pThis->m_pFontVec.push_back(pInfo);
	return TRUE;
}

void CFontComboBox::OnDropdown()
{
	int nNumEntries = GetCount();
	int nWidth = 0;
	CString str;
	CClientDC dc(this);
	int nSave = dc.SaveDC();
	dc.SelectObject(GetFont());
	int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);      //取滚动条宽度
	for (int i = 0; i < nNumEntries; i++)
	{
		GetLBText(i, str);
		int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
		nWidth = max(nWidth, nLength);
	}
	nWidth += dc.GetTextExtent(_T("0")).cx;
	dc.RestoreDC(nSave);
	if (!m_pFontVec.empty())
		SetDroppedWidth(nWidth + FNTIMG_XSIZE);                 //设置宽度值
}

void CFontComboBox::OnDestroy()
{
	for (UINT i=0; i<m_pFontVec.size(); ++i)
		delete m_pFontVec[i];
	m_pFontVec.erase(m_pFontVec.begin(), m_pFontVec.end());
	CComboBox::OnDestroy();
}

void CFontComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	int height;
	CWindowDC dc(NULL);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	height = tm.tmHeight - tm.tmInternalLeading;
	lpMeasureItemStruct->itemHeight = height - 1;
}

void CFontComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_COMBOBOX);
	CString strText;
	//判断当前索引的字体名称是否为空
	int nIndex = lpDrawItemStruct->itemID;
	if (GetLBTextLen(nIndex) < 0)
		return ;
	GetLBText(nIndex, strText);
	ASSERT(!strText.IsEmpty());
	int nImage = 0;
	//根据索引值从m_pFontVec得到字体图像类型
	if (!m_pFontVec.empty())
		nImage = m_pFontVec[nIndex]->GetImage();
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	//如果Item处于选择焦点、状态下，用系统高亮色改变文本和背景色
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
	CRect     rItem(lpDrawItemStruct->rcItem);
	CRect     rText(rItem);
	CRect     rBmp(&rItem);
	if(m_FontBmp)
	{
		//计算Item中字体预览图的贴图位置
		rBmp.top +=  (rBmp.Height() - FNTIMG_Y) / 2;
		rBmp.bottom = rBmp.top + FNTIMG_Y + 1;
		rText.left += FNTIMG_X;
		if (nImage != (int)0XFF)
		{
			int x,y;
			x = nImage * FNTIMG_X;   //根据预览图类型计算源图的矩形大小
			y = FNTIMG_Y;
			CDC mdc;
			mdc.CreateCompatibleDC(&dc);
			CBitmap* pOldBmp = mdc.SelectObject(CBitmap::FromHandle(m_FontBmp));
			COLORREF clrTransparent = mdc.GetPixel(0, y);
			//调用TransparentBlt进行透明贴图，此函数需要加入MsImg32.Lib文件
			::TransparentBlt (dc.GetSafeHdc(),rBmp.left, rBmp.top, FNTIMG_X, FNTIMG_Y,  
				mdc, x, y, FNTIMG_X, FNTIMG_Y, clrTransparent);
		}
	}
	else
		rText.left += 10;
	//rText所代表的字体名称填充位置需要右移图片的宽度
	dc.DrawText(strText, rText, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	dc.Detach();
} 