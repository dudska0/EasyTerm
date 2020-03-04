#pragma once
enum {
	TIME_HMSM_COLON, TIME_HMSM_HYPEN, TIME_HMSM_SLASH, TIME_HMSM_CONTINUE,
	TIME_HMS_COLON, TIME_HMS_HYPEN, TIME_HMS_SLASH, TIME_HMS_CONTINUE,
	TIME_HM_COLON, TIME_HM_HYPEN, TIME_HM_SLASH, TIME_HM_CONTINUE
};

enum {
	DATE_YYYYMMDD_SLASH, DATE_YYYYMMDD_HYPEN, DATE_YYYYMMDD_CONTINUE, DATE_YYYYMMDD_PERIOD,
	DATE_YYMMDD_SLASH, DATE_YYMMDD_HYPEN, DATE_YYMMDD_CONTINUE, DATE_YYMMDD_PERIOD
};


class CKStr
{
	public:
		CKStr();
		~CKStr();

		bool NumStr_Entirety_Check(char* szStr);
		bool REMOVE_STR_TAIL_BLANK(char* pszData);
		int	 Char_Find_and_Replace(char* pszData, char chFind, char chchanged);
		bool Append_Quotes_Mark(char* pszData, int iArraySize);

		bool Str_Right_Shift(char* pszData, int iArraySize);


		int Str_Extract_With_Seperator(char* pszText, char sep, int iNo, char* pszRetVal, int iMaxData);
		int Str_Extract_With_Seperator(char* pszText, char sep, int iNo, int*  piRetVal);
		int Str_Extract_With_Seperator(char* pszText, char sep, int iNo, double* pdbRetVal);

		bool Check_Valid_Number_String(char* pstr);		// ���� kaStr_to_NUM_Valid_Check
		int Check_Integer_And_Return(char* szBuffer, int iLength, BOOL *bResult);
		double Check_Float_And_Return(char* szBuffer, int iLength, BOOL *bResult);

		int Str_Has_Char(char* pszStr, char ch);																// string�� Character�� ������ �ִ��� Ȯ���Ѵ�. ���ϰ��� char�� ��ġ
		int Get_Char_Count_In_Str(char* pszStr, char ch, int iSize);											// string ���� �ش� character�� ��� �ִ����� Ȯ���Ѵ�.

		int	Get_IO_Type_From_String(char* pszStr);
		void GetTimeData(char* szData, int dMode);
		void GetDateData(char* szData, int dMode);

		int Get_Module_Number(char* pszHeader, char* pszMdlStr); 												// PM?, TM? ���� ?�ڸ��� Module ��ȣ�� �����Ѵ�.
																												// 0 : Not match Header, 1 ~ ?? PM Module�� Number�� �´� 

																												// printf( "Pos = %d\n", Get_Char_PosIndex( "f:/data/rcp/rxt.rcp:(RUN)xxxxxxxxxxx:sd", ':', 3 ) );		// 3��° ':'�� ��ġ�� �����Ѵ�.
		int Get_Char_PosIndex(char* pszStr, char cFindChar, int iNo);											// Ư�������� ��ġ�� �����Ѵ�.
		long Get_Size_By_Index(unsigned int iStart, unsigned iEnd);												// return -1�� Error
};

