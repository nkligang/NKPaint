#ifndef __NKPAINT_PLUGIN_INCLUDED__
#define __NKPAINT_PLUGIN_INCLUDED__

#ifdef NKPAINT
#	include "NPRes.h"
#	include "PaintView.h"
#	include "PaintDoc.h"
#	include "BaseFrame.h"
#	define CBaseView		CPaintView
#	define CBaseDoc			CPaintDoc
#	define CBaseMainFrame	CBaseFrame
#else
#	define CBaseView		CView
#	define CBaseDoc			CDocument
#	define CBaseMainFrame	CFrameWnd
#endif

#endif