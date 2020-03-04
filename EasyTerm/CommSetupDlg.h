#pragma once
#include "CommConfig.h"
#include "CComboDropDownList.h"
#include "CSerialPort.h"


// CCommSetupDlg ��ȭ �����Դϴ�.
class CCommSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CCommSetupDlg)

private:
	CStringList* m_pCStrList_Port;

public:
	CCommConfig*  m_pConfig;
	CCommSetupDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCommSetupDlg();

// ��ȭ ���� �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	void OnDataBitSelected(UINT uID);
	void OnDataStopSelected(UINT uID);
	void OnDataParitySelected(UINT uID);
	void OnDataDTRDSRSelected(UINT uID);
	void OnDataRTSCTSSelected(UINT uID);
	void OnDataXONXOFFSelected(UINT uID);

	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()

	// Comm Data Bit ����	// Comm Data Bit ����
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
