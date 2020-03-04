#include "pch.h"
#include "SecsSerial.h"
#include "AsciiTable.h"

using namespace SECSCOMM;

CSecsSerial::CSecsSerial()
{
	m_iCommStep = 0;
	m_pStr = new kaMemStr();
	m_pEncoder = new kaSecsEncoder();
}


CSecsSerial::~CSecsSerial()
{
	if (m_pStr != NULL) {
		delete m_pStr;
		m_pStr = NULL;
	}

	if (m_pEncoder != NULL) {
		delete m_pEncoder;
		m_pEncoder = NULL;
	}
}


int CSecsSerial::ReadSecsData(int& iStream, int& iFunction, bool& bEnd, bool& bRecvWait, bool& bToHost, unsigned char* pucPayLoad)
{
	return 0;
}

bool CSecsSerial::WriteSecsData(unsigned int uiSize, unsigned char* pucBuffer)
{
	unsigned int ui = 0;

	// �ܼ��� ������ ��ɸ� �߰���

	if ((Get_CommStep() != WAIT_ENQ_0) && (Get_CommStep() != SEND_MAINDATA_4)) {
		return false;
	}


	for (ui = 0; ui < uiSize; ui++) {
		if (WriteChar((char) *(pucBuffer + ui)) == false) {
			return false;
		}
	}

	return true;
}

unsigned int CSecsSerial::Get_Buffer_Length()
{
	return m_pStr->Get_Mem_Length();
}

unsigned char* CSecsSerial::Get_Buffer()
{
	return m_pStr->GetString();
}

void CSecsSerial::Clear_Buffer()
{
	m_pStr->ClearString(false);
}

void CSecsSerial::Clear_Data()
{
	m_pStr->ClearString(true);
}

void CSecsSerial::Set_CommStep(int iCommStep)
{
	m_iCommStep = iCommStep;
}

int CSecsSerial::Get_CommStep()
{
	return m_iCommStep;
}

bool CSecsSerial::Wait_Control_Char(unsigned char* uChar, unsigned int iSize, unsigned char uCh_Wait, unsigned char uCh_Send, int iOKSts, int iErroSts )
{
	if (iSize == 1) {
		if (*uChar == uCh_Wait) {													// ENQ�� ������, EOT�� ������.
			if (uCh_Send == 0xff) {
				Set_CommStep(iOKSts);
				return true;
			}
			else {
				if (WriteChar(uCh_Send)) {
					Set_CommStep(iOKSts);											// Main data ���
					return true;
				}
			}
		}
		else {																			// �ٸ� ���ڿ�
			Set_CommStep(iErroSts);
			return false;
		}
	}

	return false;
}

bool CSecsSerial::Process_Read_Data(unsigned char* uChar, unsigned int iSize, CString& sResKey )				// Process data�� ���⼭ ó���Ѵ�.
{
	switch (Get_CommStep()) 
	{
		case WAIT_ENQ_0 :		// ENQ�� �޾��� ��-> ENQ�� ������ -> EOT�� ������. -> Maindata ��� ���·� ����.
			return Wait_Control_Char(uChar, iSize, CODE_ASCII_ENQ, CODE_ASCII_EOT, WAIT_MAINDATA_5, WAIT_ENQ_0);

		/*
		case SEND_EOT_1 :			// ������.
			break;
		*/

		/*
		case SEMD_ENQ_2 :			// ������
			break;
		*/

		case WAIT_EOT_3 :		// EOT�� ������ ->	Main data�� ���� �� �ִ� ���·� �����.
			return Wait_Control_Char(uChar, iSize, CODE_ASCII_EOT, 0xff, SEND_MAINDATA_4, WAIT_ENQ_0);

		//case SEND_MAINDATA_4:																// �ܺ� ���¿��� Data�� ���� �� �ֵ��� �Ѵ�.
		//	break;																			// Data�� ������ ACK�� ��ٸ��� ���·� ��ȯ�Ѵ�.

		case WAIT_MAINDATA_5:																// ���� SECS Data�� ȭ�鿡 ǥ���ϵ��� �Ѵ�. (���⿡ ������ SECS Data�� �޴´�.)
			Set_CommStep(SEND_ACK_6);														// ���� ����
			break;

		case WAIT_ACK_7 :
			return Wait_Control_Char(uChar, iSize, CODE_ASCII_ACK, 0xff, WAIT_ENQ_0, WAIT_ENQ_0);
			break;

		default :
			Set_CommStep(WAIT_ENQ_0);
			return false;
	}
	

	return true;
}


bool CSecsSerial::Send_Response(kaCSecsResponseData* pRespItem)										// Response Item
{
	// make response packet
	char ch;
	int iStreamNo = pRespItem->Get_StreamNo();
	int iFuncNO = pRespItem->Get_FunctionNo();

	// Encode Data
	m_pStr->ClearString(true);
	if (m_pEncoder->Make_Packet(Get_DeviceID(), pRespItem, m_pStr) == false) {
		Set_CommStep(WAIT_ENQ_0);
		return false;
	}

	// Send ENQ
	WriteChar(CODE_ASCII_ENQ);

	Set_CommStep(WAIT_EOT_3);

	// Receive EOT
	if (ReadWaitChar(CODE_ASCII_EOT, 10) == false) {
		Set_CommStep(WAIT_ENQ_0);
		return false;
	}
	   
	Set_CommStep(SEND_MAINDATA_4);
	// Send Response
	if (WriteSecsData(m_pStr->Get_Str_Length(), m_pStr->GetString()) == false ) {
		Set_CommStep(WAIT_ENQ_0);
		return false;
	}

	// Receive ACK
	Set_CommStep(WAIT_ACK_7);
	if (ReadWaitChar(CODE_ASCII_ACK, 10) == false) {
		Set_CommStep(WAIT_ENQ_0);
		return false;
	}

	Set_CommStep(WAIT_ENQ_0);

	return true;
}

short CSecsSerial::Get_DeviceID()
{
	return m_sDevceID;
}

void CSecsSerial::Set_DeviceID(short sID)
{
	m_sDevceID = sID;
}

kaSecsEncoder* CSecsSerial::GetEncoder()
{
	return m_pEncoder;
}

kaMemStr* CSecsSerial::GetMemStr()
{
	return m_pStr;
}
