
//
#include "stdafx.h"
#include "Utility.H"

	CString CUtility ::GenerateID()
	{
		CString strID;

		SYSTEMTIME sysTime;
		::GetSystemTime(&sysTime);
		DWORD dwTickCount = ::GetTickCount();
		DWORD dwRandom = rand();
		strID.Format(_T("%04d%02d%02d%d%d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, dwTickCount, dwRandom);
		
		return strID;		
	}

	void CUtility::PrepareDC(HDC hdc, float fZoomScale)
	{
		::SetMapMode(hdc,MM_ISOTROPIC);
		int XLogMm = ::GetDeviceCaps(hdc, HORZSIZE);
		int YLogMm = ::GetDeviceCaps(hdc, VERTSIZE);
		int XLogPix = ::GetDeviceCaps(hdc, HORZRES);
		int YLogPix = ::GetDeviceCaps(hdc, VERTRES);
		::SetWindowExtEx(hdc,XLogMm*100,YLogMm*100,NULL);
		::SetViewportExtEx(hdc,(int)(XLogPix*fZoomScale), (int)(YLogPix*fZoomScale),NULL);	
	}

