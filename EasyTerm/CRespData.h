#pragma once
#include "CResponseItem.h"
#include "CSecsResponseData.h"

#define	MAX_RESP_SIZE		512

class CResponse
{
	private :
		int	m_iMsgType;					// 0: Text, 1: Binary, 2:SECS
		int	m_iRespType;
			
		CResponseItem* m_pData;
	
	public :
		CResponse();
		CResponse( int iMsgType, int iRespType );
		~CResponse();

		int Get_Message_Type();
		void Set_Message_Type(int itype);			// 호출하면 모든 데이터가 지워질 수 있다.
		int Get_Response_Mode();					// 0번부터 10번까지의 Response Mode
		void Set_Response_Mode(int itype);

		bool Get_Use(int idx);
		void Set_Use(int idx, bool bUse);
		bool Set_Response_Data(char* pszData);
		bool Set_Response_Data(int idx, char* pszKeyVal, int iRespMode, char* pszSendMsg, short sdata);
		bool Append_Response_Data(char* pszKeyVal, int iRespMode, char* pszSendMsg, short sdata);
		void Delete_Response_Data(char* pszKeyVal);
		void Delete_Response_Data(int index);
		int Get_Item_Response_Mode(int idx);


		int Find_IndexbyKey(char* pszKeyVal);
		int Find_IndexbyKey(char* pszKeyVal, int iStartIdx); 

		int Find_Empty_Space();


		kaCSecsResponseData* Get_ResponseData(int idx);
		char* Get_Response_Message(int idx);
		char* Get_Key_Value(int idx);
		bool Get_Wait_Response(int idx);
		bool Get_End_Flag(int idx);
		bool Get_To_Host(int idx);
		bool Check_Response_Exist(char* pszKeyVal, int iRespMode, int iStartIdx);		// iRespMode = -1 이면 Mode Matching을 하지 않는다., Not 1이면 User 지정 모드로 동작한다.
};