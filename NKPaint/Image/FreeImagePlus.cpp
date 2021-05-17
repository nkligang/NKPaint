#include "StdAfx.h"
#include "FreeImagePlus.h"

#ifdef FREEIMAGE_LIB
#include "FreeImage.h"
#ifdef _DEBUG
#pragma comment(lib, "FreeImaged.lib")
#else
#pragma comment(lib, "FreeImage.lib")
#endif

FIBITMAP * Open_Image(LPCTSTR lpszPathName);
HBITMAP MakeBitmap(HDC hDC, LPBITMAPINFO pBitmapInfo, BYTE * pBits);
BOOL setSize(FIBITMAP ** ppDib, FREE_IMAGE_TYPE image_type, WORD width, WORD height, WORD bpp, unsigned red_mask = 0, unsigned green_mask = 0, unsigned blue_mask = 0);
BOOL CopyFromBitmap(FIBITMAP ** ppDib, HBITMAP hbmp);

FIBITMAP * Open_Image(LPCTSTR lpszPathName)
{
	FIBITMAP * dib = NULL;
	try {
		FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(lpszPathName);

		dib = FreeImage_Load(fif, lpszPathName, 0);
		if(!dib) throw(1);
		
		return dib;
	} 
	catch(int) {
		if(dib) FreeImage_Unload(dib);
		return NULL;
	}
	
	return NULL; 
}

HBITMAP MakeBitmap(HDC hDC, LPBITMAPINFO pBitmapInfo, BYTE * pBits)
{
	if (!pBits || !pBitmapInfo)
		return NULL;

	if (!hDC){
		HDC hMemDC = CreateCompatibleDC(NULL);
		LPVOID pBit32;
		HBITMAP bmp = CreateDIBSection(hMemDC,(LPBITMAPINFO)pBitmapInfo,DIB_RGB_COLORS, &pBit32, NULL, 0);
		if (pBit32) memcpy(pBit32, pBits, pBitmapInfo->bmiHeader.biSizeImage);
		DeleteDC(hMemDC);
		return bmp;
	}

	LPVOID pBit32;
	HBITMAP bmp = CreateDIBSection(hDC, (LPBITMAPINFO)pBitmapInfo, DIB_RGB_COLORS, &pBit32, NULL, 0);
	if (pBit32) memcpy(pBit32, pBits, pBitmapInfo->bmiHeader.biSizeImage);
	return bmp;
}

BOOL setSize(FIBITMAP ** ppDib, FREE_IMAGE_TYPE image_type, WORD width, WORD height, WORD bpp, unsigned red_mask, unsigned green_mask, unsigned blue_mask) {
	FIBITMAP * dib = *ppDib;
	if(dib) {
		FreeImage_Unload(dib);
	}
	if((dib = FreeImage_AllocateT(image_type, width, height, bpp, red_mask, green_mask, blue_mask)) == NULL)
		return FALSE;

	if(image_type == FIT_BITMAP) {
		// Create palette if needed
		switch(bpp)	{
			case 1:
			case 4:
			case 8:
				RGBQUAD *pal = FreeImage_GetPalette(dib);
				for(unsigned i = 0; i < FreeImage_GetColorsUsed(dib); i++) {
					pal[i].rgbRed = i;
					pal[i].rgbGreen = i;
					pal[i].rgbBlue = i;
				}
				break;
		}
	}
	*ppDib = dib;

	return TRUE;
}

BOOL CopyFromBitmap(FIBITMAP ** ppDib, HBITMAP hbmp) {
	if(hbmp) { 
		int Success;
        BITMAP bm;
		// Get informations about the bitmap
        GetObject(hbmp, sizeof(BITMAP), (LPSTR) &bm);
		// Create the image
        setSize(ppDib, FIT_BITMAP, (WORD)bm.bmWidth, (WORD)bm.bmHeight, (WORD)bm.bmBitsPixel);
		FIBITMAP * dib = *ppDib;

		// The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why) 
		// So we save these infos below. This is needed for palettized images only. 
		int nColors = FreeImage_GetColorsUsed(dib);

		// Create a device context for the bitmap
        HDC dc = GetDC(NULL);
		// Copy the pixels
		Success = GetDIBits(dc,								// handle to DC
								hbmp,						// handle to bitmap
								0,							// first scan line to set
								FreeImage_GetHeight(dib),	// number of scan lines to copy
								FreeImage_GetBits(dib),	// array for bitmap bits
								FreeImage_GetInfo(dib),	// bitmap data buffer
								DIB_RGB_COLORS				// RGB 
								);
		if(Success == 0) {
			FreeImage_OutputMessageProc(FIF_UNKNOWN, "Error : GetDIBits failed");
			ReleaseDC(NULL, dc);
			return FALSE;
        }
        ReleaseDC(NULL, dc);

		// restore BITMAPINFO members
		
		FreeImage_GetInfoHeader(dib)->biClrUsed = nColors;
		FreeImage_GetInfoHeader(dib)->biClrImportant = nColors;

		return TRUE;
    }

	return FALSE;
}
#endif

//#define USE_CREATE_DIB_SECTION
HRESULT FreeImage_LoadImageFromFile(HBITMAP * pHBitmap, LPCTSTR lpszPathName)
{
#ifndef FREEIMAGE_LIB
	return (E_NOTIMPL);
#else
	FIBITMAP * pFiBitmap;
	if ((pFiBitmap = Open_Image(lpszPathName)) != NULL)
	{
#ifdef USE_CREATE_DIB_SECTION
		HDC hDC = CreateCompatibleDC(NULL);
#else
		HDC hDC = ::GetDC(NULL);
#endif
		BYTE * Bits = FreeImage_GetBits(pFiBitmap);
		BITMAPINFOHEADER * bmiHeader = FreeImage_GetInfoHeader(pFiBitmap);
		BITMAPINFO * bitmapInfo = FreeImage_GetInfo(pFiBitmap);
#ifdef USE_CREATE_DIB_SECTION
		LPVOID pBit32;
		*pHBitmap = CreateDIBSection(
									hDC,
									bitmapInfo,
									DIB_RGB_COLORS, 
									&pBit32, 
									NULL, 
									0);
		if (pBit32) memcpy(pBit32, Bits, FreeImage_GetDIBSize(pFiBitmap));
#else
		*pHBitmap = CreateDIBitmap(
									hDC,			// handle to device context
									bmiHeader,		// pointer to BITMAPINFOHEADER
									CBM_INIT,		// initialization flag
									Bits,			// pointer to initialization data
									bitmapInfo,		// pointer to bitmap color-format data
									DIB_RGB_COLORS);// color-data usage
#endif

#ifdef USE_CREATE_DIB_SECTION
		::DeleteDC(hDC);
#else
		::ReleaseDC(NULL, hDC);
#endif
		FreeImage_Unload(pFiBitmap);
		return ( NOERROR );
	}
	
	return (E_FAIL);
#endif
}

HRESULT FreeImage_SaveImageToFile(HBITMAP hBitmap, LPCTSTR lpszPathName)
{
#ifndef FREEIMAGE_LIB
	return (E_NOTIMPL);
#else
	FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(lpszPathName);

	FIBITMAP * pFiBitmap = NULL;
	if (CopyFromBitmap(&pFiBitmap, hBitmap))
	{
		if (FreeImage_Save(fif, pFiBitmap, lpszPathName))
		{
			FreeImage_Unload(pFiBitmap);
			return ( NOERROR );
		}
		FreeImage_Unload(pFiBitmap);
	}
	return (E_FAIL);
#endif
}

void FreeImage_Init()
{
#if defined(FREEIMAGE_LIB) || !defined(WIN32)
	FreeImage_Initialise();
#endif
}

void FreeImage_DeInit()
{
#if defined(FREEIMAGE_LIB) || !defined(WIN32)
	FreeImage_DeInitialise();
#endif
}

HBITMAP FreeImage_Exposure(HBITMAP hBitmap, FLOAT fGamma, LONG lBrightness, LONG lContrast)
{
#ifndef FREEIMAGE_LIB
	return (NULL);
#else
	return (NULL);
#endif
}