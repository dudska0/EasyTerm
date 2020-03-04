#include "pch.h"
#include <windows.h>
#include <malloc.h>

#include "kaMemStr.h"

kaMemStr::kaMemStr(void)
{
	Set_Mem_Length(0);
	Set_Str_Length(0);
	m_pbyStr = NULL;
}


kaMemStr::~kaMemStr(void)
{
	if (GetString() != NULL) {
		this->ClearString(false);
	}
}

void kaMemStr::Set_Mem_Length(long length)
{
	this->m_lSize = length;
	if (length <= 0) {
		Set_Str_Length(0);
	}
}

void kaMemStr::Set_Str_Length(long length)
{
	this->m_lContentsLength = length;
}

bool kaMemStr::Alloc_Str(long lSize, bool bRemoveOld)
{
	if (lSize > Get_Mem_Length()) {
		if (bRemoveOld) this->ClearString(false);

		m_pbyStr = (unsigned char*)malloc(lSize);
		if (m_pbyStr == NULL) {
			if (Get_Mem_Length() != 0) {
				Set_Mem_Length(0);
			}
			return false;
		}
		else {
		}
		memset(m_pbyStr, 0, sizeof(unsigned char)*lSize);
		Set_Mem_Length(lSize);
	}

	return true;
}


bool kaMemStr::SetString(unsigned char* pucStr, long lSize)
{
	if (Alloc_Str(lSize, true) == false) {
		return false;
	}

	if (pucStr == NULL) {
		memset(this->m_pbyStr, 0, lSize);
		Set_Str_Length(0);
	}
	else {
		memcpy(this->m_pbyStr, pucStr, lSize);
		Set_Str_Length(lSize);
	}


	return true;
}

bool kaMemStr::AppendString(unsigned char* pucStr, long lSize)
{
	if (Get_Mem_Length() != Get_Str_Length()) {
		//if (Get_Mem_Length() >= Get_Str_Length() + lSize + 1 ) {
		if ((Get_Mem_Length() - 1) >= Get_Str_Length() + lSize ) {
			memcpy(m_pbyStr + m_lContentsLength, pucStr, sizeof(unsigned char) * lSize);
			//*(m_pbyStr + m_lContentsLength + lSize) = 0x00;
			Set_Str_Length(Get_Str_Length() + lSize);
			return true;
		}
	}

	unsigned char* pTemp = NULL;
	long lStrSize = Get_Str_Length();
	long lMemSize = Get_Mem_Length();

	if (lMemSize > 0) {
		// Current Data 대피
		pTemp = m_pbyStr;
	}

//	if (Alloc_Str(lStrSize + lSize + 1, false) == false) {						// reallocate str
	if (Alloc_Str(lStrSize + lSize + 1, false) == false) {						// reallocate str
		return false;
	}

	if (lMemSize > 0) {														// Data 원복 (Old String 복원)
		memcpy(m_pbyStr, pTemp, lStrSize);
	}

	memcpy(m_pbyStr + lStrSize, pucStr, sizeof(unsigned char) * lSize);		// New Data Write
	//*(m_pbyStr + m_lContentsLength + lSize) = 0x00;
	Set_Str_Length(lStrSize + lSize);										// Size 정리

	if (lMemSize > 0) {														// Remove Old String
		free(pTemp);
		pTemp = NULL;
	}

	return true;
}

bool kaMemStr::AppendChar(unsigned char ch )
{
	if (Get_Mem_Length() != Get_Str_Length()) {
		if (Get_Mem_Length() >= Get_Str_Length() + 1) {
			//memcpy(this->m_pbyStr + this->m_lContentsLength, pucStr, lSize);
			*(m_pbyStr + m_lContentsLength) = ch;
			Set_Str_Length(Get_Str_Length() + 1);
			return true;
		}
	}

	unsigned char* pTemp = NULL;
	long lStrSize = Get_Str_Length();
	long lMemSize = Get_Mem_Length();

	if (lMemSize > 0) {
		// Current Data 대피
		pTemp = m_pbyStr;
	}

	if (Alloc_Str(lStrSize + 256, false) == false) {					// reallocate str
		return false;
	}

	if (lMemSize > 0) {													// Data 원복 (Old String 복원)
		memcpy(m_pbyStr, pTemp, lStrSize);
	}

	//memcpy(m_pbyStr + lStrSize, pucStr, lSize);						// New Data Write
	*(m_pbyStr + lStrSize) = ch;
	Set_Str_Length(lStrSize + 1);										// Size 정리

	if (lMemSize > 0) {													// Remove Old String
		free(pTemp);
		pTemp = NULL;
	}

	return true;
}

bool kaMemStr::SetByte(int index, unsigned char uch)
{
	if (index >= this->Get_Mem_Length()) {
		return false;
	}

	*(m_pbyStr + index) = uch;

	return true;
}

bool kaMemStr::SetMultiByte(int index, unsigned char* pStr, long lSize)
{
	// index 2번 시작 3 Byte 사용은 2,3,4 이다.
	if (((long)index + lSize - 1) >= this->Get_Mem_Length()) {
		return false;
	}

	if (pStr == NULL) {
		return false;
	}

	memcpy(m_pbyStr + index, pStr, lSize);

	return true;
}


long kaMemStr::Get_Mem_Length(void)
{
	return m_lSize;
}

long kaMemStr::Get_Str_Length(void)
{
	return m_lContentsLength;
}


unsigned char* kaMemStr::GetString(void)
{
	return m_pbyStr;
}


void kaMemStr::ClearString(bool bOnlyContentsRemove)
{
	if (m_pbyStr == NULL) {
		if (Get_Mem_Length() != 0) {
			Set_Mem_Length(0);
		}
		return;
	}

	if (bOnlyContentsRemove)
	{
		memset(m_pbyStr, 0, m_lSize);
		Set_Str_Length(0);
	}
	else {
		if (m_pbyStr != NULL) {
			free(m_pbyStr);
		}

		m_pbyStr = NULL;
		Set_Mem_Length(0);
	}
}

long kaMemStr::StrToInt(bool& bOK)
{
	long lResult = 0;
	bOK = true;

	if (NumStr_Entirety_Check((char *) m_pbyStr) == false) {
		bOK = false;
		return 0;
	}

	lResult = strtol((char *)m_pbyStr, NULL, 10);
	
	return lResult;

}

//unsigned char*	m_pbyStr;
//long			m_lSize;
//long			m_lContentsLength;

bool kaMemStr::GetByteSum(int iStart, int iEnd, int& iRetSum)
{
	int	i;

	if (iStart < 0)		return false;
	if (iEnd == -1) {
		iEnd = Get_Str_Length();
	}
	if (iEnd > Get_Str_Length())	return false;
	if (iEnd >= Get_Mem_Length())	return false;

	iRetSum = 0;
	for (i = iStart; i < iEnd; i++) {
		iRetSum += *(m_pbyStr + i);
	}

	return true;
}

kaMemStr& kaMemStr::operator=(const kaMemStr& pStr)
{
	if (this != &pStr) {
		m_lSize = pStr.m_lSize;
		m_lContentsLength = pStr.m_lContentsLength;

		if (m_pbyStr != NULL) {
			delete(m_pbyStr);
			m_pbyStr = NULL;
		}

		m_pbyStr = (unsigned char *) malloc(m_lSize);
		if (m_pbyStr != NULL) {
			memcpy(m_pbyStr, pStr.m_pbyStr, sizeof(unsigned char) * m_lSize);
		}

	}

	return *this;
}

