
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
#include "AsciiTable.h"

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
	m_pDecoder = new kaSecsDecoder();
	m_pConfig = new CCommConfig();
}

CEasyTermDoc::~CEasyTermDoc()
{
	delete m_pResp;
	m_pResp = NULL;

	delete m_pDecoder;
	m_pDecoder = NULL;

	delete m_pConfig;
	m_pConfig = NULL;
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
	unsigned char  szLinebuf[1024] = { 0, };

	CFileException ex;

	if (dataFile.Open(sFileName, CFile::modeRead | CFile::shareDenyWrite, &ex)) {
		DWORD dwRead = 0;

		ULONG ulSize = (ULONG) dataFile.GetLength();

		CMainFrame* pFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
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
				// Data Clear
				for (int i = 0; i < MAX_RESP_SIZE; i++) {
					m_pResp->Delete_Response_Data(i);
				}

				for (ULONG i = 0; i < ulSize; i++) {
					
					if ((*(buf + i) == 0x0A) || (*(buf + i) == 0x0D) || (*(buf + i) == 0x00)) {
						szLinebuf[iBufCnt++] = 0x00;
						if (iBufCnt > 1) {
							//pView->AddDatatoListbox(0, (char *)szLinebuf);
							if (strlen((char *) szLinebuf) == 0) continue;
							if (m_pResp->Set_Response_Data((char *) szLinebuf) == false) {
								return false;
							}
							memset(szLinebuf, 0, sizeof(char) * 512);
						}
						iBufCnt = 0;
					}
					else {
						szLinebuf[iBufCnt++] = *(buf + i);
						if (i == ( ulSize - 1)){
							szLinebuf[iBufCnt++] = 0x00;
							if (m_pResp->Set_Response_Data((char *)szLinebuf) == false) {
								return false;
							}
							memset(szLinebuf, 0, sizeof(char) * 512);
						}
					}

				}
			}

			free(buf);

			return true;
		}
		catch (CException* )
		{
			free(buf);
		}

	}
	else {
		strError = "";
		strError.AppendFormat(_T("File Open Error : "));
		strError += sFileName;
		strError.AppendFormat(_T(" / Exception :"));
		strError.AppendFormat(_T(" %s"), szErrMsg );

		//MessageBox(AfxGetMainWnd()->m_hWnd, szErrMsg, "Error", MB_ICONERROR);

		return false;
	}

	return false;
}

int CEasyTermDoc::Decode_Stream(unsigned char* pucStream, CString& sSxFy, CString& sMsg)
{
	m_pDecoder->ReadStream(pucStream);
	sMsg = m_pDecoder->Get_Secs_Message();
	m_pDecoder->Get_StreamFunction(sSxFy);

	return 0;
}

void CEasyTermDoc::Set_Comm_Config(CCommConfig* pConfig)
{
	if (m_pConfig != NULL) {
		memcpy( m_pConfig, pConfig, sizeof(CCommConfig));
	}
}

void CEasyTermDoc::Set_Data(CCommConfig* pConfig)
{
	if (m_pConfig != NULL) {
		memcpy( pConfig, m_pConfig, sizeof(CCommConfig));
	}
}

bool CEasyTermDoc::Make_Response(char* szKeyVal, int iMode )
{
	int idx = -1;
	CMainFrame* pFrame = (CMainFrame *) AfxGetApp()->GetMainWnd();
	CEasyTermView* pView = (CEasyTermView *)(pFrame->GetActiveView());

	// Secs Section
	CSecsSerial* pComm = (CSecsSerial *)(pFrame->GetCommObject());
	m_pResp->Set_Response_Mode(1);
	idx = m_pResp->Find_IndexbyKey(szKeyVal);
	if (idx == -1) {
		return false;
	}
	
	kaCSecsResponseData* pData =  m_pResp->Get_ResponseData(idx);
	//if (pComm->Send_Response(pData) == false) {
	//	return false;
	//}

	CString str_buf;
	kaMemStr* pStr = NULL;
	pStr = pComm->GetMemStr();
	pStr->ClearString(true);


	long lsystembytes = m_pDecoder->Get_SystemBytes();
	pComm->GetEncoder()->Set_SystemBytes(lsystembytes);

	// Encoder data
	if (pComm->GetEncoder()->Make_Packet(m_pDecoder->Get_DeviceID(), pData, pStr) == false) {
		pComm->Set_CommStep(SECSCOMM::WAIT_ENQ_0);
		return false;
	}

	// Send ENQ
	if (pComm->WriteChar(CODE_ASCII_ENQ) == false) {
		pComm->Set_CommStep(SECSCOMM::WAIT_ENQ_0);
		return false;
	}

	str_buf.AppendFormat(">> Send ENQ ");
	pView->StrMsg_AddString(str_buf);
	str_buf.Empty();
	pComm->Set_CommStep(SECSCOMM::WAIT_EOT_3);

	// Receive EOT
	if (pComm->ReadWaitChar(CODE_ASCII_EOT, 10) == false) {
		pComm->Set_CommStep(SECSCOMM::WAIT_ENQ_0);
		return false;
	}
	str_buf.AppendFormat("Recv EOT <<");
	pView->StrMsg_AddString(str_buf);
	str_buf.Empty();
	pComm->Set_CommStep(SECSCOMM::SEND_MAINDATA_4);

	

	// Send Response
	if (pComm->WriteSecsData(pStr->Get_Str_Length(), pStr->GetString()) == false) {
		pComm->Set_CommStep(SECSCOMM::WAIT_ENQ_0);
		return false;
	}

	CString csKeyVal;
	CString csDeocdeMsg;

	csKeyVal.Empty();
	csDeocdeMsg.Empty();
	
	unsigned char* pchStr = pComm->Get_Buffer();
	
	Decode_Stream(pchStr,csKeyVal, csDeocdeMsg);
	pView->SecsMsg_AddString(szKeyVal, pData->Get_To_Host(), pData->Get_Wait_Response(), pData->Get_End_Flag(), csDeocdeMsg, pComm->GetMemStr());


	// Receive ACK
	pComm->Set_CommStep(SECSCOMM::WAIT_ACK_7);
	if (pComm->ReadWaitChar(CODE_ASCII_ACK, 10) == false) {
		pComm->Set_CommStep(SECSCOMM::WAIT_ENQ_0);
		return false;
	}
	str_buf.AppendFormat("Recv ACK <<");
	pView->StrMsg_AddString(str_buf);
	str_buf.Empty();


	pComm->Set_CommStep(SECSCOMM::WAIT_ENQ_0);

	
	return true;
}
