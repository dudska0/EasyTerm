#pragma once
#include <windows.h>

class CParser
{
	private :
		char	m_szFileName[MAX_PATH];
	
	public :
		virtual bool Parse(char* szData) = 0;


};

