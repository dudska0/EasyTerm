#include "pch.h"
#include "kaSecsDecoder.h"


kaSecsDecoder::kaSecsDecoder()
{
	m_Str.Empty();
	m_pSecsTool = new kaSecsTool();
}


kaSecsDecoder::~kaSecsDecoder()
{
	if (m_pSecsTool != NULL) {
		delete m_pSecsTool;
		m_pSecsTool = NULL;
	}
}

int kaSecsDecoder::ReadStream(unsigned char* pszStr)
{
	//------------------------------------------------------------------------------------------------
	if (Header_Anal(pszStr + 1) == false) {
		return -1;
	}
	//------------------------------------------------------------------------------------------------
	m_Str.Empty();
	//------------------------------------------------------------------------------------------------
	if (*pszStr > 0x0a) {
		SecsDataDecode(pszStr + 11);
	}
	//------------------------------------------------------------------------------------------------

	return m_Str.GetLength();
}

long kaSecsDecoder::SecsDataDecode(unsigned char* ucArr)
{
	int  i;
	int idata;
	int  iLengthOfLine = 0;
	long lnextoffset = 0;			// 시작 Header 1 Byte
	long li = 0;
	short sData;
	float fdata;
	DWORD dwData;
	double dbdata;
	unsigned char ul[4] = { 0, };
	unsigned char* ptr = NULL;
	unsigned char  ucType = 0;
	unsigned char debugchar[128];

	memcpy(debugchar, ucArr, 127);

	iLengthOfLine = *ucArr & 0x03;					// 길이를 표현할 line의 수
	ucType = (*ucArr & 0xfc);						// Data Type;

	lnextoffset = 1;								// 다음 Byte를 가르키도록 하자

	for (lnextoffset = 1; lnextoffset <= iLengthOfLine; lnextoffset++) {
		ul[iLengthOfLine - lnextoffset] = *(ucArr + lnextoffset);
	}

	unInt32 ul32;

	memset(&ul32, 0, sizeof(unInt32));
	memcpy(ul32.by_i32Data, ul, 4);

	switch (ucType) {
		case LISTTYPE:
			m_Str.AppendFormat("<L%ld", ul32.i32Data);
			for (i = 0; i < ul32.i32Data; i++) {
				lnextoffset += SecsDataDecode(ucArr + lnextoffset);
			}
			m_Str.AppendFormat(">");
			break;

		case BINARYTYPE:
			m_Str.AppendFormat("<B:%d>", *(ucArr + lnextoffset));
			lnextoffset++;
			break;

		case BOOLEANTYPE:
			m_Str.AppendFormat("<BOOLEAN:");
			if (*(ucArr + lnextoffset) == 0) {
				m_Str.AppendFormat("F>");
			}
			else {
				m_Str.AppendFormat("T>");
			}
			lnextoffset++;
			break;


		case INT1TYPE:
			m_Str.AppendFormat("<I1:%d>", ((char) *(ucArr + lnextoffset)));
			lnextoffset++;
			break;

		case INT2TYPE:
			sData = Extract_Short_With_Bit(ucArr + lnextoffset, -1);
			m_Str.AppendFormat("<I2:%d>", sData);
			lnextoffset += 2;
			break;

		case INT4TYPE:
			idata = Extract_LONG_With_Bit(ucArr + lnextoffset, -1);
			m_Str.AppendFormat("<I4:%d>", idata);
			lnextoffset += 4;
			break;

		case FLT4TYPE :
			fdata = Extract_Float_Data(ucArr + lnextoffset);
			m_Str.AppendFormat("<F4:%f>", fdata);
			lnextoffset += 4;
			break;

		case FLT8TYPE:
			dbdata = Extract_Double_Data(ucArr + lnextoffset);
			m_Str.AppendFormat("<F8:%lf>", dbdata);
			lnextoffset += 8;
			break;

		case UINT1TYPE:
			m_Str.AppendFormat("<U1:%u>", ((char) *(ucArr + lnextoffset)));
			lnextoffset += 1;
			break;

		case UINT2TYPE:
			sData = Extract_Short_With_Bit(ucArr + lnextoffset, -1);
			m_Str.AppendFormat("<U2:%u>", sData);
			lnextoffset += 2;
			break;

		case UINT4TYPE:
			idata = Extract_LONG_With_Bit(ucArr + lnextoffset, -1);
			m_Str.AppendFormat("<U4:%u>", idata);
			lnextoffset += 4;
			break;

		case UINT8TYPE:
			dwData = Extract_LONG_With_Bit(ucArr + lnextoffset, -1);
			m_Str.AppendFormat("<U8:%lu>", dwData);
			lnextoffset += 8;
			break;

		case ASCIITYPE:
			m_Str.AppendFormat("<A%d:", ul32.i32Data);
			for (i = 0; i < ul32.i32Data; i++) {
				m_Str.AppendFormat("%c", *(ucArr + lnextoffset));
				lnextoffset++;
			}
			m_Str.AppendFormat(">");
			break;


		default :
			return -1;
	}

	return lnextoffset;
}

bool kaSecsDecoder::Extract_Bit(unsigned char ucByte, unsigned char ucPos)
{
	unsigned char ucMask = ( 1 << ucPos);
	if ((ucByte & ucMask) == 0) 
	{
		return false;
	}
	else
	{
		return true;
	}
}

short kaSecsDecoder::Extract_DeviceCode(unsigned char* ucByte)
{
	unsigned char ucArr[2] = { 0, };
	memcpy(ucArr, ucByte, sizeof(BYTE) * 2);
	ucArr[0] &= 0x7f;
	memcpy(ucArr, ucByte, sizeof(BYTE) * 2);

	return m_pSecsTool->kaSecs_2Byte_To_2ByteInt(ucArr);
}

void kaSecsDecoder::Set_DeviceID(short sid)
{
	this->m_sDeviceID = sid;
}

short kaSecsDecoder::Get_DeviceID()
{
	return this->m_sDeviceID;
}

void kaSecsDecoder::Set_Stream(unsigned char ucStream)
{
	m_ucSx = ucStream;
}

void kaSecsDecoder::Set_Function(unsigned char ucFunction)
{
	m_ucFy = ucFunction;
}

unsigned char kaSecsDecoder::Get_Stream()
{
	return m_ucSx;
}

unsigned char kaSecsDecoder::Get_Function()
{
	return m_ucFy;
}

void kaSecsDecoder::Set_Block(short sValue)
{
	m_sBlock = sValue;
}

short kaSecsDecoder::Get_Block()
{
	return m_sBlock;
}

void kaSecsDecoder::Set_SystemBytes(long ldata)
{
	m_lSystemBytes = ldata;
}

long kaSecsDecoder::Get_SystemBytes()
{
	return m_lSystemBytes;
}


unsigned char kaSecsDecoder::Extract_UChar_With_Bit(unsigned char ucData, int iPos)
{
	unsigned char ucMask = 1;

	if (iPos > 7) {
		return -1;
	}
	if (iPos >= 0) {
		ucMask <<= iPos;		// 1을 Extract하고자 하는 위치로 보낸다.
		ucMask ^= 0xff;			// ff와 연산을 하면  (not을 한 효과) ~(ucMask<<=iPos) 과 같다.
	}
	else {
		ucMask = 0xff;
	}

	return (unsigned char) ucData & ucMask;
}

short kaSecsDecoder::Extract_Short_With_Bit(unsigned char* pucArr, int iPos)
{
	short sData = m_pSecsTool->kaSecs_2Byte_To_2ByteInt(pucArr);
	short sMask = 1;

	if (iPos > 15) {
		return -1;
	}

	if (iPos >= 0) {
		sMask <<= iPos;				// 1을 Extract하고자 하는 위치로 보낸다.
		sMask ^= 0xffff;			// ff와 연산을 하면  (not을 한 효과) ~(ucMask<<=iPos) 과 같다.
	}
	else {
		sMask = (short)0xffff;
	}

	return (short) sData & sMask;
}

long kaSecsDecoder::Extract_LONG_With_Bit(unsigned char* pucArr, int iPos)
{
	long lData = m_pSecsTool->kaSecs_4Byte_To_ByteInt32(pucArr);
	long lMask = 1;

	if (iPos > 31) {
		return -1;
	}
	if (iPos >= 0) {
		lMask <<= iPos;				// 1을 Extract하고자 하는 위치로 보낸다.
		lMask ^= 0xffffff;			// ff와 연산을 하면  (not을 한 효과) ~(ucMask<<=iPos) 과 같다.
	}
	else {
		lMask = 0xffffffff;
	}

	return (long)lData & lMask;
}


float kaSecsDecoder::Extract_Float_Data(unsigned char* pucArr)
{
	// TODO: 여기에 구현 코드 추가.
	float fResult = m_pSecsTool->kaSecs_4Byte_To_ByteFloat(pucArr);
	return fResult;
}

double kaSecsDecoder::Extract_Double_Data(unsigned char* pucArr)
{
	double dbResult = m_pSecsTool->kaSecs_8Byte_To_ByteDouble(pucArr);
	return dbResult;
}

CString kaSecsDecoder::Get_Secs_Message()
{
	return this->m_Str;
}

int kaSecsDecoder::Get_StreamFunction(CString& StrFunc)
{
	StrFunc.Format("S%dF%d", this->m_ucSx, this->m_ucFy);
	return StrFunc.GetLength();
}


bool kaSecsDecoder::Header_Anal(unsigned char* pszStr)
{
	short sData;
	long  lData = 0;
	long  lOffset = 0;

	Set_To_Host(Extract_Bit(*pszStr, 7));									// Extract to Host

	sData = Extract_DeviceCode(pszStr);										// Get Device Code
	Set_DeviceID(sData);

	// Wait Flag
	Set_Wait_Response(Extract_Bit(*(pszStr + 2), 7));						//
	Set_Stream(Extract_UChar_With_Bit(*(pszStr + 2), 7));
	Set_Function(*(pszStr + 3));


	Set_End_Flag(Extract_Bit(*(pszStr + 4), 7));							// EndFlag
	sData = Extract_Short_With_Bit((pszStr + 4), 15);						// Block

	lData = Extract_LONG_With_Bit((pszStr + 6), -1);						// 여기까지가 Header
	Set_SystemBytes(lData);

	return true;
}
