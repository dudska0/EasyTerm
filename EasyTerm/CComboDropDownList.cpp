#include "pch.h"
#include "CComboDropDownList.h"

CComboDropDownList::CComboDropDownList()
{
	//this->SetWindowStyle
}

CComboDropDownList::~CComboDropDownList()
{
}


void CComboDropDownList::SetDropDownHeight(int itemsToShow)
{
	//Get rectangles
	CRect rctComboBox, rctDropDown;
	//Combo rect  
	this->GetClientRect(&rctComboBox);

	//DropDownList rect  
	this->GetDroppedControlRect(&rctDropDown);   //Get Item height  int itemHeight = pMyComboBox->GetItemHeight(-1); 

	//Converts coordinates  
	this->GetParent()->ScreenToClient(&rctDropDown);

	//Set height  
	int itemHeight = this->GetItemHeight(-1); //Get Item height   
	rctDropDown.bottom = rctDropDown.top + rctComboBox.Height() + itemHeight * itemsToShow;

	//apply changes  
	this->MoveWindow(&rctDropDown);
}

void CComboDropDownList::DropDownList(CStringList* m_pStr)
{
	if (m_pStr == NULL) {
		return;
	}

	POSITION ps = m_pStr->GetHeadPosition();
	while (ps != NULL) {
		this->AddString(m_pStr->GetNext(ps));
	}
}

int CComboDropDownList::GetSelectText(CString& sStr)
{
	GetLBText(GetCurSel(), sStr);
	return sStr.GetLength();
}

int CComboDropDownList::GetSelectIntValue()
{
	int iResult = 0;
	CString sStr;
	GetLBText(GetCurSel(), sStr);
	try
	{
		iResult = _ttoi(sStr);
	}
	catch (CException* )
	{
		iResult = 0;
	}
	
	return iResult;
}

void CComboDropDownList::SelectByStr(CString str)
{
	char* pszStr = LPSTR(LPCTSTR(str));
	int idx = this->FindString(0, pszStr);
	SetCurSel(idx);
}
