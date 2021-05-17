#include "StdAfx.h"
#include "Common.h"
#include "Utils.h"
#include <direct.h>
#include "DirDialog.h"

BOOL BrowseFold(HWND hwndOwner, CString & strCurDir)
{
	LPITEMIDLIST	lpi = NULL;
	TCHAR			szBuf[MAX_PATH];
	
	CString		strPrompt(_T("Select Folder..."));
	BROWSEINFO brinf = {	\
		hwndOwner,
		NULL,
		strCurDir.GetBuffer(0),
		strPrompt,
		BIF_RETURNONLYFSDIRS,
		NULL,
		NULL,
		0
	};	
	lpi = SHBrowseForFolder(&brinf);
	if (!lpi)
		return FALSE;
	if (!SHGetPathFromIDList(lpi, szBuf))
		return FALSE;

	strCurDir.Format(_T("%s\\"), szBuf);
	return TRUE;
}

BOOL SelectDirectory(CWnd * pParent, LPCTSTR lpszTitle, LPCTSTR lpszWindowTitle, CString & strCurDir)
{
	CDirDialog dlgDir;
	dlgDir.m_strTitle		= lpszTitle;
	dlgDir.m_strWindowTitle = lpszWindowTitle;
	dlgDir.m_strSelDir		= strCurDir;
	if (dlgDir.DoBrowse(pParent) == IDOK)
	{
		strCurDir = dlgDir.m_strPath;
		int nLen = strCurDir.GetLength();
		// Append backslash if necessary
		if ((nLen > 0) && 
			(strCurDir[nLen - 1] != TCHAR('\\'))) 
			strCurDir += _T("\\");
		return TRUE;
	}
	return FALSE;
}

BOOL SelectDirectory(CWnd * pParent, UINT uTitle, UINT uWindowTitle, CString & strCurDir)
{
	CString strTitle, strWindowTitle;
	CDirDialog dlgDir;
	strTitle.LoadString(uTitle);
	strWindowTitle.LoadString(uWindowTitle);
	dlgDir.m_strTitle		= strTitle;
	dlgDir.m_strWindowTitle = strWindowTitle;
	dlgDir.m_strSelDir		= strCurDir;
	if (dlgDir.DoBrowse(pParent) == IDOK)
	{
		strCurDir = dlgDir.m_strPath;
		int nLen = strCurDir.GetLength();
		// Append backslash if necessary
		if ((nLen > 0) && 
			(strCurDir[nLen - 1] != TCHAR('\\'))) 
			strCurDir += _T("\\");
		return TRUE;
	}
	return FALSE;
}

//	return value:
//	0 -- directory exist
//	1 -- directory not exist
//	2 -- directory name not a absolute name
//	3 -- directory name is empty string
int _GetDirInfo(LPCTSTR szDir)
{
    int     nReturn = 0;

	CString		sMake;
	sMake	= szDir;
	if (sMake.IsEmpty())
		return 3;

	//step1:cut the backslash at the end
	if (sMake[sMake.GetLength()-1]	== BACKSLASH)
		sMake.SetAt(sMake.GetLength()-1,_T('\0'));
	if (sMake.IsEmpty())
		return 2;

	//step2:judge the existence of the dir
    TCHAR    szCurrent[_MAX_PATH];
    _tgetcwd(szCurrent, sizeof(szCurrent)-1);
			
 	unsigned	status;
    status = _tchdir(LPCTSTR(sMake));
    nReturn = (status==0) ? 0 : 1; 

    _tchdir(szCurrent);

	//step3:prompt accepting absolute path only
	if (nReturn == 1)
	{
		LPTSTR	pCur = (LPTSTR)szDir;//sMake.GetBuffer(sMake.GetLength());
		if (*pCur == BACKSLASH) {
			//	Must begin with a network user name
			pCur ++;
			if (*pCur++ != BACKSLASH)
				nReturn = 2;
			while ((*pCur != _T('\0')) && (*pCur++ != BACKSLASH));
		}
		else {
			//	Must begin with a driver name 
			pCur ++;
			if (*pCur++ != _T(':'))
				nReturn = 2;
		}

		if (*pCur == _T('\0'))
			nReturn = 2;
		
//		sMake.ReleaseBuffer();
		pCur	= NULL;
	}

    return  nReturn;
}

UINT _CreateFolder(LPCTSTR szDir)
{
    int     nReturn = 0;

	CString		sMake;
	sMake	= szDir;
	if (sMake.IsEmpty())
		return 1;

	//cut the backslash at the end
	if (sMake[sMake.GetLength()-1]	== BACKSLASH)
		sMake.SetAt(sMake.GetLength()-1,_T('\0'));
	if (sMake.IsEmpty())
		return 1;

	unsigned	status;
    TCHAR    szCurrent[_MAX_PATH];
    _tgetcwd(szCurrent, sizeof(szCurrent)-1);
    status = _tmkdir(sMake);
    nReturn = (status==0) ? 0 : 1; 
    _tchdir(szCurrent);

    return  nReturn;
}

BOOL _RCreateFolder(LPCTSTR lpDir)
{
	int		nLen;
	TCHAR	szDir[_MAX_PATH];
    BOOL    bReturn = TRUE;
	LPTSTR	pLevel[_MAX_PATH];
	short	nCur, nCreateStart;
	LPTSTR	pCur;
	TCHAR	ch;

	//	1. Pass through driver name and network user name
	lstrcpy(szDir, lpDir);
	nLen = lstrlen(szDir);
	if (szDir[nLen-1] == BACKSLASH)
		szDir[nLen-1] = _T('\0');

	pCur = szDir;
	if (*pCur == BACKSLASH) {
		//	Must begin with a network user name
		pCur ++;
		if (*pCur++ != BACKSLASH)
			bReturn = FALSE;
		else 
			while ((*pCur != _T('\0')) && (*pCur++ != BACKSLASH));
	}
	else {
		//	Must begin with a driver name 
		pCur ++;
		if (*pCur++ != _T(':'))
			bReturn = FALSE;
		else if (*pCur == BACKSLASH)
			pCur ++;
	}

	if (*pCur == _T('\0'))
		bReturn = FALSE;

	if (bReturn == TRUE) {
		char	szCurrent[_MAX_PATH];
		getcwd(szCurrent, sizeof(szCurrent)-1);
		
		nCur = -1;
		nCreateStart = -1;

		//	2. Recursive create directory
		while (bReturn) {
			if ((*pCur == BACKSLASH) || (*pCur == _T('\0'))) {
				ch = *pCur;
				
				pLevel[++nCur] = pCur;
				*pCur = _T('\0');
				if (_tchdir(szDir) != 0) {
					if (_tmkdir(szDir) == 0) {
						if (nCreateStart == -1)
							nCreateStart = nCur;
					}
					else
						bReturn = FALSE;
				}
				
				*pCur = ch; 
			}

			if (*pCur == _T('\0'))
				break;
			else
				pCur ++;
		}

		//	3. If no success, recursive remove directory
		if ((bReturn == FALSE) && (nCreateStart != -1)) {
			for (; nCur >= nCreateStart; nCur--) {
				ch = *(pLevel[nCur]);

				*(pLevel[nCur]) = _T('\0');
				if (_trmdir(szDir) != 0) {
					*(pLevel[nCur]) = ch;
					break;
				}

				*(pLevel[nCur]) = ch;
			}
		}

		chdir(szCurrent);
	}

    return  bReturn;
}

CString FileExtFilter(CStringArray strArrExt)
{
	int iIndex;
	CString strFilter(_T(""));
	for (iIndex = 0; iIndex < strArrExt.GetSize(); iIndex++)
	{
		strFilter += strArrExt.GetAt(iIndex) + SEMICOLON;
	}
	strFilter.TrimRight(SEMICOLON);
	return strFilter;
}

void GetFileExtArray(CString strFilter, CStringArray & strArrExt)
{
	int iStart, iEnd;

	strArrExt.RemoveAll();
	iStart = 0;
	iEnd = strFilter.Find(SEMICOLON, iStart);
	while (iEnd > 0)
	{
		strArrExt.Add(strFilter.Mid(iStart, iEnd - iStart));
		iStart = iEnd + 1;
		iEnd = strFilter.Find(SEMICOLON, iStart);
	}
	iEnd = strFilter.GetLength();
	if (iEnd > iStart)
		strArrExt.Add(strFilter.Mid(iStart, iEnd - iStart));
}

BOOL IsFileExtExist(CString strExt, CStringArray & strArrExt)
{
	int iIndex;
	for (iIndex = 0; iIndex < strArrExt.GetSize(); iIndex++)
	{
		if (!strExt.CompareNoCase(strArrExt.GetAt(iIndex)))
			return TRUE;
	}
	return FALSE;
}

BOOL GetExportFileName(CString strSrcPathName, CString strSrcFullName, CString strDestPathName, CString & strDestFullName)
{
	CString	sRet;
	CString	sSrcFullName;//source file full name
	CString sSrcPathName;//source file path only
	CString	sCurPathName;//current relative path
	CString	sCurFileName;//current file name only
	CString	sTmp;

	strDestFullName = _T("");
	sRet = strDestPathName;
	sSrcFullName = strSrcFullName;
	sSrcPathName = strSrcPathName;
	sCurFileName = sSrcFullName.Right(sSrcFullName.GetLength()-sSrcFullName.ReverseFind('\\')-1);
	sTmp = sSrcFullName.Left(sSrcFullName.GetLength() - sCurFileName.GetLength());
	sCurPathName = sTmp.Right(sTmp.GetLength()-sSrcPathName.GetLength());

	//ensure that the destination directory is existed
	CString	sTmpPath = sCurPathName;
	
	while (!sTmpPath.IsEmpty())
	{
		sRet += sTmpPath.Left(sTmpPath.Find(_T('\\'))+1);
		if (_GetDirInfo(sRet))
			if (_CreateFolder(sRet))
				return FALSE;
		sTmpPath = sTmpPath.Right(sTmpPath.GetLength()-sTmpPath.Find(_T('\\'))-1);
	}

	//exported file with the suitable expanded name
	sRet += sCurFileName;
	sRet = sRet.Left(sRet.GetLength()-3);

	CString	strExt;
	sRet += "jpg";

	strDestFullName	= sRet;
	return TRUE;
}

BOOL GetExportFileInfo(CString strSrcPathName, CString strSrcFullName, CString strDestPathName, CStringArray & arrInfo)
{
	CString	sRet;
	CString	sSrcFullName;//source file full name
	CString sSrcPathName;//source file path only
	CString	sCurPathName;//current relative path
	CString	sCurFileName;//current file name only
	CString	sTmp;

	arrInfo.RemoveAll();
	sRet = strDestPathName;
	sSrcFullName = strSrcFullName;
	sSrcPathName = strSrcPathName;
	sCurFileName = sSrcFullName.Right(sSrcFullName.GetLength()-sSrcFullName.ReverseFind('\\')-1);
	sTmp = sSrcFullName.Left(sSrcFullName.GetLength() - sCurFileName.GetLength());
	sCurPathName = sTmp.Right(sTmp.GetLength()-sSrcPathName.GetLength());

	//ensure that the destination directory is existed
	CString	sTmpPath = sCurPathName;
	
	while (!sTmpPath.IsEmpty())
	{
		sRet += sTmpPath.Left(sTmpPath.Find(_T('\\'))+1);
		if (_GetDirInfo(sRet))
			if (_CreateFolder(sRet))
				return FALSE;
		sTmpPath = sTmpPath.Right(sTmpPath.GetLength()-sTmpPath.Find(_T('\\'))-1);
	}
	arrInfo.Add(sRet);
	sTmp = sCurFileName.Left(sCurFileName.ReverseFind('.'));
	arrInfo.Add(sTmp);
	sTmp = sCurFileName.Right(sCurFileName.GetLength()-sCurFileName.ReverseFind('.') - 1);
	arrInfo.Add(sTmp);
	return TRUE;
}

CString GetParentDir(CString strFullPath)
{
	return strFullPath.Left( strFullPath.GetLength() - strFullPath.ReverseFind('\\') - 1);
}

int FindSubString(LPCTSTR lpszRef, LPCTSTR lpszSub, int nStart)
{
	ASSERT(AfxIsValidString(lpszRef));
	ASSERT(AfxIsValidString(lpszSub));

	int nLength = _tcsclen(lpszRef);
	if (nStart > nLength)
		return -1;

	// find first matching substring
	LPCTSTR lpsz = _tcsstr(lpszRef + nStart, lpszSub);

	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - lpszRef);
}
