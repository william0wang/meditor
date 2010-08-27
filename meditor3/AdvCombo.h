#pragma once

class CAdvComboBox : public CComboBox
{
private:
	int m_last_size;
	int m_last_width;
	BOOL m_auto_resize;

public:
	CAdvComboBox()
	{
		m_last_size = 0;
		m_last_width = 0;
		m_auto_resize = FALSE;
	}
	CAdvComboBox& operator = (HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	void SetAutoResize(BOOL auto_resize)
	{
		m_auto_resize = auto_resize;
	}

	int AddString(CString str)
	{
		if(m_auto_resize) {
			SIZE sz;
			int size = str.GetLength();
			if(size > m_last_size) {
				m_last_size = size;
				CDCHandle hDC = GetDC();
				hDC.GetTextExtent(str, m_last_size, &sz);
				if(!m_last_width)
					m_last_width = GetDroppedWidth();
				if(sz.cx > m_last_width) {
					m_last_width = sz.cx;
					SetDroppedWidth(m_last_width);
				}
			}
		}

		return CComboBox::AddString(str);
	}
};
