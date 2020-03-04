#include "pch.h"
#include "CTermListBox.h"


CTermListBox::CTermListBox()
{

}

CTermListBox::~CTermListBox()
{

}

bool CTermListBox::SaveListboxItem(CString str, bool bAppendMode)
{
	int	 i;

	CFile cf;
	CFileException ex;
	CString csItem;
	UINT uOpenFlag = CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyNone;

	if (cf.Open(str, uOpenFlag, &ex)) {
		if (bAppendMode) {
			cf.SeekToEnd();
		}

		for (i = 0; i < this->GetCount(); i++) {
			csItem.Empty();
			GetText(i, csItem);
			csItem.AppendChar('\n');
			cf.Write(csItem, csItem.GetLength());
		}
	}
	else {
		return false;
	}

	cf.Close();

	return true;
}
