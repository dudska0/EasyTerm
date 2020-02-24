
// EasyTermView.cpp: CEasyTermView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "EasyTerm.h"
#endif

#include "EasyTermDoc.h"
#include "EasyTermView.h"

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
END_MESSAGE_MAP()

// CEasyTermView 생성/소멸

CEasyTermView::CEasyTermView() noexcept
	: CFormView(IDD_EASYTERM_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CEasyTermView::~CEasyTermView()
{
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
		m_stdout.MoveWindow( 0, 40, rc.right-rc.left, rc.bottom - rc.top, 1);
	}
}


void CEasyTermView::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_stdout.ResetContent();
}


void CEasyTermView::AddDatatoListbox(int iLevel, CString str)
{
	CString strData;
	SYSTEMTIME st;

	GetLocalTime(&st);

	strData.AppendFormat(_T("%02d:%02d:%02d.%03d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	for (int i = 0; i < iLevel + 1; i++) {
		strData = strData + _T("     ");
	}

	strData = strData + str;
	m_stdout.AddString(strData);
}

void CEasyTermView::AddDatatoListbox(int iLevel, char* pszstr)
{
	CString strData;
	SYSTEMTIME st;

	GetLocalTime(&st);

	strData.AppendFormat(_T("%02d:%02d:%02d.%03d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	for (int i = 0; i < iLevel + 1; i++) {
		strData = strData + _T("     ");
	}
	strData.AppendFormat(_T("%s"), pszstr);
	
	m_stdout.AddString(strData);
}
