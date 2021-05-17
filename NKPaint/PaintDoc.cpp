// DrawDoc.cpp : implementation of the CDrawDoc class
//

#include "stdafx.h"
#include "NKPaint.h"

#include "PaintDoc.h"
#include "BaseFrame.h"
#include "PaintView.h"
#include "AdvanceDraw.h"
#include "StretchDlg.h"
#include "FlipRotateDlg.h"
#include "AttributeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc

IMPLEMENT_DYNCREATE(CPaintDoc, CDocument)

BEGIN_MESSAGE_MAP(CPaintDoc, CDocument)
	//{{AFX_MSG_MAP(CDrawDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_NP_IMAGE_ATTRIBUTE, OnImageAttribute)
	ON_COMMAND(ID_NP_IMAGE_STRETCH, OnImageStretch)
	ON_COMMAND(ID_NP_IMAGE_INVERT_COLOR, OnImageInvertColor)
	ON_COMMAND(ID_NP_IMAGE_FLIP_ROTATE, OnImageFlipRotate)
	ON_COMMAND(ID_NP_COLOR_EDIT, OnColorEdit)
	ON_COMMAND(ID_NP_VIEW_ZOOM_CUSTOM_EIGHTH, OnViewZoomCustomEighth)
	ON_COMMAND(ID_NP_VIEW_ZOOM_CUSTOM_QUARTER, OnViewZoomCustomQuarter)
	ON_COMMAND(ID_NP_VIEW_ZOOM_CUSTOM_HALF, OnViewZoomCustomHalf)
	ON_COMMAND(ID_NP_VIEW_ZOOM_CUSTOM_FIX, OnViewZoomCustomFix)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPaintDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CDrawDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

static const IID IID_IDraw =
{ 0x4b67dfe6, 0xc8c4, 0x4889, { 0x9d, 0xad, 0x78, 0x8f, 0x1, 0xa7, 0xae, 0x3c } };

BEGIN_INTERFACE_MAP(CPaintDoc, CDocument)
	INTERFACE_PART(CPaintDoc, IID_IDraw, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc construction/destruction
CPaintView * GetActiveView()
{
 	CBaseFrame * pBaseFrame = (CBaseFrame*)AfxGetMainWnd();
	ASSERT(pBaseFrame != NULL);
 	CPaintView * pDrawView = (CPaintView *)(pBaseFrame->GetActiveView());
	ASSERT(pDrawView != NULL);
	return pDrawView;
} 

CPaintDoc::CPaintDoc()
{
	EnableAutomation();

	AfxOleLockApp();


}

CPaintDoc::~CPaintDoc()
{
	AfxOleUnlockApp();

}

BOOL CPaintDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDrawDoc serialization

void CPaintDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc diagnostics

#ifdef _DEBUG
void CPaintDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPaintDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc commands
HBITMAP CPaintDoc::GetBitmap()
{
	return GetActiveView()->GetBitmap();
}

void CPaintDoc::SetBitmap(HBITMAP hBitmap)
{
	GetActiveView()->SetBitmap(hBitmap);
}

void CPaintDoc::OnImageStretch()
{
	CDlgStretch dlg;
	if (dlg.DoModal() == IDOK)
	{
		SetBitmap(StretchBitmap(GetBitmap(), dlg.m_uStretchHorizontal, dlg.m_uStretchVertical));
	}
}


void CPaintDoc::OnImageInvertColor()
{
/*	CBaseFrame * pBaseFrame = (CBaseFrame*)AfxGetMainWnd();
	CPaintView * pDrawView = (CPaintView *)(pBaseFrame->GetActiveView());
	if (pBaseFrame->IsToolEnabled(ID_NP_TOOL_ERASER))
	{
		pBaseFrame->EnableTool(ID_NP_TOOL_ERASER, FALSE);
		pDrawView->EnableRecord(FALSE);
	}
	else
	{
		pBaseFrame->EnableTool(ID_NP_TOOL_ERASER, TRUE);
		pDrawView->EnableRecord(TRUE);
	}
	pBaseFrame->SetCurSelTool(ID_NP_TOOL_LINE, 2);*/

	HBITMAP hBitmap = CopyBitmap(InvertBitmap(GetBitmap()));
	SetBitmap(hBitmap);
}


void CPaintDoc::OnImageFlipRotate()
{
	CDlgFlipRotate dlg;
	COLORREF bkColor = GetBkColor();
	if (dlg.DoModal() == IDOK)
	{
		switch(dlg.m_eFlipRotateType)
		{
		case FRT_FLIP_HORIZONTAL:
			SetBitmap(NormalRotateBitmap(GetBitmap(),NRT_HOR));
			break;
		case FRT_FLIP_VERTICAL:
			SetBitmap(NormalRotateBitmap(GetBitmap(),NRT_VER));
			break;
		case FRT_ROTATE_90:
			SetBitmap(NormalRotateBitmap(GetBitmap(),NRT_90));
			break;
		case FRT_ROTATE_180:
			SetBitmap(NormalRotateBitmap(GetBitmap(),NRT_180));
			break;
		case FRT_ROTATE_270:
			SetBitmap(NormalRotateBitmap(GetBitmap(),NRT_270));
			break;
		case FRT_ROTATE_ANY:
			SetBitmap(RotateBitmap(GetBitmap(), dlg.m_fRotateAngle * 3.1415926 / 180, CBrush(bkColor)));
			break;
		}
	}
}

void CPaintDoc::OnColorEdit() 
{
	CBaseFrame * pFrame = (CBaseFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	CPaintBoxDlgBar * pPaintBox = pFrame->GetPaintBox();
	ASSERT(pPaintBox);
	CColorDialog colorDialog(pPaintBox->GetForegoundColor());
	if (colorDialog.DoModal() == IDOK)
	{
		pPaintBox->SetForeBackColor(SET_FORE_GROUND_COLOR,colorDialog.GetColor(),0);
		pPaintBox->SetBoxColor(0,colorDialog.GetColor());
	}
}

void CPaintDoc::OnImageAttribute()
{
	CDlgAttribute dlg;
	HBITMAP hBitmap = GetBitmap();
	if (hBitmap == NULL) return;
	BITMAP bitmap;
	::GetObject(hBitmap,sizeof(bitmap),&bitmap);
	dlg.SetSize(bitmap.bmWidth, bitmap.bmHeight);
	dlg.SetBitCount(bitmap.bmBitsPixel);
	if (dlg.DoModal() == IDOK)			
	{
		CSize sizeImage = dlg.GetSize();
		CBaseFrame * pBaseFrame = (CBaseFrame*)AfxGetMainWnd();
		CPaintView * pDrawView;
		pDrawView = (CPaintView *)pBaseFrame->GetActiveView();
		SetBitmap(ChangeBitmap(hBitmap, sizeImage.cx, sizeImage.cy, pBaseFrame->GetBkColor()));
		CPaintBoxDlgBar * pPaintBox = pBaseFrame->GetPaintBox();
		ASSERT(pPaintBox);
		pPaintBox->SendMessage(WM_MODIFY_BITCOUNTTYPE,(WPARAM)dlg.GetBitCount());
	}
}

void CPaintDoc::OnViewZoomCustomEighth() 
{
	GetActiveView()->SetZoomScale(0.125f);
}

void CPaintDoc::OnViewZoomCustomQuarter() 
{
	GetActiveView()->SetZoomScale(0.25f);
}

void CPaintDoc::OnViewZoomCustomHalf() 
{
	GetActiveView()->SetZoomScale(0.5f);
}

void CPaintDoc::OnViewZoomCustomFix() 
{
	GetActiveView()->SetZoomScale(1.0f);
}

BOOL CPaintDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}

void CPaintDoc::OnFileOpen() 
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"All files(*.*)|*.*|Bitmap(*.bmp)|*.bmp|JPEG(*.jpg)|*.jpg||",
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		if (this->IsModified())
		{
			if (AfxMessageBox(IDS_COMMON_MODIFIED, MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				this->SetModifiedFlag(FALSE);
			}
			else
			{
				GetActiveView()->AutoSave();
			}
		}
		CString strFileName = dlg.GetFileName();
		if (GetActiveView()->OpenImage(strFileName) == FALSE)
		{
			CString fmt;
			fmt.LoadString(IDS_NP_INVALID_FILE);
			AfxMessageBox(fmt,NULL,MB_OK|MB_ICONWARNING);
		}
		else
		{
			CString strTitle, strAppName;
			strAppName.LoadString(AFX_IDS_APP_TITLE);
			strTitle.Format("%s - %s", strFileName, strAppName);
			AfxGetMainWnd()->SetWindowText(strTitle);
		}
	}
}

void CPaintDoc::OnFileSave() 
{
	CString strFilter = _T("JPEG(*.jpg)|*.jpg|Bitmap(*.bmp)|*.bmp||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if (dlg.DoModal()==IDOK)
	{
		CString strFileName;
		CString strExtension;
		strFileName = dlg.m_ofn.lpstrFile;

		// add the file extension if the user didn't supply one
		if (dlg.m_ofn.nFileExtension == 0) 
		{
			switch (dlg.m_ofn.nFilterIndex)
			{
			case 1:
				strExtension = "jpg";
				break;
			case 2:
				strExtension = "bmp";
				break;
			case 3:
				strExtension = "gif";
				break;
			case 4:
				strExtension = "png";
				break;
			default:
				break;
			}
			strFileName = strFileName + '.' + strExtension;
		}
		if (GetActiveView()->SaveImage(strFileName) == FALSE)
		{
			CString fmt;
			fmt.Format(_T("Save image failed:\n%s"), strFileName);
			AfxMessageBox(fmt);
		}

	}
	else
	{
		this->SetModifiedFlag(FALSE);
	}
}
