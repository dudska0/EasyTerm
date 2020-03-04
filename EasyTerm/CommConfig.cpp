#include "pch.h"
#include "CommConfig.h"


CCommConfig::CCommConfig()
{
	m_ucPort = 1;
	m_sSpeed = 9600;
	m_ucDatabit = 8;
	m_ucStopbit = 1;
	m_ucParitybit = 0;
	m_ucDTRDSR = 0;
	m_ucRTSCTS = 0;
	m_ucXONXOFF = 0;
}

CCommConfig::~CCommConfig()
{
}

void CCommConfig::Set_Port(unsigned char ucPort)
{
	m_ucPort = ucPort;
}

void CCommConfig::Set_Port(CString str)
{
	int iPort = 0;
	char* pszStr = LPSTR(LPCTSTR(str));
	if (strncmp(pszStr, "COM", 3) == 0) {
		iPort = strtol(pszStr + 3, NULL, 10);
		m_ucPort = (unsigned char)iPort;
	}
}

unsigned char CCommConfig::Get_Port()
{
	return m_ucPort;
}

void CCommConfig::Set_Stop(unsigned char ucStop)
{
	m_ucStopbit = ucStop;
}

unsigned char CCommConfig::Get_Stop()
{
	return m_ucStopbit;
}

void CCommConfig::Set_Speed(short ucSpeed)
{
	m_sSpeed = ucSpeed;
}
short CCommConfig::Get_Speed()
{
	return m_sSpeed;
}

void CCommConfig::Set_Databit(unsigned char ucDatabit)
{
	m_ucDatabit = ucDatabit;
}

unsigned char CCommConfig::Get_Databit()
{
	return m_ucDatabit;
}

void CCommConfig::Set_Parity(unsigned char ucParity)
{
	m_ucParitybit= ucParity;
}

unsigned char CCommConfig::Get_Parity()
{
	return m_ucParitybit;
}

void CCommConfig::Set_DTRDSR(unsigned char ucDtrDsr)
{
	m_ucDTRDSR = ucDtrDsr;
}

unsigned char CCommConfig::Get_DTRDSR()
{
	return m_ucDTRDSR;
}

void CCommConfig::Set_RTSCTS(unsigned char ucRTSCTS)
{
	m_ucRTSCTS = ucRTSCTS;
}

unsigned char CCommConfig::Get_RTSCTS()
{
	return m_ucRTSCTS;
}

void CCommConfig::Set_XONXOFF(unsigned char ucXonXoff)
{
	m_ucXONXOFF = ucXonXoff;
}

unsigned char CCommConfig::Get_XONXOFF()
{
	return m_ucXONXOFF;
}

CCommConfig& CCommConfig::operator=(const CCommConfig& pCfg)
{
	if (this != &pCfg) {
		m_ucPort = pCfg.m_ucPort;
		m_sSpeed = pCfg.m_sSpeed;
		m_ucDatabit = pCfg.m_ucDatabit;
		m_ucStopbit = pCfg.m_ucStopbit;
		m_ucParitybit = pCfg.m_ucParitybit;
		m_ucDTRDSR = pCfg.m_ucDTRDSR;
		m_ucRTSCTS = pCfg.m_ucRTSCTS;
		m_ucXONXOFF = pCfg.m_ucXONXOFF;
	}

	return *this;
}