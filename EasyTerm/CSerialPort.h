#pragma once
#include <atlstr.h>
#include "CommConfig.h"
#include "Comm.h"
#include "CResponseItem.h"

class CSerialPort : public CComm
{
	private:
		CStringList* m_pList;

	public :
		CSerialPort();
		~CSerialPort();

		CStringList* Scan_Serial_Port();
		int GetNoOfPort();					// Scan_Serial_Port and return


		// �Ʒ��� RS232 ���� Code���� ���� source level���� �̽�����
		bool OpenPort(int iPortNo, int iSpeed, int iData, int iStop, int iParity, int iDTRDSR, int iRTSCTS, int iXonXOff);
		bool OpenPort(CCommConfig*  pConfig);
		void ClearBuffer();
		void ClosePort();

		bool ReadWaitChar(char chWait, int iTimeOut_Sec);
		virtual bool ReadChar(char& ch);
		virtual bool WriteChar(char ch);
		virtual bool WriteBinBlock(unsigned char* uch, short sSize);
		virtual bool LogStart(CString srFilePath, bool bBinLogMode);
		virtual void LogStop();

		//virtual bool Process_Read_Data(unsigned char* uChar, unsigned int iSize);						// Process data�� ���⼭ ó���Ѵ�.
		virtual bool Process_Read_Data(unsigned char* uChar, unsigned int iSize, CString& sResKey);			// Process data�� ���⼭ ó���Ѵ�.
		virtual bool Send_Response(CResponseItem* pRespItem);												// Response Item
};

