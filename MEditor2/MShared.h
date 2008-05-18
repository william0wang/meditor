//¹²Ïíº¯Êý

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES -1
#endif
#define IsFileExist(lpszFileName) (::GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)
void GetLanguageID();
char* UnicodeToLocal(CString instr,int &outlen);
CString LocalToUnicode(CString instr);
double StringToDouble(CString value_str);
void TestURL(CString url,int fullbyte);
bool SelectFolder(HWND hWnd,CString& strFolder);
BOOL WritePrivateProfileInt(CString section, CString key, int value, CString profile);
bool UnGZipFile(CString sourcefile,CString destfile);
bool UnRarFile(CString fn , CString Path = _T(""));
bool Decode7zFile(CString filename , CString Path = _T("") , CString ex_name = _T(""));
bool ExtractResource( LPCTSTR lpName, LPCTSTR lpType, LPCTSTR lpFilePath, bool bOverWrite,bool unzip = false,CString ex_filename = _T(""));
bool GetSpecialFolder(int nFolder , TCHAR *path);
void DeleteFolder(CString dir);