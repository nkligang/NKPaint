del /S *.ncb
del /S *.obj
del /S *.pch
del /S *.sbr
del /S *.bsc
del /S *.idb
del /S *.pdb
del /S *.exp
del /S *.ilk
del /S *.dep
@ECHO off
cls
:start
ECHO 1. Preserve compressed files
ECHO 2. Delete compressed files
set choice=
set /p choice=Type the number to deal with compressed files:
if not '%choice%'=='' set choice=%choice:~0,1%
if '%choice%'=='1' goto Preserve
if '%choice%'=='2' goto Delete
ECHO "%choice%" is not valid please try again
ECHO.
goto start
:Preserve
rar a -r -x*.ncb -x*.rar -x*.ilk -x*.exp -x*.db -ag_YYYY_MM_DD-HH_MM_SS NKPaint
goto end
:Delete
rar a -r -x*.ncb -x*.rar -x*.ilk -x*.exp -x*.db -df -ag_YYYY_MM_DD-HH_MM_SS NKPaint
goto end
:end
echo done.
exit 