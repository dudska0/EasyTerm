#include "pch.h"
#include "Comm.h"


CComm::CComm()
{
}


CComm::~CComm()
{
}

bool CComm::Get_Use()
{
	return m_bUse;
}

void CComm::Set_Use(bool bUse)
{
	m_bUse = bUse;
}


