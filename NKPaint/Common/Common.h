#ifndef __COMMON_H_INCLUDED__
#define __COMMON_H_INCLUDED__

BOOL BrowseFold(HWND hwndOwner, CString & strCurDir);
BOOL SelectDirectory(CWnd * pParent, LPCTSTR lpszTitle, LPCTSTR lpszWindowTitle, CString & strCurDir);
BOOL SelectDirectory(CWnd * pParent, UINT uTitle, UINT uWindowTitle, CString & strCurDir);
int _GetDirInfo(LPCTSTR szDir);
BOOL _RCreateFolder(LPCTSTR lpDir);
UINT _CreateFolder(LPCTSTR szDir);
CString FileExtFilter(CStringArray strArrExt);
void GetFileExtArray(CString strFilter, CStringArray & strArrExt);
BOOL IsFileExtExist(CString strExt, CStringArray & strArrExt);
BOOL GetExportFileName(CString strSrcPathName, CString strSrcFullName, CString strDestPathName, CString & strDestFullName);
CString GetParentDir(CString strFullPath);

BOOL GetExportFileInfo(CString strSrcPathName, CString strSrcFullName, CString strDestPathName, CStringArray & arrInfo);

int FindSubString(LPCTSTR lpszRef, LPCTSTR lpszSub, int nStart);

#endif // !__COMMON_H_INCLUDED__