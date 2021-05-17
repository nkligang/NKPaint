# Microsoft Developer Studio Project File - Name="NKPaint" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=NKPaint - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NKPaint.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NKPaint.mak" CFG="NKPaint - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NKPaint - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NKPaint - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NKPaint - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "./Common" /I "./Image" /I "./Addin" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 Image.lib /nologo /subsystem:windows /dll /machine:I386 /def:".\NKPaint.def" /out:"../Bin/NKPaint.dll" /implib:"../Lib/NKPaint.lib" /libpath:"../Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "NKPaint - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./Common" /I "./Image" /I "./Addin" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Imaged.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\NKPaint.def" /out:"../Bin/NKPaint.dll" /implib:"../Lib/NKPaintD.lib" /pdbtype:sept /libpath:"../Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "NKPaint - Win32 Release"
# Name "NKPaint - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Common\ADMMScrollview.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\AdvanceDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\AttributeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\ColorBoxStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\ColorCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\Common.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\DirDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\DrawObject.cpp
# End Source File
# Begin Source File

SOURCE=.\FlipRotateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MapDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NKPaint.cpp
# End Source File
# Begin Source File

SOURCE=.\NKPaint.def
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\NKPaint.rc
# End Source File
# Begin Source File

SOURCE=.\PaintBoxDlgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\PaintView.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StretchDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\structure.cpp
# End Source File
# Begin Source File

SOURCE=.\StyleListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolBoxBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\TrueColorToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\Utility.cpp
# End Source File
# Begin Source File

SOURCE=.\Common\Utils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Common\ADMMScrollview.h
# End Source File
# Begin Source File

SOURCE=.\Common\AdvanceDraw.h
# End Source File
# Begin Source File

SOURCE=.\AttributeDlg.h
# End Source File
# Begin Source File

SOURCE=.\BaseFrame.h
# End Source File
# Begin Source File

SOURCE=.\Common\ColorBoxStatic.h
# End Source File
# Begin Source File

SOURCE=.\Common\ColorCursor.h
# End Source File
# Begin Source File

SOURCE=.\Common\Common.h
# End Source File
# Begin Source File

SOURCE=.\Common\DirDialog.h
# End Source File
# Begin Source File

SOURCE=.\Common\DrawObject.h
# End Source File
# Begin Source File

SOURCE=.\FlipRotateDlg.h
# End Source File
# Begin Source File

SOURCE=.\MapDlg.h
# End Source File
# Begin Source File

SOURCE=.\NKPaint.h
# End Source File
# Begin Source File

SOURCE=.\NPRes.h
# End Source File
# Begin Source File

SOURCE=.\PaintBoxDlgBar.h
# End Source File
# Begin Source File

SOURCE=.\PaintDoc.h
# End Source File
# Begin Source File

SOURCE=.\PaintView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StretchDlg.h
# End Source File
# Begin Source File

SOURCE=.\Common\structure.h
# End Source File
# Begin Source File

SOURCE=.\StyleListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ToolBoxBar.h
# End Source File
# Begin Source File

SOURCE=.\Common\TrueColorToolBar.h
# End Source File
# Begin Source File

SOURCE=.\Common\Utility.H
# End Source File
# Begin Source File

SOURCE=.\Common\Utils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\865.bmp
# End Source File
# Begin Source File

SOURCE=.\res\866.bmp
# End Source File
# Begin Source File

SOURCE=.\res\867.bmp
# End Source File
# Begin Source File

SOURCE=.\res\869.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BF.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp102.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp859.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp862.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp863.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp864.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur1100.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur1201.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur1202.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur1203.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur1204.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur1205.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur1206.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur1207.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur135.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur8012.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur8025.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur8026.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur8027.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur8028.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur8029.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur8030.cur
# End Source File
# Begin Source File

SOURCE=.\res\ico114.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico115.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico116.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico117.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico2.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico3.ico
# End Source File
# Begin Source File

SOURCE=.\res\NKPaint.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\manifest.xml
# End Source File
# End Target
# End Project
