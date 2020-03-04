#pragma once
#include <afxwin.h>
class CTermListBox : public CListBox
{
public:
	CTermListBox();
	~CTermListBox();
	bool SaveListboxItem(CString str, bool bAppendMode);
};

