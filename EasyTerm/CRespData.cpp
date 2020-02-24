#include "pch.h"
#include "CRespData.h"
#include "CSecsResponseData.h"

CResponse::CResponse()
{
	Set_Message_Type(0);
	Set_Response_Mode(0);
	m_pData = NULL;
}

CResponse::CResponse(int iMsgType, int iRespType)
{
	Set_Message_Type(iMsgType);
	Set_Response_Mode(iRespType);
	m_pData = NULL;
}


CResponse::~CResponse()
{
	if (m_pData != NULL) {
		delete m_pData;
		m_pData = NULL;
	}
}

void CResponse::Set_Message_Type(int itype)
{
	if (m_iMsgType == itype)		return;
	
	m_iMsgType = itype;
	m_pData = (CResponseItem *)  new CSecsResponseData[MAX_RESP_SIZE];
}

int CResponse::Get_Message_Type()
{
	return m_iMsgType;
}

void CResponse::Set_Response_Mode(int itype)
{
	m_iRespType = itype;
}

int CResponse::Get_Response_Mode()
{
	return m_iRespType;
}

bool CResponse::Set_Response_Data(int idx, char* pszKeyVal, int iRespMode, char* pszSendMsg, short sdata)
{
	if (idx < 0 ) return false;
	if (idx >= MAX_RESP_SIZE) return false;
	if (m_pData == NULL)	return false;
	
	CSecsResponseData* ptr = (CSecsResponseData *) &m_pData[idx];

	ptr->Set_Aux_Data(sdata);
	ptr->Set_Use(true);
	ptr->Set_Resp_Mode(iRespMode);

	ptr->Set_Key_Value(pszKeyVal);
	ptr->Set_Resp_Data(pszSendMsg);

	return true;
}

bool CResponse::Append_Response_Data(char* pszKeyVal, int iRespMode, char* pszSendMsg, short sdata)
{
	int idx = -1;

	if (m_pData == NULL)	return false;
	idx = this->Find_Empty_Space();
	if (idx == -1) return false;

	CSecsResponseData* ptr = (CSecsResponseData *)&m_pData[idx];

	ptr->Set_Aux_Data(sdata);
	ptr->Set_Use(true);
	ptr->Set_Resp_Mode(iRespMode);

	ptr->Set_Key_Value(pszKeyVal);
	ptr->Set_Resp_Data(pszSendMsg);

	return true;
}

void CResponse::Delete_Response_Data(char* pszKeyVal)
{
	int idx = -1;

	if (m_pData == NULL)	return;

	idx = this->Find_IndexbyKey(pszKeyVal);
	if (idx == -1) return;
	
	CSecsResponseData* ptr = (CSecsResponseData *)&m_pData[idx];

	ptr->Set_Use(false);
}

void CResponse::Delete_Response_Data(int index)
{
	CSecsResponseData* ptr = (CSecsResponseData *)&m_pData[index];

	ptr->Set_Use(false);
}

int CResponse::Find_IndexbyKey(char* pszKeyVal)
{
	int		i;
	CSecsResponseData* ptr = NULL;
		

	for (i = 0; i < MAX_RESP_SIZE; i++) {
		ptr = (CSecsResponseData *)&m_pData[i];
	
		if (ptr == NULL)	continue;
		if ( ptr->Get_Use() == false) continue;
		if ( Get_Response_Mode() == ptr->Get_Resp_Mode()) {
			if (strcmp(pszKeyVal,ptr->Get_Key_Value()) == 0) {
				return i;
			}
		}       
	}

	return -1;
}

bool CResponse::Get_Use(int idx)
{
	if (m_pData == NULL)	return false;
	if (idx < 0)	return false;
	if (idx >= MAX_RESP_SIZE) return false;
	
	CSecsResponseData* ptr = NULL;
	ptr = (CSecsResponseData *)&m_pData[idx];

	if (ptr == NULL)	return false;

	return ptr->Get_Use();
}

void CResponse::Set_Use(int idx, bool bUse)
{

	if (m_pData == NULL)	return;
	if (idx < 0)	return ;
	if (idx >= MAX_RESP_SIZE) return ;

	CSecsResponseData* ptr = NULL;
	ptr = (CSecsResponseData *)&m_pData[idx];

	if (ptr == NULL)	return ;

	ptr->Set_Use(bUse);
	
}

int CResponse::Find_Empty_Space()
{
	int		i;

	if (m_pData == NULL)	return -1;

	for (i = 0; i < MAX_RESP_SIZE; i++) {
		if ( m_pData[i].Get_Use() == false)	{
			return i;
		}
	}

	return -1;
}
