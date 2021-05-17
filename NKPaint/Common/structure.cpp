#include "stdafx.h"
#include "structure.h"
#include "AdvanceDraw.h"

CHistoryStack::CHistoryStack(int len)
{
	if( len > MAXLEN )
		len = MAXLEN;
	m_iMaxLen = len;
	m_iTotalLen = 0;
	m_iCurPos = -1;
	memset(Stock, 0, sizeof(Stock));
}

CHistoryStack::~CHistoryStack()
{
}

void CHistoryStack::Add(HBITMAP element)
{
	m_iTotalLen = m_iCurPos+1;
	if( m_iTotalLen < m_iMaxLen)
	{
		++m_iCurPos;
		if (Stock[m_iCurPos] != NULL)
			::DeleteObject(Stock[m_iCurPos]);
		Stock[m_iCurPos] = element;
		m_iTotalLen++;
	}
	else
	{
		::DeleteObject(Stock[0]);
		for( int i = 0; i < m_iMaxLen - 1; i++ )
			Stock[i] = Stock[i+1];
		Stock[m_iMaxLen - 1] = element;
	}
}

bool CHistoryStack::IsEmpty()
{
	return(m_iTotalLen == 0);
}

HBITMAP  CHistoryStack::Undo()
{
	ASSERT(m_iCurPos > 0);
	return  Stock[--m_iCurPos];
}

HBITMAP  CHistoryStack::Redo()
{
	ASSERT(m_iCurPos+1 < m_iTotalLen);
	return  Stock[++m_iCurPos];
}

bool CHistoryStack::CanUndo()
{
	return( m_iCurPos > 0 );
}

bool CHistoryStack::CanRedo()
{
	return( m_iCurPos+1 < m_iTotalLen );
}

HBITMAP  CHistoryStack::GetCurrent()
{
	if (m_iCurPos>=0)
		return  Stock[m_iCurPos];
	else
		return NULL;
}

int CHistoryStack::GetLen()
{
	return m_iTotalLen;
}

int CHistoryStack::GetPos()
{
	return m_iCurPos;
}

void CHistoryStack::Clear()
{
	while (m_iTotalLen-- > 0)
	{
		::DeleteObject(Stock[m_iTotalLen]);
	}
	
	m_iTotalLen = 0;
	m_iCurPos = -1;
}