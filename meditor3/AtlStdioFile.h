#pragma once
#include <atlfile.h>

class CAtlStdioFile : public CAtlFile
{
private:
	static const int buffer_size = 512;
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

	HRESULT OpenFile(LPCTSTR szFilename, DWORD dwDesiredAccess, DWORD dwShareMode,
		DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL)
	{
		BYTE buffer_byte[2];
		DWORD readsize = 0;
		if(SUCCEEDED(Create(szFilename, dwDesiredAccess, dwShareMode, dwCreationDisposition, dwFlagsAndAttributes))) {
			if((dwDesiredAccess & GENERIC_READ) && (dwCreationDisposition & OPEN_EXISTING)) {
				if(FAILED(Read(buffer_byte, 2, readsize))){
					Close();
				} else {
					if(readsize == 2) {
						if((buffer_byte[0] == 0xff && buffer_byte[1] == 0xfe) ||
							(buffer_byte[0] == 0xfe && buffer_byte[1] == 0xff))
							return S_OK;
					}
					Seek(0, FILE_BEGIN);
					return S_OK;
				}
			} else {
				return S_OK;
			}
		}
		return S_FALSE;
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

		str.TrimRight(_T('\n'));
		str.TrimRight(_T('\r'));

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
