#pragma once
#include "CResponseItem.h"

class CComm
{
private:
	bool m_bUse;

public:
	CComm();
	~CComm();
	
	bool Get_Use();
	void Set_Use(bool bUse);

	virtual void ClearBuffer() {};
	virtual bool ReadChar(char& ch) { return false; };
	virtual bool WriteChar(char ch) { return false; };
	virtual bool WriteBinBlock(unsigned char* uch, short sSize) { return false; };
	virtual bool LogStart(CString srFilePath, bool bBinLogMode) { return false; };
	virtual void LogStop() {};
	
	// sResKey.Length가 0보다 크면 Response가 존재하는 것이다.
	virtual bool Process_Read_Data(unsigned char* uChar, unsigned int iSize, CString& sResKey) { return false; };
	virtual bool Send_Response(CResponseItem* pRespItem) { return false; };
};

