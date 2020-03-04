#pragma once
#include <Windows.h>
#include "KStr.h"

class kaMemStr : public CKStr
{
private:
	unsigned char*	m_pbyStr;
	long			m_lSize;
	long			m_lContentsLength;

private:
	void			Set_Mem_Length(long lSize);
	void			Set_Str_Length(long lSize);
	bool			Alloc_Str(long lSize, bool bRemoveOld);


public:
	kaMemStr();
	~kaMemStr();

	long Get_Mem_Length();
	long Get_Str_Length();

	unsigned char* GetString();
	bool SetString(unsigned char* pucStr, long lSize);
	bool AppendChar(unsigned char ch);
	bool AppendString(unsigned char* pucStr, long lSize);
	bool SetByte(int index, unsigned char uch);
	bool SetMultiByte(int index, unsigned char* pStr, long lSize);
	void ClearString(bool bOnlyContentsRemove);
	long StrToInt(bool& bOK);
	bool GetByteSum(int iStart, int iEnd, int& iRetSum);
	//void ByteSwap(unsigned char* pcBuf, long lSize);

	kaMemStr& operator=(const kaMemStr& pStr);

};

