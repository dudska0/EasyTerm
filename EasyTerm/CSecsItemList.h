#pragma once
#include "HeaderDef.h"

class CSecsItemList
{
private :
	long m_lSize;
	int  m_iType;
	char m_szContents[MAX_STRING_SIZE];

public:
	CSecsItemList();
	~CSecsItemList();

	long Get_Size();
	void Set_Size(long lSize);
	int Get_Type();
	void Set_Type(int iType);
	char* Get_Contents_Str();
	void Set_Contents_Str(char* pszStr);

	void Contents_Reset();

};

