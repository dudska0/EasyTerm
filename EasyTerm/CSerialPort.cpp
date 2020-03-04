#include "pch.h"
#include "CSerialPort.h"
#include "CResponseItem.h"

#include "k:\h\CimRs232.h"

#pragma comment( lib, "k:\\lib\\CimRs232.lib")


CSerialPort::CSerialPort()
{
	m_pList = new CStringList();
}

CSerialPort::~CSerialPort() 
{
	if (m_pList != NULL) {
		delete m_pList;
		m_pList = NULL;
	}
}


CStringList* CSerialPort::Scan_Serial_Port()
{
	// TODO: 여기에 구현 코드 추가.
	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[20], szName[100];
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	
	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));

	m_pList->RemoveAll();

	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms724865(v=vs.85).aspx  
	//hKey - 레지스터키 핸들  
	//index - 값을 가져올 인덱스.. 다수의 값이 있을 경우 필요  
	//szName - 항목값이 저장될 배열  
	//dwSize - 배열의 크기  
	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
	{
		index++;

		//https://msdn.microsoft.com/en-us/library/windows/desktop/ms724911(v=vs.85).aspx  
		//szName-레지터스터 항목의 이름  
		//dwType-항목의 타입, 여기에서는 널로 끝나는 문자열  
		//szData-항목값이 저장될 배열  
		//dwSize2-배열의 크기  
		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		if (strncmp(szData, "COM", 3) == 0) {
			m_pList->AddHead(szData);
		}

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;
	}

	RegCloseKey(hKey);

	return m_pList;
}


int CSerialPort::GetNoOfPort()
{
	Scan_Serial_Port();
	return m_pList->GetCount();
}

bool CSerialPort::OpenPort(CCommConfig* pConfig)
{
	int iPortNo  = pConfig->Get_Port();
	int iSpeed	 = pConfig->Get_Speed();
	int iData	 = pConfig->Get_Databit();
	int iStop	 = pConfig->Get_Stop() + 1;
	int iParity  = pConfig->Get_Parity();
	int iDTRDSR  = pConfig->Get_DTRDSR();
	int iRTSCTS  = pConfig->Get_RTSCTS();
	int iXonXOff = pConfig->Get_XONXOFF();

	return OpenPort(iPortNo, iSpeed, iData, iStop, iParity, iDTRDSR, iRTSCTS, iXonXOff);
}

bool CSerialPort::OpenPort(int iPortNo, int iSpeed, int iData, int iStop, int iParity, int iDTRDSR, int iRTSCTS, int iXonXOff)
{
	bool bResult = false;

	if (RS232_Connect_Port(iPortNo, iSpeed, iData, iStop, iParity, iDTRDSR, iRTSCTS, iXonXOff) == 0) {
		bResult = false;
	}
	else {
		bResult = true;
	}

	Set_Use(bResult);

	return bResult;
}

void CSerialPort::ClosePort()
{
	RS232_Disconnect_Port();
	Set_Use(false);
}

void CSerialPort::ClearBuffer()
{
	RS232_Clear_Buffer();
}

bool CSerialPort::ReadChar(char& ch)
{
	if (RS232_Read_Char(&ch)) {
		return true;
	}
	
	return false;
}

bool CSerialPort::WriteChar(char ch)
{
	if (RS232_Write_Char(ch)) {
		return true;
	}

	return false;
}

bool CSerialPort::WriteBinBlock(unsigned char* uch, short sSize)
{
	for (int i = 0; i < sSize; i++) {
		if (WriteChar((char) *(uch + i) == false)) {
			return false;
		}
	 }
	
	return true;
}


bool CSerialPort::LogStart(CString srFilePath, bool bBinLogMode)
{
	char* pszFilePath = LPSTR(LPCTSTR(srFilePath));


	RS232_Logging_Style(bBinLogMode, pszFilePath);
	RS232_Logging_Control(TRUE);

	return true;
}


void CSerialPort::LogStop()
{
	RS232_Logging_Control(FALSE);
}

bool CSerialPort::Process_Read_Data(unsigned char* uChar, unsigned int iSize, CString& sResKey)
{
	return false;
}

bool CSerialPort::Send_Response(CResponseItem* pRespItem) 
{ 
	return false; 
}

bool CSerialPort::ReadWaitChar(char chWait, int iTimeOut_Sec)
{
	char ch_l;
	DWORD dwTimeStart = 0;
	DWORD dwTimeOut = iTimeOut_Sec * 1000;

	dwTimeStart = GetTickCount();
	do {
		// Timeout Check
		if (iTimeOut_Sec > 0) {
			if ((GetTickCount() - dwTimeStart) > dwTimeOut) {
				return false;
			}
		}

		if (ReadChar(ch_l) == false) {
			Sleep(1);
			continue;
		}

		if (ch_l == chWait)	return true;

	} while (true);
}