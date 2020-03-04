// CommSetupDlg.cpp : 구현 파일입니다.
//
#include "pch.h"
#include "stdafx.h"
#include "EasyTerm.h"
#include "CommSetupDlg.h"
#include "afxdialogex.h"

#include "CommConfig.h"


// CCommSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCommSetupDlg, CDialog)

CCommSetupDlg::CCommSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_COMM_SETUPDLG, pParent)
{
	m_pConfig = new CCommConfig();

	m_pCStrList_Port = NULL;
}

CCommSetupDlg::~CCommSetupDlg()
{
	if (m_pConfig != NULL) {
		delete m_pConfig;
		m_pConfig = NULL;
	}
}

void CCommSetupDlg::SetupValue(CCommConfig val)
{
	*m_pConfig = val;
}

void CCommSetupDlg::SetControlValue()
{
	m_iDataBit = m_pConfig->Get_Databit() - 5;			// 초기값은 8이고 해당 GUI Radio Button값은 3이므로
	m_iStop = m_pConfig->Get_Stop() - 1;
	m_iParity = m_pConfig->Get_Parity();
	m_iDTRDSR = m_pConfig->Get_DTRDSR();
	m_iRTSCTS = m_pConfig->Get_RTSCTS();
	m_iXonXoff = m_pConfig->Get_XONXOFF();
}

void CCommSetupDlg::OnDataBitSelected(UINT uID)
{
	switch (uID)
	{
		case IDC_RADIO_DATA_5:
			m_pConfig->Set_Databit(5);
			break;

		case IDC_RADIO_DATA_6:
			m_pConfig->Set_Databit(6);
			break;

		case IDC_RADIO_DATA_7:
			m_pConfig->Set_Databit(7);
			break;

		case IDC_RADIO_DATA_8:
			m_pConfig->Set_Databit(8);
			break;
	}
}

void CCommSetupDlg::OnDataStopSelected(UINT uID)
{
	switch (uID)
	{
		case IDC_RADIO_STOP_1:
			m_pConfig->Set_Stop(1);
			break;

		case IDC_RADIO_STOP_2:
			m_pConfig->Set_Stop(2);
			break;

		case IDC_RADIO_STOP_3:
			m_pConfig->Set_Stop(3);
			break;
	}

}

void CCommSetupDlg::OnDataParitySelected(UINT uID)
{
	switch (uID)
	{
		case IDC_RADIO_PARITY_0:
			m_pConfig->Set_Parity(0);
			break;

		case IDC_RADIO_PARITY_1:
			m_pConfig->Set_Parity(1);
			break;

		case IDC_RADIO_PARITY_2:
			m_pConfig->Set_Parity(2);
			break;

		case IDC_RADIO_PARITY_3:
			m_pConfig->Set_Parity(3);
			break;

		case IDC_RADIO_PARITY_4:
			m_pConfig->Set_Parity(4);
			break;
	}
}

void CCommSetupDlg::OnDataDTRDSRSelected(UINT uID)
{
	switch (uID)
	{
	case IDC_RADIO_DTRDSR_0:
		m_pConfig->Set_DTRDSR(0);
		break;

	case IDC_RADIO_DTRDSR_1:
		m_pConfig->Set_DTRDSR(1);
		break;
	}
}

void CCommSetupDlg::OnDataRTSCTSSelected(UINT uID)
{
	switch (uID)
	{
		case IDC_RADIO_RTSCTS_0:
			m_pConfig->Set_RTSCTS(0);
			break;

		case IDC_RADIO_RTSCTS_1:
			m_pConfig->Set_RTSCTS(1);
			break;

		case IDC_RADIO_RTSCTS_2:
			m_pConfig->Set_RTSCTS(2);
			break;

		case IDC_RADIO_RTSCTS_3:
			m_pConfig->Set_RTSCTS(3);
			break;
	}
}

void CCommSetupDlg::OnDataXONXOFFSelected(UINT uID)
{
	switch (uID)
	{
		case IDC_RADIO_XONXOFF_0:
			m_pConfig->Set_XONXOFF(0);
			break;

		case IDC_RADIO_XONXOFF_1:
			m_pConfig->Set_XONXOFF(1);
			break;
	}
}


void CCommSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DATA_5, m_iDataBit);
	DDX_Radio(pDX, IDC_RADIO_STOP_1, m_iStop);
	DDX_Radio(pDX, IDC_RADIO_PARITY_0, m_iParity);
	DDX_Radio(pDX, IDC_RADIO_DTRDSR_0, m_iDTRDSR);
	DDX_Radio(pDX, IDC_RADIO_RTSCTS_0, m_iRTSCTS);
	DDX_Radio(pDX, IDC_RADIO_XONXOFF_0, m_iXonXoff);

	//m_iSpeed;
	//m_iPort;
	DDX_Control(pDX, IDC_COMBO1, m_cbPortList);
	DDX_Control(pDX, IDC_COMBO2, m_csSpeed);
}


BEGIN_MESSAGE_MAP(CCommSetupDlg, CDialog)
ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_DATA_5, IDC_RADIO_DATA_8, OnDataBitSelected)
ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_STOP_1, IDC_RADIO_STOP_3, OnDataStopSelected)
ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_PARITY_0, IDC_RADIO_PARITY_4, OnDataParitySelected)
ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_DTRDSR_0, IDC_RADIO_DTRDSR_1, OnDataDTRDSRSelected)
ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_RTSCTS_0, IDC_RADIO_RTSCTS_3, OnDataRTSCTSSelected)
ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_XONXOFF_0, IDC_RADIO_XONXOFF_1, OnDataXONXOFFSelected)
ON_WM_CREATE()
ON_BN_CLICKED(IDOK, &CCommSetupDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCommSetupDlg 메시지 처리기입니다.


int CCommSetupDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.


	return 0;
}


BOOL CCommSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString csr;
	POSITION ps;

	ps = m_pCStrList_Port->GetHeadPosition();
	for (int i = 0 ; i < m_pCStrList_Port->GetCount() ; i++) {			//		Combo data 추가
		csr = m_pCStrList_Port->GetNext(ps);
		m_cbPortList.AddString(csr);
	}

	m_cbPortList.SetDropDownHeight(3);
	csr.Format("COM%d", m_pConfig->Get_Port());
	m_cbPortList.SelectByStr(csr);

	m_csSpeed.Clear();


	CStringList* pList = new CStringList();
	if (pList != NULL) {
		pList->AddHead("110");
		pList->AddHead("300");
		pList->AddHead("600");
		pList->AddHead("1200");
		pList->AddHead("2400");
		pList->AddHead("4800");
		pList->AddHead("9600");
		pList->AddHead("14400");
		pList->AddHead("19200");
		pList->AddHead("38400");
		pList->AddHead("56000");
		pList->AddHead("57600");
		pList->AddHead("115200");
		pList->AddHead("128000");
		pList->AddHead("256000");
		
		m_csSpeed.DropDownList(pList);
		delete pList;
		pList = NULL;

		m_csSpeed.SetDropDownHeight(5);
		csr.Format("%d", m_pConfig->Get_Speed());
		m_csSpeed.SelectByStr(csr);
	}
	else
	{
		return TRUE;
	}

	UpdateData(TRUE);
	m_iRTSCTS = m_pConfig->Get_RTSCTS();
	m_iDataBit = m_pConfig->Get_Databit()-5;
	m_iDTRDSR = m_pConfig->Get_DTRDSR();
	m_iParity = m_pConfig->Get_Parity();
	m_iStop = m_pConfig->Get_Stop()-1;
	m_iXonXoff = m_pConfig->Get_XONXOFF();
	UpdateData(FALSE);

	return TRUE;
}


void CCommSetupDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	m_pConfig->Set_Speed((short)m_csSpeed.GetSelectIntValue());
	m_cbPortList.GetSelectText(str);
	m_pConfig->Set_Port(str);

	UpdateData(TRUE);
	m_pConfig->Set_RTSCTS((unsigned char) m_iRTSCTS);
	m_pConfig->Set_Databit((unsigned char) m_iDataBit+5);
	m_pConfig->Set_DTRDSR((unsigned char) m_iDTRDSR);
	m_pConfig->Set_Parity((unsigned char) m_iParity);
	m_pConfig->Set_Stop((unsigned char) m_iStop+1);
	m_pConfig->Set_XONXOFF((unsigned char) m_iXonXoff);
	UpdateData(FALSE);

	CDialog::OnOK();
}


void CCommSetupDlg::SetupPortList(CStringList* pcsrList_Port)
{
	m_pCStrList_Port = pcsrList_Port;
}
