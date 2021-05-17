#include "StdAfx.h"
#include "GdiPlus.h"

#ifdef GDIPLUS_LIB
#include "myGdiPlus.h" 
using namespace Gdiplus; 

ULONG_PTR gdiplusToken;
#endif

void GdiPlus_Init()
{
#ifdef GDIPLUS_LIB
//begin: initialize GDI+  
	GdiplusStartupInput gdiplusStartupInput;
    VERIFY(GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL ) == Ok );
//end: initialize GDI+
#endif
}

void GdiPlus_DeInit()
{
#ifdef GDIPLUS_LIB
//begin: shutdown GDI+ 
	GdiplusShutdown(gdiplusToken);
//end: shutdown GDI+
#endif
}

HRESULT GdiPlus_LoadImageFromFile(HBITMAP * pHBitmap, LPCTSTR pszFileName)
{
#ifndef GDIPLUS_LIB
	return (E_NOTIMPL);
#else
	CString strFileName(pszFileName);
	Color color(255, 255, 255);
	Bitmap * bitmap = Bitmap::FromFile( strFileName.AllocSysString() );
	Status status = bitmap->GetHBITMAP(color, pHBitmap);
	if (status == Ok)
		return (NOERROR);
	return ( E_FAIL );
#endif
}

HRESULT GdiPlus_SaveImageToFile(HBITMAP hBitmap, LPCTSTR pszFileName)
{
#ifndef GDIPLUS_LIB
	return (E_NOTIMPL);
#else
	return ( E_FAIL );
#endif
}

HBITMAP GdiPlus_Exposure(HBITMAP hBitmap, FLOAT fGamma, LONG lBrightness, LONG lContrast)
{
#ifndef GDIPLUS_LIB
	return (NULL);
#else
	return (NULL);
#endif
}