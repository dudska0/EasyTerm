#pragma once
#include "CResponseItem.h"

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

		void Set_Message_Type(int itype);
		int Get_Message_Type();
		void Set_Response_Mode(int itype);
		int Get_Response_Mode();

		bool Get_Use(int idx);
		void Set_Use(int idx, bool bUse);
		bool Set_Response_Data(int idx, char* pszKeyVal, int iRespMode, char* pszSendMsg, short sdata);
		bool Append_Response_Data(char* pszKeyVal, int iRespMode, char* pszSendMsg, short sdata);
		void Delete_Response_Data(char* pszKeyVal);
		void Delete_Response_Data(int index);
		int Get_Item_Response_Mode(int idx);

		int Find_IndexbyKey(char* pszKeyVal);
		int Find_Empty_Space();
};