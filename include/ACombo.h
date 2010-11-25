#pragma once

#include "ListCtrl/ListCtrl.h"

class AdvCombo : public  CListCtrl
{
private:
	CListArray <CString>	tComboList;
	int						infoWidth;
	UINT					flag;


public:
	AdvCombo()
	{
		infoWidth = 120;
		flag = ITEM_FLAGS_CENTRE;
	}

	void SetInfoWidth(int width)
	{
		infoWidth = width;
	}

	void AddString(CString item) 
	{
		tComboList.Add(item);
	}

	void InitCtrl(HWND hWnd, BOOL bEditor = FALSE)
	{
		RECT rc;
		if(bEditor)
			flag |= ITEM_FLAGS_COMBO_EDIT;

		RegisterClass();
		ShowBorder(FALSE);
		Show3DColor();
		SubclassWindow( hWnd );
		ShowHeader(FALSE);
		ShowSelected(TRUE);
		GetClientRect(&rc);
		AddColumn(_T(""), infoWidth, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_NONE);
		AddColumn(_T(""), rc.right-rc.left-infoWidth, ITEM_IMAGE_NONE, FALSE, ITEM_FORMAT_COMBO, flag);
		AddItem(_T(""));
		SelectItem(0, 1);
		//SetItemColours(0, 1, GetSysColor(COLOR_WINDOW), GetSysColor(COLOR_WINDOWTEXT));
	}
	
	void SetTitle(CString title)
	{
		SetItemText(0, 0, title);
	}

	void SelecteText(CString selected)
	{
		SetItemFormat(0, 1, ITEM_FORMAT_COMBO, flag, tComboList);
		SetItemText(0, 1, selected);
	}

	void SelecteIndex(UINT index)
	{
		SetItemFormat(0, 1, ITEM_FORMAT_COMBO, flag, tComboList);
		SetItemComboIndex(0, 1, index);
	}

	int FindStringExactNoCase(UINT index, CString str)
	{
		for(int i = index; i < tComboList.GetSize(); i++) {
			if(!str.CompareNoCase(tComboList[i]))
				return i;
		}

		return -1;
	}

	int FindStringExact(UINT index, CString str)
	{
		for(int i = index; i < tComboList.GetSize(); i++) {
			if(!str.Compare(tComboList[i]))
				return i;
		}

		return -1;
	}

	CString GetSelectedString()
	{
		return GetItemText(0, 1);
	}

	int GetSelectedIndex()
	{
		return GetItemComboIndex(0, 1);
	}
};