// mloader.c : 应用程序的入口点。

#include "mloader.h"
#define MAX_URI 2048

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES -1
#endif

#define IsFileExist(lpszFileName) (GetFileAttributes((lpszFileName))   !=   INVALID_FILE_ATTRIBUTES)

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    int cmdlen =  strlen(lpszArgument) + 1;

    char szFilePath[MAX_URI], szIniPath[MAX_URI];
    GetModuleFileName(NULL, szFilePath, MAX_URI);
    (strrchr(szFilePath, '\\'))[1] = 0;

    strncpy(szIniPath , szFilePath, MAX_URI);
    strncat(szIniPath , "mloader.ini", MAX_URI);

    char * mainpath = strstr(szFilePath,"\\codecs\\Real");
    if(mainpath)
    	mainpath[1] = 0;
    strncat(szFilePath , "mplayer.exe", MAX_URI);

    char *load_command = (char *)malloc(sizeof(char) * (MAX_URI + cmdlen));
    load_command[0] = 0;
    if(IsFileExist(szIniPath))
    	GetPrivateProfileString("Command", "Program_Path", "", load_command, MAX_URI, szIniPath);
    if(strlen(load_command) <= 2)
    	strncpy(load_command , szFilePath, MAX_URI);

    strncat(load_command, " ", MAX_URI + cmdlen);

    mainpath = strstr(lpszArgument,"/command:Screensize(full)");
    if(mainpath)
    {
    	int len = strlen("/command:Screensize(full)");
    	lpszArgument += len;
    }

    strncat(load_command, lpszArgument, MAX_URI + cmdlen);
    WinExec(load_command, SW_SHOW);

	return TRUE;
}
