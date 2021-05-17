//
#ifndef __UTILITY_H_INCLUDED__
#define __UTILITY_H_INCLUDED__


class CUtility
{
public:
	//CUtility();

	static CString GenerateID();
	
	static void PrepareDC(HDC hdc, float fZoomScale=1);
};

#endif