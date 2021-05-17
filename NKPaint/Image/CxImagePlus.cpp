#include "StdAfx.h"
#include "CxImagePlus.h"

#ifdef CXIMAGE_LIB
#include "ximage.h"
#pragma comment(lib, "png.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "tiff.lib")
#pragma comment(lib, "jbig.lib")
#pragma comment(lib, "jasper.lib")
#pragma comment(lib, "mng.lib")
#pragma comment(lib, "cximage.lib")
#endif

void CxImage_Init()
{

}

void CxImage_DeInit()
{

}

HRESULT CxImage_LoadImageFromFile(HBITMAP * pHBitmap, LPCTSTR pszFileName)
{
#ifndef CXIMAGE_LIB
	return (E_NOTIMPL);
#else
	CxImage * image;
	const char *nametype;				  //type part of name
	nametype = strrchr (pszFileName, '.');//find extension
	++nametype;

	if (lstrlen(nametype) < 3) return ( E_FAIL );
	int type = CxImage::GetTypeIdFromName(nametype);
	if (type == 0) return ( E_FAIL );
	image = new CxImage(pszFileName, type);

	*pHBitmap = image->MakeBitmap();
	delete image;
	if(*pHBitmap != NULL)
	{
		return ( NOERROR );
	}
	return ( E_FAIL );
#endif
}

HRESULT CxImage_SaveImageToFile(HBITMAP hBitmap, LPCTSTR pszFileName)
{
#ifndef CXIMAGE_LIB
	return (E_NOTIMPL);
#else
	CxImage image;
	const char *nametype;				  //type part of name
	nametype = strrchr (pszFileName, '.');//find extension
	BOOL bRet;

	int type = CxImage::GetTypeIdFromName(++nametype);

	if (image.CreateFromHBITMAP(hBitmap))
	{
		bRet = image.Save(pszFileName,type);
		if (bRet)
			return ( NOERROR );
	}
	return ( E_FAIL );
#endif
}

HBITMAP CxImage_Exposure(HBITMAP hBitmap, FLOAT fGamma, LONG lBrightness, LONG lContrast)
{
#ifndef CXIMAGE_LIB
	return (FALSE);
#else
	CxImage image;
	image.CreateFromHBITMAP(hBitmap);		// CxImage will release it
	image.Gamma(fGamma);
	image.Light(lBrightness, lContrast);
	return image.MakeBitmap();				// Create new bitmap
#endif
}