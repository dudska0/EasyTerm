
// EasyTerm.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "EasyTerm.h"
#include "MainFrm.h"

#include "EasyTermDoc.h"
#include "EasyTermView.h"
#include "SecsSerial.h"
#include "kaMemStr.h"
#include "AsciiTable.h"

#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEasyTermApp

BEGIN_MESSAGE_MAP(CEasyTermApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CEasyTermApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEasyTermApp 생성

CEasyTermApp::CEasyTermApp() noexcept
{
	m_bHiColorIcons = TRUE;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("EasyTerm.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CEasyTermApp 개체입니다.

CEasyTermApp theApp;


// CEasyTermApp 초기화

BOOL CEasyTermApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEasyTermDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CEasyTermView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	// Thread Invoke
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CEasyTermApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);
	_CrtDumpMemoryLeaks();

	return CWinAppEx::ExitInstance();
}

// CEasyTermApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CEasyTermApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEasyTermApp 사용자 지정 로드/저장 방법

void CEasyTermApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

// 모든게 다 Load된 이후에...
void CEasyTermApp::LoadCustomState()
{
	m_pThread = AfxBeginThread(ThreadForCommRead, this);
}

void CEasyTermApp::SaveCustomState()
{
}

void CEasyTermApp::Set_Thread_Terminate(bool bVal)
{
	this->m_bThreadTerm = bVal;
}

bool CEasyTermApp::Get_Thread_Terminate()
{
	return this->m_bThreadTerm;
}



// CEasyTermApp 메시지 처리기

bool CEasyTermApp::Get_Comm_Established()
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	if (pFrame == NULL) {
		return false;
	}

	CComm* pCom = pFrame->GetCommObject();
	// if Serial{
		// if SecsComm {
		//} SecsComm end
		return pCom->Get_Use();
	// } Serial End
	// else if Socket {


	// } Socket End

		return false;
}

bool CEasyTermApp::Read(char& ch)
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	if (pFrame == NULL) {
		return false;
	}	
	
	CComm* pCom = pFrame->GetCommObject();
	return pCom->ReadChar(ch);
}


bool CEasyTermApp::ProcessCommData(kaMemStr* pStr)
{
	CString str_buf;
	bool bResult = false;
	CMainFrame* pFrame = (CMainFrame *) AfxGetApp()->GetMainWnd();
	//if (pFrame == NULL) {
	//	return false;
	//}
	CEasyTermView* pView = (CEasyTermView *)(pFrame->GetActiveView());
	CEasyTermDoc* pDoc = pView->GetDocument();
	
	// SECS Message Section ------------------------------------------------------------------
	CString szResKey;
	CSecsSerial* pCom = (CSecsSerial *) (pFrame->GetCommObject());


	if (pStr->Get_Str_Length() == 1) {
		if (pStr->GetString()[0] == CODE_ASCII_ENQ) {
			str_buf.AppendFormat("Recv ENQ <<");
			pView->StrMsg_AddString(str_buf);
			str_buf.Empty();
			bResult = pCom->Process_Read_Data(pStr->GetString(), pStr->Get_Str_Length(), szResKey);
			if (bResult == false) {
				return false;
			}
			str_buf.AppendFormat(">> Send EOT");
			pView->StrMsg_AddString(str_buf);
			str_buf.Empty();
		}
	}
	else {
		bResult = pCom->Process_Read_Data(pStr->GetString(), pStr->Get_Str_Length(), szResKey);
		if (bResult == false) {
			return false;
		}
	}

	if (pCom->Get_CommStep() == SECSCOMM::SEND_ACK_6) {
		CString csSXFy;
		CString csMsg;
		bool bWait = false;													// Wait Response
		bool bHost = false;
		bool bEnd = false;

		csSXFy.Empty();
		csMsg.Empty();

		unsigned char* pchStr = pStr->GetString();

		pDoc->Decode_Stream(pchStr, csSXFy, csMsg );
		bWait = pDoc->m_pDecoder->Get_Wait_Response();
		bHost = pDoc->m_pDecoder->Get_To_Host();
		bEnd = pDoc->m_pDecoder->Get_End_Flag();
		pView->SecsMsg_AddString(csSXFy, bHost, bWait, bEnd, csMsg, pStr);		// 메세지 여기서 출력
		//-------------------------------------------------------------------------------------
		bResult = pCom->WriteChar(CODE_ASCII_ACK);							// Send Ack
		if (bResult == false)  return false;
		str_buf.AppendFormat(">> Send ACK");
		pView->StrMsg_AddString(str_buf);

		pCom->Set_CommStep(SECSCOMM::WAIT_ENQ_0);
		//-------------------------------------------------------------------------------------
		if (bWait) {
			// Find Response and Send SECS Message
			char szResKey[32] = { 0, };
			pCom->GetEncoder()->GetKeyValue(LPSTR(LPCTSTR(csSXFy)), szResKey);
			pStr->ClearString(true);
			pDoc->Make_Response(szResKey, 1);
		}
	}
	// ----------------------------------------------------------------------------------------

	
	return bResult;
}

UINT CEasyTermApp::ThreadForCommRead(LPVOID pParam)
{
	char ch;
	CEasyTermApp* pThis = (CEasyTermApp *)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame *)AfxGetApp()->GetMainWnd();
	//CEasyTermView* pView = (CEasyTermView *) pFrame->GetActiveView();
	kaMemStr* pStr = new kaMemStr();

	pThis->Set_Thread_Terminate(false);
	
	while (true)
	{
		// Thread End Check
		if (pThis->Get_Thread_Terminate()) {
			break;
		}

		// Serial 인 경우 처리
		if (pThis->Get_Comm_Established() == false ){
			// 끊긴 경우에는 100m 단위로 Loop를 돈다.
			//if (str.GetLength() > 0) {
			//	pThis->ProcessCommData(str);
			//}
			
			if (pStr->Get_Str_Length() > 0) {
				pThis->ProcessCommData(pStr);
				pStr->ClearString(true);
			}
			
			Sleep(15);
			continue;
		}
		else {
			if (pThis->Read(ch)) {
				//str.AppendChar(ch);
				pStr->AppendChar((unsigned char)ch);
			}
			else {
				if (pStr->Get_Str_Length() > 0) {
					pThis->ProcessCommData(pStr);
					pStr->ClearString(true);
				}
			}
		}

		// Port가 Open된 내용에 대해서 Writing을 한다.
	}

	if (pStr != NULL) {
		delete pStr;
		pStr = NULL;
	}

	return 0;
}

void CEasyTermApp::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	AfxMessageBox(_T("TEST"));
}

