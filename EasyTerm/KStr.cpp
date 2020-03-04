#include <windows.h>

#include "pch.h"
#include "KStr.h"


CKStr::CKStr()
{
}


CKStr::~CKStr()
{
}

bool CKStr::NumStr_Entirety_Check(char* szStr)
{
	int		i;
	int		dLength = 0;

	REMOVE_STR_TAIL_BLANK(szStr);
	dLength = strlen(szStr);

	for (i = 0; i < dLength; i++) {
		if ((szStr[i] < '0') || (szStr[i] > '9')) {
			if (szStr[i] == '.')		continue;
			else if (szStr[0] == '-')		continue;
			else if (szStr[0] == '+')		continue;
			else {
				//m_plog->LogWrite( FOREGROUND_RED, TRUE, __LINE__, "-", "wrong character [%d] ==> \"%c\"\n", i, szStr[i] );
				return false;
			}
		}
	}

	return true;
}

bool CKStr::REMOVE_STR_TAIL_BLANK(char* pszData)
{
	char	szTemp[255];

	int		i;
	int		dLength = 0;

	memset(szTemp, 0, sizeof(szTemp));
	dLength = strlen(pszData) - 1;

	if ((pszData[dLength] >= 33 /* White Space Low Limit*/) &&
		(pszData[dLength] <= 126 /* White Space Low Limit*/))	return true;

	for (i = dLength; i >= 0; i--)
	{
		if ((pszData[i] >= 33 /* White Space Low Limit*/) && (pszData[i] <= 126 /* White Space Low Limit*/))	break;
	}

	if (i <= 0)		return true;
	else
	{
		strncpy(szTemp, pszData, i + 1);
		strcpy(pszData, szTemp);

		return true;
	}
}

int CKStr::Str_Extract_With_Seperator(char* pszText, char sep, int iNo, char* pszRetVal, int iMaxData)
{
	char	sepstr[8] = { 0, };
	char	szStr[2048] = { 0, };
	int		iCnt = 0, iStart = -1, iEnd = -1;
	unsigned int i;

	if (pszText == NULL)  return 0;
	if (iMaxData >= 2048)	return 0;

	memset(pszRetVal, 0, sizeof(char) * iMaxData);

	if (iNo == 1)			iStart = 0;

	for (i = 0; i < strlen(pszText); i++) {
		if ((pszText[i] == sep) || (pszText[i] == 0)) {
			iCnt++;
			if (iNo > 1) {
				if (iCnt == iNo - 1)		iStart = i + 1;
			}

			if (iNo == iCnt) {
				iEnd = i;
				break;
			}
		}
	}

	if (iStart >= 0) {
		if (iEnd == -1)	iEnd = strlen(pszText);

		if (iEnd > iStart)	strncpy(pszRetVal, &pszText[iStart], iEnd - iStart);
		else					strcpy(pszRetVal, "");
	}
	else {
		strcpy(pszRetVal, "");
	}
	return strlen(pszRetVal);
}

int CKStr::Str_Extract_With_Seperator(char* pszText, char sep, int iNo, int*  piRetVal)
{
	char	szRetVal[64] = { 0, };
	int		iResult = 0;
	BOOL	bResult = FALSE;

	iResult = Str_Extract_With_Seperator(pszText, sep, iNo, szRetVal, 63);

	if (iResult <= 0) {
		if (piRetVal != NULL)		*piRetVal = 0;
		return 0;
	}

	iResult = Check_Integer_And_Return(szRetVal, 63, &bResult);

	if (piRetVal != NULL)	*piRetVal = iResult;

	if (bResult == FALSE) {
		return 0;
	}

	return 1;
}

int CKStr::Str_Extract_With_Seperator(char* pszText, char sep, int iNo, double* pdbRetVal)
{
	int		iResult = 0;
	char	szRetVal[64] = { 0, };
	BOOL	bResult = FALSE;
	double	dbResult = 0;

	iResult = Str_Extract_With_Seperator(pszText, sep, iNo, szRetVal, 63);
	if (iResult <= 0) {
		if (pdbRetVal != NULL)		*pdbRetVal = 0;
		return 0;
	}

	dbResult = Check_Float_And_Return(szRetVal, 63, &bResult);

	if (pdbRetVal != NULL)	*pdbRetVal = dbResult;

	if (bResult == FALSE) {
		return 0;
	}

	return 1;
}

bool CKStr::Check_Valid_Number_String(char* pstr)
{
	int		i, iLength = strlen(pstr);

	if (iLength == 0) return false;

	for (i = 0; i < iLength; i++) {
		if ((pstr[i] < '0') || (pstr[i] > '9'))	return false;
	}

	return true;
}

int CKStr::Check_Integer_And_Return(char* szBuffer, int iLength, BOOL *bResult)
{
	int		i;
	BOOL	bNumStart = FALSE;
	BOOL	bSpaceFind = FALSE;

	// 유효성 검사
	for (i = 0; (i < iLength) && (szBuffer[i] != 0); i++) {
		if (szBuffer[i] == ' ') {
			if (!bNumStart) continue;
			else bSpaceFind = TRUE;
		}
		else if ((szBuffer[i] >= '0') && (szBuffer[i] <= '9')) {
			if (bSpaceFind) {		// 숫자가 시작한 후, 공백뒤에 숫자가 나오면 Error
				*bResult = FALSE;
				//LogWrite( FOREGROUND_RED, TRUE, __LINE__, "CHECK_INT", "string has space characeter [%s][cnt=%d,ch='%c']\n", szBuffer, i, szBuffer[i] );
				return 0;
			}

			if (!bNumStart) bNumStart = TRUE;
		}
		else if ((szBuffer[i] == '+') || (szBuffer[i] == '-') || (szBuffer[i] == 'E') || (szBuffer[i] == '.')) {

		}
		else {						// 숫자외에 다른 문자가 섞여 있음
			*bResult = FALSE;
			//LogWrite( FOREGROUND_RED, TRUE, __LINE__, "CHECK_INT", "string has invalid characeter [%s][cnt=%d,ch='%c']\n", szBuffer, i, szBuffer[i] );
			return 0;
		}
	}

	*bResult = TRUE;

	return atoi(szBuffer);
}

double CKStr::Check_Float_And_Return(char* szBuffer, int iLength, BOOL *bResult)
{
	int		i;
	BOOL	bNumStart = FALSE;
	BOOL	bSpaceFind = FALSE;

	// 유효성 검사
	for (i = 0; (i < iLength) && (szBuffer[i] != 0); i++) {
		if (szBuffer[i] == ' ') {
			if (!bNumStart) continue;
			else bSpaceFind = TRUE;
		}
		else if (((szBuffer[i] >= '0') && (szBuffer[i] <= '9')) || (szBuffer[i] == '.')) {
			if (bSpaceFind) {		// 숫자가 시작한 후, 공백뒤에 숫자가 나오면 Error
				*bResult = FALSE;
				//LogWrite( FOREGROUND_RED, TRUE, __LINE__, "CHECK_FLOAT", "string has space characeter [%s][cnt=%d,ch='%c']\n", szBuffer, i, szBuffer[i] );
				return 0;
			}

			if (!bNumStart) bNumStart = TRUE;
		}
		else if ((szBuffer[i] == '+') || (szBuffer[i] == '-') || (szBuffer[i] == 'E') || (szBuffer[i] == '.')) {

		}
		else {						// 숫자외에 다른 문자가 섞여 있음
			*bResult = FALSE;
			//LogWrite( FOREGROUND_RED, TRUE, __LINE__, "CHECK_FLOAT", "string has invalid characeter [%s][cnt=%d,ch='%c']\n", szBuffer, i, szBuffer[i] );
			return 0;
		}
	}

	*bResult = TRUE;

	return atof(szBuffer);
}

int CKStr::Str_Has_Char(char* pszStr, char ch)
{
	int	i;
	unsigned int ilength = 0;

	if (pszStr == NULL)			return -1;

	ilength = strlen(pszStr);

	for (i = 0; i < (int)ilength; i++) {
		if (pszStr[i] == ch)		return i;
	}

	return -1;
}

int	CKStr::Get_IO_Type_From_String(char* pszStr)
{
	if (strcmp(pszStr, "DIGITAL") == 0)		return 0;
	else if (strcmp(pszStr, "ANALOG") == 0)		return 1;
	else if (strcmp(pszStr, "STRING") == 0)		return 2;
	else if (strcmp(pszStr, "FUNCTION") == 0)		return 3;

	else if (strcmp(pszStr, "_K_D_IO") == 0)		return 0;
	else if (strcmp(pszStr, "_K_A_IO") == 0)		return 1;
	else if (strcmp(pszStr, "_K_S_IO") == 0)		return 2;
	else if (strcmp(pszStr, "_K_F_IO") == 0)		return 3;

	else if (strcmp(pszStr, "D") == 0)		return 0;
	else if (strcmp(pszStr, "A") == 0)		return 1;
	else if (strcmp(pszStr, "S") == 0)		return 2;
	else if (strcmp(pszStr, "F") == 0)		return 3;

	else if (strcmp(pszStr, "digital") == 0)		return 0;
	else if (strcmp(pszStr, "analog") == 0)		return 1;
	else if (strcmp(pszStr, "string") == 0)		return 2;
	else if (strcmp(pszStr, "function") == 0)		return 3;

	else if (strcmp(pszStr, "_k_d_io") == 0)		return 0;
	else if (strcmp(pszStr, "_k_a_io") == 0)		return 1;
	else if (strcmp(pszStr, "_k_s_io") == 0)		return 2;
	else if (strcmp(pszStr, "_k_f_io") == 0)		return 3;

	else if (strcmp(pszStr, "d") == 0)		return 0;
	else if (strcmp(pszStr, "a") == 0)		return 1;
	else if (strcmp(pszStr, "s") == 0)		return 2;
	else if (strcmp(pszStr, "f") == 0)		return 3;

	return -1;
}


int	CKStr::Char_Find_and_Replace(char* pszData, char chFind, char chchanged)
{
	int iChange_Count = 0;
	unsigned int i, iLength = 0;

	iLength = strlen(pszData);

	for (i = 0; i < iLength; i++) {
		if (pszData[i] == chFind) {
			pszData[i] = chchanged;
			iChange_Count++;
		}
	}

	return iChange_Count;
}


bool CKStr::Str_Right_Shift(char* pszData, int iArraySize)
{
	unsigned int		ui, uiLength = 0;
	if ((strlen(pszData) + 1) >= (unsigned int)iArraySize)	return false;

	uiLength = strlen(pszData);
	pszData[uiLength + 1] = 0;
	for (ui = 0; ui < uiLength; ui++) {
		pszData[uiLength - ui] = pszData[uiLength - (ui + 1)];
	}

	return true;
}

bool CKStr::Append_Quotes_Mark(char* pszData, int iArraySize)
{
	if (Str_Has_Char(pszData, '"') >= 0)			return false;
	if ((strlen(pszData) + 3) > (unsigned int)iArraySize)	return false;

	strcat(pszData, "\"");						// 뒤쪽의 Quotate Mark 추가
	if (Str_Right_Shift(pszData, iArraySize) == false)	return false;
	pszData[0] = '"';

	return true;
}

int CKStr::Get_Char_Count_In_Str(char* pszStr, char ch, int iSize)
{
	int		iCount = 0;

	if (Str_Has_Char(pszStr, ch) < 0)			return 0;

	for (int i = 0; ((pszStr != 0) && (i < iSize)); i++) {			// string 끝까지
		if (pszStr[i] == ch)		iCount++;
	}

	return iCount;
}


void CKStr::GetTimeData(char* szData, int dMode)
{
	SYSTEMTIME	st;

	GetLocalTime(&st);

	switch (dMode)
	{
	case TIME_HMS_COLON:	 _snprintf(szData, 9, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);	break;
	case TIME_HMS_HYPEN:	 _snprintf(szData, 9, "%02d-%02d-%02d", st.wHour, st.wMinute, st.wSecond);	break;
	case TIME_HMS_SLASH:	 _snprintf(szData, 9, "%02d/%02d/%02d", st.wHour, st.wMinute, st.wSecond);	break;
	case TIME_HMS_CONTINUE:	 _snprintf(szData, 7, "%02d%02d%02d", st.wHour, st.wMinute, st.wSecond);	break;

	case TIME_HMSM_COLON:	 _snprintf(szData, 13,"%02d:%02d:%02d.%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);	break;
	case TIME_HMSM_HYPEN:	 _snprintf(szData, 13, "%02d-%02d-%02d-%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);	break;
	case TIME_HMSM_SLASH:	 _snprintf(szData, 13, "%02d/%02d/%02d/%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);	break;
	case TIME_HMSM_CONTINUE: _snprintf(szData, 10, "%02d%02d%02d%03d", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);	break;

	case TIME_HM_COLON:  	 _snprintf(szData, 6, "%02d:%02d", st.wHour, st.wMinute);	break;
	case TIME_HM_HYPEN:  	 _snprintf(szData, 6, "%02d-%02d", st.wHour, st.wMinute);	break;
	case TIME_HM_SLASH:  	 _snprintf(szData, 6, "%02d/%02d", st.wHour, st.wMinute);	break;
	case TIME_HM_CONTINUE:	 _snprintf(szData, 6, "%02d%02d", st.wHour, st.wMinute);	break;
	default:			 	 _snprintf(szData, 9, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	}
}

void CKStr::GetDateData(char* szData, int dMode)
{
	char		szYearData[5];
	char		szYearData2[3];

	SYSTEMTIME	st;

	GetLocalTime(&st);
	sprintf(szYearData, "%04d", st.wYear);
	strcpy(szYearData2, &szYearData[2]);

	switch (dMode)
	{
	case DATE_YYYYMMDD_SLASH:		_snprintf(szData, 11, "%s/%02d/%02d", szYearData, st.wMonth, st.wDay); 		break;
	case DATE_YYYYMMDD_HYPEN:		_snprintf(szData, 11, "%s-%02d-%02d", szYearData, st.wMonth, st.wDay); 		break;
	case DATE_YYYYMMDD_CONTINUE:	_snprintf(szData, 11, "%s%02d%02d", szYearData, st.wMonth, st.wDay); 		break;
	case DATE_YYYYMMDD_PERIOD:		_snprintf(szData, 11, "%s.%02d.%02d", szYearData, st.wMonth, st.wDay); 		break;

	case DATE_YYMMDD_SLASH:			_snprintf(szData, 9, "%s/%02d/%02d", szYearData2, st.wMonth, st.wDay); 	break;
	case DATE_YYMMDD_HYPEN:			_snprintf(szData, 9, "%s-%02d-%02d", szYearData2, st.wMonth, st.wDay); 	break;
	case DATE_YYMMDD_CONTINUE:		_snprintf(szData, 9, "%s%02d%02d", szYearData2, st.wMonth, st.wDay); 	break;
	case DATE_YYMMDD_PERIOD:		_snprintf(szData, 9, "%s.%02d.%02d", szYearData2, st.wMonth, st.wDay); 	break;

	default:						_snprintf(szData, 11, "%s-%02d-%02d", szYearData, st.wMonth, st.wDay); 		break;
	}
}

int CKStr::Get_Module_Number(char* pszHeader, char* pszMdlStr) 			// 0 : Not match Header, 1 ~ ?? PM Module의 Number가 온다 
{
	int ino = -1;
	int iHeaderSize = strlen(pszHeader);

	if (strncmp(pszHeader, pszMdlStr, iHeaderSize) != 0)		return 0;

	ino = (int)(strtol(pszMdlStr + iHeaderSize, NULL, 10));
	if (ino == 0)		ino = -1;

	return ino;
}

int CKStr::Get_Char_PosIndex(char* pszStr, char cFindChar, int iNo)
{
	int	 iPos = 0;
	char *ptr = NULL;
	static int idepth;

	ptr = strchr(pszStr, cFindChar);
	if (ptr == NULL) 		return -1;
	if (*(ptr + 1) == 0)	return -1;

	if (iNo == 1) {
		return (int)(ptr-pszStr);
	}
	else {
		if ((iNo - 1) == idepth) {
			return (int)ptr;
		}
		else {
			idepth++;
			iPos = Get_Char_PosIndex(ptr + 1, cFindChar, iNo);
			idepth--;

			if (idepth == 0) {
				if (iPos == -1)	return -1;
				else {
					return iPos - (int)pszStr;
				}
			}
			else {
				return iPos;
			}
		}
	}

	return -1;
}


long CKStr::Get_Size_By_Index(unsigned int iStart, unsigned int iEnd)
{
	if (iStart > iEnd) {
		return -1;
	}

	return (iEnd - iStart) + 1;      // S13F23   - Stream만 추출할 때 Size 계산 13의 1(iStart)은 index1, 2(iEnd)은 index 2 
	                                 // ---> Size는 iEnd - iStart + 1
}