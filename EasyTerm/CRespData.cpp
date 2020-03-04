#include "pch.h"
#include "CRespData.h"
#include "CSecsResponseData.h"

CResponse::CResponse()
{
	m_pData = NULL;
	Set_Message_Type(3);													// 여기서 Memory를 Allocate 한다.
	Set_Response_Mode(0);

	m_iMsgType = 3;

	// 임시로 여기서 Setting 한다.
	
}

CResponse::CResponse(int iMsgType, int iRespType)
{
	m_pData = NULL;
	Set_Message_Type(iMsgType);
	Set_Response_Mode(iRespType);
}


CResponse::~CResponse()
{
	if (m_pData != NULL) {
		delete[] m_pData;
		m_pData = NULL;
	}
}

void CResponse::Set_Message_Type(int itype)
{
	kaCSecsResponseData* pData = NULL;

	if (m_iMsgType == itype)		return;
	
	m_iMsgType = itype;
	if (m_pData == NULL) {
		pData = (kaCSecsResponseData *)  new kaCSecsResponseData[MAX_RESP_SIZE];			// 나중에 위치 바꿔야 함.. (ㅡ위험 Memory leak....)
	}
	else {
		delete m_pData;
		pData = (kaCSecsResponseData *)  new kaCSecsResponseData[MAX_RESP_SIZE];
	}

	m_pData = pData;
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
	
	kaCSecsResponseData* ptr = ((kaCSecsResponseData *) m_pData) + idx;

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

	kaCSecsResponseData* ptr = ((kaCSecsResponseData *) m_pData) + idx;

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
	
	kaCSecsResponseData* ptr = ((kaCSecsResponseData *) m_pData) + idx;

	memset(&m_pData[idx], 0, sizeof(kaCSecsResponseData));
}

void CResponse::Delete_Response_Data(int index)
{
	kaCSecsResponseData* ptr = ((kaCSecsResponseData *) m_pData) + index;

	ptr->Set_Use(false);
}

int CResponse::Get_Item_Response_Mode(int idx)
{
	kaCSecsResponseData* ptr = ((kaCSecsResponseData *)m_pData) + idx;

	if (m_pData == NULL) {
		return -1;
	}

	return (ptr + idx)->Get_Resp_Mode();
}

int CResponse::Find_IndexbyKey(char* pszKeyVal)
{
	int		i;
	kaCSecsResponseData* ptr = NULL;

	for (i = 0; i < MAX_RESP_SIZE; i++) {
		ptr = ((kaCSecsResponseData *)m_pData) + i;
	
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

int CResponse::Find_IndexbyKey(char* pszKeyVal, int iStartIdx)
{
	int		i;
	kaCSecsResponseData* ptr = NULL;

	for (i = iStartIdx; i < MAX_RESP_SIZE; i++) {
		ptr = ((kaCSecsResponseData *) m_pData) + i;

		if (ptr == NULL)	continue;
		if (ptr->Get_Use() == false) continue;
		if (Get_Response_Mode() == ptr->Get_Resp_Mode()) {
			if (strcmp(pszKeyVal, ptr->Get_Key_Value()) == 0) {
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
	
	kaCSecsResponseData* ptr = NULL;
	ptr = ((kaCSecsResponseData *)m_pData) + idx;
	if (ptr == NULL)	return false;

	return ptr->Get_Use();
}

void CResponse::Set_Use(int idx, bool bUse)
{
	if (m_pData == NULL)	return;
	if (idx < 0)	return ;
	if (idx >= MAX_RESP_SIZE) return ;

	kaCSecsResponseData* ptr = (kaCSecsResponseData *) m_pData;
	if (ptr == NULL)	return ;
	(ptr+idx)->Set_Use(bUse);
}

int CResponse::Find_Empty_Space()
{
	int		i;

	if (m_pData == NULL)	return -1;
	kaCSecsResponseData* ptr = (kaCSecsResponseData *)m_pData;

	for (i = 0; i < MAX_RESP_SIZE; i++) {
		if ((ptr + i)->Get_Use() == false)	{
			return i;
		}
	}

	return -1;
}

bool CResponse::Set_Response_Data(char* pszData)
{
	int		idx = -1;
	kaCSecsResponseData* ptr = (kaCSecsResponseData *) m_pData;

	if (m_pData == NULL) {
		return false;
	}

	idx = Find_Empty_Space();
	if (idx == -1)		return false;

	if (m_iMsgType == 3) {
		return (ptr + idx)->Parse(pszData);
	}

	return false;
}

char* CResponse::Get_Response_Message(int idx)
{
	char    szRet[1024] = { 0, };
	kaCSecsResponseData* ptr = (kaCSecsResponseData *) m_pData;

	return (ptr + idx)->Get_Resp_Data();
}

char* CResponse::Get_Key_Value(int idx)
{
	kaCSecsResponseData* ptr = (kaCSecsResponseData *)m_pData;
	return (ptr + idx)->Get_Key_Value();
}

bool CResponse::Get_Wait_Response(int idx)
{
	kaCSecsResponseData* ptr = (kaCSecsResponseData *)m_pData;
	return (ptr + idx)->Get_Wait_Response();
}

bool CResponse::Get_End_Flag(int idx)
{
	kaCSecsResponseData* ptr = (kaCSecsResponseData *)m_pData;
	return (ptr + idx)->Get_End_Flag();
}

bool CResponse::Get_To_Host(int idx) 
{
	kaCSecsResponseData* ptr = (kaCSecsResponseData *)m_pData;
	return (ptr + idx)->Get_To_Host();
}

kaCSecsResponseData* CResponse::Get_ResponseData(int idx)
{
	kaCSecsResponseData* ptr = (kaCSecsResponseData *) m_pData;
	return (ptr+idx);
}

bool CResponse::Check_Response_Exist(char* pszKeyVal, int iRespMode, int iStartIdx )		// iRespMode
{																		//	  -1 이면 Mode Matching을 하지 않는다. 
	kaCSecsResponseData* ptr = (kaCSecsResponseData *)m_pData;			//	  Not 1이면 User 지정 모드로 동작한다.
	int idx;

	if (iStartIdx >= MAX_RESP_SIZE) {
		return false;
	}

	idx = Find_IndexbyKey(pszKeyVal, iStartIdx);
	if (idx < 0) {
		return false;
	}

	if ((ptr + idx)->Get_Use() == false) {
		return Check_Response_Exist(pszKeyVal, iRespMode, idx+1);
	}

	if (iRespMode > 0) {
		if ((ptr + idx)->Get_Resp_Mode() != iRespMode) {
			return Check_Response_Exist(pszKeyVal, iRespMode, idx+1);
		}
		else {
			return true;
		}
	}
	else if ( iRespMode == -1 ) {
		return true;
	}
	else {
		return false;
	}

	return false;
}