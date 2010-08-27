// kFlatCombo.h: interface for the kFlatCombo class.
//
//////////////////////////////////////////////////////////////////////

//
// Copyright ?1998-99 Kirk W. Stowell
//		mailto:kstowell@codejockeys.com
//		http://www.codejockeys.com/kstowell/
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. If the source code in this file is used in 
// any  commercial application then a statement along the lines of 
// "Portions Copyright ?1998 Kirk Stowell" must be included in 
// the startup banner, "About" box or printed documentation. An email 
// letting me know that you are using it would be nice as well. That's 
// not much to ask considering the amount of work that went into this.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy, and let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into 
// this file. 
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 5/17/99 9:24p $
 * $Revision: 9 $
 * $Archive: /CodeJockey/Include/CJFlatComboBox.h $
 *
 * $History: CJFlatComboBox.h $
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 5/17/99    Time: 9:24p
 * Updated in $/CodeJockey/Include
 * Gilberto Botaro [seafox@techno.com.br] - Added code for auto
 * completion.
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 10:44p
 * Updated in $/CodeJockey/Include
 * Fixed code ommisions.
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 8:42p
 * Updated in $/CodeJockey/Include
 * Danilo Lax [Danilo_Lax@www2.alpeng.com] - The control is painted flat
 * when the mouse leaves the box, even if the control has the focus, now
 * checking for the focus in OnTimer(.
 * 
 * Removed, unhandled msgs are processed by
 * CComboBox::PreTranslateMessage(pMsg). Used reflected msgs
 * BN_GETFOCUS/CBN_KILLFOCUS instead of WM_GETFOCUS/WM_KILLFOCUS.
 * 
 * ::GetFocus() doesn't return CFlatComboBox::m_hWnd, even if the control
 * has the focus (don't know why, maybe ::GetFocus() only returns the
 * handle to the EditField part of the ComboBox), used flag m_bHasFocus
 * instead.
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 4:26p
 * Updated in $/CodeJockey/Include
 * Added comments and cleaned up code.
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 1/22/99    Time: 4:26p
 * Updated in $/CodeJockey/Include
 * Fixed accelerator problem reported by YOSH YAMASHITA
 * (yosh@muj.biglobe.ne.jp) with Matt Weagle's solution of overloading
 * PreTranslateMessage().
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 1/02/99    Time: 4:25p
 * Updated in $/CodeJockey/Include
 * Renamed class from CJComboBox to CCJFlatComboBox, changed DRAWSTATE to
 * enum, removed method Offset(), and replaced with a direct call to
 * ::GetSystemMetrics().
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 11/02/98   Time: 4:24p
 * Updated in $/CodeJockey/Include
 * Added OnSetFocus(), OnKillFocus() and modified OnPaint() so the control
 * remains beveled until it actually looses focus - (Vachik
 * Hovhannissian).
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 10/05/98   Time: 4:24p
 * Updated in $/CodeJockey/Include
 * Fixed offset bug to use GetSystemMetrics(), instead of hard coded
 * values -
 * Todd Brannam (tbrannam@msn.com).
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 10/02/98   Time: 4:23p
 * Created in $/CodeJockey/Include
 * First release version.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

/*
	port on WTL
	Konstantin Boukreev (konstantin@mail.primorye.ru)
	30.06.2000 13:47:01		

	used Atl/Aux library (CAuxThunk) by by Andrew Nosenko (andien@geocities.com), 1997-99
		http://www.geocities.com/~andien/atlaux.htm
		Version: 1.10.0011
*/
	

#if !defined(AFX_KFLATCOMBO_H__48589D0D_EF0B_49C1_A176_5F28E5381598__INCLUDED_)
#define AFX_KFLATCOMBO_H__48589D0D_EF0B_49C1_A176_5F28E5381598__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtlAux2.h"

#define OCM_COMMAND_CODE_HANDLER(code, func) \
	if(uMsg == OCM_COMMAND && code == HIWORD(wParam)) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

template <class T>
class kFlatComboT  : 
	public CAuxThunk<kFlatComboT< T >>,
	public CWindowImpl<kFlatComboT< T >, T>
{
protected:

	int		m_nOffset;		// offset used during paint.
	BOOL		m_bLBtnDown;		// TRUE if left mouse button is pressed
	BOOL		m_bPainted;			// used during paint operations
	BOOL		m_bHasFocus;		// TRUE if control has focus
	BOOL		m_bAutoComp;		// Used by Autocompleting.
	COLORREF	m_clrBtnHilite;	// set to the system color COLOR_BTNHILIGHT
	COLORREF	m_clrBtnShadow;	// set to the system color COLOR_BTNSHADOW
	COLORREF	m_clrBtnFace;		// set to the system color COLOR_BTNFACE

	DWORD		m_timer;				// timer id	

	// enum used to determine the state the combo box should be
	//
	enum STATE { normal = 1, raised = 2, pressed = 3 };

	CContainedWindowT<CEdit> m_edit;

public:
	kFlatComboT() :
	  m_edit(_T("EDIT"), this, 1)
	{
		m_timer = 0;
		InitThunk((TMFP)&kFlatComboT<T>::OnTimer, this);

		m_bLBtnDown	= FALSE;
		m_bPainted	= FALSE;
		m_bHasFocus	= FALSE;
		m_bAutoComp = TRUE;

		m_clrBtnHilite  = ::GetSysColor(COLOR_BTNHILIGHT);
		m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
		m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);
		m_nOffset		= ::GetSystemMetrics(SM_CXHTHUMB);
	}
	~kFlatComboT()
	{
		if (m_timer)
			::KillTimer(0, m_timer);		
	}
	
	DECLARE_WND_SUPERCLASS(0, T::GetWndClassName())

	BEGIN_MSG_MAP(kFlatComboT)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)		
		OCM_COMMAND_CODE_HANDLER(CBN_SETFOCUS, OnSetFocusNotify)
		OCM_COMMAND_CODE_HANDLER(CBN_KILLFOCUS, OnKillFocusNotify)
		OCM_COMMAND_CODE_HANDLER(CBN_EDITUPDATE, OnEditUpdateNotify)
		MESSAGE_HANDLER(WM_SYSCOLORCHANGE, OnSysColorChange)
		ALT_MSG_MAP(1)
			MESSAGE_HANDLER(WM_CHAR, OnCharEdit)
//		DEFAULT_REFLECTION_HANDLER( )
	END_MSG_MAP()

protected:

	LRESULT OnCharEdit(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{		
		bHandled = FALSE;
		if (wParam == 0xd) { //return
			int nLen = GetWindowTextLength();
			TCHAR* pStr = (TCHAR *) _alloca((nLen + 1) * sizeof TCHAR);
			GetWindowText(pStr, nLen);
			if (CB_ERR != SelectString(-1, pStr)) {
				// notify message
				::PostMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetWindowLong(GWL_ID), CBN_SELCHANGE), (LPARAM)m_hWnd);
				bHandled = TRUE;
			}
		}
		return 0;
	}
	
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		HWND hEdit = GetWindow(GW_CHILD);		
		ATLASSERT(hEdit);
		m_edit.SubclassWindow(hEdit);
		return lRet;
	}

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{	
		ModifyStyleEx (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
			0, SWP_FRAMECHANGED);
	
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);

		POINT pt;
		GetCursorPos(&pt);

		RECT rcItem;
		GetWindowRect(&rcItem);

		if ((PtInRect(&rcItem, pt)) || m_bHasFocus)
			DrawCombo( raised, m_clrBtnShadow, m_clrBtnHilite );
		else
			DrawCombo( normal, m_clrBtnFace, m_clrBtnFace );

		return lRet;
	}
	
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		if (m_timer == 0) {
			m_timer = ::SetTimer(0, 0, 50, (TIMERPROC)GetThunk());
			m_bPainted = FALSE;
			OnTimer(0, 0, 0, 0);
		}
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		return lRet;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		m_bLBtnDown = TRUE;
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		return lRet;
	}

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		m_bLBtnDown = FALSE;
		Invalidate();
		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		return lRet;
	}

	LRESULT OnSetFocusNotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_bHasFocus = TRUE;
		DrawCombo( raised, m_clrBtnShadow, m_clrBtnHilite );				
		return 0;
	}

	LRESULT OnKillFocusNotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_bHasFocus = FALSE;
		DrawCombo( normal, m_clrBtnFace, m_clrBtnFace );				
		return 0;
	}

	LRESULT OnEditUpdateNotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
			// if we are not to auto update the text, get outta here
		if (!m_bAutoComp) 
			return 0;

		// Get the text in the edit box		
		int nLength = GetWindowTextLength();
		TCHAR* pStr = (TCHAR *)_alloca((nLength + 1) * sizeof TCHAR);
		GetWindowText(pStr, nLength + 1);
		
		// Currently selected range
		DWORD dwCurSel = GetEditSel();
		WORD dStart = LOWORD(dwCurSel);
		WORD dEnd   = HIWORD(dwCurSel);

		// Search for, and select in, and string in the combo box that is prefixed
		// by the text in the edit box
		if (SelectString(-1, pStr) == CB_ERR)
		{
			SetWindowText(pStr);		// No text selected, so restore what was there before
			if (dwCurSel != CB_ERR)
				SetEditSel(dStart, dEnd);	//restore cursor postion
		}

		// Set the text selection as the additional text that we have added
		if (dEnd < nLength && dwCurSel != CB_ERR)
			SetEditSel(dStart, dEnd);
		else
			SetEditSel(nLength, -1);

		return 0;
	}

	LRESULT OnSysColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		m_clrBtnHilite  = ::GetSysColor(COLOR_BTNHILIGHT);
		m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
		m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);

		LRESULT lRet = DefWindowProc(uMsg, wParam, lParam);
		return lRet;
	}
	
	VOID OnTimer(HWND, UINT, UINT_PTR, DWORD)	
	{			
		POINT pt;
		GetCursorPos(&pt);
		RECT rcItem;
		GetWindowRect(&rcItem);

	// OnLButtonDown, show pressed.
		if (m_bLBtnDown==TRUE) {
			::KillTimer(0, m_timer);
			m_timer = 0;
			if (m_bPainted == TRUE) {
				DrawCombo( pressed, m_clrBtnShadow, m_clrBtnHilite );
				m_bPainted = FALSE;
			}
			return;
		}

	// If mouse leaves, show flat.
		if (!PtInRect(&rcItem, pt)) {
			::KillTimer(0, m_timer);
			m_timer = 0;
			if ((m_bPainted == TRUE) && !m_bHasFocus) {
				DrawCombo( normal, m_clrBtnFace, m_clrBtnFace );
				m_bPainted = FALSE;
			}
			return;
		}

	// On mouse over, show raised.
		else {
			if (m_bPainted == TRUE)
				return;
			else {
				m_bPainted = TRUE;
				DrawCombo( raised, m_clrBtnShadow, m_clrBtnHilite );
			}
		}
	}

public:
	BOOL PreTranslateMessage(MSG* pMsg)
	{
		if (pMsg->message == WM_SYSCHAR)
			return TRUE;
		return FALSE;
	}

public:

	// Call this member function to enable auto completion.
	//
	void EnableAutoCompletion(BOOL bEnable=TRUE)
		{ ATLASSERT(::IsWindow(m_hWnd)); m_bAutoComp = bEnable; }

protected:

	// this member function is called by the combo box whenever a paint
	// operation should occur.
	//
	void DrawCombo(STATE eState, COLORREF clrTopLeft, COLORREF clrBottomRight)
	{
		RECT rcItem;
		GetClientRect(&rcItem);		
		CClientDC dc(m_hWnd);
	
	// Cover up dark 3D shadow.
		dc.Draw3dRect(&rcItem, clrTopLeft, clrBottomRight);
		InflateRect(&rcItem, -1, -1);
	
		if (!IsWindowEnabled()) {
			dc.Draw3dRect( &rcItem, m_clrBtnHilite, m_clrBtnHilite );
		}	
		else {
			dc.Draw3dRect( &rcItem, m_clrBtnFace, m_clrBtnFace );
		}

	// Cover up dark 3D shadow on drop arrow.
		InflateRect(&rcItem, -1, -1);
		rcItem.left = rcItem.right-m_nOffset;
		dc.Draw3dRect( &rcItem, m_clrBtnFace, m_clrBtnFace );
	
	// Cover up normal 3D shadow on drop arrow.
		InflateRect(&rcItem, -1, -1);
		dc.Draw3dRect( &rcItem, m_clrBtnFace, m_clrBtnFace );
	
		if (!IsWindowEnabled()) {
			return;
		}

		switch (eState)
		{
		case normal:
			rcItem.top -= 1;
			rcItem.bottom += 1;
			dc.Draw3dRect( &rcItem, m_clrBtnHilite, m_clrBtnHilite );
			rcItem.left -= 1;
			dc.Draw3dRect( &rcItem, m_clrBtnHilite, m_clrBtnHilite );
			break;

		case raised:
			rcItem.top -= 1;
			rcItem.bottom += 1;
			dc.Draw3dRect( &rcItem, m_clrBtnHilite, m_clrBtnShadow);
			break;

		case pressed:
			rcItem.top -= 1;
			rcItem.bottom += 1;
			OffsetRect(&rcItem, 1, 1);
			dc.Draw3dRect( &rcItem, m_clrBtnShadow, m_clrBtnHilite );
			break;
		}
	}
};

typedef kFlatComboT<CComboBox> kFlatCombo;

#endif // !defined(AFX_KFLATCOMBO_H__48589D0D_EF0B_49C1_A176_5F28E5381598__INCLUDED_)
