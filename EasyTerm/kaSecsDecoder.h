#pragma once
#include "CSecsResponseData.h"
#include "kaSecsEncoder.h"

class kaSecsDecoder : public kaCSecsResponseData
{
private:
	short m_sDeviceID;
	unsigned char m_ucSx;
	unsigned char m_ucFy;
	short int m_sBlock;
	long m_lSystemBytes;
	kaSecsTool* m_pSecsTool;
	CString m_Str;

	void Set_DeviceID(short sid);
	void Set_Stream(unsigned char byStream);
	void Set_Function(unsigned char byFunction);
	void Set_Block(short sValue);
	void Set_SystemBytes(long ldata);

public:
	kaSecsDecoder();
	~kaSecsDecoder();

	int ReadStream(unsigned char* pszStr);
	bool Extract_Bit(unsigned char ucByte, unsigned char ucPos);
	short Extract_DeviceCode(unsigned char* ucByte);

	short Get_DeviceID();											
	unsigned char Get_Stream();
	unsigned char Get_Function();
	short Get_Block();
	long Get_SystemBytes();

	long SecsDataDecode(unsigned char* ucArr);

	unsigned char Extract_UChar_With_Bit(unsigned char ucStream, int iPos);
	short Extract_Short_With_Bit(unsigned char* pucArr, int iPos);
	long Extract_LONG_With_Bit(unsigned char* pucArr, int iPos);

	float Extract_Float_Data(unsigned char* pucArr);
	double Extract_Double_Data(unsigned char* pucArr);

	CString Get_Secs_Message();
	int Get_StreamFunction(CString& StrFunc);
	bool Header_Anal(unsigned char* pszStr);
};



