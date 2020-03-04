#pragma once
class CResponseItem
{
	private:
		char	m_szKeyVal[50];
		bool	m_bUse;

		int		m_iRespMode;

	public:
		CResponseItem();
		~CResponseItem();

		char* Get_Key_Value();
		void Set_Key_Value(char* pszValue);
		
		bool Get_Use();
		void Set_Use(bool bUse);

		int Get_Resp_Mode();
		void Set_Resp_Mode(int iVal);

		virtual bool Parse(char* pszData) = 0;
};

