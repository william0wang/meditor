
#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES -1
#endif
#define IsFileExist(lpszFileName) (::GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)
#define ResStr(id) CString(MAKEINTRESOURCE(id))
