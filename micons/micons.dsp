# Microsoft Developer Studio Project File - Name="micons" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=micons - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "micons.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "micons.mak" CFG="micons - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "micons - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "micons - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "micons - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ICONS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /w /W0 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ICONS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "micons - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ICONS_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ICONS_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "micons - Win32 Release"
# Name "micons - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\icons.cpp
# End Source File
# Begin Source File

SOURCE=.\icons.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icons\3gp.ico
# End Source File
# Begin Source File

SOURCE=.\icons\aac.ico
# End Source File
# Begin Source File

SOURCE=.\icons\ac3.ico
# End Source File
# Begin Source File

SOURCE=.\icons\amr.ico
# End Source File
# Begin Source File

SOURCE=.\icons\asf.ico
# End Source File
# Begin Source File

SOURCE=.\icons\audio.ico
# End Source File
# Begin Source File

SOURCE=.\icons\avi.ico
# End Source File
# Begin Source File

SOURCE=.\icons\dat.ico
# End Source File
# Begin Source File

SOURCE=.\icons\dts.ico
# End Source File
# Begin Source File

SOURCE=.\icons\flac.ico
# End Source File
# Begin Source File

SOURCE=.\icons\flv.ico
# End Source File
# Begin Source File

SOURCE=.\icons\list.ico
# End Source File
# Begin Source File

SOURCE=.\icons\m3u.ico
# End Source File
# Begin Source File

SOURCE=.\icons\m3u8.ico
# End Source File
# Begin Source File

SOURCE=.\icons\m4a.ico
# End Source File
# Begin Source File

SOURCE=.\icons\meditor.ico
# End Source File
# Begin Source File

SOURCE=.\icons\mka.ico
# End Source File
# Begin Source File

SOURCE=.\icons\mkv.ico
# End Source File
# Begin Source File

SOURCE=.\icons\mov.ico
# End Source File
# Begin Source File

SOURCE=.\icons\mp3.ico
# End Source File
# Begin Source File

SOURCE=.\icons\mp4.ico
# End Source File
# Begin Source File

SOURCE=.\icons\mpa.ico
# End Source File
# Begin Source File

SOURCE=.\icons\mpc.ico
# End Source File
# Begin Source File

SOURCE=.\icons\mpeg.ico
# End Source File
# Begin Source File

SOURCE=.\icons\ogg.ico
# End Source File
# Begin Source File

SOURCE=.\icons\ogm.ico
# End Source File
# Begin Source File

SOURCE=.\icons\pls.ico
# End Source File
# Begin Source File

SOURCE=.\icons\ra.ico
# End Source File
# Begin Source File

SOURCE=.\icons\rm.ico
# End Source File
# Begin Source File

SOURCE=.\icons\rmvb.ico
# End Source File
# Begin Source File

SOURCE=.\icons\tp.ico
# End Source File
# Begin Source File

SOURCE=.\icons\ts.ico
# End Source File
# Begin Source File

SOURCE=.\icons\video.ico
# End Source File
# Begin Source File

SOURCE=.\icons\vob.ico
# End Source File
# Begin Source File

SOURCE=.\icons\wav.ico
# End Source File
# Begin Source File

SOURCE=.\icons\wma.ico
# End Source File
# Begin Source File

SOURCE=.\icons\wmv.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
