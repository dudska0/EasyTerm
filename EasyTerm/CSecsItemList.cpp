#include "pch.h"
#include "CSecsItemList.h"

CSecsItemList::CSecsItemList()
{
	memset(m_szContents, 0, sizeof(MAX_STRING_SIZE));
}

CSecsItemList::~CSecsItemList() 
{
}

long CSecsItemList::Get_Size()
{
	return m_lSize;
}
void CSecsItemList::Set_Size(long lSize)
{
	m_lSize = lSize;
}

int CSecsItemList::Get_Type()
{
	return m_iType;
}

void CSecsItemList::Set_Type(int iType)
{
	m_iType = iType;
}

char* CSecsItemList::Get_Contents_Str()
{
	return m_szContents;
}

void CSecsItemList::Set_Contents_Str(char* pszStr)
{
	strncpy(m_szContents, pszStr, MAX_STRING_SIZE);
}

void CSecsItemList::Contents_Reset()
{
	m_iType = -1;
	this->m_lSize = 0;
	memset(m_szContents, 0, sizeof(MAX_STRING_SIZE));
}