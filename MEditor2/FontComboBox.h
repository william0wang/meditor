#pragma once
#include <vector>

class CFontInfo
{
public:
	CFontInfo(){}
	~CFontInfo(){}
public:
	int           GetImage() const { return m_nImage; }
	void        SetImage(int nImage) { m_nImage = nImage; }
	CString   GetFontName() const { return m_szName ; }
	void        SetFontName(CString str) { m_szName = str; }
	int           GetFontType() const { return m_nFontType; }
	void        SetFontType(int Type) { m_nFontType = Type; }
private:
	CString   m_szName;
	int           m_nFontType;
	int           m_nImage;
};

// CFontComboBox

class CFontComboBox : public CComboBox
{
public:
	//CFontComboBox();
	//virtual ~CFontComboBox();
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void FillFontList();
	static int CALLBACK EnumFontProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, DWORD FontType, LPARAM lParam);
	static bool CompareFontName(const CFontInfo *pInfo1, const CFontInfo *pInfo2);
	HBITMAP      m_FontBmp;
protected:
	afx_msg void OnDropdown();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	std::vector<CFontInfo *> m_pFontVec;
};

