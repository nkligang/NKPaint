#ifndef __HISTORY_STACK_INCLUDED__
#define __HISTORY_STACK_INCLUDED__

#define  MAXLEN  10

class CHistoryStack :public CObject
{
public:
	CHistoryStack(int TotalLen);
	~CHistoryStack();
private:
	int  m_iMaxLen;              // Max length of stack
	int  m_iTotalLen;            // Total length of stack
	int  m_iCurPos;              // Current position 
	HBITMAP  Stock[MAXLEN];      // Array of store bitmaps
public:
	int GetLen();
	int GetPos();
	HBITMAP  Undo();
	HBITMAP  Redo();
	HBITMAP  GetCurrent();
	void  Add(HBITMAP i);
	bool  IsEmpty();
	bool  CanUndo();
	bool  CanRedo();
	void  Clear();
};

#endif
