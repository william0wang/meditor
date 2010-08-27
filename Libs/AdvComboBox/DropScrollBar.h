/////////////////////////////////////////////////////////////////////////////
// DropScrollBar.h : header file
// 
// CAdvComboBoxMod Control
// Version: 2.1
// Date: September 2002
// Author: Mathias Tunared
// Email: Mathias@inorbit.com
// Copyright (c) 2002. All Rights Reserved.
//
// This code, in compiled form or as source code, may be redistributed 
// unmodified PROVIDING it is not sold for profit without the authors 
// written consent, and providing that this notice and the authors name 
// and all copyright notices remains intact.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DROPSCROLLBAR_H__8814CD20_3D94_4B9C_8A64_DF4E9F6DD4DC__INCLUDED_)
#define AFX_DROPSCROLLBAR_H__8814CD20_3D94_4B9C_8A64_DF4E9F6DD4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DropWnd.h"
class CAdvDropListBox;

/////////////////////////////////////////////////////////////////////////////
// CAdvDropScrollBar window

class CAdvDropScrollBar : public CScrollBar
{
// Construction
public:
	CAdvDropScrollBar();

// Attributes
public:

// Operations
public:
	void SetListBox( CAdvDropListBox* pListBox );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvDropScrollBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdvDropScrollBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdvDropScrollBar)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void VScroll(UINT nSBCode, UINT nPos);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnSetCapture( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnReleaseCapture( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

private:
	CAdvDropListBox* m_pListBox;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DROPSCROLLBAR_H__8814CD20_3D94_4B9C_8A64_DF4E9F6DD4DC__INCLUDED_)
