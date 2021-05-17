// StretchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NKPaint.h"
#include "StretchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStretchDlg dialog


CDlgStretch::CDlgStretch(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStretch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStretchDlg)
	m_uStretchHorizontal = 100;
	m_uStretchVertical   = 100;
	m_iContortHorizontal = 0;
	m_iContortVertical	 = 0;
	//}}AFX_DATA_INIT
}


void CDlgStretch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStretchDlg)
	DDX_Text(pDX, ID_DLG_STRETCH_STRETCH_HORIZONTAL, m_uStretchHorizontal);
	DDV_MinMaxUInt(pDX, m_uStretchHorizontal, 1, 500);
	DDX_Text(pDX, ID_DLG_STRETCH_STRETCH_VERTICAL, m_uStretchVertical);
	DDV_MinMaxUInt(pDX, m_uStretchVertical, 1, 500);
	DDX_Text(pDX, ID_DLG_STRETCH_CONTORT_HORIZONTAL, m_iContortHorizontal);
	DDV_MinMaxInt(pDX, m_iContortHorizontal, -89, 89);
	DDX_Text(pDX, ID_DLG_STRETCH_CONTORT_VERTICAL, m_iContortVertical);
	DDV_MinMaxInt(pDX, m_iContortVertical, -89, 89);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStretch, CDialog)
	//{{AFX_MSG_MAP(CStretchDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStretchDlg message handlers

BOOL CDlgStretch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
