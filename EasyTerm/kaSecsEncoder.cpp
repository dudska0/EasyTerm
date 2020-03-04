#include "pch.h"
#include <windows.h>
#include <math.h>

#include "HeaderDef.h"

#include "CSecsResponseData.h"

#include "kaMemStr.h"

#include "CResponseItem.h"
#include "kaSecsEncoder.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
kaSecsTool::kaSecsTool()
{
}


kaSecsTool::~kaSecsTool()
{
}


bool kaSecsTool::ByteSwap(unsigned char* pucByte, short size)
{
	short i;
	BYTE  byBuf[16] = { 0, };

	if (size > 16) {
		return false;
	}

	for (i = 0; i < size; i++) {
		*(byBuf + i) = *(pucByte + (size - (i + 1)));
	}

	memcpy(pucByte, byBuf, size);

	return true;
}


int kaSecsTool::kaSecs_2Byte_Int_To_Byte(short sNum, unsigned char* pBuf)
{
	unShort	shData;

	if (pBuf == NULL) {
		return 0;
	}

	memset(&shData, 0, sizeof(unShort));
	shData.sData = sNum;

	if (ByteSwap(shData.ucByte, 2)) {
		memcpy(pBuf, shData.ucByte, 2);
		return 2;
	}

	return 0;
}

int kaSecsTool::kaSecs_ByteInt32_To_Byte(INT32 i32Num, unsigned char* pBuf)
{
	unInt32 ui32Data;

	if (pBuf == NULL) {
		return 0;
	}

	memset(&ui32Data, 0, sizeof(unInt32));
	ui32Data.i32Data = i32Num;

	if (ByteSwap(ui32Data.by_i32Data, 4)) {
		memcpy(pBuf, ui32Data.by_i32Data, 4);
		return 2;
	}

	return 0;
}

int  kaSecsTool::kaSecs_ByteInt64_To_Byte(INT64 i64Num, unsigned char* pBuf)
{
	unInt64 ui64Data;

	if (pBuf == NULL) {
		return 0;
	}

	memset(&ui64Data, 0, sizeof(unInt64));
	ui64Data.i64Data = i64Num;

	if (ByteSwap(ui64Data.by_i64Data, 8)) {
		memcpy(pBuf, ui64Data.by_i64Data, 8);
		return 2;
	}

	return 0;
}

int kaSecsTool::kaSecs_Float_To_Byte(float fnum, unsigned char* pBuf)
{
	unFloat ufData;

	if (pBuf == NULL) {
		return 0;
	}

	memset(&ufData, 0, sizeof(unFloat));
	ufData.fdata = fnum;

	if (ByteSwap(ufData.by_float, 4)) {
		memcpy(pBuf, ufData.by_float, 4);
		return 2;
	}

	return 0;
}


int kaSecsTool::kaSecs_Double_To_Byte(double dbnum, unsigned char* pBuf)
{
	unDouble udbData;

	if (pBuf == NULL) {
		return 0;
	}

	memset(&udbData, 0, sizeof(unFloat));
	udbData.dbdata = dbnum;

	if (ByteSwap(udbData.by_double, 8)) {
		memcpy(pBuf, udbData.by_double, 8);
		return 2;
	}

	return 0;
}


short kaSecsTool::kaSecs_2Byte_To_2ByteInt(unsigned char* pBuf)		// 2Byte -> Short
{
	unShort	shData;
	if (pBuf == NULL) {
		return 0;
	}

	memset(&shData, 0, sizeof(unShort));
	memcpy(shData.ucByte, pBuf, 2);
	ByteSwap(shData.ucByte, 2);

	return shData.sData;
}

long  kaSecsTool::kaSecs_4Byte_To_ByteInt32(unsigned char* pBuf)		// 4Byte -> Long
{
	unInt32 uI32_Data;

	memcpy(uI32_Data.by_i32Data, pBuf, 4);
	ByteSwap(uI32_Data.by_i32Data, 4);

	return (long) uI32_Data.i32Data;
}

INT64 kaSecsTool::kaSecs_8Byte_To_ByteInt64(unsigned char* pBuf)		// 28yte -> Long
{
	unInt64 uI64_Data;

	memcpy(uI64_Data.by_i64Data, pBuf, 8);
	ByteSwap(uI64_Data.by_i64Data, 8);

	return (INT64)uI64_Data.i64Data;
}

float  kaSecsTool::kaSecs_4Byte_To_ByteFloat(unsigned char* pBuf)		// 2Byte -> Long
{
	unFloat ulFloatData;

	memcpy(ulFloatData.by_float, pBuf, 4);
	ByteSwap(ulFloatData.by_float, 4);

	return ulFloatData.fdata;
}

double kaSecsTool::kaSecs_8Byte_To_ByteDouble(unsigned char* pBuf)		// 2Byte -> Long
{
	unDouble ulDouble;

	memcpy(ulDouble.by_double, pBuf, 8);
	ByteSwap(ulDouble.by_double, 8);

	return ulDouble.dbdata;
}

bool kaSecsTool::kaSecs_Find_Secondary_Message(char* pszPrimaryMsg, char* pszSecondaryMsg)
{
	long li;
	long lsize = strlen(pszPrimaryMsg);
	char szBuf[10] = { 0, };
	short sFunc = 0;

	if (pszPrimaryMsg == NULL) {
		return false;
	}

	if (pszSecondaryMsg == NULL) {
		return false;
	}

	for (li = 0; li < lsize; li++) {
		if (*(pszPrimaryMsg + li) == 'F') {
			break;
		}
	}

	strncpy(szBuf, pszPrimaryMsg, li );						// Stream
	sFunc = (short) strtol(pszPrimaryMsg + li + 1, NULL, 10);		// Function
	if ((sFunc % 2) == 0) {
		return false;
	}

	sprintf(pszSecondaryMsg, "%sF%d", szBuf, sFunc + 1);
	
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

kaSecsEncoder::kaSecsEncoder()
{
	m_pTool = new kaSecsTool();
	m_sDevceID = 66;				// 초기 Device ID

	//m_pList = new CSecsItemList[MAX_MSG_NO];
	m_pStr = new CKStr();
}

kaSecsEncoder::~kaSecsEncoder()
{
	if (m_pTool != NULL) {
		delete m_pTool;
		m_pTool = NULL;
	}

	if (m_pStr != NULL) {
		delete m_pStr;
		m_pStr = NULL;
	}

	//if (m_pList != NULL) {
	//	delete m_pList;
	//	m_pList = NULL;
	//}
}

int kaSecsEncoder::Get_SizeDefine_LineNo(int iElementsNo, int iType, kaMemStr* pStr_ByteLength)
{
	int i;
	int	iResult = 0;

	//unInt32Size	unSize;
	unsigned char buf[32] = { 0, };

	switch (iType)
	{
	case LISTTYPE:								// ListType
	case BINARYTYPE:
	case INT1TYPE:
	case ASCIITYPE:
		for (i = 1; i <= 3; i++) {
			if (iElementsNo < pow(2, 8 * i)) {
				break;
			}
		}

		if (i > 3) {
			return -1;
		}

		iResult = i;			// 길이를 표시하는 line이 몇줄이 될지를 return 한다.

								//memset( &unSize, 0, sizeof(unInt32Size));
		if (pStr_ByteLength != NULL) {
			//Get_4ByteData(iElementsNo, &unSize);
			m_pTool->kaSecs_ByteInt32_To_Byte(iElementsNo, buf);
			pStr_ByteLength->SetMultiByte(1, buf, (long)iResult);
		}

		break;

	default:
		iResult = 0;
		break;
	}

	return iResult;
}

void kaSecsEncoder::AppendShortToByte_With_Flag(bool bFlag, short sData, unsigned char* pBuf)
{
	if (pBuf == NULL) {
		return;
	}

	m_pTool->kaSecs_2Byte_Int_To_Byte(sData, pBuf);
	if (bFlag) {
		*pBuf = *pBuf | 0x80;
	}
}

void kaSecsEncoder::AppendShortToByte_With_Flag(bool bFlag, short sData, kaMemStr* pRetStr)
{
	unsigned char buf[2] = { 0, };

	if (pRetStr == NULL) {
		return;
	}

	AppendShortToByte_With_Flag(bFlag, sData, buf);
	pRetStr->AppendString(buf, 2);
}

void kaSecsEncoder::AppendShortToByte_With_Flag(bool bFlag, unsigned char ucData1, unsigned char ucData2, unsigned char* pBuf)
{
	if (pBuf == NULL) {
		return;
	}

	*pBuf = ucData1;
	*(pBuf + 1) = ucData2;

	if (bFlag) {
		*pBuf = *pBuf | 0x80;
	}
}

void kaSecsEncoder::AppendShortToByte_With_Flag(bool bFlag, unsigned char ucData1, unsigned char ucData2, kaMemStr* pRetStr)
{
	unsigned char buf[2] = { 0, };

	if (pRetStr == NULL) {
		return;
	}

	AppendShortToByte_With_Flag(bFlag, ucData1, ucData2, buf);
	pRetStr->AppendString(buf, 2);
}



void kaSecsEncoder::AppendInt32ToByte(long i32_Data, unsigned char* pBuf)
{
	if (pBuf == NULL) {
		return;
	}

	m_pTool->kaSecs_ByteInt32_To_Byte(i32_Data, pBuf);
}


void kaSecsEncoder::AppendInt32ToByte(long i32_Data, kaMemStr* pRetStr)
{
	unsigned char buf[4] = { 0, };

	if (pRetStr == NULL) {
		return;
	}

	m_pTool->kaSecs_ByteInt32_To_Byte(i32_Data, buf);
	pRetStr->AppendString(buf, 4);
}


void kaSecsEncoder::AppendInt64ToByte(INT64 i64_Data, unsigned char* pBuf)
{
	if (pBuf == NULL) {
		return;
	}

	m_pTool->kaSecs_ByteInt64_To_Byte(i64_Data, pBuf);
}


void kaSecsEncoder::AppendInt64ToByte(INT64 i64_Data, kaMemStr* pRetStr)
{
	unsigned char buf[8] = { 0, };

	if (pRetStr == NULL) {
		return;
	}

	m_pTool->kaSecs_ByteInt64_To_Byte(i64_Data, buf);
	pRetStr->AppendString(buf, 8);
}

void kaSecsEncoder::AppendFloatToByte(float fdata, unsigned char* pBuf)
{
	if (pBuf == NULL) {
		return;
	}

	m_pTool->kaSecs_Float_To_Byte(fdata, pBuf);
}

void kaSecsEncoder::AppendFloatToByte(float fdata, kaMemStr* pRetStr)
{
	unsigned char buf[4] = { 0, };

	if (pRetStr == NULL) {
		return;
	}

	m_pTool->kaSecs_Float_To_Byte(fdata, buf);
	pRetStr->AppendString(buf, 4);
}


void kaSecsEncoder::AppendDoubleToByte(double dbdata, unsigned char* pBuf)
{
	if (pBuf == NULL) {
		return;
	}

	m_pTool->kaSecs_Double_To_Byte(dbdata, pBuf);
}


void kaSecsEncoder::AppendDoubleToByte(double dbdata, kaMemStr* pRetStr)
{
	unsigned char buf[8] = { 0, };

	if (pRetStr == NULL) {
		return;
	}

	m_pTool->kaSecs_Double_To_Byte(dbdata, buf);
	pRetStr->AppendString(buf, 8);
}



bool kaSecsEncoder::Append_Secs_Header(bool bToHost, short sDevID, bool bWait, unsigned char ucStream, unsigned char ucFunction, bool bEnd, short sBlock, long lSystemByte, kaMemStr* pRetStr)
{
	if (pRetStr == NULL) {
		return false;
	}

	AppendShortToByte_With_Flag(bToHost, sDevID, pRetStr);						// Device id
	AppendShortToByte_With_Flag(bWait, ucStream, ucFunction, pRetStr);			// SxFy / wait
	AppendShortToByte_With_Flag(bEnd, sBlock, pRetStr);							// Block Number
	AppendInt32ToByte(lSystemByte, pRetStr);									// System Byte

	return true;
}

void kaSecsEncoder::AppendDataHeader(int iType, long iNoOfElements, kaMemStr* pRetStr)
{
	long lDefLine = 0;
	unsigned char ucTypeInfo;

	if (pRetStr == NULL) {
		return;
	}

	lDefLine = Get_SizeDefine_LineNo(iNoOfElements, iType, NULL);

	ucTypeInfo = iType | (BYTE)lDefLine;
	pRetStr->AppendString(&ucTypeInfo, 1);

	AppendLengthLine_Data(iType, iNoOfElements, lDefLine, pRetStr);
}

void kaSecsEncoder::Append_ListData_Type(long iNoOfElements, kaMemStr* pRetStr)
{
	if (pRetStr == NULL) {
		return;
	}

	AppendDataHeader(LISTTYPE, iNoOfElements, pRetStr);
}

void kaSecsEncoder::AppendLengthLine_Data(int iType, int iNoOfElements, long lDefLine, kaMemStr* pRetStr)
{
	unsigned char ucTypeInfo[4] = { 0, };

	if (pRetStr == NULL) {
		return;
	}

	switch (lDefLine)
	{
	case 1:
		ucTypeInfo[0] = iNoOfElements;
		break;

	case 2:
		m_pTool->kaSecs_2Byte_Int_To_Byte(iNoOfElements, ucTypeInfo);
		break;

	case 3:
		m_pTool->kaSecs_ByteInt32_To_Byte(iNoOfElements, ucTypeInfo);
		break;

	default:
		return;
	}

	pRetStr->AppendString(ucTypeInfo, lDefLine);
}

void kaSecsEncoder::Append_1Byte_Type_With_Data(int iType, long iNoOfElements, unsigned char ucData, kaMemStr* pRetStr)
{
	if (pRetStr == NULL) {
		return;
	}

	if ((iType != BINARYTYPE) && (iType != BOOLEANTYPE) && (iType != INT1TYPE)) {
		return;
	}

	AppendDataHeader(iType, iNoOfElements, pRetStr);
	pRetStr->AppendString(&ucData, 1);
}

void kaSecsEncoder::Append_nByteStr_Type_With_Data(long iNoOfElements, char* pucArrData, kaMemStr* pRetStr)
{
	if (pRetStr == NULL) {
		return;
	}

	AppendDataHeader(ASCIITYPE, iNoOfElements, pRetStr);
	pRetStr->AppendString((unsigned char*)pucArrData, iNoOfElements);

}

int kaSecsEncoder::Get_MessageType_From_String(char* pszStr, int& iSize, long& lOffset_Contents_Start, long& lContents_Size)
{
	char ch;
	char buf[32] = { 0, };
	int  icnt = 0;

	// Parse Type String
	do {
		ch = *(pszStr + icnt);
		if (ch == '<')	break;
		else if (ch == ':') break;
		else if (ch == '>') break;
		else if (ch == 0x00) break;
		else if (ch == '\n') break;
		else buf[icnt] = ch;
		
		if (++icnt >= 32)	break;
	} while (ch != 0);

	if (icnt < 31) {
		buf[icnt] = 0x00;
	}

	if (buf[0] == 'L') {
		iSize = strtol(&buf[1], NULL, 10);
		return LISTTYPE;
	}
	else if (buf[0] == 'B') {
		if (buf[1] == ':') {
			iSize = 1;
			return BINARYTYPE;
		}
		else if (buf[1] == 'O') {
			iSize = 1;
			return BOOLEANTYPE;
		}
	}
	else if (buf[0] == 'I') {
		if (buf[1] == '1') {
			iSize = 1;
			return INT1TYPE;
		}
		else if (buf[1] == '2') {
			iSize = 2;
			return INT2TYPE;
		}
		else if (buf[1] == '4') {
			iSize = 4;
			return INT4TYPE;
		}
		else if (buf[1] == '8') {
			iSize = 8;
			return INT8TYPE;
		}
	}
	else if (buf[0] == 'U') {
		if (buf[1] == '1') {
			iSize = 1;
			return UINT1TYPE;
		}
		else if (buf[1] == '2') {
			iSize = 2;
			return UINT2TYPE;
		}
		else if (buf[1] == '4') {
			iSize = 4;
			return UINT4TYPE;
		}
		else if (buf[1] == '8') {
			iSize = 8;
			return UINT8TYPE;
		}
	}
	else if (buf[0] == 'F') {
		if (buf[1] == '4') {
			iSize = 4;
			return FLT4TYPE;
		}
		else if (buf[1] == '8') {
			iSize = 8;
			return FLT8TYPE;
		}
	}
	else if (buf[0] == 'A') {

		return LISTTYPE;
	}


	return -1;
}


bool kaSecsEncoder::Make_Packet_Contents(char* pszMainData, kaMemStr* pStr)
{
	long lSize;
	long lCnt = 0;
	int  iType = -1;
	int  iConentsSize = 0;
	INT64 i64data = 0;
	float fdata = 0;
	double dbdata = 0;
	int  iData = 0;
	bool bStrStart = false;
	
	char szContents[128] = { 0, };
	char szBuffer[2][64] = { 0, };

	lSize = strlen(pszMainData);

	if (*pszMainData == 'L') {
		iConentsSize = strtol( pszMainData+1, NULL, 10 );
		Append_ListData_Type(iConentsSize, pStr);
		return true;
	}
	else if (*pszMainData == 'A') {
		memset(szBuffer[0], 0, sizeof(char) * 64);
		m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, szBuffer[0], 63);
		Append_nByteStr_Type_With_Data(strlen(szBuffer[0]), szBuffer[0], pStr);
	}
	else if (*pszMainData == 'I') {
		memset(szBuffer[0], 0, sizeof(char) * 64);
		memset(szBuffer[1], 0, sizeof(char) * 64);
		m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 1, szBuffer[0], 63);

		if (strcmp(szBuffer[0], "I1") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, &iData);
			Append_1Byte_Type_With_Data(INT1TYPE, 1,(char) iData, pStr);
		}
		else if (strcmp(szBuffer[0], "I2") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, &iData);
			AppendShortToByte_With_Flag( false, (short)iData, pStr);
		}
		else if (strcmp(szBuffer[0], "I4") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, &iData);
			AppendInt32ToByte(iData, pStr);
		}
		else if (strcmp(szBuffer[0], "I8") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, szBuffer[1], 64);
			try
			{
				AppendInt64ToByte(_atoi64(szBuffer[1]), pStr);
			}
			catch (CException* )
			{
				return false;
			}
			
		}
	}
	else if (*pszMainData == 'U') {
		memset(szBuffer[0], 0, sizeof(char) * 64);
		memset(szBuffer[1], 0, sizeof(char) * 64);
		m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 1, szBuffer[0], 63);

		if (strcmp(szBuffer[0], "U1") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, &iData);
			Append_1Byte_Type_With_Data(INT1TYPE, 1, (unsigned char)iData, pStr);
		}
		else if (strcmp(szBuffer[0], "U2") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, &iData);
			AppendShortToByte_With_Flag(false, (unsigned short)iData, pStr);
		}
		else if (strcmp(szBuffer[0], "U4") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, &iData);
			AppendInt32ToByte((unsigned int) iData, pStr);
		}
		else if (strcmp(szBuffer[0], "U8") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, szBuffer[1], 64);
			try
			{
				AppendInt64ToByte((UINT64) _atoi64(szBuffer[1]), pStr);
			}
			catch (CException* )
			{
				return false;
			}

		}
		else {
			return false;
		}
	}
	else if (*pszMainData == 'F') {
		memset(szBuffer[0], 0, sizeof(char) * 64);
		memset(szBuffer[1], 0, sizeof(char) * 64);
		m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 1, szBuffer[0], 63);

		if (strcmp(szBuffer[0], "F4") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, &dbdata);
			AppendFloatToByte((float)dbdata, pStr);
		}
		else if (strcmp(szBuffer[0], "F8") == 0) {
			m_pStr->Str_Extract_With_Seperator(pszMainData, ':', 2, &dbdata);
			AppendDoubleToByte(dbdata, pStr);
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}

	return true;
}


bool kaSecsEncoder::Make_Packet(short sdevid, kaCSecsResponseData* pItem, kaMemStr* pStr)
{
	if (pStr == NULL) {
		return false;
	}

	unsigned int li;
	long lCnt = 0;
	bool bToHost = pItem->Get_To_Host();
	bool bWait = pItem->Get_Wait_Response();
	bool bEnd = pItem->Get_End_Flag();

	unsigned char ucStream = pItem->Get_StreamNo();
	unsigned char ucFunction = pItem->Get_FunctionNo();

	long lSytstemBytes = 0;

	lSytstemBytes = Get_SystemBytes();

	pStr->ClearString(true);
	pStr->AppendChar(0);
	Append_Secs_Header(bToHost, sdevid, bWait, ucStream, ucFunction, bEnd, 1, lSytstemBytes, pStr);
	//if (Make_Packet_Contents(pItem->Get_Resp_Data(), pStr) == false) {
	//	return false;
	//}

	unsigned char szBuf[MAX_STRING_SIZE] = { 0, };
	//memset(szBuf, 0, sizeof(char) * MAX_STRING_SIZE);
	char* pszStr = pItem->Get_Resp_Data();

	for (li = 0, lCnt = 0; li < strlen(pszStr); li++) {
		if ((pszStr[li] == '<') || (pszStr[li] == '>')){
			if (strlen((char *) szBuf) > 0) {
				szBuf[++lCnt] =   0x00;
				Make_Packet_Contents((char *) szBuf, pStr);

				lCnt = 0;
				memset(szBuf, 0, sizeof(unsigned char) * MAX_STRING_SIZE);
			}
		}
		else {
			*(szBuf + lCnt++) = pszStr[li];
		}

	}
	
	unsigned char ucCnt = ((unsigned char) pStr->Get_Str_Length()) - 1;
	pStr->SetByte(0, ucCnt);

	// Check Sum
	int iCheckSum = 0;
	unsigned char szShortBuf[2] = { 0, };

	if (pStr->GetByteSum(1, -1, iCheckSum) == false) {
		return false;
	}	
	m_pTool->kaSecs_2Byte_Int_To_Byte((short)iCheckSum, szShortBuf);
	pStr->AppendString(szShortBuf, 2);

	return true;
}


bool kaSecsEncoder::GetKeyValue(char* pszPrimary, char* pszRetKey)
{
	return m_pTool->kaSecs_Find_Secondary_Message(pszPrimary, pszRetKey);
}


long kaSecsEncoder::Get_SystemBytes()
{
	return m_lSystemBytes;
}

void kaSecsEncoder::Set_SystemBytes(long lbytes)
{
	m_lSystemBytes = lbytes;
}


void kaSecsEncoder::Set_DeviceID(short sid)
{
	m_sDevceID = sid;
}

short kaSecsEncoder::Get_DeviceID()
{
	return m_sDevceID;
}

