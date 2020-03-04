#pragma once
#include "CSerialPort.h"
#include "kaMemStr.h"
#include "CSecsResponseData.h"
#include "kaSecsEncoder.h"

namespace SECSCOMM
{
	enum {	
			WAIT_ENQ_0,			// �ʱ���� (ENQ�� ��ٸ��ų�(Next 5��) /  ENQ�� �����ų� (Next 3��))
		 /*	SEND_EOT_1,*/ 		// ������ - EOT������, �ٷ� WAIT_MAINDATA�� 
		 /*	SEMD_ENQ_2,*/		// ������ - ENQ������, �ٷ� WAIT_EOT���·� �����Ѵ�.
	  		WAIT_EOT_3,			// Dev->EC (ENQ�� ������ ����) (NEXT SEND_MAINDATA_4)
			SEND_MAINDATA_4,	// DEV->EC EOT�� �ް� ���¸� �ٲ㼭 �ٸ� Thread�� ������ ��ǿ��� ó�� �� �� �ֵ��� �Ѵ�.(Next 6��)
			WAIT_MAINDATA_5,	// EC->DEV (EOT�� ������ ����)  (���� ACK�� ������)
			SEND_ACK_6,			// Main Data�� ȭ�鿡 ǥ���ϱ� ���ؼ�
			WAIT_ACK_7,			// DEV->EC ACK�� ��ٸ���.
	};
}

class CSecsSerial : public CSerialPort
{
private:
	short m_sDevceID;

	int m_iCommStep;
	kaMemStr* m_pStr;
	kaSecsEncoder* m_pEncoder;

public:
	CSecsSerial();
	~CSecsSerial();

	kaSecsEncoder* GetEncoder();
	kaMemStr* GetMemStr();


	short Get_DeviceID();
	void Set_DeviceID(short sID);

	int ReadSecsData(int& iStream, int& iFunction, bool& bEnd, bool& bRecvWait, bool& bToHost, unsigned char* pucPayLoad);
	bool WriteSecsData(unsigned int uiSize, unsigned char* pucBuffer);

	unsigned int Get_Buffer_Length();
	unsigned char* Get_Buffer();
	void Clear_Buffer();
	void Clear_Data();

	void Set_CommStep(int iCommStep);
	int Get_CommStep();


	bool Wait_Control_Char(unsigned char* uChar, unsigned int iSize, unsigned char uCh_Wait, unsigned char uCh_Send, int iOKSts, int iErroSts);
	virtual bool Process_Read_Data(unsigned char* uChar, unsigned int iSize, CString& sResKey);					// Process data�� ���⼭ ó���Ѵ�.
	virtual bool Send_Response(kaCSecsResponseData* pRespItem);													// Response Item
};

