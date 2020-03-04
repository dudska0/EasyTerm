#pragma once

#define		ENQ					0x05
#define		EOT					0x04	
#define		ACK					0x06
#define		NAK					0x15
//-----------------------------------------------------------------------------------
#define		DEVICEID			0x00
//-----------------------------------------------------------------------------------
#define		HEADERBLOCK_LENGTH	11
#define		HEADER_LENGTH		10
#define		MESSAGE_LENGTH		1
#define		SYSTEM_LENGTH		4
#define		DEVID_LENGTH		2
#define		CHECK_LENGTH		2
//-----------------------------------------------------------------------------------
#define		DATA_LENGTH			512
#define		DATA_LENGTH2		256
//----------------------------------------------------------------------------------------
#define		LISTTYPE			0x00
#define		BINARYTYPE			0x20
#define		BOOLEANTYPE			0x24
#define		ASCIITYPE			0x40
#define		JIS8TYPE			0x44
#define		INT8TYPE			0x60
#define		INT1TYPE			0x64			// 1 Byte Integer
#define		INT2TYPE			0x68
#define		INT4TYPE			0x70
#define		FLT8TYPE			0x80
#define		FLT4TYPE			0x90
#define		UINT8TYPE			0xA0
#define		UINT1TYPE			0xA4
#define		UINT2TYPE			0xA8
#define		UINT4TYPE			0xB0


typedef struct {
	unsigned char by_Stream;
	unsigned char by_Function;
} strMsgID;

typedef union {
	unsigned char ucByte[2];
	short sData;
} unShort;

typedef union {
	unsigned char by_i32Data[4];
	int	 i32Data;
} unInt32;

typedef union {
	unsigned char by_i64Data[8];
	INT64	 i64Data;
} unInt64;

typedef union {
	unsigned char by_float[4];
	float fdata;
} unFloat;

typedef union {
	unsigned char by_double[8];
	double dbdata;
} unDouble;


//typedef struct {
//	unDEV_ID		unDevID;			// 2 Byte
//	strMsgID		strMessageID;		// 2 Byte
//	unShort			unBlockNo;			// 2 Byte		// 6 Byte
//}  strHeader;
//
//typedef union {
//	BYTE	byArrPacket[6];
//	strHeader	strHd;
//} unHeaderPacket;
//
//typedef struct {
//	BYTE byTypeInfo_No;
//	int iNoOfElements;
//} strListData;
//
//typedef union {
//	BYTE by_ArPacket[5];
//	strListData strData;
//} unListPacket;

#include "CSecsItemList.h"
#include "kaMemStr.h"
#include "KStr.h"

class kaSecsTool
{
public:
	kaSecsTool();
	~kaSecsTool();
	bool ByteSwap(unsigned char* pucByte, short Size);

	int kaSecs_2Byte_Int_To_Byte(short sNum, unsigned char* pBuf);
	int kaSecs_ByteInt32_To_Byte(INT32 i32Num, unsigned char* pBuf);
	int kaSecs_ByteInt64_To_Byte(INT64 i64Num, unsigned char* pBuf);

	int kaSecs_Float_To_Byte(float fnum, unsigned char* pBuf);
	int kaSecs_Double_To_Byte(double dbnum, unsigned char* pBuf);

	short kaSecs_2Byte_To_2ByteInt(unsigned char* pBuf);				// 2Byte -> Short
	long  kaSecs_4Byte_To_ByteInt32(unsigned char* pBuf);				// 4Byte -> Long
	INT64 kaSecs_8Byte_To_ByteInt64(unsigned char* pBuf);				// 8yte -> Long

	float  kaSecs_4Byte_To_ByteFloat(unsigned char* pBuf);				// 2Byte -> Long
	double kaSecs_8Byte_To_ByteDouble(unsigned char* pBuf);				// 2Byte -> Long

	bool kaSecs_Find_Secondary_Message(char* pszPrimaryMsg, char* pszSecondaryMsg);
};

class kaSecsEncoder
{
private:

protected:
	kaSecsTool*	m_pTool;
	short m_sDevceID;
	long m_lSystemBytes;
	CKStr* m_pStr;
	//CSecsItemList* m_pList;

public:
	kaSecsEncoder();
	~kaSecsEncoder();

	int Get_SizeDefine_LineNo(int iElementsNo, int iType, kaMemStr* pStr_ByteLength);

	void AppendShortToByte_With_Flag(bool bFlag, short sData, unsigned char* pBuf);
	void AppendShortToByte_With_Flag(bool bFlag, short sData, kaMemStr* pRetStr);
	void AppendShortToByte_With_Flag(bool bFlag, unsigned char ucData1, unsigned char ucData2, unsigned char* pBuf);
	void AppendShortToByte_With_Flag(bool bFlag, unsigned char ucData1, unsigned char ucData2, kaMemStr* pRetStr);
	void AppendInt32ToByte(long i32_Data, unsigned char* pBuf);
	void AppendInt32ToByte(long i32_Data, kaMemStr* pRetStr);
	void AppendInt64ToByte(INT64 i32_Data, unsigned char* pBuf);
	void AppendInt64ToByte(INT64 i32_Data, kaMemStr* pRetStr);
	void AppendFloatToByte(float fdata, unsigned char* pBuf);
	void AppendFloatToByte(float fdata, kaMemStr* pBuf);
	void AppendDoubleToByte(double dbdata, unsigned char* pBuf);
	void AppendDoubleToByte(double dbdata, kaMemStr* pBuf);

	void AppendDataHeader(int iType, long iNoOfElements, kaMemStr* pRetStr);
	void AppendLengthLine_Data(int iType, int iNoOfElements, long lDefLine, kaMemStr* pRetStr);

	void Append_ListData_Type(long iNoOfElements, kaMemStr* pRetStr);
	void Append_1Byte_Type_With_Data(int iType, long iNoOfElements, unsigned char ucData, kaMemStr* pRetStr);
	void Append_nByteStr_Type_With_Data(long iNoOfElements, char* pucArrData, kaMemStr* pRetStr);

	bool Append_Secs_Header(bool bToHost, short sDevID, bool bWait, unsigned char ucStream, unsigned char ucFunction, bool bEnd, short sBlock, long iSystemByte, kaMemStr* pRetStr);

	bool Make_Packet(short sdevid, kaCSecsResponseData* pItem, kaMemStr* pStr);
	bool Make_Packet_Contents(char* pszMainData, kaMemStr* pStr);

	int Get_MessageType_From_String(char* pszStr, int& iSize, long& lOffset_Contents_Start, long& lContents_Size);

	bool GetKeyValue(char* pszPrimary, char* pszRetKey);

	void Set_DeviceID(short sid);
	short Get_DeviceID();

	long Get_SystemBytes();
	void Set_SystemBytes(long lbytes);

};

