
// EasyTermView.cpp: CEasyTermView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EasyTerm.h"
#endif

#include "MainFrm.h"

#include "EasyTermDoc.h"
#include "EasyTermView.h"

#include "CSecsResponseData.h"
#include "SECS_DISPLAY.h"
#include "CommSetupDlg.h"

#include "Comm.h"
#include "CSerialPort.h"
#include "SecsSerial.h"
#include "HeaderDef.h"

#include "kaSecsEncoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEasyTermView

IMPLEMENT_DYNCREATE(CEasyTermView, CFormView)

BEGIN_MESSAGE_MAP(CEasyTermView, CFormView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEasyTermView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CEasyTermView::OnBnClickedButton1)
	ON_COMMAND(ID_COMM_SETUP, &CEasyTermView::OnCommSetup)
	ON_COMMAND(ID_COMM_PORTOPEN, &CEasyTermView::OnCommPortopen)
	ON_COMMAND(ID_COMM_PORTCLOSE, &CEasyTermView::OnCommPortclose)
	ON_BN_CLICKED(IDC_BUT_LOGSAVE, &CEasyTermView::OnBnClickedButLogsave)
END_MESSAGE_MAP()

// CEasyTermView 생성/소멸

CEasyTermView::CEasyTermView() noexcept
	: CFormView(IDD_EASYTERM_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pSECSDP = new CSECS_DISPLAY(this->MessageDisplay);
	m_pSECSDP->Set_DP_Offset(23);

	//m_pCSp = new CSerialPort();
	//m_pCSp = (CComm *) new CSecsSerial();			// 여기서 생성한다.
}

CEasyTermView::~CEasyTermView()
{
	if (m_pSECSDP != NULL) {
		delete m_pSECSDP;
		m_pSECSDP = NULL;
	}
}

void CEasyTermView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_stdout);
}

BOOL CEasyTermView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CEasyTermView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}


// CEasyTermView 인쇄
void CEasyTermView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEasyTermView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CEasyTermView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CEasyTermView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CEasyTermView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: 여기에 사용자 지정 인쇄 코드를 추가합니다.
}

void CEasyTermView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEasyTermView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CEasyTermView 진단

#ifdef _DEBUG
void CEasyTermView::AssertValid() const
{
	CFormView::AssertValid();
}

void CEasyTermView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEasyTermDoc* CEasyTermView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEasyTermDoc)));
	return (CEasyTermDoc*)m_pDocument;
}
#endif //_DEBUG


// CEasyTermView 메시지 처리기


void CEasyTermView::OnSize(UINT nType, int cx, int cy)
{
	RECT	rc;

	CFormView::OnSize(nType, cx, cy);

	GetClientRect(&rc);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_stdout) {
		m_stdout.MoveWindow( 0, 40, rc.right-rc.left, rc.bottom - rc.top - 30, 1);
	}
}


void CEasyTermView::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_stdout.ResetContent();
}


void CEasyTermView::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_pSECSDP->SECS_Message_Output("S1F1", true, true, true, "<L2<A:MDLN><A:SOFTREV>>");
	//m_pSECSDP->SECS_Message_Output("S18F2", true, true, true, "<L2<A:01><L8<A:00><A:0><A:BUSY><A:1.04><A:CIDR_><A:KEYENCE><A:N-410><A:BUSY>>>");
	//m_pSECSDP->SECS_Message_Output("S18F10", true, true, true, "<L4<A:01><A:NO><A:SS11.11111><L4<A:NE><A:0><A:IDLE><A:BUSY>>>");
	//--------------------------------------------------------------------------------------
	CEasyTermDoc* pDoc = (CEasyTermDoc *) GetDocument();
	kaMemStr* pStr = new kaMemStr();
	pDoc->Make_Response("S18F10", 1);
	//--------------------------------------------------------------------------------------
}

void CEasyTermView::OnBnClickedButton3()
{
	CString sSxFy, sMsg;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//BYTE pData[] = { 0x80, 0x42,0x05, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01,0x03, 0x21, 0x01,0x84, 0x65, 0x01, 0x11, 0x41, 0x07, 0x54, 0x31, 0x20, 0x48, 0x49, 0x47, 0x48};
	BYTE pData[] = { 0x0A, 0x00, 0x00, 0x81, 0x01, 0x80, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x04 };

	(CEasyTermDoc *)GetDocument()->Decode_Stream(pData, sSxFy, sMsg);
	m_pSECSDP->SECS_Message_Output(sSxFy, true, true, true, sMsg);
}


// static
void CEasyTermView::MessageDisplay(CString str)
{
	CMainFrame* pFrm = (CMainFrame *)AfxGetApp()->GetMainWnd();
	CEasyTermView* pView = (CEasyTermView *)pFrm->GetActiveView();

	pView->m_stdout.AddString(str);
}



void CEasyTermView::OnCommSetup()
{
	//TODO: 여기에 명령 처리기 코드를 추가합니다.
	CCommSetupDlg dlg;
	CEasyTermDoc* pDoc = (CEasyTermDoc *)GetDocument();

	// if Serial
	CMainFrame* pFrm = (CMainFrame *) AfxGetApp()->GetMainWnd();;
	CSecsSerial* pComm = (CSecsSerial*) pFrm->GetCommObject();
	if (pComm->GetNoOfPort() == 0) {
		MessageBox("No Serial Port", "Error", MB_ICONERROR);
	}
	else {
		pDoc->Set_Data(dlg.m_pConfig );
		dlg.SetupPortList(pComm->Scan_Serial_Port());
		if (dlg.DoModal() == IDOK) {
			pDoc->Set_Comm_Config(dlg.m_pConfig);
		}
	}
	// end if Serial
}



void CEasyTermView::OnCommPortopen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrm = (CMainFrame *)AfxGetMainWnd();
	CSecsSerial* pComm = (CSecsSerial*)pFrm->GetCommObject();
	CEasyTermDoc* pDoc = (CEasyTermDoc *) GetDocument();
	
	if (pComm->OpenPort(pDoc->m_pConfig) == false) {
		char pszstr[64] = { 0, };
		_snprintf(pszstr, 63, "Port Open Fail (COM%d)", pDoc->m_pConfig->Get_Port());
		MessageBox(pszstr, "Error", MB_ICONERROR);
	}
}


void CEasyTermView::OnCommPortclose()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* pFrm = (CMainFrame *)AfxGetMainWnd();
	CSecsSerial* pComm = (CSecsSerial*)pFrm->GetCommObject();
	pComm->ClosePort();
}


//int CEasyTermView::DisplaySecsMessage(unsigned char* pucMsg)
//{
//	CString	sSxFy;
//	CString sMsg;
//
//	(CEasyTermDoc *) GetDocument()->Decode_Stream(pucMsg, sSxFy, sMsg);
//	m_pSECSDP->SECS_Message_Output(sSxFy, true, true, true, sMsg);
//
//	return 0;
//}
//

// View에서 Message 출력
int CEasyTermView::SecsMsg_AddString(CString sSxFy, bool bToHost, bool bWait, bool bEnd, CString sMsg, kaMemStr* pkaStr)
{
	m_pSECSDP->SECS_Message_Output( sSxFy, bWait, bEnd, bToHost, sMsg);
	m_pSECSDP->BIN_Data_Output(pkaStr->Get_Str_Length(), pkaStr->GetString());
	m_pSECSDP->SkipLine();

	return 0;
}


void CEasyTermView::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	kaSecsTool* pTool = new kaSecsTool();

	char szResp[40] = { 0, };
	char *szList[20] = { "S1F3", "S3F11", "S10F1", "S11F13", "S192F1", "S192F21", "S192F111" };

	for (int i = 0; i < 7; i++) {
		memset(szResp, 0, sizeof(char) * 40);
		pTool->kaSecs_Find_Secondary_Message(szList[i], szResp);
	}

	delete pTool;
}


void CEasyTermView::OnBnClickedButLogsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString szFilter("Log Files|*.log|All Files(*.*)|*.*||");
	CFileDialog dlg(FALSE , _T("*.dat"), NULL, OFN_HIDEREADONLY, szFilter);
	
	if (dlg.DoModal() == IDOK) {
		m_stdout.SaveListboxItem(dlg.GetPathName(), true);
	}

}


void CEasyTermView::StrMsg_AddString(CString str)
{
	m_pSECSDP->String_Message_Ouput(str);
	//m_pSECSDP->SkipLine();
}
