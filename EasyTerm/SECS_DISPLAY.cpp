#include "pch.h"
#include "SECS_DISPLAY.h"

CSECS_DISPLAY::CSECS_DISPLAY(OutPut_Func pFunc)
{
	m_pOutFunc = pFunc;
	m_iDPOffset = 18;
}

CSECS_DISPLAY::~CSECS_DISPLAY()
{
}

void CSECS_DISPLAY::Set_DP_Offset(int iOffset)
{
	m_iDPOffset = iOffset;
}

int  CSECS_DISPLAY::Get_DP_Offset()
{
	return m_iDPOffset;
}

void CSECS_DISPLAY::Make_String_With_Space(int ispace, CString strx)
{
	char szFormat[20] = { 0, };
	CString str;

	str.Empty();
	_snprintf(szFormat, 19, "%%%ds", ispace);
	str.Format(szFormat, " ");
	str += strx;

	if (m_pOutFunc != NULL) {
		(*m_pOutFunc)(str);
	}
}

void CSECS_DISPLAY::SECS_Message_Output(CString sSxFx, bool bWait, bool bEnd, bool bToHost, CString sMsg)
{
	int		iDepthCnt = 0;
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString	str;

	// Make Time Data
	str.AppendFormat(_T("%02d:%02d:%02d.%03d    "), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	// Make Header Data
	str += sSxFx;
	if (bWait) {
		str += ",W";
	}

	if (bEnd) {
		str += ",E";
	}

	if (bToHost) {
		str += "[EQ->HOST]";
	}
	else {
		str += "[HOST->EQ]";
	}

	Make_String_With_Space(0, str);

	char ch;
	int  icnt = 0;
	int  iBufCnt = 0;
	bool bOutputSkip = true;
	char szLineBuf[512] = { 0, };
	CString sMsg_Out;

	iDepthCnt = -1;
	sMsg_Out.Empty();

	do {
		ch = sMsg[icnt++];
		if (ch == '<') {
			if (bOutputSkip) {
				bOutputSkip = false;
			}
			else {
				Make_String_With_Space((iDepthCnt * 4) + m_iDPOffset, sMsg_Out);
				sMsg_Out.Empty();
				iBufCnt = 0;
			}
			++iDepthCnt;
		}
		else if (ch == 0x00) {
			break;
		}
		else if (ch == '>') {
			szLineBuf[iBufCnt++] = ch;
			sMsg_Out.AppendChar(ch);
			if (iDepthCnt == 0) {
				sMsg_Out.AppendChar('.');
				Make_String_With_Space(((iDepthCnt) * 4) + m_iDPOffset, sMsg_Out);
				//sMsg_Out.Empty();
				//Make_String_With_Space(((iDepthCnt) * 4) + m_iDPOffset, sMsg_Out);
			}
			else {
				Make_String_With_Space(((iDepthCnt) * 4) + m_iDPOffset, sMsg_Out);
			}
			iDepthCnt--;

			sMsg_Out.Empty();

			iBufCnt = 0;
			bOutputSkip = true;
			continue;
		}


		sMsg_Out.AppendChar(ch);
	} while (true);

}


void CSECS_DISPLAY::BIN_Data_Output(unsigned int uiSize, unsigned char* pucStr)
{
	unsigned int	ui;
	CString	str;
	
	str.Empty();

	for (ui = 0; ui < uiSize; ui++) {
		if (ui > 0) {
			str.AppendFormat(",%d:0x%02x", ui, pucStr[ui]);
		}
		else {
			str.AppendFormat("%d:0x%02x", ui, pucStr[ui]);
		}
	}

	Make_String_With_Space(m_iDPOffset, str);
}


void CSECS_DISPLAY::SkipLine()
{
	CString str;

	str.Empty();
	Make_String_With_Space(m_iDPOffset, str);
}


void CSECS_DISPLAY::String_Message_Ouput(CString str)
{
	CString	strx;
	SYSTEMTIME st;
	GetLocalTime(&st);

	// Make Time Data
	strx.Empty();
	strx.AppendFormat(_T("%02d:%02d:%02d.%03d    "), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	strx += str;

	Make_String_With_Space(0, strx);
}
