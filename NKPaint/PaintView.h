#ifndef __PAINTVIEW_H_INCLUDED__
#define __PAINTVIEW_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PaintDoc.h"
#include "Utils.h"

#include "ADMMScrollview.h"
#include "CImage.h"
#include "DrawObject.h"

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - 
	//
	// Description:
	//    - 
	//
	// Inputs:
	//    - 
	//
	// Returns:
	//    - 
	//
	// Notes:
	//    - 
	//
	// See also:
	//    - 
	//


class CHistoryStack;
class CDrawObject;
class CMainFrame;

enum SelectMode
{
	SM_NONE,
	SM_SELECT,
	SM_MOVE,
	SM_SIZE,
	SM_SIZE_CANVAS,
	SM_DRAW_PEN,
	SM_DRAW_SHAPE,
	SM_DRAW_REGION,
	SM_DRAW_TEXT,
	SM_EDIT,
};

enum
{
	WM_USER_DRAW_DONE = WM_USER + 400,
};

#define INFINITESIMAL			0.000001		// Infinitesimal

#define PAINT_MAX_ZOOM_SCALE	8.0f			// the max scaling

class AFX_EXT_CLASS CPaintView : public CADMMScrollView, public CDrawHandle
{
public: 

	typedef enum 
	{
		BCT_UPDATE = 1,	// bitmap update
		BCT_INIT,		// bitmap initialize
	} BitmapChangedType;

	CPaintView();
	virtual ~CPaintView();
	DECLARE_DYNCREATE(CPaintView)
	
// Attributes
public:
	CPaintDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaintView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CPaintView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg LRESULT OnDrawDone(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CPoint		m_ptMove;				// coordinate when mouse is moving
	CPoint		m_ptStart;				// coordinate when mouse is left-button down

	FLOAT		m_fZoomScale;			// scaling
	CSize		m_sizeCanvas;			// the size of the canvas-real painting area(bimap size * scaling + hot points)

	CDrawObject*m_pCurObj;				// the current active object
	SelectMode	m_curMode;				// the painting mode of the current object
	UINT		m_emBrushStyle;			// the type of brush
	UINT		m_uLineWidth;			// the width of pen
	UINT		m_emShapeStyle;			// the type of shape
	BOOL		m_bCurObjectIsActive;	// the current object is active?[show tracker or not?]

	EdgeHandle	m_eCanvasHandle;		// handle of moving mouse on canvas edge
	EdgeHandle	m_nDragHandle;			// handle of draging mouse on canvas edge

	BOOL		  m_bRecord;				// draw the current object to background when the object is finished editing?

	CEdit*		m_pEditBox;				// text edit box
	CFont		  m_fontEditBox;			// font of text

	CBrush *	   m_pClientBkBrush;		// the client background brush
	CHistoryStack*	m_pHistoryStack;	// history bitmap stack

	CString		m_strCurFile;
public:

	inline void GetCoordinate(CPoint & ptLeftDown, CPoint & ptMove) 
	{ 
		ptLeftDown = m_ptStart;
		ptMove = m_ptMove;
	}

	inline BOOL AutoSave() { return SaveImage(m_strCurFile);}
	void AutoZoom();

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Set the current client background's brush
	//
	// Description:
	//    - Set the current client background's brush
	//
	// Inputs:
	//    - CBrush * pBrush : new brush
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - this view will use this brush to fill client background
	//
	inline void SetClientBkBrush(CBrush * pBrush) { ASSERT(pBrush != NULL); m_pClientBkBrush = pBrush;}

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Update this view when the current tool is changed
	//
	// Description:
	//    - Update this view when the current tool is changed
	//
	// Inputs:
	//    - UINT uCurSelTool : the current selected tool
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - this function is only called by CBaseFrame::SetCurSelTool
	//
	// See also:
	//    - HidenEditBox
	//	  - InvalidateObject
	//
	void OnToolChanged(UINT uCurSelTool);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Change the current object's pen and brush when the tool's style is changed
	//
	// Description:
	//    - Change the current object's pen and brush when the tool's style is changed
	//
	// Inputs:
	//    - UINT uCurSelTool : the current selected tool
	//	  -	UINT uCurSelToolStyle : the style of the current selected tool
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - This function is only called by CStyleListCtrl::SetCurSelToolStyle
	//
	// See also:
	//    - CBaseFrame::GetCurSelTool
	//
	void OnToolStyleChanged(UINT uCurSelTool, UINT uCurSelToolStyle);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Open a image by its absolute path
	//
	// Description:
	//    - Open a image by its absolute path
	//
	// Inputs:
	//    - LPCSTR lpFileName : the full path of image
	//
	// Returns:
	//    - BOOL : TRUE - image is opened successfully.
	//			   FALSE - some thing wrong happend
	//
	// See also:
	//    - CImage::Load
	//
	BOOL OpenImage(LPCTSTR lpFileName);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Save the current bitmap to one image
	//
	// Description:
	//    - Save the current bitmap to one image
	//
	// Inputs:
	//    - LPCSTR lpFileName : the file name to be saved
	//
	// Returns:
	//    - BOOL : TRUE - image is saved successfully.
	//			   FALSE - some thing wrong happend
	//
	// See also:
	//    - CImage::Save
	//
	BOOL SaveImage(LPCTSTR lpFileName);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Get the current bitmap
	//
	// Description:
	//    - Get the handle of the current bitmap on the canvas. we needn't to
	//		release the bitmap after using and we shouldn't release it, because 
	//		there is a mechanism to manage the create/delete/update operation
	//
	// Inputs:
	//    - Nothing
	//
	// Returns:
	//    - The handle of the current bitmap
	//
	// Notes:
	//    - This function is frequently used by many other functions. the handle
	//		we got is the key to access the canvas. this function may be designed
	//		as a interface for calling comes from external modules
	//
	// See also:
	//    - CHistoryStack
	//
	HBITMAP GetBitmap();

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Set the current bitmap
	//
	// Description:
	//    - Set one NEW bitmap to the current canvas.
	//
	// Inputs:
	//    - The handle of one bitmap
	//
	// Returns:
	//    - BOOL : set current bitmap successfully?
	//
	// Notes:
	//    - The handle we set to our current bitmap is create by caller and
	//		managed by the hostory stack.
	//			HBITMAP hBitmap = create a bitmap;
	//			SetBitmap(hBitmap);
	//			DeleteObject(hBitmap) is forbid
	//		So, it's wrong to use as follows:
	//			SetBitmap(GetBitmap());
	//
	// See also:
	//    - RecordHistory
	//
	BOOL SetBitmap(HBITMAP hBitmap);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Get the size of the current bitmap
	//
	// Description:
	//    - Get the size of the current bitmap
	//
	// Returns:
	//    - SIZE : the size of the current bitmap
	//
	// See also:
	//    - GetBitmap
	//
	SIZE GetBitmapSize();

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Get the current scaling
	//
	// Description:
	//    - Get the scaling before setting scaling is often used for persistent zooming.
	//
	// Inputs:
	//    - Nothing
	//
	// Returns:
	//    - FLOAT : the value of scaling
	//
	inline FLOAT GetZoomScale() { return m_fZoomScale; }

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Set the current scaling
	//
	// Description:
	//    - There are many problems that relate to scaling
	//		1. [mapping mode]which mode that graphic device use to draw
	//		2. the size of the current bitmap is not changing when zooming, 
	//		   but the size of the canvas is changing and which need to be update
	//		3. the area need to be update is related to scaling when drawing object
	//
	// Inputs:
	//    - FLOAT fValue : scaling to be set. fValue belongs to range 
	//					   [1/PAINT_MAX_ZOOM_SCALE, PAINT_MAX_ZOOM_SCALE]
	//
	// Returns:
	//    - Whether setting scaling is successful?
	//
	// Notes:
	//    - This view will be update after setting scaling automatically.
	//
	// See also:
	//    - UpdateViewSize
	//
	BOOL SetZoomScale(FLOAT fValue = 1.0f);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Allow/prohibit drawing object into the background
	//
	// Description:
	//    - Some special situation may only need to draw the target object when editing, 
	//		but not add the object to the background after editing. Example, Draw a 
	//		straight line in the canvas , and the image is to rotate according to the 
	//		angle between the straight-line and the horizontal line. at this time, the 
	//		role of the straight line is to indicate their nature. After rotation, the 
	//		straight line should not be added to the image.
	//
	// Inputs:
	//    - BOOL bRecord : Decide whether or not to allow the drawing object into the background
	//
	inline void EnableRecord(BOOL bRecord = TRUE) { m_bRecord = bRecord;}

protected:
	
	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Draw the rubber line according to the logic coordinates
	//
	// Description:
	//    - Draw the rubber line according to the logic coordinates
	//
	// Inputs:
	//    - const CPoint& ptLeftTop : the point of left-top
	//	  - const CPoint& ptBottomRight : the point of bottom-right
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - TODO: the function may have problems
	//
	// See also:
	//    - ::DrawFocusRect
	//
	void DrawFocusRect(const CPoint& ptLeftTop, const CPoint& ptBottomRight);
	
	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Hit test for canvas
	//
	// Description:
	//    - To determine whether a point on the canvas edge[hot points]
	//
	// Inputs:
	//    - CPoint point : the point to be determined
	//
	// Returns:
	//    - EdgeHandle : which hot point would be
	//
	// Notes:
	//    - the runtime rectangle of canvas is not changed with scaling
	//
	// See also:
	//    - HitTest
	//
	EdgeHandle HitCanvasTest(CPoint point);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Invalidate Object
	//
	// Description:
	//    - This function reflects the core idea of partial refresh technology
	//		we just need to update the bounding box of this object and we needn't
	//		to update the whole view.
	//		the intersection of this object's bounding box and the canvas's bounding box
	//		is the final region to be updated.
	//
	// Inputs:
	//    - CDrawObject * pObj : the object to be update
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - The difficulty is to calculate the real area to be updated
	//
	// See also:
	//    - GetBoundRect
	//	  - GetScrollPosition
	//	  - InvalidateRect
	//
	void InvalidateObject(CDrawObject * pObj);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Set the current object to a new one
	//
	// Description:
	//    - Set the current object to a new one
	//
	// Inputs:
	//    - CDrawObject * pNewObj : the new object
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - This function just set the current object's handle, but not 
	//		create graphic elements
	//
	void SetCurrentObject(CDrawObject * pNewObj);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Whether one coordinate is in canvas
	//
	// Description:
	//    - Whether one coordinate is in canvas. the coordinate 
	//		is not decrescent when canvas is zoom out because of
	//		map mode is set.
	//
	// Inputs:
	//    - const CPoint & point : the coordinate to be judged
	//
	// Returns:
	//    - BOOL : TRUE - the coordinate does in the canvas
	//			   FLASE - not in
	//
	// Notes:
	//    - The function is relative to the map mode
	//
	// See also:
	//    - OnLButtonDown
	//	  - OnLButtonUp
	//	  - OnMouseMove
	//
	BOOL IsPointInCanvas(const CPoint & point);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Fill devie with background color
	//
	// Description:
	//    - Fill devie with background color
	//
	// Inputs:
	//    - CDrawDC * pDC : the device to be filled.
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - Do not call OnEraseBkgnd anymore
	//
	// See also:
	//    - GetClientRect
	//	  - FillRect
	//
	void DrawBackGround(CDrawDC * pDC);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Convert the coordinate from the client region to the current bitmap
	//
	// Description:
	//    - Convert the coordinate from the client region to the canvas firstly, and then,
	//		convert the coordinate from the canvas to the current bitmap.
	//
	// Inputs:
	//    - LPPOINT pt : the coordinate to be convert and also the result coordinate
	//					 to come out.
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - the real coordinate to paint is different from the coordinate of client region
	//
	// See also:
	//    - GetScrollPosition
	//
	void ClientToBitmap(LPPOINT pt);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Convert the coordinate from the current bitmap to the client region
	//
	// Description:
	//    - Convert the coordinate from the current bitmap to the canvas firstly, and then,
	//		convert the coordinate from the canvas to the client region.
	//
	// Inputs:
	//    - LPPOINT pt : the coordinate to be convert and also the result coordinate
	//					 to come out.
	//
	// Returns:
	//    - Nothing
	//
	// See also:
	//    - GetScrollPosition
	//
	void BitmapToClient(LPPOINT pt);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Draw the current bitmap and the active object to one device
	//
	// Description:
	//    - Draw real-time rendering objects to the screen device.
	//	  - The problems refered to the process of operating active objects:
	//		1. the width of pen is alterable
	//		2. the canvas can be zoomed in or zoomed out
	//		3. the scroll bar is supported(even than the position of view is changed)
	//		all those problems should be solved harmoniously
	//    - Some issue should pay attention
	//		1. wherever the position of this view is, the active object is drawn to 
	//		   the copy of the current bitmap. the reason of using the copy is the current
	//		   bitmap can not be changed before new bitmap came out.
	//		2. the start position is different between the canvas and the client region
	//
	// Inputs:
	//    - CDrawDC * pDC : the device to be drawn
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - This function is only called by OnDraw or OnPaint
	//		This function is one of the core functions.
	//
	// See also:
	//    - 
	//
	void DoDrawToScreen(CDrawDC * pDC);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Draw the current bitmap and the active object to one device
	//
	// Description:
	//    - Draw to one device which contains the former bitmap and 
	//		the current object(finished edit)
	//
	// Inputs:
	//    - CDrawDC * pDC : the device to be drawn
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - this function is only used for CanvasToBitmap
	//
	// See also:
	//    - CanvasToBitmap
	//	  -	GetBitmap
	//
	void DoDrawToBitmap(CDrawDC * pDC);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Update the size of the current view
	//
	// Description:
	//    - Get the size of the current bitmap and calculate the size of canvas
	//		and then update the scroll bar
	//
	// Inputs:
	//    - Nothing
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - This function is used when the current bitmap is changed.
	//
	// See also:
	//    - GetBitmapSize
	//	  - SetScrollSizes
	//	  - Invalidate
	//
	void UpdateViewSize();

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Current bitmap changed and update
	//
	// Description:
	//    - When the current bitmap is changed, this view and other window need
	//		to be update(Thumbnails)
	//
	// Inputs:
	//    - BitmapChangedType eType : the type of action
	//		- BCT_UPDATE : just update current canvas
	//		- BCT_INIT : update current canvas and initialize edit mode
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - the pattern of updating may be re-designed.
	//		callback function is may be the better way.
	//
	// See also:
	//    - UpdateViewSize
	//
	void BitmapChanged(BitmapChangedType eType);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Initialize the bitmap history stack
	//
	// Description:
	//    - Because of getting the current bitmap in stack [GetCurrent], the stack
	//		can not be empty.
	//
	// Inputs:
	//    - HBITMAP hBitmap : the basic bitmap in the stack
	//
	// Returns:
	//    - Nothing
	//
	void InitHistory(HBITMAP hBitmap);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Add the handle of a bitmap to bitmap history stack
	//
	// Description:
	//    - Add the handle of a bitmap to bitmap history stack and update this view and other wnds
	//
	// Inputs:
	//    - HBITMAP hBitmap : the handle of a bitmap to be added
	//
	// Returns:
	//    - Nothing
	//
	// Notes:
	//    - The handle is detached from any device and is a new one, we cann't add one handle which
	//		which had already been added to the history stack
	//
	// See also:
	//    - BitmapChanged
	//
	void RecordHistory(HBITMAP hBitmap = NULL);

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Create a bitmap to which canvas draws 
	//
	// Description:
	//	  - How to create the bitmap?
	//    - 1. create a blank bitmap
	//	  -	2. get the current bitmap and copy it to the blank bitmap
	//	  - 3. draw the current active object[pen,shape,region.etc] to the bitmap
	//
	// Inputs:
	//    - Nothing
	//
	// Returns:
	//    - The handle of bitmap
	//
	// Notes:
	//    - Don't forget to release the space of bitmap after using it
	//		because the returned handle is just be created without being released
	//		and it will not be released automatically
	//
	// See also:
	//    - DoDrawToBitmap
	//
	HBITMAP CanvasToBitmap();

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Get the background color
	//
	// Description:
	//    - Get the background color
	//
	// Inputs:
	//    - Nothing
	//
	// Returns:
	//    - COLORREF : RGB value
	//
	// See also:
	//    - CBaseFrame::GetBkColor
	//
	inline COLORREF GetBkColor() { return ((CBaseFrame*)AfxGetMainWnd())->GetBkColor(); }

	//
	// *** Paint ***
	//
	// Synopsis:
	//    - Get the foreground color
	//
	// Description:
	//    - Get the foreground color
	//
	// Inputs:
	//    - Nothing
	//
	// Returns:
	//    - COLORREF : RGB value
	//
	// See also:
	//    - CBaseFrame::GetFgColor
	//
	COLORREF GetFgColor() { return ((CBaseFrame*)AfxGetMainWnd())->GetFgColor(); }

	
	//
	// *** Paint ***
	//
	// Synopsis:
	//    - 
	//
	// Description:
	//    - 
	//
	// Inputs:
	//    - 
	//
	// Returns:
	//    - 
	//
	// Notes:
	//    - 
	//
	// See also:
	//    - 
	//
	void ShowEditBox(const CRect& rect, const CString& strText=_T(""));

	void HidenEditBox(void);
	void OnDelete();

};

#ifndef _DEBUG  // debug version in ProjectFrameworkView.cpp
inline CPaintDoc* CPaintView::GetDocument()
   { return (CPaintDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DRAWVIEW_H_INCLUDED__)
