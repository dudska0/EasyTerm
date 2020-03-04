#pragma once

#include "CResponseItem.h"
#include "KStr.h"
#include "kaMemStr.h"
#include "CSecsResponseData.h"

class kaCSecsResponseData : public CResponseItem
{
	private:
		bool  m_bWaitResponse;			//	Get_Wait_Response
		bool  m_bEndPacket;				//	Get_End_Flag
		bool  m_bToHost;				//	Get_To_Host
		int	  m_iAuxData;				//	Get_Aux_Data
		char  m_szRespData[1024];		//	Get_Resp_Data
		CKStr* m_pKStr;					
		kaMemStr* m_pMemStr;

	public :
		kaCSecsResponseData();
		~kaCSecsResponseData();
		void Set_Resp_Data(char* szRespData);
		void Set_Resp_Data(int iAuxData, int iRespMode, char* szRespData);
		char* Get_Resp_Data();

		unsigned char Get_StreamNo();			// Key value에서 구한다.
		unsigned char Get_FunctionNo();			// Key value에서 구한다.

		void Set_Wait_Response(bool bsts);
		bool Get_Wait_Response();

		void Set_End_Flag(bool bFlag);
		bool Get_End_Flag();

		void Set_To_Host(bool bFlag);
		bool Get_To_Host();

		int Get_Aux_Data();
		void Set_Aux_Data(int iData);

		virtual bool Parse(char* pszData);
		kaCSecsResponseData& operator=(const kaCSecsResponseData& pRespData);

};

