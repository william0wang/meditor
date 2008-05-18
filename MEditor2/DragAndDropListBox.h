#pragma once


// CDragAndDropListBox

class CDragAndDropListBox : public CListBox
{
	DECLARE_DYNAMIC(CDragAndDropListBox)

public:
	CDragAndDropListBox();
	virtual ~CDragAndDropListBox();

protected:
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
   int   m_MovingIndex;
   int   m_MoveToIndex;
   BOOL  m_Captured;
   DWORD m_Interval;    //scroll speed when an item is dragged above or below the window

   void InsertDraggedItem();
   void DoTheScrolling(CPoint Point,CRect ClientRect);
   void DrawTheLines(int Index);
};


