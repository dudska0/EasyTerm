#pragma once
#include "CSerialPort.h"
#include "kaMemStr.h"
#include "CSecsResponseData.h"
#include "kaSecsEncoder.h"

namespace SECSCOMM
{
	enum {	
			WAIT_ENQ_0,			// 초기상태 (ENQ를 기다리거나(Next 5번) /  ENQ를 보내거나 (Next 3번))
		 /*	SEND_EOT_1,*/ 		// 사용안함 - EOT보내고, 바로 WAIT_MAINDATA로 
		 /*	SEMD_ENQ_2,*/		// 사용안함 - ENQ보내고, 바로 WAIT_EOT상태로 변경한다.
	  		WAIT_EOT_3,			// Dev->EC (ENQ를 보내고 나서) (NEXT SEND_MAINDATA_4)
			SEND_MAINDATA_4,	// DEV->EC EOT를 받고 상태를 바꿔서 다른 Thread나 보내는 펑션에서 처리 할 수 있도록 한다.(Next 6번)
			WAIT_MAINDATA_5,	// EC->DEV (EOT를 보내고 나서)  (끝에 ACK를 보낸다)
			SEND_ACK_6,			// Main Data를 화면에 표시하기 위해서
			WAIT_ACK_7,			// DEV->EC ACK를 기다린다.
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
	virtual bool Process_Read_Data(unsigned char* uChar, unsigned int iSize, CString& sResKey);					// Process data를 여기서 처리한다.
	virtual bool Send_Response(kaCSecsResponseData* pRespItem);													// Response Item
};

