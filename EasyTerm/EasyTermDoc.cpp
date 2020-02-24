
// EasyTermDoc.cpp: CEasyTermDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EasyTerm.h"
#endif

#include "EasyTermDoc.h"
#include "MainFrm.h"
#include "EasyTermView.h"

#include "CSecsResponseData.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEasyTermDoc

IMPLEMENT_DYNCREATE(CEasyTermDoc, CDocument)

BEGIN_MESSAGE_MAP(CEasyTermDoc, CDocument)
END_MESSAGE_MAP()


// CEasyTermDoc 생성/소멸

CEasyTermDoc::CEasyTermDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_pResp = new CResponse();
}

CEasyTermDoc::~CEasyTermDoc()
{
	delete m_pResp;
	m_pResp = NULL;
}

BOOL CEasyTermDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CEasyTermDoc serialization

void CEasyTermDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CEasyTermDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CEasyTermDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CEasyTermDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CEasyTermDoc 진단

#ifdef _DEBUG
void CEasyTermDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEasyTermDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEasyTermDoc 명령
bool CEasyTermDoc::OpenDataFile(CString sFileName, CString &strError)
{
	//AfxMessageBox(sFileName);
	int	  iBufCnt = 0;
	CFile dataFile;
	char  szErrMsg[512] = { 0, };
	unsigned char  szLinebuf[512] = { 0, };

	CFileException ex;

	if (dataFile.Open(sFileName, CFile::modeRead | CFile::shareDenyWrite, &ex)) {
		DWORD dwRead = 0;

		ULONG ulSize = (ULONG) dataFile.GetLength();

		CMainFrame* pFrm = (CMainFrame *)AfxGetMainWnd();
		CEasyTermView* pView = (CEasyTermView *)pFrm->GetActiveView();

		BYTE* buf = (BYTE *)malloc(ulSize);
		if (buf == NULL) {
			strError = "Buffer memory allocate fail[162]";
			return false;
		}

		try
		{
			dwRead = dataFile.Read(buf, ulSize);
			if (dwRead > 0) {
				for (ULONG i = 0; i < ulSize; i++) {
					szLinebuf[iBufCnt++] = *(buf+i);
					if ((*(buf + i) == 0x0A) || (*(buf + i) == 0x0D) || (*(buf + i) == 0x00)) {
						if (iBufCnt > 1) {
							iBufCnt = 0;
							//pView->AddDatatoListbox(0, (char *)szLinebuf);
							memset(szLinebuf, 0, sizeof(char) * 512);
						}
					}

				}
			}

			free(buf);

			return true;
		}
		catch (CException* ex)
		{
			free(buf);
			AfxMessageBox("Error" );
		}

	}
	else {
		strError = "";
		strError.AppendFormat(_T("File Open Error : "));
		strError += sFileName;
		strError.AppendFormat(_T(" / Exception :"));
		ex.GetErrorMessage( szErrMsg, 511, 0);
		strError.AppendFormat(_T(" %s"), szErrMsg );

		//MessageBox(AfxGetMainWnd()->m_hWnd, szErrMsg, "Error", MB_ICONERROR);

		return false;
	}

	return false;
}
