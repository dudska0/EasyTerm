#include "pch.h"
#include "CResponseItem.h"

CResponseItem::CResponseItem()
{
}

CResponseItem::~CResponseItem()
{
}

char* CResponseItem::Get_Key_Value()
{
	return m_szKeyVal;
}

void CResponseItem::Set_Key_Value(char* pszValue)
{
	memset(m_szKeyVal, 0, sizeof(char) * 50);
	memcpy(m_szKeyVal, pszValue, sizeof(char) * 49);
}

bool CResponseItem::Get_Use()
{
	return m_bUse;
}

void CResponseItem::Set_Use(bool bUse)
{
	m_bUse = bUse;
}

int CResponseItem::Get_Resp_Mode()
{
	return m_iRespMode;
}

void CResponseItem::Set_Resp_Mode(int iVal)
{
	m_iRespMode = iVal;
}

char* CResponseItem::Get_Resp_Data(void *pParam)
{
	return NULL;
}