; nkpaint.nsi
;
; This script is based on example1.nsi, but it remember the directory, 
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

; The name of the installer
Name "NKPaint"

!ifndef VERSION
  !define VERSION 'v1.1'
!endif

;--------------------------------
;Configuration

!ifdef OUTFILE
  OutFile "${OUTFILE}"
!else
  OutFile NKPaint-${VERSION}-setup.exe
!endif

; The default installation directory
InstallDir $PROGRAMFILES\NKPaint

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\NKPaint" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "NKPaint (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "Bin\NKPaint.exe"
  File "Bin\Image.dll"
  File "Bin\MFCO42D.DLL"
  File "Bin\MFC42D.DLL"
  File "Bin\MSVCRTD.DLL"
  File "Bin\NKPaint.dll"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\NKPaint "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NKPaint" "DisplayName" "NKPaint"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NKPaint" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NKPaint" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NKPaint" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\NKPaint"
  CreateShortCut "$SMPROGRAMS\NKPaint\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\NKPaint\NKPaint.lnk" "$INSTDIR\NKPaint.exe" "" "$INSTDIR\NKPaint.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NKPaint"
  DeleteRegKey HKLM SOFTWARE\NKPaint

  ; Remove files and uninstaller
  Delete $INSTDIR\*.*

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\NKPaint\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\NKPaint"
  RMDir "$INSTDIR"

SectionEnd
