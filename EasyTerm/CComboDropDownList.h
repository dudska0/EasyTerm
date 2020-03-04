#pragma once
#include <afxwin.h>
class CComboDropDownList :public CComboBox
{
public:
	CComboDropDownList();
	~CComboDropDownList();
	
	void SetDropDownHeight(int itemsToShow);
	void DropDownList(CStringList* m_pStr);

	int GetSelectText(CString& sStr);
	int GetSelectIntValue();
	void SelectByStr(CString str);
};

