#pragma once
#include <atlfile.h>

class CAtlStdioFile : public CAtlFile
{
private:
	static const int buffer_size = 128;
	TCHAR *buffer;

public:
	CAtlStdioFile()
	{
		buffer = new TCHAR[buffer_size];
	}
	~CAtlStdioFile()
	{
		delete buffer;
	}

	BOOL ReadLine(CString &str)
	{
		int sp, pos;
		CString tmp;
		DWORD readsize = 0, size;
		str = _T("");

		ZeroMemory(buffer, buffer_size*sizeof(TCHAR));
		while(Read(buffer, buffer_size, size) == S_OK) {
			if(size <= 0)
				break;
			readsize += size;
			tmp += buffer;
			sp = tmp.Find(_T('\n'));
			if(sp >= 0) {
				++sp;
				str = tmp.Left(sp);
				pos = sp*sizeof(TCHAR) - size;
				readsize+=pos;
				Seek(pos, FILE_CURRENT);
				break;
			}
			ZeroMemory(buffer, buffer_size*sizeof(TCHAR));
		}

		if(readsize <= 0)
			return FALSE;

		str.TrimRight(_T('\r'));
		str.TrimRight(_T('\n'));

		return TRUE;
	}

	HRESULT WriteLine(CString str)
	{
		HRESULT hr = S_OK;
		str += _T("\r\n");
		int len = str.GetLength();
		if(len > 1) {
			hr = Write(str.GetBuffer(), len*sizeof(TCHAR));
			str.ReleaseBuffer();
		}
		return hr;
	}
};
