// ATTR.cpp : implementation file
//

#include "stdafx.h"
#include "NKPaint.h"
#include "AttributeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAttributeDlg dialog


CDlgAttribute::CDlgAttribute(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAttribute::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAttributeDlg)
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	//}}AFX_DATA_INIT
	m_eCurUnitType = UT_PIXEL;
	m_uBitCount = ILC_COLOR24;
}


void CDlgAttribute::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAttributeDlg)
	DDX_Text(pDX, IDC_DLG_ATTRIBUTE_WEITH, m_fWidth);
	DDX_Text(pDX, IDC_DLG_ATTRIBUTE_HEIGHT, m_fHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAttribute, CDialog)
	//{{AFX_MSG_MAP(CAttributeDlg)
	ON_BN_CLICKED(IDC_DLG_ATTRIBUTE_RADIO_CM, OnRadioCm)
	ON_BN_CLICKED(IDC_DLG_ATTRIBUTE_RADIO_INCH, OnRadioInch)
	ON_BN_CLICKED(IDC_DLG_ATTRIBUTE_RADIO_PIXEL, OnRadioPixel)
	ON_BN_CLICKED(IDC_DLG_ATTRIBUTE_BUTTON_DEFAULT, OnButtonDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAttributeDlg message handlers
BOOL CDlgAttribute::GetLastSaveFileInfo(LPTSTR lpStrFileName, LONG & dwDiskSpace)
{
  CString strTemp;
  strTemp.LoadString(IDS_ATTRIBUTE_DLG_FILE_LAST_NOT_SAVE);
	strcpy(lpStrFileName, strTemp);
	dwDiskSpace = -1;
	return TRUE;
}

CSize CDlgAttribute::GetSize()
{
	CSize size;
	size.cx = (long)ConventUnit(m_eCurUnitType, UT_PIXEL, m_fWidth);
	size.cy = (long)ConventUnit(m_eCurUnitType, UT_PIXEL, m_fHeight);
	return size;
}

void CDlgAttribute::SetSize(CSize size)
{
	m_fWidth = ConventUnit(UT_PIXEL, m_eCurUnitType, (float)size.cx);
	m_fHeight = ConventUnit(UT_PIXEL, m_eCurUnitType, (float)size.cy);
}

void CDlgAttribute::SetSize(int nWidth, int nHeight)
{
	SetSize(CSize(nWidth, nHeight));
}

BOOL CDlgAttribute::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strLastSavedFile;
	LONG dwDiskSpace;
	GetLastSaveFileInfo(strLastSavedFile.GetBuffer(0),dwDiskSpace);
	CString strTemp1,strTemp2;
	strTemp1.Format(IDS_ATTRIBUTE_DLG_FILE_LAST_SAVED,strLastSavedFile);
	GetDlgItem(IDC_DLG_ATTRIBUTE_STATIC_LAST_SAVED)->SetWindowText(strTemp1);
	if (dwDiskSpace == -1)
		strTemp2.Format(IDS_ATTRIBUTE_DLG_FILE_SIZE_INVALID);
	else
		strTemp2.Format("%d",dwDiskSpace);
	strTemp1.Format(IDS_ATTRIBUTE_DLG_FILE_SIZE,strTemp2);
	GetDlgItem(IDC_DLG_ATTRIBUTE_STATIC_DISK_SPACE)->SetWindowText(strTemp1);
	strTemp2.Format(IDS_ATTRIBUTE_DLG_RESOLUTION_FORMAT,97,96);
	strTemp1.Format(IDS_ATTRIBUTE_DLG_RESOLUTION,strTemp2);
	GetDlgItem(IDC_DLG_ATTRIBUTE_STATIC_DPI)->SetWindowText(strTemp1);
	
	((CButton *)GetDlgItem(IDC_DLG_ATTRIBUTE_RADIO_PIXEL))->SetCheck(TRUE);

	switch(m_uBitCount)
	{
	case 1:
		((CButton *)GetDlgItem(IDC_DLG_ATTRIBUTE_RADIO_GRAY))->SetCheck(TRUE);
		break;
	case 4:
	case 8:
	case 24:
	case 32:
		((CButton *)GetDlgItem(IDC_DLG_ATTRIBUTE_RADIO_COLOR))->SetCheck(TRUE);
		break;
	}
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAttribute::OnRadioCm()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_fWidth  = ConventUnit(m_eCurUnitType, UT_CM, m_fWidth);
	m_fHeight = ConventUnit(m_eCurUnitType, UT_CM, m_fHeight);
	m_eCurUnitType = UT_CM;
	UpdateData(FALSE);
}

void CDlgAttribute::OnRadioInch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_fWidth  = ConventUnit(m_eCurUnitType, UT_INCH, m_fWidth);
	m_fHeight = ConventUnit(m_eCurUnitType, UT_INCH, m_fHeight);
	m_eCurUnitType = UT_INCH;
	UpdateData(FALSE);
}

void CDlgAttribute::OnRadioPixel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_fWidth  = (float)((int)ConventUnit(m_eCurUnitType, UT_PIXEL, m_fWidth));
	m_fHeight = (float)((int)ConventUnit(m_eCurUnitType, UT_PIXEL, m_fHeight));

	m_eCurUnitType = UT_PIXEL;
	UpdateData(FALSE);
}

void CDlgAttribute::OnButtonDefault() 
{
	// TODO: Add your control notification handler code here
	m_fWidth  = ConventUnit(UT_PIXEL, m_eCurUnitType, SYSTEM_DEFAULT_WIDTH);
	m_fHeight = ConventUnit(UT_PIXEL, m_eCurUnitType, SYSTEM_DEFAULT_HEIGHT);
	UpdateData(FALSE);
}

void CDlgAttribute::OnOK() 
{
	// TODO: Add extra validation here
	static UINT eLastBitCountType = m_uBitCount != 1 ? m_uBitCount : 24;
	if (((CButton *)GetDlgItem(IDC_DLG_ATTRIBUTE_RADIO_GRAY))->GetCheck() == 1)
	{
		if (m_uBitCount != 1)
			eLastBitCountType = m_uBitCount;
		m_uBitCount = 1;
	}
	if (((CButton *)GetDlgItem(IDC_DLG_ATTRIBUTE_RADIO_COLOR))->GetCheck() == 1)
	{
		m_uBitCount = eLastBitCountType;
	}
	CDialog::OnOK();
}

void CDlgAttribute::SetBitCount(UINT uBitCount)
{
	m_uBitCount = uBitCount;
}

UINT CDlgAttribute::GetBitCount()
{
	return m_uBitCount;
}