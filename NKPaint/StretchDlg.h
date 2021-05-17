#if !defined(AFX_STRETCHDLG_H__44BEFEFB_0702_463A_ABC3_51E4D4630E3E__INCLUDED_)
#define AFX_STRETCHDLG_H__44BEFEFB_0702_463A_ABC3_51E4D4630E3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StretchDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStretchDlg dialog

class CDlgStretch : public CDialog
{
// Construction
public:
	CDlgStretch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStretchDlg)
	enum { IDD = IDD_DLG_STRETCH };
	UINT	m_uStretchHorizontal;
	UINT	m_uStretchVertical;
	int		m_iContortHorizontal;
	int		m_iContortVertical;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStretchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStretchDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRETCHDLG_H__44BEFEFB_0702_463A_ABC3_51E4D4630E3E__INCLUDED_)
