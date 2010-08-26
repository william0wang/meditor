#pragma once
#include <atlfile.h>
#include <string>

std::wstring mutil2unicode(std::string mut_str, UINT code_page)
{
	std::wstring outstr;

	int nWideByteLength = MultiByteToWideChar(code_page,0, mut_str.c_str(), -1, NULL,0);

	if(nWideByteLength <= 0)
		nWideByteLength = mut_str.length();

	wchar_t * buff = new wchar_t[nWideByteLength];
	memset(buff, 0, sizeof(wchar_t) * nWideByteLength);

	MultiByteToWideChar(code_page, 0, mut_str.c_str(), -1, buff, nWideByteLength);

	outstr = buff;
	delete buff;

	return outstr;
}

class CAtlStdioFile : public CAtlFile
{
private:
	static const int buffer_size = 128;
	char *buffer;

public:
	CAtlStdioFile()
	{
		buffer = new char[buffer_size];
	}
	~CAtlStdioFile()
	{
		delete buffer;
	}

	BOOL ReadLine(CString &str,int codepage)
	{
		int sp, pos;
		CString tmp;
		DWORD readsize = 0, size;
		str = _T("");

		ZeroMemory(buffer, buffer_size);
		while(Read(buffer, buffer_size, size) == S_OK) {
			if(size <= 0)
				break;
			readsize += size;
			tmp += mutil2unicode(buffer, codepage).c_str();
			sp = tmp.Find(_T('\n'));
			if(sp >= 0) {
				++sp;
				str = tmp.Left(sp);
				pos = sp - size;
				readsize+=pos;
				Seek(pos, FILE_CURRENT);
				break;
			}
			ZeroMemory(buffer, buffer_size);
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
