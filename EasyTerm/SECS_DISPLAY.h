#pragma once

typedef void(*OutPut_Func)(CString);

class CSECS_DISPLAY
{
	private:
		int  m_iDPOffset;
		OutPut_Func	m_pOutFunc;

	public:
		CSECS_DISPLAY(OutPut_Func pFunc);
		~CSECS_DISPLAY();

		void Set_DP_Offset(int iOffset);
		int  Get_DP_Offset();

		void Make_String_With_Space(int ispace, CString strx);
		void SECS_Message_Output(CString sSxFx, bool bWait, bool bEnd, bool bToHost, CString sMsg);
		void BIN_Data_Output(unsigned int uiSize, unsigned char* pucStr);
		void SkipLine();
		void String_Message_Ouput(CString str);
};

