#if !defined(AFX_STYLELISTCTRL_H__CC2BD346_33D2_4F19_8BC6_EBA80E3CEBCC__INCLUDED_)
#define AFX_STYLELISTCTRL_H__CC2BD346_33D2_4F19_8BC6_EBA80E3CEBCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStyleListCtrl window
#define INVALID_STYLE	MAXUINT_PTR

class CStyleListCtrl : public CListCtrl
{
// Construction
public:
	CStyleListCtrl();

// Attributes
public:
	UINT m_nItemHeight;
	UINT m_nCurSelToolID;
// Operations
public:
	void SetItemHeight(UINT nHeight);
	static CImageList * GetSpecImageList(int index);
	UINT GetCurSelToolStyle();
	void SetCurSelTool(UINT nToolID, UINT uStyle = INVALID_STYLE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleListCtrl)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStyleListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStyleListCtrl)
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
protected:
	void SetCurSelToolStyle(UINT index);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLELISTCTRL_H__CC2BD346_33D2_4F19_8BC6_EBA80E3CEBCC__INCLUDED_)
