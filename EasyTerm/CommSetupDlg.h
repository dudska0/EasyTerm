#pragma once
#include "CommConfig.h"
#include "CComboDropDownList.h"
#include "CSerialPort.h"


// CCommSetupDlg 대화 상자입니다.
class CCommSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommSetupDlg)

private:
	CStringList* m_pCStrList_Port;

public:
	CCommConfig*  m_pConfig;
	CCommSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCommSetupDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMM_SETUPDLG };
#endif

public : 
	//CComboBox m_cbPortList;
	CComboDropDownList m_cbPortList;
	CComboDropDownList m_csSpeed;

	void SetControlValue();
	void SetupValue(CCommConfig val);
	void SetupPortList(CStringList* pcsrList_Port);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	void OnDataBitSelected(UINT uID);
	void OnDataStopSelected(UINT uID);
	void OnDataParitySelected(UINT uID);
	void OnDataDTRDSRSelected(UINT uID);
	void OnDataRTSCTSSelected(UINT uID);
	void OnDataXONXOFFSelected(UINT uID);

	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()

	// Comm Data Bit 설정	// Comm Data Bit 설정
	int m_iDataBit;
	int m_iSpeed;
	int m_iPort;
	int m_iStop;
	int m_iParity;
	int m_iDTRDSR;
	int m_iRTSCTS;
	int m_iXonXoff;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
};
