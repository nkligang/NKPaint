# Microsoft Developer Studio Project File - Name="gdiplus" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=gdiplus - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "gdiplus.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "gdiplus.mak" CFG="gdiplus - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "gdiplus - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "gdiplus - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0

MTL=midl.exe

!IF  "$(CFG)" == "gdiplus - Win32 Release"

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

!ELSEIF  "$(CFG)" == "gdiplus - Win32 Debug"

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

!ENDIF 

# Begin Target

# Name "gdiplus - Win32 Release"
# Name "gdiplus - Win32 Debug"
# Begin Group "DLL"

# PROP Default_Filter "dll"
# Begin Source File

SOURCE=.\gdiplus.dll
# End Source File
# End Group
# Begin Group "LIB"

# PROP Default_Filter "lib"
# Begin Source File

SOURCE=.\GdiPlus.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\GdiPlus.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusBase.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusBitmap.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusBrush.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusCachedBitmap.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusColor.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusColorMatrix.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusEnums.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusFlat.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusFont.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusFontCollection.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusFontFamily.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusGpStubs.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusGraphics.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusHeaders.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusimageAttributes.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusImageCodec.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusImaging.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusInit.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusLineCaps.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusMatrix.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusMem.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusMetaFile.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusMetaHeader.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusPath.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusPen.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusPixelFormats.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusRegion.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusStringFormat.h
# End Source File
# Begin Source File

SOURCE=.\GdiPlusTypes.h
# End Source File
# Begin Source File

SOURCE=.\myGdiPlus.h
# End Source File
# End Group
# Begin Group "TXT"

# PROP Default_Filter "txt"
# Begin Source File

SOURCE=.\license.txt
# End Source File
# Begin Source File

SOURCE=.\redist.txt
# End Source File
# End Group
# End Target
# End Project
