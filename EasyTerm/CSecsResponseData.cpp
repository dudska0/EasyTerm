#include <windows.h>

#include "pch.h"

#include "CSecsResponseData.h"


#if _MSC_VER >= 1900
#pragma comment( lib, "legacy_stdio_definitions.lib")
#endif


kaCSecsResponseData::kaCSecsResponseData()
{
	m_pKStr = new CKStr();
	m_pMemStr = new kaMemStr();

	m_bWaitResponse = 0;
	m_bEndPacket = 0;
	m_bToHost = 0;
	m_iAuxData = 0;
	memset(m_szRespData, 0, sizeof(char) * 1024);
}

kaCSecsResponseData::~kaCSecsResponseData()
{
	if (m_pKStr != NULL) {
		delete m_pKStr;
		m_pKStr = NULL;
	}

	if (m_pMemStr != NULL) {
		delete m_pMemStr;
		m_pMemStr = NULL;
	}
}

char* kaCSecsResponseData::Get_Resp_Data()
{
	return this->m_szRespData;
}

void kaCSecsResponseData::Set_Resp_Data(char* szRespData)
{
	//memcpy(m_szRespData, szRespData, sizeof(char) * 1024);
	strncpy(m_szRespData, szRespData, 1023);
}

void kaCSecsResponseData::Set_Resp_Data(int iAuxData, int iRespMode, char* szRespData)
{
	Set_Aux_Data(iAuxData);
	Set_Resp_Mode(iRespMode);
	Set_Resp_Data(szRespData);
}

void kaCSecsResponseData::Set_Wait_Response(bool bsts)
{
	m_bWaitResponse = bsts;
}

bool kaCSecsResponseData::Get_Wait_Response()
{
	return m_bWaitResponse;
}

void kaCSecsResponseData::Set_End_Flag(bool bFlag)
{
	m_bEndPacket = bFlag;
}

bool kaCSecsResponseData::Get_End_Flag()
{
	return m_bEndPacket;
}

void kaCSecsResponseData::Set_To_Host(bool bFlag)
{
	m_bToHost = bFlag;
}

bool kaCSecsResponseData::Get_To_Host()
{
	return m_bToHost;
}

bool kaCSecsResponseData::Parse(char* pszData)
{
	int		ix;
	char	szData[1024] = { 0, };

	// Header 분리
	if (m_pKStr->Str_Extract_With_Seperator(pszData, '|', 1, szData, 1023) <= 0) {
		return false;
	}
	Set_Key_Value(szData);

	// Response Mode
	memset(szData, 0, sizeof(char) * 1024);
	if (m_pKStr->Str_Extract_With_Seperator(pszData, '|', 2, &ix ) <= 0 ) {
		return false;
	}
	Set_Resp_Mode(ix);


	// HEW check
	memset(szData, 0, sizeof(char) * 1024);
	if (m_pKStr->Str_Extract_With_Seperator(pszData, '|', 3, szData, 1023) <= 0) {
		return false;
	}

	if (m_pKStr->Str_Has_Char(szData, 'E') >= 0) {
		Set_End_Flag(true);
	}
	else {
		Set_End_Flag(false);
	}

	if (m_pKStr->Str_Has_Char(szData, 'H') >= 0) {
		Set_To_Host(true);
	}
	else {
		Set_To_Host(false);
	}

	if (m_pKStr->Str_Has_Char(szData, 'W') >= 0) {
		Set_Wait_Response(true);
	}
	else {
		Set_Wait_Response(false);
	}

	// Response Data Check
	memset(szData, 0, sizeof(char) * 1024);
	if (m_pKStr->Str_Extract_With_Seperator(pszData, '|', 4, szData, 1023) <= 0) {
		return false;
	}

	Set_Resp_Data(szData);

	Set_Use(true);

	return true;
}

int kaCSecsResponseData::Get_Aux_Data()
{
	return m_iAuxData;
}

void kaCSecsResponseData::Set_Aux_Data(int iData)
{
	m_iAuxData = iData;
}

kaCSecsResponseData& kaCSecsResponseData::operator=(const kaCSecsResponseData& pRespData)
{
	if (this != &pRespData) {
		//m_ucPort = pCfg.m_ucPort;
		m_bWaitResponse = pRespData.m_bWaitResponse;
		m_bEndPacket = pRespData.m_bEndPacket;
		m_bToHost = pRespData.m_bToHost;
		m_iAuxData = pRespData.m_iAuxData;
		memcpy(this->m_szRespData, pRespData.m_szRespData, 1024);

		if (m_pKStr == NULL) {
			m_pKStr = new CKStr();
		}

	}

	return *this;
}


unsigned char kaCSecsResponseData::Get_StreamNo()			// Key value에서 구한다.
{
	char* pStr = Get_Key_Value();
	int	idx = -1;
	bool bOk = false;
	long lSize = 0;
	unsigned char ucResult = 0;

	idx = m_pKStr->Get_Char_PosIndex(pStr, 'F', 1);
	lSize = m_pKStr->Get_Size_By_Index(1, idx - 1);

	m_pMemStr->ClearString(true);
	m_pMemStr->AppendString((unsigned char *) (pStr + 1), lSize);			// Stream Data를 잘라낸다.
	
	ucResult = (unsigned char) m_pMemStr->StrToInt(bOk);
	if (bOk == false) {
		return -1;
	}

	return ucResult;
}


unsigned char kaCSecsResponseData::Get_FunctionNo()			// Key value에서 구한다.
{
	char* pStr = Get_Key_Value();
	int	idx = -1;
	long lSize = 0;
	bool bOk = false;
	unsigned char ucResult = 0;

	idx = m_pKStr->Get_Char_PosIndex(pStr, 'F', 1) + 1;
	lSize = m_pKStr->Get_Size_By_Index(idx, strlen(pStr) - 1);

	m_pMemStr->ClearString(true);
	m_pMemStr->AppendString((unsigned char *)(pStr + idx), lSize );			// Func Data를 잘라낸다.

	ucResult = (unsigned char) m_pMemStr->StrToInt(bOk);
	if (bOk == false) {
		return -1;
	}

	return ucResult;
}