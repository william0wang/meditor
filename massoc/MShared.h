//¹²Ïíº¯Êý

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES -1
#endif
#define IsFileExist(lpszFileName) (::GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)
#define ResStr(id) CString(MAKEINTRESOURCE(id))
void GetLanguageID();
char* UnicodeToLocal(CString instr,int &outlen);
CString LocalToUnicode(CString instr);
double StringToDouble(CString value_str);
void TestURL(CString url,int fullbyte);
bool SelectFolder(HWND hWnd,CString& strFolder);
BOOL WritePrivateProfileInt(CString section, CString key, int value, CString profile);
bool GetSpecialFolder(int nFolder , TCHAR *path);
void DeleteFolder(CString dir);
bool IsDigit(CString dig_str);
