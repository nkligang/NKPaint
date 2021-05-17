// DrawDoc.h : interface of the CDrawDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDOC_H__5FF6B29C_5187_4FB5_AFF4_C1B5C7FEE9F8__INCLUDED_)
#define AFX_DRAWDOC_H__5FF6B29C_5187_4FB5_AFF4_C1B5C7FEE9F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "structure.h"
#include "BaseFrame.h"

class AFX_EXT_CLASS CPaintDoc : public CDocument
{
protected: // create from serialization only
	CPaintDoc();
	DECLARE_DYNCREATE(CPaintDoc)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaintDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HBITMAP GetBitmap();
	void SetBitmap(HBITMAP hBitmap);
	inline COLORREF GetBkColor() {return ((CBaseFrame*)AfxGetMainWnd())->GetBkColor();}
	inline COLORREF GetFgColor() {return ((CBaseFrame*)AfxGetMainWnd())->GetFgColor();}

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawDoc)
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnImageAttribute();
	afx_msg void OnImageStretch();
	afx_msg void OnImageInvertColor();
	afx_msg void OnImageFlipRotate();
	afx_msg void OnColorEdit();
	afx_msg void OnViewZoomCustomEighth();
	afx_msg void OnViewZoomCustomQuarter();
	afx_msg void OnViewZoomCustomHalf();
	afx_msg void OnViewZoomCustomFix();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CDrawDoc)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWDOC_H__5FF6B29C_5187_4FB5_AFF4_C1B5C7FEE9F8__INCLUDED_)
