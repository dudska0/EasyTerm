#pragma once

#include "CResponseItem.h"

class CSecsResponseData : public CResponseItem
{
	private:
		char  m_szRespData[255];
		bool  m_bWaitResponse;
		bool  m_bEndPacket;
		bool  m_bToHost;
		int	  m_iAuxData;

	public :
		CSecsResponseData();
		~CSecsResponseData();
		void Set_Resp_Data(char* szRespData);
		void Set_Resp_Data(int iAuxData, int iRespMode, char* szRespData);
		char* Get_Resp_Data(void *pParam);

		void Set_Wait_Response(bool bsts);
		bool Get_Wait_Response();

		void Set_End_Flag(bool bFlag);
		bool Get_End_Flag();

		void Set_To_Host(bool bFlag);
		bool Get_To_Host();

		int Get_Aux_Data();
		void Set_Aux_Data(int iData);

		bool Parse(char* pszData);
};

