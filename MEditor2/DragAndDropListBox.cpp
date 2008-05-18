// DragAndDropListBox.cpp : implementation file
//

#include "stdafx.h"
#include "DragAndDropListBox.h"

#define TID_SCROLLDOWN  100
#define TID_SCROLLUP    101

// CDragAndDropListBox

IMPLEMENT_DYNAMIC(CDragAndDropListBox, CListBox)
CDragAndDropListBox::CDragAndDropListBox()
: m_MovingIndex(-1)
, m_MoveToIndex(-1)
, m_Captured(FALSE)
, m_Interval(0)
{
}

CDragAndDropListBox::~CDragAndDropListBox()
{
}


BEGIN_MESSAGE_MAP(CDragAndDropListBox, CListBox)
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_TIMER()
END_MESSAGE_MAP()



// CDragAndDropListBox message handlers


void CDragAndDropListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
   CListBox::OnLButtonDown(nFlags, point);

   //clear all the flags
   m_MovingIndex = LB_ERR;
   m_MoveToIndex = LB_ERR;
   m_Captured = FALSE;
   m_Interval = 0;
   BOOL Outside;
   //find the item that they want to drag
   //and keep track of it. Later in the mouse move
   //we will capture the mouse if this value is set
   int Index = ItemFromPoint(point,Outside);
   if (Index != LB_ERR && !Outside)
   {
      m_MovingIndex = Index;
      SetCurSel(Index);
   }
}

void CDragAndDropListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
   if (m_MovingIndex != LB_ERR && m_Captured)
   {
      KillTimer(TID_SCROLLDOWN);
      KillTimer(TID_SCROLLUP);
      m_Interval = 0;
      m_Captured = FALSE;
      ReleaseCapture();

      CRect Rect;
      GetClientRect(&Rect);
      //if they are still within the listbox window
      if (Rect.PtInRect(point))
      {
         InsertDraggedItem();
      }
      else
      {
         Invalidate();
         UpdateWindow();
      }
      m_MovingIndex = LB_ERR;
      m_MoveToIndex = LB_ERR;
   }

   CListBox::OnLButtonUp(nFlags, point);
}


void CDragAndDropListBox::OnMouseMove(UINT nFlags, CPoint point)
{
   CListBox::OnMouseMove(nFlags, point);
   if (nFlags & MK_LBUTTON)
   {
      if (m_MovingIndex != LB_ERR && !m_Captured)
      {
         SetCapture();
         m_Captured = TRUE;
      }
      BOOL Outside;
      int Index = ItemFromPoint(point,Outside);
      //if they our not on a particular item
      if (Outside)
      {
         CRect ClientRect;
         GetClientRect(&ClientRect);

         //if they are still within the listbox window, then
         //simply select the last item as the drop point
         //else if they are outside the window then scroll the items
         if (ClientRect.PtInRect(point))
         {
            KillTimer(TID_SCROLLDOWN);
            KillTimer(TID_SCROLLUP);
            m_Interval = 0;
            Index = LB_ERR;
            Outside = FALSE;
         }
         else
         {
            DoTheScrolling(point,ClientRect);
         }
      }
      else
      {
         KillTimer(TID_SCROLLDOWN);
         KillTimer(TID_SCROLLUP);
         m_Interval = 0;
      }
      
      if (Index != m_MoveToIndex && !Outside)
      {
         DrawTheLines(Index);
      }
   }
}

void CDragAndDropListBox::DrawTheLines(int Index)
{
   CRect ClientRect;
   GetClientRect(&ClientRect);

   CDC *pDC = GetDC();

   CRect Rect;
   int Width = 2;
   if (m_MoveToIndex != m_MovingIndex)
   {
      CPen Pen(PS_SOLID,Width,RGB(255,255,255));
      CPen *pOldPen = pDC->SelectObject(&Pen);
      if (m_MoveToIndex != LB_ERR)
      {
         GetItemRect(m_MoveToIndex,&Rect);
         if (ClientRect.PtInRect(Rect.TopLeft()))
         {
            pDC->MoveTo(Rect.left,Rect.top+1);
            pDC->LineTo(Rect.right-(Width/2),Rect.top+1);
         }
      }
      else
      {
         GetItemRect(GetCount()-1,&Rect);
         if (ClientRect.PtInRect(CPoint(0,Rect.bottom)))
         {
            pDC->MoveTo(Rect.left,Rect.bottom);
            pDC->LineTo(Rect.right-(Width/2),Rect.bottom);
         }
      }
      pDC->SelectObject(pOldPen);
   }
   m_MoveToIndex = Index;
   if (m_MoveToIndex != m_MovingIndex)
   {
      CPen Pen(PS_SOLID,Width,RGB(0,0,0));
      CPen *pOldPen = pDC->SelectObject(&Pen);
      if (m_MoveToIndex != LB_ERR)
      {
         GetItemRect(Index,&Rect);
         if (ClientRect.PtInRect(Rect.TopLeft()))
         {
            pDC->MoveTo(Rect.left,Rect.top+1);
            pDC->LineTo(Rect.right-(Width/2),Rect.top+1);
         }
      }
      else
      {
         GetItemRect(GetCount()-1,&Rect);
         if (ClientRect.PtInRect(CPoint(0,Rect.bottom)))
         {
            pDC->MoveTo(Rect.left,Rect.bottom);
            pDC->LineTo(Rect.right-(Width/2),Rect.bottom);
         }
      }
      pDC->SelectObject(pOldPen);
   }
   ReleaseDC(pDC);
}
void CDragAndDropListBox::InsertDraggedItem()
{
   CString Text;
   unsigned long ItemData;
   GetText(m_MovingIndex,Text);
   ItemData = GetItemData(m_MovingIndex);
   if (m_MovingIndex < m_MoveToIndex)
   {
      SetRedraw(FALSE);
      int TopIndex = GetTopIndex();
      int NewIndex = InsertString(m_MoveToIndex,Text);
      SetItemData(NewIndex,ItemData);
      DeleteString(m_MovingIndex);
      SetCurSel(NewIndex-1);
      SetTopIndex(TopIndex);
      SetRedraw(TRUE);
      //we have to redraw the window since we are deleting a string
      //after we set the current selection. DeleteString causes a 
      //focus rect to show up under the current selection
      Invalidate();
      UpdateWindow();
   }
   else if (m_MovingIndex > m_MoveToIndex)
   {
      SetRedraw(FALSE);
      int TopIndex = GetTopIndex();
      DeleteString(m_MovingIndex);
      int NewIndex = InsertString(m_MoveToIndex,Text);
      SetItemData(NewIndex,ItemData);
      SetCurSel(NewIndex);
      SetTopIndex(TopIndex);
      SetRedraw(TRUE);
      //we have to redraw the window since we are deleting a string
      //after we set the current selection. DeleteString causes a 
      //focus rect to show up under the current selection
      Invalidate();
      UpdateWindow();
   }
   GetParent()->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),LBN_SELCHANGE),(LPARAM)m_hWnd);
}

void CDragAndDropListBox::DoTheScrolling(CPoint Point,CRect ClientRect)
{
   if (Point.y > ClientRect.Height())
   {
      DWORD Interval = 250 / (1+ ((Point.y-ClientRect.Height())/GetItemHeight(0)));
      if (Interval != m_Interval)
      {
         m_Interval = Interval;
         SetTimer(TID_SCROLLDOWN,Interval,NULL);
         OnTimer(TID_SCROLLDOWN);
      }
   }
   else if (Point.y < 0)
   {
      DWORD Interval = 250 / (1+(abs(Point.y)/GetItemHeight(1)));
      if (Interval != m_Interval)
      {
         m_Interval = Interval;
         SetTimer(TID_SCROLLUP,Interval,NULL);
         OnTimer(TID_SCROLLUP);
      }
   }
   else
   {
      KillTimer(TID_SCROLLDOWN);
      KillTimer(TID_SCROLLUP);
      m_Interval = 0;
   }
}

void CDragAndDropListBox::OnTimer(UINT nIDEvent)
{
   if (nIDEvent == TID_SCROLLDOWN)
   {
      DrawTheLines(m_MoveToIndex+1);
      SetTopIndex(GetTopIndex()+1);
   }
   else if (nIDEvent == TID_SCROLLUP)
   {
      DrawTheLines(m_MoveToIndex-1);
      SetTopIndex(GetTopIndex()-1);
   }

   CListBox::OnTimer(nIDEvent);
}
