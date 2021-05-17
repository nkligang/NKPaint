// DrawDoc.h : interface of the CDrawDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDOC_H__D45879A1_ACE3_4B5E_B394_AC74D4CE590F__INCLUDED_)
#define AFX_DRAWDOC_H__D45879A1_ACE3_4B5E_B394_AC74D4CE590F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NKPaintPlugin.h"

class CDrawDoc : public CBaseDoc
{
protected: // create from serialization only
	CDrawDoc();
	DECLARE_DYNCREATE(CDrawDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWDOC_H__D45879A1_ACE3_4B5E_B394_AC74D4CE590F__INCLUDED_)
