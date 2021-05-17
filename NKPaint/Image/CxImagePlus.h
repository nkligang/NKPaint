#ifndef __CXIMAGE_PIUS_H_INCLUDED__
#define __CXIMAGE_PIUS_H_INCLUDED__


void CxImage_Init();

HRESULT CxImage_LoadImageFromFile(HBITMAP * pHBitmap, LPCTSTR pszFileName);

HRESULT CxImage_SaveImageToFile(HBITMAP hBitmap, LPCTSTR pszFileName);

void CxImage_DeInit();

HBITMAP CxImage_Exposure(HBITMAP hBitmap, FLOAT fGamma, LONG lBrightness, LONG lContrast);

#endif