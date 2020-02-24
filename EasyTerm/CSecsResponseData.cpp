#include <windows.h>

#include "pch.h"

#include "CSecsResponseData.h"


CSecsResponseData::CSecsResponseData()
{

}

CSecsResponseData::~CSecsResponseData()
{

}

char* CSecsResponseData::Get_Resp_Data(void *pParam)
{
	return this->m_szRespData;
}

void CSecsResponseData::Set_Resp_Data(char* szRespData)
{
	memcpy(m_szRespData, szRespData, sizeof(char) * 255);
}

void CSecsResponseData::Set_Resp_Data(int iAuxData, int iRespMode, char* szRespData)
{
	Set_Aux_Data(iAuxData);
	Set_Resp_Mode(iRespMode);
	Set_Resp_Data(szRespData);
}

void CSecsResponseData::Set_Wait_Response(bool bsts)
{
	m_bWaitResponse = bsts;
}

bool CSecsResponseData::Get_Wait_Response()
{
	return m_bWaitResponse;
}

void CSecsResponseData::Set_End_Flag(bool bFlag)
{
	m_bEndPacket = bFlag;
}

bool CSecsResponseData::Get_End_Flag()
{
	return m_bEndPacket;
}

void CSecsResponseData::Set_To_Host(bool bFlag)
{
	m_bToHost = bFlag;
}

bool CSecsResponseData::Get_To_Host()
{
	return m_bToHost;
}

bool CSecsResponseData::Parse(char* pszData)
{
	// Header ºÐ¸®

	// HEW check

	// Response Data Check


	return true;
}

int CSecsResponseData::Get_Aux_Data()
{
	return m_iAuxData;
}

void CSecsResponseData::Set_Aux_Data(int iData)
{
	m_iAuxData = iData;
}
