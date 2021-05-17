#ifndef __UTILS_H_INCLUDED__
#define __UTILS_H_INCLUDED__

#define COLOR_SYSTEM_WHITE		0x00FFFFFF
#define COLOR_SYSTEM_BLACK		0x00000000
#define COLOR_SYSTEM_CURSOR		0x00008080
static CBrush g_cbSystemBackground(::GetSysColor(COLOR_APPWORKSPACE));
static CBrush g_cbSystemWhite(COLOR_SYSTEM_WHITE);
static CBrush g_cbSystemPlan(::GetSysColor(COLOR_BTNFACE));
static CBrush g_cbSystemHighLight(::GetSysColor(COLOR_HIGHLIGHT));
static CBrush g_cbSystemCursor(COLOR_SYSTEM_CURSOR);

// user defined message
enum{
	WM_SET_BACKGROUND = WM_USER + 100,
	WM_SET_FOREGROUND,
	WM_MODIFY_BITCOUNTTYPE,
	WM_TOOL_SELECT_CHANGED,
};

#define BACKSLASH	'\\'
#define SEMICOLON	';'

#endif