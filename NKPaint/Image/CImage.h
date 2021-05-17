// Image.h: interface for the CImage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __IMAGE_H_INCLUDED__
#define __IMAGE_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif

const int CIMAGE_DC_CACHE_SIZE = 4;

class AFX_EXT_CLASS CImage
{
public:
	class CDCCache
	{
	public:
		CDCCache() throw();
		~CDCCache() throw();

		HDC GetDC() throw();
		void ReleaseDC( HDC ) throw();

	private:
		HDC m_ahDCs[CIMAGE_DC_CACHE_SIZE];
	};
public:
	enum DIBOrientation
	{
		DIBOR_DEFAULT,
		DIBOR_TOPDOWN,
		DIBOR_BOTTOMUP
	};
public:
	CImage()  throw();
	~CImage()  throw();
	HDC GetDC() const  throw();
	void ReleaseDC() const throw();
	int GetWidth() const;
	int GetHeight() const;
	CSize GetSize() const;
	void Attach( HBITMAP hBitmap, DIBOrientation eOrientation = DIBOR_DEFAULT ) throw();
	HBITMAP Detach() throw();
	BOOL StretchBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, 
		int nDestHeight, DWORD dwROP = SRCCOPY ) const;
	BOOL StretchBlt( HDC hDestDC, const RECT& rectDest, DWORD dwROP = SRCCOPY ) const;
	BOOL StretchBlt( HDC hDestDC, int xDest, int yDest, int nDestWidth, 
		int nDestHeight, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
		DWORD dwROP = SRCCOPY ) const;
	BOOL StretchBlt( HDC hDestDC, const RECT& rectDest, const RECT& rectSrc,
		DWORD dwROP = SRCCOPY ) const;
	void UpdateBitmapInfo( DIBOrientation eOrientation );
	bool IsNull() const throw();
	void Destroy() throw();
	HRESULT Load( LPCTSTR pszFileName ) throw();
	HRESULT Save( LPCTSTR pszFileName) const throw();
	HRESULT CreateFromHBITMAP(HBITMAP hBitmap);
	BOOL Exposure(FLOAT fGamma = 1.0f, LONG lBrightness = 0, LONG lContrast = 0);

private:
	HBITMAP m_hBitmap;
	void* m_pBits;
	int m_nWidth;
	int m_nHeight;
	int m_nPitch;
	int m_nBPP;
	bool m_bIsDIBSection;
	bool m_bHasAlphaChannel;
	LONG m_iTransparentColor;
private:
	mutable HDC m_hDC;
	mutable int m_nDCRefCount;
	mutable HBITMAP m_hOldBitmap;
};

int	 AFX_EXT_API GetSupportedFileExt(LPTSTR lpszStringBuf, int nMaxCount);
BOOL AFX_EXT_API IsFileSupported(LPCTSTR lpszFileName, LPCTSTR lpszExtFilter);

#endif // !defined(__IMAGE_H_INCLUDED__)
